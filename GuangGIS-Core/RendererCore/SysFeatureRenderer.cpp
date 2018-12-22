#include "SysFeatureRenderer.h"
#include "SysRGBColor.h"

#include "SysSingleSymbolRenderer.h"
#include "SysUniqueValueRenderer.h"
#include "SysClassBreakRenderer.h"

#include "SysSimpleFillSymbol.h"
#include "SysSimpleLineSymbol.h"
#include "SysSimplePointSymbol.h"


namespace SysDraw
{


	SysFeatureRenderer::SysFeatureRenderer(void)
	{
	}

	SysFeatureRenderer::~SysFeatureRenderer(void)
	{
	}

	SysFeatureRenderer* SysFeatureRenderer::GetDefaultRenderer( int nGeomType )
	{
		SysSingleSymbolRenderer* pRender = (SysSingleSymbolRenderer *)CreateFeatureRenderer(SINGLE_SYMBOL_RENDERER);
		
		if (NULL == pRender)
		{
			return NULL;
		}

		SysSymbol* pSymbol = GetDefaultSymbol(nGeomType);
		pRender->SetSymbol(pSymbol);
		return pRender;
	}

	SysSymbol* SysFeatureRenderer::GetDefaultSymbol(int nGeomType)
	{
		SysSymbol* pSymbol = NULL;
		switch (nGeomType)
		{
		case wkbPoint:
		case wkbPoint25D:
		case wkbMultiPoint:
		case wkbMultiPoint25D:
			{
				pSymbol = new(std::nothrow) SysSimplePointSymbol;
			}
			break;

		case wkbLineString:
		case wkbLineString25D:
		case wkbMultiLineString:
		case wkbMultiLineString25D:
			{
				pSymbol = new(std::nothrow) SysSimpleLineSymbol;
			}
			break;

		case wkbPolygon:
		case wkbPolygon25D:
		case wkbMultiPolygon:
		case wkbMultiPolygon25D:
			{
				pSymbol = new(std::nothrow) SysSimpleFillSymbol;
			}
			break;

		default:
			break;
		}

		//随机设置颜色
		if (pSymbol != NULL)
		{
			//pSymbol->SetOutLineColor(SysRGBAColor(156.0f/255,156.0f/255,156.0f/255));
			pSymbol->SetOutLineColor(SysRGBAColor(0.0f/255,0.0f/255,0.0f/255));
			pSymbol->SetOutLineWidth(0.4);

			srand((unsigned int)time(NULL));
			float fRed = rand()%255 / 255.0f;
			float fGreen = rand()%255 / 255.0f;
			float fBlue = rand()%255 / 255.0f;
			pSymbol->SetColor(SysRGBAColor(fRed,fGreen,fBlue));
		}
		return pSymbol;
	}

	//以下是创建和销毁渲染器
	SysFeatureRenderer* CreateFeatureRenderer(Render_Type eType)
	{
		//单一符号渲染器
		if (SINGLE_SYMBOL_RENDERER == eType)
		{
			return new(std::nothrow) SysSingleSymbolRenderer;
		}

		//分级渲染
		else if (CLASS_BREAK_RENDERER == eType)
		{
			return new(std::nothrow) SysClassBreakRenderer;
		}

		//唯一值渲染
		else if (UNIQUE_VALUE_RENDERER == eType)
		{
			return new(std::nothrow) SysUniqueValueRenderer;
		}

		return NULL;
	}

	void DestroyFeatureRenderer(SysFeatureRenderer* pRender)
	{
		if (NULL != pRender)
		{
			delete pRender;
			pRender = NULL;
		}
	}

}
