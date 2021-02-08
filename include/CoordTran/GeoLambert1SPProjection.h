#ifndef GEOLAMBERT_PROJECTION_2_58847FGGKJH_INCLUDE_H
#define GEOLAMBERT_PROJECTION_2_58847FGGKJH_INCLUDE_H


#include "GeoMapProjection.h"

class COORDTRAN_API GeoLambert1SPProjection : public GeoMapProjection
{
public:
	GeoLambert1SPProjection(const GeoEllipsoid& ellipsoid,
		const double dbStandardLat,
		const double dbOriginLat = 0,
		const double dbOriginLon = 0,
		const double dFalseEasting = 0,
		const double dFalseNorthing = 0);

	~GeoLambert1SPProjection(void);
};

#endif
