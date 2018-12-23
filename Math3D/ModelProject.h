#ifndef MODELPROJECT_INCLUDE
#define MODELPROJECT_INCLUDE

#include "Math3DCommon.h"
#include "Matrix4x4.h"

NS_MATH3D_BEGIN

MATH3D_API Matrix4x4 cgFrustum(double left, double right, double bottom, double top, double zNear, double zFar);

MATH3D_API Matrix4x4 cgOrtho(double left, double right, double bottom, double top, double zNear, double zFar);

MATH3D_API Matrix4x4 cgLookAt(double eyex, 
							  double eyey, 
							  double eyez, 
							  double centerx, 
							  double centery, 
							  double centerz, 
							  double upx, 
							  double upy, 
							  double upz);

MATH3D_API Matrix4x4 cgPerspective (
									double fovy, 
									double aspect, 
									double zNear, 
									double zFar);


NS_MATH3D_END

#endif
