

#include "EulerAngles.h"
#include "Quaternion.h"
#include "MathUtil.h"
#include "Matrix4x3.h"
#include "RotationMatrix.h"

NS_MATH3D_BEGIN

const EulerAngles kEulerAnglesIdentity(0.0f, 0.0f, 0.0f);


void	EulerAngles::canonize() 
{
	m_fPitch = wrapPi(m_fPitch);

	if (m_fPitch < -kPiOver2) 
	{
		m_fPitch = -kPi - m_fPitch;
		m_fHeading += kPi;
		m_fBank += kPi;
	} 
	else if (m_fPitch > kPiOver2) 
	{
		m_fPitch = kPi - m_fPitch;
		m_fHeading += kPi;
		m_fBank += kPi;
	}

	if (fabs(m_fPitch) > kPiOver2 - 1e-4) 
	{
		m_fHeading += m_fBank;
		m_fBank = 0.0f;

	} 
	else 
	{ 
		m_fBank = wrapPi(m_fBank);
	}

	m_fHeading = wrapPi(m_fHeading);
}

void	EulerAngles::fromObjectToInertialQuaternion(const Quaternion &q) 
{

	float sp = -2.0f * (q.m_dfY*q.m_dfZ - q.m_dfW*q.m_dfX);

	if (fabs(sp) > 0.9999f) 
	{
		m_fPitch = kPiOver2 * sp;

		m_fHeading = atan2(-q.m_dfX*q.m_dfZ + q.m_dfW*q.m_dfY, 0.5f - q.m_dfY*q.m_dfY - q.m_dfZ*q.m_dfZ);
		m_fBank = 0.0f;

	} 
	else 
	{

		m_fPitch	= asin(sp);
		m_fHeading	= atan2(q.m_dfX*q.m_dfZ + q.m_dfW*q.m_dfY, 0.5f - q.m_dfX*q.m_dfX - q.m_dfY*q.m_dfY);
		m_fBank	= atan2(q.m_dfX*q.m_dfY + q.m_dfW*q.m_dfZ, 0.5f - q.m_dfX*q.m_dfX - q.m_dfZ*q.m_dfZ);
	}
}

void	EulerAngles::fromInertialToObjectQuaternion(const Quaternion &q) 
{
	float sp = -2.0f * (q.m_dfY*q.m_dfZ + q.m_dfW*q.m_dfX);

	if (fabs(sp) > 0.9999f) 
	{
		m_fPitch = kPiOver2 * sp;

		m_fHeading = atan2(-q.m_dfX*q.m_dfZ - q.m_dfW*q.m_dfY, 0.5f - q.m_dfY*q.m_dfY - q.m_dfZ*q.m_dfZ);
		m_fBank = 0.0f;

	} 
	else 
	{
		m_fPitch	= asin(sp);
		m_fHeading	= atan2(q.m_dfX*q.m_dfZ - q.m_dfW*q.m_dfY, 0.5f - q.m_dfX*q.m_dfX - q.m_dfY*q.m_dfY);
		m_fBank	= atan2(q.m_dfX*q.m_dfY - q.m_dfW*q.m_dfZ, 0.5f - q.m_dfX*q.m_dfX - q.m_dfZ*q.m_dfZ);
	}
}


void	EulerAngles::fromObjectToWorldMatrix(const Matrix4x3 &m) 
{

	float	sp = -m.m32;
	
	if (fabs(sp) > 9.99999f) 
	{
		m_fPitch = kPiOver2 * sp;

		m_fHeading = atan2(-m.m23, m.m11);
		m_fBank = 0.0f;

	} 
	else 
	{
		m_fHeading = atan2(m.m31, m.m33);
		m_fPitch = asin(sp);
		m_fBank = atan2(m.m12, m.m22);
	}
}

void	EulerAngles::fromWorldToObjectMatrix(const Matrix4x3 &m) 
{
	float	sp = -m.m23;
	
	if (fabs(sp) > 9.99999f) 
	{
		m_fPitch = kPiOver2 * sp;

		m_fHeading = atan2(-m.m31, m.m11);
		m_fBank = 0.0f;

	} 
	else 
	{
		m_fHeading = atan2(m.m13, m.m33);
		m_fPitch = asin(sp);
		m_fBank = atan2(m.m21, m.m22);
	}
}

void	EulerAngles::fromRotationMatrix(const RotationMatrix &m) 
{

	float	sp = -m.m23;
	
	if (fabs(sp) > 9.99999f) 
	{
		m_fPitch = kPiOver2 * sp;

		m_fHeading = atan2(-m.m31, m.m11);
		m_fBank = 0.0f;

	} 
	else 
	{
		m_fHeading = atan2(m.m13, m.m33);
		m_fPitch = asin(sp);
		m_fBank = atan2(m.m21, m.m22);
	}
}

NS_MATH3D_END
