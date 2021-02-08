
#include <memory.h>
#include "Vector4.h"

NS_MATH3D_BEGIN

Vector4::Vector4(const Real *pfMatrix)
{
	memcpy(&x,pfMatrix,sizeof(Real)*4);
}

Vector4::Vector4(Real a1,Real a2,Real a3,Real a4)
{
	x = a1;
	y = a2;
	z = a3;
	w = a4;
}

Vector4::Vector4(const Vector3& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f)
{

}

Vector4::~Vector4(void)
{
}

Vector4& Vector4::operator +(Real fValue)
{
	x += fValue;
	y += fValue;
	z += fValue;
	w += fValue;

	return *this;
}

Vector4& Vector4::operator -(Real fValue)
{
	x -= fValue;
	y -= fValue;
	z -= fValue;
	w -= fValue;

	return *this;
}

Vector4& Vector4::operator *(Real fValue)
{
	x *= fValue;
	y *= fValue;
	z *= fValue;
	w *= fValue;

	return *this;
}

Vector4& Vector4::operator /(Real fValue)
{
	x /= fValue;
	y /= fValue;
	z /= fValue;
	w /= fValue;

	return *this;
}

Vector4& Vector4::operator=(const Vector4& rkVector)
{
	x = rkVector.x;
	y = rkVector.y;
	z = rkVector.z;
	w = rkVector.w;

	return *this;
}

Vector4& Vector4::operator=(const Vector3& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = 1.0f;
	return *this;
}


NS_MATH3D_END

