/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
@ �ļ�����	: GeoPolygon.cpp
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2012��3��23��
@ ����ժҪ	: opengis�ж���εı��
@ �޸ļ�¼	:
@ ��    ��		��    ��		�޸���		�޸�ժҪ

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

//����ε�ά��
int GeoPolygon::GetDimension() const
{
	return 2;
}

//��ն����
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

//��ȡ����������
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

	//�����һ������������
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
		
		//�����һ��Ҳ�ӽ���
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

	
	/*     �����ֽ���       */
	//poData[0] = DB2_V72_UNFIX_BYTE_ORDER((unsigned char) eByteOrder);

	/*      ���ü�������    */
	unsigned int nGType = GetGeometryType();

	/*if( eByteOrder == wkbNDR )
		nGType = CPL_LSBWORD32( nGType );
	else
		nGType = CPL_MSBWORD32( nGType );*/

	memcpy( poData + 1, &nGType, 4 );

	/*      ����������          */
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

	/*      ���л�ÿһ���ߴ�                                    */
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
	/*������ĸ���<=1����ֻ��һ���⻷��û�е�*/
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

	//�����һ��Ԫ�ظ�ֵΪpoNewRing
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
		return 0;	//�����ɶ����
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

//���ߺ��߶εĹ�ϵ :�ཻ����1�����ཻ����0������������߶��Ϸ���-1
int IsIntersectAnt(double x,double y,double X1,double Y1,double X2,double Y2)
{
	//�����߶ε���С���������ֵ
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

	//��������޽���Ŀ����ж�
	if (y<minY || y>maxY || x<minX)
	{
		return 0;
	}

	//�����ˮƽ�߶Σ����߶��Ϸ���-1�����򷵻�0
	if (fabs(maxY - minY) < eps)
	{
		return (x >= minX && x <= maxX)? (-1):0;
	}

	//���������������ֱ�ߵĽ���ĺ�����
	double x0 = X1 + (double)(y - Y1)*(X2 - X1)/(Y2 - Y1);
	
	//�����������Ҳ࣬���ཻ
	if (x0 > x)
	{
		return 0;
	}
	//��������������ͬ
	if (fabs(x-x0)< eps)
	{
		return -1;
	}
	//�����¶˵�Ҳ������
	if (fabs(y-minY) < eps)
	{
		return 0;
	}
	return 1;

}

int GeoPolygon::PointInPolygon(const GeoPoint& poPoint)
{
	//����㲻�ڶ���ε���С��Ӿ����У���һ�����ڶ������
	GeoEnvelope env;
	GetEnvelope(&env);
	if (!env.Contains(poPoint.GetX(),poPoint.GetY()))
	{
		return 0;
	}

	//����õ������������������ߵĽ������
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
			return 2;	//���ڱ���
		}
		nCount += nFlag;
	}

	if (nCount % 2 == 1)   //���ڶ������
	{
		return 1;
	}
	else
		return 0;
}

int GeoPolygon::PolygonInRect(GeoEnvelope &env)
{
	//���������ε���С��׾���
	GeoEnvelope envPolygon(0,0,0,0);
	GetEnvelope(&envPolygon);

	return envPolygon.Intersects(env);
}
