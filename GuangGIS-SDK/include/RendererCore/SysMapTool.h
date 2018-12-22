/**
* @file              SysMapTool.h
* @brief             地图交互工具基类
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年8月3日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSMAPTOOL_H_22E5CC84_43C3_4460_958C_014608531360__
#define __SYSMAPTOOL_H_22E5CC84_43C3_4460_958C_014608531360__

//鼠标相关消息
enum MOUSE_BUTTON
{
	Left_Button,
	Right_Button,
	UnDefine_Button
};

//相关的虚拟键
enum VIRTUAL_KEY
{    
	SYS_KEY_DELETE,
	SYS_KEY_UP,
	SYS_KEY_DOWN
};

#include "SysDrawCommon.h"

/**
* @namespace       SysDraw
* @brief		   
* @details     
* @author    周旭光
* @date      2015年8月3日
* @version    3.0
* @par Copyright (c): 
*         周旭光
* @par History:         
*    version: author, date, desc
*/
namespace SysDraw
{

	class SYSDRAW_API SysMapTool
	{
	public:
		explicit SysMapTool(const std::string& strName);

		virtual ~SysMapTool(void);

		virtual SysMapTool* Clone() const;

		/**
		* @brief GetToolName
		* @return std::string
		* @author 周旭光
		* @date 2015年8月22日
		* @note 
		*/
		std::string GetToolName() const;

		/**
		* @brief OnMousePress
		* @param[in] MOUSE_BUTTON eMouse
		* @param[in] int x
		* @param[in] int y
		* @return virtualvoid 
		* @author 周旭光
		* @date 2015年8月22日
		* @note 
		*/
		virtual void OnMousePress(MOUSE_BUTTON eMouse,int x,int y);

		/**
		* @brief OnMouseRelease
		* @param[in] MOUSE_BUTTON eMouse
		* @param[in] int x
		* @param[in] int y
		* @return void
		* @author 周旭光
		* @date 2015年8月22日
		* @note 
		*/
		virtual void OnMouseRelease(MOUSE_BUTTON eMouse,int x,int y);

		/**
		* @brief OnMouseMove
		* @param[in] MOUSE_BUTTON eMouse
		* @param[in] int x
		* @param[in] int y
		* @return 
		* @author 周旭光
		* @date 2015年8月22日
		* @note 
		*/
		virtual void OnMouseMove(MOUSE_BUTTON eMouse,int x,int y);

		/**
		* @brief OnMouseWheel
		* @param[in] MOUSE_BUTTON eMouse
		* @param[in] int nDelta
		* @param[in] int x
		* @param[in] int y
		* @return void
		* @author 周旭光
		* @date 2015年8月22日
		* @note 
		*/
		virtual void OnMouseWheel(MOUSE_BUTTON eMouse, int nDelta,int x,int y);

		/**
		* @brief OnMouseDoubleClick
		* @param[in] MOUSE_BUTTON eMouse
		* @param[in] int x
		* @param[in] int y
		* @return void
		* @author 周旭光
		* @date 2015年8月22日
		* @note 
		*/
		virtual void OnMouseDoubleClick(MOUSE_BUTTON eMouse,int x,int y);

		/**
		* @brief OnKeyPress
		* @param[in] int nChar
		* @param[in] int nRepCnt
		* @param[in] int nFlags
		* @return void
		* @author 周旭光
		* @date 2015年8月22日
		* @note 
		*/
		virtual void OnKeyPress(int nChar, int nRepCnt, int nFlags);

		/**
		* @brief OnKeyRelease
		* @param[in] int nChar
		* @param[in] int nRepCnt
		* @param[in] int nFlags
		* @return void
		* @author 周旭光
		* @date 2015年8月22日
		* @note 
		*/
		virtual void OnKeyRelease(int nChar, int nRepCnt, int nFlags);

		/**
		* @brief /系统内置的创建和销毁函数
		* @param[in] const std::string & strName
		* @return SysMapTool*
		* @author 周旭光
		* @date 2015年8月22日
		* @note 
		*/
		static SysMapTool* CreateMapTool(const std::string & strName);

		/**
		* @brief DestroyTool
		* @param[in] SysMapTool* pTool
		* @return void
		* @author 周旭光
		* @date 2015年8月22日
		* @note 
		*/
		static void DestroyTool(SysMapTool* pTool);

	private:
		std::string m_strName;
	};

}


#endif // end of __SYSMAPTOOL_H_22E5CC84_43C3_4460_958C_014608531360__
