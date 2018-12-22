#include "SysFeatureLabel.h"
#include "SysFeatureLabelAttribute.h"
#include "SysRGBColor.h"
#include "SysDrawText.h"
#include "SysCoordinate.h"
#include "SysDisplayTransform.h"
#include "SysRenderContext.h"
#include "SysClipper.h"

#include "SysUtilFunc.h"


namespace SysDraw
{

	SysFeatureLabel::SysFeatureLabel(void)
	{
		m_nFieldIndex = 1;
		m_pLabelAttributes = new SysFeatureLabelAttribute;
	}

	SysFeatureLabel::~SysFeatureLabel(void)
	{
		if (m_pLabelAttributes != NULL)
		{
			delete m_pLabelAttributes;
			m_pLabelAttributes = NULL;
		}
	}

	void SysFeatureLabel::RenderFeature( 
							SysRenderContext *pRenderContext, 
							SysFeature *pFeature, 
							bool bSelected, 
							SysFeatureLabelAttribute *pLabelAttributes /*= 0 */ ) /*const*/
	{
		//1、设置绘制文本时的属性

		SysDrawText drawText("C:\\Windows\\Fonts\\STLITI.TTF");
		
		drawText.SetTextColor(m_pLabelAttributes->GetColor());

		int nTextWidth = 20;
		int nTextHeight = 20;
		drawText.SetTextWidth(nTextWidth);
		drawText.SetTextHeight(nTextHeight);
		double dXoffset = m_pLabelAttributes->GetXOffSet();
		double dYoffset = m_pLabelAttributes->GetYOffSet();

		const char* pszValue = ((GeoFeature *)pFeature)->GetFieldAsString(m_nFieldIndex);
		std::vector<std::string> vecValueStrs;
		//vecValueStrs.push_back(pszValue);
		//SplitLabelStr(pszValue,vecValueStrs);

		//计算出每个标注文字的坐标和旋转的角度(应该根据不同的几何体类型区别对待)

		std::vector<LabelPoint> labelPoints;
		//GetLabelPoint(points,pFeature);			//普通的简单点模式标注

		SysDisplayTransform & tran = pRenderContext->GetDisplayTran();
		SysEnvelope envView;
		tran.GetVisibleBounds(envView);

		//GetLineStringLabelPoint((SysLineString *)((GeoFeature *)pFeature)->GetGeometryRef(),pszValue,envView,points,vecValueStrs,0);

		GetLabelPoint(pFeature,envView,labelPoints,vecValueStrs);

		assert(vecValueStrs.size() == labelPoints.size());

		int nCount = vecValueStrs.size();

		//2、找到合适的位置输出标注文本(需要另外封装函数)
		for (int i = 0; i < nCount; i ++)
		{
			LabelPoint point = labelPoints[i];
			RenderLabel(pRenderContext, point.m_dGeoX, point.m_dGeoY, vecValueStrs[i],
				drawText,dXoffset,dYoffset,point.m_dAngle,900,0);
		}
	}

	void SysFeatureLabel::RenderLabel( 
							SysRenderContext *pRenderContext, 
							double dGeoX,
							double dGeoY,
							std::string strText, 
							SysDrawText &drawFont, 
							double dXoffset, 
							double dYoffset, 
							double dAng, 
							int nWidth, 
							int nHeight, 
							int nAlignment /*= 0 */ )
	{
		assert(pRenderContext != NULL);
		SysDisplayTransform& tran = pRenderContext->GetDisplayTran();

		long x,y;
		tran.ToScreenPoint(dGeoX,dGeoY, x,y);
		drawFont.DrawTextExt(strText.c_str(),x,y,nWidth,nHeight);
	}

	void SysFeatureLabel::SetFieldIndex( int nFieldIndex )
	{
		m_nFieldIndex = nFieldIndex;
	}

	int SysFeatureLabel::GetFieldIndex() const
	{
		return m_nFieldIndex;
	}

	void SysFeatureLabel::GetLabelPoint( std::vector<LabelPoint>& points, SysFeature * pFeature )
	{
		GeoGeometry *pGeometry = ((GeoFeature *)pFeature)->GetGeometryRef();
		OGRPoint* poPoint = (OGRPoint *)pGeometry;

		int nWkbSize = pGeometry->WkbSize();
		unsigned char *pGeomBuf = new unsigned char[nWkbSize];
		pGeometry->exportToWkb(wkbNDR,pGeomBuf);

		unsigned char *pGeomBufTmp = pGeomBuf;

		OGRwkbGeometryType wkbType = pGeometry->getGeometryType();
		LabelPoint point;

		switch ( wkbType )
		{
		case wkbPoint25D:
		case wkbPoint:
		case wkbLineString25D:
		case wkbLineString:
		case wkbPolygon25D:
		case wkbPolygon:
			{
				std::vector<LabelPoint> vecPoints;
				GetLabelPoint(vecPoints,pGeomBuf,nWkbSize);

				for (int i = 0; i < vecPoints.size(); i ++)
				{
					points.push_back( vecPoints[i] );
				}
			}
			break;

		case wkbMultiPoint25D:
		case wkbMultiPoint:
		case wkbMultiLineString25D:
		case wkbMultiLineString:
		case wkbMultiPolygon25D:
		case wkbMultiPolygon:
			{
				pGeomBuf += 1 + sizeof( wkbType );
				int nFeatures = *( unsigned int * )pGeomBuf;
				pGeomBuf += sizeof( int );

				unsigned char *pSubData = pGeomBuf;
				for ( int i = 0; i < nFeatures && pSubData != NULL; ++i )
				{
					std::vector<LabelPoint> vecPoints;
					pSubData = GetLabelPoint(vecPoints,pSubData,pGeomBuf + nWkbSize - pSubData);

					for (int i = 0; i < vecPoints.size(); i ++)
					{
						points.push_back( vecPoints[i] );
					}
				}
			}
			break;
		default:
			break;
		}

		delete []pGeomBufTmp;
		pGeomBufTmp = NULL;
	}

	unsigned char* SysFeatureLabel::GetLabelPoint( std::vector<LabelPoint>& vecPoints, unsigned char* pWkbBuf, size_t nWkbLen )
	{
		assert( sizeof( int ) == 4 );
		assert( sizeof( OGRwkbGeometryType ) == 4 );
		assert( sizeof( double ) == 8 );

		if ( NULL == pWkbBuf)
		{
			return NULL;
		}

		OGRwkbGeometryType wkbType;
		//assert( pWkbBuf + 1 + sizeof( wkbType ) <= nWkbLen );

		pWkbBuf++; 
		memcpy( &wkbType, pWkbBuf, sizeof( wkbType ) );
		pWkbBuf += sizeof( wkbType );

		int nDims = 2;		//维度

		LabelPoint point;

		switch ( wkbType )
		{
		case wkbPoint:
		case wkbPoint25D:
			{
				//assert( pWkbBuf + 2*sizeof( double ) <= nWkbLen );
				double *pts = ( double * )pWkbBuf;
				point.m_dGeoX = pts[0];
				point.m_dGeoY = pts[1];
				point.m_dAngle = 0.0;
				pWkbBuf += 2 * sizeof( double );
			}
			break;

		case wkbLineString25D:
			nDims = 3;
		case wkbLineString: 
			{
				//assert( pWkbBuf + sizeof( int ) <= nWkbLen );
				int nPoints = *( unsigned int * )pWkbBuf;
				pWkbBuf += sizeof( int );

				//assert( pWkbBuf + nPoints*sizeof( double )*nDims <= nWkbLen );

				double *pts = ( double * )pWkbBuf;
				double tl = 0.0;
				for ( int i = 1; i < nPoints; i++ )
				{
					double dx = pts[nDims*i]   - pts[nDims*( i-1 )];
					double dy = pts[nDims*i+1] - pts[nDims*( i-1 )+1];
					tl += sqrt( dx * dx + dy * dy );
				}
				tl /= 2.0;

				double l = 0.0;
				for ( int i = 1; i < nPoints; i++ )
				{
					double dx = pts[nDims*i]   - pts[nDims*( i-1 )];
					double dy = pts[nDims*i+1] - pts[nDims*( i-1 )+1];
					double dl = sqrt( dx * dx + dy * dy );

					if ( l + dl > tl )
					{
						double k = ( tl - l ) / dl;

						point.m_dGeoX = pts[nDims*( i-1 )]   + k * dx;
						point.m_dGeoY = pts[nDims*( i-1 )+1] + k * dy;
						//point.m_dAngle = atan2( dy, dx ) * 180.0 * M_1_PI;
						point.m_dAngle = 0;
						break;
					}

					l += dl;
				}

				pWkbBuf += nPoints * sizeof( double ) * nDims;
			}
			break;

		case wkbPolygon25D:
			nDims = 3;
		case wkbPolygon: 
			{
				//assert( pWkbBuf + sizeof( int ) <= nWkbLen );
				int nRings = *( unsigned int * )pWkbBuf;
				pWkbBuf += sizeof( int );

				for ( int i = 0; i < nRings; ++i )
				{
					int nPoints = *( unsigned int * )pWkbBuf;
					pWkbBuf += sizeof( int );

					if ( i == 0 )
					{
						double sx = 0.0, sy = 0.0;
						double *pts = ( double* ) pWkbBuf;
						for ( int j = 0; j < nPoints - 1; j++ )
						{
							sx += pts[nDims*j];
							sy += pts[nDims*j+1];
						}

						point.m_dGeoX = sx / ( nPoints - 1 );
						point.m_dGeoY = sy / ( nPoints - 1 );
						point.m_dAngle = 0.0;
					}

					pWkbBuf += nPoints * sizeof( double ) * nDims;
				}
			}
			break;

		default:
			return NULL;
		}

		vecPoints.push_back(point);

		return pWkbBuf;
	}

	void SysFeatureLabel::GetLabelPoint( SysFeature * pFeature, const SysEnvelope& envView, 
										std::vector<LabelPoint>& points, std::vector<std::string> &vecLabelStr )
	{
		GeoGeometry *pGeometry = ((GeoFeature *)pFeature)->GetGeometryRef();
		points.clear();
		vecLabelStr.clear();

		const char* pszFieldValue = ((GeoFeature *)pFeature)->GetFieldAsString(m_nFieldIndex);

		if (NULL == pGeometry)
		{
			return;
		}

		OGRwkbGeometryType wkbType = pGeometry->getGeometryType();

		switch ( wkbType )
		{
		case wkbPoint25D:
		case wkbPoint:
			{
				//
			}
			break;

		case wkbLineString25D:
		case wkbLineString:
			{
				SysLineString* pLine = (SysLineString *)pGeometry;
				GetLineStringLabelPoint(pLine,pszFieldValue,envView,points,vecLabelStr,0);
			}
			break;

		case wkbPolygon:
		case wkbPolygon25D:
			{
				SysPolygon* pPolygon = (SysPolygon *)pGeometry;
				GetPolygonLabelPoint(pPolygon,pszFieldValue,envView,points,vecLabelStr,0);
			}
			break;

		case wkbMultiPoint25D:
		case wkbMultiPoint:
			{
				//
			}
			break;

		case wkbMultiLineString25D:
		case wkbMultiLineString:
			{
				OGRMultiLineString* pMultiLine = (OGRMultiLineString *)pGeometry;
				int nGeomCount = pMultiLine->getNumGeometries();
				for (int i = 0; i < nGeomCount; i ++)
				{
					SysLineString* pLine = (SysLineString *)pMultiLine->getGeometryRef(i);
					GetLineStringLabelPoint(pLine,pszFieldValue,envView,points,vecLabelStr,0);
				}
			}
			break;

		case wkbMultiPolygon25D:
		case wkbMultiPolygon:
			{
				OGRMultiPolygon* pMultiPolygon = (OGRMultiPolygon *)pGeometry;
				int nGeomCount = pMultiPolygon->getNumGeometries();
				for (int i = 0; i < nGeomCount; i ++)
				{
					SysPolygon* pPolygon = (SysPolygon *)pMultiPolygon->getGeometryRef(i);
					GetPolygonLabelPoint(pPolygon,pszFieldValue,envView,points,vecLabelStr,0);
				}
			}
			break;
		default:
			break;
		}
		
	}

	void SysFeatureLabel::SetLabelAttribute( SysFeatureLabelAttribute *pLabelAttributes )
	{
		*m_pLabelAttributes = *pLabelAttributes;
	}

	void SysFeatureLabel::GetLineStringLabelPoint( 
							SysLineString* pLine, const char* pszFieldValue, const SysEnvelope& envView, 
							std::vector<LabelPoint>& points, std::vector<std::string>& vecLabelStr, double dbScale )
	{
		std::vector<int> vecIndex;
		std::vector<SysCoordinate> vecPoints;

		int nPointCount = ((GeoLineString *)pLine)->getNumPoints();
		SysCoordinate* pCoords = new SysCoordinate[nPointCount];
		((GeoLineString *)pLine)->getPoints((OGRRawPoint *)pCoords);

		SysClipper::PolylineClip(pCoords,nPointCount,(SysEnvelope &)envView,vecIndex,vecPoints);

		if (vecPoints.empty() || vecIndex.empty())
		{
			return;
		}

		//计算起始点坐标的倾角
		double dbX0 = vecPoints[0].x;
		double dbY0 = vecPoints[0].y;
		double dbXn = vecPoints[vecPoints.size()-1].x;
		double dbYn = vecPoints[vecPoints.size()-1].y;
		double dbTheta = fabs( (dbYn - dbY0)/(dbXn - dbX0));

		//smartlog<<dbTheta;

		//采样顺序的预处理
		bool bReverse = false;
		/*if ((dbTheta >= -45 && dbTheta < 45) || (dbTheta >= 135 && dbTheta < 180) || (dbTheta >= -180 && dbTheta < -135))
		{
			if (dbX0 >= dbXn)
			{
				bReverse = true;
			}
		}

		else if ((dbTheta >= 45 && dbTheta < 135) || (dbTheta >= -135 && dbTheta < -45))
		{
			if (dbY0 > dbYn)
			{
				bReverse = true;
			}
		}*/
		if (dbTheta > 1)
		{
			if (dbY0 < dbYn)
			{
				bReverse = true;
			}
		}

		else if (dbTheta <= 1)
		{
			if (dbX0 >= dbXn)
			{
				bReverse = true;
			}
		}

		//std::vector<std::string> vecAllFieldValue;
		SplitLabelStr(pszFieldValue,vecLabelStr);

		int nLen = vecLabelStr.size();

		if (bReverse)
		{
			//smartlog<<"坐标逆序";
			std::reverse(vecLabelStr.begin(),vecLabelStr.end());
		}

		double dbLength = SysClipper::GetLineStringLength( &vecPoints[0],vecPoints.size() );
		double dbDet = dbLength / (nLen+1);			//每个汉字的间隔

		LabelPoint point;
		SysCoordinate coordPt;
		for (int i = 0; i < nLen; i ++)
		{
			SysClipper::GetValuePoint(&vecPoints[0],vecPoints.size(),dbDet*(i+1),&coordPt,&point.m_dAngle);
			point.m_dGeoX = coordPt.x;
			point.m_dGeoY = coordPt.y;
			
			//还需要计算这个点倾角
			//point.m_dAngle = 0;

			points.push_back(point);
		}

		if (pCoords != NULL)
		{
			delete []pCoords;
			pCoords = NULL;
		}
	}

	void SysFeatureLabel::GetPolygonLabelPoint( 
								SysPolygon* pSysPolygon, 
								const char* pszFieldValue, 
								const SysEnvelope& envView, 
								std::vector<LabelPoint>& points, 
								std::vector<std::string>& vecLabelStr, 
								double dbScale )
	{
		//获得裁剪后的多边形
		GeoGeometry* pGeometry = (GeoGeometry *)SysClipper::PolygonClip(pSysPolygon,(SysEnvelope &)envView);

		/*points.clear();
		vecLabelStr.clear();*/

		LabelPoint point;
		

		OGRwkbGeometryType wkbType = pGeometry->getGeometryType();
		switch (wkbType)
		{
		case wkbPolygon:
		case wkbPolygon25D:
			{
				GeoPolygon* pPolygon = (GeoPolygon *)pGeometry;

				OGRPoint pt; //= new OGRPoint;
				pPolygon->Centroid(&pt);
				point.m_dGeoX = pt.getX();
				point.m_dGeoY = pt.getY();
				point.m_dAngle = 0;
				points.push_back(point);
				vecLabelStr.push_back(pszFieldValue);
			}
			break;

		case wkbMultiPolygon:
		case wkbMultiPolygon25D:
			{
				OGRMultiPolygon* pMultiPolygon = (OGRMultiPolygon *)pGeometry;
				int nGeomCount = pMultiPolygon->getNumGeometries();
				for (int i = 0; i < nGeomCount; i ++)
				{
					GeoPolygon* pPolygon = (GeoPolygon *)pMultiPolygon->getGeometryRef(i);

					OGRPoint pt; // = new OGRPoint;
					pPolygon->Centroid(&pt);
					point.m_dGeoX = pt.getX();
					point.m_dGeoY = pt.getY();
					point.m_dAngle = 0;
					points.push_back(point);
					vecLabelStr.push_back(pszFieldValue);
				}
			}
			break;

		default:
			break;
		}

		//GeoGeometryFactory::destroyGeometry(pGeometry);
	}

	SysFeatureLabelAttribute * SysFeatureLabel::GetLabelAttribute() const
	{
		return m_pLabelAttributes;
	}
}
