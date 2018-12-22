#include "SysMapLayerList.h"
#include "SysMapLayer.h"


namespace SysDraw
{

	SysMapLayerList::SysMapLayerList(void)
	{
		m_vecMapLayers.clear();
		m_pCRS = NULL;
		m_pCurIter = m_vecMapLayers.begin();
		m_pEndIter = m_vecMapLayers.end();
	}

	SysMapLayerList::~SysMapLayerList(void)
	{
		RemoveAllMapLayers();
		if (m_pCRS != NULL)
		{
			delete m_pCRS;
			//SysSRS::CSpatialReference::DestroySpatialReference(m_pCRS);
			m_pCRS = NULL;
		}
	}

	bool SysMapLayerList::AddMapLayer(SysMapLayer* pMapLayer)
	{
		if (NULL == pMapLayer)
		{
			return false;
		}

		//如果有图层名字是一样的图层，则不加入
		std::list<SysMapLayer *>::iterator iter = m_vecMapLayers.begin();
		std::list<SysMapLayer *>::iterator iterEnd = m_vecMapLayers.end();

		for (; iter != iterEnd; ++iter)
		{
			SysMapLayer* pCurLayer = *iter;
			if (pMapLayer->GetLayerName() == pCurLayer->GetLayerName())
			{
				iter = m_vecMapLayers.begin();
				return false;
			}
		}

		//需要将迭代器指向开始
		iter = m_vecMapLayers.begin();

		if (iterEnd == iter)
		{
			//如果为空，则第一个加进来的空间参考系统时地图的参考系统
			SysSRS::CSpatialReference* pTmSrs = pMapLayer->GetSpatialRef();
			if (pTmSrs != NULL)
			{
				std::string strWkt;
				pTmSrs->ExportToWkt(strWkt);
				m_pCRS = new SysSRS::CSpatialReference;
				if (!m_pCRS->ImportFromWkt(strWkt))
				{
					delete m_pCRS;
					m_pCRS = NULL;
				}
			}
		}

		/*for (; iter != iterEnd ; ++iter)
		{
			if (*iter == pMapLayer)
			{
				continue;
			}
				
		}*/


		m_vecMapLayers.push_back(pMapLayer);

		return true;
	}

	bool SysMapLayerList::RemoveMapLayer(const std::string& strName)
	{
		bool bFalg = false;
		std::list<SysMapLayer *>::iterator iter = m_vecMapLayers.begin();
		std::list<SysMapLayer *>::iterator iterEnd = m_vecMapLayers.end();

		for (; iter != iterEnd ; ++iter)
		{
			if ((*iter)->GetLayerName() != strName)
			{
				continue;
			}
			else
			{
				//还需要delete掉图层
				SysMapLayer* pLayer = *iter;
				delete pLayer;
				pLayer = NULL;
				m_vecMapLayers.erase(iter);
				int nCount = m_vecMapLayers.size();
				if (0 == nCount)
				{
					if (m_pCRS != NULL)
					{
						//这里有问题
						delete m_pCRS;
						m_pCRS = NULL;
					}
				}

				bFalg = true;
				break;
			}
		}

		return bFalg;
	}

	bool SysMapLayerList::RemoveMapLayer(const SysMapLayer* pLayer)
	{
		if (NULL == pLayer)
		{
			return false;
		}
		return RemoveMapLayer(pLayer->GetLayerName());

	}

	size_t SysMapLayerList::GetLayerCount() const
	{
		return m_vecMapLayers.size();
	}

	void SysMapLayerList::RemoveAllMapLayers()
	{
		//size_t nCount = m_vecMapLayers.size();

		std::list<SysMapLayer *>::iterator iter = m_vecMapLayers.begin();
		std::list<SysMapLayer *>::iterator iterEnd = m_vecMapLayers.end();

		for (; iter != iterEnd ; ++iter)
		{
			delete (*iter);
			(*iter) = NULL;
		}

		m_vecMapLayers.clear();
	}

	void SysMapLayerList::ResetIterator()
	{
		m_pCurIter = m_vecMapLayers.begin();
	}

	SysMapLayer* SysMapLayerList::GetNextLayer()
	{
		SysMapLayer* pLayer = NULL;
		if (m_pCurIter != m_pEndIter)
		{
			pLayer = *m_pCurIter;
			++m_pCurIter;
		}

		return pLayer;
		
	}

}
