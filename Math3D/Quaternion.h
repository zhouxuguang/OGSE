
#ifndef __MATH3D_QUATERNION_H_INCLUDED__
#define __MATH3D_QUATERNION_H_INCLUDED__

//四元数的类

#include "Math3DCommon.h"
#include "Vector3.h"

NS_MATH3D_BEGIN

class EulerAngles;
class Matrix3x3;


class MATH3D_API Quaternion 
{
public:
	Real	m_dfW, m_dfX, m_dfY, m_dfZ;

	//构造函数

	Quaternion();

	Quaternion(Real w,Real x,Real y,Real z);

	Quaternion(const Quaternion& rhs);

	Quaternion & operator=(const Quaternion& rhs);
    
    ~Quaternion();

	//成员函数

	//由旋转矩阵生成四元数
	void FromRotateMatrix(const Matrix3x3& mRot);

	//转换到旋转矩阵
	void ToRotateMatrix(Matrix3x3& mRot) const;

	//由轴角对生成四元数,角度是度数，方位向量是单位向量
	void FromAngleAxis(const Real fAngle,const Vector3& vecAxis);

	//由四元数生成轴角对,角度是度数，方位向量是单位向量
	void ToAngleAxis(Real& fAngle, Vector3& vecAxis) const;

	//四元数求模
	Real Norm() const;


	void	SetToRotateAboutX(float theta);
	void	SetToRotateAboutY(float theta);
	void	SetToRotateAboutZ(float theta);

	void	SetToRotateObjectToInertial(const EulerAngles &orientation);
	void	SetToRotateInertialToObject(const EulerAngles &orientation);


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


	//插值
	static Quaternion Slerp(const Quaternion &p, const Quaternion &q, Real t);

	//共轭
	static Quaternion Conjugate(const Quaternion &q);

	//求幂
	static Quaternion Pow(const Quaternion &q, float fExponent);
};

NS_MATH3D_END
 

#endif // #ifndef __QUATERNION_H_INCLUDED__
