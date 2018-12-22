#include "SysColorRamp.h"
#include "SysRGBColor.h"
#include "SysImagePreProcess.h"
#include "SysDrawDef.h"

namespace SysDraw
{


	SysColorRamp::SysColorRamp(const SysRGBAColor& startColor,const SysRGBAColor& endColor)
	{
		m_pStartColor = new(std::nothrow) SysRGBAColor(startColor);
		m_pEndColor = new(std::nothrow) SysRGBAColor(endColor);
	}

	SysColorRamp::SysColorRamp( const SysColorItem& startColor,const SysColorItem& endColor )
	{
		m_pStartColor = new(std::nothrow) SysRGBAColor(startColor.c1/255.0,startColor.c2/255.0,startColor.c3/255.0);
		m_pEndColor = new(std::nothrow) SysRGBAColor(endColor.c1/255.0,endColor.c2/255.0,endColor.c3/255.0);
	}

	SysColorRamp::~SysColorRamp(void)
	{
		if (m_pStartColor != NULL)
		{
			delete m_pStartColor;
			m_pStartColor = NULL;
		}

		if (m_pEndColor != NULL)
		{
			delete m_pEndColor;
			m_pEndColor = NULL;
		}
	}

	void SysColorRamp::GetColors( SysRGBAColor* pColors,int nCount ) const
	{
		if (NULL == pColors)
		{
			return;
		}

		eByte eRedStart = m_pStartColor->GetRed()*255;
		eByte eGreenStart = m_pStartColor->GetGreen()*255;
		eByte eBlueStart = m_pStartColor->GetBlue()*255;

		float i1,h1,s1;
		Rgb2Ihs(eRedStart,eGreenStart,eBlueStart,i1,h1,s1);

		eByte eRedEnd = m_pEndColor->GetRed()*255;
		eByte eGreenEnd = m_pEndColor->GetGreen()*255;
		eByte eBlueEnd = m_pEndColor->GetBlue()*255;

		float i2,h2,s2;
		Rgb2Ihs(eRedEnd,eGreenEnd,eBlueEnd,i2,h2,s2);

		float fIntervalH = (h2 - h1)/nCount;
		float fIntervalI = (i2 - i1)/nCount;
		float fIntervalS = (s2 - s1)/nCount;

		pColors[0] = *m_pStartColor;
		pColors[nCount-1] = *m_pEndColor;

		for (int i = 1; i < nCount - 1; i ++)
		{
			float iRet = 127.0f;
			float hRet = h1 + i * fIntervalH;
			float sRet = s1 + i * fIntervalS;
			eByte eRed,eGreen,eBlue;
			HIS2RGB(hRet,iRet,sRet,eRed,eGreen,eBlue);

			pColors[i].SetRGBA(eRed/255.0,eGreen/255,eBlue/255.0);
		}

		return;
	}

	void SysColorRamp::GetColors( SysColorItem* pColors,int nCount ) const
	{
		if (NULL == pColors)
		{
			return;
		}

		eByte eRedStart = m_pStartColor->GetRed()*255;
		eByte eGreenStart = m_pStartColor->GetGreen()*255;
		eByte eBlueStart = m_pStartColor->GetBlue()*255;

		float i1,h1,s1;
		Rgb2Ihs(eRedStart,eGreenStart,eBlueStart,i1,h1,s1);

		eByte eRedEnd = m_pEndColor->GetRed()*255;
		eByte eGreenEnd = m_pEndColor->GetGreen()*255;
		eByte eBlueEnd = m_pEndColor->GetBlue()*255;

		float i2,h2,s2;
		Rgb2Ihs(eRedEnd,eGreenEnd,eBlueEnd,i2,h2,s2);

		float fIntervalH = (h2 - h1)/nCount;
		float fIntervalI = (i2 - i1)/nCount;
		float fIntervalS = (s2 - s1)/nCount;

		pColors[0].c1 = eRedStart;
		pColors[0].c2 = eGreenStart;
		pColors[0].c3 = eBlueStart;

		pColors[nCount-1].c1 = eRedEnd;
		pColors[nCount-1].c2 = eGreenEnd;
		pColors[nCount-1].c3 = eBlueEnd;

		for (int i = 1; i < nCount-1; i ++)
		{
			float iRet = 127.0f/*(i1 + i2)*0.5*/;
			//float iRet = i1 + i * fIntervalI;
			float hRet = h1 + i * fIntervalH;
			float sRet = s1 + i * fIntervalS;
			eByte eRed,eGreen,eBlue;
			HIS2RGB(hRet,iRet,sRet,eRed,eGreen,eBlue);

			pColors[i].c1 = eRed;
			pColors[i].c2 = eGreen;
			pColors[i].c3 = eBlue;
		}

		return;
	}
}
