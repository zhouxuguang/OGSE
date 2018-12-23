
#ifndef AFX_MATERIAL_H__1161180E_9A92_4DAE_AB2E_757671AB6E12__INCLUDED_
#define AFX_MATERIAL_H__1161180E_9A92_4DAE_AB2E_757671AB6E12__INCLUDED_

#include "ColorRGBA.h"

NS_MATH3D_BEGIN

class MATH3D_API Material  
{
public:
	Material();

	~Material();

	void SetAmbient(const ColorRGBA&);//设置材质对环境光的反射率

	void SetDiffuse(const ColorRGBA&);//设置材质对漫反射光的反射率

	void SetSpecular(const ColorRGBA&);//设置材质对镜面反射光的反射率

	void SetEmit(const ColorRGBA&);//设置材质自身辐射的颜色

	void SetExp(Real);//设置材质的高光指数	
public:
	ColorRGBA M_Ambient;//材质对环境光的反射率
	ColorRGBA M_Diffuse;//材质对漫反射光的反射率
	ColorRGBA M_Specular;//材质对镜面反射光的反射率
	ColorRGBA M_Emit;//材质自身辐射的颜色
	Real M_n;//材质的高光指数
};

NS_MATH3D_END

#endif // 
