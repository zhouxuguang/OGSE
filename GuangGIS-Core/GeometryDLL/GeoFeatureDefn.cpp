
#include <string.h>

#include "GeoFieldDefn.h"
#include "GeoGeometry.h"
#include "GeoFeatureDefn.h"

GeoFeatureDefn::GeoFeatureDefn( const char * pszName )
{
	m_pszFeatureClassName = strdup(pszName);
//	nRefCount = 0;
	m_nFieldCount = 0; 
	m_papoFieldDefn = NULL;
	m_eGeomType = gWkbUnKnow;
	m_bIgnoreGeometry = false;
	m_bIgnoreStyle = false;
}

GeoFeatureDefn* GeoFeatureDefn::Clone() const
{
	GeoFeatureDefn* poFeaDefn = new GeoFeatureDefn(m_pszFeatureClassName);
	if (m_nFieldCount > 0)
	{
		for (int nIndex = 0; nIndex < m_nFieldCount; nIndex ++)
		{
			GeoFieldDefn *poFieldDefn = m_papoFieldDefn[nIndex];
			poFeaDefn->AddFieldDefn(poFieldDefn);
		}
	}

	return poFeaDefn;

}

GeoFeatureDefn::~GeoFeatureDefn(void)
{
	if (m_papoFieldDefn != NULL)
	{
		for (int i = 0; i < m_nFieldCount; i ++)
		{
			delete m_papoFieldDefn[i];
			m_papoFieldDefn[i] = NULL;
		}
		delete m_papoFieldDefn;
		m_papoFieldDefn = NULL;
	}

	if (m_pszFeatureClassName != NULL)
	{
		free(m_pszFeatureClassName);
		m_pszFeatureClassName = NULL;
	}
	m_nFieldCount = 0; 
	m_eGeomType = gWkbUnKnow;
	m_bIgnoreGeometry = false;
	m_bIgnoreStyle = false;
}

GeoFieldDefn* GeoFeatureDefn::GetFieldDefn(int nIndex) const
{
	if (nIndex < 0 || nIndex >= m_nFieldCount)
	{
		return NULL;
	}
	else
		return m_papoFieldDefn[nIndex];
}

int GeoFeatureDefn::GetFieldIndex(const char *pszFieldName) const
{
	for (int i = 0; i < m_nFieldCount; i ++)
	{
		if ( 1 == EQUAL(pszFieldName,m_papoFieldDefn[i]->GetName()) )
		{
			return i;
		}
	}
	return -1;
}

void GeoFeatureDefn::AddFieldDefn(GeoFieldDefn *poField)
{
	m_papoFieldDefn = (GeoFieldDefn **)realloc(m_papoFieldDefn,
		sizeof(void*) * (m_nFieldCount + 1));
	if (m_papoFieldDefn != NULL)
	{
		m_papoFieldDefn[m_nFieldCount] = new GeoFieldDefn(poField);
	}
	m_nFieldCount += 1;
}

void GeoFeatureDefn::SetGeomType(GeoWkbGeometryType eGeomType)
{
	this->m_eGeomType = eGeomType;
}
