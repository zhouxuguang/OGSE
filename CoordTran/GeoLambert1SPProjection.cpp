#include "GeoLambert1SPProjection.h"


GeoLambert1SPProjection::GeoLambert1SPProjection(const GeoEllipsoid& ellipsoid, 
												 const double dbStandardLat, 
												 const double dbOriginLat /*= 0*/, 
												 const double dbOriginLon /*= 0*/, 
												 const double dFalseEasting /*= 0*/, 
												 const double dFalseNorthing /*= 0*/)
												 :GeoMapProjection(ellipsoid)
{

}

GeoLambert1SPProjection::~GeoLambert1SPProjection(void)
{
}
