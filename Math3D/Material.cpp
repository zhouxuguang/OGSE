
#include "Material.h"

NS_MATH3D_BEGIN

Material::Material()
{
	M_Ambient = ColorRGBA(0.2,0.2,0.2);//材质对环境光的反射率
	M_Diffuse = ColorRGBA(0.8,0.8,0.8);//材质对漫反射光的反射率
	M_Specular = ColorRGBA(0.0,0.0,0.0);//材质对镜面反射光的反射率
	M_Emit = ColorRGBA(0.0,0.0,0.0);//材质自身发散的颜色
	M_n = 1.0;//高光指数
}

Material::~Material()
{

}

void Material::SetAmbient(const ColorRGBA& c)
{
	M_Ambient = c;
}

void Material::SetDiffuse(const ColorRGBA& c)
{
	M_Diffuse = c;
}

void Material::SetSpecular(const ColorRGBA& c)
{
	M_Specular = c;
}

void Material::SetEmit(const ColorRGBA& emi)
{
	M_Emit = emi;
}

void Material::SetExp(Real n)
{
	M_n = n;
}

NS_MATH3D_END