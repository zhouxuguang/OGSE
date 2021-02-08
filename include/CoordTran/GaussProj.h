/*******************************************************************************
* ��Ȩ����(C) pyhcx 2013
* �ļ�����	: GaussProj.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang236@126.com)
* �������	: 2013��10��10��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GAUSSPROJ_H_21BDBAEF_009C_4736_9233_91C3AD980EC5__
#define __GAUSSPROJ_H_21BDBAEF_009C_4736_9233_91C3AD980EC5__

/**********************************  ͷ�ļ� ************************************/


class COORDTRAN_API CGaussProj
{
public:
	CGaussProj(CGeoEllipse* pGeoEllipse,double dbCenterLat,double dbCenterLon,
		double dbFalseN,double dbFalseE,int nType);
	~CGaussProj(void);

	//��������ת��Ϊ��˹ƽ��ֱ������
	bool LonLat2Gauss(int zoneWide,double dbLat,double dbLon,double *x,double *y);

	//��˹ƽ��ֱ������ת����������
	bool Gauss2LonLat(double x,double y,double *dbLat,double *dbLon);

private:
	CGeoEllipse* m_pGeoEllipse;
	double m_dbCenterLat;			//����γ��
	double m_dbCenterLon;			//���뾭��
	double m_dbFalseN;				//����ƫ��
	double m_dbFalseE;				//����ƫ��
	int m_nType;			//3 or 6
};

#endif // end of __GAUSSPROJ_H_21BDBAEF_009C_4736_9233_91C3AD980EC5__
