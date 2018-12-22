/*******************************************************************************
* 版权所有(C) pyhcx 2012
* 文件名称	: CGCommon.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年10月21日
* 内容摘要	:
* 修改记录	:
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __CGCOMMON_H_1DFB73A5_0295_4682_9A13_FAC2D641BB22__
#define __CGCOMMON_H_1DFB73A5_0295_4682_9A13_FAC2D641BB22__

#include <math.h>
#include <memory.h>
#include <assert.h>

#include <algorithm>
#include <vector>
using namespace std;


#if defined _WIN32 || defined __CYGWIN__
	#ifdef COMPUTEGEOMETRY_EXPORTS		
		#ifdef __GNUC__
			#define COMPUTEGEOMETRY_API __attribute__((dllexport))
		#else
			#define COMPUTEGEOMETRY_API __declspec(dllexport)
		#endif
	#else
	#ifdef __GNUC__
		#define COMPUTEGEOMETRY_API __attribute__((dllimport))
		#else
		#define COMPUTEGEOMETRY_API __declspec(dllimport)
		#endif
	#endif
	#define COMPUTEGEOMETRY_HIDE
#else
	#if __GNUC__>=4
		#define COMPUTEGEOMETRY_API __attribute__ ((visibility("default")))
		#define COMPUTEGEOMETRY_HIDE __attribute__ ((visibility("hidden")))
	#else
		#define COMPUTEGEOMETRY_API
		#define COMPUTEGEOMETRY_HIDE
	#endif
#endif



#ifdef  __cplusplus
extern "C" {
#endif


//数学常用常量

	#if !defined(_USE_MATH_DEFINES) || defined(_MATH_DEFINES_DEFINED)

		#define M_E        2.71828182845904523536
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
		#define M_SQRT1_2  0.707106781186547524401

	#endif

#define	EPS		   1E-10

//点结构体
typedef struct CG_Point
{
	double x;
	double y;

	CG_Point(double dbX = 0,double dbY = 0)
	{
		x = dbX;
		y = dbY;
	}
}SPOINT, *PSPOINT;

//线段
typedef struct CG_Segment
{
	SPOINT start;
	SPOINT end;

	CG_Segment(SPOINT a, SPOINT b) 
	{ 
		start = a; 
		end = b;
	} 
	CG_Segment() 
	{ } 
}SEGMENT, *PSEGMENT;


#ifdef  __cplusplus
}
#endif


#endif // end of __CGCOMMON_H_1DFB73A5_0295_4682_9A13_FAC2D641BB22__
