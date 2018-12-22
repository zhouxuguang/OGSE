/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2012
* 文件名称	: CG_LineSimplify.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年10月21日
* 内容摘要	: 曲线化简算法
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __CGLINESIMPLIFY_H_1DFB73A5_0295_4682_9A13_FAC2D641BB22__
#define __CGLINESIMPLIFY_H_1DFB73A5_0295_4682_9A13_FAC2D641BB22__

#include <vector>
#include "CG_Common.h"

#ifdef  __cplusplus
extern "C" {
#endif

	//道格拉斯-普克算法
	COMPUTEGEOMETRY_API void DouglasPeucker(
		CG_Point *poPoints, 
		bool* pFlags, 
		int nCount, 
		int nStart, 
		int nEnd, 
		double dbEps,
		std::vector<CG_Point> &vecPoint);

#ifdef  __cplusplus
}
#endif

#endif
