/**
* @file              SysSpatialIndex.h
* @brief             空间索引
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年10月16日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSSPATIALINDEX_H_58054C27_FF11_4F84_B16B_D4CB5AF51176__
#define __SYSSPATIALINDEX_H_58054C27_FF11_4F84_B16B_D4CB5AF51176__

#include "SysDrawCommon.h"

namespace SpatialIndex
{
	class IStorageManager;
	class ISpatialIndex;
	class Region;
	class Point;

	namespace StorageManager
	{
		class IBuffer;
	}
}

namespace SysDraw
{
	class SysFeature;
	class SysEnvelope;
	class SysCoordinate;

	class SYSDRAW_API SysSpatialIndex
	{
	public:
		/**
		* @brief 创建内存空间索引
		* @param[in] void
		* @return 
		* @author 周旭光
		* @date 2015年11月18日
		* @note 
		*/
		SysSpatialIndex(void);

		/**
		* @brief 创建文件空间索引
		* @param[in] const std::string& strFileName
		* @param[in] bool bCreateNewIndex = false
		* @return explicit
		* @author 周旭光
		* @date 2015年10月18日
		* @note 
		*/
		explicit SysSpatialIndex(const std::string& strFileName,bool bCreateNewIndex = false);

		~SysSpatialIndex(void);

		/**
		* @brief 将要素加入到索引中
		* @param[in] SysFeature* pFeature
		* @return bool
		* @author 周旭光
		* @date 2015年10月18日
		* @note 
		*/
		bool InsertFeature( SysFeature* pFeature );

		/**
		* @brief 从索引中删除要素
		* @param[in] SysFeature* pFeature
		* @return bool
		* @author 周旭光
		* @date 2015年10月18日
		* @note 
		*/
		bool DeleteFeature( SysFeature* pFeature );

		/**
		* @brief 矩形查询
		* @param[in] const SysEnvelope& rect
		* @return std::vector<long>
		* @author 周旭光
		* @date 2015年10月18日
		* @note 
		*/
		std::vector<long> IntersectQuery( const SysEnvelope& rect ) const;

		/**
		* @brief 点查询
		* @param[in] double dbX
		* @param[in] double dbY
		* @return std::vector<long>
		* @author 周旭光
		* @date 2015年10月18日
		* @note 
		*/
		std::vector<long> PointLocationQuery(double dbX,double dbY) const;

		/**
		* @brief 点查询
		* @param[in] const SysCoordinate& point
		* @return std::vector<long>
		* @author 周旭光
		* @date 2015年10月18日
		* @note 
		*/
		std::vector<long> PointLocationQuery(const SysCoordinate& point) const;

		/**
		* @brief 最邻近查询
		* @param[in] double dbX
		* @param[in] double dbY
		* @param[in] int nNeighbors
		* @return std::vector<long>
		* @author 周旭光
		* @date 2015年11月11日
		* @note 
		*/
		std::vector<long> NearestNeighbor( double dbX, double dbY, int nNeighbors ) const;

		/**
		* @brief 最邻近查询
		* @param[in]  const SysCoordinate& point
		* @param[in] int nNeighbors
		* @return std::vector<long>
		* @author 周旭光
		* @date 2015年10月18日
		* @note 
		*/
		std::vector<long> NearestNeighbor( const SysCoordinate& point, int nNeighbors ) const;

		/**
		* @brief 将索引保存在文件中
		* @return bool
		* @author 周旭光
		* @date 2015年11月18日
		* @note 
		*/
		bool SaveToDisk() const;


	private:

		SpatialIndex::IStorageManager* m_pStorageManager;			//存储管理器

		SpatialIndex::StorageManager::IBuffer* m_pStorage;		//索引缓冲数据

		SpatialIndex::ISpatialIndex* m_pRTree;					//索引中的R树

		std::string m_strIndexFile;								//空间索引文件

		//void* m_pStorageManager;			//存储管理器

		//void* m_pStorage;		//索引缓冲数据

		//void* m_pRTree;					//索引中的R树
	};

}

#endif // end of __SYSSPATIALINDEX_H_58054C27_FF11_4F84_B16B_D4CB5AF51176__
