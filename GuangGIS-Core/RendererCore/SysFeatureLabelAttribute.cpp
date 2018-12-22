#include "SysFeatureLabelAttribute.h"

namespace SysDraw
{

	SysFeatureLabelAttribute::SysFeatureLabelAttribute(void)
	{
		m_bBold = false;
		m_bItalic = false;
		m_bStrikeOut = false;
		m_bUnderline = false;

		m_Color = SysRGBAColor(1.0,0.0,1.0);
		m_dXOffSet = 0;
		m_dYOffSet = 0;
		m_nOffSetType = 0;
		//mSizeType = rhs.mSizeType;
		//m_strText = rhs.m_strText;
	}

	SysFeatureLabelAttribute& SysFeatureLabelAttribute::operator=( const SysFeatureLabelAttribute& rhs )
	{
		m_bBold = rhs.m_bBold;
		m_bItalic = rhs.m_bItalic;
		m_bStrikeOut = rhs.m_bStrikeOut;
		m_bUnderline = rhs.m_bUnderline;

		m_Color = rhs.m_Color;
		m_dXOffSet = rhs.m_dXOffSet;
		m_dYOffSet = rhs.m_dYOffSet;
		m_nOffSetType = rhs.m_nOffSetType;
		mSizeType = rhs.mSizeType;
		m_strText = rhs.m_strText;

		return *this;
	}

	SysFeatureLabelAttribute::~SysFeatureLabelAttribute(void)
	{
	}

	std::string SysFeatureLabelAttribute::GetUnitsName( int nUnits )
	{
		if ( nUnits == MapUnits )
		{
			return std::string( ("mu") );
		}

		return std::string( "pt" );
	}

	int SysFeatureLabelAttribute::GetUnitsCode( const std::string &strName )
	{
		if ( strName.compare( ("mu") ) == 0 )
		{
			return MapUnits;
		}

		return PointUnits;
	}

	void SysFeatureLabelAttribute::SetBold( bool enable )
	{
		m_bBold = enable;
	}

	bool SysFeatureLabelAttribute::IsBold( void ) const
	{
		return m_bBold;
	}

	void SysFeatureLabelAttribute::SetItalic( bool enable )
	{
		m_bItalic = enable;
	}

	bool SysFeatureLabelAttribute::IsItalic( void ) const
	{
		return m_bItalic;
	}

	void SysFeatureLabelAttribute::SetUnderline( bool enable )
	{
		m_bUnderline = enable;
	}

	bool SysFeatureLabelAttribute::IsUnderline( void ) const
	{
		return m_bUnderline;
	}

	void SysFeatureLabelAttribute::SetStrikeOut( bool enable )
	{
		m_bStrikeOut = enable;
	}

	bool SysFeatureLabelAttribute::IsStrikeOut( void ) const
	{
		return m_bStrikeOut;
	}

	void SysFeatureLabelAttribute::SetSize( double size, int type )
	{

	}

	int SysFeatureLabelAttribute::GetSizeType( void ) const
	{
		return mSizeType;
	}

	void SysFeatureLabelAttribute::SetColor( const SysRGBAColor &color )
	{
		m_Color = color;
	}

	const SysRGBAColor & SysFeatureLabelAttribute::GetColor( void ) const
	{
		return m_Color;
	}

	void SysFeatureLabelAttribute::SetOffSet( double x, double y, int type )
	{
		m_dXOffSet = x;
		m_dYOffSet = y;
		m_nOffSetType = type;
	}

	int SysFeatureLabelAttribute::GetOffSetType( void ) const
	{
		return m_nOffSetType;
	}

	double SysFeatureLabelAttribute::GetXOffSet( void ) const
	{
		return m_dXOffSet;
	}

	double SysFeatureLabelAttribute::GetYOffSet( void ) const
	{
		return m_dYOffSet;
	}

	void SysFeatureLabelAttribute::SetAngle( double angle )
	{
		//
	}

	double SysFeatureLabelAttribute::GetAngle( void ) const
	{
		return 0;
	}

	
}
