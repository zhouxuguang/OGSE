#include "GeoDataSource.h"
#include "GeoMitabDataSource.h"
#include "GeoMitabDriver.h"
#include "GeoDataDriverManager.h"

#include <string.h>

GeoMitabDriver::GeoMitabDriver(void)
{
	m_pszName = strdup("MapInfo File");
}

GeoMitabDriver::~GeoMitabDriver(void)
{
}

const char* GeoMitabDriver::GetName()
{
	return "MapInfo File";
}

GeoDataSource * GeoMitabDriver::Open( const char *pszName, bool bUpdate/* =false */ )
{
	GeoMitabDataSource *poShpDS = new GeoMitabDataSource();

	if (!poShpDS->Open(pszName,bUpdate))
	{
		delete poShpDS;
		return NULL;
	}
	return poShpDS;
}

GeoDataSource* GeoMitabDriver::CreateGeoDataSource( const char *pszName, char ** /* = NULL */ )
{
	return NULL;
}

bool GeoMitabDriver::DeleteGeoDataSource( const char *pszName )
{
	return false;
}

GeoDataSource *GeoMitabDriver::CopyGeoDataSource( GeoDataSource *poSrcDS, const char *pszNewName
												 , char **papszOptions /* = NULL */ )
{
	return NULL;
}

//×¢²ámapinfoÇý¶¯
void GEOMETRY_API_CALL RegisterMapinfo()
{
	GeoDataDriverManager::GetInstance()->RegisterDriver(new GeoMitabDriver);
}
