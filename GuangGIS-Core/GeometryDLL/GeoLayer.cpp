

#include "GeoEnvelope.h"
#include "GeoSpatialReference.h"
#include "GeoLayer.h"


GeoLayer::GeoLayer(void)
{
	m_pszLayerName = NULL;
	m_pszDataSource = NULL;
}

GeoLayer::~GeoLayer(void)
{
	if (m_pszLayerName != NULL)
	{
		free(m_pszLayerName);
		m_pszLayerName = NULL;
	}
}

char* GeoLayer::GetLayerName() const
{
	return m_pszLayerName;
}

void GeoLayer::SetLayerName(char *pszLayerName)
{
	this->m_pszLayerName = (char*)malloc(strlen(pszLayerName)+1);
	strcpy(this->m_pszLayerName,pszLayerName);
}
