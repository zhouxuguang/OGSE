#include "SysMapLayer.h"
#include "SysRasterLayer.h"
#include "SysFeatureLayer.h"


namespace SysDraw
{

	SysMapLayer::SysMapLayer(SysMapLayer::LayerType type, std::string lyrname)
		:m_LayerType(type),
		m_strLayerName(lyrname),
		m_bVisible(true),
		m_pCRS(NULL),
		m_pForwardTran(NULL),
		m_pInverseTran(NULL),
		m_fMaxScale(FLT_MAX),
		m_fMinScale(-FLT_MAX)
	{
		m_LayerExtent.SetNull();
	}

	SysMapLayer::~SysMapLayer()
	{
		if (m_pCRS != NULL)
		{
			SysSRS::CSpatialReference::DestroySpatialReference(m_pCRS);
			m_pCRS = NULL;
		}

		if (m_pForwardTran != NULL)
		{
			delete m_pForwardTran;
			m_pForwardTran = NULL;
		}

		if (m_pInverseTran != NULL)
		{
			delete m_pInverseTran;
			m_pInverseTran = NULL;
		}

		m_strLayerName = ("");
		m_bVisible = false,
		m_fMaxScale = (FLT_MAX);
		m_fMinScale = (-FLT_MAX);
	}

	SysSRS::CSpatialReference* SysMapLayer::GetSpatialRef() const
	{
		return m_pCRS;
	}

	void SysMapLayer::SetSpatialRef(const std::string& strWkt)
	{
		if (NULL == m_pCRS)
		{
			m_pCRS = new SysSRS::CSpatialReference;
		}

		if (!m_pCRS->ImportFromWkt(strWkt))
		{
			delete m_pCRS;
			m_pCRS = NULL;
		}
		
	}

	SysMapLayer::LayerType SysMapLayer::GetLayerType() const
	{
		return m_LayerType;
	}

	void SysMapLayer::SetLayerName( const std::string & name )
	{
		m_strLayerName = name;
	}

	std::string const & SysMapLayer::GetLayerName() const
	{
		return m_strLayerName;
	}

	std::string SysMapLayer::GetFileName() const
	{
		TCHAR szFileName[255] = {0};
		TCHAR szExtName[255] = {0};
		TCHAR szDirName[255] = {0};
		TCHAR szDriverName[255] = {0};
		_tsplitpath(m_strLayerName.c_str(),szDriverName,szDirName,szFileName,szExtName);

		std::string strFileName = szFileName;
		strFileName += szExtName;
		return strFileName;
	}

	const SysEnvelope& SysMapLayer::GetLayerExtent() const
	{
		return m_LayerExtent;
	}

	SysEnvelope& SysMapLayer::GetLayerExtent()
	{
		return m_LayerExtent;
	}

	void SysMapLayer::SetLayerExtent(const SysEnvelope& layerExtent)
	{
		m_LayerExtent = layerExtent;
	}

	float SysMapLayer::GetMaxScale() const
	{
		return m_fMaxScale;
	}

	void SysMapLayer::SetMaxScale(float fMaxScale)
	{
		m_fMaxScale = fMaxScale;
	}

	float SysMapLayer::GetMinScale() const
	{
		return m_fMinScale;
	}

	void SysMapLayer::SetMinScale(float fMinScale)
	{
		m_fMinScale = fMinScale;
	}

	bool SysMapLayer::IsVisible() const
	{
		return m_bVisible;
	}

	void SysMapLayer::SetVisible(bool bVisible)
	{
		m_bVisible = bVisible;
	}

	SysSRS::CCoordTransformation* SysMapLayer::GetForwardCoordTran() const
	{
		return m_pForwardTran;
	}

	/*void SysMapLayer::SetForwardCoordTran(SysSRS::CCoordTransformation *pCoordTran)
	{
		if (NULL == pCoordTran)
		{
			if (m_pForwardTran != NULL)
			{
				delete m_pForwardTran;
				m_pForwardTran = NULL;
			}
		}

		else
		{
			if (NULL == m_pForwardTran)
			{
				m_pForwardTran = new SysSRS::CCoordTransformation;
			}

			memcpy(m_pForwardTran,pCoordTran,sizeof(SysSRS::CCoordTransformation));
		}
	}*/

	void SysMapLayer::SetForwardCoordTran( SysSRS::CSpatialReference* pSrcSrs,SysSRS::CSpatialReference* pDstSrs )
	{
		if (m_pForwardTran != NULL)
		{
			delete m_pForwardTran;
			m_pForwardTran = NULL;
		}

		m_pForwardTran = new SysSRS::CCoordTransformation;
		m_pForwardTran->InitCoordTransform(pSrcSrs,pDstSrs);
	}

	SysSRS::CCoordTransformation* SysMapLayer::GetInverseCoordTran() const
	{
		return m_pInverseTran;
	}

	/*void SysMapLayer::SetInverseCoordTran(SysSRS::CCoordTransformation *pCoordTran)
	{
		if (NULL == pCoordTran)
		{
			if (m_pInverseTran != NULL)
			{
				delete m_pInverseTran;
				m_pInverseTran = NULL;
			}
		}

		else
		{
			if (NULL == m_pInverseTran)
			{
				m_pInverseTran = new SysSRS::CCoordTransformation;
			}

			memcpy(m_pInverseTran,pCoordTran,sizeof(SysSRS::CCoordTransformation));
		}
	}*/

	void SysMapLayer::SetInverseCoordTran( SysSRS::CSpatialReference* pSrcSrs,SysSRS::CSpatialReference* pDstSrs )
	{
		if (m_pInverseTran != NULL)
		{
			delete m_pInverseTran;
			m_pInverseTran = NULL;
		}

		m_pInverseTran = new SysSRS::CCoordTransformation; 
		m_pInverseTran->InitCoordTransform(pDstSrs,pSrcSrs);
	}


	
	//下面函数打开数据，先临时放在这边，还需要做适配
	SysRasterLayer* OpenRaster(std::string strFileName, DrawProgressFunc ProgressFunc, void* pProgressArg)
	{
		//根据数据驱动管理类获取数据读取引擎
		SysDataSource::CDataDriverFactory* pFactory = SysDataSource::CDataDriverFactory::Instance();
		pFactory->GetAllDriverInfo();
		SysDataSource::stOpenFileWay open_way;
		open_way.m_PyramidBuildMode = SysDataSource::BuildNoTip;
		open_way.m_fSample = pFactory->GetPyramidScale();
		open_way.m_SampleType = pFactory->GetSampleScale();
		open_way.m_AccessType = GA_ReadOnly;

		open_way.m_ProgressFunc = NULL;
		SysDataSource::IDataLayerPtr ptrDataLayer = pFactory->CreateDataLayer(strFileName,open_way);
		//SysDataSource::IDataLayerPtr ptrTmpDataLayer = ptrDataLayer;

		return SysRasterLayer::CreateRasterLayer(ptrDataLayer.get(),ProgressFunc,pProgressArg);
		
	}

	SysFeatureLayer* OpenFeature(std::string strFileName)
	{
		// 根据数据驱动管理类获取数据读取引擎
		SysDataSource::CDataDriverFactory* pFactory = SysDataSource::CDataDriverFactory::Instance();
		pFactory->GetAllDriverInfo();
		SysDataSource::stOpenFileWay open_way;
		SysDataSource::IFeatureDataLayerPtr ptrFeatureDataLayer = pFactory->CreateDataLayer(strFileName,open_way);

		return SysFeatureLayer::CreateFeatureLayer(ptrFeatureDataLayer.get());
	}

}
