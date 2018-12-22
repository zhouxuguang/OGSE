#include "SysMapPanTool.h"

#include <SysDraw/SysEnvelope.h>
#include <SysDraw/SysMap.h>
#include <SysDraw/SysDrawCanvas.h>
#include <SysDraw/SysRenderContext.h>
#include <SysDraw/SysMapManager.h>

#include <GL/glew.h>

#include "SysUtility/SysLog.h"

namespace SysDraw
{

	SysMapPanTool::SysMapPanTool(const std::string& strName)	
		:SysMapTool(strName)
	{
		m_nStartX = 0;
		m_nStartY = 0;
	}

	SysMapPanTool::~SysMapPanTool(void)
	{
	}

	SysMapTool* SysMapPanTool::Clone() const
	{
		SysMapPanTool* pTool = new SysMapPanTool(GetToolName());
		pTool->m_nStartX = m_nStartX;
		pTool->m_nStartY = m_nStartY;

		return pTool;
	}

	void SysMapPanTool::OnMousePress(MOUSE_BUTTON eMouse,int x,int y)
	{
		if (eMouse == Left_Button)
		{
			m_nStartX = x;
			m_nStartY = y;
		}
	}

	void SysMapPanTool::OnMouseRelease(MOUSE_BUTTON eMouse,int x,int y)
	{
		if (eMouse == Left_Button)
		{
			//SysDraw::SysMapManager* pManager = SysDraw::SysMapManager::GetInstance();
			if ( abs(m_nStartX - x) <= 1 || abs(m_nStartY - y) <= 1)
			{
				return;
			}

			//计算鼠标点击和弹起点的地理坐标
			SysDraw::SysDisplayTransform tran = pManager->GetDisplayTran();

			//平移之前的绘制范围
			SysEnvelope VisEnv;
			tran.GetVisibleBounds(VisEnv);

			double dbDownGeoX = 0;
			double dbDownGeoY = 0;
			tran.ToMapPoint(m_nStartX,m_nStartY,dbDownGeoX,dbDownGeoY);

			double dbUpGeoX = 0;
			double dbUpGeoY = 0;
			tran.ToMapPoint(x,y,dbUpGeoX,dbUpGeoY);

			/*int dx = m_nStartX - x;
			int dy = y - m_nStartY;*/

			//计算实际移动的距离
			
			//double dbScale = tran.GetMapUnitsPerPixel();
			double dbOffsetX = dbUpGeoX - dbDownGeoX;
			double dbOffsetY = dbUpGeoY - dbDownGeoY;

			smartlog<<dbOffsetX<<"---"<<dbOffsetY;
			pManager->MoveContent(-dbOffsetX,-dbOffsetY);

			
			//平移之后的矩形范围
			SysEnvelope PanEnv(VisEnv.minX-dbOffsetX,VisEnv.maxX-dbOffsetX,VisEnv.minY-dbOffsetY,VisEnv.maxY-dbOffsetY);

			//pManager->CopyBackToFront();
			pManager->UpDateView();

			//if (!VisEnv.Intersects(PanEnv))	//如果拖出去的范围和原来范围没有重叠，则直接绘图
			//{
			//	pManager->UpDateView();
			//}

			//else
			//{

			//	pManager->SetUseDrawView(true);

			//	SysEnvelope envA;
			//	SysEnvelope envB;

			//	if (dbOffsetX >= 0 && dbOffsetY >= 0)
			//	{
			//		envA = SysEnvelope(PanEnv.minX,PanEnv.minX+PanEnv.GetWidth(),
			//			PanEnv.minY,PanEnv.minY+dbOffsetY);

			//		envB = SysEnvelope(PanEnv.minX,PanEnv.minX+dbOffsetX,
			//			PanEnv.minY+dbOffsetY,PanEnv.maxY);

			//	}

			//	else if (dbOffsetX >= 0 && dbOffsetY <= 0)
			//	{
			//		envA = SysEnvelope(PanEnv.minX,PanEnv.maxX,
			//			PanEnv.maxY-fabs(dbOffsetY),PanEnv.maxY);

			//		envB = SysEnvelope(PanEnv.minX,PanEnv.minX+fabs(dbOffsetX),
			//			PanEnv.minY,PanEnv.maxY-fabs(dbOffsetY));

			//	}

			//	else if (dbOffsetX <= 0 && dbOffsetY <= 0)
			//	{
			//		envA = SysEnvelope(PanEnv.maxX+dbOffsetX,PanEnv.maxX,
			//			PanEnv.minY,PanEnv.maxY);

			//		envB = SysEnvelope(PanEnv.minX,PanEnv.maxX+dbOffsetX,
			//			PanEnv.maxY+dbOffsetY,PanEnv.maxY);
			//		
			//	}

			//	else if (dbOffsetX <= 0 && dbOffsetY >= 0)
			//	{
			//		envA = SysEnvelope(PanEnv.minX,PanEnv.maxX,
			//			PanEnv.minY,PanEnv.minY+fabs(dbOffsetY));

			//		envB = SysEnvelope(PanEnv.maxX-fabs(dbOffsetX),PanEnv.maxX,
			//			PanEnv.minY+fabs(dbOffsetY),PanEnv.maxY);
			//		
			//	}

			//	glEnable(GL_SCISSOR_TEST);

			//	long lViewHeight = tran.GetViewHeight();

			//	RECT rtViewA;
			//	tran.ToScreenRect(envA,rtViewA);
			//	glScissor(rtViewA.left,lViewHeight-rtViewA.bottom,abs(rtViewA.left - rtViewA.right), abs(rtViewA.bottom - rtViewA.top));
			//	pManager->SetDarwView(envA);
			//	pManager->UpDateView();

			//	RECT rtViewB;
			//	tran.ToScreenRect(envB,rtViewB);
			//	glScissor(rtViewB.left,lViewHeight-rtViewB.bottom,abs(rtViewB.left - rtViewB.right), abs(rtViewB.bottom - rtViewB.top));
			//	pManager->SetDarwView(envB);
			//	pManager->UpDateView();

			//	glDisable(GL_SCISSOR_TEST);
			//}


			//int x = event->x();	//窗口坐标
			//int y = event->y();
			//GLdouble dx1,dy1,dz1;
			//ScreenToWorld(x,y,&dx1,&dy1,&dz1);

			//glRasterPos2i(0,0);

			//const GLubyte* errorStr;
			//GLenum error = glGetError();
			//if (error != GL_NO_ERROR)
			//{
			//	errorStr = gluErrorString(error);
			//}

			//unsigned char *poData = new unsigned char[600*400*3];
			//for (int i = 0; i < 600*400*3; i ++)
			//{
			//	poData[i] = i;
			//}
			////glPixelStorei(GL_PACK_ALIGNMENT,1);
			////glReadPixels(0,0,600,400,GL_RGB,GL_UNSIGNED_BYTE,poData);
			////glDrawPixels(600,400,GL_RGB,GL_UNSIGNED_BYTE,poData);

			////GLfloat * pt1 = (GLfloat*)glMapBuffer(GL_PIXEL_PACK_BUFFER,GL_READ_ONLY);

			//delete []poData;

			//GLdouble dbPixel[4];
			//glGetDoublev(GL_CURRENT_RASTER_POSITION,dbPixel);

			//glCopyPixels(300,300,300,200,GL_COLOR);

			//error = glGetError();
			//if (error != GL_NO_ERROR)
			//{
			//	errorStr = gluErrorString(error);
			//}

			//glFlush();

			//updateGL();

			//updateGL();

		}
	}

	void SysMapPanTool::OnMouseMove(MOUSE_BUTTON eMouse,int x,int y)
	{
		if (eMouse == Left_Button)
		{
			//SysDraw::SysMapManager* pManager = SysDraw::SysMapManager::GetInstance();

			int dx = m_nStartX - x;
			int dy = (m_nStartY - y);

			pManager->SetCopyStartPoint(-dx,dy);

			pManager->UpDateView(false);

		}
	}

}
