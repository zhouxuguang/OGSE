

#ifndef __ROTATIONMATRIX_H_INCLUDED__
#define __ROTATIONMATRIX_H_INCLUDED__

#include "Math3DCommon.h"

NS_MATH3D_BEGIN

class Vector3;
class EulerAngles;
class Quaternion;


class MATH3D_API RotationMatrix 
{
public:

	float	m11, m12, m13;
	float	m21, m22, m23;
	float	m31, m32, m33;

	void	identity();

	void	setup(const EulerAngles &orientation);

	void	fromInertialToObjectQuaternion(const Quaternion &q);
	void	fromObjectToInertialQuaternion(const Quaternion &q);

	Vector3	inertialToObject(const Vector3 &v) const;
	Vector3	objectToInertial(const Vector3 &v) const;
};

NS_MATH3D_END

#endif // #ifndef __ROTATIONMATRIX_H_INCLUDED__
