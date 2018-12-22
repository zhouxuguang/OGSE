/**
* @file              SysLineSymbol.h
* @brief             线状符号类
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年11月27日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSLINESYMBOL_H_F8F3CF0F_150A_417F_8729_F206BCCCFA87__
#define __SYSLINESYMBOL_H_F8F3CF0F_150A_417F_8729_F206BCCCFA87__

#include "SysSymbol.h"

namespace SysDraw
{

	class SYSDRAW_API SysLineSymbol : public SysSymbol
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
		SysLineSymbol(void);

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
		~SysLineSymbol(void);

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

#endif // end of __SYSLINESYMBOL_H_F8F3CF0F_150A_417F_8729_F206BCCCFA87__
