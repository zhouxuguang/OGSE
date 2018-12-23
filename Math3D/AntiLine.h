#pragma once

#include "Math3DCommon.h"

NS_MATH3D_BEGIN

class ColorRGBA;

void cgAntiLineDDA(int x1,int y1,int x2,int y2,const ColorRGBA& color,ColorRGBA& colorBk,float fWidth);

NS_MATH3D_END
