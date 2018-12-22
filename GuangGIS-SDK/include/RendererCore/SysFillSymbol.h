/**
* @file              SysFillSymbol.h
* @brief             填充符号类
* @details           
* @author            周旭光(zhouxuguang@aeroimginfo.com)
* @date              2015年11月27日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSFILLSYMBOL_H_7324BA8C_F436_42F2_B87D_012518A65717__
#define __SYSFILLSYMBOL_H_7324BA8C_F436_42F2_B87D_012518A65717__

#include "SysSymbol.h"

namespace SysDraw
{

	class SYSDRAW_API SysFillSymbol : public SysSymbol
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
		SysFillSymbol(void);

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
		~SysFillSymbol(void);

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

#endif // end of __SYSFILLSYMBOL_H_7324BA8C_F436_42F2_B87D_012518A65717__
