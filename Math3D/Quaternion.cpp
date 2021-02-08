

#include <assert.h>
#include <math.h>

#include "Quaternion.h"
#include "MathUtil.h"
#include "Vector3.h"
#include "Matrix3x3.h"

NS_MATH3D_BEGIN


Quaternion::Quaternion()
{
	m_dfW = 1;
	m_dfX = 0;
	m_dfY = 0;
	m_dfZ = 0;
}

Quaternion::Quaternion(Real w,Real x,Real y,Real z)
{
	m_dfW = w;
	m_dfX = x;
	m_dfY = y;
	m_dfZ = z;
}

Quaternion::Quaternion(const Quaternion& rhs)
{
	m_dfW = rhs.m_dfW;
	m_dfX = rhs.m_dfX;
	m_dfY = rhs.m_dfY;
	m_dfZ = rhs.m_dfZ;
}

Quaternion& Quaternion::operator =(const Quaternion& rhs)
{
	if (&rhs == this)
	{
		return *this;
	}
	m_dfW = rhs.m_dfW;
	m_dfX = rhs.m_dfX;
	m_dfY = rhs.m_dfY;
	m_dfZ = rhs.m_dfZ;

	return *this;
}

Vector3 Quaternion::operator* (const Vector3& v) const
{
    // nVidia SDK implementation
    Vector3 uv, uuv;
    Vector3 qvec(m_dfX, m_dfY, m_dfZ);
    
    uv = CrossProduct(uv,v);
    uuv = CrossProduct(uuv, uv);
    uv *= (2.0 * m_dfW);
    uuv *= 2.0;
    
    return v + uv + uuv;
    
}

Quaternion::~Quaternion()
{
    m_dfW = 1;
    m_dfX = 0;
    m_dfY = 0;
    m_dfZ = 0;
}

void Quaternion::FromRotateMatrix(const Matrix3x3& kRot)
{
	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	// article "Quaternion Calculus and Fast Animation".

	Real fTrace = kRot[0][0]+kRot[1][1]+kRot[2][2];
	Real fRoot;

	if ( fTrace > 0.0 )
	{
		// |w| > 1/2, may as well choose w > 1/2
		fRoot = sqrt(fTrace + 1.0f);  // 2w
		m_dfW = 0.5f*fRoot;
		fRoot = 0.5f/fRoot;  // 1/(4w)
		m_dfX = (kRot[2][1]-kRot[1][2])*fRoot;
		m_dfY = (kRot[0][2]-kRot[2][0])*fRoot;
		m_dfZ = (kRot[1][0]-kRot[0][1])*fRoot;
	}
	else
	{
		// |w| <= 1/2
		static size_t s_iNext[3] = { 1, 2, 0 };
		size_t i = 0;
		if ( kRot[1][1] > kRot[0][0] )
			i = 1;
		if ( kRot[2][2] > kRot[i][i] )
			i = 2;
		size_t j = s_iNext[i];
		size_t k = s_iNext[j];

		fRoot = sqrt(kRot[i][i]-kRot[j][j]-kRot[k][k] + 1.0f);
		Real* apkQuat[3] = { &m_dfX, &m_dfY, &m_dfZ };
		*apkQuat[i] = 0.5f*fRoot;
		fRoot = 0.5f/fRoot;
		m_dfW = (kRot[k][j]-kRot[j][k])*fRoot;
		*apkQuat[j] = (kRot[j][i]+kRot[i][j])*fRoot;
		*apkQuat[k] = (kRot[k][i]+kRot[i][k])*fRoot;
	}
    
    //3D开发书籍
//    float fourw = kRot[0][0] + kRot[1][1] + kRot[2][2];
//    float fourx = kRot[0][0] - kRot[1][1] - kRot[2][2];
//    float foury = kRot[1][1] - kRot[0][0] - kRot[2][2];
//    float fourz = kRot[2][2] - kRot[0][0] - kRot[1][1];
//    
//    //寻找最大位置
//    unsigned char big = 0;
//    float fourb = fourw;
//    if (fourx > fourb)
//    {
//        fourb = fourx;
//        big = 1;
//    }
//    if (foury > fourb)
//    {
//        fourb = foury;
//        big = 2;
//    }
//    if (fourz > fourb)
//    {
//        fourb = fourz;
//        big = 3;
//    }
//    
//    //计算平方根和除法
//    float bigV = sqrt(fourb + 1.0) * 0.5;
//    float mult = 0.25/bigV;
//    
//    //根据解的位置计算出四元数
//    switch (big)
//    {
//        case 0:
//            m_dfW = bigV;
//            m_dfX = (kRot[1][2] - kRot[2][1]) * mult;
//            m_dfY = (kRot[2][0] - kRot[0][2]) * mult;
//            m_dfZ = (kRot[0][1] - kRot[1][0]) * mult;
//            break;
//            
//        case 1:
//            m_dfX = bigV;
//            m_dfW = (kRot[1][2] - kRot[2][1]) * mult;
//            m_dfY = (kRot[0][1] + kRot[1][0]) * mult;
//            m_dfZ = (kRot[2][0] + kRot[0][2]) * mult;
//            break;
//            
//        case 2:
//            m_dfY = bigV;
//            m_dfW = (kRot[2][0] - kRot[0][2]) * mult;
//            m_dfX = (kRot[0][1] + kRot[1][0]) * mult;
//            m_dfZ = (kRot[1][2] + kRot[2][1]) * mult;
//            break;
//            
//        case 3:
//            m_dfZ = bigV;
//            m_dfW = (kRot[0][1] - kRot[1][0]) * mult;
//            m_dfX = (kRot[2][0] + kRot[0][2]) * mult;
//            m_dfY = (kRot[1][2] + kRot[2][1]) * mult;
//            break;
//            
//        default:
//            break;
//    }
}

void Quaternion::ToRotateMatrix(Matrix3x3& mRot) const
{
	Real a1 = 1.0f - 2*m_dfY*m_dfY - 2*m_dfZ*m_dfZ;
	Real a2 = 2*m_dfX*m_dfY + 2*m_dfW*m_dfZ;
	Real a3 = 2*m_dfX*m_dfZ - 2*m_dfW*m_dfY;

	Real b1 = 2*m_dfX*m_dfY - 2*m_dfW*m_dfZ;
	Real b2 = 1.0f - 2*m_dfX*m_dfX - 2*m_dfZ*m_dfZ;
	Real b3 = 2*m_dfY*m_dfZ + 2*m_dfW*m_dfX;

	Real c1 = 2*m_dfX*m_dfZ + 2*m_dfW*m_dfY;
	Real c2 = 2*m_dfY*m_dfZ - 2*m_dfW*m_dfX;
	Real c3 = 1.0f - 2*m_dfX*m_dfX - 2*m_dfY*m_dfY;

	mRot.Init(a1,a2,a3,b1,b2,b3,c1,c2,c3);
    
//    Real fTx  = m_dfX+m_dfX;
//    Real fTy  = m_dfY+m_dfY;
//    Real fTz  = m_dfZ+m_dfZ;
//    Real fTwx = fTx*m_dfW;
//    Real fTwy = fTy*m_dfW;
//    Real fTwz = fTz*m_dfW;
//    Real fTxx = fTx*m_dfX;
//    Real fTxy = fTy*m_dfX;
//    Real fTxz = fTz*m_dfX;
//    Real fTyy = fTy*m_dfY;
//    Real fTyz = fTz*m_dfY;
//    Real fTzz = fTz*m_dfZ;
//    
//    mRot[0][0] = 1.0f-(fTyy+fTzz);
//    mRot[0][1] = fTxy-fTwz;
//    mRot[0][2] = fTxz+fTwy;
//    mRot[1][0] = fTxy+fTwz;
//    mRot[1][1] = 1.0f-(fTxx+fTzz);
//    mRot[1][2] = fTyz-fTwx;
//    mRot[2][0] = fTxz-fTwy;
//    mRot[2][1] = fTyz+fTwx;
//    mRot[2][2] = 1.0f-(fTxx+fTyy);
}

void Quaternion::FromAngleAxis(const Real fAngle,const Vector3& vecAxis)
{
	assert(fabs(vectorMag(vecAxis) - 1.0f) < .01f);
	Real fAng = fAngle * DEGTORAD;

	Real	thetaOver2 = fAng * 0.5f;
	Real	sinThetaOver2 = sin(thetaOver2);

	m_dfW = cos(thetaOver2);
	m_dfX = vecAxis.x * sinThetaOver2;
	m_dfY = vecAxis.y * sinThetaOver2;
	m_dfZ = vecAxis.z * sinThetaOver2;
}

void Quaternion::ToAngleAxis(Real& fAngle, Vector3& vecAxis) const
{
    Real fSqrLength = m_dfX*m_dfX + m_dfY*m_dfY + m_dfZ*m_dfZ;
    if ( fSqrLength > 0.0 )
    {
        fAngle = 2.0*acos(m_dfW);
        Real fInvLength = 1.0f / sqrt(fSqrLength);
        vecAxis.x = m_dfX*fInvLength;
        vecAxis.y = m_dfY*fInvLength;
        vecAxis.z = m_dfZ*fInvLength;
    }
    else
    {
        // angle is 0 (mod 2*pi), so any axis will do
        fAngle = 0.0;
        vecAxis.x = 1.0;
        vecAxis.y = 0.0;
        vecAxis.z = 0.0;
    }

    //下面的方法是鬼火引擎的方法
	/*const f32 scale = sqrtf(X*X + Y*Y + Z*Z);

	if (core::iszero(scale) || W > 1.0f || W < -1.0f)
	{
		angle = 0.0f;
		axis.X = 0.0f;
		axis.Y = 1.0f;
		axis.Z = 0.0f;
	}
	else
	{
		const f32 invscale = reciprocal(scale);
		angle = 2.0f * acosf(W);
		axis.X = X * invscale;
		axis.Y = Y * invscale;
		axis.Z = Z * invscale;
	}*/
}

Real Quaternion::Norm() const
{
	return m_dfX*m_dfX + m_dfY*m_dfY + m_dfZ*m_dfZ;
}

void Quaternion::SetToRotateAboutX(float theta) 
{
	float	thetaOver2 = theta * .5f;

	m_dfW = cos(thetaOver2);
	m_dfX = sin(thetaOver2);
	m_dfY = 0.0f;
	m_dfZ = 0.0f;
}

void Quaternion::SetToRotateAboutY(float theta) 
{
	float	thetaOver2 = theta * .5f;

	m_dfW = cos(thetaOver2);
	m_dfX = 0.0f;
	m_dfY = sin(thetaOver2);
	m_dfZ = 0.0f;
}

void Quaternion::SetToRotateAboutZ(float theta) 
{
	float	thetaOver2 = theta * .5f;

	m_dfW = cos(thetaOver2);
	m_dfX = 0.0f;
	m_dfY = 0.0f;
	m_dfZ = sin(thetaOver2);
}


//void Quaternion::SetToRotateObjectToInertial(const EulerAngles &orientation)
//{
//	float	sp, sb, sh;
//	float	cp, cb, ch;
//	sinCos(&sp, &cp, orientation.m_fPitch * 0.5f);
//	sinCos(&sb, &cb, orientation.m_fBank * 0.5f);
//	sinCos(&sh, &ch, orientation.m_fHeading * 0.5f);
//
//	m_dfW =  ch*cp*cb + sh*sp*sb;
//	m_dfX =  ch*sp*cb + sh*cp*sb;
//	m_dfY = -ch*sp*sb + sh*cp*cb;
//	m_dfZ = -sh*sp*cb + ch*cp*sb;
//}
//
//void Quaternion::SetToRotateInertialToObject(const EulerAngles &orientation)
//{
//
//	float	sp, sb, sh;
//	float	cp, cb, ch;
//	sinCos(&sp, &cp, orientation.m_fPitch * 0.5f);
//	sinCos(&sb, &cb, orientation.m_fBank * 0.5f);
//	sinCos(&sh, &ch, orientation.m_fHeading * 0.5f);
//
//	m_dfW =  ch*cp*cb + sh*sp*sb;
//	m_dfX = -ch*sp*cb - sh*cp*sb;
//	m_dfY =  ch*sp*sb - sh*cb*cp;
//	m_dfZ =  sh*sp*cb - ch*cp*sb;
//}

Quaternion Quaternion::operator *(const Quaternion &a) const 
{
	Quaternion result;

	result.m_dfW = m_dfW*a.m_dfW - m_dfX*a.m_dfX - m_dfY*a.m_dfY - m_dfZ*a.m_dfZ;
	result.m_dfX = m_dfW*a.m_dfX + m_dfX*a.m_dfW + m_dfZ*a.m_dfY - m_dfY*a.m_dfZ;
	result.m_dfY = m_dfW*a.m_dfY + m_dfY*a.m_dfW + m_dfX*a.m_dfZ - m_dfZ*a.m_dfX;
	result.m_dfZ = m_dfW*a.m_dfZ + m_dfZ*a.m_dfW + m_dfY*a.m_dfX - m_dfX*a.m_dfY;

	return result;
}

Quaternion &Quaternion::operator *=(const Quaternion &a) 
{
	*this = *this * a;

	return *this;
}

void Quaternion::Normalize() 
{
	float	mag = (float)sqrt(m_dfW*m_dfW + m_dfX*m_dfX + m_dfY*m_dfY + m_dfZ*m_dfZ);

	if (mag > 0.0f) 
	{
		float	oneOverMag = 1.0f / mag;
		m_dfW *= oneOverMag;
		m_dfX *= oneOverMag;
		m_dfY *= oneOverMag;
		m_dfZ *= oneOverMag;

	} 
	else 
	{
		assert(false);
	}
}

Real Quaternion::DotProduct(const Quaternion &b) const
{
	return m_dfW*b.m_dfW + m_dfX*b.m_dfX + m_dfY*b.m_dfY + m_dfZ*b.m_dfZ;
}

bool Quaternion::Inverse()
{
    Real n = m_dfX * m_dfX + m_dfY * m_dfY + m_dfZ * m_dfZ + m_dfW * m_dfW;
    if (n == 1.0f)
    {
        m_dfX = -m_dfX;
        m_dfY = -m_dfY;
        m_dfZ = -m_dfZ;
        
        return true;
    }
    
    // Too close to zero.
    if (n < 0.000001)
        return false;
    
    n = 1.0 / n;
    m_dfX = -m_dfX * n;
    m_dfY = -m_dfY * n;
    m_dfZ = -m_dfZ * n;
    m_dfW = m_dfW * n;
    
    return true;
}

Quaternion Quaternion::GetInversed() const
{
    Quaternion result(*this);
    result.Inverse();
    return result;
}

Quaternion Quaternion::Slerp(const Quaternion &p, const Quaternion &q, Real t)
{
	if (t <= 0.0f) return p;
	if (t >= 1.0f) return q;

	double cosOmega = p.DotProduct(q);

	double q1w = q.m_dfW;
	double q1x = q.m_dfX;
	double q1y = q.m_dfY;
	double q1z = q.m_dfZ;
	if (cosOmega < 0.0f) 
	{
		q1w = -q1w;
		q1x = -q1x;
		q1y = -q1y;
		q1z = -q1z;
		cosOmega = -cosOmega;
	}

	assert(cosOmega < 1.1f);

	double k0, k1;
	if (cosOmega > 0.999999f) 
	{

		k0 = 1.0f-t;
		k1 = t;

	} 
	else 
	{

		double sinOmega = sqrt(1.0f - cosOmega*cosOmega);
		double omega = atan2(sinOmega, cosOmega);
		double oneOverSinOmega = 1.0f / sinOmega;

		k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
		k1 = sin(t * omega) * oneOverSinOmega;
	}

	Quaternion result;
	result.m_dfX = k0*p.m_dfX + k1*q1x;
	result.m_dfY = k0*p.m_dfY + k1*q1y;
	result.m_dfZ = k0*p.m_dfZ + k1*q1z;
	result.m_dfW = k0*p.m_dfW + k1*q1w;

	return result;
}

Quaternion Quaternion::Conjugate(const Quaternion &q) 
{
	Quaternion result;

	result.m_dfW = q.m_dfW;

	result.m_dfX = -q.m_dfX;
	result.m_dfY = -q.m_dfY;
	result.m_dfZ = -q.m_dfZ;

	return result;
}


Quaternion Quaternion::Pow(const Quaternion &q, float fExponent)
{
	if (fabs(q.m_dfW) > 0.9999f) 
	{
		return q;
	}

	double	alpha = acos(q.m_dfW);
	double	newAlpha = alpha * fExponent;

	Quaternion result;
	result.m_dfW = cos(newAlpha);

	double	mult = sin(newAlpha) / sin(alpha);
	result.m_dfX = q.m_dfX * mult;
	result.m_dfY = q.m_dfY * mult;
	result.m_dfZ = q.m_dfZ * mult;

	return result;
}

NS_MATH3D_END
