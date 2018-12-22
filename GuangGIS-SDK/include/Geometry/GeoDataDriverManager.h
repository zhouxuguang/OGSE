/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: GeoDataDriverManager.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年11月22日
@ 内容摘要	: 矢量数据驱动管理类
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEODATADRIVERMANAGER_H_43F621D3_637F_417A_BFE0_BBE005DE5CE2__
#define __GEODATADRIVERMANAGER_H_43F621D3_637F_417A_BFE0_BBE005DE5CE2__

#include "GeoDefine.h"

class GeoDataDriver;
class GeoDataSource;

class GEOMETRY_API GeoDataDriverManager
{
public:
	static GeoDataDriverManager* GetInstance();

	int GetDriverCount() const;

	void RegisterDriver( GeoDataDriver * poDriver );

	void DeleteDriver( GeoDataDriver * poDriver );

	GeoDataDriver* GetDriver(int ) const;

	static GeoDataSource *Open( const char *pszName, bool bUpdate = false,
		GeoDataDriver ** ppoDriver = NULL );

private:
	GeoDataDriverManager(void);

	GeoDataDriverManager(const GeoDataDriverManager& rhs);

	GeoDataDriverManager& operator=(const GeoDataDriverManager& rhs);

	~GeoDataDriverManager(void);

private:
	int m_nDrivers;		//驱动个数
	GeoDataDriver **m_poDrivers;		//驱动数组

	static GeoDataDriverManager* m_DriverManager;
};

#endif // end of __GEODATADRIVERMANAGER_H_43F621D3_637F_417A_BFE0_BBE005DE5CE2__
