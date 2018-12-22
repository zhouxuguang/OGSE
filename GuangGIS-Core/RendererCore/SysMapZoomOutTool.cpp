#include "SysMapZoomOutTool.h"

#include <RendererCore/SysEnvelope.h>
#include <RendererCore/SysMap.h>
#include <RendererCore/SysDrawCanvas.h>
#include <RendererCore/SysRenderContext.h>
#include <RendererCore/SysMapManager.h>
#include <RendererCore/SysImagePreProcess.h>
#include <RendererCore/SysGraphLayer.h>
#include <RendererCore/SysRectElement.h>

#include <GL/glew.h>

namespace SysDraw
{

	SysMapZoomOutTool::SysMapZoomOutTool(const std::string &strName):SysMapTool(strName)
	{
	}

	SysMapZoomOutTool::~SysMapZoomOutTool(void)
	{
	}

	SysMapTool* SysMapZoomOutTool::Clone() const
	{
		SysMapZoomOutTool* pTool = new SysMapZoomOutTool(GetToolName());
		pTool->m_nStartX = m_nStartX;
		pTool->m_nStartY = m_nStartY;

		return pTool;
	}

	void SysMapZoomOutTool::OnMousePress(MOUSE_BUTTON eMouse,int x,int y)
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

	void SysMapZoomOutTool::OnMouseRelease(MOUSE_BUTTON eMouse,int x,int y)
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
			SysEnvelope envVis;
			tran.GetVisibleBounds(envVis);

			double dbGeoX1,dbGeoY1;
			tran.ToMapPoint(m_nStartX,m_nStartY,dbGeoX1,dbGeoY1);

			double dbGeoX2,dbGeoY2;
			tran.ToMapPoint(x,y,dbGeoX2,dbGeoY2);

			SysEnvelope envNew;
			envNew.Init(dbGeoX1,dbGeoX2,dbGeoY1,dbGeoY2);
			double dbCenterX = 0;
			double dbCenterY = 0;
			envNew.Center(dbCenterX,dbCenterY);
			pManager->ZoomOutContent(envVis,dbCenterX,dbCenterY);

			SysGraphLayer* pGraLayer = pManager->GetCurrentMap()->GetBrowseLayer();
			pGraLayer->RemoveGraphicElement(0);

			pManager->UpDateView();

		}
	}

	void SysMapZoomOutTool::OnMouseMove(MOUSE_BUTTON eMouse,int x,int y)
	{
		if (eMouse == Left_Button)
		{
			//SysDraw::SysMapManager* pManager = SysDraw::SysMapManager::GetInstance();
			SysDraw::SysDisplayTransform tran = pManager->GetDisplayTran();

			tran.ToMapPoint(x,y,m_dbGeoX2,m_dbGeoY2);

			SysGraphLayer* pGraLayer = pManager->GetCurrentMap()->GetBrowseLayer();
			SysRectElement* pElem = (SysRectElement*)pGraLayer->GetGraphicElement(0);
			pElem->Init(m_dbGeoX1,m_dbGeoX2,m_dbGeoY1,m_dbGeoY2);

			pManager->UpDateView(false);
		}
	}

}
