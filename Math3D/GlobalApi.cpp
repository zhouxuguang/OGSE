
#include <memory.h>
#include <math.h>
#include <stdlib.h>

#include "GlobalApi.h"
#include "Matrix4x4.h"
#include "Vector4.h"
#include "AlphaBlend.h"

#include "ColorRGBA.h"
#include "TriangleRender.h"

#include "AntiLine.h"

#include "ModelProject.h"

#if defined _WIN32 || defined _WIN64
#include "Win32/DIB.h"

#include <time.h>

#endif


NS_MATH3D_BEGIN

//归一化的坐标转换为屏幕坐标，起始点为左上角
static void cgNDCoord2Window(const Vector4& vec,int &x,int &y);

static void cgLineDDA(int x1,int y1,int x2,int y2,const Vector4& color);

static void cgLine3Inter(double x1,double y1,double z1,double x2,double y2,double z2);

//绘制颜色缓冲区
static void cgDrawColorBuffer(int viewPort[4],float *pfColorBuf);

//模型视图矩阵
Matrix4x4 g_matViewModel;

//投影矩阵
Matrix4x4 g_matProject;

//视口变换参数
int pfViewPort[4] = {0};

//全局绘图对象
static void* h_gDC;

//帧缓存之一之颜色缓存
float *g_pfColorBuf = NULL;

//设置模型视图矩阵
void cgSetViewModelMatrix(const Matrix4x4& matViewModel)
{
	g_matViewModel = matViewModel;
}

//设置模型视图矩阵
void cgSetProjectMatrix(const Matrix4x4& matProject)
{
	//const double* pfMatValues = &matProject[0];
	//memcpy(pfMatProject,pfMatValues,sizeof(double)*16);
	const Real* pfValues = matProject.GetValues();
	g_matProject.SetValues((Real*)pfValues);
	
}

//视口变换
void cgViewPort(int x,int y,int nWidth,int nHeight)
{
	pfViewPort[0] = x;
	pfViewPort[1] = y;
	pfViewPort[2] = nWidth;
	pfViewPort[3] = nHeight;

	if (g_pfColorBuf != NULL)
	{
		delete []g_pfColorBuf;
		g_pfColorBuf = new float[nWidth*nHeight*4];
		memset(g_pfColorBuf,0,nWidth*nHeight*4);
	}

	else
	{
		g_pfColorBuf = new float[nWidth*nHeight*4];
		memset(g_pfColorBuf,0,nWidth*nHeight*4);
	}
}

static void cgNDCoord2Window(const Vector4& vec,int &x,int &y)
{
	x = (vec[0]+1)*(pfViewPort[2]/2.0) + pfViewPort[0];
	y = pfViewPort[3] - ((vec[1]+1)*(pfViewPort[3]/2.0) + pfViewPort[1]);
    int nFar = 1;
    int nNear = 0;
    
    //屏幕的Z坐标计算
    int z = (nFar - nNear)/2*vec[2] + (nNear + nFar)*0.5;
}

void cgInit()
{
	//初始化模型视图矩阵单位矩阵
	Matrix4x4 mat;
	mat.MakeIdentity();
	cgSetViewModelMatrix(mat);

	mat = cgOrtho(-1,1,-1,1,1,100);
	cgSetProjectMatrix(mat);
}

void cgSetRenderContext(void* pHandle)
{
	h_gDC = pHandle;
}

void cgFinish()
{
#ifdef WIN32
	cgDrawColorBuffer(pfViewPort,g_pfColorBuf);
    
#elif defined __APPLE__
    cgDrawColorBuffer(pfViewPort,g_pfColorBuf);
#endif
}

void cgLine(double x1,double y1,double x2,double y2)
{
	cgLine3Inter(x1,y1,1,x2,y2,1);
}

void cgLine3(double x1,double y1,double z1,double x2,double y2,double z2)
{
	cgLine3Inter(x1,y1,z1,x2,y2,z2);
}

//#include <Windows.h>

void cgLineDDA(int x1,int y1,int x2,int y2,const ColorRGBA& color)
{
	int nLenght = 0;
	if (abs(x2-x1) >= abs(y2-y1))
	{
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

	int i = 1;
	while (i <= nLenght)
	{
		int nCol = (int)x;
		int nRow = (int)y;
		if(nCol >= 0 && nCol < pfViewPort[2] && nRow >= 0 && nRow < pfViewPort[3])
		{
			g_pfColorBuf[(pfViewPort[2]*nRow+nCol)*4] = color[0];
			g_pfColorBuf[(pfViewPort[2]*nRow+nCol)*4+1] = color[1];
			g_pfColorBuf[(pfViewPort[2]*nRow+nCol)*4+2] = color[2];
			g_pfColorBuf[(pfViewPort[2]*nRow+nCol)*4+3] = color[3];
		}
		//SetPixel((HDC)h_gDC,(int)x,(int)y,RGB(255,0,0));
		
		x += dx;
		y += dy;
		i += 1;
	}
}

static void cgLine3Inter(double x1,double y1,double z1,double x2,double y2,double z2)
{
	Vector4 vec1(x1,y1,z1,1);
	Vector4 vec2(x2,y2,z2,1);
	//Matrix4x4 matModel(pfMatViewModel);
	//Matrix4x4 matProj(pfMatProject);
	vec1 = g_matViewModel*vec1;
	vec1 = g_matProject*vec1;

	//透视除法
	//if (fabs(vec1[3]) < 0.00000001)
	{
		vec1 = vec1/(vec1[3]);
	}
	//vec1 = vec1/vec1[3];

	//计算屏幕坐标
	x1 = (vec1[0]+1)*(pfViewPort[2]/2.0) + pfViewPort[0];
	y1 = pfViewPort[3] - ((vec1[1]+1)*(pfViewPort[3]/2.0) + pfViewPort[1]);

	vec2 = g_matViewModel*vec2;
	vec2 = g_matProject*vec2;

	//vec2 = vec2/vec2[3];
	//if (fabs(vec2[3]) < 0.00000001)
	{
		vec2 = vec2/(vec2[3]+0.00000001);
	}

	//计算屏幕坐标
	x2 = (vec2[0]+1)*(pfViewPort[2]/2.0) + pfViewPort[0];
	y2 = pfViewPort[3] - ((vec2[1]+1)*(pfViewPort[3]/2.0) + pfViewPort[1]);

	//cgLineDDA(x1,y1,x2,y2,ColorRGBA(1,0,0,1));
	ColorRGBA colorBk = ColorRGBA(0,0,0);
	cgAntiLineDDA(x1,y1,x2,y2,ColorRGBA::Blue,colorBk,2);
}


#if defined _WIN32 || defined _WIN64
#include "Win32/DIB.h"

#include <time.h>

void cgDrawColorBuffer(int viewPort[4],float *pfColorBuf)
{
	int nWidth = viewPort[2];
	int nHeight = viewPort[3];
	int x = viewPort[0];
	int y = viewPort[1];

	//取出颜色缓冲区中的数据构建DIB
	unsigned char* pRed = new unsigned char[nWidth*nHeight];
	unsigned char* pGreen = new unsigned char[nWidth*nHeight];
	unsigned char* pBlue = new unsigned char[nWidth*nHeight];

	time_t t;
	srand(time(&t));
	for(int i = 0; i < nWidth*nHeight; i ++)
	{
		pRed[i] = int(pfColorBuf[i*4]*255);
		pGreen[i] = int(pfColorBuf[i*4+1]*255 + 0.5);
		pBlue[i] = int(pfColorBuf[i*4+2]*255 + 0.5);

		/*pRed[i] = sResColor[0]*255;
		pGreen[i] = sResColor[1]*255;
		pBlue[i] = sResColor[2]*255;*/
	}

	CDIB viewDib;
	viewDib.Create(nWidth,nHeight,0);
	viewDib.SetImgData(0,1,1,nWidth,nHeight,pRed);
	viewDib.SetImgData(1,1,1,nWidth,nHeight,pGreen);
	viewDib.SetImgData(2,1,1,nWidth,nHeight,pBlue);

	delete []pRed;
	delete []pGreen;
	delete []pBlue;

	//绘制像素
	RECT viewRect;
	viewRect.left = x;
	viewRect.right = x + nWidth - 1;
	viewRect.top = y;
	viewRect.bottom = y + nHeight - 1;
	viewDib.Draw((HDC)h_gDC,x,y,nWidth,nHeight,viewRect);
	
}

void cgTriangle2(double x1,double y1,double x2,double y2,double x3,double y3)
{
	Vector4 vec1(x1,y1,1,1);
	Vector4 vec2(x2,y2,1,1);
	Vector4 vec3(x3,y3,1,1);
	vec1 = g_matViewModel*vec1;
	vec1 = g_matProject*vec1;

	//透视除法
	//if (fabs(vec1[3]) < 0.00000001)
	{
		vec1 = vec1/(vec1[3]+0.00000001);
	}
	//vec1 = vec1/vec1[3];

	//计算屏幕坐标
	x1 = (vec1[0]+1)*(pfViewPort[2]/2.0) + pfViewPort[0];
	y1 = pfViewPort[3] - ((vec1[1]+1)*(pfViewPort[3]/2.0) + pfViewPort[1]);

	vec2 = g_matViewModel*vec2;
	vec2 = g_matProject*vec2;

	//vec2 = vec2/vec2[3];
	//if (fabs(vec2[3]) < 0.00000001)
	{
		vec2 = vec2/(vec2[3]+0.00000001);
	}

	//计算屏幕坐标
	x2 = (vec2[0]+1)*(pfViewPort[2]/2.0) + pfViewPort[0];
	y2 = pfViewPort[3] - ((vec2[1]+1)*(pfViewPort[3]/2.0) + pfViewPort[1]);

	vec3 = g_matViewModel*vec3;
	vec3 = g_matProject*vec3;

	//if (fabs(vec3[3]) < 0.00000001)
	{
		vec3 = vec3/(vec3[3]+0.00000001);
	}

	//计算屏幕坐标
	x3 = (vec3[0]+1)*(pfViewPort[2]/2.0) + pfViewPort[0];
	y3 = pfViewPort[3] - ((vec3[1]+1)*(pfViewPort[3]/2.0) + pfViewPort[1]);

	FillTriangle(x1,y1,x2,y2,x3,y3,ColorRGBA(0.3,0.5,0.78,1));
}

#elif defined __linux__

void cgDrawColorBuffer(int viewPort[4],float *pfColorBuf)
{}

#elif defined __APPLE__
void cgDrawColorBuffer(int viewPort[4],float *pfColorBuf)
{
    int nWidth = viewPort[2];
    int nHeight = viewPort[3];
    int x = viewPort[0];
    int y = viewPort[1];
    
    //取出颜色缓冲区中的数据构建DIB
    unsigned char* pData = new unsigned char[nWidth*nHeight*4];
    for(int i = 0; i < nWidth*nHeight*4; i ++)
    {
        pData[i] = int(pfColorBuf[i*4]*255);
    }
    
    //创建Apple上的位图
    
    delete []pData;
    
    //绘制像素
    
}

#endif


NS_MATH3D_END

