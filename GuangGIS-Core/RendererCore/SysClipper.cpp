#include "SysClipper.h"
#include "SysEnvelope.h"
#include "SysCoordinate.h"



//以下是内部使用函数

namespace SysDraw
{

	//线段求交测试
	extern bool ClipTest(double p, double q, double *u1, double *u2);

	//梁友栋算法
	extern bool LiangLineClip(double *x1, double *y1, double *x2, double *y2
		, const SysEnvelope& rect,SysCoordinate &p1,SysCoordinate &p2);

	//判断两条线段是否相交
	extern bool IsLineIntersect(const SysCoordinate& p1,const SysCoordinate& p2
		,const SysCoordinate& q1,const SysCoordinate& q2);

	//判断线段和裁剪矩形是否相交
	extern bool IsLineIntersectWithRect(const SysCoordinate& p1,const SysCoordinate& p2,SysEnvelope& env);

	extern bool IsLineIntersectWithRect(double x1,double y1,double x2,double y2,SysEnvelope& env);

}

namespace SysDraw
{

	SysClipper::SysClipper(void)
	{
	}

	SysClipper::~SysClipper(void)
	{
	}

	void SysClipper::GetValuePoint( SysCoordinate * poPoints, int nPointCount, double dfDistance, SysCoordinate * pPoint ,double *pAngle ) 
	{
		if( dfDistance < 0 )
		{
			pPoint->SetX(poPoints[0].x);
			pPoint->SetY(poPoints[0].y);
			*pAngle = 0;
			return;
		}

		double  dfLength = 0;

		for( int i = 0; i < nPointCount-1; i++ )
		{
			double dfDeltaX = poPoints[i+1].x - poPoints[i].x;
			double dfDeltaY = poPoints[i+1].y - poPoints[i].y;
			double dfSegLength = sqrt(dfDeltaX*dfDeltaX + dfDeltaY*dfDeltaY);

			if (dfSegLength > 0)
			{
				if( (dfLength <= dfDistance) && ((dfLength + dfSegLength) >= dfDistance) )
				{
					double dfRatio = (dfDistance - dfLength) / dfSegLength;

					pPoint->SetX( poPoints[i].x * (1 - dfRatio)
						+ poPoints[i+1].x * dfRatio );
					pPoint->SetY( poPoints[i].y * (1 - dfRatio)
						+ poPoints[i+1].y * dfRatio );

					double dbAng = atan2(dfDeltaY, dfDeltaX);
					if (dbAng < 0)
					{
						dbAng += M_PI*2;
					}

					dbAng *= RADTODEG;
					*pAngle = dbAng;

					return;
				}

				dfLength += dfSegLength;
			}
		}

		pPoint->SetX(poPoints[nPointCount-1].x);
		pPoint->SetY(poPoints[nPointCount-1].y);
		*pAngle = 0;
	}

	bool SysClipper::PolylineClip(SysCoordinate* poPoints,int nCount,SysEnvelope& rect,std::vector<int> &vecIndex,std::vector<SysCoordinate> &outPoints)
	{
		if (NULL == poPoints || nCount < 1)
		{
			return false;
		}

		int nLineCount = 0;
		if (rect.Contains(poPoints[0].x,poPoints[0].y))
		{
			nLineCount = 1;
			vecIndex.push_back(0);
			outPoints.push_back(poPoints[0]);
		}

		//裁剪后更新的坐标
		SysCoordinate p1;
		SysCoordinate p2;

		SysEnvelope lineEnv;
		for (int i = 0; i < nCount-1; i ++)
		{
			double minx = min(poPoints[i].x,poPoints[i+1].x);
			double maxx = max(poPoints[i].x,poPoints[i+1].x);
			double miny = min(poPoints[i].y,poPoints[i+1].y);
			double maxy = max(poPoints[i].y,poPoints[i+1].y);
			lineEnv.Init(minx,maxx,miny,maxy);
			SysEnvelope resultEnv;			
			if (!rect.Intersection(lineEnv,resultEnv))	//如果线段在裁剪矩形外，忽略此线段
			{
				continue;
			}

			else if (rect.Contains(lineEnv))	//如果线段在裁剪矩形内，直接保留原来的点，不需要裁剪
			{
				outPoints.push_back(poPoints[i]);
				/*if (i == nCount-2)
				{
				outPoints.push_back(poPoints[i+1]);
				}*/
			}

			else if (rect.Intersects(lineEnv))	//相交则进一步判断
			{
				if (!rect.Contains(poPoints[i].x,poPoints[i].y)
					&& rect.Contains(poPoints[i+1].x,poPoints[i+1].y))		//进点
				{
					nLineCount ++;
					vecIndex.push_back(outPoints.size());
					LiangLineClip(&poPoints[i].x,&poPoints[i].y,&poPoints[i+1].x,&poPoints[i+1].y,rect,p1,p2);
					outPoints.push_back(p1);
				}

				else if (rect.Contains(poPoints[i].x,poPoints[i].y)
					&& !rect.Contains(poPoints[i+1].x,poPoints[i+1].y))		//出点
				{
					LiangLineClip(&poPoints[i].x,&poPoints[i].y,&poPoints[i+1].x,&poPoints[i+1].y,rect,p1,p2);
					outPoints.push_back(p1);
					outPoints.push_back(p2);
				}

				else if (!rect.Contains(poPoints[i].x,poPoints[i].y)
					&& !rect.Contains(poPoints[i+1].x,poPoints[i+1].y)
					&& IsLineIntersectWithRect(poPoints[i],poPoints[i+1],rect))		//与裁剪矩形不相交
				{
					continue;
				}

				else if (!rect.Contains(poPoints[i].x,poPoints[i].y)
					&& !rect.Contains(poPoints[i+1].x,poPoints[i+1].y)
					&& !IsLineIntersectWithRect(poPoints[i],poPoints[i+1],rect))		//与裁剪矩形相交
				{
					nLineCount ++;
					vecIndex.push_back(outPoints.size());
					LiangLineClip(&poPoints[i].x,&poPoints[i].y,&poPoints[i+1].x,&poPoints[i+1].y,rect,p1,p2);
					outPoints.push_back(p1);
					outPoints.push_back(p2);
				}	
			}	
		}
		return false;
	}

	bool SysClipper::PolylineClipEx( double* poCoords, int nCount,SysEnvelope& rect, std::vector<int> &vecIndex, std::vector<double> &outPoints )
	{
		if ( nCount < 1)
		{
			return false;
		}

		int nLineCount = 0;
		if (rect.Contains(poCoords[0],poCoords[1]))
		{
			nLineCount = 1;
			vecIndex.push_back(0);
			outPoints.push_back(poCoords[0]);
			outPoints.push_back(poCoords[1]);
		}

		//裁剪后更新的坐标
		SysCoordinate p1;
		SysCoordinate p2;

		SysEnvelope lineEnv;
		for (int i = 0; i < nCount-1; i ++)
		{
			double x1 = poCoords[2*i];
			double y1 = poCoords[2*i+1];
			double x2 = poCoords[2*(i+1)];
			double y2 = poCoords[2*(i+1)+1];

			double minx = min(x1,x2);
			double maxx = max(x1,x2);
			double miny = min(y1,y2);
			double maxy = max(y1,y2);

			lineEnv.Init(minx,maxx,miny,maxy);

			SysEnvelope resultEnv;			
			if (!rect.Intersection(lineEnv,resultEnv))	//如果线段在裁剪矩形外，忽略此线段
			{
				continue;
			}

			else if (rect.Contains(lineEnv))	//如果线段在裁剪矩形内，直接保留原来的点，不需要裁剪
			{
				outPoints.push_back(poCoords[2*i]);
				outPoints.push_back(poCoords[2*i+1]);
				/*if (i == nCount-2)
				{
				outPoints.push_back(poPoints[i+1]);
				}*/
			}

			else if (rect.Intersects(lineEnv))	//相交则进一步判断
			{
				if (!rect.Contains(x1,y1)
					&& rect.Contains(x2,y2))		//进点
				{
					nLineCount ++;
					vecIndex.push_back(outPoints.size());
					LiangLineClip(&x1,&y1,&x2,&y2,rect,p1,p2);
					outPoints.push_back(p1.x);
					outPoints.push_back(p1.y);
				}

				else if (rect.Contains(x1,y1)
					&& !rect.Contains(x2,y2))		//出点
				{
					LiangLineClip(&x1,&y1,&x2,&y2,rect,p1,p2);
					outPoints.push_back(p1.x);
					outPoints.push_back(p1.y);
					outPoints.push_back(p2.x);
					outPoints.push_back(p2.y);
				}

				else if (!rect.Contains(x1,y1)
					&& !rect.Contains(x2,y2)
					&& IsLineIntersectWithRect(x1,y1,x2,y2,rect))		//与裁剪矩形不相交
				{
					continue;
				}

				else if (!rect.Contains(x1,y1)
					&& !rect.Contains(x2,y2)
					&& !IsLineIntersectWithRect(x1,y1,x2,y2,rect))		//与裁剪矩形相交
				{
					nLineCount ++;
					vecIndex.push_back(outPoints.size());
					LiangLineClip(&x1,&y1,&x2,&y2,rect,p1,p2);
					outPoints.push_back(p1.x);
					outPoints.push_back(p1.y);
					outPoints.push_back(p2.x);
					outPoints.push_back(p2.y);
				}	
			}	
		}
		return false;
	}

	double SysClipper::GetLineStringLength( SysCoordinate * poPoints, int nPointCount )
	{
		double dbLen = 0;

		for (int i = 0; i < nPointCount-1; i ++)
		{
			double dfDeltaX = poPoints[i+1].x - poPoints[i].x;
			double dfDeltaY = poPoints[i+1].y - poPoints[i].y;
			double dfSegLength = sqrt(dfDeltaX*dfDeltaX + dfDeltaY*dfDeltaY);
			dbLen += dfSegLength;
		}

		return dbLen;
	}

	SysGeometry* SysClipper::PolygonClip( SysPolygon* pSysPolygon,SysEnvelope& visEnv )
	{
		GeoPolygon* pPolygon = (GeoPolygon *)pSysPolygon;
		int nOutRingPtCnt = pPolygon->GetExteriorRing()->GetNumPoint();
		int nRings = pPolygon->GetNumInteriorRings();

		//先判定本多边形和可见范围是否相交
		SysEnvelope env;
		GeoEnvelope ogrEnv;
		pPolygon->getEnvelope(&ogrEnv);
		memcpy(&env,&ogrEnv,sizeof(env));

		SysGeometry *pReturnGeom = NULL;

		if (/*visEnv.Contains(env)*/1)		//在可见范围内直接返回本多边形
		{
			GeoPolygon *pClone = (GeoPolygon *)pPolygon->clone();
			pReturnGeom = (SysGeometry *)pClone;
			return pReturnGeom;
		}

		else if (visEnv.Intersects(env))	//相交的话需要做裁剪
		{
			gpc_polygon gpc_poly;
			gpc_poly.num_contours = /*nRings + */1;
			gpc_poly.contour = NULL;
			int bHole = /*nRings +*/ 1;
			gpc_poly.hole = &bHole;

			//构造裁剪多边形
			gpc_vertex_list vertexClip;
			vertexClip.num_vertices = 5;
			gpc_vertex vertexPointClip[5];
			vertexPointClip[0].x = visEnv.minX;
			vertexPointClip[0].y = visEnv.minY;
			vertexPointClip[1].x = visEnv.minX;
			vertexPointClip[1].y = visEnv.maxY;
			vertexPointClip[2].x = visEnv.maxX;
			vertexPointClip[2].y = visEnv.maxY;
			vertexPointClip[3].x = visEnv.maxX;
			vertexPointClip[3].y = visEnv.minY;
			vertexPointClip[4].x = visEnv.minX;
			vertexPointClip[4].y = visEnv.minY;
			vertexClip.vertex = vertexPointClip;
			gpc_polygon gpc_poly_clip;
			gpc_poly_clip.num_contours = 1;
			gpc_poly_clip.contour = &vertexClip;
			bHole = 1;
			gpc_poly_clip.hole = &bHole;

			GeoLineString* pRing = pPolygon->getExteriorRing();
			if (pRing != NULL)
			{
				OGRRawPoint *poRawPoints = new OGRRawPoint[nOutRingPtCnt];
				pRing->getPoints(poRawPoints);

				gpc_vertex_list *pVertexList = new gpc_vertex_list;
				pVertexList->num_vertices = nOutRingPtCnt;
				pVertexList->vertex = new gpc_vertex[nOutRingPtCnt];
				memcpy(pVertexList->vertex,poRawPoints,sizeof(OGRRawPoint)*nOutRingPtCnt);

				//gpc_poly.contour[0] = *pVertexList;
				gpc_poly.contour = pVertexList;

				if (NULL != poRawPoints)
				{
					delete []poRawPoints;
					poRawPoints = NULL;
				}
			}

			for (int i = 0; i < /*nRings*/0; i ++)
			{
				pRing = pPolygon->getInteriorRing(i);

				OGRRawPoint *poRawPoints = new OGRRawPoint[nOutRingPtCnt];
				pRing->getPoints(poRawPoints);

				gpc_vertex_list *pVertexList = new gpc_vertex_list;
				pVertexList->num_vertices = nOutRingPtCnt;
				pVertexList->vertex = new gpc_vertex[nOutRingPtCnt];
				memcpy(pVertexList->vertex,poRawPoints,sizeof(OGRRawPoint)*nOutRingPtCnt);

				gpc_poly.contour[i+1] = *pVertexList;

				if (NULL != poRawPoints)
				{
					delete []poRawPoints;
					poRawPoints = NULL;
				}
			}

			//开始裁剪
			gpc_polygon *out_poly = new gpc_polygon;
			gpc_polygon_clip(GPC_INT,&gpc_poly,&gpc_poly_clip,out_poly);

			//组装几何体

			OGRMultiPolygon* poMulReturn = (OGRMultiPolygon *)GeoGeometryFactory::createGeometry(wkbMultiPolygon);

			if (out_poly != NULL && out_poly->contour != NULL)
			{
				for (int k = 0; k < out_poly->num_contours; k ++)
				{
					OGRRawPoint *poRawPoints = (OGRRawPoint*)out_poly->contour[k].vertex;
					int nCounts = out_poly->contour[k].num_vertices;

					GeoPolygon* poPolygon = (GeoPolygon *)GeoGeometryFactory::createGeometry(wkbPolygon);
					GeoLineString *poRing = (GeoLineString *)GeoGeometryFactory::createGeometry(wkbLineString);
					poRing->setPoints(nCounts,poRawPoints);
					//poRing->addPoint(poRawPoints[0].x,poRawPoints[0].y);
					poPolygon->addRing(poRing);
					poMulReturn->addGeometry(poPolygon);
				}

			}

			pReturnGeom = (SysGeometry *)poMulReturn;

			//gpc_free_polygon(out_poly);
		}

		return pReturnGeom;
	}
}

//内部使用函数

namespace SysDraw
{

	bool ClipTest(double p, double q, double *u1, double *u2)
	{
		double r;
		if(p < 0)// 计算rk= qk/pk，将0和各rk的值中最大值赋给t1;
		{
			r = q/p;
			if(r > *u2)
			{
				return false;
			}
			else if(r > *u1)
			{
				*u1 = r;
				return true;
			}
		}
		else if(p > 0) // 计算rk= qk/pk，将1和各rk的值中最小值赋给t2;
		{
			r = q/p;
			if(r < *u1)
				return false;
			else if(r < *u2)
			{
				*u2 = r;
				return true;
			}
		}
		else if(q < 0)
		{
			return false;
		}
		return true;
	}

	bool LiangLineClip(double *x1, double *y1, double *x2, double *y2, 
		const SysEnvelope& rect,SysCoordinate &p1,SysCoordinate &p2)
	{
		double dx,dy,u1,u2;
		//初始化u1,u2
		u1 = 0;
		u2 = 1;
		dx = *x2 - *x1;
		dy = *y2 - *y1;

		if (ClipTest(-dx,*x1-rect.minX,&u1,&u2))	//左
		{
			if(ClipTest(dx,rect.maxX-*x1,&u1,&u2))   //右
			{
				if (ClipTest(-dy,*y1-rect.minY,&u1,&u2))	//下
				{
					if (ClipTest(dy,rect.maxY-*y1,&u1,&u2))	 //上
					{
						//更新线段坐标的值
						p1.x = *x1 + u1 * dx;
						p1.y = *y1 + u1 * dy;
						p2.x = *x1 + u2 * dx;
						p2.y = *y1 + u2 * dy;

						return true;
					}
				}
			}
		}

		return false;
	}

	bool IsLineIntersect(const SysCoordinate& p1,const SysCoordinate& p2
		,const SysCoordinate& q1,const SysCoordinate& q2)
	{
		//首先判断两个线段的MBR是否相交
		SysEnvelope env1;
		env1.Init(min(p1.x,p2.x),max(p1.x,p2.x),min(p1.y,p2.y),max(p1.y,p2.y));
		SysEnvelope env2;
		env1.Init(min(q1.x,q2.x),max(q1.x,q2.x),min(q1.y,q2.y),max(q1.y,q2.y));
		bool bIsIntersect = env1.Intersects(env2);
		if (!bIsIntersect)
		{
			return false;
		}

		//跨立实验
		SysUtility::Vector2 q1p1(q1.x,q1.y,p1.x,p1.y);
		SysUtility::Vector2 q1p2(q1.x,q1.y,p2.x,p2.y);
		SysUtility::Vector2 q1q2(q1.x,q1.y,q2.x,q2.y);
		SysUtility::Vector2 p1q1(p1.x,p1.y,q1.x,q1.y);
		SysUtility::Vector2 p1q2(p1.x,p1.y,q2.x,q2.y);
		SysUtility::Vector2 p1p2(p1.x,p1.y,p2.x,p2.y);

		double dbFlag1 = SysUtility::Vector2::crossProduct(q1p1,q1q2)*SysUtility::Vector2::crossProduct(q1q2,q1p2);
		double dbFalg2 = SysUtility::Vector2::crossProduct(p1q1,p1p2)*SysUtility::Vector2::crossProduct(p1p2,p1q2);

		if (dbFlag1 >= 0 && dbFalg2 >= 0)
		{
			return true;
		}
		return false;
	}

	bool IsLineIntersectWithRect(const SysCoordinate& p1,const SysCoordinate& p2,SysEnvelope& env)
	{
		SysCoordinate pt1;
		SysCoordinate pt2;

		pt1.x = env.minX;
		pt1.y = env.minY;
		pt2.x = env.minX;
		pt2.y = env.maxY;
		bool bFlag1 = IsLineIntersect(p1,p2,pt1,pt2);

		pt1.x = env.minX;
		pt1.y = env.maxY;
		pt2.x = env.maxX;
		pt2.y = env.maxY;
		bool bFlag2 = IsLineIntersect(p1,p2,pt1,pt2);

		pt1.x = env.maxX;
		pt1.y = env.maxY;
		pt2.x = env.maxX;
		pt2.y = env.minY;
		bool bFlag3 = IsLineIntersect(p1,p2,pt1,pt2);

		pt1.x = env.maxX;
		pt1.y = env.minY;
		pt2.x = env.minX;
		pt2.y = env.minY;
		bool bFlag4 = IsLineIntersect(p1,p2,pt1,pt2);
		if (!bFlag1 && !bFlag2 && !bFlag3 && !bFlag4)
		{
			return false;
		}
		else
		{
			return true;
		}
		return false;
	}

	bool IsLineIntersectWithRect( double x1,double y1,double x2,double y2,SysEnvelope& env )
	{
		SysCoordinate pt1;
		SysCoordinate pt2;

		SysCoordinate p1(x1,y1);
		SysCoordinate p2(x2,y2);

		pt1.x = env.minX;
		pt1.y = env.minY;
		pt2.x = env.minX;
		pt2.y = env.maxY;
		bool bFlag1 = IsLineIntersect(p1,p2,pt1,pt2);

		pt1.x = env.minX;
		pt1.y = env.maxY;
		pt2.x = env.maxX;
		pt2.y = env.maxY;
		bool bFlag2 = IsLineIntersect(p1,p2,pt1,pt2);

		pt1.x = env.maxX;
		pt1.y = env.maxY;
		pt2.x = env.maxX;
		pt2.y = env.minY;
		bool bFlag3 = IsLineIntersect(p1,p2,pt1,pt2);

		pt1.x = env.maxX;
		pt1.y = env.minY;
		pt2.x = env.minX;
		pt2.y = env.minY;
		bool bFlag4 = IsLineIntersect(p1,p2,pt1,pt2);
		if (!bFlag1 && !bFlag2 && !bFlag3 && !bFlag4)
		{
			return false;
		}
		else
		{
			return true;
		}
		return false;
	}

}
