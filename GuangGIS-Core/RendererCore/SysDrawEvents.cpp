#include "SysDrawEvents.h"

namespace SysDraw
{

	SysDrawEvents::SysDrawEvents(void)
	{
		m_vecLayerAddFuncs.clear();
		m_vecLayerRemoveFuncs.clear();
	}

	SysDrawEvents::~SysDrawEvents(void)
	{
		m_vecLayerAddFuncs.clear();
		m_vecLayerRemoveFuncs.clear();
	}

	SysDrawEvents* SysDrawEvents::GetInstance()
	{
		static SysDrawEvents instance;
		return &instance;
	}

	void SysDrawEvents::RegisterMapLayerAddEvent( OnMapLayerAdd pEventFunc )
	{
		m_vecLayerAddFuncs.push_back(pEventFunc);
	}

	void SysDrawEvents::RegisterMapLayerRemoveEvent( OnMapLayerRemove pEventFunc )
	{
		m_vecLayerRemoveFuncs.push_back(pEventFunc);
	}

	void SysDrawEvents::ActiveMapLayerAddEvents(SysMapLayer* pLayer, void* pArg) const
	{
		int nCount = m_vecLayerAddFuncs.size();
		for (int i = 0; i < nCount; i ++)
		{
			OnMapLayerAdd pFun = m_vecLayerAddFuncs[i];
			if (pFun != NULL)
			{
				pFun(pLayer,pArg);
			}
		}
	}

	void SysDrawEvents::ActiveMapLayerRemoveEvents(SysMapLayer* pLayer, void* pArg) const
	{
		int nCount = m_vecLayerRemoveFuncs.size();
		for (int i = 0; i < nCount; i ++)
		{
			OnMapLayerRemove pFun = m_vecLayerRemoveFuncs[i];
			if (pFun != NULL)
			{
				pFun(pLayer,pArg);
			}
		}
	}

}
