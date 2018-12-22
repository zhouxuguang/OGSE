/*******************************************************************************
* 版权所有(C) pyhcx 2012
* 文件名称	: GeoGeometryCollection.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年8月6日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOGEOMETRYCOLLECTION_H_34DCA488_27DB_47FB_B148_EF38DAB57A14__
#define __GEOGEOMETRYCOLLECTION_H_34DCA488_27DB_47FB_B148_EF38DAB57A14__

/**********************************  头文件 ************************************/

class GeoGeometry;
class GeoEnvelope;
class GeoPoint;
class GeoLineString;
class GeoPolygon;


class GEOMETRY_API GeoGeometryCollection : public GeoGeometry
{

public:
	GeoGeometryCollection(void);
	virtual ~GeoGeometryCollection(void);

public:
	//标准接口
	virtual int GetDimension() const;   //几何体的维数
	virtual bool Empty();	  //清空几何体
	virtual GeoWkbGeometryType GetGeometryType() const;  //获取几何体的类型
	virtual const char *GetGeometryName() const;		//获取几何体的名字
	virtual void GetEnvelope(GeoEnvelope *env) const;	//获取几何体的最小外包矩形
	virtual void Move(double dx,double dy);				//移动几何体
	virtual double GetArea() const;							//计算面积
	virtual double GetLength() const;							//计算长度

	virtual std::string AsText();			//转化为WKT格式
	virtual int WkbByteSize() const;

	//非标准接口
	int GetNumGeoms() const;						//获取几何体的个数
	GeoGeometry* GetGeometryRefs(int nIndex);		//根据编号获取几何体
	const GeoGeometry* GetGeometryRefs(int nIndex) const;

	bool AddGeometry(GeoGeometry *poGeom);
	bool RemoveGeometry(int iGeom,bool bDelete);

private:
	int nGeomCount;				//几何体的个数
	GeoGeometry **papoGeoms;	//几何体集合
};

#endif // end of __GEOGEOMETRYCOLLECTION_H_34DCA488_27DB_47FB_B148_EF38DAB57A14__
