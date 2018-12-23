#ifndef MATH3D_PLANE3D_INCLUDE_H_02FG458HK6T5UIK
#define MATH3D_PLANE3D_INCLUDE_H_02FG458HK6T5UIK

#include "Vector3.h"
#include "GeometryObject.h"

NS_MATH3D_BEGIN

//三维平面类的封装 Ax + By + Cz + D = 0

class MATH3D_API Plane3D 
{
public:

	enum Side
	{
		NEGATIVE_SIDE,//正面
		POSITIVE_SIDE,//反
		NO_SIDE
	};

	Plane3D(const Vector3& rkNormal,Real fConstant);

	Plane3D(Real a,Real b,Real c,Real d);

	Plane3D(const Vector3& rkPoint1, const Vector3& rkPoint2,
		const Vector3& rkPoint3);

	Plane3D(const Plane3D& rhs);

	~Plane3D(void);

	Plane3D& operator = (const Plane3D& rhs);

	//求点到平面的距离
	Real GetDistance(const Vector3& rkPoint) const;

	//判断点和平面的关系
	Plane3D::Side GetSide(const Vector3& rkPoint) const;

	//用三个不共线的点定义平面方程
	void ReDefine(const Vector3& rkPoint1, const Vector3& rkPoint2,const Vector3& rkPoint3);

	//规范化
	Real Normalise(void);

	//继承来的函数

	bool Hit(const Ray3D& ray, double& t, ShadeRec& s) const;

private:
	Vector3 m_Normal;		//法线向量
	Real m_fD;				//距离

	static const Real kEpsilon;
};

NS_MATH3D_END

#endif
