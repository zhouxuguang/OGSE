/*******************************************************************************
* 版权所有(C) pyhcx 2012
* 文件名称	: GeoLineString.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年8月26日
* 内容摘要	: 折线类，OGC标准里面的LINESTRING
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOLINESTRING_H_BD4CA47E_DA05_4800_8CEC_90035368CDE7__
#define __GEOLINESTRING_H_BD4CA47E_DA05_4800_8CEC_90035368CDE7__

/**********************************  头文件 ************************************/

class GeoPoint;
class GeoEnvelope;

class GEOMETRY_API GeoCurve : public GeoGeometry
{
public:
	GeoCurve(){}

	virtual ~GeoCurve(){}

	virtual double GetLength() const = 0;

	virtual void GetStartPoint(GeoPoint* poPoint) const = 0;

	virtual void GetEndPoint(GeoPoint* poPoint) const = 0;

	virtual bool IsClosed() const{return false;}

};

class GEOMETRY_API GeoLineString : public GeoCurve
{
public:
	GeoLineString(void);
	GeoLineString( const GeoLineString * poSrcRing );
	virtual ~GeoLineString(void);

	/*标准接口*/
	virtual int GetDimension() const;   //几何体的维数
	virtual bool Empty();	  //清空几何体
	virtual GeoWkbGeometryType GetGeometryType() const;  //获取几何体的类型
	virtual const char *GetGeometryName() const;//获取几何体的名字
	virtual void GetEnvelope(GeoEnvelope *env) const; //获取几何体的最小外包矩形

	//移动线串
	virtual void Move(double dx,double dy);
	virtual double GetArea() const;
	virtual double GetLength() const;

	//OGC格式交换
	virtual std::string AsText();			//转化为WKT格式
	virtual int WkbByteSize() const;
	virtual int AsBinary( GeoByteOrder eByteOrder,unsigned char *poData ) const;

	//clone
	virtual GeoGeometry *Clone() const;

	//LINESTRING的方法
	//点得个数
	int GetNumPoint() const;
	//长度
	virtual double Length() const;
	//第一个点和最后一个点
	virtual void GetStartPoint(GeoPoint* poPoint) const;
	virtual void GetEndPoint(GeoPoint* poPoint) const;
	//得到第i个坐标点x值
	double GetX(int i) const;
	//得到第i个坐标点y值
	double GetY(int i) const;
	//设置X坐标
	void SetX(int i,double xIn);
	//设置Y坐标
	void SetY(int i,double yIn);

	//得到第i点的坐标
	void GetPoint(int i,GeoPoint *poPoint) const;
	void GetPoint(int i,double &x,double &y) const;
	void GetPoint(int i,GeoCoordinate& coord) const;

	//获得坐标数组
	void GetPoints( GeoCoordinate *poCoords, double *paZ = NULL ) const;
	void GetPoints( double* padX, double* padY, double* padZ = NULL ) const;
	void GetPoints( GeoCoordinate *poCoords, double* padZ ,int nStride = 0) const;
	void GetPoints( double* padX, double* padY, double* padZ ,int nStride = 0) const;
	void GetPointArray(double *padVertexs) const;

	GeoCoordinate* GetPointsBuf() const;

	//设置指定点的坐标
	void SetPoint(int i,GeoPoint* poPoint);
	void SetPoint(int i,double x,double y);
	void SetPoint(int i,GeoCoordinate *poPoint);

	void SetPoints(int nPoints, GeoCoordinate *paoPointsIn);
	void SetPoints(int nPoints,double* padX,double* padY,double* padZ = NULL);

	//设置最小外包矩形
	void SetEnvelope(GeoEnvelope &env);

	//计算到另一个点的距离
	double DistanceToPoint(GeoPoint &poPoint);
	double DistanceToPoint(double x,double y);

	//判断一个线串是否在一个矩形内
	int IsLineInRect(GeoEnvelope &env);


	//添加点到线串中
	void AddPoint(GeoPoint& poPoint);
	void AddPoint(double x,double y);
	void AddPoint(GeoCoordinate& poPoint);

	//指定点的个数
	void SetPointsNumber(int nNewPointCount);

	//是否闭合
	bool IsClosed() const;

	//是否是简单的线串
	bool IsSimple() const;

	//是否为空
	bool IsEmpty() const;

	//点是否逆时针排列
	bool IsClockWise() const;

	//将曲线闭合
	void CloseRings();

	//交换点的顺序,开始点和结束点交换等
	void ReversePoints( void );


private:

	int         m_nPointCount;			//坐标点的个数
	int			m_nCurMaxPointCount;	//当前最大点的个数容量
	GeoCoordinate *m_paoPoints;			//点的坐标序列
};

#endif // end of __GEOLINESTRING_H_BD4CA47E_DA05_4800_8CEC_90035368CDE7__
