#include "Vector3.h"

NS_MATH3D_BEGIN


const Vector3 Vector3::ZERO( 0, 0, 0 );
const Vector3 Vector3::UNIT_SCALE(1, 1, 1);
const Vector3 Vector3::UNIT_X(1, 0, 0);
const Vector3 Vector3::UNIT_Y(0, 1, 0);
const Vector3 Vector3::UNIT_Z(0, 0, 1);

const Real Vector3::Length() const
{
	return sqrt(x * x + y * y + z*z);
}

Real Vector3::Length()
{
	return sqrt(x * x + y * y + z*z);
}

Real Vector3::DotProduct(const Vector3 &a) const
{
	return x*a.x + y*a.y + z*a.z;
}

void Vector3::Normalize()
{
	Real magSq = x*x + y*y + z*z;
	if (magSq > 0.0f) 
	{ 
		float oneOverMag = 1.0f / sqrt(magSq);
		x *= oneOverMag;
		y *= oneOverMag;
		z *= oneOverMag;
	}
}

Real Vector3::LengthSq() const
{
	return x * x + y * y + z*z;
}

Vector3 Vector3::Reflection(const Vector3& vecLight,const Vector3& vecNormal)
{
	return vecNormal*(vecNormal.DotProduct(vecNormal))*2 - vecLight;
}

Vector3 Vector3::Refraction(const Vector3& vecLight,const Vector3& vecNormal,float eta)
{
	Real k = 1.0 - eta*eta *(1.0-vecNormal.DotProduct(vecLight)*vecNormal.DotProduct(vecLight));
	if ( k < 0)
	{
		return Vector3::ZERO;
	}

	else
		return vecLight*eta - vecNormal * (eta*vecNormal.DotProduct(vecLight)+sqrt(k));
}

Vector3 CrossProduct(const Vector3 &a, const Vector3 &b)
{
	return Vector3(
		a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x
		);
}

Real Distance(const Vector3 &a, const Vector3 &b)
{
	Real dx = a.x - b.x;
	Real dy = a.y - b.y;
	Real dz = a.z - b.z;
	return sqrt(dx*dx + dy*dy + dz*dz);
}

Real DistanceSquared(const Vector3 &a, const Vector3 &b)
{
	Real dx = a.x - b.x;
	Real dy = a.y - b.y;
	Real dz = a.z - b.z;
	return dx*dx + dy*dy + dz*dz;
}

NS_MATH3D_END
