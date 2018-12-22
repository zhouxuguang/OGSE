/**
* @file              SysFeatureLabel.h
* @brief             要素文本标注管理和绘制
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年10月16日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSFEATURELABEL_H_39598E62_04D0_4A4E_96A5_95BE1C89F48B__
#define __SYSFEATURELABEL_H_39598E62_04D0_4A4E_96A5_95BE1C89F48B__

#include "SysDrawCommon.h"

namespace SysDraw
{
	class SysFeature;
	class SysDrawText;
	class SysCoordinate;
	class SysRenderContext;
	class SysFeatureLabelAttribute;
	class SysLineString;
	class SysPolygon;
	class SysEnvelope;

	class SYSDRAW_API SysFeatureLabel
	{
	public:

		struct LabelPoint		//标注点
		{
			double m_dGeoX;
			double m_dGeoY;
			double m_dAngle;
		};

		SysFeatureLabel(void);

		~SysFeatureLabel(void);

		/**
		* @brief 绘制标注
		* @param[in] SysRenderContext *pRenderContext
		* @param[in] SysFeature *pFeature
		* @param[in] bool bSelected
		* @param[in] SysFeatureLabelAttribute *pLabelAttributes
		* @return void
		* @author 周旭光
		* @date 2015年10月16日
		* @note 
		*/
		void RenderFeature( 
						SysRenderContext *pRenderContext, 
						SysFeature *pFeature, 
						bool bSelected, 
						SysFeatureLabelAttribute *pLabelAttributes = NULL 
			);

		/**
		* @brief 设置用于标注的字段索引
		* @param[in] int nFieldIndex
		* @return void
		* @author 周旭光
		* @date 2015年10月16日
		* @note 
		*/
		void SetFieldIndex(int nFieldIndex);

		/**
		* @brief 获得用于标注字段的索引
		* @return int
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		int GetFieldIndex() const;

		/**
		* @brief 设置绘制标注时的属性
		* @param[in] SysFeatureLabelAttribute *pLabelAttributes
		* @return void
		* @author 周旭光
		* @date 2015年10月19日
		* @note 
		*/
		void SetLabelAttribute(SysFeatureLabelAttribute *pLabelAttributes);

		/**
		* @brief  获得标注时的属性
		* @return SysFeatureLabelAttribute
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		SysFeatureLabelAttribute *GetLabelAttribute() const;

	private:

		void GetLabelPoint( std::vector<LabelPoint>& points, SysFeature * pFeature );

		unsigned char* GetLabelPoint( std::vector<LabelPoint>& vecPoints, unsigned char* pWkbBuf, size_t nWkbLen );

		void RenderLabel( SysRenderContext *pRenderContext,
							double dGeoX,double dGeoY,
							std::string strText, SysDrawText &drawFont,
							double dXoffset, double dYoffset,
							double dAng,
							int nWidth, int nHeight, int nAlignment = 0 );

		/**
		* @brief 获得折线的标注点位
		* @param[in] SysLineString* pLine
		* @param[in] const char* pszFieldValue  字段值
		* @param[out] std::vector<LabelPoint>& points 得到的标注点
		* @param[in] const SysEnvelope& envView 视图范围
		* @param[out] std::vector<std::string>& vecLabelStr 得到的标注的字符串数组
		* @param[in] double dbScale 地图缩放比例系数
		* @return void
		* @author 周旭光
		* @date 2015年10月21日
		* @note 
		*/
		void GetLineStringLabelPoint(SysLineString* pLine, const char* pszFieldValue,const SysEnvelope& envView, 
			std::vector<LabelPoint>& points, std::vector<std::string>& vecLabelStr,double dbScale);

		/**
		* @brief 获得多边形的标注点位
		* @param[in] ysPolygon* pPolygon
		* @param[in] const char* pszFieldValue 字段值
		* @param[in] const SysEnvelope& envView
		* @param[in] std::vector<LabelPoint>& points 得到的标注点
		* @param[in] std::vector<std::string>& vecLabelStr 得到的标注的字符串数组
		* @param[in] double dbScale
		* @return void
		* @author 周旭光
		* @date 2015年10月22日
		* @note 
		*/
		void GetPolygonLabelPoint(
						SysPolygon* pPolygon, 
						const char* pszFieldValue,
						const SysEnvelope& envView, 
						std::vector<LabelPoint>& points, 
						std::vector<std::string>& vecLabelStr,
						double dbScale);

		void GetLabelPoint( SysFeature * pFeature, const SysEnvelope& envView, std::vector<LabelPoint>& points, std::vector<std::string> &vecLabelStr );

		int m_nFieldIndex;			//标注的字段索引

		SysFeatureLabelAttribute* m_pLabelAttributes;		//标注文本属性
	};

}

#endif // end of __SYSFEATURELABEL_H_39598E62_04D0_4A4E_96A5_95BE1C89F48B__
