
#include <memory.h>
#include "ModelTransform.h"
#include "Matrix4x4.h"

NS_MATH3D_BEGIN

extern Matrix4x4 g_matViewModel;

//投影矩阵
extern Matrix4x4 g_matProject;


void m3dTranslated(double x,double y,double z)
{
	Real adfValues[16];
	memset(adfValues,0,sizeof(Real)*16);
	adfValues[0] = 1;
	adfValues[5] = 1;
	adfValues[10] = 1;
	adfValues[15] = 1;

	adfValues[3] = x;
	adfValues[7] = y;
	adfValues[11] = z;

	g_matViewModel = Matrix4x4(adfValues)*g_matViewModel;
}

void m3dTranslatef(float x,float y,float z)
{
	m3dTranslated(x,y,z);
}

void m3dScaled(double x,double y,double z)
{
	Real adfValues[16];
	memset(adfValues,0,sizeof(Real)*16);
	adfValues[0] = x;
	adfValues[5] = y;
	adfValues[10] = z;
	adfValues[15] = 1;

	g_matViewModel = Matrix4x4(adfValues)*g_matViewModel;
}

void m3dScalef(float x,float y,float z)
{
	return m3dScaled(x,y,z);
}

void m3dRotated(double angle,double x,double y,double z)
{
	Real adfValues[16];
	memset(adfValues,0,sizeof(Real)*16);
	adfValues[15] = 1;

	//旋转轴单位化
	double dbLen = sqrt(x*x+y*y+z*z);
	x /= dbLen;
	y /= dbLen;
	z /= dbLen;

	angle = angle*DEGTORAD;

	double db1costheta = 1.0-cos(angle);
	double dbSinTheta = sin(angle);
	double dbCosTheta = cos(angle);

	//逆时针为正
	adfValues[0] = x*x*db1costheta + dbCosTheta;
	adfValues[1] = x*y*db1costheta - z*dbSinTheta;
	adfValues[2] = x*z*db1costheta + y*dbSinTheta;

	adfValues[4] = x*y*db1costheta + z*dbSinTheta;
	adfValues[5] = y*y*db1costheta + dbCosTheta;
	adfValues[6] = y*z*db1costheta - x*dbSinTheta;

	adfValues[8] = x*z*db1costheta - y*dbSinTheta;
	adfValues[9] = y*z*db1costheta + x*dbSinTheta;
	adfValues[10] = z*z*db1costheta + dbCosTheta;

	g_matViewModel = Matrix4x4(adfValues)*g_matViewModel;
}

void m3dRotatef(float angle,float x,float y,float z)
{
	return m3dRotated(angle,x,y,z);
}



NS_MATH3D_END
