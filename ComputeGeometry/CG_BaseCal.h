/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2013
* 文件名称	: BaseCal.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年10月21日
* 内容摘要	: 几何运算基本运算
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __BASECAL_H_60C8CA6D_A89B_417E_9E1F_9997AC3E50BA__
#define __BASECAL_H_60C8CA6D_A89B_417E_9E1F_9997AC3E50BA__

/**********************************  头文件 ************************************/

#include "CG_Vector2.h"
#include "CG_Envelope.h"

#ifdef  __cplusplus
extern "C" {
#endif

/*******************************************************************************
@ 函数名称	: GetEnvelope
@ 功能描述	: 计算一组点的最小外包矩形
@ 参　　数	: const SPOINT* points
@ 参　　数	: int nCount
@ 参　　数	: CG_Envelope& env
@ 返 回 值	: void
@ 作　　者	: guang
@ 设计日期	: 2013年11月4日
@ 修改日期		   修改人		  修改内容
*******************************************************************************/
COMPUTEGEOMETRY_API void GetEnvelope(const SPOINT* points,int nCount,CG_Envelope& env);


/*******************************************************************************
@ 函数名称	: Distance
@ 功能描述	: 计算点p1和p2之间的欧几里得距离
@ 参　　数	: const SPOINT &p1
@ 参　　数	: const SPOINT &p2
@ 返 回 值	: double
@ 作　　者	: guang
@ 设计日期	: 2013年11月4日
@ 修改日期		   修改人		  修改内容
*******************************************************************************/
COMPUTEGEOMETRY_API double Distance(const SPOINT &p1,const SPOINT &p2); 


/*******************************************************************************
@ 函数名称	: DirectionAngle
@ 功能描述	: 计算线段p1p2的方向角,方向角从x正轴开始逆时针旋转的角度
@ 参　　数	: const SPOINT &p1
@ 参　　数	: const SPOINT &p2
@ 返 回 值	: double
@ 作　　者	: guang
@ 设计日期	: 2013年11月4日
@ 修改日期		   修改人		  修改内容
*******************************************************************************/
COMPUTEGEOMETRY_API double DirectionAngle(const SPOINT &p1,const SPOINT &p2);


/*******************************************************************************
@ 函数名称	: Equal_point
@ 功能描述	: 判断两个点是否重合
@ 参　　数	: SPOINT p1
@ 参　　数	: SPOINT p2
@ 返 回 值	: bool
@ 作　　者	: guang
@ 设计日期	: 2013年11月4日
@ 修改日期		   修改人		  修改内容
*******************************************************************************/
COMPUTEGEOMETRY_API bool Equal_point(SPOINT p1,SPOINT p2);  


/*******************************************************************************
@ 函数名称	: Multiply
@ 功能描述	: r=multiply(sp,ep,op),得到(sp-op)和(ep-op)的叉积 
				r>0：ep在矢量opsp的逆时针方向； 
				=0：opspep三点共线； 
				r<0：ep在矢量opsp的顺时针方向
@ 参　　数	: SPOINT sp
@ 参　　数	: SPOINT ep
@ 参　　数	: SPOINT op
@ 返 回 值	: double
@ 作　　者	: guang
@ 设计日期	: 2013年11月4日
@ 修改日期		   修改人		  修改内容
*******************************************************************************/
COMPUTEGEOMETRY_API double Multiply(SPOINT sp,SPOINT ep,SPOINT op);


/*******************************************************************************
@ 函数名称	: IsPointOnLine
@ 功能描述	: 判断点point是否在线段lineSeg上
@ 参　　数	: const SPOINT &point
@ 参　　数	: const SEGMENT& lineSeg
@ 返 回 值	: bool
@ 作　　者	: guang
@ 设计日期	: 2013年11月4日
@ 修改日期		   修改人		  修改内容
*******************************************************************************/
COMPUTEGEOMETRY_API bool IsPointOnLine(const SPOINT &point,const SEGMENT& lineSeg);

/*******************************************************************************
@ 函数名称	: IsPointOnLineEx
@ 功能描述	: 判断点point是否在线段上
@ 参　　数	: const SPOINT &point
@ 参　　数	: const SPOINT& p1
@ 参　　数	: const SPOINT& p2
@ 返 回 值	: bool
@ 作　　者	: guang
@ 设计日期	: 2013年11月4日
@ 修改日期		   修改人		  修改内容
*******************************************************************************/
COMPUTEGEOMETRY_API bool IsPointOnLineEx(const SPOINT &point,const SPOINT& p1,const SPOINT& p2);

/*******************************************************************************
@ 函数名称	: SplitLine
@ 功能描述	: 打断线段
@ 参　　数	: const SPOINT& p1
@ 参　　数	: const SPOINT& p2
@ 参　　数	: double dbDist
@ 返 回 值	: void
@ 作　　者	: guang
@ 设计日期	: 2013年11月4日
@ 修改日期		   修改人		  修改内容
*******************************************************************************/
COMPUTEGEOMETRY_API void SplitLine(const SPOINT& p1,const SPOINT& p2,double dbDist, SPOINT& ptResult);


/*******************************************************************************
@ 函数名称	: IsLineIntersect
@ 功能描述	: 判断线段lineSeg1和lineSeg2是否相交
@ 参　　数	: const SEGMENT& lineSeg1
@ 参　　数	: const SEGMENT& lineSeg2
@ 返 回 值	: bool
@ 作　　者	: guang
@ 设计日期	: 2013年11月4日
@ 修改日期		   修改人		  修改内容
*******************************************************************************/
COMPUTEGEOMETRY_API bool IsLineIntersect(const SEGMENT& lineSeg1,const SEGMENT& lineSeg2);

//求得线段的交点坐标.线段s1e1,s2e2，结果stResult
COMPUTEGEOMETRY_API void GetCrossPoint(const SPOINT& s1,const SPOINT& e1,const SPOINT& s2,const SPOINT& e2,SPOINT &stResult);

//三角形面积计算公式Heron公式 a b c为三边长
COMPUTEGEOMETRY_API double Heron(double a,double b,double c);

//计算点到直线的垂足距离(pt为点，startPt。endPt为直线)
COMPUTEGEOMETRY_API double PointToLine(const SPOINT &pt, const SPOINT &startPt, const SPOINT &endPt);

#ifdef  __cplusplus
}
#endif

#endif // end of __BASECAL_H_60C8CA6D_A89B_417E_9E1F_9997AC3E50BA__
