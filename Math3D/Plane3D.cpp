#include "Plane3D.h"

NS_MATH3D_BEGIN

const Real Plane3D::kEpsilon = 0.000001;

Plane3D::~Plane3D(void)
{
}

Plane3D::Plane3D(const Vector3& rkNormal,Real fConstant)
{
	m_Normal = rkNormal;
	m_fD = fConstant;
}

Plane3D::Plane3D(Real a,Real b,Real c,Real d):m_Normal(a, b, c), m_fD(d)
{
}

Plane3D::Plane3D(const Plane3D& rhs)
{
	m_Normal = rhs.m_Normal;
	m_fD = rhs.m_fD;
}

Plane3D::Plane3D(const Vector3& rkPoint1, const Vector3& rkPoint2, const Vector3& rkPoint3)
{
	ReDefine(rkPoint1,rkPoint2,rkPoint3);
}

Plane3D& Plane3D::operator=(const Plane3D& rhs)
{
	m_Normal = rhs.m_Normal;
	m_fD = rhs.m_fD;

	return *this;
}

Real Plane3D::GetDistance(const Vector3& rkPoint) const
{
	return m_Normal.DotProduct(rkPoint) + m_fD;
}

Plane3D::Side Plane3D::GetSide(const Vector3& rkPoint) const
{
	Real fDistance = GetDistance(rkPoint);

	if ( fDistance < 0.0 )
		return Plane3D::NEGATIVE_SIDE;

	if ( fDistance > 0.0 )
		return Plane3D::POSITIVE_SIDE;

	return Plane3D::NO_SIDE;
}

void Plane3D::ReDefine(const Vector3& rkPoint1, const Vector3& rkPoint2,const Vector3& rkPoint3)
{
	Vector3 e3 = rkPoint2 - rkPoint1;
	Vector3 e1 = rkPoint3 - rkPoint2;
	m_Normal = CrossProduct(e1,e3);
	m_Normal.Normalize();

	m_fD = -m_Normal.DotProduct(rkPoint1);
}

Real Plane3D::Normalise(void)
{
	Real fLength = m_Normal.Length();

	if ( fLength > Real(0.0f) )
	{
		Real fInvLength = 1.0f / fLength;
		m_Normal *= fInvLength;
		m_fD *= fInvLength;
	}

	return fLength;
}

bool Plane3D::Hit(const Ray3D& ray, double& t, ShadeRec& sr) const
{
	Real t1 = m_Normal.DotProduct(ray.mDirection);
	if (t1 > kEpsilon)		//射线和平面不平行
	{
		Real t2 = -m_fD - m_Normal.DotProduct(ray.mOrigin);
		t = t2/t1;

		if (t > kEpsilon)
		{
			sr.normal = m_Normal;
			sr.local_hit_point = ray.GetPoint(t);
			return true;
		}
		
		return false;
	}

	return false;
}

NS_MATH3D_END
