/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: GeoVectorLayer.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年12月23日
@ 内容摘要	: 矢量数据图层类
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOVECTORLAYER_H_769AF0EF_CF0C_4FBF_8995_6A22281462EA__
#define __GEOVECTORLAYER_H_769AF0EF_CF0C_4FBF_8995_6A22281462EA__

#include "GeoLayer.h"

class GeoEnvelope;
class GeoFeature;
class GeoGeometry;

//矢量图层
class GEOMETRY_API GeoVectorLayer : public GeoLayer
{
public:
	GeoVectorLayer(void);
	virtual ~GeoVectorLayer(void);

	//创建一个要素加入到图层中
	virtual bool CreateFeature( GeoFeature *poFeature ) = 0;

	//获得要素
	virtual GeoFeature *GetFeature( long nFID ) = 0;

	//获得几何体
	virtual GeoGeometry *GetGeometryRef(long nFID) = 0;

	//设置要素
	virtual bool UpdateFeature( GeoFeature *poFeature ) = 0;

	//删除一个要素
	virtual bool DeleteFeature(int nIndex,bool bDelete) = 0;

	//设置和获得整个图层的最小外包矩形
	virtual bool GetEnvelope(GeoEnvelope* pEnv) = 0;
	virtual void SetEnvelope(GeoEnvelope* pEnv) = 0;

	//获得图层的类型
	virtual GeoWkbGeometryType GetGeometryType() const = 0;

	//获得要素的个数
	virtual int GetFetureCount() const = 0;
};

#endif // end of __GEOVECTORLAYER_H_769AF0EF_CF0C_4FBF_8995_6A22281462EA__
