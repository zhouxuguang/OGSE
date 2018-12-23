#ifndef MATH3D_RAY3D_INCLUDE_H
#define MATH3D_RAY3D_INCLUDE_H

#include "Vector3.h"

//射线定义

NS_MATH3D_BEGIN

class MATH3D_API Ray3D
{
public:
	Ray3D(void);

	Ray3D(const Vector3& origin,const Vector3& direction);

	Ray3D(const Ray3D& ray);

	Ray3D& operator = (const Ray3D& ray);

	~Ray3D(void);

	void SetOrigin(const Vector3& origin); 

	const Vector3& GetOrigin(void) const; 

	void SetDirection(const Vector3& dir); 

	const Vector3& SetDirection(void) const; 

	Vector3 GetPoint(Real t) const;


public:
	Vector3 mOrigin;		//原点
	Vector3 mDirection;		//方向
};

NS_MATH3D_END

#endif
