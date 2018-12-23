#ifndef MATH3D_GLOBALAPI_INCLUDE
#define MATH3D_GLOBALAPI_INCLUDE

#include "Math3DCommon.h"

NS_MATH3D_BEGIN


class Matrix4x4;

#ifdef __cplusplus
	extern "C" {
#else
	extern {
#endif


//设置模型视图矩阵
MATH3D_API void cgSetViewModelMatrix(const Matrix4x4& matViewModel);

//设置模型视图矩阵
MATH3D_API void cgSetProjectMatrix(const Matrix4x4& matProject);

//视口变换
MATH3D_API void cgViewPort(int x,int y,int nWidth,int nHeight);

//引擎初始化
MATH3D_API void cgInit();

//设置绘图的句柄
MATH3D_API void cgSetRenderContext(void* pHandle);

//刷新视图
MATH3D_API void cgFinish();

MATH3D_API void cgLine(double x1,double y1,double x2,double y2);

MATH3D_API void cgLine3(double x1,double y1,double z1,double x2,double y2,double z2);

MATH3D_API void cgTriangle2(double x1,double y1,double x2,double y2,double x3,double y3);

#ifdef __cplusplus
	}
#else
	} 
#endif


NS_MATH3D_END

#endif


