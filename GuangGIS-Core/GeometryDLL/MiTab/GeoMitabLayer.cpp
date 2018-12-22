
#include <assert.h>
#include "GeoEnvelope.h"
#include "GeoGeometry.h"
#include "GeoFieldDefn.h"
#include "GeoFeature.h"
#include "Mitab2Geometry.h"
#include "GeoMitabLayer.h"
#include "GeoSpatialReference.h"

GeoMitabLayer::GeoMitabLayer(GeoDataSource  *poDS,GeoSpatialReference *poSRS,const char* pszLayerName
							 ,mitab_handle hMitab,bool bUpdate)
{
	m_poDS = poDS;
	m_poSRS = poSRS;
	m_pszFullName = strdup(pszLayerName);
	m_hMitabHandle = hMitab;
	m_bUpdateAccess = bUpdate;

	if (m_hMitabHandle != NULL)
	{
		m_nFeatureCount = mitab_c_get_feature_count(m_hMitabHandle);
	}
	else
	{
		m_hMitabHandle = 0;
	}
}

GeoMitabLayer::~GeoMitabLayer(void)
{
}

bool GeoMitabLayer::GetEnvelope(GeoEnvelope* pEnv)
{
	assert(pEnv != NULL);
	if (m_nFeatureCount == 0)
	{
		return false;
	}

	//这种遍历的方式效率有点低
	GetFeature(0)->GetGeometry()->GetEnvelope(pEnv);

	double minx,maxx,miny,maxy;
	minx = pEnv->minX;
	maxx = pEnv->maxX;
	miny = pEnv->minY;
	maxy = pEnv->maxY;
	
	for (int iFid = 1; iFid < m_nFeatureCount; iFid ++)
	{
		GetFeature(iFid)->GetGeometry()->GetEnvelope(pEnv);
		if (maxx < pEnv->maxX)
		{
			maxx = pEnv->maxX;
		}
		if (minx > pEnv->minX)
		{
			minx = pEnv->minX;
		}
		if (maxy < pEnv->maxY)
		{
			maxy = pEnv->maxY;
		}
		if (miny > pEnv->minY)
		{
			miny = pEnv->minY;
		}
	}

	pEnv->minX = minx;
	pEnv->maxX = maxx;
	pEnv->minY = miny;
	pEnv->maxY = maxy;

	return true;
	
}

void GeoMitabLayer::SetEnvelope(GeoEnvelope* pEnv)
{
	return;
}

GeoFeature* GeoMitabLayer::GetFeature( long nFID )
{
	GeoFeatureDefn* poDefn = NULL;
	GeoFeature* poFeature = NULL;
	if (m_hMitabHandle != NULL)
	{
		poDefn = GeoReadMitabFeatureDefn(m_pszFullName,m_hMitabHandle);
		poFeature = GeoReadMitabFeature(m_hMitabHandle,nFID,poDefn);
	}

	return poFeature;
}

GeoSpatialReference *GeoMitabLayer::GetSpatialRef() const
{
	if (m_hMitabHandle != NULL)
	{
		mitab_projinfo hProjInfo = mitab_c_get_projinfo(m_hMitabHandle);
		int nProjID = 0;
		if (NULL == hProjInfo)
		{
			return NULL;
		}
		else if (hProjInfo != NULL)
		{
			mitab_c_get_projection_info(hProjInfo,&nProjID,NULL,NULL,NULL);
			GeoSpatialReference *poSRS = new GeoSpatialReference;
			poSRS->ImportFromEPSG(nProjID);
			return poSRS;
		}
		
	}
	return NULL;
}

GeoWkbGeometryType GeoMitabLayer::GetGeometryType() const
{
	return m_eGeomType;
}

bool GeoMitabLayer::UpdateFeature( GeoFeature *poFeature )
{
	return false;
}

bool GeoMitabLayer::CreateFeature( GeoFeature *poFeature )
{
	return false;
}

bool GeoMitabLayer::DeleteFeature( int nIndex,bool bDelete )
{
	return false;
}

int GeoMitabLayer::GetFetureCount() const
{
	return m_nFeatureCount;
}

bool GeoMitabLayer::CreateField( GeoFieldDefn *poField, bool bApproxOK /* = true */ )
{
	return false;
}
