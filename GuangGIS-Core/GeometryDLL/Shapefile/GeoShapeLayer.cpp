#include "GeoGeometry.h"
#include "GeoEnvelope.h"
#include "GeoFeature.h"
#include "GeoFieldDefn.h"
#include "Shape2Geometry.h"
#include "GeoShapeLayer.h"
#include "GeoSpatialReference.h"
#include "GeoGeometry.h"

#if defined _WIN32 || defined _WIN64
#include <io.h>
#endif

GeoShapeLayer::GeoShapeLayer(GeoDataSource* poDSIn,
							 const char * pszName,
							 SHPHandle hSHP, DBFHandle hDBF,
							 GeoSpatialReference *poSRS, int bSRSSet,
							 int bUpdate, 
							 GeoWkbGeometryType eReqType)
{
	iNextShapeId = 0;
	m_poDS = poDSIn;
	m_pszFullName = strdup(pszName);
	m_hSHP = hSHP;
	m_hDBF = hDBF;
	m_poSRS = poSRS;
	m_bSRSSet = bSRSSet;
	m_bUpdateAccess = bUpdate;
	m_eGeomType = eReqType;

	m_nFeatureCount = -1;
	if (m_hSHP != NULL)
	{
		SHPGetInfo(m_hSHP,&m_nFeatureCount,NULL,NULL,NULL);
	}

	m_poFeatureDefn = GeoReadFeatureDefn(m_pszFullName,m_hSHP,m_hDBF);
}

GeoShapeLayer::~GeoShapeLayer(void)
{
	if (m_pszFullName != NULL)
	{
		free(m_pszFullName);
		m_pszFullName = NULL;
	}

	if (m_hSHP != NULL)
	{
		SHPClose(m_hSHP);
		m_hSHP = NULL;
	}

	if (m_hDBF != NULL)
	{
		DBFClose(m_hDBF);
		m_hDBF = NULL;
	}
	
}

GeoGeometry *GeoShapeLayer::FetchShape(int iShapeId)
{
	GeoGeometry* poGeometry = NULL;
	if (m_hDBF != NULL && m_hSHP != NULL)
	{
		poGeometry = GeoReadSHPObject(m_hSHP,iShapeId,NULL);
	}

	return poGeometry;
}

GeoFeature* GeoShapeLayer::GetFeature( long nFId )
{
	GeoFeatureDefn* poDefn = NULL;
	GeoFeature* poFeature = NULL;
	if (m_hDBF != NULL && m_hSHP != NULL)
	{
		poDefn = GeoReadFeatureDefn(m_pszFullName,m_hSHP,m_hDBF);
		poFeature = GeoReadFeature(m_hSHP,m_hDBF,poDefn,nFId,NULL);
	}

	delete poDefn;
	poDefn = NULL;

	return poFeature;
}

GeoGeometry* GeoShapeLayer::GetGeometryRef(long nFID)
{
	if (m_hSHP != NULL && m_hDBF != NULL)
	{
		return GeoReadSHPObject( m_hSHP, nFID, NULL );
	}

	return NULL;
}

GeoSpatialReference* GeoShapeLayer::GetSpatialRef() const
{
	//判断prj文件是否存在、存在就读取，不存在就不读取
	static GeoSpatialReference* poSRS = NULL;
	std::string strFileName = std::string(m_pszFullName);
	int nPos = strFileName.find_last_of('.');
	strFileName = strFileName.replace(nPos+1,3,"prj");
	if (0 == access(strFileName.c_str(),06))		//存在这个文件
	{
		poSRS = new GeoSpatialReference;

		//读取prj文件
		FILE *fpPrj = fopen(strFileName.c_str(),"r");
		if (NULL == fpPrj)
		{
			fclose(fpPrj);
			return NULL;
		}
		fseek(fpPrj,0,SEEK_END);
		long nFileLen = ftell(fpPrj);
		fseek(fpPrj,0,SEEK_SET);

		char *pszWkt = new char[nFileLen+1];
		fread(pszWkt,1,nFileLen,fpPrj);
		fclose(fpPrj);
		pszWkt[nFileLen] = '\0';
		poSRS->ImportFromWkt(pszWkt);

		if (pszWkt != NULL)
		{
			delete []pszWkt;
			pszWkt = NULL;
		}

		return poSRS;
	}

	return NULL;
}

GeoWkbGeometryType GeoShapeLayer::GetGeometryType() const
{
	return m_eGeomType;
}

bool GeoShapeLayer::UpdateFeature( GeoFeature *poFeature )
{
	if( !m_bUpdateAccess )
	{
		return false;
	}

	long nFID = poFeature->GetFID();
	if( nFID < 0
		|| (m_hSHP != NULL && nFID >= m_hSHP->nRecords)
		|| (m_hDBF != NULL && nFID >= m_hDBF->nRecords) )
	{
		char szMsg[256];
		sprintf(szMsg,"FID超出范围:%d",nFID);
		return false;
	}

	return GeoWriteFeature( m_hSHP, m_hDBF, m_poFeatureDefn, poFeature,m_hDBF->pszCodePage);
}

bool GeoShapeLayer::CreateFeature( GeoFeature *poFeature )
{
	//如果不是更新方式打开，则直接返回false
	if (!m_bUpdateAccess)
	{
		return false;
	}

	if( poFeature->GetGeometry() != NULL )
	{
		GeoGeometry *poGeom = poFeature->GetGeometry();
		int nShapeType = -1;

		switch( poGeom->GetGeometryType() )
		{
		case gWkbPoint:
			nShapeType = SHPT_POINT;
			break;

		case gWkbPoint25D:
			nShapeType = SHPT_POINTZ;
			break;

		case gWkbMultiPoint:
			nShapeType = SHPT_MULTIPOINT;
			break;

		case gWkbMultiPoint25D:
			nShapeType = SHPT_MULTIPOINTZ;
			break;

		case gWkbLineString:
		case gWkbMultiLineString:
			nShapeType = SHPT_ARC;
			break;

		case gWkbLineString25D:
		case gWkbMultiLineString25D:
			nShapeType = SHPT_ARCZ;
			break;

		case gWkbPolygon:
		case gWkbMultiPolygon:
			nShapeType = SHPT_POLYGON;
			break;

		case gWkbPolygon25D:
		case gWkbMultiPolygon25D:
			nShapeType = SHPT_POLYGONZ;
			break;

		default:
			nShapeType = -1;
			break;
		}

	}

	bool bFlag = GeoWriteFeature(m_hSHP,m_hDBF,m_poFeatureDefn,poFeature,m_hDBF->pszCodePage);

	if( m_hSHP != NULL )
		m_nFeatureCount = m_hSHP->nRecords;
	else 
		m_nFeatureCount = m_hDBF->nRecords;

	return bFlag;
}

bool GeoShapeLayer::DeleteFeature( int nIndex,bool bDelete )
{
	return false;
}

bool GeoShapeLayer::CreateField( GeoFieldDefn *poField, bool bApproxOK /* = true */ )
{
	return true;
}

bool GeoShapeLayer::GetEnvelope(GeoEnvelope* env)
{
	if (NULL == env)
	{
		return false;
	}
	double adfMinBound[4];		//最小值
	double adfMaxBound[4];		//最大值
	if (m_hSHP != NULL)
	{
		SHPGetInfo(m_hSHP,NULL,NULL,adfMinBound,adfMaxBound);
		env->minX = adfMinBound[0];
		env->minY = adfMinBound[1];
		env->maxX = adfMaxBound[0];
		env->maxY = adfMaxBound[1];

		return true;
	}

	return false;
}

void GeoShapeLayer::SetEnvelope(GeoEnvelope* pEnv)
{
	return;
}

int GeoShapeLayer::GetFetureCount() const
{
	if (m_nFeatureCount == -1 && m_hSHP != NULL)
	{
		SHPGetInfo(m_hSHP,(int*)&m_nFeatureCount,NULL,NULL,NULL);
	}

	/*else if (m_nFeatureCount != -1)
	{
		return m_nFeatureCount;
	}*/

	return m_nFeatureCount;
	
}

