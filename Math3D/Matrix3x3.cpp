#include "Matrix3x3.h"
#include "Vector3.h"

NS_MATH3D_BEGIN

const Matrix3x3 Matrix3x3::IDENTITY = Matrix3x3(
                                                1.0, 0.0, 0.0,
                                                0.0, 1.0, 0.0,
                                                0.0, 0.0, 1.0);

const Matrix3x3 Matrix3x3::ZERO = Matrix3x3(
                                            0, 0, 0,
                                            0, 0, 0,
                                            0, 0, 0 );

Matrix3x3::Matrix3x3(void)
{
	memset(m_adfValues,0,sizeof(Real)*9);
}

Matrix3x3::Matrix3x3(Real a1,Real a2,Real a3,
					 Real b1,Real b2,Real b3,
					 Real c1,Real c2,Real c3)
{
	m_adfValues[0] = a1;
	m_adfValues[1] = a2;
	m_adfValues[2] = a3;
	m_adfValues[3] = b1;
	m_adfValues[4] = b2;
	m_adfValues[5] = b3;
	m_adfValues[6] = c1;
	m_adfValues[7] = c2;
	m_adfValues[8] = c3;
}

Matrix3x3::Matrix3x3(const Vector3& vec1,const Vector3& vec2,const Vector3& vec3)
{
    memcpy(m_adfValues, &vec1, sizeof(Real)*3);
    memcpy(m_adfValues+3, &vec2, sizeof(Real)*3);
    memcpy(m_adfValues+6, &vec3, sizeof(Real)*3);
}

Matrix3x3::Matrix3x3(const Real arr[3][3])
{
	memcpy(m_adfValues,arr,sizeof(Real)*9);
}

Matrix3x3::Matrix3x3(const Matrix3x3& rkMatrix)
{
	memcpy(m_adfValues,rkMatrix.m_adfValues,sizeof(Real)*9);
}

Matrix3x3::Matrix3x3(const Real* pfArr)
{
	memcpy(m_adfValues,pfArr,sizeof(Real)*9);
}

Matrix3x3::~Matrix3x3(void)
{
}

Matrix3x3 Matrix3x3::operator +(Real fValue) const
{
	Matrix3x3 rtMat;
	rtMat[0][0] = m_Values[0][0] + fValue;
	rtMat[0][1] = m_Values[0][1] + fValue;
	rtMat[0][2] = m_Values[0][2] + fValue;

	rtMat[1][0] = m_Values[1][0] + fValue;
	rtMat[1][1] = m_Values[1][1] + fValue;
	rtMat[1][2] = m_Values[1][2] + fValue;

	rtMat[2][0] = m_Values[2][0] + fValue;
	rtMat[2][1] = m_Values[2][1] + fValue;
	rtMat[2][2] = m_Values[2][2] + fValue;

	return rtMat;
}

Matrix3x3 Matrix3x3::operator+(const Matrix3x3& rhs) const
{
	Matrix3x3 rtMat;
	rtMat[0][0] = m_Values[0][0] + rhs[0][0];
	rtMat[0][1] = m_Values[0][1] + rhs[0][1];
	rtMat[0][2] = m_Values[0][2] + rhs[0][2];

	rtMat[1][0] = m_Values[1][0] + rhs[1][0];
	rtMat[1][1] = m_Values[1][1] + rhs[1][1];
	rtMat[1][2] = m_Values[1][2] + rhs[1][2];

	rtMat[2][0] = m_Values[2][0] + rhs[2][0];
	rtMat[2][1] = m_Values[2][1] + rhs[2][1];
	rtMat[2][2] = m_Values[2][2] + rhs[2][2];

	return rtMat;
}

Matrix3x3 Matrix3x3::operator -(Real fValue) const
{
	Matrix3x3 rtMat;
	rtMat[0][0] = m_Values[0][0] - fValue;
	rtMat[0][1] = m_Values[0][1] - fValue;
	rtMat[0][2] = m_Values[0][2] - fValue;

	rtMat[1][0] = m_Values[1][0] - fValue;
	rtMat[1][1] = m_Values[1][1] - fValue;
	rtMat[1][2] = m_Values[1][2] - fValue;

	rtMat[2][0] = m_Values[2][0] - fValue;
	rtMat[2][1] = m_Values[2][1] - fValue;
	rtMat[2][2] = m_Values[2][2] - fValue;

	return rtMat;
}

Matrix3x3 Matrix3x3::operator *(Real fValue) const
{
	Matrix3x3 rtMat;
	rtMat[0][0] = m_Values[0][0] * fValue;
	rtMat[0][1] = m_Values[0][1] * fValue;
	rtMat[0][2] = m_Values[0][2] * fValue;

	rtMat[1][0] = m_Values[1][0] * fValue;
	rtMat[1][1] = m_Values[1][1] * fValue;
	rtMat[1][2] = m_Values[1][2] * fValue;

	rtMat[2][0] = m_Values[2][0] * fValue;
	rtMat[2][1] = m_Values[2][1] * fValue;
	rtMat[2][2] = m_Values[2][2] * fValue;

	return rtMat;
}

Matrix3x3 Matrix3x3::operator /(Real fValue) const
{
	Matrix3x3 rtMat;
	rtMat[0][0] = m_Values[0][0] / fValue;
	rtMat[0][1] = m_Values[0][1] / fValue;
	rtMat[0][2] = m_Values[0][2] / fValue;

	rtMat[1][0] = m_Values[1][0] / fValue;
	rtMat[1][1] = m_Values[1][1] / fValue;
	rtMat[1][2] = m_Values[1][2] / fValue;

	rtMat[2][0] = m_Values[2][0] / fValue;
	rtMat[2][1] = m_Values[2][1] / fValue;
	rtMat[2][2] = m_Values[2][2] / fValue;

	return rtMat;
}

Vector3 Matrix3x3::operator*(const Vector3 &v)
{
    return Vector3(
                   m_Values[0][0] * v[0] + m_Values[0][1] * v[1] + m_Values[0][2] * v[2],
                   m_Values[1][0] * v[0] + m_Values[1][1] * v[1] + m_Values[1][2] * v[2],
                   m_Values[2][0] * v[0] + m_Values[2][1] * v[1] + m_Values[2][2] * v[2]
                   );
}

Matrix3x3 Matrix3x3::operator *(const Matrix3x3& rhs) const
{
	const Real *pfOther = rhs.m_adfValues;
	Real adfResult[9];
	memset(adfResult,0,sizeof(Real)*9);

	for (int i = 0; i < 3; i ++)
	{
		for (int j = 0; j < 3; j ++)
		{
			for (int k = 0; k < 3; k ++)
			{
				adfResult[i*3+j] += m_adfValues[i*3+k]*pfOther[k*3+j];
			}
		}
	}

	return Matrix3x3(adfResult);
}

void Matrix3x3::Init(Real a1,Real a2,Real a3,
					 Real b1,Real b2,Real b3,
					 Real c1,Real c2,Real c3)
{
	m_adfValues[0] = a1;
	m_adfValues[1] = a2;
	m_adfValues[2] = a3;
	m_adfValues[3] = b1;
	m_adfValues[4] = b2;
	m_adfValues[5] = b3;
	m_adfValues[6] = c1;
	m_adfValues[7] = c2;
	m_adfValues[8] = c3;
}

Matrix3x3 Matrix3x3::operator-() const
{
	Matrix3x3 rtMat;
	rtMat[0][0] = -m_Values[0][0];
	rtMat[0][1] = -m_Values[0][1];
	rtMat[0][2] = -m_Values[0][2];

	rtMat[1][0] = -m_Values[1][0];
	rtMat[1][1] = -m_Values[1][1];
	rtMat[1][2] = -m_Values[1][2];

	rtMat[2][0] = -m_Values[2][0];
	rtMat[2][1] = -m_Values[2][1];
	rtMat[2][2] = -m_Values[2][2];

	return rtMat;
}

Matrix3x3 Matrix3x3::operator-(const Matrix3x3& rhs) const
{
	Matrix3x3 rtMat;
	rtMat[0][0] = m_Values[0][0] - rhs[0][0];
	rtMat[0][1] = m_Values[0][1] - rhs[0][1];
	rtMat[0][2] = m_Values[0][2] - rhs[0][2];

	rtMat[1][0] = m_Values[1][0] - rhs[1][0];
	rtMat[1][1] = m_Values[1][1] - rhs[1][1];
	rtMat[1][2] = m_Values[1][2] - rhs[1][2];

	rtMat[2][0] = m_Values[2][0] - rhs[2][0];
	rtMat[2][1] = m_Values[2][1] - rhs[2][1];
	rtMat[2][2] = m_Values[2][2] - rhs[2][2];

	return rtMat;
}

Real Matrix3x3::Determinant() const
{
	return m_Values[0][0]*(m_Values[1][1]*m_Values[2][2] - m_Values[1][2]*m_Values[2][1]) +
		m_Values[0][1]*(m_Values[1][2]*m_Values[2][0] - m_Values[1][0]*m_Values[2][2]) + 
		m_Values[0][2]*(m_Values[1][0]*m_Values[2][1] - m_Values[1][1]*m_Values[2][0]);
}

bool Matrix3x3::Inverse(Matrix3x3& rkInverse, Real fTolerance /*= 1e-06*/) const
{
	rkInverse[0][0] = m_Values[1][1]*m_Values[2][2] -
		m_Values[1][2]*m_Values[2][1];
	rkInverse[0][1] = m_Values[0][2]*m_Values[2][1] -
		m_Values[0][1]*m_Values[2][2];
	rkInverse[0][2] = m_Values[0][1]*m_Values[1][2] -
		m_Values[0][2]*m_Values[1][1];
	rkInverse[1][0] = m_Values[1][2]*m_Values[2][0] -
		m_Values[1][0]*m_Values[2][2];
	rkInverse[1][1] = m_Values[0][0]*m_Values[2][2] -
		m_Values[0][2]*m_Values[2][0];
	rkInverse[1][2] = m_Values[0][2]*m_Values[1][0] -
		m_Values[0][0]*m_Values[1][2];
	rkInverse[2][0] = m_Values[1][0]*m_Values[2][1] -
		m_Values[1][1]*m_Values[2][0];
	rkInverse[2][1] = m_Values[0][1]*m_Values[2][0] -
		m_Values[0][0]*m_Values[2][1];
	rkInverse[2][2] = m_Values[0][0]*m_Values[1][1] -
		m_Values[0][1]*m_Values[1][0];

	Real fDet =
		m_Values[0][0]*rkInverse[0][0] +
		m_Values[0][1]*rkInverse[1][0]+
		m_Values[0][2]*rkInverse[2][0];

	if ( fabs(fDet) <= fTolerance )
		return false;

	Real fInvDet = 1.0/fDet;
	for (size_t iRow = 0; iRow < 3; iRow++)
	{
		for (size_t iCol = 0; iCol < 3; iCol++)
			rkInverse[iRow][iCol] *= fInvDet;
	}

	return true;
}

Matrix3x3 Matrix3x3::Inverse(Real fTolerance /*= 1e-06*/) const
{
	//ÓÐ±ÀÀ££¿
	Matrix3x3 kInverse;
	kInverse.MakeIdentity();
	Inverse(kInverse,fTolerance);
	return kInverse;
}

void Matrix3x3::MakeIdentity()
{
	memset(m_adfValues,0,sizeof(Real)*9);
	m_adfValues[0] = 1;
	m_adfValues[4] = 1;
	m_adfValues[8] = 1;
}

Matrix3x3 Matrix3x3::Transpose() const
{
	return Matrix3x3(m_Values[0][0],m_Values[1][0],m_Values[2][0],
		m_Values[0][1],m_Values[1][1],m_Values[2][1],
		m_Values[0][2],m_Values[1][2],m_Values[2][2]);
}

NS_MATH3D_END
