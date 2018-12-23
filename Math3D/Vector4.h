#ifndef MATH3DLIB_MATRIX410_INCLUDE
#define MATH3DLIB_MATRIX410_INCLUDE

#include "Math3DCommon.h"
#include "Vector3.h"

NS_MATH3D_BEGIN

//4*1矩阵，代表列向量

class MATH3D_API Vector4
{
public:
	Vector4(const Real *pfMatrix);

	Vector4(Real a1,Real a2,Real a3,Real a4);

	explicit Vector4(const Vector3& rhs);

	Vector4& operator = ( const Vector4& rkVector );

	~Vector4(void);

	Vector4 &operator +(Real fValue);

	Vector4 &operator -(Real fValue);

	Vector4 &operator *(Real fValue);

	Vector4 &operator /(Real fValue);

	inline const Real& operator [](size_t nIndex) const
	{
		assert(nIndex < 4);
		return *(&x+nIndex);
	}

	inline Real& operator [](size_t nIndex)
	{
		assert(nIndex < 4);
		return *(&x+nIndex);
	}

	inline Real* GetPtr()
	{
		return &x;
	}

	inline const Real* GetPtr() const
	{
		return &x;
	}

	Vector4& operator = (const Vector3& rhs);

	inline Vector4 operator + ( const Vector4& rkVector ) const
	{
		return Vector4(
			x + rkVector.x,
			y + rkVector.y,
			z + rkVector.z,
			w + rkVector.w);
	}

	inline Vector4 operator - ( const Vector4& rkVector ) const
	{
		return Vector4(
			x - rkVector.x,
			y - rkVector.y,
			z - rkVector.z,
			w - rkVector.w);
	}

	inline Vector4 operator * ( const Real fScalar ) const
	{
		return Vector4(
			x * fScalar,
			y * fScalar,
			z * fScalar,
			w * fScalar);
	}

	inline Vector4 operator * ( const Vector4& rhs) const
	{
		return Vector4(
			rhs.x * x,
			rhs.y * y,
			rhs.z * z,
			rhs.w * w);
	}

	inline Vector4 operator / ( const Real fScalar ) const
	{
		assert( fScalar != 0.0 );

		Real fInv = 1.0f / fScalar;

		return Vector4(
			x * fInv,
			y * fInv,
			z * fInv,
			w * fInv);
	}

	inline Vector4 operator / ( const Vector4& rhs) const
	{
		return Vector4(
			x / rhs.x,
			y / rhs.y,
			z / rhs.z,
			w / rhs.w);
	}

	inline const Vector4& operator + () const
	{
		return *this;
	}

	inline Vector4 operator - () const
	{
		return Vector4(-x, -y, -z, -w);
	}

public:
	Real x, y, z, w;
};


NS_MATH3D_END

#endif
