/**
* @file              SysMapToolFactory.h
* @brief             地图交互工具工厂
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年8月2日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSMAPTOOLFACTORY_H_F050F55C_36CF_49BD_B40C_6023A9984CA6__
#define __SYSMAPTOOLFACTORY_H_F050F55C_36CF_49BD_B40C_6023A9984CA6__

#include "SysDrawCommon.h"

namespace SysDraw
{

	class SysMapTool;

	class SYSDRAW_API SysMapToolFactory
	{
	public:
		typedef SysMapTool* (*CreateMapToolFun)(const std::string& );			//创建对象回调函数

		typedef void (*DestroyMapToolFun)(SysMapTool *);			//释放对象的回调函数

		/**
		* @brief 注册地图交互工具
		* @param[in] const std::string& strName
		* @param[in] CreateMapToolFun pCallBackFun
		* @param[in] DestroyMapToolFun pDestroyFun
		* @return void
		* @author 周旭光
		* @date 2015年8月2日
		* @note 
		*/
		static void RegisterMapTool(
			const std::string& strName,
			CreateMapToolFun pCallBackFun,
			DestroyMapToolFun pDestroyFun);

		/**
		* @brief 反注册地图交互工具
		* @param[in] const std::string& strName 工具的名字
		* @return void
		* @author 周旭光
		* @date 2015年8月2日
		* @note 
		*/
		static void UnRegisterMapTool(const std::string& strName);

		/**
		* @brief 创建唯一实例
		* @return SysMapToolFactory* 
		* @author 周旭光
		* @date 2015年8月2日
		* @note 
		*/
		static SysMapToolFactory* GetInstance();

		/**
		* @brief 创建地图交互工具
		* @param[in] const std::string& strName
		* @return SysMapTool*
		* @author 周旭光
		* @date 2015年8月2日
		* @note 
		*/
		static SysMapTool* CreateMapTool(const std::string& strName);

		/**
		* @brief 销毁地图交互工具
		* @param[in] const std::string& strName
		* @param[in] SysMapTool* pTool
		* @return void
		* @author 周旭光
		* @date 2015年8月2日
		* @note 
		*/
		static void DestroyMapTool(const std::string& strName,SysMapTool* pTool);

	private:
		SysMapToolFactory(void);
		~SysMapToolFactory(void);
		SysMapToolFactory(const SysMapToolFactory& rhs);
		SysMapToolFactory& operator=(const SysMapToolFactory& rhs);

		typedef std::map<std::string,CreateMapToolFun> CreateCallBackMap;
		typedef std::map<std::string,DestroyMapToolFun> DestroyCallBackMap;
		static CreateCallBackMap m_mapCreateMap;						//回调函数map
		static DestroyCallBackMap m_mapDestroyMap;
	};

}

#endif // end of __SYSMAPTOOLFACTORY_H_F050F55C_36CF_49BD_B40C_6023A9984CA6__
