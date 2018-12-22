#include "SysRGBColor.h"

namespace SysDraw
{
	static float GetClamp(float x,float fMin,float fMax)
	{
		if (x < fMin)
		{
			x = fMin;
		}

		else if (x > fMax)
		{
			x = fMax;
		}

		return x;
	}

	SysRGBAColor::SysRGBAColor()
	{
		m_fRed = 0.0f;
		m_fGreen = 0.0f;
		m_fBlue = 0.0f;
		m_fAlpha = 1.0f;
	}

	SysRGBAColor::SysRGBAColor(float fRed,float fGreen,float fBlue,float fAlpha)
	{
		SetRGBA(fRed,fGreen,fBlue,fAlpha);
	}

	SysRGBAColor::SysRGBAColor( const SysRGBAColor& rhs )
	{
		SetRGBA(rhs.m_fRed,rhs.m_fGreen,rhs.m_fBlue,rhs.m_fAlpha);
	}

	SysRGBAColor& SysRGBAColor::operator =(const SysRGBAColor& rhs)
	{
		SetRGBA(rhs.m_fRed,rhs.m_fGreen,rhs.m_fBlue,rhs.m_fAlpha);

		return *this;
	}

	SysRGBAColor::~SysRGBAColor(void)
	{
	}

	float SysRGBAColor::GetRed() const
	{
		return m_fRed;
	}

	float SysRGBAColor::GetGreen() const
	{
		return m_fGreen;
	}

	float SysRGBAColor::GetBlue() const
	{
		return m_fBlue;
	}

	float SysRGBAColor::GetAlpha() const
	{
		return m_fAlpha;
	}

	void SysRGBAColor::SetRGBA(float fRed,float fGreen,float fBlue ,float fAlpha)
	{
		//╨Л
		m_fRed = GetClamp(fRed,0,1);

		//бл
		m_fGreen = GetClamp(fGreen,0,1);

		//ю╤
		m_fBlue = GetClamp(fBlue,0,1);

		//alpha
		m_fAlpha = GetClamp(fAlpha,0,1);
	}

	SysRGBAColor& SysRGBAColor::operator+( const SysRGBAColor& rhs )
	{
		m_fRed = GetClamp(m_fRed + rhs.m_fRed,0,1);

		//бл
		m_fGreen = GetClamp(m_fGreen + rhs.m_fGreen,0,1);

		//ю╤
		m_fBlue = GetClamp(m_fBlue + rhs.m_fBlue,0,1);

		//alpha
		m_fAlpha = GetClamp(m_fAlpha + rhs.m_fAlpha,0,1);

		return *this;
	}

	SysRGBAColor& SysRGBAColor::operator*( const SysRGBAColor& rhs )
	{
		m_fRed = GetClamp(m_fRed * rhs.m_fRed,0,1);

		//бл
		m_fGreen = GetClamp(m_fGreen * rhs.m_fGreen,0,1);

		//ю╤
		m_fBlue = GetClamp(m_fBlue * rhs.m_fBlue,0,1);

		//alpha
		m_fAlpha = GetClamp(m_fAlpha * rhs.m_fAlpha,0,1);

		return *this;
	}
}
