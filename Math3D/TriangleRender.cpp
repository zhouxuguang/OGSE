#include "TriangleRender.h"
#include "ColorRGBA.h"

#include "AntiLine.h"

NS_MATH3D_BEGIN

extern void cgLineDDA(int x1,int y1,int x2,int y2,const ColorRGBA& color);

//渲染底部和轴对齐的三角形
extern void FillBottomTriangle(int x1,int y1,int x2,int y2,int x3,int y3,const ColorRGBA & color);

//渲染顶部和轴对齐的三角形
extern void FillTopTriangle(int x1,int y1,int x2,int y2,int x3,int y3,const ColorRGBA & color);

void FillBottomTriangle(int x1,int y1,int x2,int y2,int x3,int y3,const ColorRGBA & color)
{
	float fInvSlope1 = (x2 - x1)/float(y2 - y1);
	float fInvSlope2 = (x3 - x1)/float(y3 - y1);

	float fCurX1 = x1;
	float fCurX2 = x1;
	for (int nScaneLineY = y1; nScaneLineY <= y2; nScaneLineY++)
	{
		//cgLineDDA(fCurX1,nScaneLineY,fCurX2,nScaneLineY,color);
		ColorRGBA colorBk = ColorRGBA(0,0,0);
		cgAntiLineDDA(fCurX1,nScaneLineY,fCurX2,nScaneLineY,color,colorBk,1);
		fCurX1 += fInvSlope1;
		fCurX2 += fInvSlope2;
	}
}

void FillTopTriangle(int x1,int y1,int x2,int y2,int x3,int y3,const ColorRGBA & color)
{
	float fInvSlope1 = (x3 - x1)/float(y3 - y1);
	float fInvSlope2 = (x3 - x2)/float(y3 - y2);

	float fCurX1 = x3;
	float fCurX2 = x3;
	for (int nScaneLineY = y3; nScaneLineY > y1; nScaneLineY--)
	{
		//cgLineDDA(fCurX1,nScaneLineY,fCurX2,nScaneLineY,color);
		ColorRGBA colorBk = ColorRGBA(0,0,0);
		cgAntiLineDDA(fCurX1,nScaneLineY,fCurX2,nScaneLineY,color,colorBk,1);
		fCurX1 -= fInvSlope1;
		fCurX2 -= fInvSlope2;
	}
}

struct Pt
{
	int x;
	int y;

	bool operator <(const Pt &rhs) const
	{
		if (y < rhs.y)
		{
			return true;
		}

		return false;
	}
};

void FillTriangle(int x1,int y1,int x2,int y2,int x3,int y3,const ColorRGBA& color)
{
	//首先根据Y坐标从小到大排序
	std::vector<Pt> vecPt;
	vecPt.resize(3);
	vecPt[0].x = x1;
	vecPt[0].y = y1;
	vecPt[1].x = x2;
	vecPt[1].y = y2;
	vecPt[2].x = x3;
	vecPt[2].y = y3;
	std::sort(vecPt.begin(),vecPt.end());

	x1 = vecPt[0].x;
	y1 = vecPt[0].y;
	x2 = vecPt[1].x;
	y2 = vecPt[1].y;
	x3 = vecPt[2].x;
	y3 = vecPt[2].y;

	if (y2 == y3)
	{
		FillBottomTriangle(x1,y1,x2,y2,x3,y3,color);
	}

	else if (y1 == y2)
	{
		FillTopTriangle(x1,y1,x2,y2,x3,y3,color);
	}

	else
	{
		int x4 = x1 + float(y2 - y1)/float(y3 - y1) * (x3 - x1);
		int y4 = y2;
		FillBottomTriangle(x1,y1,x2,y2,x4,y4,color);
		FillTopTriangle(x2,y2,x4,y4,x3,y3,color);
	}
}

NS_MATH3D_END
