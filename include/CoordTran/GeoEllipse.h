/*******************************************************************************
* ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2013
* �ļ�����	: GeoEllipse.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zxg)
* �������	: 2013��9��22��
* ����ժҪ	: ������������㣬��Ҫ��һЩ�����ļ����ת����ϵ
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOELLIPSE_H_05958735_A7BF_409C_A444_CF9F202FCBE9__
#define __GEOELLIPSE_H_05958735_A7BF_409C_A444_CF9F202FCBE9__

/**********************************  ͷ�ļ� ************************************/


#include <math.h>
#include <assert.h>
#include "CoordCommon.h"


class COORDTRAN_API CGeoEllipse
{
public:
	explicit CGeoEllipse(double dbRadiusA,double dbRadiusB);
	~CGeoEllipse(void);

	double GetRadiusA() const
	{
		return m_dbRadiusA;
	}

	double GetRadiusB() const
	{
		return m_dbRadiusB;
	}

	double GetE1() const
	{
		return m_dbE1;
	}

	double GetE2() const
	{
		return m_dbE2;
	}

	//����Ȧ���ʰ뾶,M
	double GetMeridianRadius(double lat) const;

	//î��Ȧ���ʰ뾶,N
	double GetPrimeRadius(double lat) const;

	//γ��Ȧ�İ뾶
	double GetLatCycleRadius(double lat) const;

	//ƽ�����ʰ뾶
	double GetAveRadius() const;

	//γ�߻���������ָ��γ��dbLat�Ͼ��Ȳ�ΪdbDLon��γ�߻���
	double GetParallelArcLen(double dbLat,double dbDLon) const;

	//�����߻���
	double GetMeridianArcLen(double dbLat) const;

	double GetLat(double dbLen) const;

	//�������������,������γ�ߺ���������֮�����е��������
	double GetTrapeziumArea(double dbLat1,double dbLat2,double dbLon1,double dbLon2) const;

	//����Uֵ��dbLat�Ի���Ϊ��λ������γ��
	double GetValueU(double dbLat);

	//�������ת��Ϊ�ռ�ֱ������
	bool BLH_XYZ(double dbLon,double dbLat,double dbHei,double& x,double &y,double &z);

	//�ռ�ֱ������ת��Ϊ�������
	bool XYZ_BLH(double X,double Y,double Z, double &dbLon,double &dbLat,double &dbHei);

private:
	double m_dbRadiusA;			//������
	double m_dbRadiusB;			//�̰���
	double m_dbE1;				//��һƫ����
	double m_dbE2;				//�ڶ�ƫ����
};


#endif // end of __GEOELLIPSE_H_05958735_A7BF_409C_A444_CF9F202FCBE9__
