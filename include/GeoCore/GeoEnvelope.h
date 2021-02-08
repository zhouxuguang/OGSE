/*******************************************************************************
* ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
* �ļ�����	: GeoEnvelope.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang@126.com)
* �������	: 2012��3��6��
* ����ժҪ	: ����ռ�����࣬���Ա�Ｘ������������С������
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOENVELOPE_H_E9CBC39A_81BD_4AC7_BD29_EB1D0D58CF03__
#define __GEOENVELOPE_H_E9CBC39A_81BD_4AC7_BD29_EB1D0D58CF03__

#include "GeoDefine.h"

class GeoCoordinate;

class GEOMETRY_API GeoEnvelope
{
public:
	//Ĭ�Ϲ��캯��
	GeoEnvelope();

	//�������Ĺ��캯��
	GeoEnvelope(double minX,double maxX,double minY,double maxY);

	//�������캯��
	GeoEnvelope(const GeoEnvelope& envelope);

	//���ظ�ֵ�����
	GeoEnvelope &operator=(const GeoEnvelope& other);

	//������������ʼ��
	GeoEnvelope(GeoCoordinate *coord1,GeoCoordinate *coord2);

	~GeoEnvelope(void);

	//��ʼ������
	void Init(double x1,double x2,double y1,double y2);


	// �жϾ����Ƿ�Ϊ��
	bool IsNull(void) const;

	// ��ȡ��С������εĿ��
	double GetWidth(void) const;

	// ��ȡ��С�����εĸ߶�
	double GetHeight(void) const;

	// ��þ��ε����ĵ�����
	bool Center(GeoCoordinate &coord) const;

	bool Center(double &x,double &y) const;

	//�����Ƿ������һ��MBR
	bool Contains(const GeoEnvelope &env) const;

	//�ж�һ�����Ƿ��ڸþ�����
	bool Contains(const GeoCoordinate &pt) const;

	//�Ƿ���������
	bool Contains(double x, double y) const;

	//�ж�һ�����Ƿ��ھ�����
	DEPRECATE_API bool IsPointInRect(double x,double y) const;

	//�������������ཻ�Ĳ���
	bool Intersection(const GeoEnvelope& env,GeoEnvelope &envResult) const;

	//�������������Ƿ��ཻ
	bool Intersects(const GeoEnvelope *pOther) const;

	bool Intersects(const GeoEnvelope &env) const;

	//��������
	void ExpandToInclude(double x, double y);

	void ExpandToInclude(const GeoCoordinate &pt);

	void ExpandToInclude(const GeoEnvelope &other);

	void ExpandToInclude(const GeoEnvelope *other);

	//���㵽��һ��MBR�ľ���
	double DistanceTo(GeoEnvelope &env) const;

	//�������
	double GetArea() const;

	//�����ܳ�
	double Perimeter() const;

	//�任��ƽ��
	void Translate(double transX, double transY);

	//��̬����

	//�ж�p1,p2���ɵľ��κ�q1,q2���ɵľ����Ƿ��ཻ
	static bool Intersects(const GeoCoordinate &p1, const GeoCoordinate &p2, const GeoCoordinate &q1, const GeoCoordinate &q2);

public:
	double minX;	//��С������ε���Сxֵ
	double maxX;	//��С������ε����xֵ
	double minY;	//��С������ε���Сyֵ
	double maxY;	//��С������ε����yֵ
};

#endif // end of __GEOENVELOPE_H_E9CBC39A_81BD_4AC7_BD29_EB1D0D58CF03__
