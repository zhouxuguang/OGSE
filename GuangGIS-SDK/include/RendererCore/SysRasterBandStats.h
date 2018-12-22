/**
* @file              SysRasterBandStats.h
* @brief             栅格图像单个波段统计信息
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年9月28日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSRASTERBANDSTATS_H_8B326131_D709_4491_A378_82BB6A2C9FBD__
#define __SYSRASTERBANDSTATS_H_8B326131_D709_4491_A378_82BB6A2C9FBD__

#include "SysDrawCommon.h"

namespace SysDraw
{

	class SYSDRAW_API SysRasterBandStats
	{
	public:
		SysRasterBandStats(void)
		{			
			m_dbMinimumValue = DBL_MAX;
			m_dbMaximumValue = -DBL_MAX;
			m_dbRange = 0.0;
			m_dbMean = 0.0;
			m_dbSumOfSquares = 0.0;
			m_dbStdDev = 0.0;
			m_dbSum = 0.0;
			m_nElementCount = 0;
			memset(m_pHistogramArray,0,sizeof(eDouble)*256);
		}

		eInt32 m_nBandNumber;		//波段序号

		eUInt32 m_nElementCount;	//总的象元个数

		eDouble m_dbMaximumValue;	//最大值

		eDouble m_dbMinimumValue;	//最小值

		eDouble m_dbMean;			//中值

		eDouble m_dbRange;			//值域

		eDouble m_dbStdDev;			//标准差

		eDouble m_dbSum;				//总和

		eDouble m_dbSumOfSquares;	//平方和

		eDouble m_pHistogramArray[256];	//直方图数组
	};

}

#endif // end of __SYSRASTERBANDSTATS_H_8B326131_D709_4491_A378_82BB6A2C9FBD__
