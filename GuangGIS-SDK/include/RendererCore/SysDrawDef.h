/**
* @file              SysDrawDef.h
* @brief             基本类型和枚举定义
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年6月26日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSDRAWDEF_H_88E767D9_174D_4A95_98CC_AF04365CB3EF__
#define __SYSDRAWDEF_H_88E767D9_174D_4A95_98CC_AF04365CB3EF__


#if !defined(WIN32)

//窗口矩形定义
typedef struct tagRECT
{
	long    left;
	long    top;
	long    right;
	long    bottom;
} RECT;

//窗口坐标定义
/*typedef struct tagPOINT
{
	long  x;
	long  y;
} POINT;*/

//窗口大小定义
typedef struct tagSIZE
{
	long        cx;
	long        cy;
} SIZE;

#endif

typedef struct tagRECTD
{
	double    left;
	double    top;
	double    right;
	double    bottom;
} RECTD, *PRECTD;

//颜色表的一项
struct SysColorItem
{
	short  c1;   //红
    
	short  c2;    //绿

	short  c3;    //蓝

	short  c4;    //alpha
} ;

//以下是绘制的一些枚举类型

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 

#define HIST_SIZE 256.0


/** 
* 影像拉伸方式
*/
enum StretchType
{
	NoStretch = 0,							//没有拉伸
	StretchPercentOne,						//1%线性拉伸
	StretchPercentTwo,						//2%线性拉伸
	StretchPercentFive,						//5%线性拉伸
	StretchToMinimumMaximum,				//最大-最小值拉伸
	StretchStandardDeviations,				//标准差拉伸
	StretchOptimizedLinear,					//优化线性拉伸(ENVI默认显示方式) 
	UserDefinedStretch						//用户自定义拉伸
};

enum DrawStyle	//影像绘制方式
{
	UndefinedDrawingStyle,
	SingleBandGray,                 // 单波段按灰度绘制
	SingleBandPseudoColor,          // 单波段假彩色绘制
	PalettedColor,                  // 通过颜色表绘制
	MultiBandColor,                  //多波段RGB绘制
};


// 图元类型
enum GraphicsElementType
{
	type_SimplePoint,	// 简单点
	type_PolyLine,		// 不闭合折线
	type_CurveLine,		// 抛物曲线
	type_CloseLine,		// 闭合曲线
	type_Ploygon,		// 填充多边形
	type_FakeCor,		// 伪彩色图层
	type_Symbol,			// 符号图层
	type_Cycle,			// 圆形
	type_Rect,			// 矩形
	type_Text,			// 文本
	type_Arc,			// 圆形弧段
	type_Ellipse,		// 椭圆
	type_MultiElement,	// 多几何体图元
	type_MultiLine,		// 多折线
	type_MultiPolygon,	// 多多边形
	type_Contour			// 等值线
};

// 卷帘模式
enum enSwipeMode
{
	swipe_none = 0,		// 无
	swipe_horiz_left,	// 横向卷帘右至左
	swipe_horiz_right,	// 横向卷帘左至右
	swipe_vert_up,		// 纵向卷帘下至上
	swipe_vert_down		// 纵向卷帘上至下
};


#endif // end of __SYSDRAWDEF_H_88E767D9_174D_4A95_98CC_AF04365CB3EF__
