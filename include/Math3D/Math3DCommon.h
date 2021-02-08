#ifndef MATH32_COMMON_INCLUDE_45
#define MATH32_COMMON_INCLUDE_45


//C header
#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <float.h>

#define _USE_MATH_DEFINES
#include <math.h>

//STL
#include <algorithm>
#include <vector>
#include <string>
#include <list>
#include <complex>

//ƽ̨��ʾ����
#if defined WIN32 || defined _WIN64
	#define M3_PLATFORM_WINDOWS 1
	#define M3_TARGET_PLATFORM M3_PLATFORM_WINDOWS

#elif defined __ANDROID__
	#define M3_PLATFORM_ANDROID 2
	#define M3_TARGET_PLATFORM M3_PLATFORM_ANDROID

#elif defined __IOS__
	#define M3_PLATFORM_IOS 3
	#define M3_TARGET_PLATFORM M3_PLATFORM_IOS

#elif defined __linux__
	#define M3_PLATFORM_LINUX 4
	#define M3_TARGET_PLATFORM M3_PLATFORM_LINUX

#elif defined(__MACH__) && defined(__APPLE__)
    #define M3_PLATFORM_MACOSX 5
    #define M3_TARGET_PLATFORM M3_PLATFORM_MACOSX

#endif



//����������
#if defined _WIN32 || defined __CYGWIN__
	#ifdef MATH3D_EXPORTS		
		#ifdef __GNUC__
			#define MATH3D_API __attribute__((dllexport))
	#else
		#define MATH3D_API __declspec(dllexport)
	#endif
	#else
#ifdef __GNUC__
	#define MATH3D_API __attribute__((dllimport))
#else
	#define MATH3D_API __declspec(dllimport)
#endif
#endif
#define MATH3D_HIDE
#else
	#if __GNUC__>=4
		#define MATH3D_API __attribute__ ((visibility("default")))
		#define MATH3D_HIDE __attribute__ ((visibility("hidden")))
	#else
		#define MATH3D_API
		#define MATH3D_HIDE
	#endif
#endif

//���ȺͽǶ�֮�以��ת���ĳ���
#define RADTODEG	57.29577951308232
#define DEGTORAD	0.0174532925199432958

//ɾ���ڴ�ĺ�
#define FREEPTR(x)  if(x != NULL) \
                    {               \
                        free(x);    \
                        x = NULL;   \
                    }

#define DELETEPTR(x) if(x != NULL) \
                     {               \
                         delete x;    \
                         x = NULL;   \
                     }

#define DELETEARRAY(x)  if(x != NULL) \
                        {               \
                            delete []x;    \
                            x = NULL;   \
                        }

#ifdef USE_DOUBLE
    typedef double Real;
#else
    typedef float Real;
#endif

//�����ռ�����
#ifdef __cplusplus
    #define NS_MATH3D_BEGIN                     namespace math3d {
    #define NS_MATH3D_END                       }
    #define USING_NS_MATH3D                     using namespace math3d;
#else
    #define NS_MATH3D_BEGIN
    #define NS_MATH3D_END
    #define USING_NS_MATH3D
#endif


#endif
