/**
* @file              SysDrawEvents.h
* @brief             绘制事件定义
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年10月14日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSDRAWEVENTS_H_72D2E6C1_932A_4377_B9A6_760299318C72__
#define __SYSDRAWEVENTS_H_72D2E6C1_932A_4377_B9A6_760299318C72__

#include "SysDrawCommon.h"

namespace SysDraw
{

	class SysMapLayer;

	SYSDRAW_EXTERNC_START

	/**
	* @brief 图层添加时的回调函数声明
	* @param[in] SysMapLayer*
	* @param[in] void*
	* @return typedefvoid 
	* @author 周旭光
	* @date 2015年10月14日
	* @note 
	*/
	typedef void (SYSDRAW_STD_CALL* OnMapLayerAdd)(SysMapLayer* ,void* );  

	/**
	* @brief 图层删除时的回调函数声明
	* @param[in] SysMapLayer*
	* @param[in] void*
	* @return typedefvoid 
	* @author 周旭光
	* @date 2015年10月14日
	* @note 
	*/
	typedef void (SYSDRAW_STD_CALL* OnMapLayerRemove)(SysMapLayer* ,void* );  

	SYSDRAW_EXTERNC_END

	class SYSDRAW_API SysDrawEvents
	{
	private:
		SysDrawEvents(void);

		SysDrawEvents(const SysDrawEvents& rhs);

		SysDrawEvents& operator =(const SysDrawEvents& rhs);

		~SysDrawEvents(void);

	public:
		static SysDrawEvents* GetInstance();

		/**
		* @brief 注册图层添加事件
		* @param[in] OnMapLayerAdd pEventFunc
		* @return void
		* @author 周旭光
		* @date 2015年10月15日
		* @note 
		*/
		void RegisterMapLayerAddEvent(OnMapLayerAdd pEventFunc);

		/**
		* @brief 注册图层删除事件
		* @param[in] OnMapLayerRemove pEventFunc
		* @return void
		* @author 周旭光
		* @date 2015年10月15日
		* @note 
		*/
		void RegisterMapLayerRemoveEvent(OnMapLayerRemove pEventFunc);

		/**
		* @brief 激活图层添加事件
		* @param[in] SysMapLayer*
		* @param[in] void*
		* @return void
		* @author 周旭光
		* @date 2015年10月15日
		* @note 
		*/
		void ActiveMapLayerAddEvents(SysMapLayer* ,void* ) const;

		/**
		* @brief 激活图层删除事件
		* @param[in] SysMapLayer*
		* @param[in] void*
		* @return void
		* @author 周旭光
		* @date 2015年10月15日
		* @note 
		*/
		void ActiveMapLayerRemoveEvents(SysMapLayer* ,void* ) const;

	private:
		std::vector<OnMapLayerAdd> m_vecLayerAddFuncs;		//图层添加时的回调函数
		std::vector<OnMapLayerRemove> m_vecLayerRemoveFuncs;		//图层删除时的回调函数

	};

}

#endif // end of __SYSDRAWEVENTS_H_72D2E6C1_932A_4377_B9A6_760299318C72__
