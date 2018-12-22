/**
* @file              SysDrawCommon.h
* @brief             绘图基本定义
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年6月25日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSDRAWCOMMON_H_64A743A3_999B_4FB6_8C77_8816357D3751__
#define __SYSDRAWCOMMON_H_64A743A3_999B_4FB6_8C77_8816357D3751__


//C
#include <assert.h>
#include <stdio.h>

#if !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>

//STL
#include <algorithm>
#include <vector>
#include <string>
#include <list>
#include <map>

using namespace std;

#if defined _WIN32 || defined _WIN64
#include <windows.h>
#elif defined linux
#include <unistd.h>
#endif

#include "PreCompile.h"
#include "Geometry_Priv.h"

#if defined _WIN32 || defined _WIN64
	#ifdef RENDERERCORE_EXPORTS
		#define SYSDRAW_API __declspec(dllexport)
	#else
		#define SYSDRAW_API __declspec(dllimport)
#endif

#elif defined __linux__
#if __GNUC__ >= 4
#define SYSDRAW_API __attribute__((visibility("default")))
#else 
#define SYSDRAW_API
#endif
#endif

//标准调用定义
#if defined _WIN32 ||_WIN64 || defined __CYGWIN__
#define SYSDRAW_STD_CALL __stdcall
#else
/*#if __GNUC__>=4
#define SYSDRAW_STD_CALL __attribute__((__stdcall__))
#else
#define SYSDRAW_STD_CALL
#endif*/
#define SYSDRAW_STD_CALL
#endif

//C语言声明
#ifdef  __cplusplus
#define SYSDRAW_EXTERNC_START extern "C" {
#define SYSDRAW_EXTERNC_END }
#else
#define SYSDRAW_EXTERNC_START extern
#define SYSDRAW_EXTERNC_END
#endif

//变量内存对齐
#ifndef ATTR_ALIGN
#if defined(__GNUC__) // GCC
#define ATTR_ALIGN(n)   __attribute__((aligned(n))) 
#else
#define ATTR_ALIGN(n)   __declspec(align(n))
#endif
#endif





SYSDRAW_EXTERNC_START

//回调函数定义
typedef int (SYSDRAW_STD_CALL *DrawProgressFunc)(double dfComplete, const char *pszMessage, void *pProgressArg);

SYSDRAW_EXTERNC_END


//定义栅格数据存储类型
typedef enum {
				RAS_Unknown = 0,
				RAS_Byte = 1,
				RAS_UInt16 = 2,
				RAS_Int16 = 3,
				RAS_UInt32 = 4,
				RAS_Int32 = 5,
				RAS_Float32 = 6,
				RAS_Float64 = 7,
				RAS_CInt16 = 8,
				RAS_CInt32 = 9,
				RAS_CFloat32 = 10,
				RAS_CFloat64 = 11,
				RAS_TypeCount = 12   
} SysRasDataType;




#endif // end of __SYSDRAWCOMMON_H_64A743A3_999B_4FB6_8C77_8816357D3751__
