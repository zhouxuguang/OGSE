/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2013
@ �ļ�����	: GeoDataDriver.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2013��11��23��
@ ����ժҪ	: ����������
@ �޸ļ�¼	: 
@ ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEODATADRIVER_H_2D1EC449_8F69_4153_82E3_438DAD8EAAC6__
#define __GEODATADRIVER_H_2D1EC449_8F69_4153_82E3_438DAD8EAAC6__

#include <stdlib.h>
#include "GeoDefine.h"


class GeoDataSource;

//��������
class GEOMETRY_API GeoDataDriver
{
public:
	GeoDataDriver(void);
	virtual ~GeoDataDriver();

	virtual const char *GetName() = 0;

	virtual GeoDataSource *Open( const char *pszName, bool bUpdate=false ) = 0;

	virtual GeoDataSource *CreateGeoDataSource( const char *pszName,
		char ** = NULL );
	virtual bool DeleteGeoDataSource( const char *pszName );

	virtual GeoDataSource *CopyGeoDataSource( GeoDataSource *poSrcDS, 
		const char *pszNewName, 
		char **papszOptions = NULL );
};

#endif // end of __GEODATADRIVER_H_2D1EC449_8F69_4153_82E3_438DAD8EAAC6__
