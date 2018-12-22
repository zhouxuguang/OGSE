#include "SysMapWheelTool.h"

#include <SysDraw/SysRenderContext.h>
#include <SysDraw/SysMapManager.h>


namespace SysDraw
{

	SysMapWheelTool::SysMapWheelTool(const std::string &strName)
		:SysMapTool(strName)
	{
	}

	SysMapWheelTool::~SysMapWheelTool(void)
	{
	}

	void SysMapWheelTool::OnMouseWheel(MOUSE_BUTTON eMouse, int nDelta,int x,int y)
	{
		SysDraw::SysMapManager* pMapManager = SysDraw::SysMapManager::GetInstance();
		if (eMouse == Left_Button)
		{
			SysDraw::SysDisplayTransform tran = pMapManager->GetDisplayTran();
			double dbGeoX,dbGeoY;
			tran.ToMapPoint(x,y,dbGeoX,dbGeoY);
			pMapManager->ZoomContent(nDelta,dbGeoX,dbGeoY);
			pMapManager->UpDateView(true,true,false);
		}
	}

}
