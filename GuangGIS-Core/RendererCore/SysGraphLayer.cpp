#include "SysGraphLayer.h"
#include "SysGraphElement.h"
#include "SysRectElement.h"
#include "SysLineElement.h"
#include "SysPolygonElement.h"
#include "SysDisplayTransform.h"
#include "SysRenderContext.h"
#include "SysImagePreProcess.h"
#include "SysCoordinate.h"

#include "SysCycleElement.h"

#include <GL/glew.h>

using namespace SysDraw;

//各种图元的绘制函数

void DrawRectElem(SysRectElement* pRect,const SysDisplayTransform& DispTran)
{
	double x1 = pRect->GetMinX();
	double y1 = pRect->GetMinY();
	long nX1,nY1;
	DispTran.ToScreenPoint(x1,y1,nX1,nY1);

	double dbX1,dbY1,z;
	ScreenToWorld(nX1,nY1,&dbX1,&dbY1,&z);

	double x2 = pRect->GetMaxX();
	double y2 = pRect->GetMaxY();
	long nX2,nY2;
	DispTran.ToScreenPoint(x2,y2,nX2,nY2);

	double dbX2,dbY2;
	ScreenToWorld(nX2,nY2,&dbX2,&dbY2,&z);

	glRectd(dbX1,dbY1,dbX2,dbY2);

	glColor3f(1.0f,0.0f,0.0f);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glLineStipple(1,0xffff);
	glRectd(dbX1,dbY1,dbX2,dbY2);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	//glRectd(-0.5,-0.5,0.5,0.5);
	glFlush();
}

void DrawLineElem(SysLineElement* pLine,const SysDisplayTransform& DispTran)
{
	SysCoordinate *pCoords = pLine->GetPoints();
	int nPointCount = pLine->GetPointCount();

	glBegin(GL_LINE_STRIP);
	for (int nIndex = 0; nIndex < nPointCount; nIndex ++)
	{
		/*long nX1,nY1;
		DispTran.ToScreenPoint(pCoords[nIndex].x,pCoords[nIndex].y,nX1,nY1);

		double dbX1,dbY1,z;
		ScreenToWorld(nX1,nY1,&dbX1,&dbY1,&z);*/

		glVertex2d(pCoords[nIndex].x,pCoords[nIndex].y);
	}
	glEnd();
}

void DrawPolyElem(SysPolygonElement* pPolygon,const SysDisplayTransform& DispTran)
{
	SysLineElement* pExtRing = pPolygon->GetExteriorRing();
	SysCoordinate* pPoints = pExtRing->GetPoints();
	int nPtCount = pExtRing->GetPointCount();

	double *vecVertexs = new(std::nothrow) double[nPtCount * 2];
	GLuint *pIndex = new(std::nothrow) GLuint[nPtCount];	//索引数组


	for (int j = 0; j < nPtCount; j ++)
	{
		vecVertexs[j*2] = pPoints[j].x;
		vecVertexs[j*2+1] = pPoints[j].y;
		pIndex[j] = j;
	}

	//渲染多边形
	glEnable(GL_STENCIL_TEST);          // 启动模板测试

	glFrontFace(GL_CW);
	glPolygonMode(GL_FRONT_AND_BACK,GL_POLYGON);

	glVertexPointer(2,GL_DOUBLE,0,&vecVertexs[0]);		//指定顶点
	glBufferData(GL_ARRAY_BUFFER,sizeof(double)*nPtCount*2,&vecVertexs[0],GL_STATIC_DRAW);//分配空间

	glVertexPointer(2,GL_DOUBLE,0,NULL);		//指定顶点

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // 
	glStencilFunc(GL_ALWAYS, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);

	glDrawElements(GL_TRIANGLE_FAN,nPtCount,GL_UNSIGNED_INT,pIndex);

	//在颜色缓冲中绘图
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);    // 
	glStencilFunc(GL_EQUAL, 0x1, 0x1);                  // 
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glDrawElements(GL_POLYGON,nPtCount,GL_UNSIGNED_INT,pIndex);


	glDisable(GL_STENCIL_TEST);

	//线框方式绘制
	glColor3f(1.0, 0.0, 0.0);
	glDrawElements(GL_LINE_LOOP,nPtCount,GL_UNSIGNED_INT,pIndex);
	glFlush();

	delete []pIndex;
	delete []vecVertexs;

}

void DrawCycleElem(SysCycleElement* pCycle,const SysDisplayTransform& DispTran)
{
	double dMapUnitPerPixel = DispTran.GetMapUnitsPerPixel();
	double dbRadius = dMapUnitPerPixel * 10;

	double dbX = 0;
	double dbY = 0;
	pCycle->GetCenterPoint(dbX,dbY);

	//绘制外部边框 
	glBegin(GL_POLYGON);          
	for(int i = 0; i <= 360; i ++)  
	{  
		double x = dbRadius*cos(2*i*M_PI/360);
		x += dbX;
		double y = dbRadius*sin(2*i*M_PI/360);
		y += dbY;
		glVertex2f(x,y); 
	}
	glEnd();
}


namespace SysDraw
{

	SysGraphLayer::SysGraphLayer(std::string strLyrName):SysMapLayer(GraphicsLayer,strLyrName)
	{
		m_vecGraphicElement.clear();
	}

	SysGraphLayer::~SysGraphLayer(void)
	{
		int nCount = m_vecGraphicElement.size();
		for (int i = 0; i < nCount; i ++)
		{
			DestroyGraphElement( m_vecGraphicElement[i]);
			m_vecGraphicElement[i] = NULL;
		}

		m_vecGraphicElement.clear();

		SysEnvelope env;
		env.SetNull();
		SetLayerExtent(env);
	}

	void SysGraphLayer::AddGraphicElement(SysGraphElement* ptrElement)
	{
		m_vecGraphicElement.push_back(ptrElement);

		SysEnvelope env = GetLayerExtent();
		env.ExpandToInclude(ptrElement->GetEnvelope());

		//然后设置
		SetLayerExtent(env);
	}

	SysGraphElement* SysGraphLayer::GetGraphicElement(int nIndex) const
	{
		if (nIndex < 0 || nIndex >= m_vecGraphicElement.size())
		{
			return NULL;
		}

		return m_vecGraphicElement[nIndex];
	}

	size_t SysGraphLayer::GetGraphicElementCount() const
	{
		return m_vecGraphicElement.size();
	}

	bool SysGraphLayer::RemoveGraphicElement(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecGraphicElement.size())
		{
			return false;
		}

		std::vector<SysGraphElement*>::iterator iter = m_vecGraphicElement.begin();
		iter += nIndex;

		SysGraphElement* pGra = *iter;
		DestroyGraphElement(pGra);
		m_vecGraphicElement.erase(iter);
		iter = m_vecGraphicElement.begin();

		return true;
	}

	bool SysGraphLayer::Draw( SysRenderContext* rendererContext ) 
	{
		//SysDisplayTransform& dispTran = rendererContext->GetDisplayTran();

		/*glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_XOR);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);*/

		//

		/*glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

		GLuint nBuffer;
		glGenBuffers(1,&nBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,nBuffer);		//绑定对象

		glLineWidth(2.0);


		int nCount = m_vecGraphicElement.size();
		for (int i = 0; i < nCount; i ++)
		{
			//glColor4f(0.0,1.0,0,0.02);

			DrawElement(i,rendererContext);

			//glColor4f(0.0,1.0,0,1.0);
			
		}

		/*glLogicOp(GL_COPY);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);*/

		glDeleteBuffers(1,&nBuffer);
		

		return 1;
	}

	bool SysGraphLayer::DrawElement(int nIndex, SysRenderContext* rendererContext ) const
	{
		SysGraphElement* pElem = m_vecGraphicElement[nIndex];
		if (NULL == pElem)
		{
			return false;
		}
		SysDisplayTransform& dispTran = rendererContext->GetDisplayTran();
		GraphicsElementType graType = pElem->GetElementType();

		switch (graType)
		{
		case type_Rect:
			{
				SysRectElement* pRect = (SysRectElement *)pElem;

				//glBlendColor(0,1,0,0.2);

				glColor4f(0.0,1.0,0,0.1);
				DrawRectElem(pRect,dispTran);
				glColor4f(0.0,1.0,0,1.0);
			}
			break;

		case type_PolyLine:
			{
				SysLineElement* pLine = (SysLineElement*)pElem;

				//glDisable(GL_BLEND);
				//glColor4f(1,0,0,0.4);
				DrawLineElem(pLine,dispTran);
				glColor3f(1,0,0);

				/*glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
			}
			break;

		case type_Ploygon:
			{
				SysPolygonElement* pPolygon = (SysPolygonElement *)pElem;
				glColor4f(0.0,1.0,0,0.3);
				DrawPolyElem(pPolygon,dispTran);
				//glColor4f(0.0,1.0,0,1.0);
			}
			break;

		case type_Cycle:
			{
				SysCycleElement* pCycle = (SysCycleElement *)pElem;
				glColor4f(0.0,0.0,1.0,0.8);
				DrawCycleElem(pCycle,dispTran);
			}

			break;

		default:
			break;
		}

		return 1;
	}

	void SysGraphLayer::RemoveAllGraphicElement()
	{
		int nCount = m_vecGraphicElement.size();
		std::vector<SysGraphElement *>::iterator iter = m_vecGraphicElement.begin();
		std::vector<SysGraphElement *>::iterator iterEnd = m_vecGraphicElement.end();

		for (; iter != iterEnd; ++iter)
		{
			SysGraphElement* pGra = *iter;
			DestroyGraphElement(pGra);
		}

		m_vecGraphicElement.clear();
	}
}
