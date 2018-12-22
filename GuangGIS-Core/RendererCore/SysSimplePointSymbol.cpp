#include "SysSimplePointSymbol.h"
#include "SysRGBColor.h"


//#include "ogrsf_frmts.h"
#include "SysGeometryDef.h"
#include "SysDisplayTransform.h"
#include "SysRenderContext.h"

#include <GL/glew.h>

namespace SysDraw
{

	SysSimplePointSymbol::SysSimplePointSymbol(void)
	{
		Init();
	}

	SysSimplePointSymbol::~SysSimplePointSymbol(void)
	{
	}

	SysSymbolType SysSimplePointSymbol::GetSymbolType() const
	{
		return PointSymbol;
	}

	void SysSimplePointSymbol::Init()
	{
		m_fSize = 7.0f;		
		m_fAngle = 0.0f;		
		m_fXOffset = 0.0f;
		m_fYOffset = 0.0f;
		m_bUseOutLine = true;
		m_eType = CYCLE_POINT;

		SysSymbol::SetOutLineWidth(0.4f);
		SysSymbol::SetOutLineColor(SysRGBAColor(0,0,0));

		//随机设置里面的填充颜色
		srand((unsigned int)time(NULL));
		float fRed = rand()%255 / 255.0f;
		float fGreen = rand()%255 / 255.0f;
		float fBlue = rand()%255 / 255.0f;
		SysSymbol::SetColor(SysRGBAColor(fRed,fGreen,fBlue));
		
	}

	void SysSimplePointSymbol::RenderSimplePoint(SysPoint* pPoint,SysRenderContext* pContext)
	{
		OGRPoint* pOgrPoint = (OGRPoint *)pPoint;
		glPointSize(m_fSize);
		glColor3f(m_pColor->GetRed(),m_pColor->GetGreen(),m_pColor->GetBlue());

		double dbX = pOgrPoint->getX();
		double dbY = pOgrPoint->getY();

		glBegin(GL_POINTS);
			glVertex2d(dbX,dbY);
		glEnd();

		//glRectd(dbX-100,dbY-100,dbX+100,dbY+100);

		SysDisplayTransform& tran = pContext->GetDisplayTran();
		double dMapUnitPerPixel = tran.GetMapUnitsPerPixel();
		double dbRadius = dMapUnitPerPixel * m_fSize * 0.5;

		//绘制外部边框
		glColor3f(m_pOutLineColor->GetRed(),m_pOutLineColor->GetGreen(),m_pOutLineColor->GetBlue());  
		glBegin(GL_LINE_STRIP);          
		for(int i = 0; i <= 360; i ++)  
		{  
			double x = dbRadius*cos(2*i*M_PI/360);
			x += dbX;
			double y = dbRadius*sin(2*i*M_PI/360);
			y += dbY;
			glVertex2f(x,y); 
		}
		glEnd();

		

		//GeoLineString* pLine = (GeoLineString *)GeoGeometryFactory::approximateArcAngles(dbX,dbY,0,dbRadius,dbRadius,0,0,M_PI*2,M_PI/180.0);
	}

	void SysSimplePointSymbol::RenderFeature(const SysFeature* pFeature,SysRenderContext* pContext, bool bHighLight)
{
		GeoFeature* pGeoFeature = (GeoFeature *)pFeature;
		GeoGeometry* pGeometry = pGeoFeature->GetGeometry();
		if (NULL == pGeometry)
		{
			return;
		}

		if (pGeometry->getGeometryType() == wkbPoint || pGeometry->getGeometryType() == wkbPoint25D)
		{
			SysPoint* pPoint = (SysPoint *)pGeometry;
			RenderSimplePoint(pPoint,pContext);
		}

		else if (pGeometry->getGeometryType() == wkbMultiPoint || pGeometry->getGeometryType() == wkbMultiPoint25D)
		{
			OGRMultiPoint* pMultiPoint = (OGRMultiPoint *)pGeometry;
			int nGeomNum = pMultiPoint->getNumGeometries();
			for (int i = 0; i < nGeomNum; i ++)
			{
				RenderSimplePoint((SysPoint *)pMultiPoint->getGeometryRef(i),pContext);
			}
		}
	}

}
