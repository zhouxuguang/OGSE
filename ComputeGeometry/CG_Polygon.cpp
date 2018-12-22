#include "CG_BaseCal.h"
#include "CG_Polygon.h"

//向Y轴作垂线，并指向左边
int IsIntersectAnt(double x,double y,double X1,double Y1,double X2,double Y2)
{
	//计算线段的最小和最大坐标值
	double minX,maxX,minY,maxY;

	minX = min(X1,X2);
	minY = min(Y1,Y2);
	maxX = max(X1,X2);
	maxY = max(Y1,Y2);

	//射线与边无交点的快速判断
	if (y<minY || y>maxY || x<minX)
	{
		return 0;
	}

	//如果是水平线段，在线段上返回-1，否则返回0
	if (fabs(maxY - minY) < EPS)
	{
		return (x >= minX && x <= maxX)? (-1):0;
	}

	//计算射线与边所在直线的交点的横坐标
	double x0 = X1 + (double)(y - Y1)*(X2 - X1)/(Y2 - Y1);

	//交点在射线右侧，则不相交
	if (x0 > x)
	{
		return 0;
	}
	//交点和射线起点相同
	if (fabs(x-x0)< EPS)
	{
		return -1;
	}
	//穿过下端点也不计数
	if (fabs(y-minY) < EPS)
	{
		return 0;
	}
	return 1;

}

int PointInPolygon(const SPOINT& poPoint,const SPOINT* poPoints,int nCnt,int &PtCount)
{
	//如果点不在多边形的最小外接矩形中，则一定不在多边形内
	CG_Envelope env;
	GetEnvelope(poPoints,nCnt,env);
	if (!env.IsPointInRect(poPoint.x,poPoint.y))
	{
		return 0;
	}

	//计算该点向左方向的射线与各个边的交点个数
	int nCount = 0;
	double X = poPoint.x;
	double Y = poPoint.y;
	int nFlag = 0;

	for (int i = 0; i < nCnt-1; i ++)
	{
		nFlag = IsIntersectAnt(X,Y,poPoints[i].x,poPoints[i].y,
			poPoints[i+1].x,poPoints[i+1].y);
		if (nCount < 0) 
		{
			return 2;	//点在边上
		}
		nCount += nFlag;
	}

	if (nCount % 2 == 1)   //点在多边形内
	{
		return 1;
	}
	else
		return 0;
}

int PointInPolyPolygon(const SPOINT& poPoint,const SPOINT* lpPoints,
				   const int *lpPolyCounts,int nCount)
{
	if (lpPolyCounts == NULL || lpPoints == NULL || nCount == 0)
	{
		return 0;
	}
	int nSumCount = 0;		//总交点个数
	int nPtCount = 0;		//每个多边形交点的个数

	/*PointInPolygon(poPoint,lpPoints,lpPolyCounts[0],nPtCount);
	lpPoints += lpPolyCounts[0];
	nSumCount += nPtCount;*/

	for (int i = 0; i < nCount; i ++)
	{
		PointInPolygon(poPoint,lpPoints,lpPolyCounts[i],nPtCount);
		lpPoints += lpPolyCounts[i];
		nSumCount += nPtCount;
	}

	if (nSumCount % 2 == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}

	return 0;
}

void PolyCentroid(const SPOINT* points,int nCount,double *xCentroid,double *yCentroid)
{
	assert(points != NULL);
	if (nCount < 3)
	{
		return;
	}

	int i = 0;
	int j = 0;
	double ai = 0;
	double aTmp = 0;
	double xTmp = 0;
	double yTmp = 0;

	for (i = nCount-1,j = 0; j < nCount; i = j,j ++)
	{
		ai = points[i].x * points[j].y - points[j].x * points[i].y;
		aTmp += ai;
		xTmp += (points[i].x + points[j].x) * ai;
		yTmp += (points[i].y + points[j].y) * ai;
	}

	if (aTmp != 0)
	{
		*xCentroid = xTmp / (3 * aTmp);
		*yCentroid = yTmp / (3 * aTmp);
	}
}

int IsPolygonSelfIntersect(const SPOINT* points,int nCount)
{
	//

	return 0;
}
