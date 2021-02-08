#include "GeoMitabLayer.h"
#include "GeoMitabDataSource.h"

GeoMitabDataSource::GeoMitabDataSource(void)
{
	m_papoLayers = NULL;
	m_nLayers = 0;
	m_pszName = NULL;
}

GeoMitabDataSource::~GeoMitabDataSource(void)
{
	if (m_papoLayers != NULL)
	{
		for (int i = 0; i < m_nLayers; i ++)
		{
			delete m_papoLayers[i];
		}

		delete m_papoLayers;
		m_papoLayers = NULL;
	}
}

int GeoMitabDataSource::Open(const char * pszFilename, int bUpdate /* = 0 */)
{
	if (pszFilename == NULL)
	{
		return 0;
	}

	//截取文件名的后缀
	char szExtension[50];
	_splitpath(pszFilename,NULL,NULL,NULL,szExtension);

	int b1 = EQUAL(szExtension,".mif");
	int b2 = EQUAL(szExtension,".tab");

	//和mapinfo的后缀比较
	if (b1 == 0 && b2 == 0)
	{
		return 0;
	}

	mitab_handle hTabHandle = mitab_c_open(pszFilename);

	m_nLayers = 1;
	m_papoLayers = (GeoMitabLayer**)malloc(sizeof(GeoMitabLayer*));
	m_papoLayers[0] = new GeoMitabLayer(this,NULL,m_pszName,hTabHandle,true);

	m_pszName = strdup(pszFilename);

	m_vecLayerName.push_back(pszFilename);

	return 1;
}

int GeoMitabDataSource::GetLayerCount()
{
	return m_nLayers;
}

GeoLayer* GeoMitabDataSource::GetLayer(int nIndex)
{
	if (NULL == m_papoLayers)
	{
		return NULL;
	}
	if (nIndex < 0 || nIndex >= m_nLayers)
	{
		return NULL;
	}

	return m_papoLayers[nIndex];
}

GeoLayer* GeoMitabDataSource::GetLayerByName(const char *pszName)
{
	return GeoDataSource::GetLayerByName(pszName);
}

GeoLayer* GeoMitabDataSource::CreateGeoLayer( const char *pszName, 
											 GeoSpatialReference *poSpatialRef /* = NULL */, 
											 GeoWkbGeometryType eGType /* = wkbUnKnow */ )
{
	return NULL;
}

void GeoMitabDataSource::AddLayer(GeoMitabLayer* poLayer)
{
	m_papoLayers = (GeoMitabLayer **)realloc(m_papoLayers,sizeof(GeoMitabLayer*) * (m_nLayers+1));
	m_papoLayers[m_nLayers++] = poLayer;
}
