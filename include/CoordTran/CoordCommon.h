/*******************************************************************************
* 版权所有(C) pyhcx 2013
* 文件名称	: CoordCommon.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang@huadimap.com)
* 设计日期	: 2013年9月28日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __COORDCOMMON_H_BFA560CA_2C80_48F8_9C36_6514A4B44046__
#define __COORDCOMMON_H_BFA560CA_2C80_48F8_9C36_6514A4B44046__

#if defined (COORDTRAN_EXPORTS)

#define COORDTRAN_API	__declspec(dllexport)
#else

//#define COORDTRAN_API	__declspec(dllimport) 
#define COORDTRAN_API

#endif

#include <assert.h>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#include <math.h>

#endif

typedef struct GeoAngle
{
	int du;
}GeoAngle;

/************************************************************************/
/* 以下定义一些常量，加快计算                                           */
/************************************************************************/

/*#define M_E        2.71828182845904523536
#define M_LOG2E    1.44269504088896340736
#define M_LOG10E   0.434294481903251827651
#define M_LN2      0.693147180559945309417
#define M_LN10     2.30258509299404568402
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616
#define M_1_PI     0.318309886183790671538
#define M_2_PI     0.636619772367581343076
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2    1.41421356237309504880
#define M_SQRT1_2  0.707106781186547524401*/

#define TWO_PI     (2.0 * M_PI)

//定义弧度和角度之间转换的常数
#define RAD_TO_DEG  57.29577951308232
#define DEG_TO_RAD	0.0174532925199432958

enum CoordUnitType
{
	COORD_UNIT_UNKNOWN    = 0,
	COORD_METERS          = 1,
	COORD_FEET            = 2,
	COORD_DEGREES         = 4,
	COORD_RADIANS         = 5,
	COORD_NAUTICAL_MILES  = 6,
	COORD_SECONDS         = 7,
	COORD_MINUTES         = 8,
	COORD_KILOMETERS      = 16
};


#endif // end of __COORDCOMMON_H_BFA560CA_2C80_48F8_9C36_6514A4B44046__
