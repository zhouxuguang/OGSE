/**
* @file              SysFeatureCache.h
* @brief             矢量要素的缓存
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年11月17日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSFEATURECACHE_H_96AC063E_EE20_4F3F_80BC_8B4A01160F01__
#define __SYSFEATURECACHE_H_96AC063E_EE20_4F3F_80BC_8B4A01160F01__

#include "SysDrawCommon.h"

namespace SysDraw
{

	class SysFeature;

	class SYSDRAW_API SysFeatureCache
	{
	public:
		SysFeatureCache(void);

		~SysFeatureCache(void);

		/**
		* @brief 获得缓存的最大要素个数
		* @return int
		* @author 周旭光
		* @date 2015年11月17日
		* @note 
		*/
		int GetMaxFeatureCount() const;

		/**
		* @brief 设置缓存的最大要素个数
		* @param[in] int nMaxCount
		* @return void
		* @author 周旭光
		* @date 2015年11月17日
		* @note 
		*/
		void SetMaxFeatureCount(int nMaxCount);

		/**
		* @brief 插入一个FID和要素到缓存中
		* @param[in] long nFid
		* @param[in] SysFeature *pFeature
		* @return bool
		* @author 周旭光
		* @date 2015年11月17日
		* @note 
		*/
		bool InsertFeature(long nFid,SysFeature *pFeature);

		/**
		* @brief 从缓存中删除FID为nFid的要素，并销毁要素所占的内存
		* @param[in] long nFid
		* @return bool
		* @author 周旭光
		* @date 2015年11月17日
		* @note 
		*/
		bool EraseFeature(long nFid);

		/**
		* @brief 测试FID为nFid的要素是否在缓存中,如果成功pFeature即返回的要素
		* @param[in] long nFid
		* @param[out] SysFeature*& pFeature
		* @return bool
		* @author 周旭光
		* @date 2015年11月17日
		* @note 
		*/
		bool FindFeature(long nFid,SysFeature*& pFeature) const;

	private:
		typedef std::map<long,SysFeature *> mapFeatureCache;		//矢量要素缓存
		mapFeatureCache m_mapFeaCache;								//矢量缓存map
		int				m_nMaxCount;								//最大要素个数
	};

}

#endif // end of __SYSFEATURECACHE_H_96AC063E_EE20_4F3F_80BC_8B4A01160F01__
