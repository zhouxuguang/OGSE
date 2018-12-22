/**
* @file              SysImagePreProcess.h
* @brief             图像拉伸等一些基本的处理，主要用于显示渲染
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年7月9日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSIMAGEPREPROCESS_H_E61EE64A_5317_453B_BF59_82A884B03FFE__
#define __SYSIMAGEPREPROCESS_H_E61EE64A_5317_453B_BF59_82A884B03FFE__

#include "SysDrawCommon.h"


/**
* @brief 图像的标准差拉伸
* @param[in] onst T* poData
* @param[out] unsigned char* puBytes
* @param[in] int nLen
* @param[in] double dbDev
* @param[in] int nDevCount
* @param[in] 
* @return void
* @author 周旭光
* @date 2015年7月9日
* @note 
*/
template <typename T>
SYSDRAW_API void StandDevStretch(
				   const T* poData,
				   unsigned char* puBytes,
				   int nLen,
				   double dbDev,
				   float fDevCount,
				   double dbMean,
				   double dbEps = FLOAT_ZERO
				   );

/**
* @brief 图像的最大最小拉伸
* @param[in] const T* poData
* @param[out] unsigned char* puBytes
* @param[in] int nLen
* @param[in] double dbMin
* @param[in] double dbMax
* @return void
* @author 周旭光
* @date 2015年7月9日
* @note 
*/
template <typename T>
SYSDRAW_API void MinMaxStretch(const T* poData,unsigned char* puBytes,int nLen,double dbMin,double dbMax,double dbEps = FLOAT_ZERO);

/**
* @brief 创建最优化线性拉伸的参数
* @param[in] float* pHist
* @param[out] float &dbMin
* @param[out] float &dbMax
* @param[in] int nPixelCount
* @return void
* @author 周旭光
* @date 2015年11月5日
* @note 
*/
SYSDRAW_API void CreateOptimizedStretch(float* pHist,float &dbMin,float &dbMax,int nPixelCount);

SYSDRAW_API void CreateLinearStretch(float* pHist,float &dbMin,float &dbMax,int nPixelCount,int nPercent);

/**
* @brief 灰度翻转
* @param[in] unsigned char* poData
* @param[in] size_t nLen
* @param[in] size_t nExcludeIndex
* @return void
* @author 周旭光
* @date 2015年11月6日
* @note 
*/
SYSDRAW_API void BrightnessContrast(unsigned char* poData,size_t nLen, size_t nExcludeIndex);

/**
* @brief 亮度控制
* @param[in] unsigned char uDst
* @param[in] float fBrightNess
* @return unsignedchar 
* @author 周旭光
* @date 2015年11月6日
* @note 
*/
SYSDRAW_API unsigned char BrightnessStretch(unsigned char uDst,float fBrightNess);

/**
* @brief 对比度调整(-128<nPara<128)
* @param[in] unsigned char uSrc
* @param[in] int nPara
* @return unsigned char 
* @author 周旭光
* @date 2015年11月6日
* @note 
*/
SYSDRAW_API unsigned char ContrastStretch(unsigned char uSrc,int nPara);

SYSDRAW_API void ScreenToWorld(double winx, double winy, double *objx, double *objy, double *objz);


/**
* @brief 亮度调整（数组）
* @param[in] nsigned char* poData
* @param[in] int nBitPlanes
* @param[in] int nImgWidth
* @param[in] int nImgHeight
* @param[in] int nStartX
* @param[in] int nStartY
* @param[in] int nWidth
* @param[in] int nHeight
* @param[in] float fBrightNess
* @return void
* @author 周旭光
* @date 2015年11月6日
* @note 
*/
SYSDRAW_API void BrightnessAdjust(
								  unsigned char* poData,
								  int nBitPlanes,
								  int nImgWidth,
								  int nImgHeight,
								  int nStartX,
								  int nStartY,
								  int nWidth,
								  int nHeight,
								  float fBrightNess);

/**
* @brief RGB颜色空间到HSI空间转换
* @param[in] unsigned char R
* @param[in] unsigned char G
* @param[in] unsigned char B
* @param[in] float &i
* @param[in] float &h
* @param[in] float &s
* @return bool
* @author 周旭光
* @date 2015年11月6日
* @note 
*/
SYSDRAW_API bool Rgb2Ihs(unsigned char R,unsigned char G,unsigned char B,float &i,float &h,float &s);

/**
* @brief HSI颜色空间到RGB空间转换
* @param[in] float fh
* @param[in] float fi
* @param[in] float fs
* @param[in] unsigned char& r
* @param[in] unsigned char& g
* @param[in] unsigned char& b
* @return void
* @author 周旭光
* @date 2015年11月6日
* @note 
*/
SYSDRAW_API void HIS2RGB(float fh, float fi, float fs, unsigned char& r, unsigned char& g, unsigned char& b);

#include "SysImagePrePriv.h"

#endif
