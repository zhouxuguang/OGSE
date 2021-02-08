/*******************************************************************************
* ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
* �ļ�����	: GeoFeature.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang236@126.com)
* �������	: 2012��10��6��
* ����ժҪ	: Ҫ���ඨ�壬����һ���������һ������ֵ
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOFEATURE_H_55A5C49A_2862_41EE_BB24_18DD43976580__
#define __GEOFEATURE_H_55A5C49A_2862_41EE_BB24_18DD43976580__

/**********************************  ͷ�ļ� ************************************/
                                                              
#include "GeoDefine.h"
#include "GeoFeatureDefn.h"

class GeoFeatureDefn;
class GeoGeometry;

class GEOMETRY_API GeoFeature
{
private:
	long m_nFID;					//Ҫ��ID��
	GeoFeatureDefn* m_poDefn;		//Ҫ�صĶ���
	GeoGeometry* m_poGeometry;		//Ҫ�ع����ļ�����
	GeoField* m_pauFields;			//Ҫ�ص�����ֵ

protected:
	char*		m_pszTmpFieldValue;	//�ֶ���ʱֵ
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

	//�����ֶ�ֵ
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

	//����������Ҫ��
	static GeoFeature *CreateFeature( GeoFeatureDefn * poDefn);
	static void DestroyFeature( GeoFeature * poFeature);
};

#endif // end of __GEOFEATURE_H_55A5C49A_2862_41EE_BB24_18DD43976580__
