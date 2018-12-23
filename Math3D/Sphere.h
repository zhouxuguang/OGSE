#ifndef MATH3D_SPHERE_INCLUDE_H_
#define MATH3D_SPHERE_INCLUDE_H_

#include "Vector3.h"
#include "GeometryObject.h"

NS_MATH3D_BEGIN

class AxisAlignedBox;
class Plane3D;

class MATH3D_API Sphere //: public GeometryObject
{
public:
	Sphere(void);

	Sphere(const Vector3& center, Real radius);

	~Sphere(void);

	Real GetRadius(void) const;

	void SetRadius(Real radius);

	const Vector3& GetCenter(void) const;

	void SetCenter(const Vector3& center);

	bool Intersects(const Sphere& s) const;
	
	bool Intersects(const AxisAlignedBox& box) const;
	
	bool Intersects(const Plane3D& plane) const;
	
	bool Intersects(const Vector3& v) const;
	
	void Merge(const Sphere& oth);

	//¼Ì³Ðº¯Êý
	bool Hit(const Ray3D& ray, double& t, ShadeRec& s) const ;

public:
	Real mRadius;			//°ë¾¶
	Vector3 mCenter;		//ÇòÐÄ

	static const Real kEpsilon;
};

NS_MATH3D_END

#endif
