#include "SysSimpleFillSymbol.h"
#include "SysSimpleLineSymbol.h"
#include "SysRGBColor.h"
#include "SysMapManager.h"
#include "SysMap.h"

#include "SysDrawText.h"
#include "SysRGBColor.h"
#include "SysUtilFunc.h"

#include "ogrsf_frmts.h"

#include "SysUtility/SmartPtr.h"
#include "SysDataSource/SpatialReference.h"
#include "SysDataSource/CoordTransformation.h"

#include <GL/glew.h>

namespace SysDraw
{

	SysSimpleFillSymbol::SysSimpleFillSymbol(void)
	{
		m_pOutLineSymbol = new(std::nothrow) SysSimpleLineSymbol;
	}

	SysSimpleFillSymbol::~SysSimpleFillSymbol(void)
	{
		if (m_pOutLineSymbol != NULL)
		{
			delete m_pOutLineSymbol;
			m_pOutLineSymbol = NULL;
		}
	}

	SysSymbolType SysSimpleFillSymbol::GetSymbolType() const
	{
		return FillSymbol;
	}

	static void DrawPolygonEXT(GeoPolygon* pPolygon,const SysRGBAColor &color,const SysRGBAColor& outlineColor, bool bHighLight)
	{
		int nPointCount = 0;
		std::vector<GLsizei> vecCounts;
		std::vector<std::vector<GLuint> > vecIndices;
		double *pCoords = GetPloygonPoints(pPolygon,nPointCount,vecCounts,vecIndices);

		int nRingCount = vecIndices.size();
		GLuint ** pIndices = new GLuint *[nRingCount];
		for (int i = 0; i < nRingCount; i ++)
		{
			pIndices[i] = &(vecIndices[i][0]);
		}

		glPolygonOffset(1.0f,1.0f);

		glColor4f(color.GetRed(),color.GetGreen(),color.GetBlue(),color.GetAlpha());
		glVertexPointer(2,GL_DOUBLE,0,pCoords);		//指定顶点
		//glBufferData(GL_ARRAY_BUFFER,sizeof(double)*nPointCount*2,pCoords,GL_STATIC_DRAW);//分配空间

		//glVertexPointer(2,GL_DOUBLE,0,NULL);		//指定顶点

		/*以下是在模板测试*/
		glEnable(GL_STENCIL_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); //
		glDepthMask(GL_FALSE);
		glStencilFunc(GL_ALWAYS, 0x1, 0x1);
		glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);

		glMultiDrawElements(GL_TRIANGLE_FAN,&vecCounts[0],GL_UNSIGNED_INT,(const GLvoid **)pIndices,nRingCount);

		//在颜色缓冲绘图
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glStencilMask(GL_TRUE);

		glStencilOp(GL_ZERO,GL_ZERO,GL_ZERO);
		glStencilFunc(GL_EQUAL,0x1,0x1); 
		//glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
		glMultiDrawElements(GL_TRIANGLE_FAN,&vecCounts[0],GL_UNSIGNED_INT,(const GLvoid **)pIndices,nRingCount);

		//glStencilFunc(GL_ALWAYS,0,0x1); 

		glDisable(GL_STENCIL_TEST);

		//线框方式绘制
		glLineStipple(1,0xffff);
		if (bHighLight)
		{
			glColor3f(0.0f,1.0f,1.0f);
			glLineWidth(3.0f);
		}

		else
		{
			glColor4f(outlineColor.GetRed(),outlineColor.GetGreen(),outlineColor.GetBlue(),outlineColor.GetAlpha());
		}

		glMultiDrawElements(GL_LINE_LOOP,&vecCounts[0],GL_UNSIGNED_INT,(const GLvoid **)pIndices,nRingCount);
		
		glFlush();

		delete []pCoords;
		vecCounts.clear();
		vecIndices.clear();

		delete []pIndices;
	}

	static void DrawPolygon(GeoPolygon* pPolygon,const SysRGBAColor &color,const SysRGBAColor& outlineColor, bool bHighLight)
	{
		GeoPolygon* poPolygon = (GeoPolygon*)pPolygon;

		GeoLineString* poRing = poPolygon->getExteriorRing();

		int nPointCount = poRing->getNumPoints();

		OGRRawPoint* poPoints = new OGRRawPoint[nPointCount];
		poRing->getPoints(poPoints);
		double *vecVertexs = new double[nPointCount*2];
		GLuint *pIndex = new GLuint[nPointCount];	//索引数组

		for (int j = 0; j < nPointCount; j ++)
		{
			vecVertexs[j*2] = poPoints[j].x;
			vecVertexs[j*2+1] = poPoints[j].y;
			pIndex[j] = j;
		}

		//渲染多边形
		//glFrontFace(GL_CW);
		//glPolygonMode(GL_FRONT,GL_FILL);
		glPolygonOffset(1.0f,1.0f);

		srand((unsigned int)time(NULL));
		float fRed = rand()%255 / 255.0f;
		float fGreen = rand()%255 / 255.0f;
		float fBlue = rand()%255 / 255.0f;

		//glColor3f(fRed,fGreen,fBlue);

		glColor4f(color.GetRed(),color.GetGreen(),color.GetBlue(),color.GetAlpha());
		glVertexPointer(2,GL_DOUBLE,0,&vecVertexs[0]);		//指定顶点
		//glBufferData(GL_ARRAY_BUFFER,sizeof(double)*nPointCount*2,&vecVertexs[0],GL_STATIC_DRAW);//分配空间

		//glVertexPointer(2,GL_DOUBLE,0,NULL);		//指定顶点

		glEnable(GL_STENCIL_TEST);

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); //
		glDepthMask(GL_FALSE);
		glStencilFunc(GL_ALWAYS, 0x1, 0x1);
		glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		glDrawElements(GL_TRIANGLE_FAN,nPointCount,GL_UNSIGNED_INT,pIndex);

		int nRingCount = poPolygon->getNumInteriorRings();
		for (int nIndex = 0; nIndex < nRingCount; nIndex ++)
		{
			GeoLineString* poRing = poPolygon->getInteriorRing(nIndex);

			int nPointCount = poRing->getNumPoints();

			OGRRawPoint* poPoints = new OGRRawPoint[nPointCount];
			poRing->getPoints(poPoints);

			glColor4f(color.GetRed(),color.GetGreen(),color.GetBlue(),color.GetAlpha());
			glBegin(GL_TRIANGLE_FAN);
			for (int i = 0; i < nPointCount; i ++)
			{
				glVertex2d(poPoints[i].x,poPoints[i].y);
			}
			glEnd();

			delete []poPoints;
		}

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		
		glStencilOp(GL_ZERO,GL_ZERO,GL_ZERO);
		glStencilFunc(GL_EQUAL,0x1,0x1);                
		//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glDrawElements(GL_TRIANGLE_FAN,nPointCount,GL_UNSIGNED_INT,pIndex);

		for (int nIndex = 0; nIndex < nRingCount; nIndex ++)
		{
			GeoLineString* poRing = poPolygon->getInteriorRing(nIndex);

			int nPointCount = poRing->getNumPoints();

			OGRRawPoint* poPoints = new OGRRawPoint[nPointCount];
			poRing->getPoints(poPoints);

			glColor4f(color.GetRed(),color.GetGreen(),color.GetBlue(),color.GetAlpha());
			glBegin(GL_TRIANGLE_FAN);
			for (int i = 0; i < nPointCount; i ++)
			{
				glVertex2d(poPoints[i].x,poPoints[i].y);
			}
			glEnd();

			//绘制边框
			glColor4f(outlineColor.GetRed(),outlineColor.GetGreen(),outlineColor.GetBlue(),outlineColor.GetAlpha());
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < nPointCount; i ++)
			{
				glVertex2d(poPoints[i].x,poPoints[i].y);
			}
			glEnd();

			delete []poPoints;
		}

		glDisable(GL_STENCIL_TEST);


		//glDisable(GL_STENCIL_TEST);

		//线框方式绘制
		glColor4f(outlineColor.GetRed(),outlineColor.GetGreen(),outlineColor.GetBlue(),outlineColor.GetAlpha());
		glDrawElements(GL_LINE_STRIP,nPointCount,GL_UNSIGNED_INT,pIndex);
		glFlush();

		delete []poPoints;
		delete []pIndex;
		delete []vecVertexs;

	}

	void SysSimpleFillSymbol::RenderFeature(const SysFeature* pFeature,SysRenderContext* pContext, bool bHighLight)
{
		GeoFeature* pGeoFeature = (GeoFeature *)pFeature;
		GeoGeometry* pGeometry = pGeoFeature->GetGeometryRef();

		//glDisable(GL_BLEND);

		glLineWidth(GetOutLineWidth());

		if (pGeometry->getGeometryType() == wkbPolygon || pGeometry->getGeometryType() == wkbPolygon25D)
		{
			DrawPolygonEXT((GeoPolygon *)pGeometry,*m_pColor,*m_pOutLineColor, bHighLight);
			//DrawPolygon((GeoPolygon *)pGeometry,*m_pColor,*m_pOutLineColor, bHighLight);

		}
		else if (pGeometry->getGeometryType() == wkbMultiPolygon || pGeometry->getGeometryType() == wkbMultiPolygon25D)
		{
			OGRMultiPolygon* poMultiPolygon = (OGRMultiPolygon *)pGeometry;
			int nGeomNum = poMultiPolygon->getNumGeometries();
			for (int i = 0; i < nGeomNum; i ++)
			{
				DrawPolygonEXT((GeoPolygon *)poMultiPolygon->getGeometryRef(i),*m_pColor,*m_pOutLineColor, bHighLight);
				//DrawPolygon((GeoPolygon *)poMultiPolygon->getGeometryRef(i),*m_pColor,*m_pOutLineColor, bHighLight);
			}
		}

		
		/*glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		glLoadIdentity();
		glOrtho(0,1300,900,0,-1,1);

		glEnable( GL_TEXTURE_2D );

		SysDraw::SysRGBAColor color(1,0,0.4);

		SysDraw::SysDrawText drawText1("C:\\Windows\\Fonts\\STLITI.TTF");
		drawText1.SetTextColor(color);
		drawText1.SetTextWidth(30);
		drawText1.SetTextHeight(30);
		drawText1.DrawTextExt("FreeType绘制的文字",500,500,900,0);

		glDisable(GL_TEXTURE_2D);

		glPopMatrix();*/

		
		//glColor4f(1,1,1,1);

	}

}
