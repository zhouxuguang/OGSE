#ifndef MATH3D_TRIANGLE3D_INCLUDE_H_
#define MATH3D_TRIANGLE3D_INCLUDE_H_

#include "Plane3D.h"

NS_MATH3D_BEGIN

//三维三角形，可用于碰撞检测
class MATH3D_API Triangle3D
{
public:
	Triangle3D(void);

	Triangle3D(const Vector3& v1, const Vector3& v2, const Vector3& v3);

	~Triangle3D(void);

	//获得法向量，获得的法向量不一定是单位向量
	Vector3 GetNormal(void) const;

	//获得三角形所在的平面
	Plane3D GetPlane(void) const;

	//计算面积
	Real GetArea(void) const;

	//判断点是否在三角形内，需要注意的是点必须和三角形在同一个平面内
	bool IsPointInside(const Vector3& p) const;

	Vector3 mPointA;
	Vector3 mPointB;
	Vector3 mPointC;
};

NS_MATH3D_END

#endif
