/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2013
* 文件名称	: CG_Polyline.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2013年10月26日
* 内容摘要	: 折线相关算法
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __CG_POLYLINE_H_461DA143_4CAB_4FC8_B364_0A0A885E0B17__
#define __CG_POLYLINE_H_461DA143_4CAB_4FC8_B364_0A0A885E0B17__


#include "CG_Envelope.h"

//折线数据结构
struct TPolyline
{
	SPOINT* points;		//点集
	int nCount;			//点数
	int nChainNum;		//单调链的个数
};

//单调链
struct TChain
{
	SPOINT* points;		//点集
	int nCount;			//点的个数
	CG_Envelope box;	//最小外包矩形
	bool bIncrement;	//是否递增，增为1，否则为0
	bool bAxisX;		//是否x方向
};

//线段相交结构体
struct Tline_Intersect
{
	int nLineNum;		//和本线段相交的数目
	int* poLines;		//线段列表
};

//扫描结构体
struct TSweep
{
	int nIndex;				//线段在单调链上的序号，有正负之分
	bool bIsFirstLine;		//是否在第一条线段上，是为1，否则为0
};

#ifdef __cplusplus
	extern "C" {
#endif

		//单调连是否相交
COMPUTEGEOMETRY_API void IsChianIntersect(const TChain& chain1,const TChain& chain2,Tline_Intersect &inter_info);

//点到线段的最短距离
COMPUTEGEOMETRY_API double Point2Segment(const SPOINT &point,SPOINT& pt0,SPOINT& pt1);

	//点到折线的最短距离
COMPUTEGEOMETRY_API double PointDisPolyline(const SPOINT& point,const SPOINT* poString,int nCount);


#ifdef __cplusplus
	}
#endif

#endif // end of __CG_POLYLINE_H_461DA143_4CAB_4FC8_B364_0A0A885E0B17__