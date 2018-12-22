
#include "GeoLayer.h"
#include "GeoSpatialReference.h"
#include "GeoDataSource.h"

GeoDataSource::GeoDataSource(void)
{
	m_poDriver = NULL;
	m_hMutex = NULL;
}

GeoDataSource::~GeoDataSource(void)
{
	if (m_poDriver != NULL)
	{
		delete m_poDriver;
		m_poDriver = NULL;
	}
}

void GeoDataSource::DestroyGeoDataSource( GeoDataSource * poDS)
{
	delete poDS;
	poDS = NULL;
}

GeoLayer* GeoDataSource::GetLayerByName(const char *pszName)
{
	if (NULL == pszName)
	{
		return NULL;
	}

	for (int i = 0; i < GetLayerCount(); i ++)
	{
		GeoLayer* poLayer = GetLayer(i);
		if (strcmp(pszName,poLayer->GetLayerName()) == 0)
		{
			return poLayer;
		}
	}

	return NULL;
}

GeoLayer* GeoDataSource::CreateGeoLayer( const char *pszName, 
										GeoSpatialReference *poSpatialRef /* = NULL */, 
										GeoWkbGeometryType eGType /* = wkbUnKnow */ )
{
	return NULL;
}

GeoDataDriver* GeoDataSource::GetDriver() const
{
	return m_poDriver;
}

void GeoDataSource::SetDriver( GeoDataDriver *poDriver )
{
	m_poDriver = poDriver;
}

