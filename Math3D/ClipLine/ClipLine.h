#pragma once

#include "../Math3DCommon.h"
#include <bitset>

enum VisibleFlag
{
	Visible,		//可见
	NoVisible,		//完全不可见
	PartVisible		//部分可见
};

//线段端点编码
void EndCode(float x,float y,
			 float Window[4],
			 std::bitset<4>& bitCode);

//求两个端点编码的逻辑与操作是否为0
int Logical(const std::bitset<4> &pCode1,const std::bitset<4> &pCode2);

//获得二进制编码各位的和
int CodeSum(const std::bitset<4> &pCode);

VisibleFlag IsVisible(const std::bitset<4> &pCode1,
					  const std::bitset<4> &pCode2,
					  int nSum1,
					  int nSum2);

//线段裁剪，坐标有可能更新
MATH3D_API VisibleFlag CoSutherLineClip(float &x1,float &y1,float &x2,float &y2,
					  float Window[4]);


//中点分割算法
MATH3D_API VisibleFlag MidPointLineClip(float &x1,float &y1,float &x2,float &y2,
											 float Window[4]);
