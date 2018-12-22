#include "SysDrawCanvas.h"
#include "SysMap.h"
#include "SysMapLayer.h"
#include "SysGraphLayer.h"
#include "SysRenderContext.h"

namespace SysDraw
{

	SysDrawCanvas::SysDrawCanvas(SysRenderContext* pRenderContext)
	{
		//m_pRenderContext = new SysRenderContext;
		if (pRenderContext != NULL)
		{
			m_pRenderContext = pRenderContext;
		}
		m_nViewWidth = 0;
		m_nViewHeight = 0;
	}

	SysDrawCanvas::~SysDrawCanvas(void)
	{
		//这里怎么析构还需要考虑清楚
		if (m_pRenderContext != NULL)
		{
			delete m_pRenderContext;
			m_pRenderContext = NULL;
		}

		m_nViewWidth = 0;
		m_nViewHeight = 0;
	}

	void SysDrawCanvas::OnSize(eUInt32 nType, int cx, int cy)
	{
		if (cx == m_nViewWidth && cy == m_nViewHeight)
		{
			return;
		}

		m_nViewWidth = cx;
		m_nViewHeight = cy;
	}

	void SysDrawCanvas::SetRenderContext(SysRenderContext* pRenderContext)
	{
		if (pRenderContext != NULL)
		{
			m_pRenderContext = pRenderContext;
		}
	}

	void SysDrawCanvas::DrawMap(SysMap* pMap) const
	{
		if (NULL == pMap)
		{
			return;
		}

		pMap->Reset();
		SysMapLayer* pLayer = NULL;
		while ((pLayer = pMap->GetNextLayer()) != NULL)
		{
			pLayer->Draw(m_pRenderContext);
		}

		DrawMarkLayer(pMap);
	}

	void SysDrawCanvas::DrawMap( SysMap* pMap,const SysEnvelope& envDraw ) const
	{
		if (NULL == pMap)
		{
			return;
		}

		pMap->Reset();
		SysMapLayer* pLayer = NULL;
		while ((pLayer = pMap->GetNextLayer()) != NULL)
		{
			pLayer->Draw(m_pRenderContext,envDraw);
		}

		DrawMarkLayer(pMap);
	}

	void SysDrawCanvas::DrawLayer(SysMap* pMap,SysMapLayer* pLayer) const
	{
		if (NULL == pMap)
		{
			return;
		}

		if (NULL == pLayer)
		{
			return;
		}

		pMap->Reset();
		SysMapLayer* pCurLayer = NULL;
		while ((pCurLayer = pMap->GetNextLayer()) != NULL)
		{
			if (pLayer == pCurLayer)
			{
				pLayer = pCurLayer;
				break;
			}
		}

		pMap->Reset();

		pLayer->Draw(m_pRenderContext);
	}

	void SysDrawCanvas::DrawLayer( SysMap* pMap,SysMapLayer* pLayer,const SysEnvelope& envDraw ) const
	{
		if (NULL == pMap)
		{
			return;
		}

		if (NULL == pLayer)
		{
			return;
		}

		pMap->Reset();
		SysMapLayer* pCurLayer = NULL;
		while ((pCurLayer = pMap->GetNextLayer()) != NULL)
		{
			if (pLayer == pCurLayer)
			{
				pLayer = pCurLayer;
				break;
			}
		}

		pMap->Reset();

		pLayer->Draw(m_pRenderContext,envDraw);
	}

	void SysDrawCanvas::DrawBrowseLayer(SysMap* pMap) const
	{
		//绘制临时层
		SysGraphLayer* pLayer = pMap->GetBrowseLayer();
		if (NULL == pLayer)
		{
			return;
		}
		pLayer->Draw(m_pRenderContext);
	}

	void SysDrawCanvas::DrawMarkLayer(SysMap* pMap,bool bDrawAll, int nIndex) const
	{
		//
		SysGraphLayer* pLayer = pMap->GetMarkLayer();
		if (NULL == pLayer)
		{
			return;
		}

		if (bDrawAll)
		{
			pLayer->Draw(m_pRenderContext);
		}

		else
		{
			pLayer->DrawElement(nIndex,m_pRenderContext);
		}
	}

	int SysDrawCanvas::GetViewWidth() const
	{
		return m_nViewWidth;
	}

	int SysDrawCanvas::GetViewHeight() const
	{
		return m_nViewHeight;
	}

	void SysDrawCanvas::DrawGraphicLayers( SysMap* pMap ) const
	{
		if (NULL == pMap)
		{
			return;
		}

		pMap->Reset();
		SysMapLayer* pLayer = NULL;
		while ((pLayer = pMap->GetNextLayer()) != NULL)
		{
			if (pLayer->GetLayerType() == SysMapLayer::GraphicsLayer)
			{
				pLayer->Draw(m_pRenderContext);
			}
			
		}
	}
}
