#ifndef MATH3D_VIEWPORT_INCLUDE_H
#define MATH3D_VIEWPORT_INCLUDE_H

#include "Math3DCommon.h"

class MATH3D_API ViewPort
{
public:
	ViewPort(void);

	ViewPort(int x,int y,int width,int height);

	~ViewPort(void);

private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
};

#endif
