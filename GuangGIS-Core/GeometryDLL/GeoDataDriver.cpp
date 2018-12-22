
#include <stdlib.h>

#include "GeoDataDriver.h"

GeoDataDriver::GeoDataDriver(void)
{
	//
}

GeoDataDriver::~GeoDataDriver()
{
	/*if (m_po)
	{
	}*/
}

GeoDataSource* GeoDataDriver::CreateGeoDataSource( const char *pszName, char ** /* = NULL */ )
{
	return NULL;
}

bool GeoDataDriver::DeleteGeoDataSource( const char *pszName )
{
	return true;
}

GeoDataSource *GeoDataDriver::CopyGeoDataSource( GeoDataSource *poSrcDS, 
										   const char *pszNewName, 
										   char **papszOptions /* = NULL */ )
{
	return NULL;
}
