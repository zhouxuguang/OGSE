#include "GeoImagePreProcess.h"



//图像的标准差拉伸
void MinMaxStreh(unsigned char* poData,int nLen,double dbMin,double dbMax)
{
	#pragma omp parallel for
	for (int i = 0;i < nLen; i ++)
	{
		poData[i] = double(poData[i]-dbMin)/(double)(dbMax-dbMin) * 255;

	}
}
