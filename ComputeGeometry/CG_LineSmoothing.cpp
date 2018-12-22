#include "CG_LineSmoothing.h"

void WipeAngleMethod(SPOINT* pPoints,int nCount,int k)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < k; i ++)
	{
		for (j = 1; j < nCount-1; j ++)
		{
		}
	}
	
}


static double CalKValue(SPOINT pt1,SPOINT pt2)
{
	return (pt2.y - pt1.y) / (pt2.x - pt1.x);
}

bool FivePointsSmoothing(const SPOINT* poPoints,int nCount,double dbEps,std::vector<SPOINT>& vecResult)
{
	bool bFlag = false;
	if (nCount < 3)
	{
		vecResult.resize(nCount);
		std::copy(poPoints,poPoints+nCount,vecResult.begin());
		return bFlag;
	}

	//1、首先在首尾处插补两点
	std::vector<SPOINT> vecCopyPoints;
	vecCopyPoints.resize(nCount+4);
	std::copy(poPoints,+poPoints+nCount,vecCopyPoints.begin()+2);
	nCount += 4;

	vecCopyPoints[1].x = 3*(vecCopyPoints[2].x - vecCopyPoints[3].x) + vecCopyPoints[4].x;
	vecCopyPoints[0].x = 3*(vecCopyPoints[1].x - vecCopyPoints[2].x) + vecCopyPoints[3].x;

	vecCopyPoints[1].y = 3*(vecCopyPoints[2].y - vecCopyPoints[3].y) + vecCopyPoints[4].y;
	vecCopyPoints[0].y = 3*(vecCopyPoints[1].y - vecCopyPoints[2].y) + vecCopyPoints[3].y;

	vecCopyPoints[nCount-2].x = 3*(vecCopyPoints[nCount-3].x - vecCopyPoints[nCount-4].x) + vecCopyPoints[nCount-5].x;
	vecCopyPoints[nCount-1].x = 3*(vecCopyPoints[nCount-2].x - vecCopyPoints[nCount-3].x) + vecCopyPoints[nCount-4].x;

	vecCopyPoints[nCount-2].y = 3*(vecCopyPoints[nCount-3].y - vecCopyPoints[nCount-4].y) + vecCopyPoints[nCount-5].y;
	vecCopyPoints[nCount-1].y = 3*(vecCopyPoints[nCount-2].y - vecCopyPoints[nCount-3].y) + vecCopyPoints[nCount-4].y;

	vecResult.clear();

	//2、然后从第三点开始循环
	for (int nIndex = 2; nIndex < nCount - 3; nIndex ++)
	{
		double dbKAdd1 = CalKValue(vecCopyPoints[nIndex+1],vecCopyPoints[nIndex+2]);
		double dbK = CalKValue(vecCopyPoints[nIndex],vecCopyPoints[nIndex+1]);
		double dbKSub1 = CalKValue(vecCopyPoints[nIndex-1],vecCopyPoints[nIndex]);
		double dbKSub2 = CalKValue(vecCopyPoints[nIndex-2],vecCopyPoints[nIndex-1]);
		double dbKAdd2 = CalKValue(vecCopyPoints[nIndex+2],vecCopyPoints[nIndex+3]);

		double dbKAdd1SubK = fabs(dbKAdd1 - dbK);
		double dbKSub1SubKSub2 = fabs(dbKSub1 - dbKSub2);
		double dbKAdd2SubKAdd1 = fabs(dbKAdd2 - dbKAdd1);
		double dbKSubKSub1 = fabs(dbK - dbKSub1);

		//计算theta1
		double dbK1Num = dbKAdd1SubK*dbKSub1 + dbKSub1SubKSub2*dbK;
		double dbK1Den = dbKAdd1SubK + dbKSub1SubKSub2;
		double dbTheta1 = atan2(dbK1Num,dbK1Den);

		//计算theta2
		double dbK2Num = dbKAdd2SubKAdd1*dbK + dbKSubKSub1*dbKAdd1;
		double dbK2Den = dbKAdd2SubKAdd1 + dbKSubKSub1;
		double dbTheta2 = atan2(dbK2Num,dbK2Den);

		double dbSinTheta1 = sin(dbTheta1);
		double dbCosTheta1 = cos(dbTheta1);
		double dbSinTheta2 = sin(dbTheta2);
		double dbCosTheta2 = cos(dbTheta2);

		double dbX2X1 = (vecCopyPoints[nIndex+1].x - vecCopyPoints[nIndex].x);
		double dbY2Y1 = (vecCopyPoints[nIndex+1].y - vecCopyPoints[nIndex].y);

		double dbRadius = sqrt(dbX2X1*dbX2X1 + dbY2Y1*dbY2Y1);

		//计算参数方程的参数
		double dbP0 = vecCopyPoints[nIndex].x;
		double dbP1 = dbRadius * dbCosTheta1;
		double dbP2 = 3*dbX2X1 - dbRadius*(dbCosTheta2 + 2*dbCosTheta1);
		double dbP3 = -2*dbX2X1 + dbRadius*(dbCosTheta2 + dbCosTheta1);

		double dbQ0 = vecCopyPoints[nIndex].y;
		double dbQ1 = dbRadius*dbSinTheta1;
		double dbQ2 = 3*dbY2Y1 - dbRadius*(dbSinTheta2 + 2*dbSinTheta1);
		double dbQ3 = -2*dbY2Y1 + dbRadius*(dbSinTheta2 + dbSinTheta1);

		//确定插值的步长
		double dbFlag = 3*dbP3*(dbP2 + 0.75*dbP3) + 3*dbQ3*(dbQ2 + 0.75*dbQ3);
		double dbMaxFlag = 4*(dbP2*dbP2 + dbQ2*dbQ2);
		if (dbFlag > 0)
		{
			dbMaxFlag += 4*dbFlag;
		}

		double dbT = 0;
		if (dbMaxFlag > 0)
		{
			dbT = 2*sqrt(2*dbEps)/dbMaxFlag;
		}
		else
			dbT = 0.9999;

		dbT = 0.1;

		double dbCurT = 0;
		while (dbCurT < 1)
		{
			SPOINT pt;
			pt.x = dbP0 + dbP1*dbCurT + dbP2*dbCurT*dbCurT + dbP3*dbCurT*dbCurT*dbCurT;
			pt.y = dbQ0 + dbQ1*dbCurT + dbQ2*dbCurT*dbCurT + dbQ3*dbCurT*dbCurT*dbCurT;
			vecResult.push_back(pt);

			dbCurT += dbT;
		}
	}

	//将最后一个点加入到结果中
	vecResult.push_back(vecCopyPoints[nCount-3]);
	vecCopyPoints.clear();

	return bFlag;
	
}

bool OrthoaxisParabolaSmoothing(const SPOINT* poPoints,int nCount,double dbEps,std::vector<SPOINT>& vecResult)
{
	bool bFlag = false;
	if (nCount < 4)
	{
		vecResult.resize(nCount);
		std::copy(poPoints,poPoints+nCount,vecResult.begin());
		return bFlag;
	}

	//判断是否是开曲线，然后补点
	std::vector<SPOINT> vecCopyPoints;
	vecCopyPoints.resize(nCount+2);
	
	std::copy(poPoints,+poPoints+nCount,vecCopyPoints.begin()+1);
	if (fabs(poPoints[0].x - poPoints[nCount-1].x) > 0.0000000000001 && fabs(poPoints[0].y - poPoints[nCount-1].y) > 0.0000000000001)
	{
		nCount += 2;
		vecCopyPoints[0].x = 3*(vecCopyPoints[1].x - vecCopyPoints[2].x) + vecCopyPoints[3].x;
		vecCopyPoints[0].y = 3*(vecCopyPoints[1].y - vecCopyPoints[2].y) + vecCopyPoints[3].y;

		vecCopyPoints[nCount-1].x = 3*(vecCopyPoints[nCount-2].x - vecCopyPoints[nCount-3].x) + vecCopyPoints[nCount-4].x;
		vecCopyPoints[nCount-1].y = 3*(vecCopyPoints[nCount-2].y - vecCopyPoints[nCount-3].y) + vecCopyPoints[nCount-4].y;
	}

	else 
	{
		nCount += 2;
		vecCopyPoints[0] = vecCopyPoints[nCount-3];
		vecCopyPoints[nCount-1] = vecCopyPoints[2];
	}

	vecResult.clear();
	for (int nIndex = 1; nIndex < nCount-2; nIndex ++)
	{
		//计算参数方程的参数
		double dbP0 = vecCopyPoints[nIndex].x;
		double dbP1 = 4 * vecCopyPoints[nIndex+1].x -vecCopyPoints[nIndex+1].x - 5*vecCopyPoints[nIndex-1].x;
		double dbP2 = (8 * vecCopyPoints[nIndex-1].x - 18 * vecCopyPoints[nIndex].x + 
			12 * vecCopyPoints[nIndex+1].x - 2 * vecCopyPoints[nIndex+2].x);
		double dbP3 = -4 * (vecCopyPoints[nIndex-1].x - 3*vecCopyPoints[nIndex].x +
			3*vecCopyPoints[nIndex+1].x - vecCopyPoints[nIndex+2].x);

		double dbQ0 = vecCopyPoints[nIndex].y;
		double dbQ1 = 4 * vecCopyPoints[nIndex+1].y -vecCopyPoints[nIndex+1].y - 5*vecCopyPoints[nIndex-1].y;
		double dbQ2 = (8 * vecCopyPoints[nIndex-1].y - 18 * vecCopyPoints[nIndex].y + 
			12 * vecCopyPoints[nIndex+1].y - 2 * vecCopyPoints[nIndex+2].y);
		double dbQ3 = -4 * (vecCopyPoints[nIndex-1].y - 3*vecCopyPoints[nIndex].y +
			3*vecCopyPoints[nIndex+1].y - vecCopyPoints[nIndex+2].y);

		//确定插值的步长
		double dbFlag = 3*dbP3*(2 * dbP2 + 3*dbP3) + 3*dbQ3*(2*dbQ2 + 3*dbQ3);
		double dbMaxFlag = 4*(dbP2*dbP2 + dbQ2*dbQ2);
		if (dbFlag > 0)
		{
			dbMaxFlag += 4*dbFlag;
		}

		double dbT = 0;
		if (dbMaxFlag > 0)
		{
			dbT = 2*sqrt(2*dbEps)/dbMaxFlag;
		}
		else
			dbT = 0.49999;

		dbT = 0.5;

		double dbCurT = 0;
		while (dbCurT < 0.51)
		{
			SPOINT pt;
			pt.x = dbP0 + dbP1*dbCurT + dbP2*dbCurT*dbCurT + dbP3*dbCurT*dbCurT*dbCurT;
			pt.y = dbQ0 + dbQ1*dbCurT + dbQ2*dbCurT*dbCurT + dbQ3*dbCurT*dbCurT*dbCurT;
			vecResult.push_back(pt);

			dbCurT += dbT;
		}
	}
}
