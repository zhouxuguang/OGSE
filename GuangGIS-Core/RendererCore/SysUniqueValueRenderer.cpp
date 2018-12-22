#include "SysUniqueValueRenderer.h"
#include "SysFeatureLayer.h"
#include "SysDisplayTransform.h"
#include "SysRenderContext.h"
#include "SysMapManager.h"
#include "SysUtilFunc.h"
#include "SysSymbol.h"
#include "SysRenderContext.h"


#include <GL/glew.h>

#include "ogrsf_frmts.h"

//#include "SysUtility/SysLog.h"
#include "SysUtility/SmartPtr.h"
#include "SysDataSource/IFeatureDataLayer.h"
#include "SysUtility/SysLog.h"

namespace SysDraw
{

	SysUniqueValueRenderer::SysUniqueValueRenderer(void)
	{
		m_nFieldIndex = 0;
		m_mapSymbols.clear();

		glGenBuffers(1,&m_nVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,m_nVertexBuffer);		//绑定对象
	}

	SysUniqueValueRenderer::~SysUniqueValueRenderer(void)
	{
		std::map<std::string,SysSymbol* >::const_iterator iter = m_mapSymbols.begin();
		std::map<std::string,SysSymbol* >::const_iterator iterEnd = m_mapSymbols.end();
		for (; iter != iterEnd; ++iter)
		{
			delete iter->second;
		}

		m_mapSymbols.clear();

		glDeleteBuffers(1,&m_nVertexBuffer);
	}

	std::string SysUniqueValueRenderer::GetRendererName() const
	{
		return _T("unique value renderer");
	}

	std::string SysUniqueValueRenderer::GetRendererChsName() const
	{
		return _T("唯一值渲染器");
	}

	Render_Type SysUniqueValueRenderer::GetRendererType() const
	{
		return UNIQUE_VALUE_RENDERER;
	}

	SysFeatureRenderer* SysUniqueValueRenderer::Clone() const
	{
		assert(false);
		return NULL;
	}

	void SysUniqueValueRenderer::BeginRender( SysRenderContext* pCcontext, const SysFeatureLayer *vlayer )
	{
		//DWORD t1 = GetTickCount();

		//SysDisplayTransform disTran = pCcontext->GetDisplayTran();

		SysEnvelope envView;
		MapExtentToLayerExtent(pManager->GetCurrentMap(),(SysMapLayer *)vlayer,envView);

		//disTran.GetVisibleBounds(envView);
		((SysFeatureLayer *)vlayer)->SetSpatialFilter(envView);

		SysDataSource::IFeatureDataLayer* pFeaLayer = (SysDataSource::IFeatureDataLayer *)vlayer->GetDataSource();

		std::vector<GeoFeature* > vecSelFeatures;
		std::vector<std::string> vecAttrValues;

		GeoFeature* pFeature = NULL;
		while ((pFeature = pFeaLayer->GetNextFeature()) != NULL)
		{
			const char* pszValue = pFeature->GetFieldAsString(m_nFieldIndex);
			std::string strValue = pszValue;

			bool bHighLight = vlayer->IsSelected(pFeature->GetFID());
			if (bHighLight)
			{
				vecSelFeatures.push_back(pFeature);
				vecAttrValues.push_back(strValue);
			}

			else
			{
				SysSymbol* pSymbol = m_mapSymbols[strValue];
				if (NULL != pSymbol)
				{
					pSymbol->RenderFeature((const SysFeature*)pFeature,pCcontext, false);
				}
				GeoFeature::DestroyFeature(pFeature);
			}			
		}

		//绘制高亮的元素
		int nSelCount = vecSelFeatures.size();
		for (int i = 0; i < nSelCount; i ++)
		{
			pFeature = vecSelFeatures[i];
			SysSymbol* pSymbol = m_mapSymbols[vecAttrValues[i]];
			pSymbol->RenderFeature((const SysFeature*)pFeature,pCcontext, true);
			GeoFeature::DestroyFeature(pFeature);
		}
		vecSelFeatures.clear();

		//DWORD t2 = GetTickCount();
		//smartlog<<(t2-t1)<<"毫秒";
	}

	void SysUniqueValueRenderer::EndRender( SysRenderContext* pCcontext )
	{
		//
	}

	const SysSymbolList SysUniqueValueRenderer::GetSymbols() const
	{
		SysSymbolList symList;
		std::map<std::string,SysSymbol* >::const_iterator iter = m_mapSymbols.begin();
		std::map<std::string,SysSymbol* >::const_iterator iterEnd = m_mapSymbols.end();
		for (; iter != iterEnd; ++iter)
		{
			symList.push_back(iter->second);
		}
		return symList;
	}

	void SysUniqueValueRenderer::SetClassificationField( int nFieldIndex )
	{
		m_nFieldIndex = nFieldIndex;
	}

	int SysUniqueValueRenderer::GetClassificationField() const
	{
		return m_nFieldIndex;
	}

	void SysUniqueValueRenderer::InsertValue( std::string strValue, SysSymbol* pSymbol )
	{
		m_mapSymbols.insert(make_pair(strValue,pSymbol));
	}

	SysSymbol* SysUniqueValueRenderer::GetSymbol( SysFeature* pSysFeature ) const
	{
		GeoFeature* pFeature = (GeoFeature *)pSysFeature;
		const char* pszValue = pFeature->GetFieldAsString(m_nFieldIndex);

#ifdef UNICODE
		std::string strValue = SysUtility::toWideString(pszValue);
#else
		std::string strValue = pszValue;
#endif

		SysSymbol* pSymbol = NULL;
		std::map<std::string,SysSymbol* >::const_iterator iter = m_mapSymbols.find(strValue);
		if (iter != m_mapSymbols.end())
		{
			pSymbol = iter->second;
		}

		return pSymbol;
	}

	/*SysSymbol* SysUniqueValueRenderer::GetDefaultSymbol( int nGeomType )
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

		return pSymbol;
	}*/
}
