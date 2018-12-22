/**
* @file              SysPointSymbol.h
* @brief             点状符号类
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年11月27日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSPOINTSYMBOL_H_F4D34DAD_2548_44E5_81D9_35F54D82ABC0__
#define __SYSPOINTSYMBOL_H_F4D34DAD_2548_44E5_81D9_35F54D82ABC0__

#include "SysSymbol.h"

namespace SysDraw
{

	class SYSDRAW_API SysPointSymbol : public SysSymbol
	{
	public:
		/**
		* @brief 构造函数
		* @return 
		* @author 周旭光
		* @date 2015年11月27日
		* @par Sample code
		* @code @endcode
		* @par Output
		* @see 
		* @note 
		*/
		SysPointSymbol(void);

		/**
		* @brief 析构函数
		* @return 
		* @author 周旭光
		* @date 2015年11月27日
		* @par Sample code
		* @code @endcode
		* @par Output
		* @see 
		* @note 
		*/
		~SysPointSymbol(void);

		/**
		* @brief 获得符号类型
		* @return SysSymbolType
		* @author 周旭光
		* @date 2015年11月27日
		* @par Sample code
		* @code @endcode
		* @par Output
		* @see 
		* @note 
		*/
		SysSymbolType GetSymbolType() const;
	};

}

#endif // end of __SYSPOINTSYMBOL_H_F4D34DAD_2548_44E5_81D9_35F54D82ABC0__
