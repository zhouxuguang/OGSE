#include "Sphere.h"

NS_MATH3D_BEGIN

const Real Sphere::kEpsilon = 0.0000001;

Sphere::Sphere(void): mRadius(1.0), mCenter(Vector3::ZERO)
{
}

Sphere::Sphere(const Vector3& center, Real radius) : mRadius(radius), mCenter(center)
{
}

Sphere::~Sphere(void)
{
}

Real Sphere::GetRadius(void) const
{
	return mRadius;
}

void Sphere::SetRadius(Real radius)
{
	mRadius = radius;
}

const Vector3& Sphere::GetCenter(void) const
{
	return mCenter;
}

void Sphere::SetCenter(const Vector3& center)
{
	mCenter = center;
}

bool Sphere::Intersects(const Sphere& s) const
{
	return (s.mCenter - mCenter).LengthSq() <=
	 		(s.mRadius + mRadius)*(s.mRadius + mRadius);
}

bool Sphere::Intersects(const AxisAlignedBox& box) const
{
	//return Math::intersects(*this, box);
	return false;
}

bool Sphere::Intersects(const Plane3D& plane) const
{
	//return Math::intersects(*this, plane);
	return false;
}

bool Sphere::Intersects(const Vector3& v) const
{
	return (v - mCenter).LengthSq() <= mRadius * mRadius;
}

void Sphere::Merge(const Sphere& oth)
{
	Vector3 diff =  oth.mCenter - mCenter;
	Real lengthSq = diff.LengthSq();
	Real radiusDiff = oth.mRadius - mRadius;

	if (radiusDiff*radiusDiff >= lengthSq) 
	{
		if (radiusDiff <= 0.0f) 
			return;
		else 
		{
			mCenter = oth.mCenter;
			mRadius = oth.mRadius;
			return;
		}
	}

	Real length = sqrt(lengthSq);

	Vector3 newCenter;
	Real newRadius;
	if ((length + oth.mRadius) > mRadius) 
	{
		Real t = (length + radiusDiff) / (2.0f * length);
		newCenter = mCenter + diff * t;
	} 

	newRadius = 0.5f * (length + mRadius + oth.mRadius);

	mCenter = newCenter;
	mRadius = newRadius;
}

bool Sphere::Hit(const Ray3D& ray, double& tMin, ShadeRec& sr) const
{
//	Real t = 0;
//	Vector3 temp = ray.mOrigin - mCenter;
//	Real a = ray.mDirection*ray.mDirection;
//	Real b = 2.0*temp*ray.mDirection;
//	Real c = temp * temp - mRadius * mRadius;
//	Real disc = b * b - 4.0 * a * c;
//	if (disc < 0)
//	{
//		return false;
//	}
//
//	else
//	{
//		double e = sqrt(disc);
//		double denom = 2.0 * a;
//		t = (-b-e)/denom;
//
//		if (t > kEpsilon)
//		{
//			tMin = t;
//			sr.normal = (temp + t*ray.mDirection)/mRadius;
//			sr.local_hit_point = ray.GetPoint(t);
//			return true;
//		}
//
//		t = (-b+e)/denom;
//		if (t > kEpsilon)
//		{
//			tMin = t;
//			sr.normal = (temp + t*ray.mDirection)/mRadius;
//			sr.local_hit_point = ray.GetPoint(t);
//			return true;
//		}
//	}

	return false;
}

NS_MATH3D_END
