
#include <assert.h>
#include "GeoGeometry.h"
#include "GeoPolygon.h"
#include "GeoPoint.h"
#include "GeoGeometryCollection.h"
#include "GeoMultiGeometry.h"
#include "GeoLineString.h"
#include "GeoFeature.h"
#include "GeoFeatureDefn.h"
#include "GeoFieldDefn.h"
#include "Shape2Geometry.h"

#include <stdlib.h>
#include <memory.h>


GeoGeometry *GeoReadSHPObject( SHPHandle hSHP, int iShape, SHPObject *psShape )
{
	GeoGeometry* poGeometry = NULL;

	if( psShape == NULL )
	{
		psShape = SHPReadObject( hSHP, iShape );
	}

	if( psShape == NULL )
	{
		return poGeometry;
	}

	//点类型
	else if (psShape->nSHPType == SHPT_POINT)
	{
		poGeometry = new GeoPoint(psShape->padfX[0],psShape->padfY[0]);
	}
	else if (psShape->nSHPType == SHPT_POINTZ)
	{
	}
	else if (psShape->nSHPType == SHPT_POINTM)
	{
	}

	//多点类型
	else if (psShape->nSHPType == SHPT_MULTIPOINT
		|| psShape->nSHPType == SHPT_MULTIPOINTM
		|| psShape->nSHPType == SHPT_MULTIPOINTZ)
	{
		if (psShape->nVertices == 0)
		{
			poGeometry = NULL;
		}

		GeoMultiPoint* poMultiPoint = new GeoMultiPoint();
		for (int i = 0; i < psShape->nVertices; i ++)
		{
			for( int i = 0; i < psShape->nVertices; i++ )
			{
				GeoPoint *poPoint = NULL;

				if( psShape->nSHPType == SHPT_MULTIPOINTZ 
					|| psShape->nSHPType == SHPT_MULTIPOINTM)
				{
					//
				}
				else
					poPoint = new GeoPoint( psShape->padfX[i], psShape->padfY[i] );

				poMultiPoint->AddGeometry(poPoint);

				//delete poPoint;
			}

			poGeometry = poMultiPoint;
		}
	}

	//弧段类型linestring
	else if( psShape->nSHPType == SHPT_ARC
		|| psShape->nSHPType == SHPT_ARCM
		|| psShape->nSHPType == SHPT_ARCZ )
	{
		if( psShape->nParts == 0 )
		{
			poGeometry = NULL;
		}

		//一个线串
		else if( psShape->nParts == 1 )
		{
			GeoLineString *poLine = new GeoLineString();

			if( psShape->nSHPType == SHPT_ARCZ )
			{

			}
			else if( psShape->nSHPType == SHPT_ARCM )
			{

			}
			else
				poLine->SetPoints(psShape->nVertices,psShape->padfX,psShape->padfY);

			poGeometry = poLine;
		}

		//多线串
		else
		{
			int iRing;
			GeoMultiLineString *poMultiLine;

			poGeometry = poMultiLine = new GeoMultiLineString();

			for( iRing = 0; iRing < psShape->nParts; iRing++ )
			{
				GeoLineString   *poLine = NULL;
				int     nRingPoints = 0;
				int     nRingStart = 0;

				poLine = new GeoLineString();

				if( psShape->panPartStart == NULL )
				{
					nRingPoints = psShape->nVertices;
					nRingStart = 0;
				}
				else
				{

					if( iRing == psShape->nParts - 1 )
						nRingPoints = psShape->nVertices - psShape->panPartStart[iRing];
					else
						nRingPoints = psShape->panPartStart[iRing+1]
					- psShape->panPartStart[iRing];
					nRingStart = psShape->panPartStart[iRing];
				}

				if( psShape->nSHPType == SHPT_ARCZ )
				{
					//
				}
				else if( psShape->nSHPType == SHPT_ARCM )
				{
					//
				}
				else
					poLine->SetPoints( nRingPoints,
					psShape->padfX + nRingStart,
					psShape->padfY + nRingStart );

				poMultiLine->AddGeometry( poLine );
			}
		}
	}

	//多边形
	if (psShape->nSHPType == SHPT_POLYGON
		|| psShape->nSHPType == SHPT_POLYGONZ
		|| psShape->nSHPType == SHPT_POLYGONM)
	{
		if (psShape->nVertices == 0)
		{
			poGeometry = NULL;
		}

		//简单多边形，不包含岛的情况
		else if (psShape->nParts == 1)
		{
			GeoPolygon* poPolygon = new GeoPolygon();
			GeoLineString *poLine = new GeoLineString();

			if( psShape->nSHPType == SHPT_POLYGONZ )
			{

			}
			else if( psShape->nSHPType == SHPT_POLYGONM )
			{

			}
			else
				poLine->SetPoints(psShape->nVertices,psShape->padfX,psShape->padfY);

			poPolygon->AddRing(poLine);
			delete poLine;
			poGeometry = poPolygon;
		}

		//带岛多边形
		else if (psShape->nParts > 1)
		{
			GeoPolygon* poPolygon = NULL;
			poGeometry = poPolygon = new GeoPolygon();

			for (int iRing = 0; iRing < psShape->nParts; iRing ++)
			{
				GeoLineString* poRing = new GeoLineString();
				int     nRingPoints;
				int     nRingStart;

				//以下计算每个线串的点个数以及起始索引
				if (iRing == psShape->nParts-1)
				{
					nRingPoints = psShape->nVertices - psShape->panPartStart[iRing];
					nRingStart = psShape->panPartStart[iRing];
				}
				else
					nRingPoints = psShape->panPartStart[iRing+1] - psShape->panPartStart[iRing];
					nRingStart = psShape->panPartStart[iRing];

				if( psShape->nSHPType == SHPT_POLYGONZ )
				{
					//
				}
				else if( psShape->nSHPType == SHPT_POLYGONM )
				{
					//
				}
				else
					poRing->SetPoints( nRingPoints,
					psShape->padfX + nRingStart,
					psShape->padfY + nRingStart );

				poPolygon->AddRing(poRing);
			}
		}
	}

	//patch，其中有包括多多边形
	if (psShape->nSHPType == SHPT_MULTIPATCH)
	{
	}

	if (psShape != NULL)
	{
		SHPDestroyObject(psShape);
		psShape = NULL;
	}

	return poGeometry;
}

GeoFeatureDefn* GeoReadFeatureDefn( const char * pszName,
								   SHPHandle hSHP, DBFHandle hDBF )
{
	GeoFeatureDefn  *poDefn = new GeoFeatureDefn( pszName );
    int iField = 0;
	int nFieldCount = DBFGetFieldCount( hDBF );

    for( iField = 0; hDBF != NULL && iField < nFieldCount; iField++ )
    {
        char szFieldName[20];		//字段名字
        int nWidth = 0;				//宽度
		int nPrecision = 0;			//精度
        DBFFieldType    eDBFType;
        GeoFieldDefn    oField;
        char            chNativeType;

        chNativeType = DBFGetNativeFieldType( hDBF, iField );
        eDBFType = DBFGetFieldInfo( hDBF, iField, szFieldName,
                                    &nWidth, &nPrecision );

        oField.SetName( szFieldName );
        oField.SetWidth( nWidth );
        oField.SetPrecision( nPrecision );

        if( chNativeType == 'D' )
        {
            oField.SetWidth( nWidth + 2 );
            oField.SetType( gWkbDate );
        }
        else if( eDBFType == FTDouble )
		{
            oField.SetType( gWkbReal );
		}
        else if( eDBFType == FTInteger )
		{
            oField.SetType( gWkbInteger );
		}
        else
		{
            oField.SetType( gWkbString );
		}

        poDefn->AddFieldDefn( &oField );
    }

    if( hSHP == NULL )
	{
        poDefn->SetGeomType( gWkbNone );
	}
    else
    {
        switch( hSHP->nShapeType )
        {
          case SHPT_POINT:
            poDefn->SetGeomType( gWkbPoint );
            break;

          case SHPT_POINTZ:
          case SHPT_POINTM:
            poDefn->SetGeomType( gWkbPoint25D );
            break;

          case SHPT_ARC:
            poDefn->SetGeomType( gWkbLineString );
            break;

          case SHPT_ARCZ:
          case SHPT_ARCM:
            poDefn->SetGeomType( gWkbLineString25D );
            break;

          case SHPT_MULTIPOINT:
            poDefn->SetGeomType( gWkbMultiPoint );
            break;

          case SHPT_MULTIPOINTZ:
          case SHPT_MULTIPOINTM:
            poDefn->SetGeomType( gWkbMultiPoint25D );
            break;

          case SHPT_POLYGON:
            poDefn->SetGeomType( gWkbPolygon );
            break;

          case SHPT_POLYGONZ:
          case SHPT_POLYGONM:
            poDefn->SetGeomType( gWkbPolygon25D );
            break;
            
        }
    }

    return poDefn;
	
}

GeoFeature *GeoReadFeature( SHPHandle hSHP, DBFHandle hDBF,
						   GeoFeatureDefn * poDefn, int iShape, SHPObject *psShape )
{
	if( iShape < 0 
        || (hSHP != NULL && iShape >= hSHP->nRecords)
        || (hDBF != NULL && iShape >= hDBF->nRecords) )
    {
        return NULL;
    }

    if( hDBF && DBFIsRecordDeleted( hDBF, iShape ) )
    {
        return NULL;
    }

    GeoFeature  *poFeature = new GeoFeature( poDefn );

	/* -------------------------------------------------------------------- */
	/*      从shapefile文件中提取几何体到要素中                             */
	/* -------------------------------------------------------------------- */
    if( hSHP != NULL )
    {
        if( !poDefn->IsGeometryIgnored() )
        {
            GeoGeometry* poGeometry = NULL;
            poGeometry = GeoReadSHPObject( hSHP, iShape, psShape );

            
			if (poGeometry != NULL)
			{
				poFeature->SetGeometryDirectly( poGeometry );
				delete poGeometry;
				poGeometry = NULL;
			}
			
        }
        else if( psShape != NULL )
        {
            SHPDestroyObject( psShape );
        }
    }

	/* -------------------------------------------------------------------- */
	/*      从shapefile文件中提取属性数据到要素中                           */
	/* -------------------------------------------------------------------- */

    for( int iField = 0; iField < poDefn->GetFieldCount(); iField++ )
    {
        GeoFieldDefn* poFieldDefn = poDefn->GetFieldDefn(iField);
        if (poFieldDefn->IsIgnored() )
            //continue;

        switch( poFieldDefn->GetType() )
        {
          case gWkbString:
          {
              const char *pszFieldVal = 
                  DBFReadStringAttribute( hDBF, iShape, iField );
              if( pszFieldVal != NULL && pszFieldVal[0] != '\0' )
              {
				  poFeature->SetField( iField, pszFieldVal );
              }
          }
          break;

          case gWkbInteger:

            if( !DBFIsAttributeNULL( hDBF, iShape, iField ) )
                poFeature->SetField( iField,
                                    DBFReadIntegerAttribute( hDBF, iShape,
                                                             iField ) );
            break;

          case gWkbReal:
            if( !DBFIsAttributeNULL( hDBF, iShape, iField ) )
                poFeature->SetField( iField,
                                    DBFReadDoubleAttribute( hDBF, iShape,
                                                            iField ) );
            break;

          case gWkbDate:
          {
              GeoField sFld;
              if( DBFIsAttributeNULL( hDBF, iShape, iField ) )
                  continue;

              const char* pszDateValue = 
                  DBFReadStringAttribute(hDBF,iShape,iField);

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

    return poFeature;
}

bool GeoWriteSHPObject(SHPHandle hSHP, int iShape, GeoGeometry* poGeometry)
{
	if (hSHP == NULL)
	{
		return NULL;
	}

	int nShapeID = -1;

	SHPObject *psShape = NULL;

	if( poGeometry == NULL/* || poGeometry->IsEmpty()*/ )
	{
		psShape = SHPCreateSimpleObject( SHPT_NULL, 0, NULL, NULL, NULL );
		nShapeID = SHPWriteObject( hSHP, iShape, psShape );
		SHPDestroyObject( psShape );
		if( nShapeID == -1 )
		{
			return false;
		}
	}

	//点类型
	if (poGeometry->GetGeometryType() == gWkbPoint)
	{
		GeoPoint* poPoint = (GeoPoint*)poGeometry;
		double x = poPoint->GetX();
		double y = poPoint->GetY();
		psShape = SHPCreateSimpleObject(SHPT_POINT,1,&x,&y,NULL);
		nShapeID = SHPWriteObject(hSHP,iShape,psShape);
		SHPDestroyObject(psShape);
		if (nShapeID == -1)
		{
			return false;
		}
	}

	//多点类型
	else if (poGeometry->GetGeometryType() == gWkbMultiPoint)
	{
		GeoMultiPoint* poMultiPoint = (GeoMultiPoint*)poGeometry;
		int nGeomNums = poMultiPoint->GetNumGeoms();
		double *padfX = new double[nGeomNums];
		double *padfY = new double[nGeomNums];
		double *padfZ = new double[nGeomNums];

		int iDstPoints = 0;
		int i = 0;
		for( i = 0; i < nGeomNums; i++ )
		{
			GeoPoint* poPoint = (GeoPoint*)poMultiPoint->GetGeometryRefs(i);

			padfX[iDstPoints] = poPoint->GetX();
			padfY[iDstPoints] = poPoint->GetX();
			padfZ[iDstPoints] = poPoint->GetX();
			iDstPoints ++;
		}

		psShape = SHPCreateSimpleObject( SHPT_MULTIPOINT,
			iDstPoints,padfX, padfY, padfZ );
		nShapeID = SHPWriteObject( hSHP, iShape, psShape );
		SHPDestroyObject( psShape );

		delete []padfX;
		delete []padfY;
		delete []padfZ;
		if( nShapeID == -1 )
		{
			return false;
		}
	}

	//折线
	else if (poGeometry->GetGeometryType() == gWkbLineString)
	{
		GeoLineString *poLine = (GeoLineString*)poGeometry;
		int nPointCount = poLine->GetNumPoint();
		double *dbX = new double[nPointCount];
		double *dbY = new double[nPointCount];
		poLine->GetPoints(dbX,dbY);
		psShape = SHPCreateSimpleObject(SHPT_ARC,nPointCount,dbX,dbY,NULL);
		nShapeID = SHPWriteObject(hSHP,iShape,psShape);
		delete []dbX;
		delete []dbY;
		SHPDestroyObject(psShape);
	}

	//多条折线
	else if (poGeometry->GetGeometryType() == gWkbMultiLineString)
	{
	}

	//多边形
	else if (poGeometry->GetGeometryType() == gWkbPolygon)
	{
		GeoPolygon* poPolygon = (GeoPolygon*)poGeometry;
		int nRingCount = poPolygon->GetNumRings();
		int *panPartStart = new int[nRingCount];

		int nPointCount = 0;
		for (int iRing = 0; iRing < nRingCount; iRing ++)
		{
			panPartStart[iRing] = nPointCount;

			GeoLineString* poLine = poPolygon->GetRing(iRing);
			if (NULL != poLine)
			{
				nPointCount += poLine->GetNumPoint();
			}
			 
		}

		int nCount = nPointCount;
		double *padX = new double[nCount];
		double *padY = new double[nCount];
		nPointCount = 0;
		for (int iRing = 0; iRing < nRingCount; iRing ++)
		{
			GeoLineString* poLine = poPolygon->GetRing(iRing);
			if (NULL != poLine)
			{
				poLine->GetPoints(padX+nPointCount,padY+nPointCount);
				nPointCount += poLine->GetNumPoint();
			}

		}
		psShape = SHPCreateObject(SHPT_POLYGON,iShape,nRingCount,panPartStart,NULL,nRingCount,padX,padY,NULL,NULL);
		nShapeID = SHPWriteObject(hSHP,iShape,psShape);
		delete []padX;
		delete []padY;
		delete []panPartStart;
		SHPDestroyObject(psShape);
	}

	//多多边形
	else if (poGeometry->GetGeometryType() == gWkbMultiPolygon)
	{
	}
	
	return poGeometry;
}

bool GeoWriteFeature(SHPHandle hSHP,DBFHandle hDBF,GeoFeatureDefn * poDefn,
					 GeoFeature* poFeature,char* pszEncode)
{
	bool bFlag = false;

	if( hSHP != NULL )
	{
		bFlag = GeoWriteSHPObject( hSHP, poFeature->GetFID(),poFeature->GetGeometry() );
		if( bFlag == false )
			return bFlag;
	}

	//如果没有DBF文件直接结束
	if( hDBF == NULL )
	{
		if( hSHP != NULL )
			poFeature->SetFID( hSHP->nRecords - 1 );

		return false;
	}

	//如果是一个新的要素，在分配合适的FID
	if( poFeature->GetFID() == -1 )
		poFeature->SetFID( DBFGetRecordCount( hDBF ) );

	//如果是第一个要素插入并且没有合适的字段，则创建一个整形字段存储FID
	if( DBFGetRecordCount( hDBF ) == 0 && DBFGetFieldCount( hDBF ) == 0 )
	{
		DBFAddField( hDBF, "FID", FTInteger, 11, 0 );
	}

	if( DBFGetFieldCount( hDBF ) == 1 && poDefn->GetFieldCount() == 0 )
	{
		DBFWriteIntegerAttribute( hDBF, poFeature->GetFID(), 0, poFeature->GetFID() );
	}

	/* -------------------------------------------------------------------- */
	/*      写入所有的字段                                                  */
	/* -------------------------------------------------------------------- */
	for( int iField = 0; iField < poDefn->GetFieldCount(); iField++ )
	{
		GeoFieldDefn* poFieldDefn = poDefn->GetFieldDefn(iField);

		switch( poFieldDefn->GetType() )
		{
		case gWkbString:
			{
				const char *pszStr = poFeature->GetFieldAsString(iField);
				int nStrLen = (int) strlen(pszStr);
				if (nStrLen > 254)
				{
					nStrLen = 254;
				}

				if ( nStrLen > poFieldDefn->GetWidth() )
				{
					return false;
				}

				DBFWriteStringAttribute( hDBF, poFeature->GetFID(), iField,pszStr );
			}
			break;

		case gWkbInteger:
			{
				char szFormat[20];
				char szValue[20];
				int nFieldWidth = poFieldDefn->GetWidth();
				sprintf(szFormat, "%%%dd", nFieldWidth);
				sprintf(szValue, szFormat, poFeature->GetFieldAsInteger(iField) );
				int nStrLen = strlen(szValue);

				DBFWriteAttributeDirectly( hDBF, poFeature->GetFID(), iField, szValue );
				break;
			}

		case gWkbReal:
			DBFWriteDoubleAttribute( hDBF, poFeature->GetFID(), iField, 
				poFeature->GetFieldAsDouble(iField) );
			break;

		case gWkbDate:
			{
				int  nYear, nMonth, nDay;

				if( poFeature->GetFieldAsDateTime( iField, &nYear, &nMonth, &nDay,
					NULL, NULL, NULL, NULL ) )
				{
					if( nYear < 0 || nYear > 9999 )
					{
						return false;
					}
					else
						DBFWriteIntegerAttribute( hDBF, poFeature->GetFID(), iField, 
						nYear*10000 + nMonth*100 + nDay );
				}
			}
			break;

		default:
			{
				/* 忽略其他类型字段 */
				break;
			}
		}

	}

	return true;
}
