#include "SysSingleSymbolRenderer.h"
#include "SysSimpleFillSymbol.h"
#include "SysSimpleLineSymbol.h"
#include "SysSimplePointSymbol.h"
#include "SysRGBColor.h"
#include "SysFeatureLayer.h"
#include "SysDisplayTransform.h"
#include "SysRenderContext.h"
#include "SysMapManager.h"
#include "SysUtilFunc.h"
#include "SysMap.h"
#include "SysSpatialIndex.h"
#include "SysFeatureCache.h"



//ogr
#include "ogrsf_frmts.h"

#include "SysUtility/SmartPtr.h"
#include "SysDataSource/SpatialReference.h"

#include "SysUtility/SysLog.h"

#include "SysDataSource/IFeatureDataLayer.h"

#include <GL/glew.h>

namespace SysDraw
{

	SysSingleSymbolRenderer::SysSingleSymbolRenderer(void)
	{
		m_pSymbol = NULL;

		
	}

	SysSingleSymbolRenderer::~SysSingleSymbolRenderer(void)
	{
		//glDeleteBuffers(1,&m_nVertexBuffer);
	}

	//SysSymbol* SysSingleSymbolRenderer::GetDefaultSymbol(int nGeomType)
	//{
	//	SysSymbol* pSymbol = NULL;
	//	switch (nGeomType)
	//	{
	//	case wkbPoint:
	//	case wkbPoint25D:
	//	case wkbMultiPoint:
	//	case wkbMultiPoint25D:
	//		{
	//			pSymbol = new(std::nothrow) SysSimplePointSymbol;
	//		}
	//		break;

	//	case wkbLineString:
	//	case wkbLineString25D:
	//	case wkbMultiLineString:
	//	case wkbMultiLineString25D:
	//		{
	//			pSymbol = new(std::nothrow) SysSimpleLineSymbol;
	//		}
	//		break;

	//	case wkbPolygon:
	//	case wkbPolygon25D:
	//	case wkbMultiPolygon:
	//	case wkbMultiPolygon25D:
	//		{
	//			pSymbol = new(std::nothrow) SysSimpleFillSymbol;
	//		}
	//		break;

	//	default:
	//		break;
	//	}

	//	//随机设置颜色
	//	if (pSymbol != NULL)
	//	{
	//		//pSymbol->SetOutLineColor(SysRGBAColor(156.0f/255,156.0f/255,156.0f/255));
	//		pSymbol->SetOutLineColor(SysRGBAColor(0.0f/255,0.0f/255,0.0f/255));
	//		pSymbol->SetOutLineWidth(0.4);

	//		srand((unsigned int)time(NULL));
	//		float fRed = rand()%255 / 255.0f;
	//		float fGreen = rand()%255 / 255.0f;
	//		float fBlue = rand()%255 / 255.0f;
	//		pSymbol->SetColor(SysRGBAColor(fRed,fGreen,fBlue));
	//	}
	//	return pSymbol;
	//}

	std::string SysSingleSymbolRenderer::GetRendererName() const
	{
		return _T("Single Symbol Renderer");
	}

	std::string SysSingleSymbolRenderer::GetRendererChsName() const
	{
		return _T("单一符号渲染器");
	}

	Render_Type SysSingleSymbolRenderer::GetRendererType() const
	{
		return SINGLE_SYMBOL_RENDERER;
	}

	SysFeatureRenderer *SysSingleSymbolRenderer::Clone() const
	{
		return new SysSingleSymbolRenderer;
	}

	void SysSingleSymbolRenderer::BeginRender( SysRenderContext* pCcontext, const SysFeatureLayer *pLayer )
	{
		SysDisplayTransform disTran = pCcontext->GetDisplayTran();

		SysEnvelope envView;
		MapExtentToLayerExtent(pManager->GetCurrentMap(),(SysMapLayer *)pLayer,envView);

		SysDataSource::IFeatureDataLayer* pFeaLayer = (SysDataSource::IFeatureDataLayer *)pLayer->GetDataSource();

		Envelope envSearch;
		envSearch.init(envView.minX,envView.maxX,envView.minY,envView.maxY);
		//pFeaLayer->SetSpatialFilter(envSearch);

		SysSpatialIndex* pIndex = pLayer->GetSpatialIndex();
		std::vector<long> vecQuery = pIndex->IntersectQuery(envView);
		std::sort(vecQuery.begin(),vecQuery.end());

		std::vector<GeoFeature* > vecSelFeatures;

		clock_t t1 = clock();

		//GeoFeature* pFeature = NULL;
		//while ((pFeature = pFeaLayer->GetNextFeature()) != NULL)
		//{
		//	//动态投影
		//	GeoGeometry* pGeometry = pFeature->GetGeometryRef();
		//	OGRSpatialReference *pMapSrs = (OGRSpatialReference *)pManager->GetCurrentMap()->GetSpatialRef()->GetOGRSpatialReference();
		//	if (pMapSrs != NULL)
		//	{
		//		if (pLayerSrs != NULL)
		//		{
		//			OGRSpatialReference* pOgrLyrSrs = (OGRSpatialReference *)pLayerSrs->GetOGRSpatialReference();
		//			if (!pMapSrs->IsSame(pOgrLyrSrs))
		//			{
		//				pGeometry->transformTo(pMapSrs);
		//			}
		//		}
		//	}
		//	
		//	bool bHighLight = vlayer->IsSelected(pFeature->GetFID());
		//	if (bHighLight)
		//	{
		//		vecSelFeatures.push_back(pFeature);
		//	}

		//	else
		//	{
		//		m_pSymbol->RenderFeature((const SysFeature*)pFeature, pCcontext, bHighLight);
		//		GeoFeature::DestroyFeature(pFeature);
		//	}
		//	
		//}

		//判断是否需要做动态投影
		SysSRS::CSpatialReference* pLayerSrs = pLayer->GetSpatialRef();
		SysSRS::CSpatialReference* pMapSrs = pManager->GetCurrentMap()->GetSpatialRef();
		bool bDynamicProj = false;
		OGRSpatialReference *pOgrMapSrs = NULL;
		if (pLayerSrs != NULL && pMapSrs != NULL)
		{
			pOgrMapSrs = (OGRSpatialReference *)pMapSrs->GetOGRSpatialReference();
			OGRSpatialReference* pOgrLyrSrs = (OGRSpatialReference *)pLayerSrs->GetOGRSpatialReference();

			if (pOgrMapSrs != NULL && pOgrLyrSrs != NULL)
			{
				if (!pOgrMapSrs->IsSame(pOgrLyrSrs))
				{
					bDynamicProj = true;
				}
			}
		}
		

		SysFeatureLayer::mapFeatureCache::const_iterator iter = pLayer->m_mapFeaCache.begin();
		SysFeatureLayer::mapFeatureCache::const_iterator iterEnd = pLayer->m_mapFeaCache.end();

		std::vector<long>::const_iterator IterId = vecQuery.begin();
		std::vector<long>::const_iterator IterIdEnd = vecQuery.end();

		SysFeatureCache* pFeaCache = pLayer->GetFeatureCache();

		GeoFeature* pFeature = NULL;
		while (IterId != IterIdEnd)
		{
			long nFid = *IterId;

			/*iter = pLayer->m_mapFeaCache.find(nFid);
			if (iter != iterEnd)
			{
				pFeature = (GeoFeature *)iter->second;
			}

			else
			{
				pFeature = pFeaLayer->GetFeature(nFid);
			}*/

			pFeaCache->FindFeature(nFid,(SysFeature *&)pFeature);
			if (NULL == pFeature)
			{
				pFeature = pFeaLayer->GetFeature(nFid);
			}

			//动态投影
			GeoGeometry* pGeometry = pFeature->GetGeometryRef();
			if (bDynamicProj)
			{
				pGeometry->transformTo(pOgrMapSrs);
			}

			bool bHighLight = pLayer->IsSelected(pFeature->GetFID());
			if (bHighLight)
			{
				vecSelFeatures.push_back(pFeature);
			}

			else
			{
				m_pSymbol->RenderFeature((const SysFeature*)pFeature, pCcontext, bHighLight);
				//GeoFeature::DestroyFeature(pFeature);
			}

			++IterId;

		}

		clock_t t2 = clock();
		smartlog<<"矢量绘制时间"<<t2-t1;

		//绘制高亮的元素
		int nSelCount = vecSelFeatures.size();
		for (int i = 0; i < nSelCount; i ++)
		{
			pFeature = vecSelFeatures[i];
			m_pSymbol->RenderFeature((const SysFeature*)pFeature,pCcontext, true);
			//GeoFeature::DestroyFeature(pFeature);
		}
		vecSelFeatures.clear();
	}

	const SysSymbolList SysSingleSymbolRenderer::GetSymbols() const
	{
		SysSymbolList lstSym;
		lstSym.push_back(m_pSymbol);
		return lstSym;
	}

	void SysSingleSymbolRenderer::SetSymbol(SysSymbol* pSymbol)
	{
		if (m_pSymbol != NULL)
		{
			delete m_pSymbol;
		}

		m_pSymbol = pSymbol;
	}

	SysSymbol* SysSingleSymbolRenderer::GetSymbol( SysFeature* pFeature ) const
	{
		return m_pSymbol;
	}
}
