//欧拉角实现类

#ifndef __EULERANGLES_H_INCLUDED__
#define __EULERANGLES_H_INCLUDED__

#include "Math3DCommon.h"

NS_MATH3D_BEGIN

class Quaternion;
class Matrix4x3;
class RotationMatrix;


class MATH3D_API EulerAngles 
{
public:

	float	m_fHeading;
	float	m_fPitch;
	float	m_fBank;

	EulerAngles() {}

	EulerAngles(float h, float p, float b) :
		m_fHeading(h), m_fPitch(p), m_fBank(b) {}

	void	identity() { m_fPitch = m_fBank = m_fHeading = 0.0f; }

	void	canonize();

	void	fromObjectToInertialQuaternion(const Quaternion &q);
	void	fromInertialToObjectQuaternion(const Quaternion &q);

	void	fromObjectToWorldMatrix(const Matrix4x3 &m);
	void	fromWorldToObjectMatrix(const Matrix4x3 &m);

	void	fromRotationMatrix(const RotationMatrix &m);
};

extern const EulerAngles kEulerAnglesIdentity;

NS_MATH3D_END


#endif 
