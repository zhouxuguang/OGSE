#include "CG_ConvexHull.h"


/********************************************** 
寻找凸包的graham扫描法 
**********************************************/ 
void Graham_scan(SPOINT *PointSet,SPOINT *ch,int n,int &len) 
{ 
	int i = 0;
	int j = 0;
	int k = 0;
	int top = 2; 
	SPOINT tmp; 

	// 选取PointSet中y坐标最小的点PointSet[k]，如果这样的点有多个，则取最左边的一个 
	for(i=1; i<n; i++) 
	{
		if ( PointSet[i].y < PointSet[k].y 
			|| (PointSet[i].y == PointSet[k].y) 
			&& (PointSet[i].x < PointSet[k].x) ) 
		{
			k = i;
		}
	}

	tmp = PointSet[0]; 
	PointSet[0] = PointSet[k]; 
	PointSet[k] = tmp; // 现在PointSet中y坐标最小的点在PointSet[0] 

	for (i=1;i<n-1;i++) /* 对顶点按照相对PointSet[0]的极角从小到大进行排序，极角相同的按照距离PointSet[0]从近到远进行排序 */ 
	{ 
		k = i; 
		for (j=i+1;j<n;j++) 
		{
			if ( Multiply(PointSet[j],PointSet[k],PointSet[0])>0 ||  // 极角更小    
				(Multiply(PointSet[j],PointSet[k],PointSet[0])==0) && /* 极角相等，距离更短 */        
				 Distance(PointSet[0],PointSet[j]) < Distance(PointSet[0],PointSet[k])
				) 
			{
				k = j; 
			}
		}
		tmp = PointSet[i]; 
		PointSet[i] = PointSet[k]; 
		PointSet[k] = tmp; 
	} 

	ch[0] = PointSet[0]; 
	ch[1] = PointSet[1]; 
	ch[2] = PointSet[2]; 
	for (i=3; i<n; i++) 
	{ 
		while (Multiply(PointSet[i],ch[top],ch[top-1])>=0) 
		{
			top--; 
		}

		ch[++top] = PointSet[i]; 
	} 
	len = top + 1; 
} 