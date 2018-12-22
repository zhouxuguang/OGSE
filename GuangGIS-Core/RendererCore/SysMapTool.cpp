
//#include <assert.h>
#include "SysMapTool.h"

#include "SysMapZoomInTool.h"
#include "SysMapPanTool.h"
#include "SysMapWheelTool.h"
#include "SysMapZoomOutTool.h"

namespace SysDraw
{


	SysMapTool::SysMapTool(const std::string& strName)
	{
		m_strName = strName;
	}

	SysMapTool::~SysMapTool(void)
	{
	}

	SysMapTool* SysMapTool::Clone() const
	{
		SysMapTool *pTool = new SysMapTool(m_strName);
		return pTool;
	}

	/*void SysMapTool::Destroy(SysMapTool* pTool)
	{
		delete pTool;
		pTool = NULL;
	}*/

	std::string SysMapTool::GetToolName() const
	{
		return m_strName;
	}

	void SysMapTool::OnMousePress(MOUSE_BUTTON eMouse,int x,int y)
	{
		assert(true);
		return;
		/*int x1 = x;
		int y1 = y;*/

	}

	void SysMapTool::OnMouseRelease(MOUSE_BUTTON eMouse,int x,int y)
	{
		assert(true);
	}

	void SysMapTool::OnMouseMove(MOUSE_BUTTON eMouse,int x,int y)
	{
		assert(true);
	}

	void SysMapTool::OnMouseWheel(MOUSE_BUTTON eMouse, int nDelta,int x,int y)
	{
		assert(true);
	}

	void SysMapTool::OnMouseDoubleClick(MOUSE_BUTTON eMouse,int x,int y)
	{
		assert(true);
	}

	void SysMapTool::OnKeyPress(int nChar, int nRepCnt, int nFlags)
	{
		assert(true);
	}

	void SysMapTool::OnKeyRelease(int nChar, int nRepCnt, int nFlags)
	{
		assert(true);
	}

	SysMapTool* SysMapTool::CreateMapTool( const std::string & strName )
	{
		if (("ZoomInTool") == strName)
		{
			return new(std::nothrow) SysMapZoomInTool(strName);
		}

		else if (("ZoomOutTool") == strName)
		{
			return new(std::nothrow) SysMapZoomOutTool(strName);
		}

		else if (("PanTool") == strName)
		{
			return new(std::nothrow) SysMapPanTool(strName);
		}

		else if (("WheelTool") == strName)
		{
			return new(std::nothrow) SysMapWheelTool(strName);
		}

		return NULL;
	}

	void SysMapTool::DestroyTool( SysMapTool* pTool )
	{
		delete pTool;
		pTool = NULL;
	}

	//创建工具的代码
	/*SysMapTool* CreateSysMapTool(SYS_MAP_TOOL eType)
	{
		if (SYS_ZOOM_IN_TOOL == eType)
		{
			return new(std::nothrow) SysMapZoomInTool;
		}

		else if (SYS_ZOOM_OUT_TOOL == eType)
		{
			return new(std::nothrow) SysMapZoomOutTool;
		}

		else if (SYS_PAN_TOOL == eType)
		{
			return new(std::nothrow) SysMapPanTool("");
		}

		else if (SYS_WHEEL_ZOOM_TOOL == eType)
		{
			return new(std::nothrow) SysMapWheelTool;
		}

		return NULL;
	}

	void DestroySysMapTool(SysMapTool* pTool)
	{
		delete pTool;
		pTool = NULL;
	}*/

}
