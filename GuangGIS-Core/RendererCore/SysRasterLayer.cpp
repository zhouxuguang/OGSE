
#include <assert.h>
#include <GL/glew.h>
#include "SysRasterLayer.h"
#include "SysRenderContext.h"
#include "SysDrawDef.h"
#include "SysImagePreProcess.h"
#include "SysRasterSource.h"
#include "SysRasterTile.h"
#include "SysRasterBandStats.h"
#include "SysColorRamp.h"
#include "SysSrsMapToLayer.h"



namespace SysDraw
{



static int GetPseudoColorIndex(void* pData,std::vector<ValueInter> vecValueInters,GDALDataType eType)
{
	double dbValue = 0;
	switch (eType)
	{
	case GDT_Byte:
		dbValue = ((eByte*)pData)[0];
		break;

	case GDT_Int16:
		dbValue = ((eInt16*)pData)[0];
		break;

	case GDT_UInt16:
		dbValue = ((eUInt16*)pData)[0];
		break;

	case GDT_Int32:
		dbValue = ((eInt32*)pData)[0];
		break;

	case GDT_UInt32:
		dbValue = ((eUInt32*)pData)[0];
		break;

	case GDT_Float32:
		dbValue = ((eFloat*)pData)[0];
		break;

	case GDT_Float64:
		dbValue = ((eDouble*)pData)[0];
		break;

	default:
		dbValue = 0;
		break;
	}

	//二分查找数据区间
	int nCount = vecValueInters.size();
	int nLow = 0;
	int nHigh = nCount - 1;

	double dbMin = 0;
	double dbMax = 0;

	
	while (nLow <= nHigh)
	{
		int nMid = (nLow + nHigh)/2;
		dbMin = vecValueInters[nMid].first;
		dbMax = vecValueInters[nMid].second;
		if (dbValue >= dbMin && dbValue <= dbMax)
		{
			return nMid;
		}

		else if (dbValue > dbMax)
		{
			nLow = nMid + 1;
		}

		else if (dbValue < dbMin)
		{
			nHigh = nMid - 1;
		}
	}

	return -1;
}

}

namespace SysDraw
{

	SysRasterLayer::SysRasterLayer(std::string strLayerName)
		:SysMapLayer( RasterLayer, strLayerName )
	{
#ifdef _WIN32
		GDALSetUseOverview(1);
#endif
		
		Init();

		glGenTextures(1, &m_nTexId);
	}

	/*SysRasterLayer::SysRasterLayer(LayerType type , std::string strLyrName)
	:SysMapLayer( RasterLayer, strLyrName )
	{
	Init();
	}*/

	SysRasterLayer::~SysRasterLayer(void)
	{
		m_eDrawStyle = UndefinedDrawingStyle;
		m_nBlueIndex = 0;
		m_nGreenIndex = 0;
		m_nRedIndex = 0;

		m_nGrayIndex = 0;

		m_nWidth = 0;
		m_nHeight = 0;

		//m_vecDataTypes.clear();

		m_bInvertColor = false;
		m_bHasPyramids = false;
		m_bDynamicProj = false;
		m_dbNoDataValue = -9999.0f;

		if (m_pDatalayer != NULL)
		{
			//delete m_pDatalayer;
			m_pDatalayer = NULL;
		}

		if (m_pDataSource != NULL)
		{
			delete m_pDataSource;
			m_pDataSource = NULL;
		}


		glDeleteTextures(1,&m_nTexId);
	}

	void SysRasterLayer::CalcStatisInfo(DrawStyle eDrawStyle)
	{
		SysDataSource::IRasterDataLayer* pRasLayer = (SysDataSource::IRasterDataLayer*)m_pDatalayer;

		if (eDrawStyle == MultiBandColor)
		{
			pRasLayer->SetBandNullValue(m_nRedIndex,0);
			pRasLayer->SetBandNullValue(m_nGreenIndex,0);
			pRasLayer->SetBandNullValue(m_nBlueIndex,0);
			
			/*pRasLayer->GetDisplayStatisInfo(m_nRedIndex,m_pfHistR,m_pfMax[0],m_pfMin[0],m_pfMean[0],m_pfVar[0]);
			pRasLayer->GetDisplayStatisInfo(m_nGreenIndex,m_pfHistG,m_pfMax[1],m_pfMin[1],m_pfMean[1],m_pfVar[1]);
			pRasLayer->GetDisplayStatisInfo(m_nBlueIndex,m_pfHistB,m_pfMax[2],m_pfMin[2],m_pfMean[2],m_pfVar[2]);*/

			int nCount1 = 0;
			GetDrawStatisInfo(m_nRedIndex,m_pHist,m_pfMax[0],m_pfMin[0],m_pfMean[0],m_pfVar[0],nCount1);
			int nCount2 = 0;
			GetDrawStatisInfo(m_nGreenIndex,m_pHist+256,m_pfMax[1],m_pfMin[1],m_pfMean[1],m_pfVar[1],nCount2);
			int nCount3 = 0;
			GetDrawStatisInfo(m_nBlueIndex,m_pHist+256*2,m_pfMax[2],m_pfMin[2],m_pfMean[2],m_pfVar[2],nCount3);

			if (m_eStretchType == StretchOptimizedLinear)
			{
				CreateOptimizedStretch(m_pHist,m_pfMin[0],m_pfMax[0],nCount1);
				CreateOptimizedStretch(m_pHist+256,m_pfMin[1],m_pfMax[1],nCount2);
				CreateOptimizedStretch(m_pHist+256*2,m_pfMin[2],m_pfMax[2],nCount3);
			}

			else if (m_eStretchType == StretchPercentOne)
			{
				CreateLinearStretch(m_pHist,m_pfMin[0],m_pfMax[0],nCount1,1);
				CreateLinearStretch(m_pHist+256,m_pfMin[1],m_pfMax[1],nCount2,1);
				CreateLinearStretch(m_pHist+256*2,m_pfMin[2],m_pfMax[2],nCount3,1);
			}

			else if (m_eStretchType == StretchPercentTwo)
			{
				CreateLinearStretch(m_pHist,m_pfMin[0],m_pfMax[0],nCount1,2);
				CreateLinearStretch(m_pHist+256,m_pfMin[1],m_pfMax[1],nCount2,2);
				CreateLinearStretch(m_pHist+256*2,m_pfMin[2],m_pfMax[2],nCount3,2);
			}

			else if (m_eStretchType == StretchPercentFive)
			{
				CreateLinearStretch(m_pHist,m_pfMin[0],m_pfMax[0],nCount1,5);
				CreateLinearStretch(m_pHist+256,m_pfMin[1],m_pfMax[1],nCount2,5);
				CreateLinearStretch(m_pHist+256*2,m_pfMin[2],m_pfMax[2],nCount3,5);
			}
			

		}

		else if (eDrawStyle == SingleBandGray)
		{
			pRasLayer->SetBandNullValue(m_nGrayIndex,0);
			//pRasLayer->GetDisplayStatisInfo(m_nGrayIndex,m_pfHistR,m_pfMax[0],m_pfMin[0],m_pfMean[0],m_pfVar[0]);

			int nCount1 = 0;
			GetDrawStatisInfo(m_nGrayIndex,m_pHist,m_pfMax[0],m_pfMin[0],m_pfMean[0],m_pfVar[0],nCount1);

			if (m_eStretchType == StretchOptimizedLinear)
			{
				CreateOptimizedStretch(m_pHist,m_pfMin[0],m_pfMax[0],nCount1);
			}

			else if (m_eStretchType == StretchPercentOne)
			{
				CreateLinearStretch(m_pHist,m_pfMin[0],m_pfMax[0],nCount1,1);
			}

			else if (m_eStretchType == StretchPercentTwo)
			{
				CreateLinearStretch(m_pHist,m_pfMin[0],m_pfMax[0],nCount1,2);
			}

			else if (m_eStretchType == StretchPercentFive)
			{
				CreateLinearStretch(m_pHist,m_pfMin[0],m_pfMax[0],nCount1,5);
			}
		}

	}

	bool SysRasterLayer::SetDataSource(SysDataSource::IDataLayer* pDatalayer,
										DrawProgressFunc ProgressFunc, 
										void* pProgressArg)
	{
		//assert(false);
		SysDataSource::DATAOBJECT_TYPE eLayer = pDatalayer->GetType();
		if (eLayer != SysDataSource::SYS_RASTERLAYER)
		{
			return false;
		}

		SysDataSource::IRasterDataLayer* pRasLayer = (SysDataSource::IRasterDataLayer*)pDatalayer;
		if (NULL == pRasLayer)
		{
			return false;
		}

		//设置地理坐标范围
		Envelope envGeo = pDatalayer->GetLayerEnvelope();
		SysEnvelope env;
		env.minX = envGeo.getMinX();
		env.maxX = envGeo.getMaxX();
		env.minY = envGeo.getMinY();
		env.maxY = envGeo.getMaxY();
		SetLayerExtent(env);

		//设置空间参考系统
		SysSRS::CSpatialReferencePtr ptrSrs = pDatalayer->GetSpatialReference();
		if (ptrSrs != NULL)
		{
			std::string strWkt = _T("");
			ptrSrs->ExportToWkt(strWkt);
			SetSpatialRef(strWkt);
		}

		//设置是否有金字塔
		m_bHasPyramids = (pRasLayer->GetPyramidLevel() > 0);

		SysDataSource::Raster_Info rasInfo = pRasLayer->GetRasterInfo();
		m_nWidth = rasInfo.m_width;
		m_nHeight = rasInfo.m_height;

		m_nBandCount = pRasLayer->GetBandCount();

		//数据类型
		/*m_vecDataTypes.clear();
		for (int i = 0; i < m_nBandCount; i ++)
		{
			SysRasDataType eDataType = (SysRasDataType)rasInfo.m_vecBandType[i];
			m_vecDataTypes.push_back(eDataType);
		}*/

		m_vecBandStatus.resize(m_nBandCount,NULL);
		if (m_nBandCount == 1)
		{
			std::vector<GDALColorEntry> vecCorTable;
			pRasLayer->GetBandColorTable(0,vecCorTable);
			bool bHasColorTbl = (vecCorTable.size() > 0);
			if (!bHasColorTbl)
			{
				m_eDrawStyle = SingleBandGray;
				m_nGrayIndex = 0;
			}
			else
			{
				m_eDrawStyle = PalettedColor;

				//复制颜色表
				m_vecColorTbl.clear();
				size_t nCount = vecCorTable.size();
				m_vecColorTbl.resize(nCount);
				for (size_t i = 0; i < nCount; i ++)
				{
					m_vecColorTbl[i].c1 = vecCorTable[i].c1;
					m_vecColorTbl[i].c2 = vecCorTable[i].c2;
					m_vecColorTbl[i].c3 = vecCorTable[i].c3;
					m_vecColorTbl[i].c4 = vecCorTable[i].c4;
				}
			}

		}

		else if (m_nBandCount == 3)
		{
			m_eDrawStyle = MultiBandColor;
			m_nRedIndex = 0;
			m_nGreenIndex = 1;
			m_nBlueIndex = 2;
		}

		else if (m_nBandCount > 3)
		{
			m_eDrawStyle = MultiBandColor;
			m_nRedIndex = 2;
			m_nGreenIndex = 1;
			m_nBlueIndex = 0;
		}

		//SetDrawStyle(SingleBandPseudoColor);

		m_pDatalayer = pDatalayer;

		SysDataSource::CDataDriverFactory* pFactory = SysDataSource::CDataDriverFactory::Instance();
		//pFactory->GetAllDriverInfo();
		SysDataSource::stOpenFileWay open_way;

		bool bBuildPyrmid = false;
		if (m_bHasPyramids)
		{
			open_way.m_PyramidBuildMode = SysDataSource::NoBuild;
		}
		else
		{
			open_way.m_PyramidBuildMode = SysDataSource::BuildNoTip;
			bBuildPyrmid = true;
		}
		
		open_way.m_fSample = pFactory->GetPyramidScale();
		open_way.m_SampleType = pFactory->GetSampleScale();
		open_way.m_AccessType = GA_ReadOnly;

		SysDataSource::IDataLayerPtr pRasDataLayer = pFactory->CreateDataLayer(GetLayerName(),open_way);
		m_pDataSource->SetDataSource(pRasDataLayer);
		m_pDatalayer = m_pDataSource->GetDataSource().get();

		//需要创建金字塔时，就创建金字塔
		if (bBuildPyrmid)
		{
			/*SysDataSource::IRasterDataLayerPtr pLayer = (SysDataSource::IRasterDataLayerPtr)pRasDataLayer;
			if (pLayer != NULL)
			{
				pLayer->BuildPyramid(open_way.m_fSample,open_way.m_SampleType);
			}*/

			BuildPyramid(open_way.m_fSample,open_way.m_SampleType,ProgressFunc,pProgressArg);
			//return false;
		}

		CalcStatisInfo(m_eDrawStyle);
		return true;
	}

	SysDataSource::IDataLayer* SysRasterLayer::GetDataSource() const
	{
		return m_pDatalayer;
	}

	SysRasterLayer* SysRasterLayer::CreateRasterLayer(SysDataSource::IDataLayer* pDatalayer,
														DrawProgressFunc ProgressFunc, 
														void* pProgressArg)
	{
		if (NULL == pDatalayer)
		{
			return NULL;
		}
		SysRasterLayer *pLayer = new SysRasterLayer(pDatalayer->GetName());
		if (!pLayer->SetDataSource(pDatalayer,ProgressFunc,pProgressArg))
		{
			delete pLayer;
			return NULL;
		}

		//pLayer->m_LayerExtent = pDatalayer->GetLayerEnvelope();

		return pLayer;
	}

	void SysRasterLayer::DestroyRasterLayer(SysRasterLayer* pRasLayer)
	{
		if (pRasLayer != NULL)
		{
			delete pRasLayer;
			pRasLayer = NULL;
		}
	}

	//建立金字塔的传递的参数
	static struct BuildThreadPara 
	{
		double m_dbSampleRate;			//采样率
		int m_nSampleType;				//采样类型
		DrawProgressFunc m_pFun;		//进度回调函数
		void* m_pProgressArg;			//参数
		SysDataSource::IRasterDataLayer* m_pDatalayer;		//图层
	};

	void* BuildPyramidThreadFunc(void* pParaThread)
	{
		BuildThreadPara* pPara = (BuildThreadPara *)pParaThread;
		SysDataSource::IRasterDataLayer* pDatalayer = pPara->m_pDatalayer;
		double fSampleRate = pPara->m_dbSampleRate;
		int nSampleType = pPara->m_nSampleType;
		DrawProgressFunc ProgressFunc = pPara->m_pFun;
		void* pProgressArg = pPara->m_pProgressArg;


		SysDataSource::DadaSampleType eSampleType = (SysDataSource::DadaSampleType)nSampleType;
		pDatalayer->BuildPyramid(fSampleRate,eSampleType,ProgressFunc,pProgressArg);

		delete pPara;

		return pProgressArg;
	}

	DWORD SYSDRAW_STD_CALL BuildPyramidThreadFunc2(void* pParaThread)
	{
		BuildThreadPara* pPara = (BuildThreadPara *)pParaThread;
		SysDataSource::IRasterDataLayer* pDatalayer = pPara->m_pDatalayer;
		double fSampleRate = pPara->m_dbSampleRate;
		int nSampleType = pPara->m_nSampleType;
		DrawProgressFunc ProgressFunc = pPara->m_pFun;
		void* pProgressArg = pPara->m_pProgressArg;


		SysDataSource::DadaSampleType eSampleType = (SysDataSource::DadaSampleType)nSampleType;
		pDatalayer->BuildPyramid(fSampleRate,eSampleType,ProgressFunc,pProgressArg);

		delete pPara;

		return 1;
	}

	bool SysRasterLayer::BuildPyramid( 
					float fSampleRate, 
					int nSampleType, 
					DrawProgressFunc ProgressFunc /*= NULL*/, 
					void* pProgressArg /*= NULL*/ )
	{
		SysDataSource::IRasterDataLayer* pDatalayer = (SysDataSource::IRasterDataLayer*)m_pDatalayer;
		if (NULL == pDatalayer)
		{
			return false;
		}

		BuildThreadPara* pPara = new BuildThreadPara;
		pPara->m_dbSampleRate = fSampleRate;
		pPara->m_nSampleType = nSampleType;
		pPara->m_pDatalayer = pDatalayer;
		pPara->m_pFun = ProgressFunc;
		pPara->m_pProgressArg = pProgressArg;

		/*SysThread sThread;
		sThread.Start(BuildPyramidThreadFunc,pPara);
		Sleep(8000);*/

		/*HANDLE hThread = CreateThread(NULL,0,BuildPyramidThreadFunc2,pPara,0,NULL);
		WaitForSingleObject(hThread,INFINITE);*/

		SysDataSource::DadaSampleType eSampleType = (SysDataSource::DadaSampleType)nSampleType;
		pDatalayer->BuildPyramid(fSampleRate,eSampleType,ProgressFunc,pProgressArg);

		return true;

		
	}

	void SysRasterLayer::GetRGBIndex(int& nRedIndex,int& nGreenIndex,int& nBlueIndex) const
	{
		nRedIndex = m_nRedIndex;
		nGreenIndex = m_nGreenIndex;
		nBlueIndex = m_nBlueIndex;
	}

	void SysRasterLayer::SetRGBIndex(const int nRedIndex,const int nGreenIndex,const int nBlueIndex)
	{
		if (m_nRedIndex != nRedIndex)
		{
			if (nRedIndex >= 0 && nRedIndex < m_nBandCount)
			{
				m_nRedIndex = nRedIndex;
				CalcDrawStatisInfo(m_eDrawStyle,nRedIndex);
			}
			
		}

		if (m_nGreenIndex != nGreenIndex)
		{
			if (nGreenIndex >= 0 && nGreenIndex < m_nBandCount)
			{
				m_nGreenIndex = nGreenIndex;
				CalcDrawStatisInfo(m_eDrawStyle,nGreenIndex);
			}
		}

		if (m_nBlueIndex != nBlueIndex)
		{
			if (nBlueIndex >= 0 && nBlueIndex < m_nBandCount)
			{
				m_nBlueIndex = nBlueIndex;
				CalcDrawStatisInfo(m_eDrawStyle,nBlueIndex);
			}
		}

		m_nGrayIndex = -1;

	}

	inline int SysRasterLayer::GetGrayIndex() const
	{
		return m_nGrayIndex;
	}

	void SysRasterLayer::SetGrayIndex( const int nGrayIndex )
	{
		if (m_nGrayIndex != nGrayIndex)
		{
			if (nGrayIndex >= 0 && nGrayIndex < m_nBandCount)
			{
				m_nGrayIndex = nGrayIndex;
				CalcDrawStatisInfo(m_eDrawStyle,nGrayIndex);
			}
		}

		m_nRedIndex = -1;
		m_nGreenIndex = -1;
		m_nBlueIndex = -1;
	}

	int SysRasterLayer::GetBandCount(void) const
	{
		return m_nBandCount;
	}

	bool SysRasterLayer::Draw( SysRenderContext* rendererContext )
	{
		if (!IsVisible())
		{
			return false;
		}
		const SysDisplayTransform& theDispTran = rendererContext->GetDisplayTran();

		SysEnvelope myProjectedViewExtent;		//当前视图的范围
		SysEnvelope myProjectedLayerExtent;		//当前图层的地理范围

		theDispTran.GetVisibleBounds(myProjectedViewExtent);
		myProjectedLayerExtent = GetLayerExtent();

		//动态投影转换
		/*SysSRS::CCoordTransformation *pTran = rendererContext->GetCoordTran();*/
		SysSRS::CCoordTransformation *pTran = GetForwardCoordTran();
		if ( pTran != NULL )
		{
			//转换视图的地理范围,是否必要待考证
			Envelope envResult;
			double dbExtent[4];
			/*dbExtent[0] = myProjectedViewExtent.minX;
			dbExtent[1] = myProjectedViewExtent.maxX;
			dbExtent[2] = myProjectedViewExtent.minY;
			dbExtent[3] = myProjectedViewExtent.maxY;
			pTran->TransformBoundingBox(dbExtent,envResult);
			memcpy(&myProjectedViewExtent,&envResult,sizeof(SysEnvelope));*/

			//转换当前图层的地理范围
			dbExtent[0] = myProjectedLayerExtent.minX;
			dbExtent[1] = myProjectedLayerExtent.maxX;
			dbExtent[2] = myProjectedLayerExtent.minY;
			dbExtent[3] = myProjectedLayerExtent.maxY;
			pTran->TransformBoundingBox(dbExtent,envResult);
			//memcpy(&myProjectedLayerExtent,&envResult,sizeof(SysEnvelope));
			myProjectedLayerExtent.minX = envResult.getMinX();
			myProjectedLayerExtent.maxX = envResult.getMaxX();
			myProjectedLayerExtent.minY = envResult.getMinY();
			myProjectedLayerExtent.maxY = envResult.getMaxY();

			m_bDynamicProj = true;
		}

		//求相交区域
		SysEnvelope myRasterExtent;
		bool bIntersect = myProjectedViewExtent.Intersection( myProjectedLayerExtent,myRasterExtent);

		//没有相交区域就不绘制
		if ( /*myRasterExtent.IsNull()*/ !bIntersect )
		{
			return true;
		}

		/*glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER,0.0f);*/

		DrawImage(NULL,rendererContext,myRasterExtent);
		//glDisable(GL_ALPHA_TEST);

		return true;
	}

	bool SysRasterLayer::Draw( SysRenderContext* rendererContext ,const SysEnvelope& envView )
	{
		if (!IsVisible())
		{
			return false;
		}
		const SysDisplayTransform& theDispTran = rendererContext->GetDisplayTran();

		SysEnvelope myProjectedViewExtent = envView;							//当前视图的范围
		SysEnvelope myProjectedLayerExtent = GetLayerExtent();					//当前图层的地理范围

		//动态投影转换
		SysSRS::CCoordTransformation *pTran = GetForwardCoordTran();
		if ( pTran != NULL )
		{
			//转换视图的地理范围,是否必要待考证
			Envelope envResult;
			double dbExtent[4];

			//转换当前图层的地理范围
			dbExtent[0] = myProjectedLayerExtent.minX;
			dbExtent[1] = myProjectedLayerExtent.maxX;
			dbExtent[2] = myProjectedLayerExtent.minY;
			dbExtent[3] = myProjectedLayerExtent.maxY;
			pTran->TransformBoundingBox(dbExtent,envResult);
			myProjectedLayerExtent.minX = envResult.getMinX();
			myProjectedLayerExtent.maxX = envResult.getMaxX();
			myProjectedLayerExtent.minY = envResult.getMinY();
			myProjectedLayerExtent.maxY = envResult.getMaxY();

			m_bDynamicProj = true;
		}

		//求相交区域
		SysEnvelope myRasterExtent;
		myProjectedViewExtent.Intersection( myProjectedLayerExtent,myRasterExtent);

		//没有相交区域就不绘制
		if ( myRasterExtent.IsNull() )
		{
			return true;
		}

		DrawImage(NULL,rendererContext,myRasterExtent);

		return true;
	}

	void SysRasterLayer::DrawLabels( SysRenderContext* rendererContext )
	{
		assert(false);
		return;
	}

	void SysRasterLayer::Init()
	{
		m_eDrawStyle = UndefinedDrawingStyle;
		m_eStretchType = StretchPercentTwo;		//默认最优化线性拉伸显示
		m_nBlueIndex = 0;
		m_nGreenIndex = 0;
		m_nRedIndex = 0;

		m_nBandCount = 0;

		m_nGrayIndex = 0;

		m_nTransparency = 0;
		m_fBrightness = 0;
		m_nContrast = 0;

		m_nWidth = 0;
		m_nHeight = 0;

		m_pDatalayer = NULL;
		m_bInvertColor = false;
		m_bHasPyramids = false;
		m_bDynamicProj = false;
		m_dbNoDataValue = -9999.0f;
		m_fStandardDeviations = 2.0;

		m_pDataSource = new SysRasterSource;

		m_vecBandStatus.clear();

		m_vecValueInters.clear();
		m_vecPseudoColor.clear();

		/*memset(m_pfHistR,0,sizeof(float)*256);
		memset(m_pfHistG,0,sizeof(float)*256);
		memset(m_pfHistB,0,sizeof(float)*256);*/
		memset(m_pHist,0,sizeof(float)*256*3);

		//test
		//m_vecValueInters.push_back(make_pair(-145,238.2352941));
		//m_vecValueInters.push_back(make_pair(238.2352941,580.4470588));
		//m_vecValueInters.push_back(make_pair(580.4470588,973.3568627));
		//m_vecValueInters.push_back(make_pair(973.3568627,1454.988235));
		//m_vecValueInters.push_back(make_pair(1454.988235,3095));

		//SysColorItem sColor1;
		//sColor1.c1 = 255;
		//sColor1.c2 = 252;
		//sColor1.c3 = 255;
		///*sColor1.c1 = 245;
		//sColor1.c2 = 0;
		//sColor1.c3 = 0;*/
		////m_vecPseudoColor.push_back(sColor1);

		//SysColorItem sColor2;
		//sColor2.c1 = 252;
		//sColor2.c2 = 139;
		//sColor2.c3 = 0;
		////m_vecPseudoColor.push_back(sColor2);

		//SysColorItem sColor3;
		//sColor3.c1 = 245;
		//sColor3.c2 = 245;
		//sColor3.c3 = 0;
		////m_vecPseudoColor.push_back(sColor3);

		//SysColorItem sColor4;
		//sColor4.c1 = 148;
		//sColor4.c2 = 247;
		//sColor4.c3 = 0;
		////m_vecPseudoColor.push_back(sColor4);

		//SysColorItem sColor5;
		//sColor5.c1 = 128;
		//sColor5.c2 = 0;
		//sColor5.c3 = 0;
		///*sColor5.c1 = 0;
		//sColor5.c2 = 245;
		//sColor5.c3 = 0;*/
		////m_vecPseudoColor.push_back(sColor5);

		//SysColorRamp colorRamp(sColor1,sColor5);

		//SysColorItem* pColors = new SysColorItem[5];
		//colorRamp.GetColors(pColors,5);
		//for (int i = 0; i< 5 ;i ++)
		//{
		//	m_vecPseudoColor.push_back(pColors[i]);
		//}

		//delete []pColors;
	}

	float SysRasterLayer::GetStandardDeviations() const
	{
		return m_fStandardDeviations;
	}

	void SysRasterLayer::SetStandardDeviations(float fStandardDeviations )
	{
		m_fStandardDeviations = fStandardDeviations;
	}

	void SysRasterLayer::SetTransparency(int nTransparency)
	{
		if (nTransparency < 0)
		{
			nTransparency = 0;
		}

		if (nTransparency > 100)
		{
			nTransparency = 100;
		}
		m_nTransparency = nTransparency;

		//m_nTransparency = (100 - m_nTransparency)*(255/100.0);
	}

	eByte SysRasterLayer::GetTransparency() const
	{
		return (int) ( (100 - m_nTransparency)*(255/100.0) + 0.5);
	}

	void SysRasterLayer::SetBrightness(float fBrightness)
	{
		if (fBrightness < -1)
		{
			m_fBrightness = -1;
		}

		if (fBrightness > 1)
		{
			m_fBrightness = 1;
		}
		m_fBrightness = fBrightness;
	}

	void SysRasterLayer::SetContrastValue(int nValue)
	{
		if (nValue < -128)
		{
			nValue = -128;
		}

		if (nValue > 127)
		{
			nValue = 127;
		}

		m_nContrast = nValue;
	}

	void SysRasterLayer::DrawImage( void * thePainter, 
		SysRenderContext* rendererContext, const SysEnvelope& envRaster1 ) const
	{
		//转换相交的地理区域到图像的像素范围
		SysDataSource::IRasterDataLayer* pRasLayer = (SysDataSource::IRasterDataLayer*)m_pDatalayer;
		SysEnvelope envRaster = envRaster1;

		//转换到影像上的坐标系统的坐标
		SysSRS::CCoordTransformation *pInverseCoord = GetInverseCoordTran();
		if (pInverseCoord != NULL)
		{
			SysSrsMapToLayer::TransExtent(pInverseCoord,envRaster);
		}

		double nCol1 = 0;
		double nRow1 = 0;
		pRasLayer->WorldToPixel_Ex(envRaster.minX,envRaster.maxY,nCol1,nRow1);
		nCol1 -= 0.5;
		nRow1 -= 0.5;

		if (nCol1 < 0)
		{
			nCol1 = 0;
		}
		if (nCol1 >= m_nWidth)
		{
			nCol1 = m_nWidth-1;
		}

		if (nRow1 < 0)
		{
			nRow1 = 0;
		}
		if (nRow1 >= m_nHeight)
		{
			nRow1 = m_nHeight-1;
		}

		double nCol2 = 0;
		double nRow2 = 0;
		pRasLayer->WorldToPixel_Ex(envRaster.maxX,envRaster.minY,nCol2,nRow2);
		nCol2 += 0.5;
		nRow2 += 0.5;

		if (nCol2 < 0)
		{
			nCol2 = 0;
		}
		if (nCol2 >= m_nWidth)
		{
			nCol2 = m_nWidth-1;
		}

		if (nRow2 < 0)
		{
			nRow2 = 0;
		}
		if (nRow2 >= m_nHeight)
		{
			nRow2 = m_nHeight-1;
		}

		int nDrawWidth = nCol2 - nCol1 + 0;
		int nDrawHeight = nRow2 - nRow1 + 0;
		if (nDrawHeight <= 0 || nDrawWidth <= 0)
		{
			return;
		}

		double dbMinx = 0,dbMaxy = 0;
		pRasLayer->PixelToWorld_Ex(nCol1,nRow2,dbMinx ,dbMaxy);

		double dbMaxx = 0,dbMiny = 0;
		pRasLayer->PixelToWorld_Ex(nCol2,nRow1,dbMaxx ,dbMiny);

		//调整后的地理范围
		SysEnvelope envNew;
		envNew.Init(dbMinx,dbMaxx,dbMiny,dbMaxy);

		//构造读取图像的区域
		RECTD rectImg;
		rectImg.left = nCol1;
		rectImg.bottom = nRow1;
		rectImg.right = nCol2;
		rectImg.top = nRow2;

		//计算对应的屏幕区域
		SysDisplayTransform &dispTran = rendererContext->GetDisplayTran();

		//转换到地图上的坐标系统的坐标
		SysSRS::CCoordTransformation *pForwardCoord = GetForwardCoordTran();
		if (pForwardCoord != NULL)
		{
			SysSrsMapToLayer::TransExtent(pForwardCoord,envNew);
		}
		RECTD rectView;
		dispTran.ToScreenRect(envNew,rectView);

		switch(m_eDrawStyle)
		{
		case SingleBandGray:
			{
				DrawSingleBandGray(thePainter,rendererContext,rectImg,rectView, envNew);
				break;
			}

		case PalettedColor:
			{
				DrawPalettedSingleBandColor(thePainter,rendererContext,rectImg,rectView, envNew);
				break;
			}

		case SingleBandPseudoColor:
			{
				DrawSingleBandPseudoColor(thePainter,rendererContext,rectImg,rectView,envNew);
				break;
			}

		case MultiBandColor:
			{
				DrawMultiBandColor(thePainter,rendererContext,rectImg,rectView, envNew);
				break;
			}

		default:
			break;
		}
	}

	void DataStretch(void* poData,unsigned char* puData,size_t nLen,
		GDALDataType eType,StretchType eStretchType,float fMin,float fMax,float fVar,float fMean,float fDevCount)
	{
		switch (eType)
		{
		case GDT_Byte:
			{
				switch (eStretchType)
				{
				case NoStretch:
				case StretchToMinimumMaximum:
				case StretchOptimizedLinear:
				case StretchPercentOne:					
				case StretchPercentTwo:					
				case StretchPercentFive:		
					MinMaxStretch<GByte>((const GByte*)poData,puData,nLen,fMin,fMax);
					break;

				case StretchStandardDeviations:
					StandDevStretch<GByte>((const GByte*)poData,puData,
						nLen,fVar,fDevCount,fMean);

					break;

				default:
					break;
				}
				
			}
			break;

		case GDT_Int16:
			{
				switch (eStretchType)
				{
				case NoStretch:
				case StretchToMinimumMaximum:
				case StretchOptimizedLinear:
				case StretchPercentOne:					
				case StretchPercentTwo:					
				case StretchPercentFive:	
					MinMaxStretch<GInt16>((const GInt16*)poData,puData,nLen,fMin,fMax);
					break;

				case StretchStandardDeviations:
					StandDevStretch<GInt16>((const GInt16*)poData,puData,
						nLen,fVar,fDevCount,fMean);

					break;

				default:
					break;
				}
			}
			break;

		case GDT_UInt16:
			{
				switch (eStretchType)
				{
				case NoStretch:
				case StretchToMinimumMaximum:
				case StretchOptimizedLinear:
				case StretchPercentOne:					
				case StretchPercentTwo:					
				case StretchPercentFive:
					MinMaxStretch<GUInt16>((const GUInt16*)poData,puData,nLen,fMin,fMax);
					break;

				case StretchStandardDeviations:
					StandDevStretch<GUInt16>((const GUInt16*)poData,puData,
						nLen,fVar,fDevCount,fMean);

					break;

				default:
					break;
				}
			}
			break;

		case GDT_Int32:
			{
				switch (eStretchType)
				{
				case NoStretch:
				case StretchToMinimumMaximum:
				case StretchOptimizedLinear:
				case StretchPercentOne:					
				case StretchPercentTwo:					
				case StretchPercentFive:	
					MinMaxStretch<GInt32>((const GInt32*)poData,puData,nLen,fMin,fMax);
					break;

				case StretchStandardDeviations:
					StandDevStretch<GInt32>((const GInt32*)poData,puData,
						nLen,fVar,fDevCount,fMean);

					break;

				default:
					break;
				}
			}
			break;

		case GDT_UInt32:
			{
				switch (eStretchType)
				{
				case NoStretch:
				case StretchToMinimumMaximum:
				case StretchOptimizedLinear:
				case StretchPercentOne:					
				case StretchPercentTwo:					
				case StretchPercentFive:	
					MinMaxStretch<GUInt32>((const GUInt32*)poData,puData,nLen,fMin,fMax);
					break;

				case StretchStandardDeviations:
					StandDevStretch<GUInt32>((const GUInt32*)poData,puData,
						nLen,fVar,fDevCount,fMean);

					break;

				default:
					break;
				}
			}
			break;

		case GDT_Float32:
			{
				switch (eStretchType)
				{
				case NoStretch:
				case StretchToMinimumMaximum:
				case StretchOptimizedLinear:
				case StretchPercentOne:					
				case StretchPercentTwo:					
				case StretchPercentFive:	
					MinMaxStretch<float>((const float*)poData,puData,nLen,fMin,fMax);
					break;

				case StretchStandardDeviations:
					StandDevStretch<float>((const float*)poData,puData,
						nLen,fVar,fDevCount,fMean);

					break;

				default:
					break;
				}
			}
			break;

		case GDT_Float64:
			{
				switch (eStretchType)
				{
				case NoStretch:
				case StretchToMinimumMaximum:
				case StretchOptimizedLinear:
				case StretchPercentOne:					
				case StretchPercentTwo:					
				case StretchPercentFive:	
					MinMaxStretch<double>((const double*)poData,puData,nLen,fMin,fMax);
					break;

				case StretchStandardDeviations:
					StandDevStretch<double>((const double*)poData,puData,
						nLen,fVar,fDevCount,fMean);

					break;

				default:
					break;
				}
			}
			break;

		default:
			break;
		}
	}

	void DataTranslate(void* poData,unsigned char* puData,size_t nLen,
		GDALDataType eType,float fVar,float fMean)
	{
		switch (eType)
		{
		case GDT_Byte:
			{
				StandDevStretch<GByte>((const GByte*)poData,puData,
					nLen,fVar,2.5,fMean);
			}
			break;

		case GDT_Int16:
			{
				StandDevStretch<GInt16>((const GInt16*)poData,puData,
					nLen,fVar,2.5,fMean);
			}
			break;

		case GDT_UInt16:
			{
				StandDevStretch<GUInt16>((const GUInt16*)poData,puData,
					nLen,fVar,2.5,fMean);
			}
			break;

		case GDT_Int32:
			{
				StandDevStretch<GInt32>((const GInt32*)poData,puData,
					nLen,fVar,2.5,fMean);
			}
			break;

		case GDT_UInt32:
			{
				StandDevStretch<GUInt32>((const GUInt32*)poData,puData,
					nLen,fVar,2.5,fMean);
			}
			break;

		case GDT_Float32:
			{
				StandDevStretch<float>((const float*)poData,puData,
					nLen,fVar,2.5,fMean);
			}
			break;

		case GDT_Float64:
			{
				StandDevStretch<double>((const double*)poData,puData,
					nLen,fVar,2.5,fMean);
			}
			break;

		default:
			break;
		}
	}

	void SysRasterLayer::DrawSingleBandGray(
		void* thePainter,
		SysRenderContext* rendererContext,
		const RECTD& rectImage,
		const RECTD& rectView, const SysEnvelope& envDrawView) const
	{
		SysDataSource::IRasterDataLayer* pRasLayer = (SysDataSource::IRasterDataLayer*)m_pDatalayer;

		int nStartX = rectImage.left;
		int nStartY = rectImage.bottom;
		int nDrawWidth = abs((int)rectImage.right - (int)rectImage.left) + 1;
		int nDrawHeight = abs((int)rectImage.bottom - (int)rectImage.top) + 1;
		int nViewWidth = abs(rectView.right - rectView.left) + 1;
		int nViewHeight = abs(rectView.bottom - rectView.top) + 1;

		int nEndX = nStartX + nDrawWidth;
		int nEndY = nStartY + nDrawHeight;

		double winMinx,winMiny,winMaxx,winMaxy;
		winMinx = envDrawView.minX;
		winMiny = envDrawView.minY;
		winMaxx = envDrawView.maxX;
		winMaxy = envDrawView.maxY;

		pRasLayer->PixelToWorld_Ex(nStartX,nStartY,winMinx,winMaxy);
		pRasLayer->PixelToWorld_Ex(nEndX,nEndY,winMaxx,winMiny);

		SysEnvelope envDraw;
		envDraw.minX = winMinx;
		envDraw.minY = winMiny;
		envDraw.maxX = winMaxx;
		envDraw.maxY = winMaxy;

		SysEnvelope envLast = envDraw;

		SysSRS::CCoordTransformation* pForwardTran = GetForwardCoordTran();
		if (pForwardTran != NULL)
		{
			SysSrsMapToLayer::TransExtent(pForwardTran,envDraw);
		}
		winMinx = envDraw.minX;
		winMiny = envDraw.minY;
		winMaxx = envDraw.maxX;
		winMaxy = envDraw.maxY;

		SysDataSource::Raster_Info rasInfo = pRasLayer->GetRasterInfo();
		GDALDataType eType = rasInfo.m_vecBandType[m_nGrayIndex];

		unsigned char* pDataGray = (unsigned char*)malloc(nViewWidth*nViewHeight*GDALGetDataTypeSize(eType)/8);

		//读取数据和构造显示的数据
		CPLErr err1 = pRasLayer->DataRead(m_nGrayIndex,0,nStartX,nStartY,nDrawWidth,nDrawHeight,
			pDataGray,nViewWidth,nViewHeight,eType);

		float fMin[1] = {0};
		float fMax[1] = {0};
		float fMean[1] = {0};
		float fVar[1] = {0};
		pRasLayer->SetBandNullValue(m_nGrayIndex,0);
		pRasLayer->GetDisplayStatisInfo(m_nGrayIndex,NULL,fMax[0],fMin[0],fMean[0],fVar[0]);

		unsigned char* puRed = (unsigned char*)malloc(nViewWidth*nViewHeight);
		memset(puRed,0,nViewWidth*nViewHeight);

		DataStretch(pDataGray,puRed,nViewWidth*nViewHeight,eType,m_eStretchType,m_pfMin[0],m_pfMax[0],m_pfVar[0],m_pfMean[0],m_fStandardDeviations);


		free(pDataGray);

		//设置透明度
		int nRealTranspancy = (int) ( (100 - m_nTransparency)*(255/100.0) + 0.5);

		int nLen = nViewWidth*nViewHeight;

		GByte* poDataGray = new GByte[nLen*2];
		memset(poDataGray,0,nLen*2);

		SysSRS::CCoordTransformation* pInverseTran = GetInverseCoordTran();
		if (NULL == pInverseTran )
		{
			#pragma omp parallel for
			for (int i = 0; i < nLen; i++)
			{
				if (puRed[i] == 0)
				{
					unsigned char uTmpRed = ContrastStretch(255,m_nContrast);
					poDataGray[i*2] = BrightnessStretch(uTmpRed,m_fBrightness);
					//poDataGray[i*2+1] = 0;
				}
				else
				{
					unsigned char uTmpRed = ContrastStretch((puRed)[i],m_nContrast);
					poDataGray[i*2] = BrightnessStretch(uTmpRed,m_fBrightness);
					poDataGray[i*2+1] = nRealTranspancy;
				}
			}
		}

		else if (NULL != pInverseTran)
		{
			double dx = envDraw.GetWidth()/nViewWidth;
			double dy = envDraw.GetHeight()/nViewHeight;
			double dbGeoY = winMaxy - 0.0*dy;

			double dImgdx = envLast.GetWidth()/nViewWidth;
			double dImgdy = -envLast.GetHeight()/nViewHeight;

			for (int nRow = 0; nRow < nViewHeight; nRow ++)
			{
				double dbGeoX = winMinx + 0.0*dx;
				for (int nCol = 0; nCol < nViewWidth; nCol ++)
				{
					double dbX = dbGeoX;
					double dbY = dbGeoY;
					pInverseTran->Transform(1,&dbX,&dbY,NULL);
					double fPixel = 0;
					double fLine = 0;
					fPixel = (dbX - envLast.minX)/dImgdx;
					fLine = (dbY - envLast.maxY)/dImgdy;
					int nPixel = int(fPixel+0.5);
					int nLine = int(fLine + 0.5);

					int nOffsetDraw = nRow*nViewWidth + nCol;
					if (nLine < 0 || nLine >= nViewHeight || nPixel < 0 || nPixel >= nViewWidth)
					{
						unsigned char uTmpRed = ContrastStretch(255,m_nContrast);
						poDataGray[nOffsetDraw*2] = BrightnessStretch(uTmpRed,m_fBrightness);
						//poDataGray[nOffsetDraw*2+1] = 0;
					}

					else
					{
						int nOffset = nLine*nViewWidth + nPixel;
						eByte uRed = (puRed)[nOffset];
						unsigned char uTmpRed = ContrastStretch(uRed,m_nContrast);

						poDataGray[nOffsetDraw*2] = BrightnessStretch(uTmpRed,m_fBrightness);
						if (uRed != 0)
						{
							poDataGray[nOffsetDraw*2+1] = nRealTranspancy;
						}//if
					}

					dbGeoX += dx;
				}

				dbGeoY -= dy;
			}
		}

		if (m_bInvertColor)
		{
			BrightnessContrast(poDataGray,nLen*2, 2);
		}

		//调用显示函数进行显示
		glEnable(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, m_nTexId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, nViewWidth,
			nViewHeight, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE,poDataGray);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR);

		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D,m_nTexId);

		glBegin(GL_QUADS);
		glVertex3d(winMinx,winMiny,0);glTexCoord2d(0.0,0.0);
		glVertex3d(winMinx,winMaxy,0);glTexCoord2d(1.0,0.0);
		glVertex3d(winMaxx,winMaxy,0);glTexCoord2d(1.0,1.0);

		/*glVertex3d(winMaxx,winMaxy,0);glTexCoord2d(1.0,1.0);
		glVertex3d(winMinx,winMiny,0);glTexCoord2d(0.0,0.0);*/
		glVertex3d(winMaxx,winMiny,0);glTexCoord2d(0.0,1.0);
		//glVertex3d(winMinx,winMiny,0);glTexCoord2d(0.0,0.0);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		//glDeleteTextures(1,&tid);

		free(puRed);

		delete []poDataGray;

	}

	void SysRasterLayer::DrawMultiBandColor(
		void* thePainter,
		SysRenderContext* rendererContext,
		const RECTD& rectImage,
		const RECTD& rectView, const SysEnvelope& envDrawView) const
	{
		SysDataSource::IRasterDataLayer* pRasLayer = (SysDataSource::IRasterDataLayer*)m_pDatalayer;
		int nImgWidth = pRasLayer->GetRasterInfo().m_width;
		int nImgHeight = pRasLayer->GetRasterInfo().m_height;

		int nStartX = rectImage.left;
		int nStartY = rectImage.bottom;
		int nDrawWidth = int(rectImage.right) - int(rectImage.left) + 2;
		int nDrawHeight = int(rectImage.top) - int(rectImage.bottom) + 2;
		int nViewWidth = fabs(rectView.right - rectView.left) + 1;
		int nViewHeight = fabs(rectView.bottom - rectView.top) + 1;

		int nEndX = nStartX + nDrawWidth;
		int nEndY = nStartY + nDrawHeight;
		if (nEndX > nImgWidth)
		{
			nDrawWidth -= (nEndX - nImgWidth);
			nEndX = nImgWidth;
		}

		if (nEndY > nImgHeight)
		{
			nDrawHeight -= (nEndY - nImgHeight);
			nEndY = nImgHeight;
		}

		double winMinx,winMiny,winMaxx,winMaxy;
		winMinx = envDrawView.minX;
		winMiny = envDrawView.minY;
		winMaxx = envDrawView.maxX;
		winMaxy = envDrawView.maxY;

		pRasLayer->PixelToWorld_Ex(nStartX,nStartY,winMinx,winMaxy);
		pRasLayer->PixelToWorld_Ex(nEndX,nEndY,winMaxx,winMiny);

		SysEnvelope envDraw;
		envDraw.minX = winMinx;
		envDraw.minY = winMiny;
		envDraw.maxX = winMaxx;
		envDraw.maxY = winMaxy;

		SysEnvelope envLast = envDraw;

		SysSRS::CCoordTransformation* pForwardTran = GetForwardCoordTran();
		if (pForwardTran != NULL)
		{
			SysSrsMapToLayer::TransExtent(pForwardTran,envDraw);
		}
		winMinx = envDraw.minX;
		winMiny = envDraw.minY;
		winMaxx = envDraw.maxX;
		winMaxy = envDraw.maxY;

		
		//计算实际对应的纹理坐标
		double dTexStartX = (rectImage.left - nStartX)/nDrawWidth;
		double dTexStartY = 1.0 -(rectImage.top - nStartY)/nDrawHeight;
		double dTexEndX = (rectImage.right - nStartX) / nDrawWidth;
		double dTexEndY = 1.0 - (rectImage.bottom - nStartY)/nDrawHeight;

		//计算金字塔
		/*float fXRatio = float(nDrawWidth)/float(nViewWidth);
		float fYRatio = float(nDrawHeight)/float(nViewHeight);
		float fRatio = min(fXRatio,fYRatio);*/

		//int nLevel= int(log((double)fRatio)/log((double)2));

		//int nLayerCount = pRasLayer->GetPyramidLevel();

		SysDataSource::Raster_Info rasInfo = pRasLayer->GetRasterInfo();
		GDALDataType eType = rasInfo.m_vecBandType[0];

		unsigned char* pDataRed = (unsigned char*)malloc(nViewWidth*nViewHeight*GDALGetDataTypeSize(eType)/8);
		unsigned char* pDataGreen = (unsigned char*)malloc(nViewWidth*nViewHeight*GDALGetDataTypeSize(eType)/8);
		unsigned char* pDataBlue = (unsigned char*)malloc(nViewWidth*nViewHeight*GDALGetDataTypeSize(eType)/8);

		int nRed = m_nRedIndex+1;
		int nGreen = m_nGreenIndex+1;
		int nBlue = m_nBlueIndex+1;

		int bandlist[3] = {nRed,nGreen,nBlue};

		//读取数据和构造显示的数据
		CPLErr err1 = pRasLayer->DataReadBand(m_nRedIndex,0,nStartX,nStartY,nDrawWidth,nDrawHeight,nViewWidth,nViewHeight,pDataRed,eType);
		CPLErr err2 = pRasLayer->DataReadBand(m_nGreenIndex,0,nStartX,nStartY,nDrawWidth,nDrawHeight,nViewWidth,nViewHeight,pDataGreen,eType);
		CPLErr err3 = pRasLayer->DataReadBand(m_nBlueIndex,0,nStartX,nStartY,nDrawWidth,nDrawHeight,nViewWidth,nViewHeight,pDataBlue,eType);

		unsigned char* puRed = (unsigned char*)malloc(nViewWidth*nViewHeight);
		unsigned char* puGreen = (unsigned char*)malloc(nViewWidth*nViewHeight);
		unsigned char* puBlue = (unsigned char*)malloc(nViewWidth*nViewHeight);


		DataStretch(pDataRed,puRed,nViewWidth*nViewHeight,eType,m_eStretchType,m_pfMin[0],m_pfMax[0],m_pfVar[0],m_pfMean[0],m_fStandardDeviations);
		DataStretch(pDataGreen,puGreen,nViewWidth*nViewHeight,eType,m_eStretchType,m_pfMin[1],m_pfMax[1],m_pfVar[1],m_pfMean[1],m_fStandardDeviations);
		DataStretch(pDataBlue,puBlue,nViewWidth*nViewHeight,eType,m_eStretchType,m_pfMin[2],m_pfMax[2],m_pfVar[2],m_pfMean[2],m_fStandardDeviations);


		free(pDataRed);
		free(pDataGreen);
		free(pDataBlue);

		GByte* poDataGray = new GByte[nViewWidth*nViewHeight*4];
		memset(poDataGray,0,nViewWidth*nViewHeight*4);

		int nLen = nViewWidth*nViewHeight;

		//设置透明度
		int nRealTranspancy =  (int) ( (100 - m_nTransparency)*(255/100.0) + 0.5);

		SysSRS::CCoordTransformation* pInverseTran = GetInverseCoordTran();
		if (NULL == pInverseTran )
		{
#pragma omp parallel for
			for (int i = 0; i < nLen; i++)
			{
				if (puRed[i] == 0 && puGreen[i] == 0 && puBlue[i] == 0)
				{
					unsigned char uTmpRed = ContrastStretch(255,m_nContrast);
					unsigned char uTmpGreen = ContrastStretch(255,m_nContrast);
					unsigned char uTmpBlue = ContrastStretch(255,m_nContrast);
					poDataGray[i*4] = BrightnessStretch(uTmpRed,m_fBrightness);
					poDataGray[i*4+1] = BrightnessStretch(uTmpGreen,m_fBrightness);
					poDataGray[i*4+2] = BrightnessStretch(uTmpBlue,m_fBrightness);
					//poDataGray[i*4+3] = 0;
				}
				else
				{
					unsigned char uTmpRed = ContrastStretch((puRed)[i],m_nContrast);
					unsigned char uTmpGreen = ContrastStretch((puGreen)[i],m_nContrast);
					unsigned char uTmpBlue = ContrastStretch((puBlue)[i],m_nContrast);

					poDataGray[i*4] = BrightnessStretch(uTmpRed,m_fBrightness);
					poDataGray[i*4+1] = BrightnessStretch(uTmpGreen,m_fBrightness);
					poDataGray[i*4+2] = BrightnessStretch(uTmpBlue,m_fBrightness);
					poDataGray[i*4+3] = nRealTranspancy;
				}
			}
		}

		else			//动态投影
		{
			double dx = envDraw.GetWidth()/nViewWidth;
			double dy = envDraw.GetHeight()/nViewHeight;
			double dbGeoY = winMaxy - 0.0*dy;

			double dImgdx = envLast.GetWidth()/nViewWidth;
			double dImgdy = -envLast.GetHeight()/nViewHeight;

			for (int nRow = 0; nRow < nViewHeight; nRow ++)
			{
				double dbGeoX = winMinx + 0.0*dx;
				//#pragma omp parallel for
				for (int nCol = 0; nCol < nViewWidth; nCol ++)
				{
					double dbX = dbGeoX;
					double dbY = dbGeoY;
					pInverseTran->Transform(1,&dbX,&dbY,NULL);
					double fPixel = 0;
					double fLine = 0;

					fPixel = (dbX - envLast.minX)/dImgdx;
					fLine = (dbY - envLast.maxY)/dImgdy;
					int nPixel = int(fPixel+0.5);
					int nLine = int(fLine + 0.5);

					int nOffsetDraw = nRow*nViewWidth + nCol;
					if (nLine < 0 || nLine >= nViewHeight || nPixel < 0 || nPixel >= nViewWidth)
					{
						//smartlog<<"没有执行1";
						unsigned char uTmpRed = ContrastStretch(255,m_nContrast);
						unsigned char uTmpGreen = ContrastStretch(255,m_nContrast);
						unsigned char uTmpBlue = ContrastStretch(255,m_nContrast);
						poDataGray[nOffsetDraw*4] = BrightnessStretch(uTmpRed,m_fBrightness);
						poDataGray[nOffsetDraw*4+1] = BrightnessStretch(uTmpGreen,m_fBrightness);
						poDataGray[nOffsetDraw*4+2] = BrightnessStretch(uTmpBlue,m_fBrightness);
						//poDataGray[nOffsetDraw*4+3] = 0;
					}

					else
					{
						//smartlog<<"没有执行2";
						int nOffset = nLine*nViewWidth + nPixel;
						eByte uRed = puRed[nOffset];
						eByte uGreen = puGreen[nOffset];
						eByte uBlue = puBlue[nOffset];
						unsigned char uTmpRed = ContrastStretch(uRed,m_nContrast);
						unsigned char uTmpGreen = ContrastStretch(uGreen,m_nContrast);
						unsigned char uTmpBlue = ContrastStretch(uBlue,m_nContrast);

						poDataGray[nOffsetDraw*4] = BrightnessStretch(uTmpRed,m_fBrightness);
						poDataGray[nOffsetDraw*4+1] = BrightnessStretch(uTmpGreen,m_fBrightness);
						poDataGray[nOffsetDraw*4+2] = BrightnessStretch(uTmpBlue,m_fBrightness);

						if (uRed != 0 || uGreen != 0 || uBlue != 0)
						{
							poDataGray[nOffsetDraw*4+3] = nRealTranspancy;
						}
						
						//if
					}

					dbGeoX += dx;
				}

				dbGeoY -= dy;
			}
		}



		if (m_bInvertColor)
		{
			BrightnessContrast(poDataGray,nLen*4, 4);
		}

		free(puRed);
		free(puGreen);
		free(puBlue);


		//渲染操作
		//启用矩形纹理映射
		glEnable(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);

		const GLubyte* errorStr;
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
		}

		/*GLuint m_TexId;
		glGenTextures(1, &m_TexId);*/
		glBindTexture(GL_TEXTURE_2D, m_nTexId);

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
		}

		//使用纹理渲染图像
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nViewWidth,
			nViewHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,poDataGray);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR);

		/*glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		GLint color[4] = {255,0,0,255};
		glTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,color);*/

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
		}

		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
		//GLfloat vecColor[4] = {0.9,0.5,0.4,0.3};
		//glTexEnvfv(GL_TEXTURE_ENV,GL_TEXTURE_ENV_COLOR,vecColor);
		glBindTexture(GL_TEXTURE_2D,m_nTexId);

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
		}

		
		/*ScreenToWorld(rectView.left,rectView.top,&winMinx,&winmaxy,&dbZ);
		ScreenToWorld(rectView.right,rectView.bottom,&winMaxx,&winminy,&dbZ);*/

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
		}

		/*glRasterPos2d(winMinx,winminy);
		glPixelZoom(1.0,-1.0);
		glDrawPixels(nViewWidth,nViewHeight,GL_RGBA,GL_UNSIGNED_BYTE,poDataGray);*/

		glColor3f(1,0,0);

		glBegin(GL_QUADS);
		glVertex3d(winMinx,winMiny,0);glTexCoord2d(0.0,0.0);
		glVertex3d(winMinx,winMaxy,0);glTexCoord2d(1.0,0.0);
		glVertex3d(winMaxx,winMaxy,0);glTexCoord2d(1.0,1.0);
		glVertex3d(winMaxx,winMiny,0);glTexCoord2d(0.0,1.0);

		/*glVertex3d(winMinx,winMiny,0);glTexCoord2d(dTexStartX,dTexStartY);
		glVertex3d(winMinx,winMaxy,0);glTexCoord2d(dTexEndX,dTexStartY);
		glVertex3d(winMaxx,winMaxy,0);glTexCoord2d(dTexEndX,dTexEndY);
		glVertex3d(winMaxx,winMiny,0);glTexCoord2d(dTexStartX,dTexEndY);*/
		glEnd();

		glDisable(GL_TEXTURE_2D);		//需要关闭，不然出现颜色干扰的情况

		//glFlush();

		//glColor4f(1,0,0,1);

		//glDisable(GL_BLEND);

		/*glBegin(GL_LINES);
		glVertex3f(winMinx,winminy,0);
		glVertex3f(winMaxx,winmaxy,0);
		glEnd();*/

		/*glBegin(GL_QUADS);
		glVertex3f(winMinx,winminy,0);glTexCoord2f(0.0,0.0);
		glVertex3f(winMaxx,winminy,0);glTexCoord2f(0.0,1.0);
		glVertex3f(winMaxx,winmaxy,0);glTexCoord2f(1.0,1.0);
		glVertex3f(winMinx,winmaxy,0);glTexCoord2f(1.0,0.0);

		glEnd();*/
		//smartlog<<"绘图"<<t4-t3<<"毫秒";

		delete []poDataGray;

	}

	void SysRasterLayer::DrawPalettedSingleBandColor(
		void* thePainter, 
		SysRenderContext* rendererContext, 
		const RECTD& rectImage, 
		const RECTD& rectView, const SysEnvelope& envDrawView) const
	{
		SysDataSource::IRasterDataLayer* pRasLayer = (SysDataSource::IRasterDataLayer*)m_pDatalayer;

		int nStartX = rectImage.left;
		int nStartY = rectImage.bottom;
		int nDrawWidth = abs((int)rectImage.right - (int)rectImage.left) + 1;
		int nDrawHeight = abs((int)rectImage.bottom - (int)rectImage.top) + 1;
		int nViewWidth = abs(rectView.right - rectView.left) + 1;
		int nViewHeight = abs(rectView.bottom - rectView.top) + 1;

		int nEndX = nStartX + nDrawWidth;
		int nEndY = nStartY + nDrawHeight;

		double winMinx,winMiny,winMaxx,winMaxy;
		winMinx = envDrawView.minX;
		winMiny = envDrawView.minY;
		winMaxx = envDrawView.maxX;
		winMaxy = envDrawView.maxY;

		pRasLayer->PixelToWorld_Ex(nStartX,nStartY,winMinx,winMaxy);
		pRasLayer->PixelToWorld_Ex(nEndX,nEndY,winMaxx,winMiny);

		SysEnvelope envDraw;
		envDraw.minX = winMinx;
		envDraw.minY = winMiny;
		envDraw.maxX = winMaxx;
		envDraw.maxY = winMaxy;

		SysEnvelope envLast = envDraw;

		SysSRS::CCoordTransformation* pForwardTran = GetForwardCoordTran();
		if (pForwardTran != NULL)
		{
			SysSrsMapToLayer::TransExtent(pForwardTran,envDraw);
		}
		winMinx = envDraw.minX;
		winMiny = envDraw.minY;
		winMaxx = envDraw.maxX;
		winMaxy = envDraw.maxY;

		SysDataSource::Raster_Info rasInfo = pRasLayer->GetRasterInfo();
		GDALDataType eType = rasInfo.m_vecBandType[0];

		unsigned char* pData = (unsigned char*)malloc(nViewWidth*nViewHeight*GDALGetDataTypeSize(eType)/8);

		//读取数据和构造显示的数据
		CPLErr err1 = pRasLayer->DataRead(0,0,nStartX,nStartY,nDrawWidth,nDrawHeight,
			pData,nViewWidth,nViewHeight,eType);

		//获得颜色表，准备渲染数据
		if (m_vecColorTbl.empty())
		{
			free(pData);
			pData = NULL;
			return;
		}

		GByte* poDataGray = new GByte[nViewWidth*nViewHeight*4];
		memset(poDataGray,0,nViewWidth*nViewHeight*4);

		//设置透明度
		int nRealTranspancy =  (int) ( (100 - m_nTransparency)*(255/100.0) + 0.5);

		int nLen = nViewWidth*nViewHeight;

		SysSRS::CCoordTransformation* pInverseTran = GetInverseCoordTran();
		if (NULL == pInverseTran )
		{
			#pragma omp parallel for
			for (int i = 0; i < nLen; i++)
			{
				if (m_vecColorTbl[pData[i]].c1 == 0 && m_vecColorTbl[pData[i]].c2 == 0
					&& m_vecColorTbl[pData[i]].c3 == 0)
				{
					unsigned char uTmpRed = ContrastStretch(255,m_nContrast);
					unsigned char uTmpGreen = ContrastStretch(255,m_nContrast);
					unsigned char uTmpBlue = ContrastStretch(255,m_nContrast);

					poDataGray[i*4] = BrightnessStretch(uTmpRed,m_fBrightness);
					poDataGray[i*4+1] = BrightnessStretch(uTmpGreen,m_fBrightness);
					poDataGray[i*4+2] = BrightnessStretch(uTmpBlue,m_fBrightness);
					//poDataGray[i*4+3] = 0;
				}

				else
				{
					unsigned char uTmpRed = ContrastStretch(m_vecColorTbl[pData[i]].c1,m_nContrast);
					unsigned char uTmpGreen = ContrastStretch(m_vecColorTbl[pData[i]].c2,m_nContrast);
					unsigned char uTmpBlue = ContrastStretch(m_vecColorTbl[pData[i]].c3,m_nContrast);

					poDataGray[i*4] = BrightnessStretch(uTmpRed,m_fBrightness);
					poDataGray[i*4+1] = BrightnessStretch(uTmpGreen,m_fBrightness);
					poDataGray[i*4+2] = BrightnessStretch(uTmpBlue,m_fBrightness);
					poDataGray[i*4+3] = nRealTranspancy;
				}

			}
		}

		else if (NULL != pInverseTran)
		{
			double dx = envDraw.GetWidth()/nViewWidth;
			double dy = envDraw.GetHeight()/nViewHeight;
			double dbGeoY = winMaxy - 0.0*dy;

			double dImgdx = envLast.GetWidth()/nViewWidth;
			double dImgdy = -envLast.GetHeight()/nViewHeight;

			for (int nRow = 0; nRow < nViewHeight; nRow ++)
			{
				double dbGeoX = winMinx + 0.0*dx;
				for (int nCol = 0; nCol < nViewWidth; nCol ++)
				{
					double dbX = dbGeoX;
					double dbY = dbGeoY;
					pInverseTran->Transform(1,&dbX,&dbY,NULL);
					double fPixel = 0;
					double fLine = 0;
					fPixel = (dbX - envLast.minX)/dImgdx;
					fLine = (dbY - envLast.maxY)/dImgdy;
					int nPixel = int(fPixel+0.5);
					int nLine = int(fLine + 0.5);

					int nOffsetDraw = nRow*nViewWidth + nCol;
					if (nLine < 0 || nLine >= nViewHeight || nPixel < 0 || nPixel >= nViewWidth)
					{
						unsigned char uTmpRed = ContrastStretch(255,m_nContrast);
						unsigned char uTmpGreen = ContrastStretch(255,m_nContrast);
						unsigned char uTmpBlue = ContrastStretch(255,m_nContrast);

						poDataGray[nOffsetDraw*4] = BrightnessStretch(uTmpRed,m_fBrightness);
						poDataGray[nOffsetDraw*4+1] = BrightnessStretch(uTmpGreen,m_fBrightness);
						poDataGray[nOffsetDraw*4+2] = BrightnessStretch(uTmpBlue,m_fBrightness);
						//poDataGray[nOffsetDraw*2+1] = 0;
					}

					else
					{
						int nOffset = nLine*nViewWidth + nPixel;

						unsigned char uTmpRed = ContrastStretch(m_vecColorTbl[pData[nOffset]].c1,m_nContrast);
						unsigned char uTmpGreen = ContrastStretch(m_vecColorTbl[pData[nOffset]].c2,m_nContrast);
						unsigned char uTmpBlue = ContrastStretch(m_vecColorTbl[pData[nOffset]].c3,m_nContrast);

						poDataGray[nOffsetDraw*4] = BrightnessStretch(uTmpRed,m_fBrightness);
						poDataGray[nOffsetDraw*4+1] = BrightnessStretch(uTmpGreen,m_fBrightness);
						poDataGray[nOffsetDraw*4+2] = BrightnessStretch(uTmpBlue,m_fBrightness);
						if (poDataGray[nOffsetDraw*4] != 0 || poDataGray[nOffsetDraw*4+1] != 0 || poDataGray[nOffsetDraw*4+2] != 0)
						{
							poDataGray[nOffsetDraw*4+3] = nRealTranspancy;
						}//if
					}

					dbGeoX += dx;
				}

				dbGeoY -= dy;
			}
		}

		if (m_bInvertColor)
		{
			BrightnessContrast(poDataGray,nLen*4, 4);
		}

		free(pData);

		glColor3f(1,1,1);

		//启用矩形纹理映射
		glEnable(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);

		glBindTexture(GL_TEXTURE_2D, m_nTexId);

		//使用纹理渲染图像
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nViewWidth,
			nViewHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,poDataGray);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR);

		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D,m_nTexId);

		glBegin(GL_QUADS);
		glVertex3f(winMinx,winMiny,0);glTexCoord2f(0.0,0.0);
		glVertex3f(winMinx,winMaxy,0);glTexCoord2f(1.0,0.0);
		glVertex3f(winMaxx,winMaxy,0);glTexCoord2f(1.0,1.0);
		glVertex3f(winMaxx,winMiny,0);glTexCoord2f(0.0,1.0);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		delete []poDataGray;

	}

	bool SysRasterLayer::IsInvertColor() const
	{
		return m_bInvertColor;
	}

	void SysRasterLayer::SetInvertColor( bool bInvert )
	{
		m_bInvertColor = bInvert;
	}

	inline DrawStyle SysRasterLayer::GetDrawStyle() const
	{
		return m_eDrawStyle;
	}

	void SysRasterLayer::SetDrawStyle( DrawStyle eStyle )
	{
		m_eDrawStyle = eStyle;
	}

	void SysRasterLayer::CaclBandStatusInfo( const int nBandIndex ) const
	{
		SysRasterBandStats* pBandStatus = m_vecBandStatus[nBandIndex];
		if (NULL == pBandStatus)
		{
			pBandStatus = new(std::nothrow) SysRasterBandStats;
			m_vecBandStatus[nBandIndex] = pBandStatus;
		}

		SysDataSource::IRasterDataLayer* pRasLayer = (SysDataSource::IRasterDataLayer*)m_pDatalayer;
		float fMax,fMin,fMean,fVar;
		float fHist[256];
		pRasLayer->GetDisplayStatisInfo(nBandIndex,fHist,fMax,fMin,fMean,fVar);

		pBandStatus->m_dbMaximumValue = fMax;
		pBandStatus->m_dbMinimumValue = fMin;
		pBandStatus->m_dbMean = fMean;
		pBandStatus->m_dbRange = fMax - fMin;
		pBandStatus->m_dbStdDev = (fVar);
		pBandStatus->m_nBandNumber = nBandIndex;
		pBandStatus->m_nElementCount = m_nWidth * m_nHeight;

		for (int i = 0; i < 256; i ++)
		{
			pBandStatus->m_pHistogramArray[i] = fHist[i];
		}
		//pBandStatus->m_pHistogramArray = 
	}

	inline const SysRasterBandStats* SysRasterLayer::GetBandStatusInfo( const int nBandIndex ) const
	{
		return m_vecBandStatus[nBandIndex];
	}

	//eByte SysRasterLayer::GetDisplayValue( int nBandIndex, double dbValue) const
	//{
	//	eByte bValue = 0;

	//	if (SingleBandGray == m_eDrawStyle || MultiBandColor == m_eDrawStyle)
	//	{
	//		DataTranslate(&dbValue,&bValue,1,GDT_Float64,fVar[nBandIndex],fMean[nBandIndex]);

	//		if (bValue == 0)
	//		{
	//			unsigned char uTmpRed = ContrastStretch(255,m_nContrast);
	//			bValue = BrightnessStretch(uTmpRed,m_fBrightness);
	//		}
	//		else
	//		{
	//			unsigned char uTmpRed = ContrastStretch(bValue,m_nContrast);

	//			bValue = BrightnessStretch(uTmpRed,m_fBrightness);
	//		}

	//		if (m_bInvertColor)
	//		{
	//			BrightnessContrast(&bValue,1, 0);
	//		}
	//	}

	//	else if (PalettedColor == m_eDrawStyle)
	//	{
	//		if (m_vecColorTbl[dbValue].c1 == 0 && m_vecColorTbl[dbValue].c2 == 0
	//			&& m_vecColorTbl[dbValue].c3 == 0)
	//		{
	//			unsigned char uTmpRed = ContrastStretch(255,m_nContrast);

	//			bValue = BrightnessStretch(uTmpRed,m_fBrightness);
	//		}

	//		else
	//		{
	//			unsigned char uTmpRed = ContrastStretch(m_vecColorTbl[dbValue].c1,m_nContrast);
	//			unsigned char uTmpGreen = ContrastStretch(m_vecColorTbl[dbValue].c2,m_nContrast);
	//			unsigned char uTmpBlue = ContrastStretch(m_vecColorTbl[dbValue].c3,m_nContrast);

	//			/*poDataGray[i*4] = BrightnessStretch(uTmpRed,m_fBrightness);
	//			poDataGray[i*4+1] = BrightnessStretch(uTmpGreen,m_fBrightness);
	//			poDataGray[i*4+2] = BrightnessStretch(uTmpBlue,m_fBrightness);
	//			poDataGray[i*4+3] = nRealTranspancy;*/
	//		}

	//		if (m_bInvertColor)
	//		{
	//			//BrightnessContrast(poDataGray,nLen*4, 4);
	//		}
	//	}


	//	return bValue;
	//}

	void SysRasterLayer::GetDisplayValue( double dbValue,eByte *pRGBAValues ) const
	{
		eByte bValue = 0;

		if (SingleBandGray == m_eDrawStyle)
		{
			//DataTranslate(&dbValue,&bValue,1,GDT_Float64,m_pfVar[0],m_pfMean[0]);
			DataStretch(&dbValue,&bValue,1,GDT_Float64,m_eStretchType,m_pfMin[0],m_pfMax[0],m_pfVar[0],m_pfMean[0],m_fStandardDeviations);

			if (bValue == 0)
			{
				unsigned char uTmpRed = ContrastStretch(255,m_nContrast);
				bValue = BrightnessStretch(uTmpRed,m_fBrightness);
			}
			else
			{
				unsigned char uTmpRed = ContrastStretch(bValue,m_nContrast);
				bValue = BrightnessStretch(uTmpRed,m_fBrightness);
			}

			if (m_bInvertColor)
			{
				BrightnessContrast(&bValue,1, 0);
			}

			pRGBAValues[0] = bValue;
			pRGBAValues[1] = bValue;
			pRGBAValues[2] = bValue;
			pRGBAValues[3] = GetTransparency();
		}

		else if (PalettedColor == m_eDrawStyle)
		{
			if (m_vecColorTbl[dbValue].c1 == 0 && m_vecColorTbl[dbValue].c2 == 0
				&& m_vecColorTbl[dbValue].c3 == 0)
			{
				unsigned char uTmpRed = ContrastStretch(255,m_nContrast);

				bValue = BrightnessStretch(uTmpRed,m_fBrightness);
			}

			else
			{
				unsigned char uTmpRed = ContrastStretch(m_vecColorTbl[dbValue].c1,m_nContrast);
				unsigned char uTmpGreen = ContrastStretch(m_vecColorTbl[dbValue].c2,m_nContrast);
				unsigned char uTmpBlue = ContrastStretch(m_vecColorTbl[dbValue].c3,m_nContrast);

				bValue = BrightnessStretch(uTmpRed,m_fBrightness);
				pRGBAValues[0] = bValue;
				bValue = BrightnessStretch(uTmpGreen,m_fBrightness);
				pRGBAValues[1] = bValue;
				bValue = BrightnessStretch(uTmpBlue,m_fBrightness);
				pRGBAValues[2] = bValue;
			}

			if (m_bInvertColor)
			{
				BrightnessContrast(pRGBAValues,3, 0);
			}

			pRGBAValues[3] = GetTransparency();
		}

		else if (SingleBandPseudoColor == m_eDrawStyle)
		{
			int nIndex = GetPseudoColorIndex(&dbValue,m_vecValueInters,GDT_Float64);

			unsigned char uRed = 0;
			unsigned char uGreen = 0;
			unsigned char uBlue = 0;

			//_aligned_malloc(10,16);

			if (nIndex >= 0)
			{
				uRed = m_vecPseudoColor[nIndex].c1;
				uGreen = m_vecPseudoColor[nIndex].c2;
				uBlue = m_vecPseudoColor[nIndex].c3;
			}

			if (uRed == 0 && uGreen == 0 && uBlue == 0)
			{
				unsigned char uTmpRed = ContrastStretch(255,m_nContrast);
				unsigned char uTmpGreen = ContrastStretch(255,m_nContrast);
				unsigned char uTmpBlue = ContrastStretch(255,m_nContrast);

				bValue = BrightnessStretch(uTmpRed,m_fBrightness);
				pRGBAValues[0] = bValue;
				bValue = BrightnessStretch(uTmpGreen,m_fBrightness);
				pRGBAValues[1] = bValue;
				bValue = BrightnessStretch(uTmpBlue,m_fBrightness);
				pRGBAValues[2] = bValue;
				pRGBAValues[3] = 0;
			}

			else
			{
				unsigned char uTmpRed = ContrastStretch(uRed,m_nContrast);
				unsigned char uTmpGreen = ContrastStretch(uGreen,m_nContrast);
				unsigned char uTmpBlue = ContrastStretch(uBlue,m_nContrast);

				bValue = BrightnessStretch(uTmpRed,m_fBrightness);
				pRGBAValues[0] = bValue;
				bValue = BrightnessStretch(uTmpGreen,m_fBrightness);
				pRGBAValues[1] = bValue;
				bValue = BrightnessStretch(uTmpBlue,m_fBrightness);
				pRGBAValues[2] = bValue;
				pRGBAValues[3] = GetTransparency();
			}

			if (m_bInvertColor)
			{
				BrightnessContrast(pRGBAValues,3, 0);
			}
		}
	}

	void SysRasterLayer::GetDisplayValue( std::vector<double> dbValue,eByte *pRGBAValues ) const
	{
		eByte bValue = 0;

		if ( MultiBandColor == m_eDrawStyle)
		{
			for (int i = 0; i < 3 ;i ++)
			{
				//DataTranslate(&(dbValue[i]),&bValue,1,GDT_Float64,m_pfVar[i],m_pfMean[i]);
				DataStretch(&(dbValue[i]),&bValue,1,GDT_Float64,m_eStretchType,m_pfMin[i],m_pfMax[i],m_pfVar[i],m_pfMean[i],m_fStandardDeviations);

				if (bValue == 0)
				{
					unsigned char uTmpRed = ContrastStretch(255,m_nContrast);
					bValue = BrightnessStretch(uTmpRed,m_fBrightness);
				}
				else
				{
					unsigned char uTmpRed = ContrastStretch(bValue,m_nContrast);

					bValue = BrightnessStretch(uTmpRed,m_fBrightness);
				}

				if (m_bInvertColor)
				{
					BrightnessContrast(&bValue,1, 0);
				}

				pRGBAValues[i] = bValue;
			}
			
		}

		pRGBAValues[3] = GetTransparency();
	}

	bool SysRasterLayer::HaveRPCModel() const
	{
		std::string strFileName = GetLayerName();
		size_t nPos = strFileName.rfind(_T('.'));
		if (nPos != -1)
		{
			std::string strRpcFileName = strFileName.substr(0, nPos + 1);

			std::string strTmp1 = strRpcFileName + _T("rpb");
			bool bFlag1 = SysUtility::IsFileExist(strTmp1.c_str());

			std::string strTmp2 = strRpcFileName + _T("RPB");
			bool bFlag2 = SysUtility::IsFileExist(strTmp2.c_str());

			std::string strTmp3 = strRpcFileName + _T("rpc");
			bool bFlag3 = SysUtility::IsFileExist(strTmp3.c_str());

			std::string strTmp4 = strRpcFileName + _T("RPC");
			bool bFlag4 = SysUtility::IsFileExist(strTmp4.c_str());

			if (!bFlag1 && !bFlag2 && !bFlag3 && !bFlag4)
			{
				return false;
			}
		}
		return true;
	}

	void SysRasterLayer::SetPseudoColorArray( std::vector<SysColorItem>& vecPseudoColor )
	{
		m_vecPseudoColor.clear();
		size_t nSize = vecPseudoColor.size();
		m_vecPseudoColor.resize(nSize);
		std::copy(vecPseudoColor.begin(),vecPseudoColor.end(),m_vecPseudoColor.begin());
	}

	void SysRasterLayer::SetPseudoColorValues( std::vector<ValueInter>& vecValueInters )
	{
		m_vecValueInters.clear();
		int nSize = vecValueInters.size();
		m_vecValueInters.resize(nSize);
		std::copy(vecValueInters.begin(),vecValueInters.end(),m_vecValueInters.begin());
	}

	void SysRasterLayer::DrawSingleBandPseudoColor( 
							void* thePainter, 
							SysRenderContext* rendererContext, 
							const RECTD& rectImage, 
							const RECTD& rectView, 
							const SysEnvelope& envDrawView ) const
	{
		//获得伪彩色颜色表，准备渲染数据
		if (m_vecPseudoColor.empty())
		{
			return;
		}

		SysDataSource::IRasterDataLayer* pRasLayer = (SysDataSource::IRasterDataLayer*)m_pDatalayer;

		int nStartX = rectImage.left;
		int nStartY = rectImage.bottom;
		int nDrawWidth = abs((int)rectImage.right - (int)rectImage.left) + 1;
		int nDrawHeight = abs((int)rectImage.bottom - (int)rectImage.top) + 1;
		int nViewWidth = abs(rectView.right - rectView.left) + 1;
		int nViewHeight = abs(rectView.bottom - rectView.top) + 1;

		int nEndX = nStartX + nDrawWidth;
		int nEndY = nStartY + nDrawHeight;

		double winMinx,winMiny,winMaxx,winMaxy;
		winMinx = envDrawView.minX;
		winMiny = envDrawView.minY;
		winMaxx = envDrawView.maxX;
		winMaxy = envDrawView.maxY;

		pRasLayer->PixelToWorld_Ex(nStartX,nStartY,winMinx,winMaxy);
		pRasLayer->PixelToWorld_Ex(nEndX,nEndY,winMaxx,winMiny);

		SysEnvelope envDraw;
		envDraw.minX = winMinx;
		envDraw.minY = winMiny;
		envDraw.maxX = winMaxx;
		envDraw.maxY = winMaxy;

		SysEnvelope envLast = envDraw;

		SysSRS::CCoordTransformation* pForwardTran = GetForwardCoordTran();
		if (pForwardTran != NULL)
		{
			SysSrsMapToLayer::TransExtent(pForwardTran,envDraw);
		}
		winMinx = envDraw.minX;
		winMiny = envDraw.minY;
		winMaxx = envDraw.maxX;
		winMaxy = envDraw.maxY;

		SysDataSource::Raster_Info rasInfo = pRasLayer->GetRasterInfo();
		GDALDataType eType = rasInfo.m_vecBandType[m_nGrayIndex];

		int nDataSize = GDALGetDataTypeSize(eType)/8;

		unsigned char* pData = (unsigned char*)malloc(nViewWidth*nViewHeight*nDataSize);

		//读取数据和构造显示的数据
		CPLErr err1 = pRasLayer->DataRead(m_nGrayIndex,0,nStartX,nStartY,nDrawWidth,nDrawHeight,
			pData,nViewWidth,nViewHeight,eType);

		GByte* poDataGray = new GByte[nViewWidth*nViewHeight*4];
		memset(poDataGray,0,nViewWidth*nViewHeight*4);

		//设置透明度
		int nRealTranspancy =  (int) ( (100 - m_nTransparency)*(255/100.0) + 0.5);

		int nLen = nViewWidth*nViewHeight;

		SysSRS::CCoordTransformation* pInverseTran = GetInverseCoordTran();
		if (NULL == pInverseTran )
		{
			#pragma omp parallel for
			for (int i = 0; i < nLen; i++)
			{
				//根据当前的值获得颜色表中显示的数据
				int nIndex = GetPseudoColorIndex(pData + i * nDataSize,m_vecValueInters,eType);

				unsigned char uRed = 0;
				unsigned char uGreen = 0;
				unsigned char uBlue = 0;

				if (nIndex >= 0)
				{
					uRed = m_vecPseudoColor[nIndex].c1;
					uGreen = m_vecPseudoColor[nIndex].c2;
					uBlue = m_vecPseudoColor[nIndex].c3;
				}

				if (uRed == 0 && uGreen == 0 && uBlue == 0)
				{
					unsigned char uTmpRed = ContrastStretch(255,m_nContrast);
					unsigned char uTmpGreen = ContrastStretch(255,m_nContrast);
					unsigned char uTmpBlue = ContrastStretch(255,m_nContrast);

					poDataGray[i*4] = BrightnessStretch(uTmpRed,m_fBrightness);
					poDataGray[i*4+1] = BrightnessStretch(uTmpGreen,m_fBrightness);
					poDataGray[i*4+2] = BrightnessStretch(uTmpBlue,m_fBrightness);
					//poDataGray[i*4+3] = 0;
				}

				else
				{
					unsigned char uTmpRed = ContrastStretch(uRed,m_nContrast);
					unsigned char uTmpGreen = ContrastStretch(uGreen,m_nContrast);
					unsigned char uTmpBlue = ContrastStretch(uBlue,m_nContrast);

					poDataGray[i*4] = BrightnessStretch(uTmpRed,m_fBrightness);
					poDataGray[i*4+1] = BrightnessStretch(uTmpGreen,m_fBrightness);
					poDataGray[i*4+2] = BrightnessStretch(uTmpBlue,m_fBrightness);
					poDataGray[i*4+3] = nRealTranspancy;
				}

			}
		}

		else if (NULL != pInverseTran)
		{
			double dx = envDraw.GetWidth()/nViewWidth;
			double dy = envDraw.GetHeight()/nViewHeight;
			double dbGeoY = winMaxy - 0.0*dy;

			double dImgdx = envLast.GetWidth()/nViewWidth;
			double dImgdy = -envLast.GetHeight()/nViewHeight;

			for (int nRow = 0; nRow < nViewHeight; nRow ++)
			{
				double dbGeoX = winMinx + 0.0*dx;
				for (int nCol = 0; nCol < nViewWidth; nCol ++)
				{
					double dbX = dbGeoX;
					double dbY = dbGeoY;
					pInverseTran->Transform(1,&dbX,&dbY,NULL);
					double fPixel = 0;
					double fLine = 0;
					fPixel = (dbX - envLast.minX)/dImgdx;
					fLine = (dbY - envLast.maxY)/dImgdy;
					int nPixel = int(fPixel+0.5);
					int nLine = int(fLine + 0.5);

					int nOffsetDraw = nRow*nViewWidth + nCol;
					if (nLine < 0 || nLine >= nViewHeight || nPixel < 0 || nPixel >= nViewWidth)
					{
						unsigned char uTmpRed = ContrastStretch(255,m_nContrast);
						unsigned char uTmpGreen = ContrastStretch(255,m_nContrast);
						unsigned char uTmpBlue = ContrastStretch(255,m_nContrast);

						poDataGray[nOffsetDraw*4] = BrightnessStretch(uTmpRed,m_fBrightness);
						poDataGray[nOffsetDraw*4+1] = BrightnessStretch(uTmpGreen,m_fBrightness);
						poDataGray[nOffsetDraw*4+2] = BrightnessStretch(uTmpBlue,m_fBrightness);
					}

					else
					{
						int nOffset = nLine*nViewWidth + nPixel;
						int nIndex = GetPseudoColorIndex(pData + nOffset * nDataSize,m_vecValueInters,eType);

						unsigned char uRed = 0;
						unsigned char uGreen = 0;
						unsigned char uBlue = 0;

						if (nIndex >= 0)
						{
							uRed = m_vecPseudoColor[nIndex].c1;
							uGreen = m_vecPseudoColor[nIndex].c2;
							uBlue = m_vecPseudoColor[nIndex].c3;
						}

						unsigned char uTmpRed = ContrastStretch(uRed,m_nContrast);
						unsigned char uTmpGreen = ContrastStretch(uGreen,m_nContrast);
						unsigned char uTmpBlue = ContrastStretch(uBlue,m_nContrast);

						poDataGray[nOffsetDraw*4] = BrightnessStretch(uTmpRed,m_fBrightness);
						poDataGray[nOffsetDraw*4+1] = BrightnessStretch(uTmpGreen,m_fBrightness);
						poDataGray[nOffsetDraw*4+2] = BrightnessStretch(uTmpBlue,m_fBrightness);
						if (poDataGray[nOffsetDraw*4] != 0 || poDataGray[nOffsetDraw*4+1] != 0 || poDataGray[nOffsetDraw*4+2] != 0)
						{
							poDataGray[nOffsetDraw*4+3] = nRealTranspancy;
						}//if
					}

					dbGeoX += dx;
				}

				dbGeoY -= dy;
			}
		}

		if (m_bInvertColor)
		{
			BrightnessContrast(poDataGray,nLen*4, 4);
		}

		free(pData);

		glColor3f(1,1,1);

		//启用矩形纹理映射
		glEnable(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);

		glBindTexture(GL_TEXTURE_2D, m_nTexId);

		//使用纹理渲染图像
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nViewWidth,
			nViewHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,poDataGray);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR);

		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D,m_nTexId);

		glBegin(GL_QUADS);
		glVertex3f(winMinx,winMiny,0);glTexCoord2f(0.0,0.0);
		glVertex3f(winMinx,winMaxy,0);glTexCoord2f(1.0,0.0);
		glVertex3f(winMaxx,winMaxy,0);glTexCoord2f(1.0,1.0);
		glVertex3f(winMaxx,winMiny,0);glTexCoord2f(0.0,1.0);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		delete []poDataGray;
	}

	StretchType SysRasterLayer::GetStretchType() const
	{
		return m_eStretchType;
	}

	void SysRasterLayer::SetStretchType( StretchType eType )
	{
		m_eStretchType = eType;
		CalcStatisInfo(m_eDrawStyle);
	}

	void SysRasterLayer::CalcDrawStatisInfo( DrawStyle eDrawStyle,const int nBandIndex )
	{
		SysDataSource::IRasterDataLayer* pRasLayer = (SysDataSource::IRasterDataLayer*)m_pDatalayer;

		if (eDrawStyle == MultiBandColor)
		{
			//首先判断nBandIndex和RGB哪个波段
			int nIndex = 0;
			if (m_nRedIndex == nBandIndex)
			{
				nIndex = 0;
			}
			else if (m_nGreenIndex == nBandIndex)
			{
				nIndex = 1;
			}
			else if (m_nBlueIndex == nBandIndex)
			{
				nIndex = 2;
			}
			pRasLayer->SetBandNullValue(nBandIndex,0);
			//pRasLayer->GetDisplayStatisInfo(nIndex,m_pfHistR,m_pfMax[nIndex],m_pfMin[nIndex],m_pfMean[nIndex],m_pfVar[nIndex]);

			int nCount = 0;
			GetDrawStatisInfo(nBandIndex,m_pHist+nIndex*256,m_pfMax[nIndex],m_pfMin[nIndex],m_pfMean[nIndex],m_pfVar[nIndex],nCount);

			if (m_eStretchType == StretchOptimizedLinear)
			{
				CreateOptimizedStretch(m_pHist+nIndex*256,m_pfMin[nIndex],m_pfMax[nIndex],nCount);
			}

			else if (m_eStretchType == StretchPercentOne)
			{
				CreateLinearStretch(m_pHist+nIndex*256,m_pfMin[nIndex],m_pfMax[nIndex],nCount,1);
			}

			else if (m_eStretchType == StretchPercentTwo)
			{
				CreateLinearStretch(m_pHist+nIndex*256,m_pfMin[nIndex],m_pfMax[nIndex],nCount,2);
			}

			else if (m_eStretchType == StretchPercentFive)
			{
				CreateLinearStretch(m_pHist+nIndex*256,m_pfMin[nIndex],m_pfMax[nIndex],nCount,5);
			}


		}

		else if (eDrawStyle == SingleBandGray)
		{
			pRasLayer->SetBandNullValue(nBandIndex,0);
			//pRasLayer->GetDisplayStatisInfo(nBandIndex,m_pfHistR,m_pfMax[0],m_pfMin[0],m_pfMean[0],m_pfVar[0]);

			int nCount = 0;
			GetDrawStatisInfo(nBandIndex,m_pHist,m_pfMax[0],m_pfMin[0],m_pfMean[0],m_pfVar[0],nCount);

			if (m_eStretchType == StretchOptimizedLinear)
			{
				CreateOptimizedStretch(m_pHist,m_pfMin[0],m_pfMax[0],nCount);
			}

			else if (m_eStretchType == StretchPercentOne)
			{
				CreateLinearStretch(m_pHist,m_pfMin[0],m_pfMax[0],nCount,1);
			}

			else if (m_eStretchType == StretchPercentTwo)
			{
				CreateLinearStretch(m_pHist,m_pfMin[0],m_pfMax[0],nCount,2);
			}

			else if (m_eStretchType == StretchPercentFive)
			{
				CreateLinearStretch(m_pHist,m_pfMin[0],m_pfMax[0],nCount,5);
			}
		}
	}

	bool SysRasterLayer::GetDrawStatisInfo( int nBandIndex,float* pfHist,float& fMax,float& fMin,float& fMean,float& fVar,int &nCount )
	{
		int nBufWidth = 256;

		SysDataSource::IRasterDataLayer* pLayer = (SysDataSource::IRasterDataLayer*)m_pDatalayer;
		const SysDataSource::Raster_Info& info = pLayer->GetRasterInfo();
		int nWidth = info.m_width;
		int nHeight = info.m_height;

		int nBufHeight = float(nHeight)/float(nWidth)*nBufWidth;

		float* pBuf = new float[nBufHeight*nBufWidth];
		pLayer->DataReadBand(nBandIndex,0,0,0,nWidth,nHeight,nBufWidth,nBufHeight,pBuf,GDT_Float32);

		double dbNullValue = pLayer->GetBandNullValue(nBandIndex);

		memset(pfHist,0,sizeof(float)*256);

		fMin = DBL_MAX;
		fMax = -DBL_MAX;
		double dbSum = 0.0;
		nCount = 0;
		int nLen = nBufHeight*nBufWidth;
		for (int i = 0; i < nLen; i ++)
		{
			if (fabs(pBuf[i] - dbNullValue) < 0.000001)
			{
				continue;
			}

			if (pBuf[i] < fMin)
			{
				fMin = pBuf[i];
			}

			if (pBuf[i] > fMax)
			{
				fMax = pBuf[i];
			}

			dbSum += pBuf[i];

			nCount++;
		}

		double dbInterval = (fMax - fMin)/256;

		fMean = dbSum / nCount;

		fVar = 0.0;
		for (int i = 0; i < nLen; i ++)
		{
			if (fabs(pBuf[i] - dbNullValue) < 0.000001)
			{
				continue;
			}
			fVar += (pBuf[i] - fMean) * (pBuf[i] - fMean)/nCount;

			int nIndex = (int)((pBuf[i] - fMin)/dbInterval);
			pfHist[nIndex]++;
		}

		fVar = sqrt(fVar);

		delete []pBuf;

		return true;
	}

	std::vector<SysRasDataType> SysRasterLayer::GetBandDataType() const
	{
		std::vector<SysRasDataType> vecDataTypes;
		vecDataTypes.clear();
		SysDataSource::IRasterDataLayer* pDatalayer = (SysDataSource::IRasterDataLayer*)m_pDatalayer;
		if (NULL == pDatalayer)
		{
			return vecDataTypes;
		}

		SysDataSource::Raster_Info rasInfo = pDatalayer->GetRasterInfo();

		//数据类型
		
		for (int i = 0; i < m_nBandCount; i ++)
		{
			SysRasDataType eDataType = (SysRasDataType)rasInfo.m_vecBandType[i];
			vecDataTypes.push_back(eDataType);
		}
		return vecDataTypes;
	}
}
