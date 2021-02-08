/*******************************************************************************
* 版权所有(C) pyhcx 2012
* 文件名称	: GeoPoint.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年8月3日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOPOINT_H_EC5553A4_54D8_4CCF_913E_F51C740C9C33__
#define __GEOPOINT_H_EC5553A4_54D8_4CCF_913E_F51C740C9C33__

/**********************************  头文件 ************************************/

#include "GeoGeometry.h"

class GeoEnvelope;
class GeoCoordinate;


class GEOMETRY_API GeoPoint : public GeoGeometry
{
public:
	GeoPoint(void);
	GeoPoint(double x,double y);	//带参数的构造函数
	GeoPoint(const GeoPoint& point);	//拷贝构造函数
	GeoPoint(GeoCoordinate *coord);
	virtual ~GeoPoint(void);

public:
	//重载赋值运算符
	GeoPoint& operator =(const GeoPoint& pt);

	virtual int GetDimension() const;
	virtual bool Empty();
	virtual GeoWkbGeometryType GetGeometryType() const;
	virtual const char * GetGeometryName() const;
	virtual void GetEnvelope(GeoEnvelope *env) const;
	virtual double GetArea() const;
	virtual double GetLength() const;

	//OGC格式交换
	virtual std::string AsText();
	virtual int WkbByteSize() const;
	virtual int AsBinary( GeoByteOrder eByteOrder,unsigned char *poData ) const;

	//克隆函数
	virtual GeoGeometry *Clone() const;

	double DistanceTo(const GeoPoint& geoPoint);//求到另一个点之间的距离
	bool   IsPointInLine(GeoPoint &p1,GeoPoint &p2);//判断点是否在线段上
	int IsPointInRect(GeoEnvelope env);				//判断一个点是否在一个矩形内(可以用来裁剪)

	double GetX() const;
	double GetY() const;
	void SetX(double xIn);
	void SetY(double yIn);

	bool IsEqual(GeoPoint &point);			//判断两点是否相等
	virtual void Move(double dx,double dy);			//移动几何体

private:
	double x;	//x坐标
	double y;	//y坐标

};

#endif // end of __GEOPOINT_H_EC5553A4_54D8_4CCF_913E_F51C740C9C33__

