
#ifndef __MATH3D_QUATERNION_H_INCLUDED__
#define __MATH3D_QUATERNION_H_INCLUDED__

//��Ԫ������

#include "Math3DCommon.h"
#include "Vector3.h"

NS_MATH3D_BEGIN

class Matrix3x3;


class MATH3D_API Quaternion 
{
public:
	Real	m_dfW, m_dfX, m_dfY, m_dfZ;

	//���캯��

	Quaternion();

	Quaternion(Real w,Real x,Real y,Real z);

	Quaternion(const Quaternion& rhs);

	Quaternion & operator=(const Quaternion& rhs);
    
    ~Quaternion();

	//��Ա����

	//����ת����������Ԫ��
	void FromRotateMatrix(const Matrix3x3& mRot);

	//ת������ת����
	void ToRotateMatrix(Matrix3x3& mRot) const;

	//����Ƕ�������Ԫ��,�Ƕ��Ƕ�������λ�����ǵ�λ����
	void FromAngleAxis(const Real fAngle,const Vector3& vecAxis);

	//����Ԫ��������Ƕ�,�Ƕ��Ƕ�������λ�����ǵ�λ����
	void ToAngleAxis(Real& fAngle, Vector3& vecAxis) const;

	//��Ԫ����ģ
	Real Norm() const;


	void	SetToRotateAboutX(float theta);
	void	SetToRotateAboutY(float theta);
	void	SetToRotateAboutZ(float theta);


	Quaternion operator *(const Quaternion &other) const;

	Quaternion &operator *=(const Quaternion &other);
    
    Vector3 operator* (const Vector3& v) const;


	void Normalize();

	Real DotProduct( const Quaternion &other) const;
    
    /**
     *
     *  @return
     */
    bool Inverse();
    
    Quaternion GetInversed() const;


	//��ֵ
	static Quaternion Slerp(const Quaternion &p, const Quaternion &q, Real t);

	//����
	static Quaternion Conjugate(const Quaternion &q);

	//����
	static Quaternion Pow(const Quaternion &q, float fExponent);
};

NS_MATH3D_END
 

#endif // #ifndef __QUATERNION_H_INCLUDED__
