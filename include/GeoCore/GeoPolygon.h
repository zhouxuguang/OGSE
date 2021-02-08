/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2012
@ 文件名称	: GeoPolygon.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2012年3月23日
@ 内容摘要	: 多边形
@ 修改记录	:
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/

#ifndef __GEOPOLYGON_H_DEDB2644_0578_4CA9_9A8A_0EFAC37BDC13__
#define __GEOPOLYGON_H_DEDB2644_0578_4CA9_9A8A_0EFAC37BDC13__

class GeoPoint;
class GeoLineString;

class GEOMETRY_API GeoSurface : public GeoGeometry
{
public:
	virtual double GetArea() const = 0;

	virtual int PointInPolygon(const GeoPoint& poPoint) = 0;
};


class GEOMETRY_API GeoPolygon : public GeoSurface
{
public:
	//默认构造函数
	GeoPolygon(void);
	virtual ~GeoPolygon(void);

	//拷贝构造函数
	GeoPolygon(const GeoPolygon& other);
	GeoPolygon& operator= (const GeoPolygon& other);

	/*标准接口*/
	virtual int GetDimension() const;   //几何体的维数
	virtual bool Empty();	  //清空几何体
	virtual GeoWkbGeometryType GetGeometryType() const;  //获取几何体的类型
	virtual const char * GetGeometryName() const;//获取几何体的名字
	virtual void GetEnvelope(GeoEnvelope *env) const; //获取几何体的最小外包矩形
	virtual void Move(double dx,double dy);
	virtual double GetArea() const;
	virtual double GetLength() const;

	//OGC格式交换
	virtual std::string AsText();
	virtual int WkbByteSize() const;
	virtual int AsBinary( GeoByteOrder eByteOrder,unsigned char *poData ) const;

	//克隆
	virtual GeoGeometry *Clone() const;

	
	int GetNumRings() const;	  //计算环的个数
	int GetNumInteriorRings() const;	 //计算“岛”的个数

	GeoLineString* GetExteriorRing();
	const GeoLineString* GetExteriorRing() const;
	GeoLineString* GetInteriorRing(int iRing);
	const GeoLineString* GetInteriorRing(int iRing) const;
	void AddRing(const GeoLineString *poNewRing);
	GeoLineString** GetRings() const { return m_papoRings;}
	GeoLineString*  GetRing(int nIndex) const;

	//多边形中心点
	virtual int Centroid( GeoPoint * poPoint ) const;

	//判断点是否在多边形内
	int PointInPolygon(const GeoPoint& poPoint);
	//判断多边形是否在矩形内
	int PolygonInRect(GeoEnvelope &env);
	void CloseRings();

private:
	//数据成员
	int m_nRingCount;				//环的个数，一个外环和0个或多个内环（岛）
	GeoLineString **m_papoRings;	//指向曲线的指针
};

#endif // end of __GEOPOLYGON_H_DEDB2644_0578_4CA9_9A8A_0EFAC37BDC13__
