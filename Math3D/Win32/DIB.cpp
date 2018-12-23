
#include "DIB.h"


#include <WindowsX.h>


#pragma comment(lib,"msimg32")


	#define IS_WIN30_DIB(lpbi)  ((*(unsigned long*)(lpbi)) == sizeof(BITMAPINFOHEADER)) 
	#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 
	#define RECT_WIDTH(rctRect)		(rctRect.right - rctRect.left + 1)
	#define RECT_HEIGHT(rctRect)	(rctRect.bottom - rctRect.top + 1)

	static DWORD WINAPI DIBHeight(LPSTR lpDIB)
	{
		LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
		LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

		lpbmi = (LPBITMAPINFOHEADER)lpDIB;
		lpbmc = (LPBITMAPCOREHEADER)lpDIB;

		if (IS_WIN30_DIB(lpDIB))
			return lpbmi->biHeight;
		else
			return (DWORD)lpbmc->bcHeight;
	}

	static WORD WINAPI DIBNumColors(LPSTR lpbi)
	{
		WORD wBitCount;  // DIB bit count

		if (IS_WIN30_DIB(lpbi))
		{
			DWORD dwClrUsed;

			dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
			if (dwClrUsed != 0)
				return (WORD)dwClrUsed;
		}

		if (IS_WIN30_DIB(lpbi))
			wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
		else
			wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;

		switch (wBitCount)
		{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
		}
	}

	static WORD WINAPI PaletteSize(LPSTR lpbi)
	{
		if (IS_WIN30_DIB (lpbi))
			return (WORD)(DIBNumColors(lpbi) * sizeof(RGBQUAD));
		else
			return (WORD)(DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
	}

	static LPSTR WINAPI FindDIBBits(LPSTR lpbi)
	{
		return (lpbi + *(LPDWORD)lpbi + PaletteSize(lpbi));
	}

	CDIB::CDIB(void)
	{
		hDib = NULL;
		m_TransLevel=0;
		m_ColorPix = RGB(0,0,0);
		m_bBackTrans = TRUE;
		m_hBitmap = NULL;
		m_poData = NULL;
	}

	CDIB::~CDIB(void)
	{
		Free();
	}

	BOOL CDIB::Create(long nWidth, long nHeight, long lColors)
	{
		m_lColor = lColors;
		m_lWidth = nWidth;
		m_lHeight = nHeight;


		long lDibHeadSize;	// Dib 头大小
		long lDibDataSize;	// Dib 数据大小

		long lBitCount;		// 颜色位面
		long lRowBytes;		// 每一行字节数

		if (0 == lColors)
			lBitCount = 24;
		else if (256 == lColors)
			lBitCount = 8;
		else 
			lBitCount = 24;

		lRowBytes = WIDTHBYTES(nWidth * lBitCount);

		lDibHeadSize = sizeof(BITMAPINFOHEADER) + lColors * sizeof(RGBQUAD);
		lDibDataSize = lRowBytes * nHeight;		//数据字节数

		/*m_poData = new unsigned char[lDibDataSize];
		memset(m_poData,0,sizeof(unsigned char)*lDibDataSize);*/

		//构造位图信息头
		
		memset(&bitinfo, 0, sizeof(BITMAPINFO));

		bitinfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
		bitinfo.bmiHeader.biWidth         = nWidth;     
		bitinfo.bmiHeader.biHeight        = nHeight;   
		bitinfo.bmiHeader.biPlanes        = 1;  
		bitinfo.bmiHeader.biBitCount      = lBitCount;   
		bitinfo.bmiHeader.biCompression   = BI_RGB;   
		bitinfo.bmiHeader.biSizeImage     = lRowBytes*nHeight;  
		bitinfo.bmiHeader.biXPelsPerMeter = 96;
		bitinfo.bmiHeader.biYPelsPerMeter = 96;   
		bitinfo.bmiHeader.biClrUsed       = 0;   
		bitinfo.bmiHeader.biClrImportant  = 0;

		HWND hWnd = GetDesktopWindow();
		HDC hDC = GetDC(hWnd);

		//BYTE *pPixel = NULL;
		m_hBitmap = ::CreateDIBSection(hDC,&bitinfo,DIB_RGB_COLORS,(void**)&m_poData,NULL,0);
		//memcpy(pPixel,m_poData,lRowBytes*nHeight);

		ReleaseDC(hWnd,hDC);


		return TRUE;
	}

	BOOL CDIB::Free()
	{
		if (NULL != hDib)
		{
			::GlobalFree((HGLOBAL)hDib);
			hDib = NULL;
		}

		if (m_hBitmap != NULL)
		{
			DeleteObject(m_hBitmap);
		}

		if (m_poData != NULL)
		{
			//delete []m_poData;
			m_poData = NULL;
		}
		return TRUE;
	}

	void CDIB::GetSize(long lWidth, long lHeight)
	{
		lWidth = m_lWidth;
		lHeight = m_lHeight;
	}

	void CDIB::GetColors(long lColors)
	{
		lColors = m_lColor;
	}

// 通道号按红、绿、蓝(1,2,3)的顺序排列
	BOOL CDIB::SetImgData(long lChanelIndex, long lRow, long lCol, long lWidth, long lHeight, unsigned char *pData)
	{
		long lIndex = 0;
		unsigned char* lpData = NULL;

		long lBitCount;		// 颜色位面
		switch(m_lColor)
		{
		case 0:
			{
				// rgb图像
				lBitCount = 24;
				long RowEndPos = lRow + lHeight;
				long ColEndPos = lCol + lWidth;
				long lRowBytes = WIDTHBYTES(m_lWidth * lBitCount);		// 每一行字节数
				long lIndex = 0;
				for (int i = lRow; i<RowEndPos; i ++)
				{
					for (int j = lCol; j<ColEndPos; j ++)
					{
						m_poData[lRowBytes * (m_lHeight - i) + (j - 1) * 3 + (2 - lChanelIndex)] = pData[lIndex++];
					}
				}
				break;
			}
		case 256:
			{
				lBitCount = 8;

				long lIndex = 0;
				long RowEndPos = lRow + lHeight;
				long ColEndPos = lCol + lWidth;
				long lRowBytes = WIDTHBYTES(m_lWidth * lBitCount);		// 每一行字节数
				for (int i = lRow;i<RowEndPos; i ++)
				{
					for (int j = lCol; j<ColEndPos;j ++)
					{
						m_poData[lRowBytes * (m_lHeight - i) + j - 1] = pData[lIndex ++];
					}
				}
				break;
			}
		default:
			return FALSE;
		}
		return TRUE;
	}

	BOOL CDIB::GetImgData(long lChannelNum, long lRow, long lCol, long lWidth, long lHeight, unsigned char *pData)
	{
		long lDibHeadSize;	// Dib 头大小
		long lBitCount;		// 颜色位面
		long lRowBytes;		// 每一行字节数
		long lIndex = 0;
		LPSTR lpData = NULL;

		if (0 == m_lColor)
			lBitCount = 24;
		else if (256 == m_lColor)
			lBitCount = 8;
		else 
			return FALSE;

		lRowBytes = WIDTHBYTES(m_lWidth * lBitCount);
		lDibHeadSize = sizeof(BITMAPINFOHEADER) + m_lColor * sizeof(RGBQUAD);

		lpData = (LPSTR)::GlobalLock((HGLOBAL)hDib) + lDibHeadSize;

		if (256 == m_lColor)
		{
			for (int i = lRow; i < lRow + lHeight; i ++)
			{
				for (int j = lCol; j < lCol + lWidth; j ++)
				{
					pData[lIndex ++] = lpData[lRowBytes * (m_lHeight - i) + j - 1];
				}
			}
		}
		else if (0 == m_lColor)
		{
			for (int i = lRow; i < lRow + lHeight; i ++)
			{
				for (int j = lCol; j < lCol + lWidth; j ++)
				{
					pData[lIndex ++] = lpData[lRowBytes * (m_lHeight - i) + (j - 1) * 3 + (3 - lChannelNum)];
				}
			}
		}
		::GlobalUnlock((HGLOBAL)hDib);
		return TRUE;
	}

	//调色板
	BOOL CDIB::SetPalette(unsigned char *pucRed, unsigned char *pucGreen, unsigned char *pucBlue)
	{
		LPSTR pDIB;
		pDIB = (LPSTR)::GlobalLock((HGLOBAL)hDib);
		PBITMAPINFO pDibInfo = (PBITMAPINFO)pDIB;

		long lColor = 0;
		GetColors(lColor);

		for (int i = 0; i < lColor; i++)
		{
			pDibInfo->bmiColors[i].rgbRed = pucRed[i];
			pDibInfo->bmiColors[i].rgbGreen = pucGreen[i];
			pDibInfo->bmiColors[i].rgbBlue = pucBlue[i];
		}

		::GlobalUnlock((HGLOBAL)hDib);

		return TRUE;
	}

	BOOL CDIB::GetPalette(unsigned char *pucRed, unsigned char *pucGreen, unsigned char *pucBlue)
	{
		LPSTR pDIB;
		pDIB = (LPSTR)::GlobalLock((HGLOBAL)hDib);
		PBITMAPINFO pDibInfo = (PBITMAPINFO)pDIB;

		long lColor = 0;
		GetColors(lColor);

		for (int i = 0; i < lColor; i++)
		{
			pucRed[i] = pDibInfo->bmiColors[i].rgbRed;
			pucGreen[i] = pDibInfo->bmiColors[i].rgbGreen;
			pucBlue[i] = pDibInfo->bmiColors[i].rgbBlue;
		}
		::GlobalUnlock((HGLOBAL)hDib);

		return TRUE;
	}

	//通道号按红、绿、蓝(1,2,3)的顺序排列
	BOOL CDIB::SetPixelBW(long lChannelNum, long lRow, long lCol, unsigned char ucValue)
	{
		long lDibHeadSize;	// Dib 头大小
		long lBitCount;		// 颜色位面
		long lRowBytes;		// 每一行字节数
		LPSTR lpData = NULL;

		if (0 == m_lColor)
			lBitCount = 24;
		else if (256 == m_lColor)
			lBitCount = 8;
		else 
			return FALSE;

		lRowBytes = WIDTHBYTES(m_lWidth * lBitCount);
		lDibHeadSize = sizeof(BITMAPINFOHEADER) + m_lColor * sizeof(RGBQUAD);

		lpData = (LPSTR)::GlobalLock((HGLOBAL)hDib) + lDibHeadSize;

		if (256 == m_lColor)
		{
			lpData[lRowBytes * (m_lHeight - lRow) + lCol - 1] = ucValue;
		}
		else if (0 == m_lColor)
		{
			lpData[lRowBytes * (m_lHeight - lRow) + (lCol - 1) * 3 + (3 - lChannelNum)] = ucValue;
		}

		::GlobalUnlock((HGLOBAL)hDib);

		return TRUE;
	}

	BOOL CDIB::GetPixelBW(long lChannelNum, long lRow, long lCol, unsigned char *pucValue)
	{
		long lDibHeadSize;	// Dib 头大小
		long lBitCount;		// 颜色位面
		long lRowBytes;		// 每一行字节数
		LPSTR lpData = NULL;

		if (0 == m_lColor)
			lBitCount = 24;
		else if (256 == m_lColor)
			lBitCount = 8;
		else 
			return FALSE;
		lRowBytes = WIDTHBYTES(m_lWidth * lBitCount);
		lDibHeadSize = sizeof(BITMAPINFOHEADER) + m_lColor * sizeof(RGBQUAD);

		lpData = (LPSTR)::GlobalLock((HGLOBAL)hDib) + lDibHeadSize;

		if (256 == m_lColor)
		{
			*pucValue = lpData[lRowBytes * (m_lHeight - lRow) + lCol - 1];
		}
		else if (0 == m_lColor)
		{
			*pucValue = lpData[lRowBytes * (m_lHeight - lRow) + (lCol - 1) * 3 + (3 - lChannelNum)];
		}

		::GlobalUnlock((HGLOBAL)hDib);
		return TRUE;
	}

	//绘制
	BOOL CDIB::Draw(HDC hDC,long lRow, long lCol, long lWidth, long lHeight, RECT rctDest)
	{
		RECT rctSrc;

		rctSrc.left = lCol - 1;  //图象数据的左上角： 列
		rctSrc.top = m_lHeight - lRow - lHeight + 1;	 //图象数据的左上角： 行
		rctSrc.right = lCol -1 + lWidth - 1;
		rctSrc.bottom = rctSrc.top + lHeight - 1;

		if ((rctSrc.left > m_lWidth) && (rctSrc.top > m_lHeight) && (rctSrc.right > m_lWidth) && (rctSrc.bottom > m_lHeight))
		{
			return FALSE;
		}

		BOOL     bSuccess=FALSE;      // Success/fail flag
		HPALETTE hPal=NULL;           // Our DIB's palette
		HPALETTE hOldPal=NULL;        // Previous palette

		::SetStretchBltMode(hDC,COLORONCOLOR);
		
		{
			HDC hdcMem = NULL;
			HBITMAP hBitmap = NULL;
			HBITMAP hOldBitmap1 = NULL;

			BLENDFUNCTION stBF;
			hdcMem = ::CreateCompatibleDC(hDC);
			hBitmap = ::CreateCompatibleBitmap(hDC,m_lWidth,m_lHeight); 
			hOldBitmap1 = SelectBitmap(hdcMem,hBitmap);



			if ((RECT_WIDTH(rctSrc) == RECT_WIDTH(rctDest)) && (RECT_HEIGHT(rctSrc) == RECT_HEIGHT(rctDest)))
				bSuccess = ::SetDIBitsToDevice(hdcMem,							// hDC
				0,								// DestX
				0, 							    // DestY
				RECT_WIDTH(rctDest),				// nDestWidth
				RECT_HEIGHT(rctDest),			// nDestHeight
				rctSrc.left,					// SrcX
				(int)m_lHeight -
				rctSrc.top-RECT_HEIGHT(rctSrc),				// SrcY
				0,								// nStartScan
				(WORD)m_lHeight,		// nNumScans
				m_poData,						// lpBits
				(LPBITMAPINFO)&bitinfo,			// lpBitsInfo
				DIB_RGB_COLORS);				// wUsage

			stBF.BlendOp = AC_SRC_OVER;
			stBF.BlendFlags = 0;
			stBF.AlphaFormat = 0 ;
			stBF.SourceConstantAlpha = 255-m_TransLevel;
			::AlphaBlend(hDC,rctDest.left,rctDest.top,lWidth,lHeight,hdcMem,0,0,lWidth,lHeight, stBF);

			SelectBitmap(hDC,hOldBitmap1);
			DeleteBitmap(hBitmap);
			::DeleteDC(hdcMem);
		}

		return TRUE;
	}

	//BOOL CDIB::Draw(HDC hDC,tagRECT tagPosRect,long lRow,long lCol,long lWidth, long lHeight, RECT rctDest)
	//{
	//	RECT rctSrc;
	//	rctSrc.left = lCol - 1;                          //图象数据的左上角： 列
	//	rctSrc.top = m_lHeight - lRow - lHeight + 1;	 //图象数据的左上角： 行
	//	rctSrc.right = lCol -1 + lWidth - 1;
	//	rctSrc.bottom = rctSrc.top + lHeight - 1;

	//	//计算贴图区域
	//	long lDestX = tagPosRect.left + rctDest.left;
	//	long lDestY = tagPosRect.top + rctDest.top;

	//	if ((rctSrc.left > m_lWidth) && (rctSrc.top > m_lHeight) && (rctSrc.right > m_lWidth) && (rctSrc.bottom > m_lHeight))
	//	{
	//		return FALSE;
	//	}

	//	LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
	//	LPSTR    lpDIBBits;           // Pointer to DIB bits
	//	BOOL     bSuccess=FALSE;      // Success/fail flag
	//	HPALETTE hPal=NULL;           // Our DIB's palette
	//	HPALETTE hOldPal=NULL;        // Previous palette

	//	if (NULL == hDib)
	//	{
	//		return FALSE;
	//	}

	//	lpDIBHdr  = (LPSTR)::GlobalLock((HGLOBAL)hDib);
	//	lpDIBBits = FindDIBBits(lpDIBHdr);

	//	::SetStretchBltMode(hDC, COLORONCOLOR);
	//	if(m_bBackTrans)
	//	{
	//		HDC hdcTrans = ::CreateCompatibleDC(hDC);
	//		HDC hdcMem = ::CreateCompatibleDC(hdcTrans);
	//		HBITMAP hBmp = ::CreateCompatibleBitmap(hDC,lWidth,lHeight);
	//		HBITMAP hTranBmp = ::CreateCompatibleBitmap(hDC,lWidth,lHeight);
	//		HBITMAP hOldBmp = SelectBitmap(hdcMem,hBmp);
	//		HBITMAP hOldTranBmp = SelectBitmap(hdcTrans,hTranBmp);

	//		if ((RECT_WIDTH(rctSrc) == RECT_WIDTH(rctDest)) && (RECT_HEIGHT(rctSrc) == RECT_HEIGHT(rctDest)))
	//			bSuccess = ::SetDIBitsToDevice(hdcMem,	 // hDC
	//			0,							                // DestX
	//			0,								            // DestY
	//			RECT_WIDTH(rctDest),						    // nDestWidth
	//			RECT_HEIGHT(rctDest),					    // nDestHeight
	//			rctSrc.left,							    // SrcX
	//			(int)DIBHeight(lpDIBHdr) -
	//			rctSrc.top -
	//			RECT_HEIGHT(rctSrc),						    // SrcY
	//			0,										    // nStartScan
	//			(WORD)DIBHeight(lpDIBHdr),				    // nNumScans
	//			lpDIBBits,								    // lpBits
	//			(LPBITMAPINFO)lpDIBHdr,					    // lpBitsInfo
	//			DIB_RGB_COLORS);						    // wUsage

	//		HDC hdcMask = ::CreateCompatibleDC(hDC);
	//		HBITMAP hMaskBmp = ::CreateBitmap(lWidth,lHeight,1,1,NULL);
	//		HBITMAP hOldMaskBmp = SelectBitmap(hdcMask,hMaskBmp);
	//		::SetBkColor(hdcMem,m_ColorPix);
	//		::BitBlt(hdcMask,0,0,lWidth,lHeight,hdcMem,0,0,SRCCOPY);
	//		::BitBlt(hdcTrans,0,0,lWidth,lHeight,hDC,lDestX,lDestY,SRCCOPY);
	//		::BitBlt(hdcTrans,0,0,lWidth,lHeight,hdcMem,0,0,SRCINVERT);
	//		::BitBlt(hdcTrans,0,0,lWidth,lHeight,hdcMask,0,0,SRCAND);
	//		::BitBlt(hdcTrans,0,0,lWidth,lHeight,hdcMem,0,0,SRCINVERT);

	//		BLENDFUNCTION stBF;
	//		stBF.BlendOp = AC_SRC_OVER;
	//		stBF.BlendFlags = 0;
	//		stBF.SourceConstantAlpha = 255-m_TransLevel;
	//		stBF.AlphaFormat = 0 ;
	//		::AlphaBlend(hDC,lDestX, lDestY, lWidth, lHeight,hdcTrans,0,0,lWidth,lHeight,stBF);

	//		SelectBitmap(hdcTrans,hOldTranBmp);
	//		DeleteBitmap(hTranBmp);
	//		::DeleteDC(hdcTrans);

	//		SelectBitmap(hdcMem,hOldMaskBmp);
	//		DeleteBitmap(hMaskBmp);
 //           ::DeleteDC(hdcMask);
	//		::DeleteDC(hdcMem);
	//	}
	//	else
	//	{
	//		HDC hdcMem = ::CreateCompatibleDC(hDC);
	//		HBITMAP hMaskBmp = ::CreateCompatibleBitmap(hDC,m_lWidth,m_lHeight);
	//		HBITMAP hOldMaskBmp = SelectBitmap(hdcMem,hMaskBmp);

	//		if ((RECT_WIDTH(rctSrc) == RECT_WIDTH(rctDest)) && (RECT_HEIGHT(rctSrc) == RECT_HEIGHT(rctDest)))
	//			bSuccess = ::SetDIBitsToDevice(hdcMem,		 // hDC
	//			0,							                // DestX
	//			0, 							                // DestY
	//			RECT_WIDTH(rctDest),					        // nDestWidth
	//			RECT_HEIGHT(rctDest),					    // nDestHeight
	//			rctSrc.left,							    // SrcX
	//			(int)DIBHeight(lpDIBHdr) -
	//			rctSrc.top -
	//			RECT_HEIGHT(rctSrc),						    // SrcY
	//			0,										    // nStartScan
	//			(WORD)DIBHeight(lpDIBHdr),				    // nNumScans
	//			lpDIBBits,								    // lpBits
	//			(LPBITMAPINFO)lpDIBHdr,					    // lpBitsInfo
	//			DIB_RGB_COLORS);						    // wUsage

	//		BLENDFUNCTION stBF;
	//		stBF.BlendOp = AC_SRC_OVER;
	//		stBF.BlendFlags = 0;
	//		stBF.AlphaFormat = 0 ;
	//		stBF.SourceConstantAlpha = 255-m_TransLevel;
	//		::AlphaBlend(hDC,lDestX,lDestY,lWidth,lHeight,hdcMem,0,0,lWidth,lHeight,stBF);

	//		SelectBitmap(hdcMem,hOldMaskBmp);
	//		DeleteBitmap(hMaskBmp);
	//		::DeleteDC(hdcMem);
	//	}
	//	::GlobalUnlock((HGLOBAL)hDib);
	//	return TRUE;
	//}
