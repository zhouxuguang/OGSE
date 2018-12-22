#include "SysMapToolFactory.h"

namespace SysDraw
{
	//³õÊ¼»¯
	SysMapToolFactory::CreateCallBackMap SysMapToolFactory::m_mapCreateMap;		
	SysMapToolFactory::DestroyCallBackMap SysMapToolFactory::m_mapDestroyMap;

	SysMapToolFactory::SysMapToolFactory(void)
	{
		m_mapCreateMap.clear();
		m_mapDestroyMap.clear();
	}

	SysMapToolFactory::~SysMapToolFactory(void)
	{
		m_mapCreateMap.clear();
		m_mapDestroyMap.clear();
	}

	SysMapToolFactory* SysMapToolFactory::GetInstance()
	{
		static SysMapToolFactory* pInstance = NULL;
		if (NULL == pInstance)
		{
			pInstance = new SysMapToolFactory;
		}

		return pInstance;
	}

	void SysMapToolFactory::RegisterMapTool( const std::string& strName,CreateMapToolFun pCallBackFun,DestroyMapToolFun pDestroyFun )
	{
		m_mapCreateMap[strName] = pCallBackFun;
		m_mapDestroyMap[strName] = pDestroyFun;
	}

	void SysMapToolFactory::UnRegisterMapTool( const std::string& strName )
	{
		m_mapCreateMap.erase(strName);
		m_mapDestroyMap.erase(strName);
	}

	SysMapTool* SysMapToolFactory::CreateMapTool( const std::string& strName )
	{
		CreateCallBackMap::iterator iter = m_mapCreateMap.find(strName);
		if (iter != m_mapCreateMap.end())
		{
			CreateMapToolFun pFun = iter->second;
			if (pFun != NULL)
			{
				return pFun(strName);
			}
			//return (iter->second)(strName);
		}

		return NULL;
	}

	void SysMapToolFactory::DestroyMapTool( const std::string& strName,SysMapTool* pTool )
	{
		DestroyCallBackMap::iterator iter = m_mapDestroyMap.find(strName);
		if (iter != m_mapDestroyMap.end())
		{
			DestroyMapToolFun pFun = iter->second;
			if (pFun != NULL)
			{
				pFun(pTool);
			}
			//(iter->second)(pTool);
		}
	}
}
