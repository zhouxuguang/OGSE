/**
* @file              SysCoordinate.h
* @brief             二维地理坐标类
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2012年1月3日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2012
* @par               History:
*/
#ifndef __SYSCOORDINATE_H_64B3BC5B_4C4E_4F4F_AD1D_54E8D3DC7712__
#define __SYSCOORDINATE_H_64B3BC5B_4C4E_4F4F_AD1D_54E8D3DC7712__


#include "SysDrawCommon.h"

namespace SysDraw
{

	class SYSDRAW_API SysCoordinate
	{
	public:
		//默认构造函数
		SysCoordinate(void);

		//带参数的构造函数
		SysCoordinate(double x,double y);

		//拷贝构造函数
		SysCoordinate(const SysCoordinate& coord);

		//析构函数
		~SysCoordinate(void);

		//计算到另一个点得距离
		double Distance(const SysCoordinate &coord) const;

		//设置X和Y坐标
		void SetX(double x);
		void SetY(double y);

	public:
		double x;	//横坐标
		double y;	//纵坐标
		//double z;	//z坐标
	};

}

#endif // end of __GEOCOORDINATE_H_FFF7BBFB_E6D0_439F_A6CE_561AD1EE0958__
