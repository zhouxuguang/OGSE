#ifndef MODELTRANSFORM_INCLUDE
#define MODELTRANSFORM_INCLUDE

#include "Math3DCommon.h"

NS_MATH3D_BEGIN

//平移操作
MATH3D_API void m3dTranslated(double x,double y,double z);

MATH3D_API void m3dTranslatef(float x,float y,float z);

//缩放操作
MATH3D_API void m3dScaled(double x,double y,double z);

MATH3D_API void m3dScalef(float x,float y,float z);

//旋转操作
MATH3D_API void m3dRotated(double angle,double x,double y,double z);

MATH3D_API void m3dRotatef(float angle,float x,float y,float z);


NS_MATH3D_END

#endif
