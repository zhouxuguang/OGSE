#ifndef MATH3D_GEOMETRYOBJECT_INCLUDE_H_
#define MATH3D_GEOMETRYOBJECT_INCLUDE_H_


#include "Vector3.h"
#include "Ray3D.h"
#include "ShadeRec.h"
#include "ColorRGBA.h"

NS_MATH3D_BEGIN

class Material;	

class GeometryObject 
{	
public:	

	GeometryObject(void);									

	GeometryObject(const GeometryObject& object);			

	virtual GeometryObject*	Clone(void) const {return NULL;}

	virtual ~GeometryObject(void);	

	virtual bool Hit(const Ray3D& ray, double& t, ShadeRec& s) const = 0;	

	/*virtual void set_material(Material* mPtr); 			

	Material* get_material(void) const;*/


	// The following three functions are only required for Chapter 3

	//void
	//	set_color(const RGBColor& c);

	//void
	//	set_color(const float r, const float g, const float b);

	//RGBColor
	//	get_color(void);

	//virtual void 
	//	set_bounding_box(void);

	//virtual BBox 
	//	get_bounding_box(void);

	//virtual void 										// required for compound objects
	//	add_object(GeometricObject* object_ptr);


	// The following two functions are only required for objects that are light sources, eg disks, rectangles, and spheres

	/*virtual Point3D 		
		sample(void);

	virtual float
		pdf(ShadeRec& sr); */


	//// The following two functions allow us to simplify the code for smooth shaded triangle meshes

	//virtual Normal
	//	get_normal(void) const; 

	//virtual Normal
	//	get_normal(const Point3D& p); 


protected:

	mutable Material*   material_ptr;   	// mutable allows the const functions Compound::hit, Instance::hit, and RegularGrid::hit to assign to material_ptr
	ColorRGBA   color;				// only used for Bare Bones ray tracing

	GeometryObject&	operator= (const GeometryObject& rhs);
};


// --------------------------------------------------------------------  set_color

//inline void
//GeometricObject::set_color(const RGBColor& c) {
//	color = c;
//}
//
//// --------------------------------------------------------------------  set_color
//
//inline void 
//GeometricObject::set_color(const float r, const float g, const float b) {
//	color.r = r;
//	color.b = b;
//	color.g = g;
//}
//
//// --------------------------------------------------------------------  get_color
//
//inline RGBColor 
//GeometricObject::get_color(void) {
//	return (color);
//}


NS_MATH3D_END


#endif
