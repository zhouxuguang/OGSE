#ifndef GEPMAPPROJECTION_FACTORY_INCLUDE_H
#define GEPMAPPROJECTION_FACTORY_INCLUDE_H

#include "CoordCommon.h"

class GeoMapProjection;

class COORDTRAN_API GeoMapProjectionFactory
{
public:
	GeoMapProjectionFactory(void);

	~GeoMapProjectionFactory(void);

	static GeoMapProjection* CreateMapProjection(const char* pszProjName);
};

#endif
