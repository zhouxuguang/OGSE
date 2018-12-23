#include "Triangle3D.h"

NS_MATH3D_BEGIN

Triangle3D::Triangle3D(void)
{
}

Triangle3D::Triangle3D(const Vector3& v1, const Vector3& v2, const Vector3& v3)
					:mPointA(v1),mPointB(v2),mPointC(v3)
{
}

Triangle3D::~Triangle3D(void)
{
}

Plane3D Triangle3D::GetPlane(void) const
{
	return Plane3D(mPointA,mPointB,mPointC);
}

Vector3 Triangle3D::GetNormal(void) const
{
	return CrossProduct(mPointB - mPointA,mPointC - mPointA);
}

Real Triangle3D::GetArea(void) const
{
	return CrossProduct(mPointB - mPointA,mPointC - mPointA).Length() * 0.5;
}

bool Triangle3D::IsPointInside(const Vector3& p) const
{
	//通过计算重心坐标判断
	const Vector3 a = mPointC - mPointA;
	const Vector3 b = mPointB - mPointA;
	const Vector3 c = p - mPointA;

	const Real dotAA = a.DotProduct( a);
	const Real dotAB = a.DotProduct( b);
	const Real dotAC = a.DotProduct( c);
	const Real dotBB = b.DotProduct( b);
	const Real dotBC = b.DotProduct( c);

	// get coordinates in barycentric coordinate system
	const Real invDenom =  1/(dotAA * dotBB - dotAB * dotAB);
	const Real u = (dotBB * dotAC - dotAB * dotBC) * invDenom;
	const Real v = (dotAA * dotBC - dotAB * dotAC ) * invDenom;

	const Real ROUNDING_ERROR_f64 = 0.00000001;

	// We count border-points as inside to keep downward compatibility.
	// Rounding-error also needed for some test-cases.
	return (u > -ROUNDING_ERROR_f64) && (v >= 0) && (u + v < 1+ROUNDING_ERROR_f64);
}

NS_MATH3D_END

