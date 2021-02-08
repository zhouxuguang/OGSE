/*******************************************************************************
* ��Ȩ����(C) pyhcx 2012
* �ļ�����	: GeoPoint.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang236@126.com)
* �������	: 2012��8��3��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOPOINT_H_EC5553A4_54D8_4CCF_913E_F51C740C9C33__
#define __GEOPOINT_H_EC5553A4_54D8_4CCF_913E_F51C740C9C33__

/**********************************  ͷ�ļ� ************************************/

#include "GeoGeometry.h"

class GeoEnvelope;
class GeoCoordinate;


class GEOMETRY_API GeoPoint : public GeoGeometry
{
public:
	GeoPoint(void);
	GeoPoint(double x,double y);	//�������Ĺ��캯��
	GeoPoint(const GeoPoint& point);	//�������캯��
	GeoPoint(GeoCoordinate *coord);
	virtual ~GeoPoint(void);

public:
	//���ظ�ֵ�����
	GeoPoint& operator =(const GeoPoint& pt);

	virtual int GetDimension() const;
	virtual bool Empty();
	virtual GeoWkbGeometryType GetGeometryType() const;
	virtual const char * GetGeometryName() const;
	virtual void GetEnvelope(GeoEnvelope *env) const;
	virtual double GetArea() const;
	virtual double GetLength() const;

	//OGC��ʽ����
	virtual std::string AsText();
	virtual int WkbByteSize() const;
	virtual int AsBinary( GeoByteOrder eByteOrder,unsigned char *poData ) const;

	//��¡����
	virtual GeoGeometry *Clone() const;

	double DistanceTo(const GeoPoint& geoPoint);//����һ����֮��ľ���
	bool   IsPointInLine(GeoPoint &p1,GeoPoint &p2);//�жϵ��Ƿ����߶���
	int IsPointInRect(GeoEnvelope env);				//�ж�һ�����Ƿ���һ��������(���������ü�)

	double GetX() const;
	double GetY() const;
	void SetX(double xIn);
	void SetY(double yIn);

	bool IsEqual(GeoPoint &point);			//�ж������Ƿ����
	virtual void Move(double dx,double dy);			//�ƶ�������

private:
	double x;	//x����
	double y;	//y����

};

#endif // end of __GEOPOINT_H_EC5553A4_54D8_4CCF_913E_F51C740C9C33__

