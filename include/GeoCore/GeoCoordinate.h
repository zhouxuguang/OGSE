/*******************************************************************************
* ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
* �ļ�����	: GeoCoordinate.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang236@126.com)
* �������	: 2012��6��7��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOCOORDINATE_H_FFF7BBFB_E6D0_439F_A6CE_561AD1EE0958__
#define __GEOCOORDINATE_H_FFF7BBFB_E6D0_439F_A6CE_561AD1EE0958__

/**********************************  ͷ�ļ� ************************************/

#include "GeoDefine.h"

class GEOMETRY_API GeoCoordinate
{
public:
	//Ĭ�Ϲ��캯��
	GeoCoordinate(void);
	//�������Ĺ��캯��
	GeoCoordinate(double x,double y);
	//�������캯��
	GeoCoordinate(const GeoCoordinate& coord);
	//��������
	~GeoCoordinate(void);

	//���㵽��һ����þ���
	double Distance(GeoCoordinate &coord) const;

	//����X��Y����
	void SetX(double x);
	void SetY(double y);

public:
	double x;	//������
	double y;	//������
	//double z;	//z����
};

#endif // end of __GEOCOORDINATE_H_FFF7BBFB_E6D0_439F_A6CE_561AD1EE0958__
