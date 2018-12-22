/**
* @file              SysRasterLayer.h
* @brief             栅格图层绘制类
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年6月26日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSRASTERLAYER_H_F506A0EA_5539_462E_BCD7_792CE362F075__
#define __SYSRASTERLAYER_H_F506A0EA_5539_462E_BCD7_792CE362F075__

#include "SysMapLayer.h"
#include "SysDrawDef.h"


namespace SysDataSource
{
	class IDataLayer;
}

namespace SysDraw
{
	class SysRasterSource;
	class SysRasterBandStats;

	typedef std::pair<double ,double> ValueInter;		//值区间

	class SYSDRAW_API SysRasterLayer : public SysMapLayer
	{
	public:
		explicit SysRasterLayer(std::string strLayerName);

		~SysRasterLayer(void);

		/**
		* @brief 设置数据源
		* @param[in] SysDataSource::IDataLayer* pDatalayer
		* @param[in] DrawProgressFunc ProgressFunc = NULL
		* @param[in] void* pProgressArg = NULL
		* @return bool
		* @author 周旭光
		* @date 2015年7月10日
		* @note 
		*/
		bool SetDataSource(SysDataSource::IDataLayer* pDatalayer,
							DrawProgressFunc ProgressFunc = NULL, 
							void* pProgressArg = NULL);

		/**
		* @brief 获得数据源
		* @return SysDataSource::IDataLayer*
		* @author 周旭光
		* @date 2015年7月10日
		* @note 
		*/
		SysDataSource::IDataLayer* GetDataSource() const;

		/**
		* @brief 创建栅格图层
		* @param[in] ysDataSource::IDataLayer* pDatalayer
		* @param[in] DrawProgressFunc ProgressFunc = NULL
		* @param[in] void* pProgressArg = NULL
		* @return SysRasterLayer* 返回创建的图层指针
		* @author 周旭光
		* @date 2015年7月1日
		* @note 
		*/
		static SysRasterLayer* CreateRasterLayer(
							SysDataSource::IDataLayer* pDatalayer,
							DrawProgressFunc ProgressFunc = NULL, 
							void* pProgressArg = NULL);

		/**
		* @brief 销毁图层
		* @param[in] SysRasterLayer* pRasLayer
		* @return void 
		* @author 周旭光
		* @date 2015年7月1日
		* @note 
		*/
		static void DestroyRasterLayer(SysRasterLayer* pRasLayer);

		/**
		* @brief 建立金字塔
		* @param[in] float fSampleRate
		* @param[in] int nSampleType
		* @param[in] DrawProgressFunc ProgressFunc = NULL
		* @param[in] void* pProgressArg = NULL
		* @return bool
		* @author 周旭光
		* @date 2015年7月1日
		* @note 
		*/
		bool BuildPyramid(float fSampleRate, int nSampleType, DrawProgressFunc ProgressFunc = NULL, void* pProgressArg = NULL);

		/**
		* @brief 获得RGB波段的序号索引
		* @param[out] int& nRedIndex
		* @param[out] int& nGreenIndex
		* @param[out] int& nBlueIndex
		* @return void
		* @author 周旭光
		* @date 2015年7月1日
		* @note 
		*/
		void GetRGBIndex(int& nRedIndex,int& nGreenIndex,int& nBlueIndex) const;

		/**
		* @brief  设置RGB波段的序号索引
		* @param[in] int nRedIndex
		* @param[in] int nGreenIndex
		* @param[in] int nBlueIndex
		* @return void
		* @author 周旭光
		* @date 2015年7月1日
		* @note 
		*/
		void SetRGBIndex(const int nRedIndex,const int nGreenIndex,const int nBlueIndex);

		/**
		* @brief 获得灰度显示的波段序号(还包括伪彩色渲染的波段序号)
		* @return int
		* @author 周旭光
		* @date 2015年9月28日
		* @note 
		*/
		int GetGrayIndex() const;

		/**
		* @brief 设置灰度显示的波段序号
		* @param[in] const int nGrayIndex
		* @return void
		* @author 周旭光
		* @date 2015年10月27日
		* @note 
		*/
		void SetGrayIndex(const int nGrayIndex);

		/**
		* @brief 获得波段数量
		* @return int
		* @author 周旭光
		* @date 2015年7月1日
		* @note 
		*/
		int GetBandCount() const;

		/**
		* @brief 获得各个波段的数据类型
		* @return std::vector<SysRasDataType> 各个波段的数据类型
		* @author 周旭光
		* @date 2015年11月23日
		* @par Sample code
		* @code @endcode
		* @par Output
		* @see 
		* @note 
		*/
		std::vector<SysRasDataType> GetBandDataType() const; 

		/**
		* @brief 获得是几倍标准差拉伸
		* @return float
		* @author 周旭光
		* @date 2015年7月1日
		* @note 
		*/
		float GetStandardDeviations() const;

		/**
		* @brief 设置标准差拉伸的几倍标准差参数
		* @param[in] float theStandardDeviations
		* @return void
		* @author 周旭光
		* @date 2015年7月1日
		* @note 
		*/
		void SetStandardDeviations(float fStandardDeviations );

		/**
		* @brief 设置图层的透明度
		* @param[in] int nTransparency
		* @return void
		* @author 周旭光
		* @date 2015年7月24日
		* @note 
		*/
		void SetTransparency(int nTransparency);

		/**
		* @brief 获得透明度，返回值是0-255之间
		* @return eByte
		* @author 周旭光
		* @date 2015年9月29日
		* @note 
		*/
		eByte GetTransparency() const;

		/**
		* @brief 设置亮度系数
		* @param[in] float fBrightness
		* @return void
		* @author 周旭光
		* @date 2015年7月24日
		* @note 
		*/
		void SetBrightness(float fBrightness);

		/**
		* @brief 设置图像对比度
		* @param[in] int nValue
		* @return void
		* @author 周旭光
		* @date 2015年9月16日
		* @note 
		*/
		void SetContrastValue(int nValue);

		virtual bool Draw( SysRenderContext* rendererContext );

		virtual bool Draw( SysRenderContext* rendererContext ,const SysEnvelope& envView);

		virtual void DrawLabels( SysRenderContext* rendererContext );

		/**
		* @brief 是否有颜色反转
		* @return bool
		* @author 周旭光
		* @date 2015年7月24日
		* @note 
		*/
		bool IsInvertColor() const;

		/**
		* @brief 设置是否颜色反转
		* @param[in] bool bInvert
		* @return void
		* @author 周旭光
		* @date 2015年7月24日
		* @note 
		*/
		void SetInvertColor(bool bInvert);

		/**
		* @brief 获得绘制风格,灰度、RGB等
		* @return DrawStyle
		* @author 周旭光
		* @date 2015年9月28日
		* @note 
		*/
		DrawStyle GetDrawStyle() const;

		/**
		* @brief 设置绘制风格,灰度、RGB等
		* @param[in] DrawStyle eStyle
		* @return void
		* @author 周旭光
		* @date 2015年9月28日
		* @note 
		*/
		void SetDrawStyle(DrawStyle eStyle);

		/**
		* @brief 获得栅格图层的拉伸方式
		* @return StretchType
		* @author 周旭光
		* @date 2015年9月6日
		* @note 
		*/
		StretchType GetStretchType() const;

		/**
		* @brief 设置栅格图层的拉伸方式
		* @param[in] StretchType eType
		* @return void
		* @author 周旭光
		* @date 2015年9月6日
		* @note 
		*/
		void SetStretchType(StretchType eType);

		/**
		* @brief 计算某个波段的详细统计信息
		* @param[in] const int nBandIndex
		* @return void
		* @author 周旭光
		* @date 2015年9月28日
		* @note 
		*/
		void CaclBandStatusInfo(const int nBandIndex) const;

		/**
		* @brief 获得某个波段的详细统计信息
		* @param[in] const int nBandIndex
		* @return const SysRasterBandStats*
		* @author 周旭光
		* @date 2015年9月28日
		* @note 
		*/
		const SysRasterBandStats* GetBandStatusInfo(const int nBandIndex) const;

		/**
		* @brief 获得图像数据的显示值(对于单波段灰度显示模式,颜色表模式以及伪彩色模式)
		* @param[in] double dbValue
		* @param[out] eByte *pRGBAValues 4个元素的数组，排列顺序是RGBA
		* @return void
		* @author 周旭光
		* @date 2015年9月29日
		* @note 
		*/
		void GetDisplayValue(double dbValue,eByte *pRGBAValues) const;

		/**
		* @brief 获得图像数据的显示值(对于多波段彩色显示模式)
		* @param[in] std::vector<double> dbValue 图像数据的三元组，排列是RGB波段的顺序
		* @param[out] eByte *pRGBAValues 4个元素的数组，排列顺序是RGBA
		* @return void
		* @author 周旭光
		* @date 2015年9月29日
		* @note 
		*/
		void GetDisplayValue(std::vector<double> dbValue,eByte *pRGBAValues) const;

		/**
		* @brief 是否有RPC模型
		* @return bool
		* @author 周旭光
		* @date 2015年9月29日
		* @note 
		*/
		bool HaveRPCModel() const;

		/**
		* @brief SetPseudoColorArray
		* @param[in] std::vector<SysColorItem>& vecPseudoColor
		* @return void
		* @author 周旭光
		* @date 2015年10月26日
		* @note 
		*/
		void SetPseudoColorArray(std::vector<SysColorItem>& vecPseudoColor);

		/**
		* @brief SetPseudoColorValues
		* @param[in] std::vector<ValueInter>& vecValueInters
		* @return void
		* @author 周旭光
		* @date 2015年10月26日
		* @note 
		*/
		void SetPseudoColorValues(std::vector<ValueInter>& vecValueInters);

	private:
		void Init();

		void SetRenderCorTable(const std::vector<SysColorItem>& CorTable);

		void CalcStatisInfo( DrawStyle eDrawStyle);

		//计算某个波段的统计信息，主要是为渲染服务
		void CalcDrawStatisInfo( DrawStyle eDrawStyle,const int nBandIndex);

		void DrawImage( void * thePainter,
			SysRenderContext* rendererContext,
			const SysEnvelope& envRaster ) const;

		//绘制单波段灰度影像
		void DrawSingleBandGray(
			void* thePainter,
			SysRenderContext* rendererContext,
			const RECTD& rectImage,
			const RECTD& rectView, const SysEnvelope& envDrawView) const;

		//绘制多波段影像
		void DrawMultiBandColor(
			void* thePainter,
			SysRenderContext* rendererContext,
			const RECTD& rectImage,
			const RECTD& rectView, const SysEnvelope& envDrawView) const;

		//绘制单波段颜色表影像
		void DrawPalettedSingleBandColor(
			void* thePainter,
			SysRenderContext* rendererContext,
			const RECTD& rectImage,
			const RECTD& rectView, const SysEnvelope& envDrawView) const;

		//绘制单波段（伪彩色模式显示）
		void DrawSingleBandPseudoColor(
			void* thePainter,
			SysRenderContext* rendererContext,
			const RECTD& rectImage,
			const RECTD& rectView, const SysEnvelope& envDrawView) const;

		bool GetDrawStatisInfo(int nBandIndex,float* pfHist,float& fMax,float& fMin,float& fMean,float& fVar,int &nCount);

	private:
		mutable int m_nRedIndex;				//红色波段索引
		mutable int m_nGreenIndex;				//绿色波段索引
		mutable int m_nBlueIndex;				//蓝色波段索引

		int m_nBandCount;					//band count

		int m_nWidth;					//影像宽度
		int m_nHeight;					//影像高度

		int m_nGrayIndex;				//灰度显示的波段序号

		int m_nTransparency;			//透明度(0-100)
		float m_fBrightness;				//亮度(-1,1)
		int m_nContrast;				//对比度参数(-128,128)

		std::vector<SysColorItem>	m_vecColorTbl;

		DrawStyle m_eDrawStyle;			//影像绘制方式
		StretchType m_eStretchType;		//影像拉伸方式
		bool m_bInvertColor;			//是否翻转显示颜色
		bool m_bHasPyramids;			//是否有金字塔
		bool m_bDynamicProj;			//是否动态投影
		double m_dbNoDataValue;			//无效值
		float m_fStandardDeviations;	//几倍标准差

		SysDataSource::IDataLayer* m_pDatalayer;		//对应的数据图层
		SysRasterSource* m_pDataSource;

		eUInt32 m_nTexId;

		mutable std::vector<SysRasterBandStats* > m_vecBandStatus;		//各个波段的统计信息

		std::vector<ValueInter> m_vecValueInters;
		std::vector<SysColorItem> m_vecPseudoColor;			//假彩色颜色表

	private:

		float m_pfMin[3];
		float m_pfMax[3];
		float m_pfMean[3];
		float m_pfVar[3];

		float m_pHist[256*3];

	};

}


#endif // end of __SYSRASTERLAYER_H_F506A0EA_5539_462E_BCD7_792CE362F075__
