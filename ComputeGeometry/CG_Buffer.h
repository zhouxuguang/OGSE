/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: CG_Buffer.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年11月4日
@ 内容摘要	: 缓冲区分析
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __CG_BUFFER_H_685DB7DA_07E0_4E78_9BB0_BE2B5D5089B2__
#define __CG_BUFFER_H_685DB7DA_07E0_4E78_9BB0_BE2B5D5089B2__

#include <vector>

#ifdef  __cplusplus
extern "C" {
#endif


	/*******************************************************************************
	@ 函数名称	: LineBuffer
	@ 功能描述	: 
	@ 参　　数	: SPOINT* poPoints
	@ 参　　数	: int nPointCount
	@ 参　　数	: double dbBuffer
	@ 参　　数	: std::vector<SPOINT>& vecResult
	@ 返 回 值	: void
	@ 作　　者	: guang
	@ 设计日期	: 2013年12月29日
	@ 修改日期		   修改人		  修改内容
	*******************************************************************************/
	COMPUTEGEOMETRY_API void LineBuffer(SPOINT* poPoints,int nPointCount,
		double dbBuffer,std::vector<SPOINT>& vecResult);

	/*******************************************************************************
	@ 函数名称	: ArcCycle
	@ 功能描述	: 圆弧拟合
	@ 参　　数	: int x0
	@ 参　　数	: int y0
	@ 参　　数	: double dbRadius
	@ 参　　数	: double dbThea
	@ 参　　数	: bool bClockWise
	@ 参　　数	: double ang1
	@ 参　　数	: double ang2
	@ 参　　数	: std::vector<CG_Point>& vecResult
	@ 返 回 值	: void
	@ 作　　者	: guang
	@ 设计日期	: 2013年12月29日
	@ 修改日期		   修改人		  修改内容
	*******************************************************************************/
	void ArcCycle(double x0, double y0, double dbRadius, double dbThea,bool bClockWise,
		double ang1, double ang2,std::vector<CG_Point>& vecResult);


#ifdef  __cplusplus
}
#endif

#endif // end of __CG_BUFFER_H_685DB7DA_07E0_4E78_9BB0_BE2B5D5089B2__

