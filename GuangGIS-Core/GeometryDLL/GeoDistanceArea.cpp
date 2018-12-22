/*
 * DistanceArea1.cpp
 *
 *  Created on: May 15, 2015
 *      Author: zxg
 */

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include "GeoDistanceArea.h"

#define DEG2RAD(x)    ((x)*M_PI/180)


GeoDistanceArea::GeoDistanceArea()
{
	//
}


GeoDistanceArea::~GeoDistanceArea()
{
}

void GeoDistanceArea::SetEllipsePara(double a,double b)
{
	mSemiMajor = a;
	mSemiMinor = b;
	//mInvFlattening = mSemiMajor

	ComputeAreaInit();
}

double GeoDistanceArea::GetQ( double x )
{
	double sinx, sinx2;

	sinx = sin( x );
	sinx2 = sinx * sinx;

	return sinx *( 1 + sinx2 *( m_QA + sinx2 *( m_QB + sinx2 * m_QC ) ) );
}


double GeoDistanceArea::GetQbar( double x )
{
	double cosx, cosx2;

	cosx = cos( x );
	cosx2 = cosx * cosx;

	return cosx *( m_QbarA + cosx2 *( m_QbarB + cosx2 *( m_QbarC + cosx2 * m_QbarD ) ) );
}


void GeoDistanceArea::ComputeAreaInit()
{
	double a2 = ( mSemiMajor * mSemiMajor );
	double e2 = 1 - ( a2 / ( mSemiMinor * mSemiMinor ) );
	double e4, e6;

	m_TwoPI = M_PI + M_PI;

	e4 = e2 * e2;
	e6 = e4 * e2;

	m_AE = a2 * ( 1 - e2 );

	m_QA = ( 2.0 / 3.0 ) * e2;
	m_QB = ( 3.0 / 5.0 ) * e4;
	m_QC = ( 4.0 / 7.0 ) * e6;

	m_QbarA = -1.0 - ( 2.0 / 3.0 ) * e2 - ( 3.0 / 5.0 ) * e4  - ( 4.0 / 7.0 ) * e6;
	m_QbarB = ( 2.0 / 9.0 ) * e2 + ( 2.0 / 5.0 ) * e4  + ( 4.0 / 7.0 ) * e6;
	m_QbarC =                     - ( 3.0 / 25.0 ) * e4 - ( 12.0 / 35.0 ) * e6;
	m_QbarD = ( 4.0 / 49.0 ) * e6;

	m_Qp = GetQ( M_PI / 2 );
	m_E  = 4 * M_PI * m_Qp * m_AE;
	if ( m_E < 0.0 )
	m_E = -m_E;
}


double GeoDistanceArea::ComputePolygonArea( const double *padX,const double* padY,int nCount )
{
	double x1, y1, dx, dy;
	double Qbar1, Qbar2;

	if (NULL == padX || NULL == padY)
	{
		return 0;
	}


	if (nCount < 3)
	{
		return 0;
	}



	double x2 = DEG2RAD( padX[nCount-1] );
	double y2 = DEG2RAD( padY[nCount-1] );
	Qbar2 = GetQbar( y2 );

	double area = 0.0;

	for ( int i = 0; i < nCount; i++ )
	{
		x1 = x2;
		y1 = y2;
		Qbar1 = Qbar2;

		x2 = DEG2RAD( padX[i] );
		y2 = DEG2RAD( padY[i] );
		Qbar2 = GetQbar( y2 );

		if ( x1 > x2 )
		  while ( x1 - x2 > M_PI )
			x2 += m_TwoPI;
		else if ( x2 > x1 )
		  while ( x2 - x1 > M_PI )
			x1 += m_TwoPI;

		dx = x2 - x1;
		area += dx * ( m_Qp - GetQ( y2 ) );

		if (( dy = y2 - y1 ) != 0.0 )
		  area += dx * GetQ( y2 ) - ( dx / dy ) * ( Qbar2 - Qbar1 );
	}
	if (( area *= m_AE ) < 0.0 )
		area = -area;

	if ( area > m_E )
		area = m_E;
	if ( area > m_E / 2 )
		area = m_E - area;

	return area;
}
