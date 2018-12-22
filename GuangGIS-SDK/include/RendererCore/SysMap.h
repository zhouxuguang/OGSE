/**
* @file              SysMap.h
* @brief             地图类，管理地图图层
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年6月26日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSMAP_H_8EE9B95C_8995_4B83_B6CD_A9747D717433__
#define __SYSMAP_H_8EE9B95C_8995_4B83_B6CD_A9747D717433__

#include "SysEnvelope.h"

namespace SysSRS
{
	class CSpatialReference;
}

namespace SysDraw
{

	class SysMapLayer;
	class SysGraphLayer;
	class SysMapLayerList;

	class SYSDRAW_API SysMap
	{
	public:
		explicit SysMap(const std::string& strMapName);

		~SysMap(void);

		/**
		* @brief 添加图层
		* @param[in] SysMapLayer* pMapLayer
		* @return void
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		void AddMapLayer(SysMapLayer* pMapLayer);

		/**
		* @brief 根据名字删除图层
		* @param[in] const std::string& strName
		* @return bool
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		bool RemoveMapLayer(const std::string& strName);

		/**
		* @brief 删除指向图层指针的图层
		* @param[in] const SysMapLayer* pLayer
		* @return bool
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		bool RemoveMapLayer(const SysMapLayer* pLayer);

		/**
		* @brief 返回地图的名字
		* @return std::string
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		std::string GetMapName() const;

		/**
		* @brief 判断图层是否存在
		* @param[in] const SysMapLayer* pLayer
		* @return bool
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		bool FindMapLayer(const SysMapLayer* pLayer) const;

		/**
		* @brief 判断图层是否存在
		* @param[in] const std::string& strName
		* @return bool
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		bool FindMapLayer(const std::string& strName) const;

		/**
		* @brief 获得图层的个数
		* @return size_t
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		inline size_t GetLayerCount() const;

		/**
		* @brief 获得当前图层
		* @return SysMapLayer*
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		const SysMapLayer* GetCurrentLayer() const;

		/**
		* @brief 获得当前图层
		* @return SysMapLayer*
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		SysMapLayer* GetCurrentLayer();

		/**
		* @brief 设置当前图层
		* @param[in] const SysMapLayer* pLayer
		* @return void
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		void SetCurrentLayer(const SysMapLayer* pLayer);

		/**
		* @brief 获得地图的空间参考系统
		* @return SysSRS::CSpatialReference*
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		SysSRS::CSpatialReference* GetSpatialRef() const;

		/**
		* @brief 获得地图的可视范围
		* @return SysEnvelope 
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		const SysEnvelope &GetVisualExtent() const;

		/**
		* @brief 获得地图的可视范围
		* @return SysEnvelope
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		SysEnvelope &GetVisualExtent();

		/**
		* @brief 设置地图的可视范围
		* @param[in] const SysEnvelope & env
		* @return void
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		void SetVisualExtent(const SysEnvelope & env);

		/**
		* @brief 获得地图的全图范围
		* @return SysEnvelope
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		const SysEnvelope &GetFullExtent() const;

		/**
		* @brief 获得地图的全图范围
		* @return SysEnvelope
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		SysEnvelope &GetFullExtent();

		/**
		* @brief 重置图层列表
		* @return void
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		void Reset();

		/**
		* @brief 获得下一个图层
		* @return SysMapLayer*
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		SysMapLayer* GetNextLayer() const;

		/**
		* @brief 获得地图浏览临时层
		* @return SysGraphLayer*
		* @author 周旭光
		* @date 2015年8月13日
		* @note 
		*/
		SysGraphLayer* GetBrowseLayer() const;

		/**
		* @brief 获得标绘图层
		* @return SysGraphLayer*
		* @author 周旭光
		* @date 2015年8月13日
		* @note 
		*/
		SysGraphLayer* GetMarkLayer() const;

	private:
		SysMapLayerList* m_pLayerList;	//图层列表
		SysMapLayer* m_pCurLayer;		//当前图层
		SysEnvelope m_VisualExtent;		//当前可见的地理范围
		SysEnvelope m_FullExtent;		//全图的地理范围,需要在同一个空间参考下	

		SysGraphLayer* m_pBrowseLayer;		//地图浏览标绘图层	
		SysGraphLayer* m_pMarkLayer;		//标绘层

		std::string m_strMapName;		//地图的名字

		void RemoveAllMapLayers();

		/**
		* @brief 重新计算全图的范围
		* @return void
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		void ReCaclFullExtent();
	};


}

#endif // end of __SYSMAP_H_8EE9B95C_8995_4B83_B6CD_A9747D717433__
