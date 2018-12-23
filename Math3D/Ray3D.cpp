#include "Ray3D.h"

NS_MATH3D_BEGIN

Ray3D::Ray3D(const Vector3& origin,const Vector3& direction)
:mOrigin(origin),mDirection(direction)
{

}

Ray3D::Ray3D(const Ray3D& ray)
{
	mOrigin = ray.mOrigin;
	mDirection = ray.mDirection;
}

Ray3D::Ray3D(void)
{
	mOrigin.Zero();
	mDirection.Zero();
}

Ray3D& Ray3D::operator=(const Ray3D& ray)
{
	if (this == &ray)
	{
		return *this;
	}
	mOrigin = ray.mOrigin;
	mDirection = ray.mDirection;
	return *this;

}

Ray3D::~Ray3D(void)
{
}

void Ray3D::SetOrigin(const Vector3& origin)
{
	mOrigin = origin;
}

const Vector3& Ray3D::GetOrigin(void) const
{
	return mOrigin;
}

void Ray3D::SetDirection(const Vector3& dir)
{
	mDirection = dir;
}

const Vector3& Ray3D::SetDirection(void) const
{
	return mDirection;
}

Vector3 Ray3D::GetPoint(Real t) const
{
	return Vector3(mOrigin + (mDirection * t));
}

NS_MATH3D_END
