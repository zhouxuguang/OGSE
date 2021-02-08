/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2013
@ �ļ�����	: GeoDataDriverManager.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2013��11��22��
@ ����ժҪ	: ʸ����������������
@ �޸ļ�¼	: 
@ ��    ��		��    ��		�޸���		�޸�ժҪ

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
	int m_nDrivers;		//��������
	GeoDataDriver **m_poDrivers;		//��������

	static GeoDataDriverManager* m_DriverManager;
};

#endif // end of __GEODATADRIVERMANAGER_H_43F621D3_637F_417A_BFE0_BBE005DE5CE2__
