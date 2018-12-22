/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2012
* 文件名称	: GeoSpatialReference.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang126@126.com)
* 设计日期	: 2012年6月9日
* 内容摘要	: 空间参考系统(包括地理坐标系统以及投影坐标系统等)
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOSPATIALREFERENCE_H_EC4670EE_AA58_4D83_B700_573180A066B9__
#define __GEOSPATIALREFERENCE_H_EC4670EE_AA58_4D83_B700_573180A066B9__

#include <stdlib.h>
#include <string>
#include "GeoDefine.h"


class GEOMETRY_API GeoSpatialReference
{
public:
	GeoSpatialReference();
	virtual ~GeoSpatialReference();

	GeoSpatialReference& operator=(const GeoSpatialReference& sref);

	//销毁一个坐标系
	static void DestroySpatialReference(GeoSpatialReference* poSRS);

	//获得坐标系的名称
	std::string GetName() const;

	//克隆一个坐标系统
	GeoSpatialReference* Clone() const;

	//克隆一个地理坐标系统
	GeoSpatialReference* CloneGeogCS() const;

	void Release();

	bool IsValid() const;

	//导出为WKT
	bool ExportToWkt( std::string& strWkt ) const;

	//导出为格式化的WKT
	bool ExportToPrettyWkt( std::string &strWkt) const;

	//从WKT导入
	bool ImportFromWkt( const std::string& strWkt );

	//导出到Proj4
	bool ExportToProj4(std::string &strProj4) const;

	//从proj4字符串导入
	bool ImportFromProj4(const std::string &strProj4);

	//根据EPSG的编号导入
	bool ImportFromEPSG(int nEpsgCode);

	//转换为ESRI风格的空间参考系
	bool MorphToESRI();


	//是否为地理坐标系
	bool IsGeographic() const;

	//是否为投影坐标系
	bool IsProjected() const;

	//是否为地心三维坐标系
	bool IsGeocentric() const;

	//是否为垂直坐标系
	bool IsVertical() const;

	//是否为局部坐标系，CAD比较常用
	bool IsLocal() const;

	//是否为复合坐标系，如加上高程坐标系（垂直坐标系）
	bool IsCompound() const;

	//是否是同一个投影
	bool IsSame(const GeoSpatialReference &cSrs) const;

	bool IsSame(const GeoSpatialReference *pSrs) const;

	//是否是同一个地理坐标系
	bool IsSameGeogCS(const GeoSpatialReference &cSrs) const;

	bool IsSameGeogCS(const GeoSpatialReference *pSrs) const;


	//获得节点的属性值
	std::string GetAttrValue( std::string strName, int iChild  = 0  ) const;

	//获得线性单位的长度
	double GetLinearUnits(char** pszName = NULL ) const;

	//获得角度单位
	double GetAngularUnits(char** pszName = NULL ) const;

	//获得本初子午线
	double GetPrimeMeridian(char ** pszOption) const;

	//获得长半轴半径
	double GetSemiMajor( int *pErr /* = NULL */) const;

	//获得短半轴半径
	double GetSemiMinor( int *pErr /* = NULL */) const;

	//获得偏心率倒数
	double GetInvFlattening( int *pErr /*=NULL*/) const;

	//坐标转换
	bool Transform(const GeoSpatialReference &cSrsTarget, int nCount, double *x, double *y, double *z = NULL) const;

	bool Transform(const GeoSpatialReference *pSrsTarget, int nCount, double *x, double *y, double *z = NULL) const;

private:

	void *m_hSRS;
};

#endif // end of __GEOSPATIALREFERENCE_H_EC4670EE_AA58_4D83_B700_573180A066B9__
