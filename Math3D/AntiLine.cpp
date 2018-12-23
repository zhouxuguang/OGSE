#include "AntiLine.h"
#include "ColorRGBA.h"

#include <time.h>

NS_MATH3D_BEGIN

extern float *g_pfColorBuf;

extern int pfViewPort[4];

int GetSubPixelCount(float y1,int n,float m)
{
	float fConstant = sqrt(m*m + 1)*0.5;
	float fIncrement = 1.0/n;
	float xstart = fIncrement/2;
	float xstop = 1 - xstart;
	float ystart = y1  - 3*fIncrement;
	float ystop = y1 + 1 - fIncrement;

	int nHit = 0;
	for (float y = ystart; y <= ystop; y += fIncrement)
	{
		float yConst = y - y1;
		for (float x = xstart; x < xstop; x += fIncrement)
		{
			float temp = yConst - m*x;
			float tfu = temp - fConstant;
			float tfl = temp + fConstant;
			if (tfl <= 0 && tfu >= 0)
			{
				++nHit;
			}
		}
	}

	int a = CLOCKS_PER_SEC;

	return nHit;
}

void cgAntiLineDDA(int x1,int y1,int x2,int y2,const ColorRGBA& color,ColorRGBA& colorBk,float fWidth)
{
	int nLenght = 0;
	bool bFlagX = false;
	if (abs(x2-x1) >= abs(y2-y1))
	{
		bFlagX = true;
		nLenght = abs(x2-x1);
	}
	else
	{
		nLenght = abs(y2-y1);
	}

	//选择迭代单元大小
	float dx = (x2-x1)/float(nLenght);
	float dy = (y2-y1)/float(nLenght);

	float x = x1 + 0.5;
	float y = y1 + 0.5;

	float m = (y2-y1)/(float)(x2-x1);
	float fYWidth = (fWidth*sqrt(m*m+1));
	float fConstant = fYWidth*0.5;

	if (bFlagX)
	{
		int i = 1;
		while (i <= nLenght)
		{
			int nCol = (int)x;
			int nRow = (int)y;
			if(nCol >= 0 && nCol < pfViewPort[2] && nRow >= 0 && nRow < pfViewPort[3])
			{
				//计算上边边缘的点到所跨相邻的像素中心点的距离，然后着色
				float temp = y /*- m*x*/;
				float tfu = temp + fConstant;
				int nOuter = (int)(tfu + 1);
				int nInner = (int)(tfu);

				int nEndY = nInner;

				float d1 = fabs(tfu - (nOuter + 0.5));
				float d2 = fabs(tfu - (nInner + 0.5));

				ColorRGBA c1 = ColorRGBA(g_pfColorBuf + (pfViewPort[2]*nOuter+nCol)*4);

				ColorRGBA color1 = (c1*d1 + color * d2)/(d1+d2);
				g_pfColorBuf[(pfViewPort[2]*nOuter+nCol)*4] = color1[0];
				g_pfColorBuf[(pfViewPort[2]*nOuter+nCol)*4+1] = color1[1];
				g_pfColorBuf[(pfViewPort[2]*nOuter+nCol)*4+2] = color1[2];
				g_pfColorBuf[(pfViewPort[2]*nOuter+nCol)*4+3] = color1[3];

				//内部颜色

				ColorRGBA color2 = color * std::min((0.5+d2),1.0);
				g_pfColorBuf[(pfViewPort[2]*nInner+nCol)*4] = color2[0];
				g_pfColorBuf[(pfViewPort[2]*nInner+nCol)*4+1] = color2[1];
				g_pfColorBuf[(pfViewPort[2]*nInner+nCol)*4+2] = color2[2];
				g_pfColorBuf[(pfViewPort[2]*nInner+nCol)*4+3] = color2[3];


				//计算上边边缘的点到所跨相邻的像素中心点的距离，然后着色
				float tfl = temp - fConstant;

				nOuter = (int)(tfl - 1);
				nInner = (int)(tfl);

				int nStartY = nInner;

				float d3 = fabs(tfl - (nInner + 0.5));
				float d4 = fabs(tfl - (nOuter + 0.5));

				ColorRGBA c4 = ColorRGBA(g_pfColorBuf + (pfViewPort[2]*nOuter+nCol)*4);

				ColorRGBA color4 = (c4*d4 + color * d3)/(d3+d4);
				g_pfColorBuf[(pfViewPort[2]*nOuter+nCol)*4] = color4[0];
				g_pfColorBuf[(pfViewPort[2]*nOuter+nCol)*4+1] = color4[1];
				g_pfColorBuf[(pfViewPort[2]*nOuter+nCol)*4+2] = color4[2];
				g_pfColorBuf[(pfViewPort[2]*nOuter+nCol)*4+3] = color4[3];

				//内部颜色

				ColorRGBA color3 = color * std::min((0.5+d3),1.0);
				g_pfColorBuf[(pfViewPort[2]*nInner+nCol)*4] = color3[0];
				g_pfColorBuf[(pfViewPort[2]*nInner+nCol)*4+1] = color3[1];
				g_pfColorBuf[(pfViewPort[2]*nInner+nCol)*4+2] = color3[2];
				g_pfColorBuf[(pfViewPort[2]*nInner+nCol)*4+3] = color3[3];

				for (int nIndex = nStartY+1; nIndex < nEndY; nIndex++)
				{
					g_pfColorBuf[(pfViewPort[2]*nIndex+nCol)*4] = color[0];
					g_pfColorBuf[(pfViewPort[2]*nIndex+nCol)*4+1] = color[1];
					g_pfColorBuf[(pfViewPort[2]*nIndex+nCol)*4+2] = color[2];
					g_pfColorBuf[(pfViewPort[2]*nIndex+nCol)*4+3] = color[3];
				}
			}

			x += dx;
			y += dy;
			i += 1;
		}
	}

	
}


NS_MATH3D_END
