//#include "GeoSrsNode.h"
#include "GeoSpatialReference.h"

#include <ogr_srs_api.h>
#include <cpl_conv.h>

static const char* pszWGS84Wkt = 
"GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.2572235630016,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433],AUTHORITY[\"EPSG\",\"4326\"]]";


GeoSpatialReference::GeoSpatialReference()
{
	m_hSRS = NULL;
}

GeoSpatialReference::~GeoSpatialReference()
{
	if (m_hSRS != NULL)
	{
		OSRDestroySpatialReference((OGRSpatialReferenceH)m_hSRS);
		m_hSRS = NULL;
	}
}

bool GeoSpatialReference::MorphToESRI()
{
	if (m_hSRS != NULL)
	{
		OGRErr enErr = OGRERR_FAILURE; 
		enErr = OSRMorphToESRI((OGRSpatialReferenceH)m_hSRS);
		return enErr == OGRERR_NONE;
	}

	return false;
}

GeoSpatialReference* GeoSpatialReference::Clone() const
{
	GeoSpatialReference *pNewSrs = NULL;
	if (m_hSRS != NULL)
	{
		pNewSrs = new GeoSpatialReference;

		std::string strWkt;
		ExportToWkt(strWkt);
		pNewSrs->ImportFromWkt(strWkt);
	}

	return pNewSrs;
}

GeoSpatialReference* GeoSpatialReference::CloneGeogCS() const
{
	GeoSpatialReference *pNewSrs = NULL;
	if (m_hSRS != NULL)
	{
		pNewSrs = new GeoSpatialReference;
		OGRSpatialReferenceH hNewSrs = OSRCloneGeogCS((OGRSpatialReferenceH)m_hSRS);

		int nFlag = OSRIsGeographic((OGRSpatialReferenceH)hNewSrs);

		pNewSrs->m_hSRS = hNewSrs;
	}

	return pNewSrs;
}


GeoSpatialReference& GeoSpatialReference::operator=(const GeoSpatialReference& sref)
{
	if (m_hSRS == sref.m_hSRS)
	{
		return *this;
	}
	Release();
	if( sref.m_hSRS )
	{
		m_hSRS = OSRClone((OGRSpatialReferenceH)sref.m_hSRS);
	}
	return *this;
}

void GeoSpatialReference::DestroySpatialReference(GeoSpatialReference* poSRS)
{
	delete poSRS;
}

std::string GeoSpatialReference::GetName() const
{
	std::string strTmp = "";
	const char* pszName = NULL;
	if (m_hSRS != NULL)
	{
		if (IsGeographic())
		{
			pszName = OSRGetAttrValue((OGRSpatialReferenceH)m_hSRS,"GEOGCS",0);
		}
		else if (IsProjected())
		{
			pszName = OSRGetAttrValue((OGRSpatialReferenceH)m_hSRS,"PROJCS",0);
		}
		else if (IsVertical())
		{
		}
		else if (IsCompound())
		{
			pszName = OSRGetAttrValue((OGRSpatialReferenceH)m_hSRS,"",0);
		}

		if (NULL != pszName)
		{
			strTmp = pszName;
		}

		
	}
	
	return strTmp;
}

void GeoSpatialReference::Release()
{
	if( NULL!=m_hSRS )
	{
		OSRDestroySpatialReference((OGRSpatialReferenceH)m_hSRS);
		m_hSRS = NULL;
	}
}

bool GeoSpatialReference::IsValid() const
{
	if( NULL != m_hSRS )
	{
		return OSRValidate((OGRSpatialReferenceH)m_hSRS) == OGRERR_NONE;
	}
	return false;
}

bool GeoSpatialReference::ExportToWkt(std::string& strWkt) const
{
	if( NULL!=m_hSRS )
	{
		char* pszWkt = NULL;

		if(OGRERR_NONE != OSRExportToWkt((OGRSpatialReferenceH)m_hSRS,&pszWkt))
			return false;

		if (NULL != pszWkt)
		{
			strWkt = pszWkt;
		}
		
		if (pszWkt != NULL)
		{
			OGRFree(pszWkt);
		}
		
		return true;
	}
	return false;
}

bool GeoSpatialReference::ExportToPrettyWkt(std::string &strWkt) const
{
	if( NULL!=m_hSRS )
	{
		char* pszWkt = NULL;

		if(OGRERR_NONE != OSRExportToPrettyWkt((OGRSpatialReferenceH)m_hSRS,&pszWkt,0))
			return false;

		if (NULL != pszWkt)
		{
			strWkt = pszWkt;
		}

		if (pszWkt != NULL)
		{
			CPLFree(pszWkt);
		}

		return true;
	}
	return false;
}

bool GeoSpatialReference::ImportFromWkt( const std::string& strWkt )
{
	if(m_hSRS == NULL)
	{
		m_hSRS = OSRNewSpatialReference(strWkt.c_str());
	}

	if( m_hSRS != NULL)
	{
		return true;
	}
	return false;
}

bool GeoSpatialReference::ExportToProj4(std::string &strProj4) const
{
	if( NULL!=m_hSRS )
	{
		char* pszProj4 = NULL;

		if(OGRERR_NONE != OSRExportToProj4((OGRSpatialReferenceH)m_hSRS,&pszProj4))
			return false;

		if (NULL != pszProj4)
		{
			strProj4 = pszProj4;
			CPLFree(pszProj4);
		}

		return true;
	}
	return false;
}

bool GeoSpatialReference::ImportFromProj4(const std::string &strProj4)
{
	if(m_hSRS == NULL)
	{
		m_hSRS = OSRNewSpatialReference(pszWGS84Wkt);
	}

	OGRErr enErr = OGRERR_FAILURE;
	if( m_hSRS != NULL)
	{
		const char* pszWkt = strProj4.c_str();
		enErr = OSRImportFromProj4((OGRSpatialReferenceH)m_hSRS,pszWkt);
	}
	return (enErr == OGRERR_NONE);
}

bool GeoSpatialReference::ImportFromEPSG(int nEpsgCode)
{
	if(m_hSRS == NULL)
	{
		m_hSRS = OSRNewSpatialReference(pszWGS84Wkt);
	}

	else if (m_hSRS != NULL)
	{
		OSRImportFromEPSG(m_hSRS,nEpsgCode);
		return true;
	}

	OSRDestroySpatialReference(m_hSRS);
	m_hSRS = NULL;
	return false;
}


bool GeoSpatialReference::IsGeographic() const
{
	if( m_hSRS != NULL)
	{
		int nErr = OSRIsGeographic((OGRSpatialReferenceH)m_hSRS);
		return nErr;
	}
	return false;
}

bool GeoSpatialReference::IsProjected() const
{
	if( NULL!=m_hSRS )
	{
		return OSRIsProjected((OGRSpatialReferenceH)m_hSRS);
	}
	return false;
}

bool GeoSpatialReference::IsGeocentric() const
{
	if (NULL != m_hSRS)
	{
		return OSRIsGeocentric((OGRSpatialReferenceH)m_hSRS);
	}
	return false;
}

bool GeoSpatialReference::IsVertical() const
{
	if (NULL != m_hSRS)
	{
		return OSRIsVertical((OGRSpatialReferenceH)m_hSRS);
	}

	return false;
}

bool GeoSpatialReference::IsLocal() const
{
	if( NULL!=m_hSRS )
	{
		return OSRIsLocal((OGRSpatialReferenceH)m_hSRS);
	}
	return false;
}

bool GeoSpatialReference::IsCompound() const
{
	if (NULL != m_hSRS)
	{
		return OSRIsCompound((OGRSpatialReferenceH)m_hSRS);
	}
	return false;
}

bool GeoSpatialReference::IsSame(const GeoSpatialReference &cSrs) const
{
	if (NULL != m_hSRS && NULL != cSrs.m_hSRS)
	{
		return OSRIsSame((OGRSpatialReferenceH)m_hSRS,(OGRSpatialReferenceH)cSrs.m_hSRS);
	}
	return false;
}

bool GeoSpatialReference::IsSame(const GeoSpatialReference *ptrcSrs) const
{
	if (NULL == ptrcSrs)
	{
		return false;
	}

	if (NULL != m_hSRS && NULL != ptrcSrs->m_hSRS)
	{
		return OSRIsSame((OGRSpatialReferenceH)m_hSRS,(OGRSpatialReferenceH)ptrcSrs->m_hSRS);
	}
	return false;
}

bool GeoSpatialReference::IsSameGeogCS(const GeoSpatialReference &cSrs) const
{
	if (NULL != m_hSRS && NULL != cSrs.m_hSRS)
	{
		return OSRIsSameGeogCS((OGRSpatialReferenceH)m_hSRS,(OGRSpatialReferenceH)cSrs.m_hSRS);
	}
	return false;
}

bool GeoSpatialReference::IsSameGeogCS(const GeoSpatialReference *ptrcSrs) const
{
	if (NULL == ptrcSrs)
	{
		return false;
	}

	if (NULL != m_hSRS && NULL != ptrcSrs->m_hSRS)
	{
		return OSRIsSameGeogCS((OGRSpatialReferenceH)m_hSRS,(OGRSpatialReferenceH)ptrcSrs->m_hSRS);
	}
	return false;
}

std::string GeoSpatialReference::GetAttrValue( std::string strName, int iChild /* = 0 */ ) const
{
	if (NULL != m_hSRS)
	{
		const char* pszName = strName.c_str();

		std::string strValue = OSRGetAttrValue((OGRSpatialReferenceH)m_hSRS,pszName,iChild);
		std::string strAttrValue = strValue;
		return strAttrValue;
	}

	return "";
}

double GeoSpatialReference::GetLinearUnits(char** pszName ) const
{
	if( NULL!=m_hSRS )
	{
		return OSRGetLinearUnits((OGRSpatialReferenceH)m_hSRS,pszName);
	}
	pszName = NULL;
	return 0;
}

double GeoSpatialReference::GetAngularUnits(char** pszName ) const
{
	if( NULL!=m_hSRS )
	{
		return OSRGetAngularUnits((OGRSpatialReferenceH)m_hSRS,pszName);
	}
	pszName = NULL;
	return 0;
}

double GeoSpatialReference::GetPrimeMeridian(char ** pszOption) const
{
	if (NULL != m_hSRS)
	{
		return OSRGetPrimeMeridian((OGRSpatialReferenceH)m_hSRS,pszOption);
	}

	pszOption = NULL;
	return 0;
}

double GeoSpatialReference::GetSemiMajor( int * pErr) const
{
	if (NULL != m_hSRS)
	{
		return OSRGetSemiMajor((OGRSpatialReferenceH)m_hSRS,pErr);
	}
	return 0;
}

double GeoSpatialReference::GetSemiMinor( int *pErr ) const
{
	if (NULL != m_hSRS)
	{
		return OSRGetSemiMinor((OGRSpatialReferenceH)m_hSRS,pErr);
	}
	return 0;
}

double GeoSpatialReference::GetInvFlattening( OGRErr *pErr ) const
{
	if (NULL != m_hSRS)
	{
		return OSRGetInvFlattening((OGRSpatialReferenceH)m_hSRS,pErr);
	}
	return 0;
}

bool GeoSpatialReference::Transform(const GeoSpatialReference &ptrTarget, int nCount, double *x, double *y, double *z) const
{
	if( m_hSRS == NULL /*|| ptrTarget == NULL*/ || ptrTarget.IsValid() == false )
		return false;

	OGRSpatialReferenceH hTarget = (OGRSpatialReferenceH)ptrTarget.m_hSRS;
	if (NULL == hTarget)
	{
		return false;
	}
	
	OGRCoordinateTransformationH hOgrCT = 
		OCTNewCoordinateTransformation((OGRSpatialReferenceH)m_hSRS,hTarget);
	if (NULL == hOgrCT)
	{
		return false;
	}

	int nFlag = OCTTransform(hOgrCT,nCount,x,y,z);
	OCTDestroyCoordinateTransformation(hOgrCT);
	return nFlag;
	
}

bool GeoSpatialReference::Transform(const GeoSpatialReference *cSrsTarget, int nCount, double *x, double *y, double *z /* = NULL */) const
{
	if( m_hSRS == NULL || cSrsTarget->IsValid() == false )
		return false;

	OGRSpatialReferenceH hTarget = (OGRSpatialReferenceH)cSrsTarget->m_hSRS;
	if (NULL == hTarget)
	{
		return false;
	}

	OGRCoordinateTransformationH hOgrCT = 
		OCTNewCoordinateTransformation((OGRSpatialReferenceH)m_hSRS,hTarget);
	if (NULL == hOgrCT)
	{
		return false;
	}

	int nFlag = OCTTransform(hOgrCT,nCount,x,y,z);
	OCTDestroyCoordinateTransformation(hOgrCT);
	return nFlag;
}
