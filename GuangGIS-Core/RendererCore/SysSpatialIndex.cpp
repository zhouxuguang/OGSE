#include "SysSpatialIndex.h"

#include "spatialindex/SpatialIndex.h"

//底层
#include "SysEnvelope.h"
#include "SysCoordinate.h"

#include "SysUtility/SysUtilityInc.h"

#include "ogrsf_frmts.h"

using namespace SpatialIndex;

class SysVisitor : public IVisitor
{
public:
	SysVisitor( std::vector<long> & list )
		: mList( list ) {}

	void visitNode( const INode& n )
	{  }

	void visitData( const IData& d )
	{
		mList.push_back( d.getIdentifier() );
	}

	void visitData( std::vector<const IData*>& v )
	{  }

private:
	std::vector<long>& mList;
};

namespace SysDraw
{
	static Region RectToRegion( SysEnvelope& rect ) ;

	static bool GetFeatureInfo( SysFeature *pFeature, SpatialIndex::Region& r, long &id );

	SysSpatialIndex::SysSpatialIndex(void)
	{
		m_strIndexFile.clear();
		m_pStorageManager = StorageManager::createNewMemoryStorageManager();

		// 创建缓存

		unsigned int capacity = 10;
		bool writeThrough = false;
		m_pStorage = StorageManager::createNewRandomEvictionsBuffer( *m_pStorageManager, capacity, writeThrough );

		// R-Tree 参数
		double fillFactor = 0.7;
		unsigned long indexCapacity = 10;
		unsigned long leafCapacity = 10;
		unsigned long dimension = 2;
		RTree::RTreeVariant variant = RTree::RV_RSTAR;

		// 创建 R-tree
		SpatialIndex::id_type indexId;
		m_pRTree = RTree::createNewRTree( *m_pStorage, fillFactor, indexCapacity,
			leafCapacity, dimension, variant, indexId );
	}

	SysSpatialIndex::SysSpatialIndex( const std::string& strFileName ,bool bCreateNewIndex)
	{
		#ifdef UNICODE
			std::string strName = SysUtility::toNarrowString(strFileName);
		#else
			std::string strName = strFileName;
		#endif

		m_strIndexFile = strFileName;

		if (bCreateNewIndex)
		{
			m_pStorageManager = StorageManager::createNewDiskStorageManager(m_strIndexFile,4096);
			//m_pStorageManager = StorageManager::createNewMemoryStorageManager();

			// 创建缓存

			unsigned int capacity = 10;
			bool writeThrough = false;
			m_pStorage = StorageManager::createNewRandomEvictionsBuffer( *m_pStorageManager, capacity, writeThrough );

			// R-Tree 参数
			double fFillFactor = 0.7;
			unsigned long nIndexCapacity = 10;
			unsigned long nLeafCapacity = 10;
			unsigned long nDimension = 2;
			RTree::RTreeVariant variant = RTree::RV_RSTAR;

			// 创建 R-tree
			SpatialIndex::id_type nIndexId;
			m_pRTree = RTree::createNewRTree( *m_pStorage, fFillFactor, nIndexCapacity,
				nLeafCapacity, nDimension, variant, nIndexId );

			nIndexId = nIndexId;
		}

		else if (!bCreateNewIndex)
		{

			m_pStorageManager = StorageManager::loadDiskStorageManager(strName);

			// 创建缓存
			m_pStorage = NULL;

			// 加载 R-tree
			SpatialIndex::id_type indexId = 1;
			if (NULL == m_pStorageManager)
			{
				//smartlog<<"执行分支3";
				m_pRTree = NULL;
				return;
			}
			m_pRTree = RTree::loadRTree( *m_pStorageManager, indexId );
			//smartlog<<"执行分支2";
		}
	}

	SysSpatialIndex::~SysSpatialIndex(void)
	{
		if (m_pRTree != NULL)
		{
			delete m_pRTree;
			m_pRTree = NULL;
		}

		if (m_pStorage != NULL)
		{
			delete m_pStorage;
			m_pStorage = NULL;
		}

		if (m_pStorageManager != NULL)
		{
			delete m_pStorageManager;
			m_pStorageManager = NULL;
		}		
		
	}

	bool SysSpatialIndex::InsertFeature( SysFeature* pFeature )
	{
		Region r;
		long id;
		if ( !GetFeatureInfo( pFeature, r, id ) )
			return false;

		try
		{
			if (NULL == m_pRTree)
			{
				return false;
			}

			m_pRTree->insertData( 0, 0, r,  id );
		}
		catch ( Tools::Exception &e )
		{
			e.what();
		}
		catch ( const std::exception &e )
		{
			e.what();
		}
		catch ( ... )
		{
			//
		}

		return true;
	}

	bool GetFeatureInfo( SysFeature *pSysFeature, SpatialIndex::Region& r, long &id )
	{
		GeoFeature *pFeature = (GeoFeature *)pSysFeature;
		GeoGeometry *pGeometry = pFeature->GetGeometryRef();
		if (NULL == pGeometry)
		{
			return false;
		}

		id = pFeature->GetFID();

		GeoEnvelope env;
		pGeometry->GetEnvelope(&env);
		//SysEnvelope envNew;
		//envNew.Init(env.MinX,env.MaxX,env.MinY,env.MaxY);
		//r = RectToRegion(envNew);

		double pt1[2], pt2[2];
		pt1[0] = env.MinX;
		pt1[1] = env.MinY;
		pt2[0] = env.MaxX;
		pt2[1] = env.MaxY;
		r = Region( pt1, pt2, 2 );
		
		return true;
	}

	SpatialIndex::Region RectToRegion( SysEnvelope& rect )
	{
		double pt1[2], pt2[2];
		pt1[0] = rect.minX;
		pt1[1] = rect.minY;
		pt2[0] = rect.maxX;
		pt2[1] = rect.maxY;
		return Region( pt1, pt2, 2 );
	}

	bool SysSpatialIndex::DeleteFeature( SysFeature* pFeature )
	{
		Region r;
		long id;
		if ( !GetFeatureInfo( pFeature, r, id ) )
			return false;

		if (NULL == m_pRTree)
		{
			return false;
		}

		return m_pRTree->deleteData( r, id);
	}

	std::vector<long> SysSpatialIndex::IntersectQuery( const SysEnvelope& rect ) const
	{
		std::vector<long> list;
		SysVisitor visitor( list );

		Region r = RectToRegion( (SysEnvelope&)rect );

		if (NULL == m_pRTree)
		{
			return list;
		}

		m_pRTree->intersectsWithQuery( r, visitor );

		return list;
	}

	std::vector<long> SysSpatialIndex::PointLocationQuery( const SysCoordinate& point ) const
	{
		std::vector<long> list;
		SysVisitor visitor(list);

		double pt[2];
		pt[0] = point.x;
		pt[1] = point.y;
		Point ptLoc(pt , 2);

		if (NULL == m_pRTree)
		{
			return list;
		}

		m_pRTree->pointLocationQuery(ptLoc,visitor);

		return list;
	}

	std::vector<long> SysSpatialIndex::PointLocationQuery( double dbX,double dbY ) const
	{
		std::vector<long> list;
		SysVisitor visitor(list);

		double pt[2];
		pt[0] = dbX;
		pt[1] = dbY;
		Point ptLoc(pt , 2);

		if (NULL == m_pRTree)
		{
			return list;
		}

		m_pRTree->pointLocationQuery(ptLoc,visitor);

		return list;
	}

	std::vector<long> SysSpatialIndex::NearestNeighbor( const SysCoordinate& point, int nNeighbors ) const
	{
		std::vector<long> list;
		SysVisitor visitor( list );

		double pt[2];
		pt[0] = point.x;
		pt[1] = point.y;
		Point p( pt, 2 );

		if (NULL == m_pRTree)
		{
			return list;
		}

		m_pRTree->nearestNeighborQuery( nNeighbors, p, visitor );

		return list;
	}

	std::vector<long> SysSpatialIndex::NearestNeighbor( double dbX, double dbY, int nNeighbors ) const
	{
		std::vector<long> list;
		SysVisitor visitor( list );

		double pt[2];
		pt[0] = dbX;
		pt[1] = dbY;
		Point p( pt, 2 );

		if (NULL == m_pRTree)
		{
			return list;
		}

		m_pRTree->nearestNeighborQuery( nNeighbors, p, visitor );

		return list;
	}

	bool SysSpatialIndex::SaveToDisk() const
	{
		if (m_strIndexFile.empty())
			return false;

		#ifdef UNICODE
			std::string strName = SysUtility::toNarrowString(m_strIndexFile);
		#else
			std::string strName = m_strIndexFile;
		#endif

		strName += ".idx";

		//m_pRTree->isIndexValid()

		/*ofstream oStream(strName.c_str());
		oStream<<*m_pRTree;*/
		return true;
	}
}
