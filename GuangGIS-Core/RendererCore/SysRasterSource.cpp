#include "SysRasterSource.h"

namespace SysDraw
{


	SysRasterSource::SysRasterSource(void)
	{
		m_pDataLayer = NULL;
	}

	SysRasterSource::~SysRasterSource(void)
	{
		m_pDataLayer = NULL;
	}

	void SysRasterSource::SetDataSource(SysDataSource::IDataLayerPtr pDataLayer)
	{
		m_pDataLayer = pDataLayer;
	}

	SysDataSource::IDataLayerPtr SysRasterSource::GetDataSource() const
	{
		return m_pDataLayer;
	}

}