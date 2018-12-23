#ifndef __MATH3D_SHADE_REC_INCLUDE_H__
#define __MATH3D_SHADE_REC_INCLUDE_H__

class Material;
class World;

#include "Ray3D.h"
#include "ColorRGBA.h"

NS_MATH3D_BEGIN

class ShadeRec 
{
	public:
	
		bool				hit_an_object;		// did the ray hit an object?
		Material* 			material_ptr;		// pointer to the nearest object's material
		Vector3 			hit_point;			// world coordinates of intersection
		Vector3				local_hit_point;	// world coordinates of hit point on untransformed object (used for texture transformations)
		Vector3				normal;				// normal at hit point
		Ray3D				ray;				// required for specular highlights and area lights
		int					depth;				// recursion depth
		ColorRGBA			color;				// used in the Chapter 3 only
		double				t;					// ray parameter
		float				u;					// texture coordinate
		float				v;					// texture coordinate
		World&				w;					// world reference
				
		ShadeRec(World& wr);					// constructor
		ShadeRec(const ShadeRec& sr);			// copy constructor
		~ShadeRec(void);						// destructor
};

NS_MATH3D_END

#endif
