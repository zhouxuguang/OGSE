/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2012
@ 文件名称	: GeoPolygon.cpp
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2012年3月23日
@ 内容摘要	: opengis中多边形的表达
@ 修改记录	:
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/

#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <string>
#include <limits>
#include "GeoEnvelope.h"
#include "GeoPoint.h"
#include "GeoLineString.h"
#include "GeoPolygon.h"

//class GeoLineString;


GeoPolygon::GeoPolygon(void)
{
	m_nRingCount = 0;
	m_papoRings = NULL;
}

GeoPolygon::GeoPolygon(const GeoPolygon& other)
{
	//nRingCount = 0;
	Empty();
	*this = other;
}

GeoPolygon& GeoPolygon::operator =(const GeoPolygon &other)
{
	if (this != &other)
	{
		Empty();
		int cnt = other.GetNumRings();
		GeoLineString** rings = other.GetRings();
		for (int i = 0; i < cnt; i ++)
		{
			AddRing(rings[i]);
		}
	}

	return *this;
}

GeoPolygon::~GeoPolygon(void)
{
	Empty();
}

//多边形的维数
int GeoPolygon::GetDimension() const
{
	return 2;
}

//清空多边形
bool GeoPolygon::Empty()
{
	if (m_papoRings != NULL)
	{
		for (int i = 0; i < m_nRingCount; i ++)
		{
			delete m_papoRings[i];
		}

		free(m_papoRings);
		m_papoRings = NULL;
	}
	m_nRingCount = 0;
	return true;
}

//获取几何体类型
GeoWkbGeometryType GeoPolygon::GetGeometryType() const
{
	return gWkbPolygon;
}

const char* GeoPolygon::GetGeometryName() const
{
	return "POLYGON";
}

void GeoPolygon::GetEnvelope(GeoEnvelope *env) const
{
	if (NULL == env)
	{
		return;
	}
 	if (m_nRingCount == 0)
 	{
 		return;
 	}
	m_papoRings[0]->GetEnvelope(env);
	GeoEnvelope ogrRingEnv;
	for (int iRing = 1; iRing < GetNumRings(); iRing ++)
	{
		m_papoRings[iRing]->GetEnvelope(&ogrRingEnv);
		if (env->minX > ogrRingEnv.minX)
		{
			env->minX = ogrRingEnv.minX;
		}
		if (env->minY > ogrRingEnv.minY)
		{
			env->minY = ogrRingEnv.minY;
		}
		if (env->maxX < ogrRingEnv.maxX)
		{
			env->maxX = ogrRingEnv.maxX;
		}
		if (env->maxY < ogrRingEnv.maxY)
		{
			env->maxY = ogrRingEnv.maxY;
		}
	}
}

void GeoPolygon::Move(double dx,double dy)
{
	for (int i = 0; i < GetNumRings(); i ++)
	{
		m_papoRings[i]->Move(dx,dy);
	}
}

double GeoPolygon::GetArea() const
{
	//
	double dArea = 0.0;

	if (GetExteriorRing() != NULL)
	{
		int iRing = 0;
		dArea = GetExteriorRing()->GetArea();

		for (; iRing < GetNumInteriorRings(); iRing ++)
		{
			dArea -= GetInteriorRing(iRing)->GetArea();
		}
	}
	return dArea;
}

double GeoPolygon::GetLength() const
{
	double dLnegth = 0.0;
	int iRing = 0;
	for (; iRing < m_nRingCount; iRing ++)
	{
		dLnegth += m_papoRings[iRing]->GetLength();
	}

	return dLnegth;
}

std::string GeoPolygon::AsText()
{
	std::stringstream ss;
	int precise = std::numeric_limits<double>::digits10;
	ss.precision(precise);
	ss<<"POLYGON(";
	int ringCount = GetNumRings();

	if (ringCount == 0)
	{
		return "";
	}

	//如果是一个环，外多边形
	else if (ringCount > 0)
	{
		GeoLineString** popoRings = GetRings();
		std::string strOutRing;
		for (int i = 0; i < ringCount-1; i ++)
		{
			strOutRing = popoRings[i]->AsText();
			strOutRing.substr(10);
			ss<<strOutRing;
			ss<<",";
		}
		
		//将最后一个也加进来
		strOutRing = m_papoRings[ringCount-1]->AsText();
		strOutRing.substr(10);
		ss<<strOutRing;
	}

	ss<<")";
	return ss.str();
}

int GeoPolygon::WkbByteSize() const
{
	int  nSize = 9;

	for( int i = 0; i < m_nRingCount; i++ )
	{
		nSize += m_papoRings[i]->WkbByteSize();
	}

	return nSize;
}

int GeoPolygon::AsBinary( GeoByteOrder eByteOrder,unsigned char *poData ) const
{
	int         nOffset;
	int         b3D = m_nCoordDimension == 3;

	
	/*     设置字节序       */
	//poData[0] = DB2_V72_UNFIX_BYTE_ORDER((unsigned char) eByteOrder);

	/*      设置几何类型    */
	unsigned int nGType = GetGeometryType();

	/*if( eByteOrder == wkbNDR )
		nGType = CPL_LSBWORD32( nGType );
	else
		nGType = CPL_MSBWORD32( nGType );*/

	memcpy( poData + 1, &nGType, 4 );

	/*      复制裸数据          */
	/*if( OGR_SWAP( eByteOrder ) )
	{
		int     nCount;

		nCount = CPL_SWAP32( nRingCount );
		memcpy( pabyData+5, &nCount, 4 );
	}
	else*/
	{
		memcpy( poData+5, &m_nRingCount, 4 );
	}

	nOffset = 9;

	/*      序列化每一个线串                                    */
	for( int iRing = 0; iRing < m_nRingCount; iRing++ )
	{
		m_papoRings[iRing]->AsBinary( eByteOrder,poData + nOffset );

		//nOffset += papoRings[iRing]->_WkbSize(b3D);
	}

	return 1;
}

GeoGeometry* GeoPolygon::Clone() const
{
	GeoPolygon* poNewPolygon = new GeoPolygon;

	poNewPolygon->SetSpatialReference( GetSpatialReference() );

	for( int i = 0; i < m_nRingCount; i++ )
	{
		poNewPolygon->AddRing( m_papoRings[i] );
	}

	return poNewPolygon;
}

int GeoPolygon::GetNumRings() const
{
	return m_nRingCount;
}

int GeoPolygon::GetNumInteriorRings() const
{
	/*如果环的个数<=1，则只有一个外环，没有岛*/
	if (m_nRingCount <= 1)
	{
		return 0;
	}
	else
	{
		return (m_nRingCount-1);
	}
}

GeoLineString* GeoPolygon::GetExteriorRing()
{
	if (m_nRingCount > 0)
	{
		return m_papoRings[0];
	}
	else
	{
		return NULL;
	}

	//return const_cast<GeoLineString*>(this->GetExteriorRing());
}

const GeoLineString* GeoPolygon::GetExteriorRing() const
{
	if (m_nRingCount > 0)
	{
		return m_papoRings[0];
	}
	else
	{
		return NULL;
	}
}

GeoLineString* GeoPolygon::GetInteriorRing(int iRing)
{
	if (iRing < 0 || iRing >= m_nRingCount - 1)
	{
		return NULL;
	}
	else
	{
		return m_papoRings[iRing+1];
	}
}

const GeoLineString* GeoPolygon::GetInteriorRing(int iRing)  const
{
	if (iRing < 0 || iRing >= m_nRingCount-1)
	{
		return NULL;
	}
	else
	{
		return m_papoRings[iRing+1];
	}
}

void GeoPolygon::AddRing(const GeoLineString *poNewRing)
{
	assert(poNewRing != NULL);
	if (m_papoRings == NULL && m_nRingCount == 0)
	{
		m_papoRings = (GeoLineString**)malloc(sizeof(GeoLineString*));
	}

	else if (m_papoRings != NULL && m_nRingCount > 0)
	{
		m_papoRings = (GeoLineString**)realloc(m_papoRings,sizeof(GeoLineString*) * (m_nRingCount + 1));
	}

	//将最后一个元素赋值为poNewRing
	m_papoRings[m_nRingCount] = new GeoLineString( poNewRing );
	assert(m_papoRings);
	m_nRingCount ++;
}

GeoLineString* GeoPolygon::GetRing(int nIndex) const
{
	if (nIndex < 0|| nIndex >= m_nRingCount)
	{
		return NULL;
	}

	return m_papoRings[nIndex];
}

int GeoPolygon::Centroid( GeoPoint * poPoint ) const
{
	if (m_papoRings[0]->GetNumPoint() < 3)
	{
		return 0;	//不构成多边形
	}

	GeoEnvelope bounds;
	GetEnvelope(&bounds);
	poPoint->SetX((bounds.minX + bounds.maxX)/2);
	poPoint->SetY((bounds.minY + bounds.maxY)/2);

	int i = 0, j = 0;
	double ai = 0,atmp = 0, xtmp = 0, ytmp = 0;
	for (i = m_papoRings[0]->GetNumPoint()-1,j = 0;
		j < m_papoRings[0]->GetNumPoint(); i = j, j ++)
	{
		//ai = papoRings[0]->GetPoint(j,poPoint);
	}

	return 1;
}

void GeoPolygon::CloseRings()
{
	for (int iRing = 0; iRing < m_nRingCount; iRing ++)
	{
		m_papoRings[iRing]->CloseRings();
	}
}

//射线和线段的关系 :相交返回1，不相交返回0，射线起点在线段上返回-1
int IsIntersectAnt(double x,double y,double X1,double Y1,double X2,double Y2)
{
	//计算线段的最小和最大坐标值
	double minX,maxX,minY,maxY;
	minX = X1;
	maxX = X2;
	if (minX > maxX)
	{
		minX = X2;
		maxX = X1;
	}
	minY = Y1;
	maxY = Y2;
	if (minY > maxY)
	{
		minY = Y2;
		maxY = Y1;
	}

	//射线与边无交点的快速判断
	if (y<minY || y>maxY || x<minX)
	{
		return 0;
	}

	//如果是水平线段，在线段上返回-1，否则返回0
	if (fabs(maxY - minY) < eps)
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
	if (fabs(x-x0)< eps)
	{
		return -1;
	}
	//穿过下端点也不计数
	if (fabs(y-minY) < eps)
	{
		return 0;
	}
	return 1;

}

int GeoPolygon::PointInPolygon(const GeoPoint& poPoint)
{
	//如果点不在多边形的最小外接矩形中，则一定不在多边形内
	GeoEnvelope env;
	GetEnvelope(&env);
	if (!env.Contains(poPoint.GetX(),poPoint.GetY()))
	{
		return 0;
	}

	//计算该点向左方向的射线与各个边的交点个数
	int nCount = 0;
	double X = poPoint.GetX();
	double Y = poPoint.GetY();
	int nFlag = 0;
	GeoLineString *exteriorRing = GetExteriorRing();
	exteriorRing->CloseRings();
	for (int i = 0; i < exteriorRing->GetNumPoint()-1; i ++)
	{
		nFlag = IsIntersectAnt(X,Y,exteriorRing->GetX(i),exteriorRing->GetY(i),
			exteriorRing->GetX(i+1),exteriorRing->GetY(i+1));
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

int GeoPolygon::PolygonInRect(GeoEnvelope &env)
{
	//先求出多边形的最小外白矩形
	GeoEnvelope envPolygon(0,0,0,0);
	GetEnvelope(&envPolygon);

	return envPolygon.Intersects(env);
}
