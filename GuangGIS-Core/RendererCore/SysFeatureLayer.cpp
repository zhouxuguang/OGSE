#include "SysFeatureLayer.h"
#include "SysRasterSource.h"
#include "SysFeatureRenderer.h"
#include "SysFeatureLabel.h"
#include "SysRenderContext.h"
#include "SysDisplayTransform.h"
#include "SysSpatialIndex.h"
#include "SysFeatureCache.h"

#include "SysMapManager.h"
#include "SysUtilFunc.h"

#include "SysUtility/SmartPtr.h"
#include "SysUtility/FileAPI.h"
#include "SysDataSource/SysDataSource.h"
#include "SysDataSource/CoordTransformation.h"

#include <GL/glew.h>

namespace SysDraw
{

	SysFeatureLayer::SysFeatureLayer(std::string strLyrName)
		:SysMapLayer(SysMapLayer::VectorLayer,strLyrName)
	{
		Init();
	}

	SysFeatureLayer::~SysFeatureLayer(void)
	{
		UnInit();
	}

	void SysFeatureLayer::Init()
	{
		m_pDatalayer = NULL;
		m_pRenderer = NULL;
		m_pDataSource = new(std::nothrow) SysRasterSource;
		m_mapSelectIds.clear();

		/*for (int i = 0; i < 36; i ++)
		{
			m_mapHighLight.insert(make_pair(i,1));
		}*/

		m_bLabeled = false;
		m_pLabel = new(std::nothrow) SysFeatureLabel;

		m_pFeatureCache = new(std::nothrow) SysFeatureCache;

		m_bUseIndex = false;
		m_pSpatialIndex = NULL;

		m_mapFeaCache.clear();
	}

	void SysFeatureLayer::UnInit()
	{
		m_pDatalayer = NULL;
		if (m_pDataSource != NULL)
		{
			delete m_pDataSource;
			m_pDataSource = NULL;
		}

		if (m_pLabel != NULL)
		{
			delete m_pLabel;
			m_pLabel = NULL;
		}

		if (m_pSpatialIndex != NULL)
		{
			delete m_pSpatialIndex;
			m_pSpatialIndex = NULL;
		}

		m_bUseIndex = false;

		if (m_pFeatureCache != NULL)
		{
			delete m_pFeatureCache;
			m_pFeatureCache = NULL;
		}

		mapFeatureCache::iterator iter = m_mapFeaCache.begin();
		mapFeatureCache::iterator iterEnd = m_mapFeaCache.begin();
		for (; iter != iterEnd; ++iter)
		{
			GeoFeature::DestroyFeature((GeoFeature *)iter->second);
		}

		m_mapFeaCache.clear();
	}

	bool SysFeatureLayer::SetDataSource(SysDataSource::IDataLayer* pDatalayer)
	{
		SysDataSource::DATAOBJECT_TYPE eLayer = pDatalayer->GetType();
		if (eLayer != SysDataSource::SYS_FEATURELAYER)
		{
			return false;
		}

		SysDataSource::IFeatureDataLayer* pVecLayer = (SysDataSource::IFeatureDataLayer*)pDatalayer;
		if (NULL == pVecLayer)
		{
			return false;
		}

		m_pDatalayer = (SysDataSource::IFeatureDataLayer *)pDatalayer;

		SysDataSource::CDataDriverFactory* pFactory = SysDataSource::CDataDriverFactory::Instance();
		SysDataSource::stOpenFileWay open_way;
		SysDataSource::IFeatureDataLayerPtr ptrFeatureDataLayer = pFactory->CreateDataLayer(GetLayerName(),open_way);

		m_pDataSource->SetDataSource(ptrFeatureDataLayer);

		m_pDatalayer = (SysDataSource::IFeatureDataLayer *)m_pDataSource->GetDataSource().get();

		//还需要设置图层的范围,坐标系
		SysEnvelope envLyr;
		Envelope envGeos = ptrFeatureDataLayer->GetLayerEnvelope();
		envLyr.Init(envGeos.getMinX(),envGeos.getMaxX(),envGeos.getMinY(),envGeos.getMaxY());
		SetLayerExtent(envLyr);

		SysSRS::CSpatialReferencePtr ptrSrs = ptrFeatureDataLayer->GetSpatialReference();

		std::string strWkt = _T("");
		if (NULL != ptrSrs)
		{
			ptrSrs->ExportToWkt(strWkt);
		}

		SetSpatialRef(strWkt);

		//还需要设置默认的渲染器以及和渲染器对应的符号
		int nGeomType = ptrFeatureDataLayer->GetFeatureType();
		m_pRenderer = SysFeatureRenderer::GetDefaultRenderer(nGeomType);

		return true;
	}

	SysDataSource::IFeatureDataLayer* SysFeatureLayer::GetDataSource() const
	{
		return m_pDatalayer;
	}

	SysFeatureRenderer* SysFeatureLayer::GetFeatureRenderer() const
	{
		return m_pRenderer;
	}

	void SysFeatureLayer::SetFeatureRenderer(SysFeatureRenderer* pRenderer)
	{
		if (pRenderer != NULL)
		{
			DestroyFeatureRenderer(m_pRenderer);
			m_pRenderer = pRenderer;
		}
		
	}

	SysFeatureLayer* SysFeatureLayer::CreateFeatureLayer(SysDataSource::IDataLayer* pDatalayer)
	{
		if (NULL == pDatalayer)
		{
			return NULL;
		}
		SysFeatureLayer *pLayer = new SysFeatureLayer(pDatalayer->GetName());
		if (!pLayer->SetDataSource(pDatalayer))
		{
			delete pLayer;
			return NULL;
		}

		return pLayer;
	}

	void SysFeatureLayer::DestroyFeatureLayer(SysFeatureLayer* pVecLayer)
	{
		delete pVecLayer;
		pVecLayer = NULL;
	}

	bool SysFeatureLayer::Draw( SysRenderContext* rendererContext )
	{

		//如果不可见直接返回
		if (!IsVisible())
		{
			return false;
		}
		
		glDisable(GL_TEXTURE_2D);
		m_pRenderer->BeginRender(rendererContext,this);


		//绘制标注文本
		if (m_bLabeled)
		{
			SysDisplayTransform &tran = rendererContext->GetDisplayTran();
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();

			glLoadIdentity();
			glOrtho(0,tran.GetViewWidth(),tran.GetViewHeight(),0,-1,1);

			glEnable( GL_TEXTURE_2D );


			SysDataSource::IFeatureDataLayer* pFeaLayer = m_pDatalayer;

			SysEnvelope envView;
			MapExtentToLayerExtent(pManager->GetCurrentMap(),(SysMapLayer *)this,envView);

			Envelope envSearch;
			envSearch.init(envView.minX,envView.maxX,envView.minY,envView.maxY);
			pFeaLayer->SetSpatialFilter(envSearch);

			ResetRead();

			SysFeature* pFeature = NULL;
			while ((pFeature = GetNextFeature()) != NULL)
			{
				m_pLabel->RenderFeature(rendererContext,pFeature,true);
			}

			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
			
		}

		return true;
	}

	void SysFeatureLayer::DrawLabels( SysRenderContext* rendererContext )
	{
		//
	}

	void SysFeatureLayer::ResetRead()
	{
		SysDataSource::IFeatureDataLayer* pVecLayer = (SysDataSource::IFeatureDataLayer*)m_pDatalayer;
		if (NULL == pVecLayer)
		{
			return;
		}

		//pVecLayer->RegisterSelectionSetChangedCallback()
	}

	SysFeature* SysFeatureLayer::GetNextFeature() const
	{
		SysDataSource::IFeatureDataLayer* pVecLayer = (SysDataSource::IFeatureDataLayer*)m_pDatalayer;
		if (NULL == pVecLayer)
		{
			return NULL;
		}

		return (SysFeature*)pVecLayer->GetNextFeature();
	}

	int SysFeatureLayer::GetFeatureCount() const
	{
		SysDataSource::IFeatureDataLayer* pVecLayer = (SysDataSource::IFeatureDataLayer*)m_pDatalayer;
		if (NULL == pVecLayer)
		{
			return 0;
		} 

		return pVecLayer->GetFeatureCount();
	}

	SysFeature* SysFeatureLayer::GetFeatureByFID(int nIndex) const
	{
		SysDataSource::IFeatureDataLayer* pVecLayer = (SysDataSource::IFeatureDataLayer*)m_pDatalayer;
		if (NULL == pVecLayer)
		{
			return NULL;
		}

		return (SysFeature*)pVecLayer->GetFeature(nIndex);
	}

	void SysFeatureLayer::SetSpatialFilter(const SysEnvelope& envFilter)
	{
		SysDataSource::IFeatureDataLayer* pVecLayer = (SysDataSource::IFeatureDataLayer*)m_pDatalayer;
		if (NULL == pVecLayer)
		{
			return;
		}

		Envelope envNew;
		envNew.init(envFilter.minX,envFilter.maxX,envFilter.minY,envFilter.maxY);

		pVecLayer->SetSpatialFilter(envNew);
	}

	bool SysFeatureLayer::InsertSelectedFID( long nFid,bool bFlag ) const
	{
		pair<SysSelectedFIDs::iterator, bool> pairRes = m_mapSelectIds.insert(make_pair(nFid,bFlag));
		return pairRes.second == true;
	}

	bool SysFeatureLayer::IsSelected( long nFid ) const
	{
		SysSelectedFIDs::const_iterator iter = m_mapSelectIds.find(nFid);

		if (iter != m_mapSelectIds.end())
		{
			return iter->second;
		}
		
		return false;
	}

	void SysFeatureLayer::RemoveSelected() /*const*/
	{
		//应该是遍历将标志改为false
		/*SysSelectedFIDs::iterator iter = m_mapSelectIds.begin();
		SysSelectedFIDs::iterator iterEnd = m_mapSelectIds.end();
		for (; iter != iterEnd; ++iter)
		{
			iter->second = false;
		}*/
		m_mapSelectIds.clear();
	}

	SysFeatureLabel * SysFeatureLayer::GetFeatureLabel() const
	{
		return m_pLabel;
	}

	void SysFeatureLayer::DrawVertexMarker( double x, double y, double dbScale, VertexMarkType eType, int nVertexSize )
	{
		if ( eType == Square )
		{
			glColor3f(0,1.0,0.0);
			glRectf(x-nVertexSize*dbScale,y-nVertexSize*dbScale,y-nVertexSize*dbScale,y+nVertexSize*dbScale);
		}
		else if ( eType == Cross )
		{
			glColor3f(0,1.0,0.0);
			glBegin(GL_LINES);
			glVertex2d(x - nVertexSize*dbScale, y + nVertexSize*dbScale);
			glVertex2d(x + nVertexSize*dbScale, y - nVertexSize*dbScale);
			glEnd();

			glBegin(GL_LINES);
			glVertex2d(x - nVertexSize*dbScale, y - nVertexSize*dbScale);
			glVertex2d(x + nVertexSize*dbScale, y + nVertexSize*dbScale);
			glEnd();
		}
	}

	void SysFeatureLayer::SetFeatureLabel( SysFeatureLabel *pLabel )
	{
		if (m_pLabel != NULL)
		{
			delete m_pLabel;
			m_pLabel = pLabel;
		}
	}

	std::vector<long> SysFeatureLayer::SelectFeatures( const SysEnvelope& envFilter )
	{
		ResetRead();
		SetSpatialFilter(envFilter);

		std::vector<long> vecFids;

		//如果没有索引
		if (!m_bUseIndex)
		{
			SysFeature* pFeature = NULL;
			while (( pFeature = GetNextFeature() ) != NULL)
			{
				GeoFeature* pGeoFeature = (GeoFeature* )pFeature;
				vecFids.push_back(pGeoFeature->GetFID());

				GeoFeature::DestroyFeature(pGeoFeature);
				
			}
		}

		else
		{
			if (NULL == m_pSpatialIndex)
			{
				m_bUseIndex = false;
				return vecFids;
			}

			vecFids = m_pSpatialIndex->IntersectQuery(envFilter);
		}

		return vecFids;
	}

	bool SysFeatureLayer::BuildSpatialIndex( DrawProgressFunc pProgressFunc , void* pProgressArg )
	{
		bool bInsert = true;		//是否需要插入要素
		if (NULL == m_pSpatialIndex)
		{
			std::string strFileName = GetLayerName();
			size_t nPos = strFileName.rfind('.');
			strFileName = strFileName.substr(0,nPos);

			std::string strIndexFile = strFileName + _T(".idx");
			std::string strIndexDataFile = strFileName + _T(".dat");
			//strFileName += _T(".sdx");
			bool bFileExist = SysUtility::IsFileExist(strIndexFile.c_str());
			if (bFileExist)
			{
				//如果索引文件大小为0，则删掉索引数据文件，重建索引
				if (0 == SysUtility::GetFileSize(strIndexFile))
				{
					remove(strIndexFile.c_str());
					remove(strIndexDataFile.c_str());
					bFileExist = false;
				}
			}

			bInsert = !bFileExist;
			m_pSpatialIndex = new SysSpatialIndex(strFileName,bInsert);

		}

		if (pProgressFunc != NULL)
		{
			pProgressFunc(0,NULL,pProgressArg);
		}

		/*if (bInsert)
		{
			if (pProgressFunc != NULL)
			{
				pProgressFunc(1,NULL,pProgressArg);
			}

			m_bUseIndex = true;
			return m_bUseIndex;
		}*/

		/*clock_t t1 = clock();
		int nFeaCount = GetFeatureCount();
		clock_t t2 = clock();

		smartlog<<"遍历时间:"<<t2-t1;*/
		int nFeaCount = GetFeatureCount();

		ResetRead();
		SetSpatialFilter(GetLayerExtent());

		int nIndex = 0;

		clock_t t1 = clock();

		int nCount = 0;

		SysFeature* pFeature = NULL;
		while (( pFeature = GetNextFeature() ) != NULL)
		{
			GeoFeature* pGeoFeature = (GeoFeature* )pFeature;

			if (bInsert)
			{
				m_pSpatialIndex->InsertFeature(pFeature);
			}
			
			//m_mapFeaCache.insert(make_pair( pGeoFeature->GetFID(),pFeature) );			//加入缓存

			nCount ++;
			//if (nCount <= 20000)
			{
				m_pFeatureCache->InsertFeature(pGeoFeature->GetFID(),pFeature);
			}

			if (pProgressFunc != NULL)
			{
				pProgressFunc(0.99*(nIndex+1)/double(nFeaCount),NULL,pProgressArg);
			}

			++nIndex;

		}

		m_pSpatialIndex->SaveToDisk();

		clock_t t2 = clock();
		smartlog<<"建索引时间:"<<t2-t1;

		if (pProgressFunc != NULL)
		{
			pProgressFunc(1,NULL,pProgressArg);
		}

		m_bUseIndex = true;
		
		return m_bUseIndex;
		
	}

	SysSpatialIndex* SysFeatureLayer::GetSpatialIndex() const
	{
		return m_pSpatialIndex;
	}

	SysFeatureCache* SysFeatureLayer::GetFeatureCache() const
	{
		return m_pFeatureCache;
	}
}
