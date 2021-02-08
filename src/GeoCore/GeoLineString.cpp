
#include <assert.h>
#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <limits>
#include "GeoPoint.h"
#include "GeoCoordinate.h"
#include "GeoEnvelope.h"
#include "GeoLineString.h"

GeoLineString::GeoLineString(void)
{
	m_nPointCount = 0;
	m_paoPoints = NULL;
	//m_nCurMaxPointCount = 100;
	//m_paoPoints = (GeoCoordinate*)malloc(sizeof(GeoCoordinate)*100);
}

GeoLineString::GeoLineString( const GeoLineString * poSrcRing )
{
	//��ʼ��
	m_nPointCount = 0;
	m_paoPoints = NULL;

	if (poSrcRing == NULL)
	{
		return;
	}
	else
	{
		SetPointsNumber(poSrcRing->GetNumPoint());
		memcpy(m_paoPoints,poSrcRing->m_paoPoints,
			sizeof(GeoCoordinate) * GetNumPoint());
	}
}

GeoLineString::~GeoLineString(void)
{
// 	if (GetNumPoint() > 0 && m_paoPoints != NULL)
// 	{
// 		free(m_paoPoints);
// 		m_paoPoints = NULL;
// 	}
// 
// 	m_nPointCount = 0;

	Empty();
}

int GeoLineString::GetDimension() const
{
	return 1;
}

bool GeoLineString::Empty()
{
	SetPointsNumber(0);
	return true;//Ҫ����
}

GeoWkbGeometryType GeoLineString::GetGeometryType() const
{
	return gWkbLineString;
}

const char* GeoLineString::GetGeometryName() const
{
	return "LINESTRING";
}

void GeoLineString::GetEnvelope(GeoEnvelope *env) const
{
	assert(env != NULL);
	double minx,maxx,miny,maxy;
	int nPoint = GetNumPoint();
	if (nPoint == 0)
	{
		return;
	}
	minx = maxx = m_paoPoints[0].x;
	miny = maxy = m_paoPoints[0].y;
	for (int iPoint = 1; iPoint < nPoint; iPoint ++)
	{
		if (maxx < m_paoPoints[iPoint].x)
		{
			maxx = m_paoPoints[iPoint].x;
		}
		if (minx > m_paoPoints[iPoint].x)
		{
			minx = m_paoPoints[iPoint].x;
		}
		if (maxy < m_paoPoints[iPoint].y)
		{
			maxy = m_paoPoints[iPoint].y;
		}
		if (miny > m_paoPoints[iPoint].y)
		{
			miny = m_paoPoints[iPoint].y;
		}
	}

	env->minX = minx;
	env->maxX = maxx;
	env->minY = miny;
	env->maxY = maxy;
}

std::string GeoLineString::AsText()
{
	std::stringstream ss;
	int precise = std::numeric_limits<double>::digits10;
	ss.precision(precise);
	ss<<"LineString (";
	int ptCount = GetNumPoint();
	for (int i = 0; i < ptCount; i ++)
	{
		ss<<m_paoPoints[i].x<<" "<<m_paoPoints[i].y;
		if (i < ptCount-1)
		{
			ss<<",";
		}
	}
	ss<<")";
	return ss.str();
}

int GeoLineString::WkbByteSize() const
{
	return 9 + m_nPointCount*16;
}

int GeoLineString::AsBinary( GeoByteOrder eByteOrder,unsigned char *poData ) const
{
	/*�����ֽ���*/
	poData[0] = /*DB2_V72_UNFIX_BYTE_ORDER((unsigned char) eByteOrder)*/gWkbNDR;

	/* -------------------------------------------------------------------- */
	/*      Set the geometry feature type.                                  */
	/* -------------------------------------------------------------------- */
	unsigned int nGType = GetGeometryType();

	/*if( eByteOrder == wkbNDR )
		nGType = CPL_LSBWORD32( nGType );
	else
		nGType = CPL_MSBWORD32( nGType );*/

	memcpy( poData + 1, &nGType, 4 );

	/*      ���Ƶ�ĸ���.    */
	memcpy( poData+5, &m_nPointCount, 4 );

	/*   ���Ƶ������   */
	int i = 0;
	if( m_nCoordDimension == 3 )
	{
		for( i = 0; i < m_nPointCount; i++ )
		{
			memcpy( poData + 9 + 24*i, m_paoPoints+i, 16 );
			//memcpy( poData + 9 + 16 + 24*i, padfZ+i, 8 );
		}
	}
	else
		memcpy( poData+9, m_paoPoints, 16 * m_nPointCount );

	return 1;
}

int GeoLineString::GetNumPoint() const
{
	return m_nPointCount;
}

double GeoLineString::Length() const
{
	double dbLength = 0.0;
	int nPointCount = m_nPointCount - 1;

	//���ߵĳ���Ϊ�����߶γ���֮��
	

	#pragma omp parallel for default(none) shared(nPointCount) reduction(+:dbLength)
	for (int i = 0; i < nPointCount; i ++)
	{
		double deltaX = m_paoPoints[i+1].x - m_paoPoints[i].x;
		double deltaY = m_paoPoints[i+1].y - m_paoPoints[i].y;
		dbLength += sqrt(deltaX*deltaX + deltaY*deltaY);
	}
	return dbLength;
}

void GeoLineString::GetStartPoint(GeoPoint* poPoint) const
{
	GetPoint(0,poPoint);
}

void GeoLineString::GetEndPoint(GeoPoint* poPoint) const
{
	GetPoint(m_nPointCount-1,poPoint);
}

double GeoLineString::GetX(int i) const
{
	if (i >= 0 && i < m_nPointCount)
	{
		return m_paoPoints[i].x;
	}
	return 0;
}

double GeoLineString::GetY(int i) const
{
	if (i >= 0 && i < m_nPointCount)
	{
		return m_paoPoints[i].y;
	}
	return 0;
}

void GeoLineString::SetX(int i, double xIn)
{
	if (i >= 0 && i < m_nPointCount)
	{
		m_paoPoints[i].x = xIn;
	}
	return;
}

void GeoLineString::SetY(int i,double yIn)
{
	if (i >= 0 && i < m_nPointCount)
	{
		m_paoPoints[i].y = yIn;
	}
	return;
}

//�õ�ָ���������
void GeoLineString::GetPoint(int i,GeoPoint *poPoint) const
{
	//assert(poPoint != NULL);
	if (i < 0 || i >= m_nPointCount || NULL == poPoint)
	{
		return;
	}
	else
	{
		poPoint->SetX(m_paoPoints[i].x);
		poPoint->SetY(m_paoPoints[i].y);
	}
}

void GeoLineString::GetPoint(int i,double &x,double &y) const
{
	if (i < 0 || i >= m_nPointCount)
	{
		return;
	}
	else
	{
		x = m_paoPoints[i].x;
		y = m_paoPoints[i].y;
	}
}

void GeoLineString::GetPoint(int i,GeoCoordinate& coord) const
{
	assert(m_paoPoints != NULL);
	if (i < 0 || i >= m_nPointCount)
	{
		return;
	}
	else
	{
		coord = m_paoPoints[i];
	}
}

void GeoLineString::GetPoints( GeoCoordinate *pPointsOut, double * padfZ/* = NULL */ ) const
{
	if (NULL == pPointsOut)
	{
		return;
	}

	//������ֱ�ӿ���pPointsOut��
	memcpy(pPointsOut,m_paoPoints,sizeof(GeoCoordinate) * m_nPointCount);

	//���Ժ���չ����Ԥ��
	if (padfZ != NULL)
	{
	}
}

void GeoLineString::GetPoints( double* padX, double* padY, double* padZ /* = NULL */ ) const
{
	if ( NULL == padX )
	{
		return;
	}
	if ( NULL == padY )
	{
		return;
	}

	//������д��double������
	for (int i = 0; i < m_nPointCount; i ++)
	{
		padX[i] = m_paoPoints[i].x;
		padY[i] = m_paoPoints[i].y;
	}

	//�Ժ�Ԥ��
	if (padZ != NULL)
	{
	}
}

void GeoLineString::GetPoints(GeoCoordinate *poCoords, double* padZ ,int nStride /* = 0 */) const
{
	if ( NULL == poCoords )
	{
		return;
	}

	if (nStride == 0)
	{
		memcpy(poCoords,m_paoPoints,sizeof(GeoCoordinate) * m_nPointCount);
	}
	else if (nStride > 0)
	{
	}
}

void GeoLineString::GetPointArray(double *padVertexs) const
{
	int nCount = m_nPointCount*2;
	int nIndex = 0;
	for (nIndex = 0; nIndex < m_nPointCount; nIndex ++)
	{
		padVertexs[nIndex] = m_paoPoints[nIndex].x;
		padVertexs[nIndex+1] = m_paoPoints[nIndex].y;
	}

// 	if ()
// 	{
// 	}
}

GeoCoordinate* GeoLineString::GetPointsBuf() const
{
	return m_paoPoints;
}

//ָ����Ӧ������
void GeoLineString::SetPoint(int i, GeoPoint *poPoint)
{
	SetPoint(i,poPoint->GetX(),poPoint->GetY());
}

//ָ����Ӧ������
void GeoLineString::SetPoint(int i, double x, double y)
{
	if (i >= m_nPointCount)
	{
		SetPointsNumber(i+1);
		if (i+1 > m_nPointCount)
		{
			return;
		}

		else
		{
			m_paoPoints[i].x = x;
			m_paoPoints[i].y = y;
		}
	}
}

void GeoLineString::AddPoint(GeoPoint &poPoint)
{
	SetPoint(m_nPointCount,poPoint.GetX(),poPoint.GetY());
}

void GeoLineString::AddPoint(double x,double y)
{
	SetPoint(m_nPointCount,x,y);
}

void GeoLineString::AddPoint(GeoCoordinate &poPoint)
{
	SetPoint(m_nPointCount,&poPoint);
}

void GeoLineString::SetPoint(int i, GeoCoordinate *poPoint)
{
	SetPoint(i,poPoint->x,poPoint->y);
}

//���ߴ���ֵһ���ߴ�
void GeoLineString::SetPoints(int nPointsIn, GeoCoordinate *paoPointsIn)
{
	//���뱣֤����������
	if (nPointsIn >= 0)
	{
		SetPointsNumber(nPointsIn);
		if (m_nPointCount < nPointsIn)
		{
			return;
		}

		//�����ڴ�
		memcpy(m_paoPoints,paoPointsIn,sizeof(GeoCoordinate) * nPointsIn);
	}
}

void GeoLineString::SetPoints(int nPoints,double* padX,double* padY,double* padZ /* = NULL */)
{
	if (nPoints > 0)
	{
		SetPointsNumber(nPoints);
		if (m_nPointCount < nPoints)
		{
			return;
		}

		for (int i = 0; i < nPoints; i ++)
		{
			m_paoPoints[i].x = padX[i];
			m_paoPoints[i].y = padY[i];
		}
	}
}

void GeoLineString::SetEnvelope(GeoEnvelope &env)
{
}

double GeoLineString::DistanceToPoint(GeoPoint &poPoint)
{
	//ʵ��������������
	double dotDistance = 0;
	double tempDis = 0;
	dotDistance = sqrt(pow(poPoint.GetX(),2) + pow(poPoint.GetY(),2));	 //��ʼֵ
	for (int i = 0; i < m_nPointCount; i ++)
	{
		tempDis = sqrt(pow(poPoint.GetX()-m_paoPoints[i].x,2) + 
			pow(poPoint.GetY()-m_paoPoints[i].y,2));		//
		if (dotDistance >= tempDis)
		{
			dotDistance = tempDis;
		}
	}

	double lineDistance = 1000;	//�ߣ�������룩
	for (int i = 0; i < m_nPointCount-1; i ++)
	{
		GeoPoint v,w;
		v.SetX(m_paoPoints[i+1].x-m_paoPoints[i].x);
		v.SetY(m_paoPoints[i+1].y-m_paoPoints[i].y);
		w.SetX(poPoint.GetX()-m_paoPoints[i].x);
		w.SetY(poPoint.GetY()-m_paoPoints[i].y);
		double c1 = v.GetX()*w.GetX()+v.GetY()*w.GetY();
		if (c1 <= 0)	  //��һ�����
		{
			tempDis = sqrt(pow(poPoint.GetX()-m_paoPoints[i].x,2)+
				pow(poPoint.GetY()-m_paoPoints[i].y,2));
		}

		double c2 = v.GetX()*v.GetX() + v.GetY()*v.GetY();
		if (c2 <= c1)	 //�ڶ������
		{
			tempDis = sqrt(pow(poPoint.GetX()-this->m_paoPoints[i+1].x,2)+
				pow(poPoint.GetY()-this->m_paoPoints[i+1].y,2));
		}

		//���������
		else
		{
			double x = 0, y = 0;	//ֱ�ߵĴ�������
			double A,B,C;			//ֱ��һ�㷽�̵�ϵ��
			double dSlope = (m_paoPoints[i+1].y-m_paoPoints[i].y)/(m_paoPoints[i+1].x-m_paoPoints[i].x);	//ֱ��б��
			A = dSlope;
			B = -1;
			C = m_paoPoints[i].y - dSlope * m_paoPoints[i].x;
			x = (B*B*poPoint.GetX()-A*B*poPoint.GetY()-A*C)/(A*A+B*B);
			y = (A*A*poPoint.GetY()-A*B*poPoint.GetX()-B*C)/(A*A+B*B);
			tempDis = sqrt(pow(x-poPoint.GetX(),2)+pow(y-poPoint.GetY(),2));	//�㵽����ľ���
		}
		if (lineDistance >= tempDis)
		{
			lineDistance = tempDis;
		}
	}

	return lineDistance;
}

double GeoLineString::DistanceToPoint(double x,double y)
{
	GeoPoint pt(x,y);
	return DistanceToPoint(pt);
}

int GeoLineString::IsLineInRect(GeoEnvelope &env)
{
	//������ߴ�����С�������
	GeoEnvelope envLine(0,0,0,0);
	GetEnvelope(&envLine);

	return envLine.Intersects(env); 

}

GeoGeometry* GeoLineString::Clone() const
{
	GeoLineString* poNewLineString = new GeoLineString();
	poNewLineString->SetSpatialReference(GetSpatialReference());
	poNewLineString->SetPoints(m_nPointCount,m_paoPoints);

	return poNewLineString;
}

//ָ����ĸ���
void GeoLineString::SetPointsNumber(int nNewPointCount)
{
	if (0 == nNewPointCount)
	{
		free(m_paoPoints);
		m_paoPoints = NULL;

		m_nPointCount = 0;
		m_nCurMaxPointCount = 0;
	}

	//����ĵ��ԭ�ȶ�
	if (nNewPointCount > m_nPointCount)
	{
		//int nTmpCount = m_nCurMaxPointCount;
		//�µ��������
		//m_nCurMaxPointCount = (m_nCurMaxPointCount + m_nCurMaxPointCount/2);
		//���·����ڴ�
		GeoCoordinate *paoNewPoints = (GeoCoordinate*)realloc(m_paoPoints,
			sizeof(GeoCoordinate) * nNewPointCount);

		if (paoNewPoints == NULL)
		{
			return;
		}

		//ָ�봫��
		m_paoPoints = paoNewPoints;
		memset(m_paoPoints + m_nPointCount, 0, 
			sizeof(GeoCoordinate) * (nNewPointCount - m_nPointCount) );
	}

	/*else if (nNewPointCount > m_nPointCount && nNewPointCount <= m_nCurMaxPointCount)
	{
	}*/

	m_nPointCount = nNewPointCount;
}

//�Ƿ�պ�
bool GeoLineString::IsClosed() const
{
	//��������3�Ͳ��ܹ��ɻ�
	if (m_nPointCount < 3)
	{
		return false;
	}

	//��һ��������һ�����Ƿ���ͬ
	GeoPoint s,e;
	GetStartPoint(&s);
	GetEndPoint(&e);
	return s.IsEqual(e);
}

bool GeoLineString::IsEmpty() const
{
	return (m_nPointCount == 0);
}

bool GeoLineString::IsClockWise() const
{
	if (!IsClosed())
	{
		return 0;
	}
	double dAreaSum = 0.0;
	int nPointCount = GetNumPoint(); //�����ĸ���
	if(nPointCount < 3)
	{
		return 0;
	}

	for (int i = 0; i < nPointCount -1; i ++)
	{
		dAreaSum += 0.5*(m_paoPoints[i].x * m_paoPoints[i+1].y - 
			m_paoPoints[i+1].x * m_paoPoints[i].y);
	}
	dAreaSum += 0.5*(m_paoPoints[nPointCount-1].x * m_paoPoints[0].y - 
		m_paoPoints[0].x * m_paoPoints[nPointCount-1].y);
	return dAreaSum < 0.0;
}

void GeoLineString::CloseRings()
{
	if (m_nPointCount <= 2)
	{
		return;
	}

	//�����һ�������һ���㲻��ͬ������ϵ�һ����ʹ���߱պ�
	GeoPoint start,end;
	GetStartPoint(&start);
	GetEndPoint(&end);
	if (!start.IsEqual(end))
	{
		AddPoint(start);
	}
	else
		return;
}

//�ƶ��ߴ�
void GeoLineString::Move(double dx, double dy)
{
	for (int i = 0; i < m_nPointCount; i ++)
	{
		m_paoPoints[i].x += dx;
		m_paoPoints[i].y += dy;
	}
}

double GeoLineString::GetArea() const
{
	//���û�бպϣ����Ϊ0
	if (!IsClosed())
	{
		return 0;
	}
	double dAreaSum = 0.0;

	for (int i = 0; i < m_nPointCount -1; i ++)
	{
		dAreaSum += 0.5*(m_paoPoints[i].x * m_paoPoints[i+1].y - 
			m_paoPoints[i+1].x * m_paoPoints[i].y);
	}
	dAreaSum += 0.5*(m_paoPoints[m_nPointCount-1].x * m_paoPoints[0].y - 
		m_paoPoints[0].x * m_paoPoints[m_nPointCount-1].y);
	return fabs(dAreaSum);
}

double GeoLineString::GetLength() const
{
	double dLength = 0.0;
	
	//���ߵĳ���Ϊ�����߶γ���֮��
	for (int i = 0; i < m_nPointCount - 1; i ++)
	{
		double deltaX,deltaY;
		deltaX = m_paoPoints[i+1].x - m_paoPoints[i].x;
		deltaY = m_paoPoints[i+1].y - m_paoPoints[i].y;
		dLength += sqrt(deltaX*deltaX + deltaY*deltaY);
	}
	return dLength;
}

void GeoLineString::ReversePoints()
{
	GeoCoordinate tempPt;
	for (int i = 0; i < m_nPointCount/2; i ++)
	{
		tempPt = m_paoPoints[i];
		m_paoPoints[i] = m_paoPoints[m_nPointCount-i-1];
		m_paoPoints[m_nPointCount-i-1] = tempPt;
	}
}
