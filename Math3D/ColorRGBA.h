#ifndef MATH3D_COLORRGBA_INCLUDE_H_4FG56FL
#define MATH3D_COLORRGBA_INCLUDE_H_4FG56FL

#include "Math3DCommon.h"

NS_MATH3D_BEGIN

class MATH3D_API ColorRGBA
{
public:
	ColorRGBA ();

	ColorRGBA (float fR, float fG, float fB,float fA = 1.0f);

	ColorRGBA (float afTuple[4]);

	~ColorRGBA(void);

	//operator获得内部缓冲
	operator const float* () const;
	operator float* ();

	//重载获得分量
	float operator[] (int i) const;
	float& operator[] (int i);

	//重载加减乘除
	ColorRGBA operator *(float kScale) const;

	ColorRGBA operator /(float kScale) const;

	ColorRGBA operator -() const; 

	ColorRGBA operator + (const ColorRGBA& other) const;

	ColorRGBA operator - (const ColorRGBA& other) const;

	ColorRGBA operator = (const ColorRGBA& other) const;

	friend ColorRGBA operator*(const ColorRGBA &a,const ColorRGBA &b);

	ColorRGBA& operator += (const ColorRGBA& other);

	//member func
	float GetR () const;
	float& GetR ();
	float GetG () const;
	float& GetG ();
	float GetB () const;
	float& GetB ();
	float GetA () const;
	float& GetA ();

	ColorRGBA Multiply(const ColorRGBA& color) const;

private:
	mutable float m_pfValues[4];			//RGB顺序

	void SetRGBA(float fRed,float fGreen,float fBlue,float fAlpha);

public:
	static const ColorRGBA& ZERO;		//四个分量都是0
	static const ColorRGBA& Black;		//黑色
	static const ColorRGBA& White;		//白色
	static const ColorRGBA& Red;			//红色
	static const ColorRGBA& Green;		//绿色
	static const ColorRGBA& Blue;		//蓝色
};

NS_MATH3D_END

#endif
