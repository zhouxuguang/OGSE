/*******************************************************************************
* ��Ȩ����(C) pyhcx 2013
* �ļ�����	: LambertProj.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang@)
* �������	: 2013��10��1��
* ����ժҪ	: ��Ҫ����������ͶӰ����
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __LAMBERTPROJ_H_59CAE94A_E937_42AD_AA27_794E467715BB__
#define __LAMBERTPROJ_H_59CAE94A_E937_42AD_AA27_794E467715BB__

/**********************************  ͷ�ļ� ************************************/

#include "CoordCommon.h"

class CGeoEllipse;


class COORDTRAN_API CLambertProj
{
public:
	CLambertProj(CGeoEllipse* pGeoEllipse,double dbCenterLat,double dbCenterLon,
		double dbStdLat1,double dbStdLat2,
		double dbFalseE = 0,double dbFalseN = 0);
	~CLambertProj(void);

	//������ͶӰ����
	void LatLon2Lambert(double dbLat,double dbLon,double &x,double &y);

	//������ͶӰ����
	void Lambert2LatLon(double x,double y,double &dbLat,double &dbLon);

private:
	CGeoEllipse* m_geoEllipse;		//ͶӰ�����ڵ�������
	double m_dbCenterLat;			//��ʼγ��
	double m_dbCenterLon;			//��ʼ����
	double m_dbStdLat1;				//��һ��׼γ��
	double m_dbStdLat2;				//�ڶ���׼γ��
	double m_dbFalseE;				//�ٶ�ƫ
	double m_dbFalseN;				//�ٱ�ƫ 
};

#endif // end of __LAMBERTPROJ_H_59CAE94A_E937_42AD_AA27_794E467715BB__

//����Uֵ��dbLat�Ի���Ϊ��λ
extern "C" double GetValueU(CGeoEllipse* pGeoEllipse,double dbLat);

//���ź���
extern "C" int Sign(double dbN);

//����M��ֵ,����Ϊ��λ
extern "C" double GetValueM(CGeoEllipse* pGeoEllipse,double dbLat);

//����Tֵ
extern "C" double GetValueT(CGeoEllipse* pGeoEllipse,double dbLat);
