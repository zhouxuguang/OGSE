#include "SysFeatureCache.h"


namespace SysDraw
{

	SysFeatureCache::SysFeatureCache(void)
	{
		m_nMaxCount = 20000;
		m_mapFeaCache.clear();
	}

	SysFeatureCache::~SysFeatureCache(void)
	{
		m_nMaxCount = 0;

		mapFeatureCache::iterator iter = m_mapFeaCache.begin();
		mapFeatureCache::iterator iterEnd = m_mapFeaCache.begin();
		for (; iter != iterEnd; ++iter)
		{
			GeoFeature::DestroyFeature((GeoFeature *)iter->second);
		}

		m_mapFeaCache.clear();
	}

	int SysFeatureCache::GetMaxFeatureCount() const
	{
		return m_nMaxCount;
	}

	void SysFeatureCache::SetMaxFeatureCount( int nMaxCount )
	{
		m_nMaxCount = nMaxCount;
	}

	bool SysFeatureCache::InsertFeature( long nFid,SysFeature *pFeature )
	{
		/*if (m_mapFeaCache.size() >= m_nMaxCount)
		{
			return false;
		}*/
		m_mapFeaCache.insert(make_pair(nFid,pFeature));

		return true;
	}

	bool SysFeatureCache::EraseFeature( long nFid )
	{	
		mapFeatureCache::iterator iter = m_mapFeaCache.find(nFid);
		if (iter != m_mapFeaCache.end())
		{
			GeoFeature::DestroyFeature((GeoFeature *)iter->second);
			m_mapFeaCache.erase(iter);

			return true;
		}

		return false;
	}

	bool SysFeatureCache::FindFeature( long nFid,SysFeature*& pFeature ) const
	{
		mapFeatureCache::const_iterator iter = m_mapFeaCache.find(nFid);
		if (iter != m_mapFeaCache.end())
		{
			pFeature = iter->second;
			return true;
		}

		pFeature = NULL;

		return false;
	}
}
