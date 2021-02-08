/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: GeoGeometryCollection.cpp
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年11月24日
@ 内容摘要	: 多个几何体
@ 修改记录	:
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/


#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include "GeoGeometry.h"
#include "GeoEnvelope.h"
#include "GeoPoint.h"
#include "GeoLineString.h"
#include "GeoPolygon.h"

#include "GeoGeometryCollection.h"


GeoGeometryCollection::GeoGeometryCollection(void)
{
	nGeomCount = 0;
	papoGeoms = NULL;
}

GeoGeometryCollection::~GeoGeometryCollection(void)
{
	Empty();
}

int GeoGeometryCollection::GetDimension() const
{
	return 2;	//这个可能是不正确的，要依据成员的实际情况
}

bool GeoGeometryCollection::Empty()
{
	int i = 0;

	//先清空每一个几何体
	if (papoGeoms != NULL)
	{
// 		for (i = 0; i < nGeomCount; i ++)
// 		{
// 			free(papoGeoms[i]);
// 		}

		free(papoGeoms);
	}

	nGeomCount = 0;
	papoGeoms = NULL;

	return true;
}

GeoWkbGeometryType GeoGeometryCollection::GetGeometryType() const
{
	return gWkbGeometryCollection;
}

const char* GeoGeometryCollection::GetGeometryName() const
{
	return "GEOMETRYCOLLECTION";
}

void GeoGeometryCollection::GetEnvelope(GeoEnvelope *env) const
{
	assert(env != NULL);
	if (nGeomCount == 0)
	{
		return;
	}
	papoGeoms[0]->GetEnvelope(env);
	GeoEnvelope ogrRingEnv;
	for (int iRing = 1; iRing < nGeomCount; iRing ++)
	{
		papoGeoms[iRing]->GetEnvelope(&ogrRingEnv);
		if (env->minX > ogrRingEnv.minX)
		{
			env->minX = ogrRingEnv.minX;
		}
		if (env->minY > ogrRingEnv.minY)
		{
			env->minY = ogrRingEnv.minY;
		}
		if (env->maxX < ogrRingEnv.maxX)
		{
			env->maxX = ogrRingEnv.maxX;
		}
		if (env->maxY < ogrRingEnv.maxY)
		{
			env->maxY = ogrRingEnv.maxY;
		}
	}
}

void GeoGeometryCollection::Move(double dx, double dy)
{
	for (int i = 0; i < nGeomCount; i ++)
	{
		papoGeoms[i]->Move(dx,dy);
	}
}

double GeoGeometryCollection::GetArea() const
{
	double dArea = 0.0;
	for (int iGeom = 0; iGeom < nGeomCount; iGeom ++)
	{
		GeoGeometry *geom = papoGeoms[iGeom];
		GeoWkbGeometryType geoType = gWkbFlatten(geom->GetGeometryType());

		//根据不同的几何体计算面积,还有待扩展
		if (geoType == gWkbLineString)
		{
			dArea += ((GeoLineString *)geom)->GetArea();
		}

		if (geoType == gWkbPolygon)
		{
			dArea += ((GeoPolygon *)geom)->GetArea();
		}

		if (geoType == gWkbGeometryCollection)
		{
			dArea += ((GeoGeometryCollection *)geom)->GetArea();
		}
	}

	return dArea;
}

double GeoGeometryCollection::GetLength() const
{
	double dLength = 0.0;

	for (int iGeom = 0; iGeom < nGeomCount; iGeom ++)
	{
		GeoGeometry *geom = papoGeoms[iGeom];
		GeoWkbGeometryType geomType = gWkbFlatten(geom->GetGeometryType());

		//根据不同的几何体计算长度
		if (geomType == gWkbPoint)
		{
			dLength += ((GeoPoint *)geom)->GetLength();
		}
		if (geomType == gWkbLineString)
		{
			dLength += ((GeoLineString *)geom)->GetLength();
		}

		if (geomType == gWkbPolygon)
		{
			dLength += ((GeoPolygon *)geom)->GetLength();
		}

		if (geomType == gWkbGeometryCollection)
		{
			dLength += ((GeoGeometryCollection *)geom)->GetLength();
		}
	}

	return dLength;
}

std::string GeoGeometryCollection::AsText()
{
	return "";
}

int GeoGeometryCollection::WkbByteSize() const
{
	int nSize = 0;

	for (int i = 0; i < nGeomCount; i ++)
	{
		nSize += papoGeoms[i]->WkbByteSize();
	}

	return nSize;
}

int GeoGeometryCollection::GetNumGeoms() const
{
	return nGeomCount;
}

GeoGeometry* GeoGeometryCollection::GetGeometryRefs(int nIndex)
{
	if (nIndex < 0 || nIndex >= nGeomCount)
	{
		return NULL;
	}
	return papoGeoms[nIndex];
}

const GeoGeometry* GeoGeometryCollection::GetGeometryRefs(int nIndex) const
{
	if (nIndex < 0 || nIndex >= nGeomCount)
	{
		return NULL;
	}
	return papoGeoms[nIndex];
}

bool GeoGeometryCollection::AddGeometry(GeoGeometry *poGeom)
{
	//
	/*GeoGeometry *poNewGeom = new GeoGeometry();*/
	assert(poGeom);
	papoGeoms = (GeoGeometry **)realloc(papoGeoms,
		sizeof(void *) * (nGeomCount + 1));

	if (papoGeoms)
	{
		papoGeoms[nGeomCount] = poGeom;
		nGeomCount += 1;

		return true;
	}
	return false;
}

bool GeoGeometryCollection::RemoveGeometry(int iGeom, bool bDelete)
{
	//越界
	if (iGeom < 0 || iGeom >= nGeomCount)
	{
		return false;
	}

	//特殊情况
	if (iGeom == -1)
	{
		while (nGeomCount > 0)
		{
			RemoveGeometry(nGeomCount - 1,bDelete);
		}
		return true;
	}

	if (bDelete)
	{
		delete papoGeoms[iGeom];
	}

	//将删除元素之后的元素移位
	memmove(papoGeoms + iGeom, papoGeoms + iGeom + 1, 
		sizeof(void *) * (nGeomCount - iGeom - 1));

	//元素个数-1
	nGeomCount -= 1;

	return true;
}
