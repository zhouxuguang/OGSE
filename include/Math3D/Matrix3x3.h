
#ifndef __MATRIX3X3_H_INCLUDED__JF456994566664
#define __MATRIX3X3_H_INCLUDED__JF456994566664

#include "Math3DCommon.h"

NS_MATH3D_BEGIN

//������Ĵ洢,��ת����

class Vector3;

class MATH3D_API Matrix3x3
{
public:
	Matrix3x3(void);

	explicit Matrix3x3(const Real arr[3][3]);

	explicit Matrix3x3(const Real* pfArr);

	Matrix3x3(const Matrix3x3& rkMatrix);

	Matrix3x3(Real a1,Real a2,Real a3,
		Real b1,Real b2,Real b3,
		Real c1,Real c2,Real c3);
    
    Matrix3x3(const Vector3& vec1,const Vector3& vec2,const Vector3& vec3);

	~Matrix3x3(void);

	void MakeIdentity();

	//���ز�����

	Matrix3x3 operator +(Real fValue) const;

	Matrix3x3 operator -(Real fValue) const;

	Matrix3x3 operator *(Real fValue) const;

	Matrix3x3 operator /(Real fValue) const;

	Vector3 operator *(const Vector3& vec);

	Matrix3x3 operator -() const;

	inline Real* operator [](size_t nIndex)
	{
		return m_Values[nIndex];
	}

	inline const Real* operator [](size_t nIndex) const
	{
		return m_Values[nIndex];
	}

	//������������֮��Ĳ�����
	Matrix3x3 operator +(const Matrix3x3& rhs) const;

	Matrix3x3 operator -(const Matrix3x3& rhs) const;

	Matrix3x3 operator *(const Matrix3x3& rhs) const;

	//��ʼ��
	void Init(Real a1,Real a2,Real a3,
		Real b1,Real b2,Real b3,
		Real c1,Real c2,Real c3);

	//��������ʽ
	Real Determinant() const;

	bool Inverse (Matrix3x3& rkInverse, Real fTolerance = 1e-06) const;

	Matrix3x3 Inverse (Real fTolerance = 1e-06) const;

	//����ת��
	Matrix3x3 Transpose() const;

private:
	union		//����Ԫ��
	{
		Real m_adfValues[9];
		Real m_Values[3][3];
	};
    
public:
    //�������
public:
    static const Matrix3x3 IDENTITY;
    static const Matrix3x3 ZERO;
	
};

NS_MATH3D_END

#endif
