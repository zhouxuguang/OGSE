#include "SysSrsMapToLayer.h"
#include "SysUtilFunc.h"

#include <SysDraw/SysEnvelope.h>
#include <SysDraw/SysMapLayer.h>
#include <SysDraw/SysMap.h>

#include <SysDataSource/SysDataSource.h>
#include <SysDataSource/CoordTransformation.h>

#include <geos/geom.h>

namespace SysDraw
{
	

	void SysSrsMapToLayer::MapToLayerExtent( SysMap* pMap,SysMapLayer* pLayer,SysEnvelope& envOut )
	{
		MapExtentToLayerExtent(pMap,pLayer,envOut);
	}

	void SysSrsMapToLayer::MapToLayerExtent( SysMap* pMap,const SysEnvelope& envIn,SysMapLayer* pLayer,SysEnvelope& envOut )
	{
		SysEnvelope CurEnv;
		SysSRS::CSpatialReference* pLyrSrs = pLayer->GetSpatialRef();
		if (pLyrSrs == NULL)
		{
			envOut = envIn;
			return;
		}

		else if (pLyrSrs != NULL )
		{
			pLyrSrs->MorphToESRI();
			SysSRS::CSpatialReference* pMapSrs = pMap->GetSpatialRef();
			if (pMapSrs == NULL)
			{
				CurEnv = envIn;
				envOut = CurEnv;
				return;
			}
			else if (pMapSrs != NULL)
			{
				pMapSrs->MorphToESRI();
				if (pLyrSrs->IsSame(*pMapSrs))		
				{
					CurEnv = envIn;
					envOut = CurEnv;
					return;
				}

				else if (!pLyrSrs->IsSame(*pMapSrs))		//动态投影
				{
					SysSRS::CCoordTransformation ForwardTran;
					ForwardTran.InitCoordTransform(pMapSrs,pLyrSrs);
					SysSRS::CCoordTransformation InverseTran; 
					InverseTran.InitCoordTransform(pLyrSrs,pMapSrs);

					//
					SysEnvelope env = envIn;
					double dbExtent[4];
					dbExtent[0] = env.minX;
					dbExtent[1] = env.maxX;
					dbExtent[2] = env.minY;
					dbExtent[3] = env.maxY;
					geos::geom::Envelope envResult;
					ForwardTran.TransformBoundingBox(dbExtent,envResult);
					CurEnv.minX = envResult.getMinX();
					CurEnv.maxX = envResult.getMaxX();
					CurEnv.minY = envResult.getMinY();
					CurEnv.maxY = envResult.getMaxY();

					envOut = CurEnv;
					return;
				}
			}
		}
	}

	void SysSrsMapToLayer::LayerToMapExtent( SysMap* pMap,SysMapLayer* pLayer,SysEnvelope& envOut )
	{
		LayerExtentToMapExtent(pMap,pLayer,envOut);
	}

	void SysSrsMapToLayer::LayerToMapExtent( SysMap* pMap,const SysEnvelope& envIn, SysMapLayer* pLayer,SysEnvelope& envOut )
	{
		SysEnvelope CurEnv;
		SysSRS::CSpatialReference* pMapSrs = pMap->GetSpatialRef();
		if (pMapSrs == NULL)
		{
			envOut = envIn;
			return;
		}

		else if (pMapSrs != NULL )
		{
			pMapSrs->MorphToESRI();
			SysSRS::CSpatialReference* pLyrSrs = pLayer->GetSpatialRef();
			if (pLyrSrs == NULL)
			{
				CurEnv = envIn;
				envOut = CurEnv;
				return;
			}
			else if (pLyrSrs != NULL)
			{
				pLyrSrs->MorphToESRI();
				if (pLyrSrs->IsSame(*pMapSrs))		
				{
					CurEnv = envIn;
					envOut = CurEnv;
					return;
				}

				else if (!pLyrSrs->IsSame(*pMapSrs))		//动态投影
				{
					SysSRS::CCoordTransformation ForwardTran;
					ForwardTran.InitCoordTransform(pLyrSrs,pMapSrs);
					SysSRS::CCoordTransformation InverseTran; 
					InverseTran.InitCoordTransform(pMapSrs,pLyrSrs);

					//将投影转换后的范围转换为当前的地图范围，设为当前显示范围
					SysEnvelope env = envIn;
					double dbExtent[4];
					dbExtent[0] = env.minX;
					dbExtent[1] = env.maxX;
					dbExtent[2] = env.minY;
					dbExtent[3] = env.maxY;
					geos::geom::Envelope envResult;
					ForwardTran.TransformBoundingBox(dbExtent,envResult);
					CurEnv.minX = envResult.getMinX();
					CurEnv.maxX = envResult.getMaxX();
					CurEnv.minY = envResult.getMinY();
					CurEnv.maxY = envResult.getMaxY();

					envOut = CurEnv;
					return;
				}
			}
		}
	}

	void SysSrsMapToLayer::TransExtent( SysSRS::CCoordTransformation* pTran,SysEnvelope& envOut )
	{
		Envelope envResult;
		double dbExtent[4];

		//转换当前图层的地理范围
		dbExtent[0] = envOut.minX;
		dbExtent[1] = envOut.maxX;
		dbExtent[2] = envOut.minY;
		dbExtent[3] = envOut.maxY;
		pTran->TransformBoundingBox(dbExtent,envResult);
		envOut.minX = envResult.getMinX();
		envOut.maxX = envResult.getMaxX();
		envOut.minY = envResult.getMinY();
		envOut.maxY = envResult.getMaxY();
	}
}


