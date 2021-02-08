/**
* @file              PreCompile.h
* @brief             Ԥ���봦��
* @details           
* @author            zxg 
* @date              2015��5��15��
* @version           1.0.0.1
* @par               Copyright (c):����� 2015
* @par               History:
*/
#ifndef __PRECOMPILE_H_C0C6C4C8_B1C4_49C3_87E4_EEF1ABC862F0__
#define __PRECOMPILE_H_C0C6C4C8_B1C4_49C3_87E4_EEF1ABC862F0__

//Ԥ�Ȱ���һЩͷ�ļ�
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <functional>
#include <algorithm>
#include <numeric>

#if(__cplusplus >= 201103L)
    #include <unordered_map>
    #include <unordered_set>
#elif (_MSC_VER >= 1500)
    #include <unordered_map>
    #include <unordered_set>

	namespace std
	{
		using std::tr1::unordered_map;
		using std::tr1::unordered_set;
		using std::tr1::hash;
	}

#else
	#include <tr1/unordered_map>
	#include <tr1/unordered_set>
	namespace std
	{
		using std::tr1::unordered_map;
		using std::tr1::unordered_set;
		using std::tr1::hash;
	}
#endif

//���������ڲ���ͷ�ļ�
#include "SysConstant.h"

//����utf16��utf32
typedef std::basic_string<eUInt16> utf16String;
typedef std::basic_string<eUInt32> utf32String;

#if defined _WIN32 || defined _WIN64 || defined WINCE
	#define OS_WINDOWS
#elif defined __ANDROID__
    #define OS_ANDROID
#elif defined __linux__
	#define OS_LINUX
#elif (defined(__MACH__) && defined(__APPLE__) )
    #define OS_MACOSX
#elif defined __IOS__
    #define OS_IOS
#endif

//����������ڴ�
#ifdef __GNUC__
    #define DECLARE_ALIGNED(n)   __attribute__((aligned(n)))
#elif defined _MSC_VER
    #define DECLARE_ALIGNED(n) __declspec(align(n))
#endif

typedef eUInt32 DECLARE_ALIGNED(4) AlignedUint32;
typedef eInt32 DECLARE_ALIGNED(4) AlignedInt32;
typedef eUInt64 DECLARE_ALIGNED(8) AlignedUint64;
typedef eInt64 DECLARE_ALIGNED(8) AlignedInt64;



//����������
#if defined _WIN32 || defined __CYGWIN__ || _WIN64
	#ifdef BASELIB_EXPORTS		
		#ifdef __GNUC__
			#define BASELIB_API __attribute__((dllexport))
		#else
			#define BASELIB_API __declspec(dllexport)
		#endif
	#else
		#ifdef __GNUC__
			#define BASELIB_API __attribute__((dllimport))
		#else
			#define BASELIB_API __declspec(dllimport)
		#endif
	#endif
	#define BASELIB_API_HIDE
#else
	#if __GNUC__>=4
		#define BASELIB_API __attribute__((visibility("default")))
		#define BASELIB_API_HIDE __attribute__ ((visibility("hidden")))
	#else
		#define BASELIB_API
		#define BASELIB_API_HIDE
	#endif
#endif


#if defined _WIN32 ||_WIN64 || defined __CYGWIN__

	#define BASELIB_API_CALL __stdcall
#else
	#if __GNUC__>=4
	#define BASELIB_API_CALL __attribute__((__stdcall__))
	#endif
#endif

//C���ӷ�ʽ�궨��
#ifdef __cplusplus
	#define EXTERN_C_START           extern "C" {
	#define EXTERN_C_END             }
#else
	#define EXTERN_C_START
	#define EXTERN_C_END
#endif

//ǿ�������ĺ궨�壬����һ���ܳɹ�
#if defined _WIN32 ||_WIN64 || defined __CYGWIN__

	#define FORCE_INLINE __forceinline
#else
	#define FORCE_INLINE __attribute__((always_inline) )
#endif

//�������ܳ��쳣��new �����
//#define new new(std::nothrow)

//�����ռ�����
#ifdef __cplusplus
    #define NS_BASELIB_BEGIN                     namespace baselib {
    #define NS_BASELIB_END                       }
    #define USING_NS_BASELIB                     using namespace baselib;
#else
    #define NS_BASELIB_BEGIN
    #define NS_BASELIB_END
    #define USING_NS_BASELIB
#endif



#endif // end of file_
