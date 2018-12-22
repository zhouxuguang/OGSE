/**
* @file              SysValueStatistics.h
* @brief             基本的数值统计方法
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年8月28日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSVALUESTATISTICS_H_9D78A9E6_6083_4D08_9494_0F0B42B7EFD2__
#define __SYSVALUESTATISTICS_H_9D78A9E6_6083_4D08_9494_0F0B42B7EFD2__

#include "SysDrawCommon.h"

namespace SysDraw
{
	enum ClassBreak_Type
	{
		NanualBreaks,					//手工分类
		EqualInterval,			//等间隔分类
		NaturalBreaks,			//自然分裂法分类
		OptimalBreaks,			//最优分割分级
		FuzzyBreaks,			//模糊分类分级
		StandardDeviationBreaks		//标准差分类
	};


	/**
	* @brief 获得基本统计值
	* @param[in] const double* pValues
	* @param[in] int nCount
	* @param[out] double *pMin
	* @param[out] double *pMax
	* @param[out] double *pSum
	* @param[out] double *pMean
	* @param[out] double *pMedian
	* @param[out] double *pStdDev
	* @return void
	* @author 周旭光
	* @date 2015年8月28日
	* @note 
	*/
	SYSDRAW_API void GetValueStatistics(
								const double* pValues,
								int nCount,
								double *pMin,
								double *pMax,
								double *pSum,
								double *pMean,
								double *pMedian,
								double* pStdDev);

	/**
	* @brief 获得分级的间隔值序列
	* @param[in] double* pValues 数据序列值
	* @param[in] int nCount
	* @param[in] onst double dMinValue 最小值
	* @param[in] const double dMaxValue 最大值
	* @param[in] int nClassNum 分级数目
	* @param[in] ClassBreak_Type eBreaksMethods 分级方法
	* @param[out] double* pIntervalValues 分级的间隔值序列（数组长度大于等于nClassNum+1），即第n级的值区间是pIntervalValues[n]到pIntervalValues[n+1]							
	* @return bool
	* @author 周旭光
	* @date 2015年10月30日
	* @note 
	*/
	SYSDRAW_API bool GetBreakValues(
								double* pValues,
								int nCount,
								const double dMinValue ,
								const double dMaxValue ,
								int nClassNum,
								ClassBreak_Type eBreaksMethods,
								double* pIntervalValues);

	/**
	* @brief 根据分级间隔值序列获得所在的分级索引
	* @param[in] const double dValue
	* @param[in] double* pIntervalValues
	* @param[in] int nClassNum
	* @return int找到返回索引值，找不到返回-1
	* @author 周旭光
	* @date 2015年11月2日
	* @note 
	*/
	SYSDRAW_API int GetValueIntervalIndex(const double dValue, double* pIntervalValues,int nClassNum);

}

#endif // end of __SYSVALUESTATISTICS_H_9D78A9E6_6083_4D08_9494_0F0B42B7EFD2__



