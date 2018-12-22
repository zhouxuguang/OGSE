
#include <math.h>
#include <float.h>
#include "CG_LineSimplify.h"
#include "CG_BaseCal.h"

void FindSplit(CG_Point *poPoints, int i, int j, int *pIndex, double *pDist)
{
	//中间点逐个计算距离
	double tempDist = 0;
	*pDist = 0;
	*pIndex = 0;

	for(int k = i+1; k< j; k ++)
	{
		tempDist = PointToLine(poPoints[k],poPoints[i],poPoints[j]);  //计算点到线的距离

		//逐个距离比较
		if (tempDist >= *pDist)
		{
			*pDist = tempDist;
			*pIndex = k;		//当前点为分裂点
		}
	}
}

void DouglasPeucker(CG_Point *poPoints, bool* pFlags, int nCount, 
					int nStart, int nEnd, double dbEps,
					std::vector<CG_Point> &vecPoint)
{
	//首先计算中间各个点到首尾连线直线的距离，求出最大距离
	double dbDist = DBL_MAX;
	int nIndex = 0;
	FindSplit(poPoints,nStart,nEnd,&nIndex,&dbDist);

	/*如果最大距离大于容差，则以保留距离最大这个点，
	再以这个点为分裂点分为两段，然后递归地对这两段重复进行化简	*/
	
	if (dbDist <= dbEps)	  //直到距离为0，则停止递归
	{
		//直接以首尾连线作为曲线的近似
 		pFlags[nStart] = 1;
		pFlags[nEnd] = 1;
		//return;
	}
	else if (dbDist > dbEps)
	{
		pFlags[nIndex] = 1;
		DouglasPeucker(poPoints,pFlags,nCount,nStart,nIndex,dbEps,vecPoint);	//对前半段进行处理
		DouglasPeucker(poPoints,pFlags,nCount,nIndex,nEnd,dbEps,vecPoint);		//对后半段进行处理
	}

	//最后将标记为TRUE的点放入结果集
	for (int i = 0; i < nCount; i ++)
	{
		if (pFlags[i])
		{
			vecPoint.push_back(poPoints[i]);
		}
	}
}
