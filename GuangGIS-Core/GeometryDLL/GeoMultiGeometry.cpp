
#include "GeoGeometry.h"
#include "GeoGeometryCollection.h"
#include "GeoMultiGeometry.h"

/************************************************************************/
/*                            GeoMultiPoint                             */
/************************************************************************/

GeoMultiPoint::GeoMultiPoint(void)
{
}

GeoMultiPoint::~GeoMultiPoint(void)
{
}

GeoWkbGeometryType GeoMultiPoint::GetGeometryType() const
{
	return gWkbMultiPoint;
}

const char* GeoMultiPoint::GetGeometryName() const
{
	return "MULTIPOINT";
}

GeoGeometry *GeoMultiPoint::Clone() const
{
	GeoMultiPoint       *poNewGC;

	poNewGC = new GeoMultiPoint;
	poNewGC->SetSpatialReference( GetSpatialReference() );

	for( int i = 0; i < GetNumGeoms(); i++ )
	{
		poNewGC->AddGeometry( (GeoGeometry *)GetGeometryRefs(i) );
	}

	return poNewGC;
}

bool GeoMultiPoint::AddGeometry(GeoGeometry *poGeom)
{
	if( poGeom->GetGeometryType() != gWkbPoint 
		&& poGeom->GetGeometryType() != gWkbPoint25D )
	{
		return false;
	}
	return GeoGeometryCollection::AddGeometry(poGeom);
}

/************************************************************************/
/*                          GeoMultiLineString                          */
/************************************************************************/

GeoMultiLineString::GeoMultiLineString(void)
{
}

GeoMultiLineString::~GeoMultiLineString(void)
{
}

GeoWkbGeometryType GeoMultiLineString::GetGeometryType() const
{
	return gWkbMultiLineString;
}

const char* GeoMultiLineString::GetGeometryName() const
{
	return "MULTILINESTRING";
}

GeoGeometry *GeoMultiLineString::Clone() const
{
	GeoMultiLineString       *poNewGC;

	poNewGC = new GeoMultiLineString;
	poNewGC->SetSpatialReference( GetSpatialReference() );

	for( int i = 0; i < GetNumGeoms(); i++ )
	{
		poNewGC->AddGeometry( (GeoGeometry *)GetGeometryRefs(i) );
	}

	return poNewGC;
}

bool GeoMultiLineString::AddGeometry(GeoGeometry *poGeom)
{
	if( poGeom->GetGeometryType() != gWkbLineString 
		&& poGeom->GetGeometryType() != gWkbLineString25D )
	{
		return false;
	}
	return GeoGeometryCollection::AddGeometry(poGeom);
}

/************************************************************************/
/*                           GeoMultiPolygon                            */
/************************************************************************/

GeoMultiPolygon::GeoMultiPolygon(void)
{
}

GeoMultiPolygon::~GeoMultiPolygon(void)
{
}

GeoWkbGeometryType GeoMultiPolygon::GetGeometryType() const
{
	return gWkbMultiPolygon;
}

const char* GeoMultiPolygon::GetGeometryName() const
{
	return "MULTIPOLYGON";
}

GeoGeometry *GeoMultiPolygon::Clone() const
{
	GeoMultiPolygon       *poNewGC;

	poNewGC = new GeoMultiPolygon;
	poNewGC->SetSpatialReference( GetSpatialReference() );

	for( int i = 0; i < GetNumGeoms(); i++ )
	{
		poNewGC->AddGeometry( (GeoGeometry *)GetGeometryRefs(i) );
	}

	return poNewGC;
}

bool GeoMultiPolygon::AddGeometry(GeoGeometry *poGeom)
{
	if( poGeom->GetGeometryType() != gWkbPolygon 
		&& poGeom->GetGeometryType() != gWkbPolygon25D )
	{
		return false;
	}
	return GeoGeometryCollection::AddGeometry(poGeom);
}
