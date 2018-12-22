/**
* @file              SysGeometryDef.h
* @brief             几何体等类型重定义
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年7月14日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSGEOMETRYDEF_H_B528AC35_DBC3_494E_814A_F472B7CA48AE__
#define __SYSGEOMETRYDEF_H_B528AC35_DBC3_494E_814A_F472B7CA48AE__

//几何体基类
typedef void SysGeometry;

//几何体工厂
typedef void SysGeometryFactory;

//线串（闭合）
typedef void SysLinearRing;

//折线串
typedef void SysLineString;

//点
typedef void SysPoint;

//多边形
typedef void SysPolygon;

//多几何体
typedef void SysGeometryCollection;

//多点
typedef void SysMultiPoint;

//多线串
typedef void SysMultiLineString;

//多多边形
typedef void SysMultiPolygon;

//下面是图层等定义

//图层
typedef void SysLayer;

//要素
typedef void SysFeature;

//要素定义
typedef void SysFeatureDefn;

//字段定义
typedef void SysFieldDefn;


#endif // end of __SYSGEOMETRYDEF_H_B528AC35_DBC3_494E_814A_F472B7CA48AE__
