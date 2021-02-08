#ifndef GEOIMAGE_PREPROCESS_INCLUDE
#define GEOIMAGE_PREPROCESS_INCLUDE

//图像的标准差拉伸
template <typename T>
void StandDevStreh(const T* poData,unsigned char* puBytes,int nLen,double dbDev,int nDevCount,double dbMean)
{
	#pragma omp parallel for
	for (int i = 0;i < nLen; i ++)
	{
		double ucMax  = dbMean + nDevCount * dbDev;
		double ucMin  = dbMean - nDevCount * dbDev;

		if (poData[i] < ucMin)
		{
			puBytes[i] = 0;
		}

		else if (poData[i] > ucMax)
		{
			puBytes[i] = 255;
		}

		else if (poData[i] >= ucMin && poData[i] <= ucMax)
		{
			puBytes[i] = double(poData[i]-ucMin)/(double)(ucMax-ucMin) * 255;
		}

	}
}

//图像的标准差拉伸
void MinMaxStreh(unsigned char* poData,int nLen,double dbMin,double dbMax);

#endif
