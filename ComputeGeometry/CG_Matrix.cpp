
#include "CG_Matrix.h"

/*************************************************
描述:         矩阵转置
输入:         参数m1待转置的矩阵，参数m,n分别为矩阵的行列数
输出：        参数m2转置后的矩阵
*************************************************/
void MatrixTranspose(const double *m1,double *m2,int m,int n) //矩阵转置
{ 
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			m2[j*m+i]=m1[i*n+j];
	return;
}

/*************************************************
描述:         矩阵转置(原地转置),必须是方阵
输入:         参数m1待转置的矩阵
输出：        参数m1转置后的矩阵
*************************************************/
void MatrixTransposeEx(double *m1, int n)
{
	for (int i = 0; i < n; i ++)
	{
		for (int j = i; j < n; j ++)
		{
			double temp = m1[i*n+j];
			m1[i*n+j] = m1[j*n+i];
			m1[j*n+i] = temp;
		}
	}
}

/*************************************************
描述:         正定矩阵求逆
输入:         a是输入的矩阵指针，采用一维数组描述二维数组，n为行数或列数
输出：        void
*************************************************/
void MatrixInverse(double *a,int n)
{ 
	int i,j,k;
	for(k=0;k<n;k++)
	{
		for(i=0;i<n;i++)
		{
			if(i!=k)
				*(a+i*n+k)=-*(a+i*n+k)/(*(a+k*n+k));
		}
		*(a+k*n+k)=1/(*(a+k*n+k));
		for(i=0;i<n;i++)
		{
			if(i!=k)
			{
				for(j=0;j<n;j++)
				{
					if(j!=k)
						*(a+i*n+j)+=*(a+k*n+j)* *(a+i*n+k);
				}
			}
		}
		for(j=0;j<n;j++)
		{
			if(j!=k)
				*(a+k*n+j)*=*(a+k*n+k);
		}
	}
}

/*************************************************
描述:         矩阵相乘
输入:         m1,m2为输入矩阵，result为相乘后的结果矩阵，i_1,j_12，j_2分别
              前两个组合m1的行列数，后两个组合维m2的行列数
输出：        void
*************************************************/
void MatrixMult(const double *m1,const double *m2,double *result, int i_1, int j_12, int j_2)
{ 
	int i,j,k; 
	for(i=0;i<i_1;i++)
	{
		for(j=0;j<j_2;j++)
		{
			result[i*j_2+j]=0.0;
			for(k=0;k<j_12;k++)
			{
				result[i*j_2+j]+=m1[i*j_12+k]*m2[j+k*j_2];
			}
		}
	}

	return;
}

double MatrixTrace(const double *m1,int m)
{
	double dbTrace = 0;
	for (int i = 0; i < m; i ++)
	{
		dbTrace += m1[i*m+i];
	}

	return dbTrace;
}