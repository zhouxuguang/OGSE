/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2012
@ 文件名称	: ISpatialIndex.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2012年11月28日
@ 内容摘要	: 空间索引基类
@ 修改记录	:
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __ISPATIALINDEX_H_EAF91DAE_8BF1_4C90_BCE7_3D2EA8275D52__
#define __ISPATIALINDEX_H_EAF91DAE_8BF1_4C90_BCE7_3D2EA8275D52__

#include <vector>

#include "GeoDefine.h"

class GeoEnvelope;

//空间索引的基类
class GEOMETRY_API ISpatialIndex
{
public:
	//将给定的索引项添加到索引中
	virtual void Insert(long key,const GeoEnvelope &itemRect)=0;

	//点查询: 查询所有与给定点选中的索引记录,保存记录ID到数组中
	virtual void Query(double cx, double cy, vector<long>& ItemSearched )=0;

	//区域查询：查询所有与给定范围searchBox相交的索引记录,保存记录ID到数组中
	//利用ID在图层中查询相关地理对象,此查询为相交查询，可以再设包含查询等
	virtual void Query(const GeoEnvelope& searchBox, vector<long>& ItemSearched )=0;

	//删除 ： 删除指定索引记录,成功返回true，失败，返回false
	virtual bool Delete(long itemID,const GeoEnvelope& itemBox)=0;

	//更新：更新(itemID,oldItemMBR) to (itemID,newItemMBR)
	virtual void Update(long itemID, const GeoEnvelope& oldItemMBR, const GeoEnvelope& newItemMBR)=0;

	//virtual GeoEnvelope GetMBR(RTreeNode *rnode); //获取索引空间的MBR
	virtual int GetIndexType()=0; //获取该索引类型

	ISpatialIndex(void);
	virtual ~ISpatialIndex(void);

protected:
private:
};

#endif // end of __ISPATIALINDEX_H_EAF91DAE_8BF1_4C90_BCE7_3D2EA8275D52__
