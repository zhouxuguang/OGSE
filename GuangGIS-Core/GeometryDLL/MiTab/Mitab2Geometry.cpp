
#include <assert.h>
#include <memory.h>
#include <string.h>
#include "GeoFeature.h"
//#include "GeoFeatureDefn.h"
#include "GeoFieldDefn.h"
#include "GeoPoint.h"
#include "GeoLineString.h"
#include "GeoPolygon.h"
#include "GeoMultiGeometry.h"
#include "Mitab2Geometry.h"

GeoGeometry *GeoReadMitabObject(/* mitab_handle hMitabHandle, int iShape, */mitab_feature hShape )
{
	int nGeomType = mitab_c_get_type(hShape);

	GeoGeometry* poGeometry = NULL;

	//点类型
	if (nGeomType == TABFC_Point)
	{
		double dbX = mitab_c_get_vertex_x(hShape,0,0);
		double dbY = mitab_c_get_vertex_y(hShape,0,0);
		poGeometry = new GeoPoint(dbX,dbY);
	}

	//多点类型
	else if (nGeomType == TABFC_MultiPoint)
	{
		//if (psShape->nVertices == 0)
		//{
		//	poGeometry = NULL;
		//}

		//GeoMultiPoint* poMultiPoint = new GeoMultiPoint();
		//for (int i = 0; i < psShape->nVertices; i ++)
		//{
		//	for( int i = 0; i < psShape->nVertices; i++ )
		//	{
		//		GeoPoint *poPoint = NULL;

		//		if( psShape->nSHPType == SHPT_MULTIPOINTZ 
		//			|| psShape->nSHPType == SHPT_MULTIPOINTM)
		//		{
		//			//
		//		}
		//		else
		//			poPoint = new GeoPoint( psShape->padfX[i], psShape->padfY[i] );

		//		poMultiPoint->AddGeometry(poPoint);

		//		delete poPoint;
		//	}

		//	poGeometry = poMultiPoint;
		//}
	}

	//弧段类型linestring
	else if( nGeomType == TABFC_Polyline )
	{
		int nParts = mitab_c_get_parts(hShape);
		if( nParts == 0 )
		{
			poGeometry = NULL;
		}

		//一个线串
		else if( nParts == 1 )
		{
			int nPointCount = mitab_c_get_vertex_count(hShape,0);
			GeoLineString *poLine = new GeoLineString();
			int nIndex = 0;
			for (; nIndex < nPointCount; nIndex ++)
			{
				poLine->AddPoint(mitab_c_get_vertex_x(hShape,0,nIndex),
					mitab_c_get_vertex_y(hShape,0,nIndex));
			}

			poGeometry = poLine;
		}

		//多线串
		else
		{
			int iRing;
			GeoMultiLineString *poMultiLine;

			poGeometry = poMultiLine = new GeoMultiLineString();

			for( iRing = 0; iRing < nParts; iRing++ )
			{
				GeoLineString   *poLine = NULL;
				int     nRingPoints = 0;
				int     nRingStart = 0;

				poLine = new GeoLineString();

				//if( psShape->panPartStart == NULL )
				//{
				//	nRingPoints = psShape->nVertices;
				//	nRingStart = 0;
				//}
				//else
				//{

				//	if( iRing == psShape->nParts - 1 )
				//		nRingPoints = psShape->nVertices - psShape->panPartStart[iRing];
				//	else
				//		nRingPoints = psShape->panPartStart[iRing+1]
				//	- psShape->panPartStart[iRing];
				//	nRingStart = psShape->panPartStart[iRing];
				//}

				//if( psShape->nSHPType == SHPT_ARCZ )
				//{
				//	//
				//}
				//else if( psShape->nSHPType == SHPT_ARCM )
				//{
				//	//
				//}
				//else
				//	poLine->SetPoints( nRingPoints,
				//	psShape->padfX + nRingStart,
				//	psShape->padfY + nRingStart );

				//poMultiLine->AddGeometry( poLine );
			}
		}
	}

	//多边形
	if (nGeomType == TABFC_Region)
	{
		int nParts = mitab_c_get_parts(hShape);
		if (nParts < 1)
		{
			poGeometry = NULL;
		}

		//简单多边形，不包含岛的情况
		else if (nParts == 1)
		{
			GeoPolygon* poPolygon = new GeoPolygon();
			GeoLineString *poLine = new GeoLineString();

			int nPointCount = mitab_c_get_vertex_count(hShape,0);
			int nIndex = 0;
			for (; nIndex < nPointCount; nIndex ++)
			{
				poLine->AddPoint(mitab_c_get_vertex_x(hShape,0,nIndex),
					mitab_c_get_vertex_y(hShape,0,nIndex));
			}

			poPolygon->AddRing(poLine);
			poGeometry = poPolygon;
		}

		//带岛多边形
		else if (nParts > 1)
		{
			GeoPolygon* poPolygon = NULL;
			poGeometry = poPolygon = new GeoPolygon();

			for (int iRing = 0; iRing < nParts; iRing ++)
			{
				GeoLineString* poRing = new GeoLineString();
				//int     nRingPoints;
				//int     nRingStart;

				//获得每一个段点的个数
				int nPointCount = mitab_c_get_vertex_count(hShape,iRing);
				for (int i = 0;i < nPointCount; i ++)
				{
					poRing->AddPoint(mitab_c_get_vertex_x(hShape,iRing,i),
						mitab_c_get_vertex_y(hShape,iRing,i));
				}

				poPolygon->AddRing(poRing);
			}

			poGeometry = poPolygon;
		}
	}

	////patch，其中有包括多多边形
	//if (psShape->nSHPType == SHPT_MULTIPATCH)
	//{
	//}

	return poGeometry;
	
	//return NULL;
}

GeoFeature *GeoReadMitabFeature( mitab_handle hMitabHandle, int iShape
								,GeoFeatureDefn * poDefn)
{
	//如果mitab_handle为空直接返回空
	if (hMitabHandle == NULL)
	{
		return NULL;
	}

	int nFeatureCount = mitab_c_get_feature_count(hMitabHandle);
	if (iShape < 0 || iShape >= nFeatureCount)
	{
		return NULL;
	}

	//mitab要素s
	mitab_feature hMitabFeature = mitab_c_read_feature(hMitabHandle,iShape+1);

	GeoFeature *poFeature = new GeoFeature(poDefn);

	/* -------------------------------------------------------------------- */
	/*      从mapinfo文件中提取几何体到要素中                             */
	/* -------------------------------------------------------------------- */
	if( hMitabHandle != NULL )
	{
		if( !poDefn->IsGeometryIgnored() )
		{
			GeoGeometry* poGeometry = NULL;
			poGeometry = GeoReadMitabObject(hMitabFeature);

			poFeature->SetGeometryDirectly( poGeometry );
			delete poGeometry;
			poGeometry = NULL;
		}
		else if( hMitabFeature != NULL )
		{
			//mitab_c_destroy_feature( hMitabFeature );
		}
	}


	/* -------------------------------------------------------------------- */
	/*      从mapinfo文件中提取属性数据到要素中                           */
	/* -------------------------------------------------------------------- */
	for( int iField = 0; iField < poDefn->GetFieldCount(); iField++ )
	{
		GeoFieldDefn* poFieldDefn = poDefn->GetFieldDefn(iField);
		if (poFieldDefn->IsIgnored() )
			continue;

		switch( poFieldDefn->GetType() )
		{
		case gWkbString:
			{
				const char *pszFieldVal = 
					mitab_c_get_field_as_string( hMitabFeature, iField );
				if( pszFieldVal != NULL && pszFieldVal[0] != '\0' )
				{
					poFeature->SetField( iField, pszFieldVal );
				}
			}
			break;

		case gWkbInteger:
			{
				int nValue = (int)mitab_c_get_field_as_double(hMitabFeature,iField);
				poFeature->SetField(iField,nValue);
				break;
			}

		case gWkbReal:
			{
				double dbValue = mitab_c_get_field_as_double(hMitabFeature,iField);
				poFeature->SetField(iField,dbValue);
				break;
			}

		case gWkbDate:
			{
				GeoField sFld;
				const char* pszDateValue = 
					mitab_c_get_field_as_string(hMitabFeature,iField);

				if (pszDateValue[0] == '\0')
					continue;

				memset( &sFld, 0, sizeof(sFld) );

				if( strlen(pszDateValue) >= 10 &&
					pszDateValue[2] == '/' && pszDateValue[5] == '/' )
				{
					sFld.Date.Month = (GByte)atoi(pszDateValue+0);
					sFld.Date.Day   = (GByte)atoi(pszDateValue+3);
					sFld.Date.Year  = (GInt16)atoi(pszDateValue+6);
				}
				else
				{
					int nFullDate = atoi(pszDateValue);
					sFld.Date.Year = (GInt16)(nFullDate / 10000);
					sFld.Date.Month = (GByte)((nFullDate / 100) % 100);
					sFld.Date.Day = (GByte)(nFullDate % 100);
				}

				poFeature->SetField( iField, &sFld );
			}
			break;

		default:
			assert( false );
		}
	}

	if( poFeature != NULL )
		poFeature->SetFID( iShape );

	if (hMitabFeature != NULL)
	{
		mitab_c_destroy_feature(hMitabFeature);
	}

	return poFeature;
}

GeoFeatureDefn *GeoReadMitabFeatureDefn(const char *pszName,mitab_handle hMitabHandle)
{
	GeoFeatureDefn *poDefn = new GeoFeatureDefn(pszName);
	int iField;
	int nFieldCount = mitab_c_get_field_count(hMitabHandle);

	for( iField = 0; hMitabHandle != NULL && iField < nFieldCount; iField++ )
	{
		int nWidth = 0;				//宽度
		int nPrecision = 0;			//精度
		GeoFieldDefn    oField;
		int            nFieldType;

		nFieldType = mitab_c_get_field_type( hMitabHandle, iField );
		nWidth = mitab_c_get_field_width(hMitabHandle,iField);
		nPrecision = mitab_c_get_field_precision(hMitabHandle,iField);
		const char *szFieldName = mitab_c_get_field_name(hMitabHandle,iField);

		oField.SetName( szFieldName );
		oField.SetWidth( nWidth );
		oField.SetPrecision( nPrecision );

		if( nFieldType == TABFT_Date )
		{
			oField.SetWidth( nWidth + 2 );
			oField.SetType( gWkbDate );
		}
		else if( nFieldType == TABFT_Float )
		{
			oField.SetType( gWkbReal );
		}
		else if( nFieldType == TABFT_Integer )
		{
			oField.SetType( gWkbInteger );
		}
		else if (nFieldType == TABFT_Char)
		{
			oField.SetType( gWkbString );
		}

		poDefn->AddFieldDefn( &oField );
	}

	if( hMitabHandle == NULL )
	{
		poDefn->SetGeomType( gWkbNone );
	}

	int nFeatureCount = mitab_c_get_feature_count(hMitabHandle);
	if (nFeatureCount >= 1)
	{
		mitab_feature hFeature = mitab_c_read_feature(hMitabHandle,1);

		int nGeomType = mitab_c_get_type(hMitabHandle);

		//获得几何体类型
		switch( nGeomType )
		{
		case TABFC_Point:
			poDefn->SetGeomType( gWkbPoint );
			break;

		case TABFC_Polyline:
			poDefn->SetGeomType( gWkbLineString );
			break;

		case TABFC_Arc:
			poDefn->SetGeomType( gWkbLineString );
			break;

		case TABFC_Region:
		case TABFC_Rectangle:
			poDefn->SetGeomType( gWkbPolygon );
			break;

		}
	}

	

	return poDefn;
}

