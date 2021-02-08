
#include <stdlib.h>
#include <string.h>
#include "GeoFieldDefn.h"

GeoFieldDefn::GeoFieldDefn(void)
{
	pszName = NULL;
	eType = gWkbMaxType;
	eJustify = gWkbUndefined;
	nWidth = 0;
	nPrecision = 0;		//针对于数字
	bIgnore = true;

	memset(&uDefault, 0, sizeof(GeoField));
}

GeoFieldDefn::GeoFieldDefn( GeoFieldDefn * poPrototype)
{
	SetName(poPrototype->GetName());
	SetType(poPrototype->GetType());
	SetJustify(poPrototype->GetJustify());
	SetWidth(poPrototype->GetWidth());
	SetPrecision(poPrototype->GetPrecision());
	SetDefault(poPrototype->GetDefaultRef());
}

GeoFieldDefn::~GeoFieldDefn(void)
{
	if (pszName != NULL)
	{
		free(pszName);
		pszName = NULL;
	}
}

void GeoFieldDefn::SetName(const char *pszNameIn)
{
	/*if (pszName != NULL)
	{
		free(pszName);
		pszName = NULL;
	}*/

	//复制内存
	//pszName = strdup(pszNameIn);
	pszName = new char[strlen(pszNameIn)+1];
	strcpy(pszName,pszNameIn);
}

const char* GeoFieldDefn::GetFieldTypeName( GeoFieldType eTypeIn )
{
	switch( eTypeIn )
	{
	case gWkbInteger:
		return "Integer";

	case gWkbReal:
		return "Real";

	case gWkbString:
		return "String";

	case gWkbIntegerList:
		return "IntegerList";

	case gWkbRealList:
		return "RealList";

	case gWkbStringList:
		return "StringList";

	case gWkbBinary:
		return "Binary";

	case gWkbDate:
		return "Date";

	case gWkbTime:
		return "Time";

	case gWkbDateTime:
		return "DateTime";

	default:
		return "unknown";
	}
}

void GeoFieldDefn::SetDefault( const GeoField * puDefault)
{
	switch( eType )
	{
	case gWkbInteger:
	case gWkbReal:
		uDefault = *puDefault;
		break;

	case gWkbString:
		break;

	default:
		break;
	}
}

void GeoFieldDefn::Set( const char *pszNameIn, GeoFieldType eTypeIn,
					   int nWidthIn, int nPrecisionIn, GeoJustification eJustifyIn )
{
	SetName( pszNameIn );
	SetType( eTypeIn );
	SetWidth( nWidthIn );
	SetPrecision( nPrecisionIn );
	SetJustify( eJustifyIn );
}

const char *OGR_GetFieldTypeName( GeoFieldType eType )

{
	return GeoFieldDefn::GetFieldTypeName( eType );
}
