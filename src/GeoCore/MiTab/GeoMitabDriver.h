/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: GeoMitabDriver.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年11月30日
@ 内容摘要	: mapinfo数据驱动
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

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
	char* m_pszName;	//驱动的名字
};

#endif // end of __GEOMITABDRIVER_H_682C6DB5_F648_44B6_9FC5_EA5763BA9B3D__
