/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2013
@ �ļ�����	: GeoMitabDriver.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2013��11��30��
@ ����ժҪ	: mapinfo��������
@ �޸ļ�¼	: 
@ ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOMITABDRIVER_H_682C6DB5_F648_44B6_9FC5_EA5763BA9B3D__
#define __GEOMITABDRIVER_H_682C6DB5_F648_44B6_9FC5_EA5763BA9B3D__

#include "GeoDataDriver.h"

class GeoDataSource;

class GeoMitabDriver : public GeoDataDriver
{
public:
	GeoMitabDriver(void);
	~GeoMitabDriver(void);

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

#endif // end of __GEOMITABDRIVER_H_682C6DB5_F648_44B6_9FC5_EA5763BA9B3D__
