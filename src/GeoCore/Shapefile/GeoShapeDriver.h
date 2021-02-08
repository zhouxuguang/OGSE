/*******************************************************************************
* ��Ȩ����(C) pyhcx 2012
* �ļ�����	: GeoShapeDriver.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang236@126.com)
* �������	: 2012��8��23��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOSHAPEDRIVER_H_0A830A27_5A68_4962_B862_D31FEDE8F8E2__
#define __GEOSHAPEDRIVER_H_0A830A27_5A68_4962_B862_D31FEDE8F8E2__

/**********************************  ͷ�ļ� ************************************/

#include <stdlib.h>
#include "GeoDataDriver.h"

class GeoDataSource;

class GeoShapeDriver : public GeoDataDriver
{
public:
	GeoShapeDriver(void);
	~GeoShapeDriver(void);

	virtual const char *GetName();

	virtual GeoDataSource *Open( const char *pszName, bool bUpdate=false );

	virtual GeoDataSource *CreateGeoDataSource( const char *pszName,
		char ** = NULL );
	virtual bool DeleteGeoDataSource( const char *pszName );

	virtual GeoDataSource *CopyGeoDataSource( GeoDataSource *poSrcDS, 
		const char *pszNewName, 
		char **papszOptions = NULL );

private:
	char* m_pszName;	//����������
};

#endif // end of __GEOSHAPEDRIVER_H_0A830A27_5A68_4962_B862_D31FEDE8F8E2__
