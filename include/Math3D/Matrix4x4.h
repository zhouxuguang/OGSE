#ifndef MATH3DLIB_MATRIX440_INCLUDE_04DDDDD
#define MATH3DLIB_MATRIX440_INCLUDE_04DDDDD

#include "Matrix3x3.h"

NS_MATH3D_BEGIN

//3D�任����

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

	//���س˷�������

	Vector3 operator * ( const Vector3 &v ) const;
	Vector4 operator * (const Vector4& other);

	Matrix4x4 operator * (const Matrix4x4& other);

	Matrix4x4& operator *= (const Matrix4x4& other);


	//��������Ϊ��λ����
	void MakeIdentity();

	//�Ƿ�Ϊ��λ����
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

	//�����������ʽ
	Real Determinant() const;

	Matrix4x4 Inverse() const;

	//����ת��
	Matrix4x4 Transpose() const;
    
    //���3�׾���
    Matrix3x3 GetMatrix3() const;
    
    //��̬��������Ҫ�Ǵ������ֱ任����
    
    /**
     *  ����͸��ͶӰ����
     *
     *  @param fieldOfView �ӳ��Ǵ�С
     *  @param aspectRatio ��߱�
     *  @param zNearPlane  ��ƽ��
     *  @param zFarPlane   Զƽ��
     *  @param dst         Ŀ�����
     */
    static void CreatePerspective(Real fieldOfView, Real aspectRatio, Real zNearPlane, Real zFarPlane, Matrix4x4& dst);
    
    /**
     *  ����͸��ͶӰ����
     *
     *  @param left       ��߽߱�
     *  @param right      �ұ߽߱�
     *  @param bottom     �ײ��߽�
     *  @param top        �ϲ��߽�
     *  @param zNearPlane ��ƽ��
     *  @param zFarPlane  Զƽ��
     *  @param dst        Ŀ�����
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
     *  ��������ͶӰ����
     *
     *  @param left       ��߽߱�
     *  @param right      �ұ߽߱�
     *  @param bottom     �ײ��߽�
     *  @param top        �ϲ��߽�
     *  @param zNearPlane ��ƽ��
     *  @param zFarPlane  Զƽ��
     *  @param dst        Ŀ�����
     */
    static void CreateOrthographic(Real left, Real right, Real bottom, Real top,
                                            Real zNearPlane, Real zFarPlane, Matrix4x4& dst);
    
    //���ž���
    static void CreateScale(const Vector3& scale, Matrix4x4& dst);
    
    static void CreateScale(Real xScale, Real yScale, Real zScale, Matrix4x4& dst);
    
    //��ת����
    static void CreateRotation(const Vector3& axis, Real angle, Matrix4x4& dst);
    
    static void CreateRotationX(Real angle, Matrix4x4& dst);
    
    static void CreateRotationY(Real angle, Matrix4x4& dst);
    
    static void CreateRotationZ(Real angle, Matrix4x4& dst);
    
    static void CreateRotation(Real x, Real y, Real z,Real angle, Matrix4x4& dst);
    
    //ƽ�ƾ���
    static void CreateTranslate(const Vector3& dist, Matrix4x4& dst);
    
    static void CreateTranslate(Real x, Real y, Real z, Matrix4x4& dst);
    
    //��ͼ����
    static void CreateLookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& up, Matrix4x4& dst);
    
    static void CreateLookAt(Real eyePositionX, Real eyePositionY, Real eyePositionZ,
                             Real targetCenterX, Real targetCenterY, Real targetCenterZ,
                             Real upX, Real upY, Real upZ, Matrix4x4& dst);

private:

	//����Ԫ��
	union
	{
		Real m_adfValues[16];
		Real m_pValue[4][4];
	};
    
    //�������
public:
    static const Matrix4x4 IDENTITY;
    static const Matrix4x4 ZERO;
	
};

NS_MATH3D_END


#endif
