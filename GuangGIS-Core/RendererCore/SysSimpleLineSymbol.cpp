#include "SysSimpleLineSymbol.h"
#include "SysRGBColor.h"


//opengl
#include <GL/glew.h>


namespace SysDraw
{

	SysSimpleLineSymbol::SysSimpleLineSymbol(void)
	{
		SysSymbol::SetOutLineWidth(1.0);
		m_eLineStyle = Dashed_Line;
		SetLineStyle(m_eLineStyle);
	}

	SysSimpleLineSymbol::~SysSimpleLineSymbol(void)
	{
	}

	SysSymbolType SysSimpleLineSymbol::GetSymbolType() const
	{
		return LineSymbol;
	}

	void SysSimpleLineSymbol::RenderFeature(const SysFeature* pFeature,SysRenderContext* pContext, bool bHighLight)
{
		GeoFeature* pGeoFeature = (GeoFeature *)pFeature;
		GeoGeometry* pGeometry = pGeoFeature->GetGeometryRef();
		GeoLineString* pLine = (GeoLineString *)pGeometry;

		/*GLuint nBuffer;
		glGenBuffers(1,&nBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,nBuffer);*/

		//smartlog<<"sssss";

		int nPointCount = pLine->getNumPoints();
		
		OGRRawPoint* poPoints = new OGRRawPoint[nPointCount];
		pLine->getPoints(poPoints);

		/*glBegin(GL_LINE_STRIP);
		for (int i = 0; i < nPointCount; i ++)
		{
			glVertex2d(poPoints[i].x,poPoints[i].y);
		}

		glEnd();

		delete []poPoints;*/

		//smartlog<<"hhhhhhhhhhhh";


		std::vector<double> vecVertexs;
		vecVertexs.resize(nPointCount*2);
		GLuint *pIndex = new GLuint[nPointCount];
		for (int j = 0; j < nPointCount; j ++)
		{
			vecVertexs[j*2] = (poPoints[j].x);
			vecVertexs[j*2+1] = (poPoints[j].y);
			pIndex[j] = j;
		}

		delete []poPoints;

		//glBufferData(GL_ARRAY_BUFFER,sizeof(double)*nPointCount*2,&vecVertexs[0],GL_STATIC_DRAW);//分配空间
		glVertexPointer(2,GL_DOUBLE,0,&vecVertexs[0]);		//指定顶点

		if (bHighLight)
		{
			glLineWidth(3.0f);
			glColor3f(0.0f,1.0f,1.0f);
			glLineStipple(1,0xffff);
		}

		else
		{
			glLineWidth(GetOutLineWidth());
			SysRGBAColor color = *m_pColor;
			glColor4f(color.GetRed(),color.GetGreen(),color.GetBlue(),color.GetAlpha());
		}

		SetLineStyle(m_eLineStyle);
		glDrawElements(GL_LINE_STRIP,nPointCount,GL_UNSIGNED_INT,pIndex);

		vecVertexs.clear();

		delete []pIndex;

		//glColor4f(1,0,1,1);

	}

	void SysSimpleLineSymbol::SetLineStyle( SysLineStyle eStyle )
	{
		m_eLineStyle = eStyle;

		if (m_eLineStyle == Solid_line)
		{
			glLineStipple(1,0xffff);
		}

		else if (m_eLineStyle == Dashed_Line)
		{
			glLineStipple(1,0x00ff);
		}

		else if (m_eLineStyle == Dotted_Line)
		{
			glLineStipple(1,0x0101);
		}
	}
}
