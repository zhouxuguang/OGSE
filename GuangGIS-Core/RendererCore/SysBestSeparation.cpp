#include "SysBestSeparation.h"
#include <float.h>


double SysDraw::BestTwoSegment( double* pMatError,int nCount,int nMaxM,int &nIndex )
{
	double dbSumTwoMin = DBL_MAX;
	for (int i = 0; i < nMaxM; i ++)
	{
		double dbSum2 = pMatError[i] + pMatError[(i+1)*nCount+nMaxM];
		if (dbSum2 < dbSumTwoMin)
		{
			dbSumTwoMin = dbSum2;
			nIndex = i;
		}
	}

	return dbSumTwoMin;
}

void SysDraw::BestKSegment( double* pMatError,int nCount,int nClassNum, int *pIndex )
{
	int nIndex = nCount-1;
	for (int i = 0; i < nClassNum-1; i ++)
	{
		BestTwoSegment(pMatError,nCount,nIndex,nIndex);
		pIndex[i] = nIndex;
	}
}