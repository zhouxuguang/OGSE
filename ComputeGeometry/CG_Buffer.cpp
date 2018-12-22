#include "CG_Common.h"
#include "CG_Buffer.h"
#include "CG_Angle.h"
#include "CG_Vector2.h"

void LineBuffer(SPOINT* poPoints,int nPointCount,double dbBuffer,std::vector<SPOINT>& vecResult)
{
	std::vector<SPOINT> vecLeftBuf;			//左侧缓冲区
	std::vector<SPOINT> vecRightBuf;		//右侧缓冲区

	double dbAng1 = 0;
	double dbAng2 = 0;

	//开始点用圆弧弥合
	CG_Point ptBegin;
	CG_Point ptEnd;
	dbAng1 = CG_Angle::angle(poPoints[0],poPoints[1]);
	dbAng2 = dbAng1;
	ptBegin.x = poPoints[0].x+dbBuffer*cos(dbAng1+M_PI_2+M_PI);
	ptBegin.y = poPoints[0].y+dbBuffer*sin(dbAng1+M_PI_2+M_PI);
	ptEnd.x = poPoints[0].x+dbBuffer*cos(dbAng2+M_PI_2);
	ptEnd.y = poPoints[0].y+dbBuffer*sin(dbAng2+M_PI_2);

	//顺时针方向圆环拟合
	double dbThea = acos(1.0-1.0/30.0);
	std::vector<CG_Point> vecArcPoints;
	ArcCycle(poPoints[0].x,poPoints[0].y,dbBuffer,dbThea,true,
		dbAng1-M_PI_2,dbAng2-M_PI_2-M_PI,vecArcPoints);

	//取出第一个点作为缓冲区边界的闭合点
	SPOINT ptClose = vecArcPoints.front();

	size_t nIndex = 0;
	for (;nIndex < vecArcPoints.size(); nIndex ++)
	{
		vecLeftBuf.push_back(vecArcPoints[nIndex]);
	}

	//从第二点开始每三个点为一个处理单元
	int i = 1;
	for (;i < nPointCount-1;i ++)
	{
		//计算与X轴正方向的方向角
		dbAng1 = CG_Angle::angle(poPoints[i],poPoints[i-1]);
		dbAng2 = CG_Angle::angle(poPoints[i],poPoints[i+1]);
		double xp = 0;
		double yp = 0;
		xp = dbBuffer*(cos(dbAng1)+cos(dbAng2))/sin(dbAng1-dbAng2);
		yp = dbBuffer*(sin(dbAng1)+sin(dbAng2))/sin(dbAng1-dbAng2);

		dbAng1 = CG_Angle::angle(poPoints[i-1],poPoints[i]);
		dbAng2 = CG_Angle::angle(poPoints[i],poPoints[i+1]);

		//计算矢量叉积，判断前后三点时候为顺时针或逆时针
		CG_Vector2 vec1(poPoints[i-1],poPoints[i]);
		CG_Vector2 vec2(poPoints[i],poPoints[i+1]);
		double dbS = CG_Vector2::crossProduct(vec1,vec2);
		if (dbS >= 0)		//如果是逆时针，在右侧是凸点，在这一侧用圆弧拟合，凹点
		{
			SPOINT pt1(poPoints[i].x+xp,poPoints[i].y+yp);
			vecLeftBuf.push_back(pt1);

			//计算线段平行线上的点
			ptBegin.x = poPoints[i].x - dbBuffer*sin(dbAng1+dbAng2);
			ptBegin.y = poPoints[i].y + dbBuffer*cos(dbAng1+dbAng2);
			ptEnd.x = poPoints[i].x + dbBuffer*sin(dbAng2);
			ptEnd.y = poPoints[i].y - dbBuffer*cos(dbAng2);

			//逆时针方向圆环拟合
			double dbThea = acos(1.0-1.0/30.0);
			std::vector<CG_Point> vecArcPoints;
			ArcCycle(poPoints[i].x,poPoints[i].y,dbBuffer,dbThea,false,
				dbAng1-M_PI_2,dbAng2-M_PI_2,vecArcPoints);

			size_t nIndex = 0;
			//vecRightBuf.push_back(ptBegin);
			for (;nIndex < vecArcPoints.size(); nIndex ++)
			{
				vecRightBuf.push_back(vecArcPoints[nIndex]);
			}
			//vecRightBuf.push_back(ptEnd);
		}
		else if (dbS < 0)	//如果是顺时针，在左侧是凸点，在这一侧用圆弧拟合，凸点
		{
			SPOINT pt2(poPoints[i].x-xp,poPoints[i].y-yp);
			vecRightBuf.push_back(pt2);

			//计算线段平行线上的点
			ptBegin.x = poPoints[i].x + dbBuffer*sin(dbAng1+dbAng2);
			ptBegin.y = poPoints[i].y - dbBuffer*cos(dbAng1+dbAng2);
			ptEnd.x = poPoints[i].x - dbBuffer*sin(dbAng2);
			ptEnd.y = poPoints[i].y + dbBuffer*cos(dbAng2);

			//顺时针方向圆环拟合
			double dbThea = acos(1.0-1.0/30.0);
			std::vector<CG_Point> vecArcPoints;
			ArcCycle(poPoints[i].x,poPoints[i].y,dbBuffer,dbThea,true,
				dbAng1+M_PI_2,dbAng2+M_PI_2,vecArcPoints);

			size_t nIndex = 0;
			//vecLeftBuf.push_back(ptBegin);
			for (;nIndex < vecArcPoints.size(); nIndex ++)
			{
				vecLeftBuf.push_back(vecArcPoints[nIndex]);
			}
			//vecLeftBuf.push_back(ptEnd);
		}

	}

	//最后一个点用圆弧弥合
	dbAng1 = CG_Angle::angle(poPoints[nPointCount-2],poPoints[nPointCount-1]);
	dbAng2 = dbAng1;
	ptBegin.x = poPoints[nPointCount-1].x+dbBuffer*cos(dbAng1+M_PI_2);
	ptBegin.y = poPoints[nPointCount-1].y+dbBuffer*sin(dbAng1+M_PI_2);
	ptEnd.x = poPoints[nPointCount-1].x+dbBuffer*cos(dbAng2-M_PI_2);
	ptEnd.y = poPoints[nPointCount-1].y+dbBuffer*sin(dbAng2-M_PI_2);

	//逆时针方向圆环拟合
	dbThea = acos(1.0-1.0/30.0);
	vecArcPoints.clear();
	ArcCycle(poPoints[nPointCount-1].x,poPoints[nPointCount-1].y,dbBuffer,dbThea,true,
		dbAng1+M_PI_2,dbAng2-M_PI_2,vecArcPoints);

	nIndex = 0;
	for (;nIndex < vecArcPoints.size(); nIndex ++)
	{
		vecLeftBuf.push_back(vecArcPoints[nIndex]);
	}

	vecResult.resize(vecLeftBuf.size()/*+vecRightBuf.size()*/);
	std::copy(vecLeftBuf.begin(),vecLeftBuf.end(),vecResult.begin());
	for (int i = vecRightBuf.size()-1; i >= 0; i --)
	{
		vecResult.push_back(vecRightBuf[i]);
	}

	vecResult.push_back(ptClose);
}

void ArcCycle(double x0, double y0, double dbRadius, double dbThea,bool bClockWise,
			  double ang1, double ang2, std::vector<CG_Point>& vecResult)
{
	//计算逼近圆的多边形的边数和角度增量值
	int N = (int)fabs(ang2-ang1)/dbThea;
	if (bClockWise)		//顺时针角度递减
	{
		dbThea = -dbThea;
	}

	//开始循环计算每个顶点的坐标
	CG_Point ptPoint;
	int i = 0;
	while (i <= N+1)
	{
		double w = ang1 + i * dbThea;
		ptPoint.x = x0 + dbRadius*cos(w);
		ptPoint.y = y0 + dbRadius*sin(w);

		vecResult.push_back(ptPoint);
		i ++;
	}
}

