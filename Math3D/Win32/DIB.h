#ifndef MATH32_DIB_INCLUDE
#define MATH32_DIB_INCLUDE



#include <windows.h>

DECLARE_HANDLE(HDIB);



class CDIB
{
public:
	CDIB(void);

public:
	~CDIB(void);

private:

	HDIB			hDib;				//图像句柄
	long			m_lWidth;			//图像宽度
	long			m_lHeight;			//图像高度
	long			m_lColor;			//图像颜色数

public:

	BOOL            m_bBackTrans;
	COLORREF		m_ColorPix;
	BYTE            m_TransLevel;

	HBITMAP m_hBitmap;	//位图句柄
	unsigned char* m_poData;

	BITMAPINFO bitinfo;

public:

	BOOL Create(long lWidth, long lHeight, long lColors);

	void SetBackTrans(BOOL bBackTrans, COLORREF color) { m_bBackTrans = bBackTrans; m_ColorPix = color;}

	BOOL Free();

	void GetSize(long lWidth, long lHeight);

	void GetColors(long lColors);

	//通道号按红、绿、蓝(1,2,3)的顺序排列
	BOOL SetImgData(long lChanelIndex, long lRow, long lCol, long lWidth, long lHeight, unsigned char *pData);

	BOOL GetImgData(long lChannelNum, long lRow, long lCol, long lWidth, long lHeight, unsigned char *pData);

	//调色板
	BOOL SetPalette(unsigned char *pucRed, unsigned char *pucGreen, unsigned char *pucBlue);

	BOOL GetPalette(unsigned char *pucRed, unsigned char *pucGreen, unsigned char *pucBlue);

	//通道号按红、绿、蓝(1,2,3)的顺序排列
	BOOL SetPixelBW(long lChannelNum, long lRow, long lCol, unsigned char ucValue);

	BOOL GetPixelBW(long lChannelNum, long lRow, long lCol, unsigned char *pucValue);

	//绘制
	BOOL Draw(HDC hDC, long lRow, long lCol, long lWidth, long lHeight, RECT rctDest);

	BOOL Draw(HDC hDC,tagRECT tagPosRect,long lRow, long lCol, long lWidth, long lHeight, RECT rctDest);
};



#endif