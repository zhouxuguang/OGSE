#include "SysUtilFunc.h"
#include "SysEnvelope.h"
#include "SysMap.h"
#include "SysMapLayer.h"


#include <limits>

namespace SysDraw
{
	//

	void MapExtentToLayerExtent( SysMap* pMap,SysMapLayer* pMapLayer,SysEnvelope& envOut )
	{
		SysEnvelope CurEnv;
		SysSRS::CSpatialReference* pLyrSrs = pMapLayer->GetSpatialRef();
		if (pLyrSrs == NULL)
		{
			envOut = pMap->GetVisualExtent();
			return;
		}

		else if (pLyrSrs != NULL )
		{
			pLyrSrs->MorphToESRI();
			SysSRS::CSpatialReference* pMapSrs = pMap->GetSpatialRef();
			if (pMapSrs == NULL)
			{
				CurEnv = pMap->GetVisualExtent();
				envOut = CurEnv;
				return;
			}
			else if (pMapSrs != NULL)
			{
				pMapSrs->MorphToESRI();
				if (pLyrSrs->IsSame(*pMapSrs))		
				{
					CurEnv = pMap->GetVisualExtent();
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
					SysEnvelope env = pMap->GetVisualExtent();
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

	void LayerExtentToMapExtent( SysMap* pMap,SysMapLayer* pMapLayer,SysEnvelope& envOut )
	{
		SysEnvelope CurEnv;
		SysSRS::CSpatialReference* pMapSrs = pMap->GetSpatialRef();
		if (pMapSrs == NULL)
		{
			envOut = pMapLayer->GetLayerExtent();
			return;
		}

		else if (pMapSrs != NULL )
		{
			pMapSrs->MorphToESRI();
			SysSRS::CSpatialReference* pLyrSrs = pMapLayer->GetSpatialRef();
			if (pLyrSrs == NULL)
			{
				CurEnv = pMapLayer->GetLayerExtent();
				envOut = CurEnv;
				return;
			}
			else if (pLyrSrs != NULL)
			{
				pLyrSrs->MorphToESRI();
				if (pLyrSrs->IsSame(*pMapSrs))		
				{
					CurEnv = pMapLayer->GetLayerExtent();
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
					SysEnvelope env = pMapLayer->GetLayerExtent();
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

	double* GetPloygonPoints(GeoPolygon* pPolygon,int& nPointCountOut,std::vector<GLsizei> &vecCounts,std::vector<std::vector<GLuint> >& vecIndices)
	{
		GeoLineString* poRing = pPolygon->GetExteriorRing();

		/*if (poRing != NULL)
		{
			nRingCountOut = 1;
		}*/

		vecCounts.clear();
		vecIndices.clear();

		//获得总的环数
		int nRingCount = pPolygon->GetNumInteriorRings();

		//获得总的点数
		int nPointCount = poRing->GetNumPoint();
		int nOutPoints = nPointCount;
		nPointCountOut = nPointCount;
		vecCounts.push_back(nOutPoints);

		for (int nIndex = 0; nIndex < nRingCount; nIndex ++)
		{
			GeoLineString* poRing = pPolygon->GetExteriorRing(nIndex);
			int nNum = poRing->GetNumPoint();
			vecCounts.push_back(nNum);
			nPointCount += nNum;
		}

		nPointCountOut = nPointCount;

		//点的数组
		double *vecVertexs = new(std::nothrow) double[nPointCount*2];

		OGRRawPoint* poPoints = new(std::nothrow) OGRRawPoint[nPointCount];
		poRing->getPoints(poPoints);
		
		//GLuint *pIndex = new GLuint[nPointCount];	//索引数组
		//vecIndices.resize(nPointCount);
		std::vector<GLuint> vecOutIndice;

		for (int j = 0; j < nOutPoints; j ++)
		{
			vecVertexs[j*2] = poPoints[j].x;
			vecVertexs[j*2+1] = poPoints[j].y;
			vecOutIndice.push_back(j);
		}

		vecIndices.push_back(vecOutIndice);

		delete []poPoints;

		int nCurIndex = nOutPoints;

		for (int nIndex = 0; nIndex < nRingCount; nIndex ++)
		{
			GeoLineString* poRing = pPolygon->getInteriorRing(nIndex);
			int nCount = poRing->getNumPoints();
			poPoints = new(std::nothrow) OGRRawPoint[nCount];
			poRing->getPoints(poPoints);

			std::vector<GLuint> vecInIndice;

			for (int j = 0; j < nCount; j ++)
			{
				vecVertexs[nCurIndex*2] = poPoints[j].x;
				vecVertexs[nCurIndex*2+1] = poPoints[j].y;
				//vecIndices[nCurIndex] = nCurIndex;
				vecInIndice.push_back(nCurIndex);
				nCurIndex++;
			}

			vecIndices.push_back(vecInIndice);

			delete []poPoints;
			
		}

		return vecVertexs;
	}

	void DrawGeometryHighLight(GeoGeometry* pGeometry)
	{
		glLineWidth(2.0f);
		if (pGeometry->getGeometryType() == wkbPolygon || pGeometry->getGeometryType() == wkbPolygon25D)
		{
			GeoPolygon *pPolygon = (GeoPolygon *)pGeometry;
			int nPointCount = 0;
			std::vector<GLsizei> vecCounts;
			std::vector<std::vector<GLuint> > vecIndices;
			double *pCoords = SysDraw::GetPloygonPoints(pPolygon,nPointCount,vecCounts,vecIndices);

			int nRingCount = vecIndices.size();
			GLuint ** pIndices = new GLuint *[nRingCount];
			for (int i = 0; i < nRingCount; i ++)
			{
				pIndices[i] = &(vecIndices[i][0]);
			}

			glVertexPointer(2,GL_DOUBLE,0,pCoords);		//指定顶点
			glBufferData(GL_ARRAY_BUFFER,sizeof(double)*nPointCount*2,pCoords,GL_STATIC_DRAW);//分配空间

			glVertexPointer(2,GL_DOUBLE,0,NULL);		//指定顶点

			//线框方式绘制
			glColor4f(1.0f,0.0f,1.0f,1.0f);
			glMultiDrawElements(GL_LINE_LOOP,&vecCounts[0],GL_UNSIGNED_INT,(const GLvoid **)pIndices,nRingCount);
			glFlush();

			delete []pCoords;
			vecCounts.clear();
			vecIndices.clear();

			delete []pIndices;
		}

	}

	void SplitLabelStr(const char* pszFieldValue,std::vector<std::string> &vecAllFieldValue)
	{
		std::string strTemp;
		while (*pszFieldValue != '\0')
		{
			char cTemp = *pszFieldValue;
			if (cTemp >= 0 && cTemp <= 127)	//英文，则继续累加
			{
				strTemp += cTemp;
				pszFieldValue++;
				cTemp = *pszFieldValue;

				//下一个字符如果是空格或者汉字,则加入到数组中
				if (cTemp == ' ' || cTemp > 127 || cTemp < 0 || cTemp == '\0')
				{
					vecAllFieldValue.push_back(strTemp);
					strTemp.clear();
				}

			}

			else if (cTemp > 127 || cTemp < 0)		//如果是中文
			{
				strTemp += cTemp;
				pszFieldValue++;
				strTemp += *pszFieldValue;
				vecAllFieldValue.push_back(strTemp);
				strTemp.clear();
				pszFieldValue++;
			}
		}
	}
}


