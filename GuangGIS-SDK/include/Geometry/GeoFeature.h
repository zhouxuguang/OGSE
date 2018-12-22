/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2012
* 文件名称	: GeoFeature.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年10月6日
* 内容摘要	: 要素类定义，代表一个几何体和一组属性值
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOFEATURE_H_55A5C49A_2862_41EE_BB24_18DD43976580__
#define __GEOFEATURE_H_55A5C49A_2862_41EE_BB24_18DD43976580__

/**********************************  头文件 ************************************/
                                                              
#include "GeoDefine.h"
#include "GeoFeatureDefn.h"

class GeoFeatureDefn;
class GeoGeometry;

class GEOMETRY_API GeoFeature
{
private:
	long m_nFID;					//要素ID号
	GeoFeatureDefn* m_poDefn;		//要素的定义
	GeoGeometry* m_poGeometry;		//要素关联的几何体
	GeoField* m_pauFields;			//要素的属性值

protected:
	char*		m_pszTmpFieldValue;	//字段临时值
public:
	GeoFeature();
	explicit GeoFeature( GeoFeatureDefn * );
	virtual ~GeoFeature(void);

	GeoFeatureDefn     *GetDefnRef() { return m_poDefn; }

	bool SetGeometryDirectly(GeoGeometry* poGeometryIn);
	GeoGeometry* GetGeometry() { return m_poGeometry; }

	int GetFieldCount() { return m_poDefn->GetFieldCount(); }

	GeoFieldDefn* GetFieldDefnRef(int iField)
	{
		return m_poDefn->GetFieldDefn(iField);
	}

	int GetFieldIndex(const char* pszName)
	{
		return m_poDefn->GetFieldIndex(pszName);
	}

	long GetFID() { return m_nFID; }
	void SetFID(long nFID) 
	{
		this->m_nFID = nFID;
	}

	void SetField(GeoFieldDefn *poField);
	GeoField *GetFieldRef(int i) { return m_pauFields + i; }

	int GetFieldAsInteger(int iField);
	double GetFieldAsDouble(int iField);
	const char* GetFieldAsString(int iField);
	GByte* GetFieldAsBinary(int iField,int* nCount);
	bool GetFieldAsDateTime(int iField,int *nYear,int *nMonth,
		int* nDay,int* nHour,int* nMinute,int *nSecond,int *nTZFlag);

	int GetFieldAsInteger(const char* pszFName)
	{
		return GetFieldAsInteger( GetFieldIndex(pszFName) );
	}

	double GetFieldAsDouble(const char* pszFName)
	{
		return GetFieldAsDouble( GetFieldIndex(pszFName) );
	}

	const char* GetFieldAsString(const char* pszFName)
	{
		return GetFieldAsString(GetFieldIndex(pszFName));
	}

	GByte* GetFieldAsBinary(const char* pszFName,int* nCount)
	{
		return GetFieldAsBinary(GetFieldIndex(pszFName),nCount);
	}

	//设置字段值
	void SetField( int iField, int nValue );
	void SetField( int iField, double dfValue );
	void SetField( int iField, const char * pszValue );
	void SetField( int iField, int nCount, int * panValues );
	void SetField( int iField, int nCount, double * padfValues );
	void SetField( int iField, GeoField * puValue );
	void SetField( int iField, char ** papszValues );
	void SetField( int iField, int nCount, GByte * pabyBinary );
	void SetField( int iField, int nYear, int nMonth, int nDay,
		int nHour=0, int nMinute=0, int nSecond=0, int nTZFlag = 0 );

	//创建和销毁要素
	static GeoFeature *CreateFeature( GeoFeatureDefn * poDefn);
	static void DestroyFeature( GeoFeature * poFeature);
};

#endif // end of __GEOFEATURE_H_55A5C49A_2862_41EE_BB24_18DD43976580__
