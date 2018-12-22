#include "SysClassBreakRenderer.h"
#include "SysMapLayer.h"
#include "SysGeometryDef.h"
#include "SysDisplayTransform.h"
#include "SysEnvelope.h"
#include "SysMapManager.h"
#include "SysRenderContext.h"
#include "SysUtilFunc.h"
#include "SysFeatureLayer.h"
#include "SysSymbol.h"

#include <GL/glew.h>

#include "ogrsf_frmts.h"
#include "SysUtility/SmartPtr.h"
#include "SysDataSource/IFeatureDataLayer.h"

namespace SysDraw
{

	SysClassBreakRenderer::SysClassBreakRenderer(void)
	{
		m_nClassFieldIndex = 0;
		m_nMinClassNum = 4;
		m_nClassNum = m_nMinClassNum;
		m_pIntervals = (double *)malloc(sizeof(double) * (m_nClassNum + 1));
		memset(m_pIntervals,0,sizeof(double) * (m_nClassNum + 1));

		m_eBreakType = EqualInterval;			//默锟较等硷拷锟�

		m_mapSymbols.clear();
	}

	SysClassBreakRenderer::~SysClassBreakRenderer(void)
	{
		if (m_pIntervals != NULL)
		{
			free(m_pIntervals);
			m_pIntervals = NULL;
		}

		m_nMinClassNum = 0;
		m_nClassNum = 0;

		BreakSymbolMap::iterator iter = m_mapSymbols.begin();
		BreakSymbolMap::iterator iterEnd = m_mapSymbols.end();
		for (; iter != iterEnd; ++iter)
		{
			delete iter->second;
			iter->second = NULL;
		}

		m_mapSymbols.clear();
	}

	std::string SysClassBreakRenderer::GetRendererName() const
	{
		return _T("class break renderer");
	}

	std::string SysClassBreakRenderer::GetRendererChsName() const
	{
		return _T("class break renderer");
	}

	SysDraw::Render_Type SysClassBreakRenderer::GetRendererType() const
	{
		return CLASS_BREAK_RENDERER;
	}

	SysFeatureRenderer* SysClassBreakRenderer::Clone() const
	{
		assert(false);
		return NULL;
	}

	void SysClassBreakRenderer::BeginRender( SysRenderContext* pContext, const SysFeatureLayer *pLayer )
	{
		SysDisplayTransform disTran = pContext->GetDisplayTran();

		SysEnvelope envView;
		MapExtentToLayerExtent(pManager->GetCurrentMap(),(SysMapLayer *)pLayer,envView);
		((SysFeatureLayer *)pLayer)->SetSpatialFilter(envView);

		SysDataSource::IFeatureDataLayer* pFeaLayer = (SysDataSource::IFeatureDataLayer *)pLayer->GetDataSource();

		GLuint nBuffer;
		glGenBuffers(1,&nBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,nBuffer);		//锟襟定讹拷锟斤拷

		GeoFeature* pFeature = NULL;
		while ((pFeature = pFeaLayer->GetNextFeature()) != NULL)
		{
			//GeoFeature* pGeoFeature = (GeoFeature *)pFeature;
			double dbValue = pFeature->GetFieldAsDouble(m_nClassFieldIndex);

			double dbKey = dbValue;
			for (int j = 0; j < m_nClassNum; j ++)
			{
				if (dbValue >= m_pIntervals[j] && dbValue < m_pIntervals[j+1])
				{
					dbKey = m_pIntervals[j];
					break;
				}
			}

			SysSymbol* pSymbol = m_mapSymbols[dbKey];
			if (NULL != pSymbol)
			{
				pSymbol->RenderFeature((const SysFeature *)pFeature,pContext, false);
			}

			GeoFeature::DestroyFeature(pFeature);
		}

		glDeleteBuffers(1,&nBuffer);
	}

	void SysClassBreakRenderer::EndRender( SysRenderContext* pContext )
	{
		//
	}

	const SysSymbolList SysClassBreakRenderer::GetSymbols() const
	{
		SysSymbolList symList;

		BreakSymbolMap::const_iterator iter = m_mapSymbols.begin();
		BreakSymbolMap::const_iterator iterEnd = m_mapSymbols.end();
		for (; iter != iterEnd; ++iter)
		{
			symList.push_back(iter->second);
		}

		return symList;
	}

	void SysClassBreakRenderer::SetClassificationField( int nFieldIndex )
	{
		m_nClassFieldIndex = nFieldIndex;
	}

	int SysClassBreakRenderer::GetClassificationField() const
	{
		return m_nClassFieldIndex;
	}

	void SysClassBreakRenderer::SetMinClassNum( int nMinClassNum )
	{
		m_nMinClassNum = nMinClassNum;
	}

	int SysClassBreakRenderer::GetMinClassNum() const
	{
		return m_nMinClassNum;
	}

	void SysClassBreakRenderer::SetClassNum( int nClassNum )
	{
		if (nClassNum > m_nClassNum)
		{
			m_pIntervals = (double *)realloc(m_pIntervals,sizeof(double) * (nClassNum + 1));
			memset(m_pIntervals,0,sizeof(double) * (nClassNum + 1));
		}
		m_nClassNum = nClassNum;
	}

	int SysClassBreakRenderer::GetClassNum() const
	{
		return m_nClassNum;
	}

	ClassBreak_Type SysClassBreakRenderer::GetClassBreakType() const
	{
		return m_eBreakType;
	}

	void SysClassBreakRenderer::SetClassBreakType( ClassBreak_Type eBreakType )
	{
		m_eBreakType = eBreakType;
	}

	void SysClassBreakRenderer::GetClassIntervals( double* pValues,int nCount ) const
	{
		double dbMin,dbMax/*,dbSum,dbMean,dbMedian,dbStdDev*/;
		GetValueStatistics(pValues,nCount,&dbMin,&dbMax,NULL,NULL,NULL,NULL);
		GetBreakValues(pValues,nCount,dbMin,dbMax,m_nClassNum,m_eBreakType,m_pIntervals);
	}

	const double* SysClassBreakRenderer::GetIntervalValues() const
	{
		return m_pIntervals;
	}

	void SysClassBreakRenderer::InsertSymbol( double dbMinValue,SysSymbol* pSymbol )
	{
		m_mapSymbols.insert(make_pair(dbMinValue,pSymbol));
	}

	bool SysClassBreakRenderer::FindKey( double dbKey ) const
	{
		BreakSymbolMap::const_iterator iter = m_mapSymbols.find(dbKey);
		return iter != m_mapSymbols.end();
	}

	SysSymbol* SysClassBreakRenderer::GetSymbol( SysFeature* pSysFeature ) const
	{
		GeoFeature* pFeature = (GeoFeature *)pSysFeature;
		double dbValue = pFeature->GetFieldAsDouble(m_nClassFieldIndex);

		/*for (int j = 0; j < m_nClassNum; j ++)
		{
			if (dbValue >= m_pIntervals[j] && dbValue < m_pIntervals[j+1])
			{
				dbKey = m_pIntervals[j];
				break;
			}
		}*/

		int nIndex = GetValueIntervalIndex(dbValue,m_pIntervals,m_nClassNum);

		SysSymbol* pSymbol = NULL;

		//找不到以第一类的符号为渲染方式
		if (-1 == nIndex)
		{
			nIndex = 0;
		}
		double dbKey = m_pIntervals[nIndex];
		BreakSymbolMap::const_iterator iter = m_mapSymbols.find(dbKey);
		if (iter != m_mapSymbols.end())
		{
			pSymbol = iter->second;
		}
		return pSymbol;
	}
}
