/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: CG_LineSmoothing.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年12月7日
@ 内容摘要	: 曲线光滑算法
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __CG_LINESMOOTHING_H_0C56DAAD_1FF2_49F2_BA92_7AEF8559D6B7__
#define __CG_LINESMOOTHING_H_0C56DAAD_1FF2_49F2_BA92_7AEF8559D6B7__

#include "CG_Common.h"

//线性迭代光滑法（抹角法）
COMPUTEGEOMETRY_API void WipeAngleMethod(SPOINT* pPoints,int nCount,int k);

//五点光滑
COMPUTEGEOMETRY_API bool FivePointsSmoothing(const SPOINT* poPoints,int nCount,double dbEps,std::vector<SPOINT>& vecResult);

//正轴抛物线曲线光滑
COMPUTEGEOMETRY_API bool OrthoaxisParabolaSmoothing(const SPOINT* poPoints,int nCount,double dbEps,std::vector<SPOINT>& vecResult);

#endif // end of __CG_LINESMOOTHING_H_0C56DAAD_1FF2_49F2_BA92_7AEF8559D6B7__
