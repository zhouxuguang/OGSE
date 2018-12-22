
#include <stdlib.h>

#include "GeoGeometryFactory.h"
#include "GeoPoint.h"
#include "GeoLineString.h"
#include "GeoPolygon.h"

GeoGeometryFactory::GeoGeometryFactory(void)
{
}

GeoGeometryFactory::~GeoGeometryFactory(void)
{
}

GeoGeometry* GeoGeometryFactory::CreateGeometry( const char* pszGeomName )
{
	GeoGeometry* poGeometry = NULL;
	if (EQUAL(pszGeomName,"Point"))
	{
		GeoPoint* pPoint = new GeoPoint;
		poGeometry = pPoint;
	}

	else if (EQUAL(pszGeomName,"Linesting"))
	{
		GeoLineString* poLine = new GeoLineString;
		poGeometry = poLine;
	}

	else if (EQUAL(pszGeomName,"Polygon"))
	{
		GeoPolygon* poPolygon = new GeoPolygon;
		poGeometry = poPolygon;
	}

	return poGeometry;
}

GeoGeometry* GeoGeometryFactory::CreateGeometry(GeoWkbGeometryType eGeomType)
{
	GeoGeometry* poGeometry = NULL;
	if (eGeomType == gWkbPoint)
	{
		GeoPoint* pPoint = new GeoPoint;
		poGeometry = pPoint;
	}

	else if (eGeomType == gWkbLineString)
	{
		GeoLineString* poLine = new GeoLineString;
		poGeometry = poLine;
	}

	else if (eGeomType == gWkbPolygon)
	{
		GeoPolygon* poPolygon = new GeoPolygon;
		poGeometry = poPolygon;
	}

	return poGeometry;
}
