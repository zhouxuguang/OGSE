#include "GeoImagePreProcess.h"



//ͼ��ı�׼������
void MinMaxStreh(unsigned char* poData,int nLen,double dbMin,double dbMax)
{
	#pragma omp parallel for
	for (int i = 0;i < nLen; i ++)
	{
		poData[i] = double(poData[i]-dbMin)/(double)(dbMax-dbMin) * 255;

	}
}
