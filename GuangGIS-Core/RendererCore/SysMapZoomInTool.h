/**
* @file              SysMapZoomInTool.h
* @brief             地图拉框放大交互工具
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年8月4日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSMAPZOOMINTOOL_H_BD13A801_D1B1_4BE4_8430_DBDD55DCDF87__
#define __SYSMAPZOOMINTOOL_H_BD13A801_D1B1_4BE4_8430_DBDD55DCDF87__

#include "SysMapTool.h"

namespace SysDraw
{

	class SysMapZoomInTool : public SysMapTool
	{
	public:
		explicit SysMapZoomInTool(const std::string &strName);

		~SysMapZoomInTool(void);

		virtual SysMapTool* Clone() const;

		virtual void OnMousePress(MOUSE_BUTTON eMouse,int x,int y);

		virtual void OnMouseRelease(MOUSE_BUTTON eMouse,int x,int y);

		virtual void OnMouseMove(MOUSE_BUTTON eMouse,int x,int y);

	private:
		int m_nStartX;
		int m_nStartY;

		double m_dbGeoX1;
		double m_dbGeoY1;

		double m_dbGeoX2;
		double m_dbGeoY2; 
	};

}

#endif // end of __SYSMAPZOOMINTOOL_H_BD13A801_D1B1_4BE4_8430_DBDD55DCDF87__
