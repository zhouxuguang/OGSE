
#include <stdlib.h>
#include <string.h>

#include "GeoDataSource.h"
#include "GeoDataDriver.h"
#include "GeoDataDriverManager.h"


GeoDataDriverManager* GeoDataDriverManager::m_DriverManager = NULL;

GeoDataDriverManager::GeoDataDriverManager(void)
{
	m_nDrivers = 0;
	m_poDrivers = NULL;
}

GeoDataDriverManager::GeoDataDriverManager(const GeoDataDriverManager& rhs)
{
	for (int i = 0; i < m_nDrivers; i ++)
	{
		delete m_poDrivers[i];
	}

	delete m_poDrivers;

	m_nDrivers = rhs.m_nDrivers;

	m_poDrivers = (GeoDataDriver**)malloc(sizeof(void*)*m_nDrivers);
	memcpy(m_poDrivers,rhs.m_poDrivers,sizeof(void*)*m_nDrivers);
}

GeoDataDriverManager& GeoDataDriverManager::operator =(const GeoDataDriverManager& rhs)
{
	if (&rhs == this)
	{
		return *this;
	}

	if (m_poDrivers != NULL)
	{
		//如果驱动列表不为空，将它删除
		for (int i = 0; i < m_nDrivers; i ++)
		{
			delete m_poDrivers[i];
		}

		delete m_poDrivers;
		m_poDrivers = NULL;
	}
	

	m_nDrivers = rhs.m_nDrivers;

	m_poDrivers = (GeoDataDriver**)malloc(sizeof(void*)*m_nDrivers);
	memcpy(m_poDrivers,rhs.m_poDrivers,sizeof(void*)*m_nDrivers);

	return *this;
}

GeoDataDriverManager::~GeoDataDriverManager(void)
{
	m_nDrivers = 0;

	if (m_poDrivers != NULL)
	{
		for (int i = 0; i < m_nDrivers; i ++)
		{
			delete m_poDrivers[i];
		}

		delete m_poDrivers;
		m_poDrivers = NULL;
	}
	
}

int GeoDataDriverManager::GetDriverCount() const
{
	return m_nDrivers;
}

void GeoDataDriverManager::RegisterDriver( GeoDataDriver * poDriver )
{
	for( int i = 0; i < m_nDrivers; i++ )
	{
		if( poDriver == m_poDrivers[i] )
		{
			return;
		}

		//已经注册这个驱动就删除这个驱动
		//const char *pszName = poDriver->GetName();
		if ( EQUAL(poDriver->GetName(), m_poDrivers[i]->GetName()) )
		{
			delete poDriver;
			poDriver = NULL;
			return;
		}
	}

	//加入这个驱动到驱动列表的尾部
	m_poDrivers = (GeoDataDriver **)
		realloc( m_poDrivers, (m_nDrivers+1) * sizeof(void*) );
	m_poDrivers[m_nDrivers++] = poDriver;
}

void GeoDataDriverManager::DeleteDriver( GeoDataDriver * poDriver )
{
	int nIndex = 0;
	for( int i = 0; i < m_nDrivers; i++ )
	{
		if( poDriver == m_poDrivers[i] )
		{
			delete m_poDrivers[i];
			nIndex = i;
			break;
		}
	}

	//将nIndex处的驱动删除掉
	if (nIndex < m_nDrivers-1)
	{
		memmove(m_poDrivers+nIndex,m_poDrivers+nIndex+1,sizeof(void*)*(m_nDrivers-nIndex-1));
		m_nDrivers--;
	}
}

GeoDataDriver* GeoDataDriverManager::GetDriver(int nIndex) const
{
	if (nIndex < 0 || nIndex >= m_nDrivers)
	{
		return NULL;
	}

	return m_poDrivers[nIndex];
}

GeoDataDriverManager* GeoDataDriverManager::GetInstance()
{
	if (NULL == m_DriverManager)
	{
		m_DriverManager = new GeoDataDriverManager();
	}

	return m_DriverManager;
}

GeoDataSource* GeoDataDriverManager::Open( const char *pszName, bool bUpdate /* = false */, 
										  GeoDataDriver ** ppoDriver /* = NULL */ )
{
	GeoDataSource* poDataSource = NULL;
	GetInstance();
	for (int i = 0; i < m_DriverManager->m_nDrivers; i ++)
	{
		GeoDataDriver *poDriver = m_DriverManager->m_poDrivers[i];
		poDataSource = m_DriverManager->GetDriver(i)->Open(pszName,bUpdate);

		if( poDataSource != NULL )
		{
			if( ppoDriver != NULL )
			{
				*ppoDriver = poDriver;
			}

			if( poDataSource->GetDriver() == NULL )
			{
				poDataSource->SetDriver(poDriver);
			}

			return poDataSource;
		}
	}

	return NULL;

}
