
#include "GeoShapeDriver.h"
#include "GeoDataDriver.h"
#include "GeoShapeDataSource.h"
#include "GeoDataDriverManager.h"

#include <string.h>
#include <stdlib.h>

GeoShapeDriver::GeoShapeDriver(void)
{
	m_pszName = strdup("ESRI Shapefile");
}

GeoShapeDriver::~GeoShapeDriver(void)
{
	free(m_pszName);
}

const char* GeoShapeDriver::GetName()
{
	return "ESRI Shapefile";
}

GeoDataSource* GeoShapeDriver::Open( const char *pszName, bool bUpdate/* =false */ )
{
	GeoShapeDataSource *poShpDS = new GeoShapeDataSource();

	if (!poShpDS->Open(pszName,bUpdate))
	{
		delete poShpDS;
		return NULL;
	}
	return poShpDS;
}

GeoDataSource* GeoShapeDriver::CreateGeoDataSource( const char *pszName, char ** papszItem )
{
	bool bSingleNewFile = false;

	if( pszName==NULL || strlen(pszName)==0 )
	{
		return NULL;
	}

	if( EQUAL(strdup(pszName),"shp") 
		|| EQUAL(strdup(pszName),"dbf") )
	{
		bSingleNewFile = true;
	}

	//返回一个新的数据源
	GeoDataSource *poDS = NULL;

	poDS = new GeoShapeDataSource();
	if( !poDS->Open( pszName, true ) )
	{
		delete poDS;
		return NULL;
	}
	else
		return poDS;
}

bool GeoShapeDriver::DeleteGeoDataSource( const char *pszName )
{
	return false;
}

GeoDataSource* GeoShapeDriver::CopyGeoDataSource( GeoDataSource *poSrcDS, 
												 const char *pszNewName, 
												 char **papszOptions /* = NULL */ )
{
	return NULL;
}

void GEOMETRY_API_CALL RegisterShapefile()
{
	GeoDataDriverManager::GetInstance()->RegisterDriver(new GeoShapeDriver);
}
