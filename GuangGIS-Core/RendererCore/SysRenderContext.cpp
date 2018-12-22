#include "SysRenderContext.h"

namespace SysDraw
{

	SysRenderContext::SysRenderContext(void)
	{
		//m_pCoordTran = NULL;
	}

	SysRenderContext::~SysRenderContext(void)
	{
		//m_pCoordTran = NULL;
	}

	const SysEnvelope& SysRenderContext::GetExtent() const
	{
		return m_Extent;
	}

	const SysDisplayTransform& SysRenderContext::GetDisplayTran() const
	{
		return m_DisplayTran;
	}

	SysEnvelope& SysRenderContext::GetExtent()
	{
		return m_Extent;
	}

	SysDisplayTransform& SysRenderContext::GetDisplayTran()
	{
		return m_DisplayTran;
	}

	void SysRenderContext::SetExtent(const SysEnvelope& extent)
	{
		m_Extent = extent;
	}

	void SysRenderContext::SetDisplayTran(const SysDisplayTransform& displayTran)
	{
		m_DisplayTran = displayTran;
	}

}
