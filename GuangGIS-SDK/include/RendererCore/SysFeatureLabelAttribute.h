/**
* @file              SysFeatureLabelAttribute.h
* @brief             绘制标注时的属性设置
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年10月16日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSFEATURELABELATTRIBUTE_H_89C0CCDE_DB24_4225_A021_8B4DEF17E715__
#define __SYSFEATURELABELATTRIBUTE_H_89C0CCDE_DB24_4225_A021_8B4DEF17E715__


#include "SysRGBColor.h"

namespace SysDraw
{
	class SysRGBAColor;

	class SYSDRAW_API SysFeatureLabelAttribute
	{
	public:
		SysFeatureLabelAttribute(void);

		SysFeatureLabelAttribute& operator = (const SysFeatureLabelAttribute& rhs);

		~SysFeatureLabelAttribute(void);

		/* 单位类型 */
		enum Units
		{
			MapUnits = 0,
			PointUnits
		};

		static std::string GetUnitsName( int nUnits );
		static int GetUnitsCode( const std::string &strName );

		/* Text */
		void SetText( const std::string & text );
		const std::string GetText( void ) const;

		/* 字体 */
		void SetFamily( const std::string & family );
		const std::string GetFamily( void ) const;

		//粗体
		void SetBold( bool enable );
		bool IsBold( void ) const;

		//斜体
		void SetItalic( bool enable );
		bool IsItalic( void ) const;

		//下划线
		void SetUnderline( bool enable );
		bool IsUnderline( void ) const;

		/* strikeout  */
		void SetStrikeOut( bool enable );
		bool IsStrikeOut( void ) const;

		void   SetSize( double size, int type );
		int    GetSizeType( void ) const;
		double size( void ) const;

		void  SetColor( const SysRGBAColor &color );
		const SysRGBAColor & GetColor( void ) const;

		/* 偏移量 */
		void   SetOffSet( double x, double y, int type );
		int    GetOffSetType( void ) const;
		double GetXOffSet( void ) const;
		double GetYOffSet( void ) const;

		/* 角度 */
		void   SetAngle( double angle );
		double GetAngle( void ) const;

		bool   AngleIsAuto( void ) const;
		void   SetAutoAngle( bool state );

		/* 对齐方式 */
		void SetAlignment( int alignment );
		int  GetAlignment( void ) const;

		/* Buffer */
		bool   bufferEnabled() const;
		void   SetBufferEnabled( bool useBufferFlag );
		void   SetBufferSize( double size, int type );
		int    GetBufferSizeType( void ) const;
		double GetBufferSize( void ) const;

		//void  SetBufferColor( const QColor &color );
		//QColor bufferColor( void ) const;

		//void  SetBufferStyle( Qt::BrushStyle style );
		//Qt::BrushStyle bufferStyle( void ) const;

		/* Border */
		//void  SetBorderColor( const QColor &color );
		//QColor borderColor( void ) const;

		void  SetBorderWidth( int width );
		int   GetBorderWidth( void ) const;

		//void  SetBorderStyle( Qt::PenStyle style );
		//Qt::PenStyle   borderStyle( void ) const;

		bool  MultilineEnabled() const;
		void  SetMultilineEnabled( bool useMultiline );

		/* label only selected features
		* added in 1.5
		*/
		bool  SelectedOnly() const;
		void  SetSelectedOnly( bool selectedonly );

	protected:
		/* Text */
		std::string m_strText;

		/** Font (family, weight, italic, underline, strikeout) */
		bool m_bFamily;
		bool m_bBold;
		bool m_bItalic;
		bool m_bUnderline;
		bool m_bStrikeOut;

		/** Font size, size type */
		int  mSizeType;
		double mSize;

		/** Color */
		SysRGBAColor m_Color;

		/** OffSet */
		int    m_nOffSetType;
		double m_dXOffSet;
		double m_dYOffSet;

		/** Angle (degrees) */
		double mAngle;

		/** Alignment */
		int  mAlignment;

		/** Buffer enablement */
		bool mBufferEnabledFlag;
		/** Buffer size, size type */
		int    mBufferSizeType;
		double mBufferSize;
	};

}

#endif // end of __SYSFEATURELABELATTRIBUTE_H_89C0CCDE_DB24_4225_A021_8B4DEF17E715__
