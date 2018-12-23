#ifndef MATH3D_AXISALIGNEDBOX_INCLUDE_H
#define MATH3D_AXISALIGNEDBOX_INCLUDE_H

#include "Vector3.h"

NS_MATH3D_BEGIN

class Plane3D;
class Sphere;

class MATH3D_API AxisAlignedBox
{
public:
	/*
	   1-----2
	  /|    /|
	 / |   / |
	5-----4  |
	|  0--|--3
	| /   | /
	|/    |/
	6-----7
	*/
	enum Corner
	{
		FAR_LEFT_BOTTOM = 0,
		FAR_LEFT_TOP = 1,
		FAR_RIGHT_TOP = 2,
		FAR_RIGHT_BOTTOM = 3,
		NEAR_RIGHT_BOTTOM = 7,
		NEAR_LEFT_BOTTOM = 6,
		NEAR_LEFT_TOP = 5,
		NEAR_RIGHT_TOP = 4
	} ;

	//构造函数
	AxisAlignedBox(void);

	AxisAlignedBox( const Vector3& min, const Vector3& max );

	AxisAlignedBox& operator=(const AxisAlignedBox& rhs);

	~AxisAlignedBox(void);

	//成员函数
	void SetMinimum( const Vector3& vec );

	void SetMinimum( Real x, Real y, Real z );

	void SetMinimumX(Real x);

	void SetMinimumY(Real y);

	void SetMinimumZ(Real z);

	void SetMaximum( const Vector3& vec );

	void SetMaximum( Real x, Real y, Real z );

	void SetMaximumX( Real x );

	void SetMaximumY( Real y );

	void SetMaximumZ( Real z );

	//设置范围
	void SetExtent( const Vector3& min, const Vector3& max );

	void SetExtent(Real mx, Real my, Real mz,Real Mx, Real My, Real Mz );

	const Vector3& GetMinimum(void) const;

	Vector3& GetMinimum(void);

	const Vector3& GetMaximum(void) const;

	Vector3& GetMaximum(void);

	Vector3 GetCenter(void) const;

	Vector3 GetCorner(Corner cornerToGet) const;

	//判断是否为空
	bool IsNull() const;

	//将AABB设置为空
	void SetNull();

	//扩张
	void Merge(const Vector3& vkPoint);

	void Merge(const AxisAlignedBox& vkBox);

	//包含和相交测试

	bool Contains(const Vector3 &vkPoint) const;

	bool Contains(const AxisAlignedBox &vkBox) const;

	bool Intersects(const AxisAlignedBox& b2) const;

	bool Intersects(const Plane3D& p) const;

	bool Intersects(const Sphere& s) const;

private:
	Vector3 mMinimum;		//最小顶点
	Vector3 mMaximum;		//最大顶点
};

NS_MATH3D_END

#endif
