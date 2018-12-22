/**
* @file              SysImagePrePriv.h
* @brief             
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年7月9日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSIMAGEPREPRIV_H_41230C17_0582_45DE_96E8_229D57C8E0BD__
#define __SYSIMAGEPREPRIV_H_41230C17_0582_45DE_96E8_229D57C8E0BD__

//图像的标准差拉伸
template <typename T>
void StandDevStretch(const T* poData,unsigned char* puBytes,int nLen,double dbDev,float fDevCount,double dbMean,double dbEps )
{
	#pragma omp parallel for
	for (int i = 0;i < nLen; i ++)
	{
		double ucMax  = dbMean + fDevCount * dbDev;
		double ucMin  = dbMean - fDevCount * dbDev;

		if (poData[i] <= ucMin)
		{
			if (fabs((double)poData[i] - dbEps) > FLOAT_ZERO)
			{
				puBytes[i] = 1;
			}
			else /*if(fabs((double)poData[i] - dbEps) <= FLOAT_ZERO)*/
			{
				puBytes[i] = 0;
			}
		}

		else if (poData[i] > ucMax)
		{
			puBytes[i] = 255;
		}

		else if (poData[i] >= ucMin && poData[i] <= ucMax)
		{
			puBytes[i] = double(poData[i]-ucMin)/(double)(ucMax-ucMin) * 254 +1;
		}

	}
}

//图像的最大最小拉伸
template <typename T>
void MinMaxStretch(const T* poData,unsigned char* puBytes,int nLen,double dbMin,double dbMax,double dbEps)
{
	#pragma omp parallel for
	for (int i = 0;i < nLen; i ++)
	{
		if (poData[i] >= dbMin && poData[i] <= dbMax)
		{
			puBytes[i] = double(poData[i]-dbMin)/(double)(dbMax-dbMin) * 254 + 1;
		}
		else if (poData[i] < dbMin)
		{
			if ((double)poData[i] - dbEps > FLOAT_ZERO)
			{
				puBytes[i] = 1;
			}
			else /*(fabs((double)poData[i] - dbEps) <= FLOAT_ZERO)*/
			{
				puBytes[i] = 0;
			}
		}

		else if (poData[i] > dbMax)
		{
			puBytes[i] = 255;
		}

	}
}


#endif // end of __SYSIMAGEPREPRIV_H_41230C17_0582_45DE_96E8_229D57C8E0BD__
