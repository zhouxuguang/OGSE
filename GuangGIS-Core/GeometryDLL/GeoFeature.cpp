
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include <vector>
#include "GeoFeatureDefn.h"
#include "GeoFieldDefn.h"
#include "GeoGeometry.h"
#include "GeoFeature.h"
#include "GeoDefine.h"

GeoFeature::GeoFeature()
{
	m_poDefn = NULL;
	m_pauFields = NULL;
	m_poGeometry = NULL;
	m_nFID = -1;
	m_pszTmpFieldValue = NULL;
}

GeoFeature::GeoFeature( GeoFeatureDefn * poDefnIn)
{
	m_poDefn = poDefnIn->Clone();

	m_nFID = GeoNullFID;

	m_poGeometry = NULL;

	m_pauFields = (GeoField *) malloc( m_poDefn->GetFieldCount()*sizeof(GeoField) );

	for( int i = 0; i < m_poDefn->GetFieldCount(); i++ )
	{
		m_pauFields[i].Set.nMarker1 = GeoUnsetMarker;
		m_pauFields[i].Set.nMarker2 = GeoUnsetMarker;
	}

	m_pszTmpFieldValue = NULL;
}

GeoFeature::~GeoFeature(void)
{
	if (m_pszTmpFieldValue != NULL)
	{
		free(m_pszTmpFieldValue);
		m_pszTmpFieldValue = NULL;
	}

	if (m_poGeometry != NULL)
	{
		delete m_poGeometry;
		m_poGeometry = NULL;
	}

	for( int i = 0; i < m_poDefn->GetFieldCount(); i++ )
	{
		GeoFieldDefn *poFDefn = m_poDefn->GetFieldDefn(i);

		switch( poFDefn->GetType() )
		{
		case gWkbString:
			if( m_pauFields[i].String != NULL )
				//free( m_pauFields[i].String );
			break;

		case gWkbBinary:
			if( m_pauFields[i].Binary.paData != NULL )
				//free( m_pauFields[i].Binary.paData );
			break;

		case gWkbStringList:
			break;

		case gWkbIntegerList:
		case gWkbRealList:
			//free( m_pauFields[i].IntegerList.paList );
			break;

		default:
			// 默认不支持
			break;
		}
	}

	if (m_poDefn != NULL)
	{
		delete m_poDefn;
		m_poDefn = NULL;
	}

	if (m_pauFields != NULL)
	{
		free(m_pauFields);
		m_pauFields = NULL;
	}
}

bool GeoFeature::SetGeometryDirectly(GeoGeometry *poGeometryIn)
{
	GeoGeometry *poGeometry = poGeometryIn->Clone();
	if (m_poGeometry != NULL)
	{
		delete poGeometry;
		m_poGeometry = poGeometry;
		return true;
	}
	else
	{
		m_poGeometry = poGeometry;
		return true;
	}

	return false;
}

int GeoFeature::GetFieldAsInteger(int iField)
{
	int specialIndex = iField - m_poDefn->GetFieldCount();

	if (specialIndex >= 0)
	{
		switch (specialIndex)
		{
		case SPF_FID:
			return GetFID();
		case SPF_OGR_GEOM_AREA:
			if (m_poGeometry == NULL)
			{
				return 0;
			}
			return (int)m_poGeometry->GetArea();
		default:
			return 0;
		}
	}

	GeoFieldDefn *pofDefn = GetFieldDefnRef(iField);

	assert( pofDefn != NULL || iField == -1 );
	if( pofDefn == NULL )
		return 0;
	GeoFieldType type = pofDefn->GetType();
	if (type == gWkbInteger)
	{
		return m_pauFields[iField].Integer;
	}
	else if (type == gWkbReal)
	{
		return (int)m_pauFields[iField].Real;
	}
	else if (type == gWkbString)
	{
		if (m_pauFields[iField].String == NULL)
		{
			return 0;
		}
		else
		{
			return atoi(m_pauFields[iField].String);
		}
	}
	else
		return 0;
}

double GeoFeature::GetFieldAsDouble(int iField)
{
	int specialIndex = iField - m_poDefn->GetFieldCount();

	if (specialIndex >= 0)
	{
		switch (specialIndex)
		{
		case SPF_FID:
			return GetFID();
		case SPF_OGR_GEOM_AREA:
			if (m_poGeometry == NULL)
			{
				return 0.0;
			}
			return m_poGeometry->GetArea();
		default:
			return 0.0;
		}
	}

	GeoFieldDefn *pofDefn = GetFieldDefnRef(iField);

	assert( pofDefn != NULL || iField == -1 );
	if( pofDefn == NULL )
	{
		return 0.0;
	}
	GeoFieldType type = pofDefn->GetType();
	if (type == gWkbInteger)
	{
		return (double)m_pauFields[iField].Integer;
	}
	else if (type == gWkbReal)
	{
		return m_pauFields[iField].Real;
	}
	else if (type == gWkbString)
	{
		if (m_pauFields[iField].String == NULL)
		{
			return 0.0;
		}
		else
		{
			return atof(m_pauFields[iField].String);
		}
	}
	else
		return 0.0;
}

const char* GeoFeature::GetFieldAsString(int iField)
{
	char pszTmpValue[260];

	free(m_pszTmpFieldValue);
	m_pszTmpFieldValue = NULL;

	int specialIndex = iField - m_poDefn->GetFieldCount();

	//特殊的字段值
	if (specialIndex >= 0)
	{
		switch (specialIndex)
		{
		case SPF_FID:
			sprintf(pszTmpValue,"%ld",GetFID());
			m_pszTmpFieldValue = strdup(pszTmpValue);
			return m_pszTmpFieldValue;

		case SPF_OGR_GEOMETRY:
			return m_poGeometry->GetGeometryName();
		default:
			return "";
		}
	}

	GeoFieldDefn *pofDefn = GetFieldDefnRef(iField);

	assert( pofDefn != NULL || iField == -1 );
	if( pofDefn == NULL )
		return "";

	GeoFieldType type = pofDefn->GetType();
	if (type == gWkbInteger)				 //整数
	{
		sprintf(pszTmpValue,"%d",m_pauFields[iField].Integer);
		m_pszTmpFieldValue = strdup(pszTmpValue);
		return m_pszTmpFieldValue;
	}

	else if (type == gWkbReal)			  //浮点数
	{
		char    szFormat[64];	//格式字符串
		if (pofDefn->GetPrecision() > 0)
		{
			int width = pofDefn->GetWidth();
			int precision = pofDefn->GetPrecision();
			sprintf(szFormat,"%%%d.%df",width,precision);
		}
		else
		{
			strcpy(szFormat,"%.15g");
		}
		sprintf(pszTmpValue,szFormat,m_pauFields[iField].Real);
		m_pszTmpFieldValue = strdup(pszTmpValue);
		return m_pszTmpFieldValue;
	}

	else if (type == gWkbString)			   //字符串
	{
		if (m_pauFields[iField].String == NULL)
		{
			return "";
		}
		else
		{
			return m_pauFields[iField].String;
		}
	}

	else if (type == gWkbDateTime)			//日期时间
	{
		sprintf(pszTmpValue,"%4d-%2d-%2d %2d-%2d-%2d",
			m_pauFields[iField].Date.Year,
			m_pauFields[iField].Date.Month,
			m_pauFields[iField].Date.Day,
			m_pauFields[iField].Date.Hour,
			m_pauFields[iField].Date.Minute,
			m_pauFields[iField].Date.Second);

		m_pszTmpFieldValue = strdup(pszTmpValue);
		return m_pszTmpFieldValue;
	}

	else if (type == gWkbDate)				//日期类型
	{
		sprintf(pszTmpValue,"%4d-%2d-%2d",
			m_pauFields[iField].Date.Year,
			m_pauFields[iField].Date.Month,
			m_pauFields[iField].Date.Day);

		m_pszTmpFieldValue = strdup(pszTmpValue);
		return m_pszTmpFieldValue;
	}

	else if (type == gWkbTime)				//时间类型
	{
		sprintf(pszTmpValue,"%2d-%2d-%2d",
			m_pauFields[iField].Date.Hour,
			m_pauFields[iField].Date.Minute,
			m_pauFields[iField].Date.Second);
	}
	else
		return "";

	return "";
}

GByte* GeoFeature::GetFieldAsBinary(int iField, int *nCount)
{
	GeoFieldDefn *pofDefn = GetFieldDefnRef(iField);
	*nCount = 0;

	assert( pofDefn != NULL || iField == -1 );
	if( pofDefn == NULL )
		return NULL;

	GeoFieldType type = pofDefn->GetType();
	if (type == gWkbBinary)				 //二进制数
	{
		*nCount = m_pauFields[iField].Binary.nCount;
		return m_pauFields[iField].Binary.paData;
	}

	else
	{
		return NULL;
	}
}

bool GeoFeature::GetFieldAsDateTime(int iField, int *nYear, int *nMonth, int *nDay, 
									int *nHour, int *nMinute, int *nSecond,int *nTZFlag)
{
	GeoFieldDefn *pofDefn = GetFieldDefnRef(iField);

	assert( pofDefn != NULL || iField == -1 );
	if( pofDefn == NULL )
	{
		return NULL;
	}

	GeoFieldType type = pofDefn->GetType();
	if (type == gWkbDateTime || 
		type == gWkbDate || 
		type == gWkbTime)				 //日期数据类型
	{
		if( nYear )
			*nYear = m_pauFields[iField].Date.Year;
		if( nMonth )
			*nMonth = m_pauFields[iField].Date.Month;
		if( nDay )
			*nDay = m_pauFields[iField].Date.Day;
		if( nHour )
			*nHour = m_pauFields[iField].Date.Hour;
		if( nMinute )
			*nMinute = m_pauFields[iField].Date.Minute;
		if( nSecond )
			*nSecond = m_pauFields[iField].Date.Second;
		if( nTZFlag )
			*nTZFlag = m_pauFields[iField].Date.TZFlag;

		return true;
	}

	else
	{
		return false;
	}
}

void GeoFeature::SetField(GeoFieldDefn *poField)
{
	m_poDefn->AddFieldDefn(poField);
}

void GeoFeature::SetField( int iField, int nValue)
{
	GeoFieldDefn *poFDefn = m_poDefn->GetFieldDefn( iField );
	if (NULL == poFDefn)
	{
		return;
	}

	if (poFDefn->GetType() == gWkbInteger)
	{
		m_pauFields[iField].Integer = nValue;
		m_pauFields[iField].Set.nMarker2 = 0;
	}
	else if (poFDefn->GetType() == gWkbReal)
	{
		m_pauFields[iField].Real = nValue;
	}
	else if (poFDefn->GetType() == gWkbString)
	{
		char    szTempBuffer[64];
		sprintf( szTempBuffer, "%d", nValue );
		m_pauFields[iField].String = strdup( szTempBuffer );
	}
}

void GeoFeature::SetField( int iField, double dfValue )

{
	GeoFieldDefn  *poFDefn = m_poDefn->GetFieldDefn( iField );

	if( poFDefn == NULL )
		return;

	if( poFDefn->GetType() == gWkbReal )
	{
		m_pauFields[iField].Real = dfValue;
	}
	else if( poFDefn->GetType() == gWkbInteger )
	{
		m_pauFields[iField].Integer = (int) dfValue;
		m_pauFields[iField].Set.nMarker2 = 0;
	}
	else if( poFDefn->GetType() == gWkbRealList )
	{
		SetField( iField, 1, &dfValue );
	}
	else if( poFDefn->GetType() == gWkbIntegerList )
	{
		int nValue = (int) dfValue;
		SetField( iField, 1, &nValue );
	}
	else if( poFDefn->GetType() == gWkbString )
	{
		char    szTempBuffer[128];

		sprintf( szTempBuffer, "%.16g", dfValue );
		m_pauFields[iField].String = strdup( szTempBuffer );
	}

}

void GeoFeature::SetField( int iField, const char * pszValue )
{
	GeoFieldDefn *poFDefn = m_poDefn->GetFieldDefn( iField );
	char *pszLast;

	if( poFDefn == NULL )
		return;

	if( poFDefn->GetType() == gWkbString )
	{
		m_pauFields[iField].String = strdup( pszValue );
	}
	else if( poFDefn->GetType() == gWkbInteger )
	{
		long nVal = strtol(pszValue, &pszLast, 10);
		m_pauFields[iField].Integer = (nVal > INT_MAX) ? INT_MAX : (nVal < INT_MIN) ? INT_MIN : (int) nVal;
		if((nVal != (long)m_pauFields[iField].Integer || !pszLast || *pszLast ) )
		{
			return;
		}
		m_pauFields[iField].Set.nMarker2 = GeoUnsetMarker;
	}
	else if( poFDefn->GetType() == gWkbReal )
	{
		m_pauFields[iField].Real = strtod(pszValue, &pszLast);
	}
	else if( poFDefn->GetType() == gWkbDate 
		|| poFDefn->GetType() == gWkbTime
		|| poFDefn->GetType() == gWkbDateTime )
	{
	}
	else if( poFDefn->GetType() == gWkbIntegerList 
		|| poFDefn->GetType() == gWkbRealList )
	{
		char **papszValueList = NULL;

		if( poFDefn->GetType() == gWkbIntegerList )
		{
			int i, nCount = atoi(papszValueList[0]);
			std::vector<int> anValues;

			for( i=0; i < nCount; i++ )
				anValues.push_back( atoi(papszValueList[i+1]) );
			SetField( iField, nCount, &(anValues[0]) );
		}
		else if( poFDefn->GetType() == gWkbRealList )
		{
			int i, nCount = atoi(papszValueList[0]);
			std::vector<double> adfValues;

			for( i=0; i < nCount; i++ )
				adfValues.push_back( atof(papszValueList[i+1]) );
			SetField( iField, nCount, &(adfValues[0]) );
		}

	}
	else
		;

}

void GeoFeature::SetField( int iField, int nCount, int * panValues )
{
	GeoFieldDefn *poFDefn = m_poDefn->GetFieldDefn( iField );

	if( poFDefn == NULL )
		return;

	if( poFDefn->GetType() == gWkbIntegerList )
	{
		GeoField uField;

		uField.IntegerList.nCount = nCount;
		uField.Set.nMarker2 = 0;
		uField.IntegerList.paList = panValues;

		SetField( iField, &uField );
	}
	else if( poFDefn->GetType() == gWkbRealList )
	{
		std::vector<double> adfValues;

		for( int i=0; i < nCount; i++ )
			adfValues.push_back( (double) panValues[i] );

		SetField( iField, nCount, &adfValues[0] );
	}
	else if( (poFDefn->GetType() == gWkbInteger || poFDefn->GetType() == gWkbReal)
		&& nCount == 1 )
	{
		SetField( iField, panValues[0] );
	}
}

void GeoFeature::SetField( int iField, int nCount, double * padfValues )

{
	GeoFieldDefn *poFDefn = m_poDefn->GetFieldDefn( iField );

	if( poFDefn == NULL )
		return;

	if( poFDefn->GetType() == gWkbRealList )
	{
		GeoField uField;

		uField.RealList.nCount = nCount;
		uField.Set.nMarker2 = 0;
		uField.RealList.paList = padfValues;

		SetField( iField, &uField );
	}
	else if( poFDefn->GetType() == gWkbIntegerList )
	{
		std::vector<int> anValues;

		for( int i=0; i < nCount; i++ )
			anValues.push_back( (int) padfValues[i] );

		SetField( iField, nCount, &anValues[0] );
	}
	else if( (poFDefn->GetType() == gWkbInteger || poFDefn->GetType() == gWkbReal)
		&& nCount == 1 )
	{
		SetField( iField, padfValues[0] );
	}
}

void GeoFeature::SetField( int iField, GeoField * puValue )
{
	GeoFieldDefn  *poFDefn = m_poDefn->GetFieldDefn( iField );

	if( poFDefn == NULL )
		return;

	if( poFDefn->GetType() == gWkbInteger )
	{
		m_pauFields[iField] = *puValue;
	}
	else if( poFDefn->GetType() == gWkbReal )
	{
		m_pauFields[iField] = *puValue;
	}
	else if( poFDefn->GetType() == gWkbString )
	{
		if( puValue->String == NULL )
			m_pauFields[iField].String = NULL;
		else if( puValue->Set.nMarker1 == GeoUnsetMarker
			&& puValue->Set.nMarker2 == GeoUnsetMarker )
			m_pauFields[iField] = *puValue;
		else
			m_pauFields[iField].String = strdup( puValue->String );
	}
	else if( poFDefn->GetType() == gWkbDate
		|| poFDefn->GetType() == gWkbTime
		|| poFDefn->GetType() == gWkbDateTime )
	{
		memcpy( m_pauFields+iField, puValue, sizeof(GeoField) );
	}
	else if( poFDefn->GetType() == gWkbIntegerList )
	{
		int nCount = puValue->IntegerList.nCount;

		if( puValue->Set.nMarker1 == GeoUnsetMarker
			&& puValue->Set.nMarker2 == GeoUnsetMarker )
		{
			m_pauFields[iField] = *puValue;
		}
		else
		{
			m_pauFields[iField].IntegerList.paList =
				(int *) malloc(sizeof(int) * nCount);
			memcpy( m_pauFields[iField].IntegerList.paList,
				puValue->IntegerList.paList,
				sizeof(int) * nCount );
			m_pauFields[iField].IntegerList.nCount = nCount;
		}
	}
	else if( poFDefn->GetType() == gWkbRealList )
	{
		int nCount = puValue->RealList.nCount;

		if( puValue->Set.nMarker1 == GeoUnsetMarker
			&& puValue->Set.nMarker2 == GeoUnsetMarker )
		{
			m_pauFields[iField] = *puValue;
		}
		else
		{
			m_pauFields[iField].RealList.paList =
				(double *) malloc(sizeof(double) * nCount);
			memcpy( m_pauFields[iField].RealList.paList,
				puValue->RealList.paList,
				sizeof(double) * nCount );
			m_pauFields[iField].RealList.nCount = nCount;
		}
	}
	else if( poFDefn->GetType() == gWkbStringList )
	{
		if( puValue->Set.nMarker1 == GeoUnsetMarker
			&& puValue->Set.nMarker2 == GeoUnsetMarker )
		{
			m_pauFields[iField] = *puValue;
		}
	}
	else if( poFDefn->GetType() == gWkbBinary )
	{
		if( puValue->Set.nMarker1 == GeoUnsetMarker
			&& puValue->Set.nMarker2 == GeoUnsetMarker )
		{
			m_pauFields[iField] = *puValue;
		}
		else
		{
			m_pauFields[iField].Binary.nCount = puValue->Binary.nCount;
			m_pauFields[iField].Binary.paData = 
				(GByte *) malloc(puValue->Binary.nCount);
			memcpy( m_pauFields[iField].Binary.paData, 
				puValue->Binary.paData, 
				puValue->Binary.nCount );
		}
	}
	else
		;
}

void GeoFeature::SetField( int iField, char ** papszValues )
{
	GeoFieldDefn *poFDefn = m_poDefn->GetFieldDefn( iField );

	if( poFDefn == NULL )
		return;

	if( poFDefn->GetType() == gWkbStringList )
	{
		GeoField  uField;

		int nItems = 0;
		if (papszValues)
		{
			while(*papszValues != NULL)
			{
				nItems++;
				papszValues++;
			}
		}

		uField.StringList.nCount = nItems;
		uField.Set.nMarker2 = 0;
		uField.StringList.paList = papszValues;

		SetField( iField, &uField );
	}
}

void GeoFeature::SetField( int iField, int nCount, GByte * pabyBinary )
{
	GeoFieldDefn *poFDefn = m_poDefn->GetFieldDefn( iField );

	if( poFDefn == NULL )
		return;

	if( poFDefn->GetType() == gWkbBinary )
	{
		GeoField        uField;

		uField.Binary.nCount = nCount;
		uField.Set.nMarker2 = 0;
		uField.Binary.paData = pabyBinary;

		SetField( iField, &uField );
	}
}

void GeoFeature::SetField( int iField, int nYear, int nMonth, int nDay, 
						  int nHour/* =0 */, int nMinute/* =0 */, int nSecond/* =0 */, int nTZFlag /* = 0 */ )
{
	GeoFieldDefn *poFDefn = m_poDefn->GetFieldDefn( iField );

	if( poFDefn == NULL )
		return;

	if( poFDefn->GetType() == gWkbDate 
		|| poFDefn->GetType() == gWkbTime 
		|| poFDefn->GetType() == gWkbDateTime )
	{
		if( (GInt16)nYear != nYear )
		{
			return;
		}

		m_pauFields[iField].Date.Year = (GInt16)nYear;
		m_pauFields[iField].Date.Month = (GByte)nMonth;
		m_pauFields[iField].Date.Day = (GByte)nDay;
		m_pauFields[iField].Date.Hour = (GByte)nHour;
		m_pauFields[iField].Date.Minute = (GByte)nMinute;
		m_pauFields[iField].Date.Second = (GByte)nSecond;
		m_pauFields[iField].Date.TZFlag = (GByte)nTZFlag;
	}
}

GeoFeature* GeoFeature::CreateFeature(GeoFeatureDefn * poDefn)
{
	return new GeoFeature(poDefn);
}

void GeoFeature::DestroyFeature(GeoFeature * poFeature)
{
	delete poFeature;
	poFeature = NULL;
}
