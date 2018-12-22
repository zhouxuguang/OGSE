#include "SysMapZoomInTool.h"

#include <SysDraw/SysEnvelope.h>
#include <SysDraw/SysMap.h>
#include <SysDraw/SysDrawCanvas.h>
#include <SysDraw/SysRenderContext.h>
#include <SysDraw/SysMapManager.h>
#include <SysDraw/SysImagePreProcess.h>
#include <SysDraw/SysMap.h>
#include <SysDraw/SysGraphLayer.h>
#include <SysDraw/SysRectElement.h>

namespace SysDraw
{

	SysMapZoomInTool::SysMapZoomInTool(const std::string &strName):SysMapTool(strName)
	{
	}

	SysMapZoomInTool::~SysMapZoomInTool(void)
	{
	}

	SysMapTool* SysMapZoomInTool::Clone() const
	{
		SysMapZoomInTool* pTool = new SysMapZoomInTool(GetToolName());
		pTool->m_nStartX = m_nStartX;
		pTool->m_nStartY = m_nStartY;

		pTool->m_dbGeoX1 = m_dbGeoX1;
		pTool->m_dbGeoX2 = m_dbGeoX2;
		pTool->m_dbGeoY1 = m_dbGeoY1;
		pTool->m_dbGeoY2 = m_dbGeoY2;

		return pTool;
	}

	void SysMapZoomInTool::OnMousePress(MOUSE_BUTTON eMouse,int x,int y)
	{
		if (eMouse == Left_Button)
		{
			m_nStartX = x;
			m_nStartY = y;

			//SysDraw::SysMapManager* pManager = SysDraw::SysMapManager::GetInstance();
			SysDraw::SysDisplayTransform tran = pManager->GetDisplayTran();

			tran.ToMapPoint(m_nStartX,m_nStartY,m_dbGeoX1,m_dbGeoY1);

			SysGraphLayer* pGraLayer = pManager->GetCurrentMap()->GetBrowseLayer();
			SysRectElement* pElem = (SysRectElement*)CreateGraphElement(type_Rect);
			pElem->Init(m_dbGeoX1,m_dbGeoX1,m_dbGeoY1,m_dbGeoY1);

			pGraLayer->AddGraphicElement(pElem);
		}
	}

	void SysMapZoomInTool::OnMouseRelease(MOUSE_BUTTON eMouse,int x,int y)
	{
		if (eMouse == Left_Button)
		{
			//SysDraw::SysMapManager* pManager = SysDraw::SysMapManager::GetInstance();
			if ( abs(m_nStartX - x) <= 1 || abs(m_nStartY - y) <= 1)
			{
				return;
			}

			//SysDraw::SysMapManager* pManager = SysDraw::SysMapManager::GetInstance();
			SysDraw::SysDisplayTransform tran = pManager->GetDisplayTran();

			double dbGeoX1,dbGeoY1;
			tran.ToMapPoint(m_nStartX,m_nStartY,dbGeoX1,dbGeoY1);

			double dbGeoX2,dbGeoY2;
			tran.ToMapPoint(x,y,dbGeoX2,dbGeoY2);

			SysEnvelope envNew;
			envNew.Init(dbGeoX1,dbGeoX2,dbGeoY1,dbGeoY2);
			pManager->ZoomInContent(envNew);

			SysGraphLayer* pGraLayer = pManager->GetCurrentMap()->GetBrowseLayer();
			pGraLayer->RemoveGraphicElement(0);

			pManager->UpDateView();

		}
	}

	void SysMapZoomInTool::OnMouseMove(MOUSE_BUTTON eMouse,int x,int y)
	{
		if (eMouse == Left_Button)
		{
			/*double x1,y1,z1;
			double x2,y2,z2;
			ScreenToWorld(m_nStartX,m_nStartY,&x1,&y1,&z1);
			ScreenToWorld(x,y,&x2,&y2,&z2);

			glColor4f(0,1,0,0.3);
			glRectd(x1,y1,x2,y2);
			glFlush();*/

			//SysDraw::SysMapManager* pManager = SysDraw::SysMapManager::GetInstance();
			SysDraw::SysDisplayTransform tran = pManager->GetDisplayTran();

			//double dbGeoX1,dbGeoY1;
			tran.ToMapPoint(x,y,m_dbGeoX2,m_dbGeoY2);

			SysGraphLayer* pGraLayer = pManager->GetCurrentMap()->GetBrowseLayer();
			SysRectElement* pElem = (SysRectElement*)pGraLayer->GetGraphicElement(0);
			pElem->Init(m_dbGeoX1,m_dbGeoX2,m_dbGeoY1,m_dbGeoY2);

			pManager->UpDateView(false);
			//pManager->StartDraw();
		}
	}

}
