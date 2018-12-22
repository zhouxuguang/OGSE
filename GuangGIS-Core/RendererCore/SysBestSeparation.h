/**
* @file              SysBestSeparation.h
* @brief             最优分割分级方法
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年11月3日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSBESTSEPARATION_H_B76ACF87_A19F_494B_9587_050DADBA0875__
#define __SYSBESTSEPARATION_H_B76ACF87_A19F_494B_9587_050DADBA0875__

namespace SysDraw
{
	
	/**
	* @brief 最优二分割
	* @param[in] double* pMatError 误差矩阵，即离差平方和矩阵
	* @param[in] int nCount 矩阵的维数，即样本数量
	* @param[in] int nMaxM	最大的分级索引值，必须小于等于nCount-1
	* @param[out] int &nIndex 分割点的索引
	* @return double 分割后的最小离差平方和
	* @author 周旭光
	* @date 2015年11月3日
	* @note 
	*/
	double BestTwoSegment(double* pMatError,int nCount,int nMaxM,int &nIndex);

	/**
	* @brief 最优K分割
	* @param[in] double* pMatError 误差矩阵，即离差平方和矩阵
	* @param[in] int nCount 矩阵的维数，即样本数量
	* @param[in] int nClassNum 分级数
	* @param[in] int *pIndex 分级点索引
	* @return void
	* @author 周旭光
	* @date 2015年11月3日
	* @note 
	*/
	void BestKSegment(double* pMatError,int nCount,int nClassNum, int *pIndex);

}

#endif // end of __SYSBESTSEPARATION_H_B76ACF87_A19F_494B_9587_050DADBA0875__
