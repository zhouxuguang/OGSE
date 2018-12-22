#include "GeoRasterDefine.h"

int GEOMETRY_API_CALL RASGetDataTypeSize( RASDataType eDataType )
{
	if (eDataType == PDT_Byte)
	{
		return 1;
	}

	else if (eDataType == PDT_CInt16 || eDataType == PDT_Int16)
	{
		return 2;
	}

	else if (eDataType == PDT_Int32 || eDataType == PDT_UInt32 || eDataType == PDT_Float32)
	{
		return 4;
	}

	else if (eDataType == PDT_Float64)
	{
		return 8;
	}

	return 0;
}


