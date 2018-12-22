/**
* @file              SysMapLayer.h
* @brief             地图图层类
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年6月25日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSMAPLAYER_H_C355D47F_C869_4D3B_B313_4C9947B6D4BC__
#define __SYSMAPLAYER_H_C355D47F_C869_4D3B_B313_4C9947B6D4BC__

#include "SysEnvelope.h"

//前置声明

namespace SysSRS
{
	class CSpatialReference;
	class CCoordTransformation;
}

namespace SysDraw
{

	class SysRenderContext;

	class SYSDRAW_API SysMapLayer 
	{

	public:
		enum LayerType
		{
			VectorLayer,		//矢量图层
			RasterLayer,		//栅格图层
			WMTSLayer,			//瓦片图层
			GraphicsLayer		//标绘图层
		};

		friend class SysMap;

		SysMapLayer( SysMapLayer::LayerType type, std::string strLyrName);

		virtual ~SysMapLayer();

		/**
		* @brief 获得图层的类型
		* @return SysMapLayer::LayerType
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		SysMapLayer::LayerType GetLayerType() const;

		/**
		* @brief 设置图层的名字
		* @param[in] const std::string & strName
		* @return void
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		void SetLayerName( const std::string & strName );

		/**
		* @brief & 获得图层的名字
		* @return std::stringconst 
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		std::string const & GetLayerName() const;

		/**
		* @brief 获得文件名（带文件后缀）
		* @return std::string
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		std::string GetFileName() const;

		/**
		* @brief 绘制，全部范围
		* @param[in] SysRenderContext* rendererContext
		* @return virtual bool 
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		virtual bool Draw( SysRenderContext* rendererContext ) = 0;

		/**
		* @brief 绘制，部分范围
		* @param[in]  SysRenderContext* rendererContext
		* @param[in] const SysEnvelope& envView
		* @return virtual bool 
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		virtual bool Draw( SysRenderContext* rendererContext ,const SysEnvelope& envView){return false;}

		/**
		* @brief 绘制标注，全部范围
		* @param[in] SysRenderContext* rendererContext
		* @return virtualvoid 
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		virtual void DrawLabels( SysRenderContext* rendererContext ){}

		/**
		* @brief 绘制标注，部分范围
		* @param[in]  SysRenderContext* rendererContext
		* @param[in] const SysEnvelope& envView
		* @return virtualvoid 
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		virtual void DrawLabels( SysRenderContext* rendererContext ,const SysEnvelope& envView ){}

		/**
		* @brief 获得图层的范围
		* @return constSysEnvelope& 
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		const SysEnvelope& GetLayerExtent() const;

		/**
		* @brief 获得图层的范围
		* @return SysEnvelope&
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		SysEnvelope& GetLayerExtent();

		/**
		* @brief 设置图层的范围
		* @param[in] const SysEnvelope& layerExtent
		* @return void
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		void SetLayerExtent(const SysEnvelope& layerExtent);


		/**
		* @brief 是否有效
		* @return bool
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		bool IsValid() const;

		/**
		* @brief 是否可以编辑
		* @return virtualbool 
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		virtual bool IsEditable() const {return 0;}; 

		/**
		* @brief 是否可见
		* @return bool
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		bool IsVisible() const;

		/**
		* @brief 设置是否可见
		* @param[in] bool bVisible
		* @return void
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		void SetVisible(bool bVisible);

		/**
		* @brief 获得最大比例尺分母
		* @return float
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		float GetMaxScale() const;

		/**
		* @brief 设置最大比例尺分母
		* @param[in] float fMaxScale
		* @return void
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		void SetMaxScale(float fMaxScale);

		/**
		* @brief 获得最小比例尺分母
		* @return float
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		float GetMinScale() const;

		/**
		* @brief 设置最小比例尺分母
		* @param[in] float fMinScale
		* @return void
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		void SetMinScale(float fMinScale);

		/**
		* @brief 获得空间参考系统
		* @return SysSRS::CSpatialReference*
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		SysSRS::CSpatialReference* GetSpatialRef() const;

		/**
		* @brief 设置空间参考系统
		* @param[in] const std::string& strWkt
		* @return void
		* @author 周旭光
		* @date 2015年7月9日
		* @note 
		*/
		void SetSpatialRef(const std::string& strWkt);

		/**
		* @brief 获得正向的坐标转换器，即从该图层到当前地图的转换
		* @return SysSRS::CCoordTransformation* 坐标转换器
		* @author 周旭光
		* @date 2015年12月2日
		* @par Sample code
		* @code @endcode
		* @par Output
		* @see 
		* @note 
		*/
		SysSRS::CCoordTransformation *GetForwardCoordTran() const;

		/**
		* @brief 获得反向的坐标转换器，即从当前地图到该图层转换
		* @return SysSRS::CCoordTransformation* 坐标转换器
		* @author 周旭光
		* @date 2015年12月2日
		* @par Sample code
		* @code @endcode
		* @par Output
		* @see 
		* @note 
		*/
		SysSRS::CCoordTransformation *GetInverseCoordTran() const;

		
	private:
		unsigned int m_TransparencyLevel;

		SysEnvelope m_LayerExtent;		//图层的范围

		bool m_bValid;					//是否有效

		std::string m_strLayerName;			//图层的名字

		SysMapLayer::LayerType m_LayerType;		//图层的类型

		bool m_bVisible;						//是否可见

		float m_fMinScale;						//最大比例尺
		float m_fMaxScale;						//最小比例尺
		bool m_bScaleBasedVisibility;

		SysSRS::CSpatialReference* m_pCRS;		//空间参考系统
		SysSRS::CCoordTransformation *m_pForwardTran;		//正向变换
		SysSRS::CCoordTransformation *m_pInverseTran;		//逆向变换

	private:

		SysMapLayer( SysMapLayer const & );

		SysMapLayer & operator=( SysMapLayer const & );

		void SetForwardCoordTran(SysSRS::CSpatialReference* pSrcSrs,SysSRS::CSpatialReference* pDstSrs);

		void SetInverseCoordTran(SysSRS::CSpatialReference* pSrcSrs,SysSRS::CSpatialReference* pDstSrs);

	};

	class SysRasterLayer;
	class SysFeatureLayer;

	SYSDRAW_API SysRasterLayer* OpenRaster(std::string strFileName,DrawProgressFunc ProgressFunc = NULL, void* pProgressArg = NULL);

	SYSDRAW_API SysFeatureLayer* OpenFeature(std::string strFileName);

}



#endif // end of __SYSMAPLAYER_H_C355D47F_C869_4D3B_B313_4C9947B6D4BC__
