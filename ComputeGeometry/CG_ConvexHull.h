/*******************************************************************************
* 版权所有(C) pyhcx 2012
* 文件名称	: ConvexHull.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年10月21日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __CONVEXHULL_H_73C855BC_30C3_4EC4_9374_C03505F2CC11__
#define __CONVEXHULL_H_73C855BC_30C3_4EC4_9374_C03505F2CC11__

/**********************************  头文件 ************************************/

//#include "CG_Common.h"
#include "CG_BaseCal.h"

COMPUTEGEOMETRY_API void Graham_scan(SPOINT *poPoints,SPOINT *ch,int nCount,int &len);

#endif // end of __CONVEXHULL_H_73C855BC_30C3_4EC4_9374_C03505F2CC11__
