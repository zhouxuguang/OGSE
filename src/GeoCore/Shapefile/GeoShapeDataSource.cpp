
#include <memory.h>
#include <string.h>

#include <vector>

#include "GeoShapeLayer.h"
#include "shapefil.h"
#include "GeoShapeDataSource.h"
#include "GeoDefine.h"

GeoShapeDataSource::GeoShapeDataSource(void)
{
	m_papoLayers = NULL;
	m_nLayers = 0;
	m_pszName = NULL;
	m_vecLayerName.clear();
}

GeoShapeDataSource::~GeoShapeDataSource(void)
{
	for (int i = 0; i < m_nLayers; i ++)
	{
		delete m_papoLayers[i];
		m_papoLayers[i] = NULL;
	}
	free( m_papoLayers );
	m_papoLayers = NULL;
}

int GeoShapeDataSource::Open( const char * pszFilename, int bUpdate )
{
	if (pszFilename == NULL)
	{
		return 0;
	}

	//截取文件名的后缀
	char szExtension[20];
	_splitpath(pszFilename,NULL,NULL,NULL,szExtension);

	//std::string strTmp = pszFilename;
	//size_t pos = strTmp.find_last_of('.');

	int b1 = EQUAL(szExtension,".shp");

	//和shapefile的后缀比较
	if (b1 == 0)
	{
		return 0;
	}

	SHPHandle hShpHandle = NULL;
	DBFHandle hDbfHandle = NULL;
	if (bUpdate)		//更新
	{
		hShpHandle = SHPOpen(pszFilename,"rb+");
		hDbfHandle = DBFOpen(pszFilename,"rb+");
	}
	else 
	{
		hShpHandle = SHPOpen(pszFilename,"rb");
		hDbfHandle = DBFOpen(pszFilename,"rb");
	}

	m_nLayers = 1;
	m_papoLayers = (GeoShapeLayer**)malloc(sizeof(GeoShapeLayer*));
	m_papoLayers[0] = new GeoShapeLayer(this,pszFilename,hShpHandle,hDbfHandle,NULL,1,bUpdate,gWkbUnKnow);

	m_pszName = strdup(pszFilename);
	m_vecLayerName.push_back(pszFilename);

	return 1;
}

int GeoShapeDataSource::GetLayerCount()
{
	return m_nLayers;
}

GeoLayer* GeoShapeDataSource::GetLayer(int nLayer)
{
	if (NULL == m_papoLayers)
	{
		return NULL;
	}
	if (nLayer < 0 || nLayer >= m_nLayers)
	{
		return NULL;
	}

	return m_papoLayers[nLayer];
}

GeoLayer* GeoShapeDataSource::GetLayerByName(const char *pszInName)
{
	return GeoDataSource::GetLayerByName(pszInName);
}

GeoLayer* GeoShapeDataSource::CreateGeoLayer( const char *pszName, 
											 GeoSpatialReference *poSpatialRef /* = NULL */, 
											 GeoWkbGeometryType eGType /* = wkbUnKnow */ )
{
	return NULL;
}

void GeoShapeDataSource::AddLayer(GeoShapeLayer* poLayer)
{
	m_papoLayers = (GeoShapeLayer **)realloc(m_papoLayers,sizeof(GeoShapeLayer*) * (m_nLayers+1));
	m_papoLayers[m_nLayers++] = poLayer;
}

