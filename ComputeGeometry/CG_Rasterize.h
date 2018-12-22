
/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: CG_Rasterize.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年11月4日
@ 内容摘要	: 矢量栅格化算法
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __CG_RASTERIZE_H_BF9B7E8F_69F3_44C4_8CB5_FD344EA626DD__
#define __CG_RASTERIZE_H_BF9B7E8F_69F3_44C4_8CB5_FD344EA626DD__

#include "CG_Common.h"

#ifdef  __cplusplus
extern "C" {
#endif

	COMPUTEGEOMETRY_API bool LineRasterize(
		SPOINT* poPoints,
		double dbMinx,
		double dbMaxy,
		double dx,
		double dy,
		unsigned short *podata,
		int nWidth,
		int nHeight);

#ifdef  __cplusplus
}
#endif


#endif // end of __CG_RASTERIZE_H_BF9B7E8F_69F3_44C4_8CB5_FD344EA626DD__
