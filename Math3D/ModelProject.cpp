
#include "ModelProject.h"

NS_MATH3D_BEGIN

Matrix4x4 cgFrustum(double left, double right, double bottom, double top, double zNear, double zFar)
{
	Real matProj[16];
	memset(matProj,0,sizeof(Real)*16);

	matProj[0] = 2*zNear/(right-left);
	matProj[2] = (right+left)/(right-left);

	matProj[5] = 2*zNear/(top-bottom);
	matProj[6] = (top+bottom)/(top-bottom);

	matProj[10] = -(zFar+zNear)/(zFar-zNear);
	matProj[11] = -2*zNear*zFar/(zFar-zNear);

	matProj[14] = -1;

	return Matrix4x4(matProj);
}

Matrix4x4 cgOrtho (double left, double right, double bottom, double top, double zNear, double zFar)
{
	Real matProj[16];
	memset(matProj,0,sizeof(Real)*16);

	matProj[0] = 2/(right-left);
	matProj[3] = -(right+left)/(right-left);

	matProj[5] = 2/(top-bottom);
	matProj[7] = -(top+bottom)/(top-bottom);

	matProj[10] = -2/(zFar-zNear);
	matProj[11] = -(zFar+zNear)/(zFar-zNear);

	matProj[15] = 1;
	return Matrix4x4(matProj);
}

Matrix4x4 cgLookAt(double eyex, 
				   double eyey, 
				   double eyez, 
				   double centerx, 
				   double centery, 
				   double centerz, 
				   double upx, 
				   double upy, 
				   double upz)
{
	//首先确定观察平面法向量N
	double dNx = -(centerx - eyex);
	double dNy = -(centery - eyey);
	double dNz = -(centerz - eyez);

	//确定观察向上的向量UP
	double dVx = upx;
	double dVy = upy;
	double dVz = upz;

	//将向量N归一化
	double dbLength = sqrt(dNx*dNx + dNy*dNy + dNz*dNz);
	dNx = dNx/dbLength;
	dNy = dNy/dbLength;
	dNz = dNz/dbLength;

	//计算U向量
	dbLength = sqrt(dVx*dVx + dVy*dVy + dVz*dVz);
	double dUx = (dVy*dNz - dVz*dNy)/dbLength;
	double dUy = (dVz*dNx - dVx*dNz)/dbLength;
	double dUz = (dVx*dNy - dVy*dNx)/dbLength;

	//计算最终归一化的V向量
	dVx = (dNy*dUz - dNz*dUy);
	dVy = (dNz*dUx - dNx*dUz);
	dVz = (dNx*dUy - dNy*dUx);

	Real dbViewMatrix[16];
	memset(dbViewMatrix,0,sizeof(Real)*16);
	dbViewMatrix[0] = dUx;
	dbViewMatrix[1] = dUy;
	dbViewMatrix[2] = dUz;
	dbViewMatrix[4] = dVx;
	dbViewMatrix[5] = dVy;
	dbViewMatrix[6] = dVz;
	dbViewMatrix[8] = dNx;
	dbViewMatrix[9] = dNy;
	dbViewMatrix[10] = dNz;
	dbViewMatrix[15] = 1.0f;

	dbViewMatrix[3] = -eyex*dUx - eyey*dUy - eyez*dUz;
	dbViewMatrix[7] = -eyex*dVx - eyey*dVy - eyez*dVz;
	dbViewMatrix[11] = -eyex*dNx - eyey*dNy - eyez*dNz;

	return Matrix4x4(dbViewMatrix);
}

Matrix4x4 cgPerspective (
						 double fovy, 
						 double aspect, 
						 double zNear, 
						 double zFar)
{
	double dbFov = fovy * DEGTORAD;
	double dbTan = tan(dbFov*0.5);
	return cgFrustum(-zNear*dbTan*aspect,
		zNear*dbTan*aspect,
		-zNear*dbTan,
		zNear*dbTan,
		zNear,zFar);
}

NS_MATH3D_END

