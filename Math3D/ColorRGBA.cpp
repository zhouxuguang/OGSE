#include "ColorRGBA.h"
#include "MathUtil.h"

NS_MATH3D_BEGIN

//ÌØÊâÑÕÉ«¶¨Òå
const ColorRGBA& ColorRGBA::ZERO = ColorRGBA(0,0,0,0);
const ColorRGBA& ColorRGBA::Black = ColorRGBA(0.0,0.0,0.0);
const ColorRGBA& ColorRGBA::White = ColorRGBA(1.0,1.0,1.0);
const ColorRGBA& ColorRGBA::Red = ColorRGBA(1.0,0.0,0.0);
const ColorRGBA& ColorRGBA::Green = ColorRGBA(0.0,1.0,0.0);
const ColorRGBA& ColorRGBA::Blue = ColorRGBA(0.0,0.0,1.0);


ColorRGBA::~ColorRGBA(void)
{
}

ColorRGBA::ColorRGBA(float fR, float fG, float fB,float fA)
{
	SetRGBA(fR,fG,fB,fA);
}

ColorRGBA::ColorRGBA(float afTuple[4])
{
	memcpy(m_pfValues,afTuple,sizeof(float)*4);
	//SetRGBA(afTuple[0],afTuple[1],afTuple[2],afTuple[3]);
}

ColorRGBA::ColorRGBA()
{
	//ºì
	m_pfValues[0] = 0;

	//ÂÌ
	m_pfValues[1] = 0;

	//À¶
	m_pfValues[2] = 0;

	//Alpha
	m_pfValues[3] = 1;
}

ColorRGBA::operator const float*() const
{
	return m_pfValues;
}

ColorRGBA::operator float*()
{
	return m_pfValues;
}

float ColorRGBA::operator[](int i) const
{
	assert(i >=0 && i < 4);
	return m_pfValues[i];
}

float& ColorRGBA::operator[](int i)
{
	assert(i >=0 && i < 4);
	return m_pfValues[i];
}

float ColorRGBA::GetR() const
{
	return m_pfValues[0];
}

float& ColorRGBA::GetR()
{
	return m_pfValues[0];
}

float ColorRGBA::GetG() const
{
	return m_pfValues[1];
}

float& ColorRGBA::GetG()
{
	return m_pfValues[1];
}

float ColorRGBA::GetB() const
{
	return m_pfValues[2];
}

float& ColorRGBA::GetB()
{
	return m_pfValues[2];
}

float ColorRGBA::GetA() const
{
	return m_pfValues[3];
}

float& ColorRGBA::GetA()
{
	return m_pfValues[3];
}

void ColorRGBA::SetRGBA(float fRed,float fGreen,float fBlue,float fAlpha)
{
	//ºì
	m_pfValues[0] = GetClamp(fRed,0,1);

	//ÂÌ
	m_pfValues[1] = GetClamp(fGreen,0,1);

	//À¶
	m_pfValues[2] = GetClamp(fBlue,0,1);

	//Alpha
	m_pfValues[3] = GetClamp(fAlpha,0,1);

}

ColorRGBA ColorRGBA::operator+(const ColorRGBA& other) const
{
	return ColorRGBA(std::min(m_pfValues[0] + other.m_pfValues[0], 1.0f),
		std::min(m_pfValues[1] + other.m_pfValues[1], 1.0f),
		std::min(m_pfValues[2] + other.m_pfValues[2], 1.0f),
		std::min(m_pfValues[3] + other.m_pfValues[3], 1.0f));
}

ColorRGBA ColorRGBA::operator-(const ColorRGBA& other) const
{
	return ColorRGBA(std::max(m_pfValues[0] - other.m_pfValues[0], 0.0f),
		std::max(m_pfValues[1] - other.m_pfValues[1], 0.0f),
		std::max(m_pfValues[2] - other.m_pfValues[2], 0.0f),
		std::max(m_pfValues[3] - other.m_pfValues[3], 0.0f));
}

ColorRGBA ColorRGBA::operator*(float kScale) const
{
	return ColorRGBA(m_pfValues[0]*kScale,m_pfValues[1]*kScale,m_pfValues[2]*kScale,m_pfValues[3]*kScale);
}

ColorRGBA ColorRGBA::operator/(float kScale) const
{
	return ColorRGBA(m_pfValues[0]/kScale,m_pfValues[1]/kScale,m_pfValues[2]/kScale,m_pfValues[3]/kScale);
}

ColorRGBA ColorRGBA::operator-() const
{
	return ColorRGBA(-m_pfValues[0],-m_pfValues[1],-m_pfValues[2],-m_pfValues[3]);
}

ColorRGBA ColorRGBA::operator=(const ColorRGBA& other) const
{
	return ColorRGBA(m_pfValues[0],m_pfValues[1],m_pfValues[2],m_pfValues[3]);
}

ColorRGBA ColorRGBA::Multiply(const ColorRGBA& color) const
{
	return ColorRGBA(m_pfValues[0]*color.m_pfValues[0],m_pfValues[1]*color.m_pfValues[1],
		m_pfValues[2]*color.m_pfValues[2],m_pfValues[3]*color.m_pfValues[3]);
}

ColorRGBA& ColorRGBA::operator+=(const ColorRGBA& other)
{
	m_pfValues[0] += other.m_pfValues[0];
	m_pfValues[1] += other.m_pfValues[1];
	m_pfValues[2] += other.m_pfValues[2];
	m_pfValues[3] += other.m_pfValues[3];
	return *this;
}

//ÖØÔØ²Ù×÷·û
ColorRGBA operator*(const ColorRGBA &a,const ColorRGBA &b)
{
	Real rValue = a.m_pfValues[0]*b.m_pfValues[0];
	Real gValue = a.m_pfValues[1]*b.m_pfValues[1];
	Real bValue = a.m_pfValues[2]*b.m_pfValues[2];
	Real aValue = a.m_pfValues[3]*b.m_pfValues[3];
	return ColorRGBA(rValue,gValue,bValue,aValue);
}

NS_MATH3D_END
