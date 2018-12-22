#include "SysValueStatistics.h"
#include "SysBestSeparation.h"

#include <float.h>

namespace SysDraw
{


	void GetValueStatistics( 
		const double* pValues,
		int nCount,
		double *pMin, 
		double *pMax, 
		double *pSum, 
		double *pMean, 
		double *pMedian, 
		double* pStdDev )
	{
		double dbMin = pValues[0];
		double dbMax = pValues[0];
		double dbSum = 0.0;
		double dbMean = 0.0;
		double dbMedian = 0.0;
		double dbStdDev = 0.0;

		//统计最值和总和
		for (int i = 0; i < nCount; i ++)
		{
			if (dbMin > pValues[i])
			{
				dbMin = pValues[i];
			}

			if (dbMax < pValues[i])
			{
				dbMax = pValues[i];
			}

			dbSum += pValues[i];
		}

		if (pSum != NULL)
		{
			*pSum = dbSum;
		}
		if (pMax != NULL)
		{
			*pMax = dbMax;
		}
		if (pMin != NULL)
		{
			*pMin = dbMin;
		}

		dbMean = dbSum / nCount;
		if (pMean != NULL)
		{
			*pMean = dbMean;
		}
		if (pMedian != NULL)
		{
			*pMedian = dbMedian;
		}

		if (pStdDev != NULL)
		{
			double dbSumDev = 0.0;
			double dbInvCount = 1.0/nCount;

			#pragma omp parallel for default(none) shared(nCount,pValues,dbMean) reduction(+:dbSumDev)
			for (int i = 0; i < nCount; i ++)
			{
				dbSumDev += ((pValues[i] - dbMean) * (pValues[i] - dbMean))/nCount;
			}

			dbStdDev = sqrt(dbSumDev);
			*pStdDev = dbStdDev;
		}
	}

	//分级函数声明
	typedef bool (* BreakValueFunc)(
		double* pValues, 
		int nCount, 
		const double dMinValue , 
		const double dMaxValue , 
		int nClassNum,  
		double* pIntervalValues );

	//等间隔分类
	static bool GetEqualIntervalValues(double* pValues, 
		int nCount, 
		const double dMinValue , 
		const double dMaxValue , 
		int nClassNum,  
		double* pIntervalValues );

	//自然分裂法
	static bool GetNaturalBreaksValues(double* pValues, 
		int nCount, 
		const double dMinValue , 
		const double dMaxValue , 
		int nClassNum,  
		double* pIntervalValues );

	//标准差分级
	static bool GetStandardDeviationValues(double* pValues, 
		int nCount, 
		const double dMinValue , 
		const double dMaxValue , 
		int nClassNum,  
		double* pIntervalValues);

	//最优分割分级
	static bool GetOptimalBreaksValues(double* pValues, 
		int nCount, 
		const double dMinValue , 
		const double dMaxValue , 
		int nClassNum,  
		double* pIntervalValues);

	//模糊分类分级
	static bool GetFuzzyBreaksValues(double* pValues, 
		int nCount, 
		const double dMinValue , 
		const double dMaxValue , 
		int nClassNum,  
		double* pIntervalValues);

	bool GetBreakValues( 
		double* pValues, 
		int nCount, 
		const double dMinValue , 
		const double dMaxValue , 
		int nClassNum, 
		ClassBreak_Type eBreaksMethods, 
		double* pIntervalValues )
	{
		BreakValueFunc pFun = NULL;

		switch (eBreaksMethods)
		{
		case EqualInterval:
			pFun = GetEqualIntervalValues;
			break;

		case NaturalBreaks:
			pFun = GetNaturalBreaksValues;
			break;

		case StandardDeviationBreaks:
			pFun = GetStandardDeviationValues;
			break;

		case OptimalBreaks:
			pFun = GetOptimalBreaksValues;
			break;

		case FuzzyBreaks:
			pFun = GetFuzzyBreaksValues;
			break;

		default:
			break;
		}

		if (pFun != NULL)
		{
			return pFun(pValues,nCount,dMinValue,dMaxValue,nClassNum,pIntervalValues);
		}

		return false;

	}

	bool GetEqualIntervalValues( double* pValues, int nCount, const double dMinValue , 
		const double dMaxValue , int nClassNum, double* pIntervalValues )
	{
		double dbInterval = dMaxValue - dMinValue;
		double dbSingleInterval = dbInterval / nClassNum;

		pIntervalValues[0] = dMinValue;
		for (int i = 0; i < nClassNum-1; i ++)
		{
			pIntervalValues[i + 1] = dMinValue + (i+1)*dbSingleInterval;
		}
		pIntervalValues[nClassNum] = dMaxValue;

		return true;
	}

	bool GetNaturalBreaksValues( double* pValues, int nCount,
		const double dMinValue , const double dMaxValue , int nClassNum, double* pIntervalValues )
	{
		assert(false);
		return false;
	}

	bool GetStandardDeviationValues( double* pValues, int nCount,
		const double dMinValue , const double dMaxValue , int nClassNum, double* pIntervalValues )
	{
		if (NULL == pValues || 0 == nCount || nClassNum <= 0)
		{
			return false;
		}

		if (nClassNum != 5 && nClassNum != 9 && nClassNum != 17)
		{
			return false;
		}

		//统计最大最小值,均值标准差
		double dbMax,dbMin,dbMean,dbStdDev;
		GetValueStatistics(pValues,nCount,&dbMin,&dbMax,NULL,&dbMean,NULL,&dbStdDev);
		pIntervalValues[0] = dbMin;
		pIntervalValues[nClassNum] = dbMax;

		double dbInterval = dbStdDev;
		if (9 == nClassNum)
		{
			dbInterval /= 2;
		}
		if (17 == nClassNum)
		{
			dbInterval /= 4;
		}

		int nTmpCount = nClassNum+10;
		double *pTmpInterval = new double[nTmpCount+1];

		int nMidIndex1 = nTmpCount/2;
		int nMidIndex2 = nTmpCount/2 + 1;
		pTmpInterval[nMidIndex1] = dbMean - dbInterval/2;
		pTmpInterval[nMidIndex2] = dbMean + dbInterval/2;
		int nIterCount = nMidIndex1;
		for (int i = 1; i <= nIterCount; i ++)
		{
			pTmpInterval[nMidIndex1-i] = pTmpInterval[nMidIndex1] - i*dbInterval;
			pTmpInterval[nMidIndex2+i] = pTmpInterval[nMidIndex2] + i*dbInterval;
		}

		int nCurIndex = 0;

		for (int i = 1; i < nClassNum; i ++)
		{
			while (1)
			{
				if (pTmpInterval[nCurIndex] < pIntervalValues[i-1])
				{
					nCurIndex ++;
				}
				else
				{
					break;
				}
			}
			pIntervalValues[i] = pTmpInterval[nCurIndex++];
		}

		delete []pTmpInterval;

		return true;
	}

	//求一个数组区间的离差平方和
	static double GetSumOfSquaresDeviation(double* pValues,int nStartIndex,int nEndIndex)
	{
		double dbRet = 0;
		if (nEndIndex <= nStartIndex)
		{
			return dbRet;
		}

		double dbMean = 0;
		double dbSum = 0;
		for (int i = nStartIndex; i < nEndIndex; i ++)
		{
			dbSum += pValues[i];
		}

		dbMean = dbSum / (nEndIndex - nStartIndex + 1);

		for (int i = nStartIndex; i < nEndIndex; i ++)
		{
			dbRet += (pValues[i] - dbMean) * (pValues[i] - dbMean);
		}
		return dbRet;
	}

	bool GetOptimalBreaksValues( double* pValues, int nCount, 
		const double dMinValue , const double dMaxValue , int nClassNum, double* pIntervalValues )
	{
		if (nClassNum <= 1 || NULL == pValues || nCount <=1 || NULL == pIntervalValues)
		{
			return false;
		}
		//数据从小到大排序
		std::sort(pValues,pValues+nCount);

		//计算误差矩阵
		double* pMatError = new double[nCount*nCount];
		for (int nRow = 0; nRow < nCount; nRow ++)
		{
			for (int nCol = 0; nCol < nCount; nCol ++)
			{
				pMatError[nRow*nCount+nCol] = GetSumOfSquaresDeviation(pValues,nRow,nCol);
			}
		}

		//最优K分割
		int *pIndex = new int[nClassNum-1];
		BestKSegment(pMatError,nCount,nClassNum,pIndex);
		pIntervalValues[0] = pValues[0];
		pIntervalValues[nClassNum] = pValues[nCount-1];

		std::reverse(pIndex,pIndex+nClassNum-1);
		for (int i = 1; i < nClassNum; i ++)
		{
			pIntervalValues[i] = pValues[pIndex[i-1]];
		}

		delete []pMatError;
		delete []pIndex;

		return true;

	}

	bool GetFuzzyBreaksValues( double* pValues, int nCount, 
		const double dMinValue , const double dMaxValue , int nClassNum, double* pIntervalValues )
	{
		if (nClassNum <= 1 || NULL == pValues || nCount <=1 || NULL == pIntervalValues)
		{
			return false;
		}

		//数据排序
		std::sort(pValues,pValues+nCount);

		//统计最大最小值
		double dbMax,dbMin;
		GetValueStatistics(pValues,nCount,&dbMin,&dbMax,NULL,NULL,NULL,NULL);

		//选择初始分级（等间隔分级）
		GetEqualIntervalValues(pValues,nCount,dbMin,dbMax,nClassNum,pIntervalValues);

		//记录中间的结果
		double* pMeans = new double[nClassNum];
		memset(pMeans,0,sizeof(double)*nClassNum);
		double* pSums = new double[nClassNum];
		memset(pSums,0,sizeof(double)*nClassNum);
		int *pCounts = new int[nClassNum];
		memset(pCounts,0,sizeof(int)*nClassNum);

		bool bIter = false;		//是否还需要迭代
		int nIterCount = 0;

		while (1)
		{
			bIter = false;
			for(int i = 0; i< nCount; i ++)
			{
				int nIndex = GetValueIntervalIndex(pValues[i],pIntervalValues,nClassNum);
				if (nIndex != -1)
				{
					pCounts[nIndex]++;
					pSums[nIndex] += pValues[i];
				}

			}

			for (int i = 0; i < nClassNum; i ++)
			{
				pMeans[i] = pSums[i]/pCounts[i];
			}

			//模糊度函数判别
			for(int i = 0; i< nCount; i ++)
			{
				//判断是属于哪个区间
				int nMaxIndex = 0;
				double dbRijMax = -DBL_MAX;
				for (int k = 0; k < nClassNum; k ++)
				{
					double dbRij = min(pValues[i],pMeans[k])/max(pValues[i],pMeans[k]);
					if (dbRij > dbRijMax)
					{
						dbRijMax = dbRij;
						nMaxIndex = k;
					}
				}

				//如果范围发生变化，还需要继续迭代
				if (pValues[i] > pIntervalValues[nMaxIndex+1])
				{
					pIntervalValues[nMaxIndex+1] = pValues[i];
					bIter = true;
				}

				if (pValues[i] < pIntervalValues[nMaxIndex])
				{
					pIntervalValues[nMaxIndex] = pValues[i];
					bIter = true;
				}
			}

			nIterCount++;

			//如果范围不发生变化，退出迭代
			if (!bIter || nIterCount >= 4)
			{
				break;
			}

			memset(pMeans,0,sizeof(double)*nClassNum);
			memset(pSums,0,sizeof(double)*nClassNum);
			memset(pCounts,0,sizeof(int)*nClassNum);
		}

		//清理内存
		delete []pMeans;
		delete []pSums;
		delete []pCounts;

		return true;
	}

	int GetValueIntervalIndex(const double dValue, double* pIntervalValues,int nClassNum)
	{
		//二分查找数据区间
		int nLow = 0;
		int nHigh = nClassNum - 1;

		double dbMin = 0;
		double dbMax = 0;

		while (nLow <= nHigh)
		{
			int nMid = (nLow + nHigh)/2;
			dbMin = pIntervalValues[nMid];
			dbMax = pIntervalValues[nMid+1];
			if (dValue >= dbMin && dValue <= dbMax)
			{
				return nMid;
			}

			else if (dValue > dbMax)
			{
				nLow = nMid + 1;
			}

			else if (dValue < dbMin)
			{
				nHigh = nMid - 1;
			}
		}

		return -1;
	}

	/**
	* @brief 标准差分级
	* @param[in] double* pValues 数据序列值
	* @param[in] int nCount	样本个数
	* @param[in] int nClassNum 分类数目
	* @param[in] double dbDevInterval 标准差的间隔，比如一个标准差，二分之一个标准差
	* @param[out] double* pIntervalValues 分级的间隔值序列
	* @return void
	* @author 周旭光
	* @date 2015年11月4日
	* @note 
	*/
	void GetStandardDeviationBreakValues( double* pValues, int nCount, int nClassNum, double dbDevInterval, double* pIntervalValues )
	{
		if (NULL == pValues || 0 == nCount || nClassNum <= 0)
		{
			return;
		}

		//统计最大最小值,均值标准差
		double dbMax,dbMin,dbMean,dbStdDev;
		GetValueStatistics(pValues,nCount,&dbMin,&dbMax,NULL,&dbMean,NULL,&dbStdDev);
	}
}
