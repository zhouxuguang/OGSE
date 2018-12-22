/**
* @file              SysFeatureLayer.h
* @brief             矢量图层
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年7月1日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSFEATURELAYER_H_78C57375_349B_49C6_A5B0_B35ADF7CF255__
#define __SYSFEATURELAYER_H_78C57375_349B_49C6_A5B0_B35ADF7CF255__

#include "SysMapLayer.h"

namespace SysDataSource
{
	class IDataLayer;
	class IFeatureDataLayer;
}

namespace SysDraw
{
	class SysEnvelope;
	class SysRasterSource;
	class SysFeature;
	class SysFeatureLabel;
	class SysFeatureRenderer;
	class SysSpatialIndex;
	class SysFeatureCache;

	enum VertexMarkType
	{
		Square,		//正方形
		Cross,		//十字丝
	};

	class SYSDRAW_API SysFeatureLayer : public SysMapLayer
	{
	public:
		explicit SysFeatureLayer(std::string strLyrName);

		~SysFeatureLayer(void);

		/**
		* @brief 设置数据源
		* @param[in] SysDataSource::IDataLayer* pDatalayer
		* @return bool
		* @author 周旭光
		* @date 2015年7月10日
		* @note 
		*/
		bool SetDataSource(SysDataSource::IDataLayer* pDatalayer);

		/**
		* @brief 获得数据源
		* @return SysDataSource::IDataLayer*
		* @author 周旭光
		* @date 2015年7月10日
		* @note 
		*/
		SysDataSource::IFeatureDataLayer* GetDataSource() const;

		/**
		* @brief 获得矢量要素渲染器
		* @return SysFeatureRenderer*
		* @author 周旭光
		* @date 2015年8月14日
		* @note 
		*/
		SysFeatureRenderer* GetFeatureRenderer() const;

		/**
		* @brief 设置矢量要素渲染器
		* @param[in] SysFeatureRenderer* pRenderer
		* @return void
		* @author 周旭光
		* @date 2015年8月14日
		* @note 
		*/
		void SetFeatureRenderer(SysFeatureRenderer* pRenderer);

		/**
		* @brief 创建矢量图层
		* @param[in] SysDataSource::IDataLayer* pDatalayer
		* @return SysRasterLayer* 返回创建的图层指针
		* @author 周旭光
		* @date 2015年7月1日
		* @note 
		*/
		static SysFeatureLayer* CreateFeatureLayer(SysDataSource::IDataLayer* pDatalayer);

		/**
		* @brief 销毁图层
		* @param[in] SysRasterLayer* pVecLayer
		* @return void 
		* @author 周旭光
		* @date 2015年7月1日
		* @note 
		*/
		static void DestroyFeatureLayer(SysFeatureLayer* pVecLayer);

		/**
		* @brief Draw
		* @param[in] SysRenderContext* rendererContext
		* @return bool
		* @author 周旭光
		* @date 2015年8月16日
		* @note 
		*/
		virtual bool Draw( SysRenderContext* rendererContext );

		/**
		* @brief DrawLabels
		* @param[in] SysRenderContext* rendererContext
		* @return void
		* @author 周旭光
		* @date 2015年8月16日
		* @note 
		*/
		virtual void DrawLabels( SysRenderContext* rendererContext );

		void ResetRead();

		/**
		* @brief 获得下一个要素
		* @return SysFeature*
		* @author 周旭光
		* @date 2015年8月16日
		* @note 
		*/
		SysFeature* GetNextFeature() const;

		/**
		* @brief 获得要素个数
		* @return int
		* @author 周旭光
		* @date 2015年8月16日
		* @note 
		*/
		int GetFeatureCount() const;

		/**
		* @brief 根据FID获得要素
		* @param[in] int nIndex
		* @return SysFeature*
		* @author 周旭光
		* @date 2015年8月16日
		* @note 
		*/
		SysFeature* GetFeatureByFID(int nIndex) const;

		/**
		* @brief 设置空间过滤
		* @param[in] const SysEnvelope& envFilter
		* @return void
		* @author 周旭光
		* @date 2015年8月16日
		* @note 
		*/
		void SetSpatialFilter(const SysEnvelope& envFilter);

		/**
		* @brief 根据矩形选择要素
		* @param[in] const SysEnvelope& envFilter
		* @return std::vector<long>
		* @author 周旭光
		* @date 2015年11月16日
		* @note 
		*/
		std::vector<long> SelectFeatures(const SysEnvelope& envFilter);

		/**
		* @brief 创建空间索引
		* @param[in] DrawProgressFunc pProgressFunc = NULL
		* @param[in] void* pProgressArg = NULL
		* @return bool
		* @author 周旭光
		* @date 2015年11月16日
		* @note 
		*/
		bool BuildSpatialIndex(DrawProgressFunc pProgressFunc = NULL, void* pProgressArg = NULL);

		/**
		* @brief GetSpatialIndex
		* @return SysSpatialIndex*
		* @author 周旭光
		* @date 2015年11月19日
		* @par Sample code
		* @code @endcode
		* @note 
		*/
		SysSpatialIndex* GetSpatialIndex() const;

		/**
		* @brief GetFeatureCache
		* @return SysFeatureCache*
		* @author 周旭光
		* @date 2015年11月19日
		* @par Sample code
		* @code @endcode
		* @par Output
		* @note 
		*/
		SysFeatureCache* GetFeatureCache() const;

		/**
		* @brief 插入选中或者未选中的FID
		* @param[in] long nFid
		* @param[in] bool bSelected
		* @return bool
		* @author 周旭光
		* @date 2015年8月16日
		* @par Sample code
		* @code
		    char **path = NULL;
		    int count = 0;
		    test_get_paths(&path, &count);
		    //use the path
		    free(path);
		    path = NULL; @endcode
		* @see BuildSpatialIndex
		*/
		bool InsertSelectedFID(long nFid,bool bSelected = true) const;

		/**
		* @brief IsSelected
		* @param[in] long nFid
		* @return bool
		* @author 周旭光
		* @date 2015年8月16日
		* @note 
		*/
		bool IsSelected(long nFid) const;

		/**
		* @brief RemoveSelected
		* @return void
		* @author 周旭光
		* @date 2015年8月16日
		* @note 
		*/
		void RemoveSelected() /*const*/;

		/**
		* @brief 获得标注渲染器
		* @return SysFeatureLabel *
		* @author 周旭光
		* @date 2015年10月20日
		* @note 
		*/
		SysFeatureLabel *GetFeatureLabel() const;

		/**
		* @brief 设置标注渲染器
		* @param[in] SysFeatureLabel *pLabel
		* @return void
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		void SetFeatureLabel(SysFeatureLabel *pLabel);

		/**
		* @brief 绘制顶点的标志
		* @param[in]  double x
		* @param[in] double y
		* @param[in] double dbScale
		* @param[in] VertexMarkType eType
		* @param[in] int nVertexSize
		* @return void
		* @author 周旭光
		* @date 2015年10月26日
		* @note 
		*/
		static void DrawVertexMarker( double x, double y, double dbScale, VertexMarkType eType, int nVertexSize );

	private:
		SysDataSource::IFeatureDataLayer* m_pDatalayer;		//对应的数据图层
		SysRasterSource* m_pDataSource;
		SysFeatureRenderer* m_pRenderer;				//对应的矢量渲染器

		typedef std::map<long,bool> SysSelectedFIDs;		//要素选择的ID集合
		mutable SysSelectedFIDs m_mapSelectIds;	

		bool m_bLabeled;								//是否绘制标注
		SysFeatureLabel *m_pLabel;						//文本标注渲染器

		bool m_bUseIndex;							//是否使用索引
		SysSpatialIndex *m_pSpatialIndex;				//空间索引

	public:
		typedef std::map<long,SysFeature *> mapFeatureCache;		//矢量要素缓存
		mapFeatureCache m_mapFeaCache;								//矢量缓存map

	private:

		SysFeatureCache* m_pFeatureCache;			//要素缓存

		void Init();		//初始化

		void UnInit();		//反初始化，清理资源
	};

}

#endif // end of __SYSFEATURELAYER_H_78C57375_349B_49C6_A5B0_B35ADF7CF255__
