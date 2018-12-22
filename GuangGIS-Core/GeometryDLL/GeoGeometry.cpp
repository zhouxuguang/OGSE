
#include "GeoSpatialReference.h"
#include "GeoEnvelope.h"
#include "GeoGeometry.h"

GeoGeometry::GeoGeometry(void)
{
	m_nCoordDimension = 0;
	m_poSRS = NULL;
}

GeoGeometry::~GeoGeometry(void)
{
}

void GeoGeometry::SetSpatialReference(GeoSpatialReference *poSR)
{
	m_poSRS = poSR;
}

GeoSpatialReference* GeoGeometry::GetSpatialReference() const
{
	return m_poSRS;
}

/************************************************************************/
/* 空间关系与空间分析                                                   */
/************************************************************************/

bool  GeoGeometry::Intersects( GeoGeometry * poGeometry) const
{
	return 1;
}

bool  GeoGeometry::Equals( GeoGeometry * poGeometry) const
{
	return 1;
}

bool  GeoGeometry::Disjoint( const GeoGeometry * poGeometry) const
{
	return 1;
}

bool  GeoGeometry::Touches( const GeoGeometry * poGeometry) const
{
	return 1;
}

bool  GeoGeometry::Crosses( const GeoGeometry * poGeometry) const
{
	return 1;
}

bool  GeoGeometry::Within( const GeoGeometry * poGeometry) const
{
	return 1;
}

bool  GeoGeometry::Contains( const GeoGeometry * poGeometry) const
{
	return 1;
}

bool  GeoGeometry::Overlaps( const GeoGeometry * poGeometry) const
{
	return 1;
}

bool  GeoGeometry::Relate( const GeoGeometry *, const char * ) const
{
	return 1;
}

//空间分析
GeoGeometry * GeoGeometry::Boundary() const
{
	return NULL;
}

double  GeoGeometry::Distance( const GeoGeometry * poGeometry) const
{
	return 0;
}

GeoGeometry * GeoGeometry::ConvexHull() const
{
	return NULL;
}

GeoGeometry * GeoGeometry::Buffer( double dfDist, int nQuadSegs ) const
{
	return NULL;
}

GeoGeometry * GeoGeometry::Intersection( const GeoGeometry *poGeometry) const
{
	return NULL;
}

GeoGeometry * GeoGeometry::Union( const GeoGeometry * poGeometry) const
{
	return NULL;
}

GeoGeometry * GeoGeometry::UnionCascaded() const
{
	return NULL;
}

GeoGeometry * GeoGeometry::Difference( const GeoGeometry * poGeometry) const
{
	return NULL;
}

GeoGeometry * GeoGeometry::SymDifference( const GeoGeometry * poGeometry) const
{
	return NULL;
}

int GeoGeometry::Centroid( GeoPoint * poPoint ) const
{
	return 1;
}

GeoGeometry * GeoGeometry::Simplify(double dTolerance) const
{
	return NULL;
}

GeoGeometry *GeoGeometry::SimplifyPreserveTopology(double dTolerance) const
{
	return NULL;
}

GeoGeometry *GeoGeometry::Polygonize() const
{
	return NULL;
}

 
bool  GeoGeometry::Intersect( GeoGeometry * poGeometry) const
{
	return 1;
}

bool  GeoGeometry::Equal( GeoGeometry * poGeometry) const
{
	return 1;
}

GeoGeometry *GeoGeometry::SymmetricDifference( const GeoGeometry * poGeometry) const
{
	return NULL;
}

GeoGeometry * GeoGeometry::getBoundary() const
{
	return NULL;
}
