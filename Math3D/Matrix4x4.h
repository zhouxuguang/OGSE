#ifndef MATH3DLIB_MATRIX440_INCLUDE_04DDDDD
#define MATH3DLIB_MATRIX440_INCLUDE_04DDDDD

#include "Matrix3x3.h"

NS_MATH3D_BEGIN

//3D变换矩阵

class Vector3;
class Vector4;

class MATH3D_API Matrix4x4
{
public:
	Matrix4x4(void);

	Matrix4x4(
		Real m00, Real m01, Real m02, Real m03,
		Real m10, Real m11, Real m12, Real m13,
		Real m20, Real m21, Real m22, Real m23,
		Real m30, Real m31, Real m32, Real m33 );

	explicit Matrix4x4(Real *pfMatValues);

	Matrix4x4(const Matrix4x4& rtMat);

	~Matrix4x4(void);

	Matrix4x4 &operator = (const Matrix4x4& rhs);

	Matrix4x4 operator +(double fValue);

	Matrix4x4 operator -(double fValue);

	Matrix4x4 operator *(double fValue);

	Matrix4x4 operator /(double fValue);

	inline const Real* operator [](size_t nIndex) const
	{
		return m_pValue[nIndex];
	}

	inline Real* operator [](size_t nIndex)
	{
		return m_pValue[nIndex];
	}

	//重载乘法操作符

	Vector3 operator * ( const Vector3 &v ) const;
	Vector4 operator * (const Vector4& other);

	Matrix4x4 operator * (const Matrix4x4& other);

	Matrix4x4& operator *= (const Matrix4x4& other);


	//将矩阵置为单位矩阵
	void MakeIdentity();

	//是否为单位矩阵
	bool IsIdentity() const;

	Real *GetValues()
	{
		return m_adfValues;
	}

	const Real* GetValues() const
	{
		return m_adfValues;
	}

	void SetValues(Real* pfValues)
	{
		memcpy(m_adfValues,pfValues,sizeof(Real)*16);
	}

	//计算矩阵行列式
	Real Determinant() const;

	Matrix4x4 Inverse() const;

	//矩阵转置
	Matrix4x4 Transpose() const;
    
    //获得3阶矩阵
    Matrix3x3 GetMatrix3() const;
    
    //静态函数，主要是创建各种变换矩阵
    
    /**
     *  创建透视投影矩阵
     *
     *  @param fieldOfView 视场角大小
     *  @param aspectRatio 宽高比
     *  @param zNearPlane  近平面
     *  @param zFarPlane   远平面
     *  @param dst         目标矩阵
     */
    static void CreatePerspective(Real fieldOfView, Real aspectRatio, Real zNearPlane, Real zFarPlane, Matrix4x4& dst);
    
    /**
     *  创建透视投影矩阵
     *
     *  @param left       左边边界
     *  @param right      右边边界
     *  @param bottom     底部边界
     *  @param top        上部边界
     *  @param zNearPlane 近平面
     *  @param zFarPlane  远平面
     *  @param dst        目标矩阵
     */
    static void CreateFrustum(Real left, Real right, Real bottom, Real top, Real zNearPlane, Real zFarPlane,Matrix4x4& dst);
    
    
    /**
     Description

     @param width width description
     @param height height description
     @param zNearPlane zNearPlane description
     @param zFarPlane zFarPlane description
     @param dst dst description
     */
    static void CreateOrthographic(Real width, Real height, Real zNearPlane, Real zFarPlane, Matrix4x4* dst);
    
    /**
     *  创建正射投影矩阵
     *
     *  @param left       左边边界
     *  @param right      右边边界
     *  @param bottom     底部边界
     *  @param top        上部边界
     *  @param zNearPlane 近平面
     *  @param zFarPlane  远平面
     *  @param dst        目标矩阵
     */
    static void CreateOrthographic(Real left, Real right, Real bottom, Real top,
                                            Real zNearPlane, Real zFarPlane, Matrix4x4& dst);
    
    //缩放矩阵
    static void CreateScale(const Vector3& scale, Matrix4x4& dst);
    
    static void CreateScale(Real xScale, Real yScale, Real zScale, Matrix4x4& dst);
    
    //旋转矩阵
    static void CreateRotation(const Vector3& axis, Real angle, Matrix4x4& dst);
    
    static void CreateRotationX(Real angle, Matrix4x4& dst);
    
    static void CreateRotationY(Real angle, Matrix4x4& dst);
    
    static void CreateRotationZ(Real angle, Matrix4x4& dst);
    
    static void CreateRotation(Real x, Real y, Real z,Real angle, Matrix4x4& dst);
    
    //平移矩阵
    static void CreateTranslate(const Vector3& dist, Matrix4x4& dst);
    
    static void CreateTranslate(Real x, Real y, Real z, Matrix4x4& dst);
    
    //视图矩阵
    static void CreateLookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& up, Matrix4x4& dst);
    
    static void CreateLookAt(Real eyePositionX, Real eyePositionY, Real eyePositionZ,
                             Real targetCenterX, Real targetCenterY, Real targetCenterZ,
                             Real upX, Real upY, Real upZ, Matrix4x4& dst);

private:

	//矩阵元素
	union
	{
		Real m_adfValues[16];
		Real m_pValue[4][4];
	};
    
    //特殊矩阵
public:
    static const Matrix4x4 IDENTITY;
    static const Matrix4x4 ZERO;
	
};

NS_MATH3D_END


#endif
