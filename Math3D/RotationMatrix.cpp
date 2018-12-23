

#include "Vector3.h"
#include "RotationMatrix.h"
#include "MathUtil.h"
#include "Quaternion.h"
#include "EulerAngles.h"


NS_MATH3D_BEGIN

void	RotationMatrix::identity() {
	m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
}

//---------------------------------------------------------------------------
// RotationMatrix::setup
//
// Setup the matrix with the specified orientation
//
// See 10.6.1

void	RotationMatrix::setup(const EulerAngles &orientation) {

	// Fetch sine and cosine of angles

	float	sh,ch, sp,cp, sb,cb;
	sinCos(&sh, &ch, orientation.m_fHeading);
	sinCos(&sp, &cp, orientation.m_fPitch);
	sinCos(&sb, &cb, orientation.m_fBank);

	// Fill in the matrix elements

	m11 = ch * cb + sh * sp * sb;
	m12 = -ch * sb + sh * sp * cb;
	m13 = sh * cp;

	m21 = sb * cp;
	m22 = cb * cp;
	m23 = -sp;

	m31 = -sh * cb + ch * sp * sb;
	m32 = sb * sh + ch * sp * cb;
	m33 = ch * cp;
}

//---------------------------------------------------------------------------
// RotationMatrix::fromInertialToObjectQuaternion
//
// Setup the matrix, given a quaternion that performs an inertial->object
// rotation
//
// See 10.6.3

void	RotationMatrix::fromInertialToObjectQuaternion(const Quaternion &q) {

	// Fill in the matrix elements.  This could possibly be
	// optimized since there are many common subexpressions.
	// We'll leave that up to the compiler...

	m11 = 1.0f - 2.0f * (q.m_dfY*q.m_dfY + q.m_dfZ*q.m_dfZ);
	m12 = 2.0f * (q.m_dfX*q.m_dfY + q.m_dfW*q.m_dfZ);
	m13 = 2.0f * (q.m_dfX*q.m_dfZ - q.m_dfW*q.m_dfY);

	m21 = 2.0f * (q.m_dfX*q.m_dfY - q.m_dfW*q.m_dfZ);
	m22 = 1.0f - 2.0f * (q.m_dfX*q.m_dfX + q.m_dfZ*q.m_dfZ);
	m23 = 2.0f * (q.m_dfY*q.m_dfZ + q.m_dfW*q.m_dfX);

	m31 = 2.0f * (q.m_dfX*q.m_dfZ + q.m_dfW*q.m_dfY);
	m32 = 2.0f * (q.m_dfY*q.m_dfZ - q.m_dfW*q.m_dfX);
	m33 = 1.0f - 2.0f * (q.m_dfX*q.m_dfX + q.m_dfY*q.m_dfY);

}

//---------------------------------------------------------------------------
// RotationMatrix::fromObjectToInertialQuaternion
//
// Setup the matrix, given a quaternion that performs an object->inertial
// rotation
//
// See 10.6.3

void	RotationMatrix::fromObjectToInertialQuaternion(const Quaternion &q) {

	// Fill in the matrix elements.  This could possibly be
	// optimized since there are many common subexpressions.
	// We'll leave that up to the compiler...

	m11 = 1.0f - 2.0f * (q.m_dfY*q.m_dfY + q.m_dfZ*q.m_dfZ);
	m12 = 2.0f * (q.m_dfX*q.m_dfY - q.m_dfW*q.m_dfZ);
	m13 = 2.0f * (q.m_dfX*q.m_dfZ + q.m_dfW*q.m_dfY);

	m21 = 2.0f * (q.m_dfX*q.m_dfY + q.m_dfW*q.m_dfZ);
	m22 = 1.0f - 2.0f * (q.m_dfX*q.m_dfX + q.m_dfZ*q.m_dfZ);
	m23 = 2.0f * (q.m_dfY*q.m_dfZ - q.m_dfW*q.m_dfX);

	m31 = 2.0f * (q.m_dfX*q.m_dfZ - q.m_dfW*q.m_dfY);
	m32 = 2.0f * (q.m_dfY*q.m_dfZ + q.m_dfW*q.m_dfX);
	m33 = 1.0f - 2.0f * (q.m_dfX*q.m_dfX + q.m_dfY*q.m_dfY);
}

//---------------------------------------------------------------------------
// RotationMatrix::inertialToObject
//
// Rotate a vector from inertial to object space

Vector3	RotationMatrix::inertialToObject(const Vector3 &v) const {

	// Perform the matrix multiplication in the "standard" way.

	return Vector3(
		m11*v.x + m21*v.y + m31*v.z,
		m12*v.x + m22*v.y + m32*v.z,
		m13*v.x + m23*v.y + m33*v.z
	);
}

//---------------------------------------------------------------------------
// RotationMatrix::objectToInertial
//
// Rotate a vector from object to inertial space

Vector3	RotationMatrix::objectToInertial(const Vector3 &v) const {

	// Multiply by the transpose

	return Vector3(
		m11*v.x + m12*v.y + m13*v.z,
		m21*v.x + m22*v.y + m23*v.z,
		m31*v.x + m32*v.y + m33*v.z
	);
}

NS_MATH3D_END
