/*******************************************************************************
* ��Ȩ����(C) pyhcx 2013
* �ļ�����	: MercatorProj.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang)
* �������	: 2013��10��3��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __MERCATORPROJ_H_1726EB1A_1F65_4874_BF18_7DE41FC2E6CD__
#define __MERCATORPROJ_H_1726EB1A_1F65_4874_BF18_7DE41FC2E6CD__

/**********************************  ͷ�ļ� ************************************/

#include "GeoEllipse.h"

class COORDTRAN_API CMercatorProj
{
public:
	CMercatorProj(CGeoEllipse* pGeoEllipse,double dbCenterLon = 0,double dbStdLat = 0,
		double dbFalseE = 0,double dbFalseN = 0);
	~CMercatorProj(void);

	//ī����ͶӰ����
	void LonLat2Mercator(double dbLat,double dbLon,double &x,double &y);

	//ī����ͶӰ���㣬ֱ�ӷ�
	void Mercator2LonLat(double x,double y,double &dbLat,double &dbLon);

	//ī����ͶӰ���㣬������
	void Mercator2LonLat2(double x,double y,double &dbLat,double &dbLon);

private:
	CGeoEllipse* m_geoEllipse;		//ͶӰ�����ڵ�������
	double m_dbCenterLon;			//ԭ�㾭��
	double m_dbStdLat;				//��׼γ��
	double m_dbFalseE;				//�ٶ�ƫ
	double m_dbFalseN;				//�ٱ�ƫ 
};


#endif // end of __MERCATORPROJ_H_1726EB1A_1F65_4874_BF18_7DE41FC2E6CD__
