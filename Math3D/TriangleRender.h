#ifndef MATH32_TRIANGLE_RENDER_INCLUDE_H
#define MATH32_TRIANGLE_RENDER_INCLUDE_H

#include "Math3DCommon.h"

NS_MATH3D_BEGIN

class ColorRGBA;

void FillTriangle(int x1,int y1,int x2,int y2,int x3,int y3,const ColorRGBA& color);


NS_MATH3D_END

#endif
