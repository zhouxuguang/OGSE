
#include <assert.h>

#include <GL/glew.h>


#include "SysMapManager.h"
#include "SysMap.h"
#include "SysRasterLayer.h"
#include "SysDrawCanvas.h"
#include "SysDisplayTransform.h"
#include "SysRenderContext.h"
#include "SysMapLayer.h"
#include "SysImagePreProcess.h"
#include "SysMapTool.h"
#include "SysMapToolFactory.h"
#include "SysUtilFunc.h"
#include "SysSrsMapToLayer.h"



//数据源相关指针
#include "SysUtility/SmartPtr.h"
#include "SysUtility/SysMutexLock.h"
#include "SysDataSource/SysDataSource.h"
#include "SysDataSource/CoordTransformation.h"

namespace SysDraw
{
	SysMapManager* SysMapManager::m_pInstance = NULL;

	SysMapManager::SysMapManager(void)
	{
		Init();
	}

	SysMapManager::~SysMapManager(void)
	{
		UnInit();
	}

	void SysMapManager::InitDraw()
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		glActiveTexture(GL_TEXTURE0);

		glGenTextures(1,&m_nBackTexture);
		glBindTexture(GL_TEXTURE_2D,m_nBackTexture);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D,m_nBackTexture);

		CreateFrameBuf();
	}

	void SysMapManager::Init()
	{
		m_nMapCount = 1;
		SysMap* pMap = new SysMap(_T("MainView"));
		m_poMaps = (SysMap**)malloc(sizeof(SysMap*));
		m_poMaps[0] = pMap;
		m_pCurrentMap = pMap;

		//初始化地图画布
		m_pCanvas = new SysDrawCanvas(NULL);
		m_pRenderContex = new SysRenderContext;

		m_nStartX = 0;
		m_nStartY = 0;
		m_nSizeX = 0;
		m_nSizeY = 0;

		m_nCopyStartX = 0;
		m_nCopyStartY = 0;

		m_bReDraw = true;
		m_bCopyBack = true;

		m_vecFun.clear();
		m_vecPara.clear();

		m_pFun = NULL;
		m_pPara = NULL;

		m_pDrawCache = NULL;
		m_pDrawCacheTmp = NULL;

		m_pBackImgPara = new(std::nothrow) BackImagePara;
		m_pBackImgPara->m_nTranspancy = 0;
		m_pBackImgPara->m_fBrightNess = 0;
		m_pBackImgPara->m_nContrast = 0;

		m_bAdjustBack = false;

		//

		m_bUseDrawView = false;
		m_pDrawView = new SysEnvelope;
		m_bRefresh = false;
	}

	void SysMapManager::UnInit()
	{
		if (m_poMaps != NULL)
		{
			for (int i = 0; i < m_nMapCount; i ++)
			{
				delete m_poMaps[i];
				m_poMaps[i] = NULL;
			}

			free(m_poMaps);
			m_poMaps = NULL;
		}

		m_nMapCount = 0;
		m_pCurrentMap = NULL;

		if (NULL != m_pCanvas)
		{
			delete m_pCanvas;
			m_pCanvas = NULL;
		}

		if (m_pRenderContex != NULL)
		{
			delete m_pRenderContex;
			m_pRenderContex = NULL;
		}

		if (m_pDrawCache != NULL)
		{
			delete []m_pDrawCache;
			m_pDrawCache = NULL;
		}

		if (m_pDrawCacheTmp != NULL)
		{
			delete []m_pDrawCacheTmp;
			m_pDrawCacheTmp = NULL;
		}

		if (m_pBackImgPara != NULL)
		{
			delete m_pBackImgPara;
			m_pBackImgPara = NULL;
		}

		if (m_pDrawView != NULL)
		{
			delete m_pDrawView;
			m_pDrawView = NULL;
		}

		//删除纹理
		glDeleteTextures(1,&m_nBackTexture);
	}

	SysMapManager* SysMapManager::GetInstance()
	{
		if (NULL == m_pInstance)
		{
			SysUtility::SysMutexLock mutexlock;
			mutexlock.Lock();
			
			if (NULL == m_pInstance)
			{
				m_pInstance = new SysMapManager;
			}

			mutexlock.UnLock();

		}

		return m_pInstance;
	}

	SysMap* SysMapManager::GetCurrentMap() const
	{
		return m_pCurrentMap;
	}

	int SysMapManager::GetCurrentMapIndex() const
	{
		for (int i = 0; i < m_nMapCount; i ++)
		{
			if (m_pCurrentMap == m_poMaps[i])
			{
				return i;
			}
		}

		return -1;
	}

	SysDisplayTransform& SysMapManager::GetDisplayTran() const
	{
		return m_pRenderContex->GetDisplayTran();
	}

	SysRasterLayer* SysMapManager::OpenRasterFile(std::string strFileName,DrawProgressFunc ProgressFunc, void* pProgressArg)
	{
		SysRasterLayer* pRasLayer = OpenRaster(strFileName,ProgressFunc,pProgressArg);
		m_pCurrentMap->AddMapLayer(pRasLayer);

		//当前地图没有图层时，需要设置视口
		if (1 == m_pCurrentMap->GetLayerCount())
		{
			//SysEnvelope& env = pRasLayer->GetLayerExtent();
			//m_pRenderContex->SetDisplayTran(tran);
			//m_pRenderContex->SetExtent(env);
			ViewPort(m_nStartX,m_nStartY,m_nSizeX,m_nSizeY);
		}

		return pRasLayer;
	}

	SysFeatureLayer* SysMapManager::OpenFeatureFile(std::string strFileName)
	{
		SysFeatureLayer* pFeaLayer = OpenFeature(strFileName);
		m_pCurrentMap->AddMapLayer((SysMapLayer*)pFeaLayer);

		//当前地图没有图层时，需要设置视口
		if (1 == m_pCurrentMap->GetLayerCount())
		{
			//SysEnvelope& env = pRasLayer->GetLayerExtent();
			//m_pRenderContex->SetDisplayTran(tran);
			//m_pRenderContex->SetExtent(env);
			ViewPort(m_nStartX,m_nStartY,m_nSizeX,m_nSizeY);
		}

		return pFeaLayer;
	}

	void SysMapManager::ViewPort(int x0,int y0,int cx,int cy) /*const*/
	{
		SysEnvelope env = m_pCurrentMap->GetVisualExtent();

		//假设范围
		if (env.IsNull())
		{
			env.Init(-1,1,-1,1);
		}

		m_nStartX = x0;
		m_nStartY = y0;
		m_nSizeX = cx;
		m_nSizeY = cy;

		//构造rect
		RECT mRect;
		mRect.left = x0;
		mRect.right = x0 + cx;
		mRect.top = y0;
		mRect.bottom = y0 + cy;


		double dbScaleX = env.GetWidth()/(mRect.right - mRect.left);
		double dbScaleY = env.GetHeight()/(mRect.bottom - mRect.top);
		SysDraw::SysDisplayTransform tran(env,max(dbScaleX,dbScaleY));
		tran.SetVisibleBoundsExt(env);
		//tran.SetViewRect(mRect);
		tran.SetExportFrame(mRect);

		//设置渲染环境
		m_pRenderContex->SetDisplayTran(tran);
		m_pRenderContex->SetExtent(env);

		m_pCanvas->OnSize(0,cx,cy);
		m_pCanvas->SetRenderContext(m_pRenderContex);

		if (m_pDrawCache != NULL)
		{
			delete []m_pDrawCache;
			m_pDrawCache = NULL;
		}

		if (m_pDrawCacheTmp != NULL)
		{
			delete []m_pDrawCacheTmp;
			m_pDrawCacheTmp = NULL;
		}

		m_pDrawCache = new(std::nothrow) unsigned char[cx*cy*4];
		m_pDrawCacheTmp = new(std::nothrow) unsigned char[cx*cy*4];

		glBindTexture(GL_TEXTURE_2D,m_nBackTexture);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,cx,cy,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);

		InitFrameBuf();
	}

#define BUFFER_OFFSET(bytes) ((GLubyte*)NULL + (bytes))

	void SysMapManager::StartDraw(SysMap* pMap) /*const*/
	{
		smartlog<<"开始绘制";
		if (m_bRefresh)
		{
			RefreshView(m_nSizeX/2,m_nSizeY/2,1.4);
			return;
		}

		if (m_bReDraw)
		{
			DrawFrameBuf();

			if (m_bUseDrawView)
			{
				m_pCanvas->DrawMap(pMap,*m_pDrawView);
			}

			else
			{
				m_pCanvas->DrawMap(pMap);//
			}
			

			glPixelStorei(GL_PACK_ALIGNMENT,1);

			glDrawBuffer(GL_COLOR_ATTACHMENT0);

			GLuint nPixelBuff;
			glGenBuffers(1,&nPixelBuff);
			glBindBuffer(GL_PIXEL_PACK_BUFFER,nPixelBuff);
			glBufferData(GL_PIXEL_PACK_BUFFER,m_nSizeX*m_nSizeY*4,NULL,GL_STREAM_READ);

			
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			glBindBuffer(GL_PIXEL_PACK_BUFFER,nPixelBuff);
			glReadPixels(0,0,m_nSizeX,m_nSizeY,GL_RGBA,GL_UNSIGNED_BYTE,BUFFER_OFFSET(0));

			glBindBuffer(GL_PIXEL_PACK_BUFFER,nPixelBuff);
			GLubyte* pBytes = (GLubyte *)glMapBuffer(GL_PIXEL_PACK_BUFFER,GL_READ_ONLY);

			int nPixelSpace = 4;
			int nLineSpace = nPixelSpace*m_nSizeX;

			int nOffset = nLineSpace * (m_nSizeY -1 );
			int nOffset1 = 0;

			for (int i = 0; i < m_nSizeY; i ++)
			{
				memcpy(m_pDrawCacheTmp + nOffset1, pBytes + nOffset, nLineSpace);
				nOffset -= nLineSpace;
				nOffset1 += nLineSpace;
			}

			glUnmapBuffer(GL_PIXEL_PACK_BUFFER);

			CopyFrameBuf();

			glReadBuffer(GL_FRONT);
			glReadPixels(0,0,m_nSizeX,m_nSizeY,GL_RGBA,GL_UNSIGNED_BYTE,BUFFER_OFFSET(0));

			pBytes = (GLubyte *)glMapBuffer(GL_PIXEL_PACK_BUFFER,GL_READ_ONLY);

			//重置索引
			nOffset = nLineSpace * (m_nSizeY -1 );
			nOffset1 = 0;
			for (int i = 0; i < m_nSizeY; i ++)
			{
				memcpy(m_pDrawCache + nOffset1, pBytes + nOffset, nLineSpace);
				nOffset -= nLineSpace;
				nOffset1 += nLineSpace;
			}

			glUnmapBuffer(GL_PIXEL_PACK_BUFFER);

			glBindBuffer(GL_PIXEL_PACK_BUFFER,0);
			glDeleteBuffers(1,&nPixelBuff);

		}

		else
		{
			DrawViewGround(m_bCopyBack);

			m_pCanvas->DrawBrowseLayer(pMap);

			if (!m_bCopyBack)
			{
				m_pCanvas->DrawMarkLayer(pMap);
				m_pCanvas->DrawGraphicLayers(pMap);
			}

			m_nCopyStartX = 0;
			m_nCopyStartY = 0;
		}
	}

	void SysMapManager::StartDrawLayer(SysMapLayer* pLayer) /*const*/
	{
		DrawFrameBuf();
		m_pCanvas->DrawLayer(m_pCurrentMap,pLayer);

		glPixelStorei(GL_PACK_ALIGNMENT,1);

		GLuint nPixelBuff;
		glGenBuffers(1,&nPixelBuff);
		glBindBuffer(GL_PIXEL_PACK_BUFFER,nPixelBuff);
		glBufferData(GL_PIXEL_PACK_BUFFER,m_nSizeX*m_nSizeY*3,NULL,GL_STATIC_READ);

		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glReadPixels(0,0,m_nSizeX,m_nSizeY,GL_RGB,GL_UNSIGNED_BYTE,NULL);

		GLubyte* pBytes = (GLubyte *)glMapBuffer(GL_PIXEL_PACK_BUFFER,GL_READ_ONLY);
		memcpy(m_pDrawCache,pBytes,m_nSizeX*m_nSizeY*3);

		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);

		glDeleteBuffers(1,&nPixelBuff);

		CopyFrameBuf();

		int nPixelSpace = 3;
		int nLineSpace = nPixelSpace*m_nSizeX;

		int nOffset = nLineSpace * (m_nSizeY -1 );
		int nOffset1 = 0;

		for (int i = 0; i < m_nSizeY; i ++)
		{
			memcpy(m_pDrawCacheTmp + nOffset1, m_pDrawCache + nOffset, nLineSpace);
			nOffset -= nLineSpace;
			nOffset1 += nLineSpace;
		}

		
	}

	void SysMapManager::DrawViewGround(bool bBackGround) const
	{
		int nWidth = m_nSizeX;
		int nHeight = m_nSizeY;

		/*glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBindTexture(GL_TEXTURE_2D,m_nBackTexture);

		if (bBackGround)
		{
			glTexSubImage2D(GL_TEXTURE_2D,0,0,0,nWidth,nHeight,GL_RGBA,GL_UNSIGNED_BYTE,m_pDrawCacheTmp);
		}

		else
		{
			glTexSubImage2D(GL_TEXTURE_2D,0,0,0,nWidth,nHeight,GL_RGBA,GL_UNSIGNED_BYTE,m_pDrawCacheTmp);
		}*/

		/*glReadBuffer(GL_COLOR_ATTACHMENT0);
		glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,m_nSizeX,m_nSizeY);*/

		double winMinx,winMiny,winMaxx,winMaxy,dbZ;
		/*ScreenToWorld(m_nCopyStartX,nHeight - m_nCopyStartY,&winMinx,&winMiny,&dbZ);
		ScreenToWorld(m_nCopyStartX + nWidth,nHeight - (m_nCopyStartY + nHeight),&winMaxx,&winMaxy,&dbZ);*/

		/*SysDisplayTransform dispTran = m_pRenderContex->GetDisplayTran();
		dispTran.ToMapPoint(m_nCopyStartX,nHeight - m_nCopyStartY,winMinx,winMiny);
		dispTran.ToMapPoint(m_nCopyStartX + nWidth,nHeight - (m_nCopyStartY + nHeight),winMaxx,winMaxy);

		glBegin(GL_QUADS);
			glVertex3f(winMinx,winMiny,0);glTexCoord2f(0,0);
			glVertex3f(winMinx,winMaxy,0);glTexCoord2f(1,0);
			glVertex3f(winMaxx,winMaxy,0);glTexCoord2f(1,1);
			glVertex3f(winMaxx,winMiny,0);glTexCoord2f(0,1);
		glEnd();*/

		glBindFramebuffer(GL_READ_FRAMEBUFFER,m_nFrameBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);

		glViewport(0,0,nWidth,nHeight);
		glClearColor(1,1,1,1);

		glBlitFramebuffer(0,0,nWidth,nHeight,m_nCopyStartX,m_nCopyStartY,
			m_nCopyStartX + nWidth,(m_nCopyStartY + nHeight),GL_COLOR_BUFFER_BIT ,GL_NEAREST);

	
		m_nCopyStartX = 0;
		m_nCopyStartY = 0;

	}

	bool SysMapManager::AddMap(SysMap* pMap)
	{
		if (m_nMapCount == 0 && m_poMaps == NULL)
		{
			m_poMaps = (SysMap**)malloc(sizeof(SysMap*));
			m_poMaps[m_nMapCount] = pMap;
			m_nMapCount += 1;
		}
		else if (m_nMapCount > 0 && m_poMaps != NULL)
		{
			m_poMaps = (SysMap **)realloc(m_poMaps,sizeof(SysMap*)*(m_nMapCount+1));
			m_poMaps[m_nMapCount] = pMap;
			m_nMapCount += 1;
		}

		return 1;
	}

	void SysMapManager::MoveContent( double dx, double dy )
	{
		SysEnvelope& env = m_pCurrentMap->GetVisualExtent();
		env.Translate(dx,dy);

		UpdateVisualExtent(env);
		
	}

	void SysMapManager::ZoomContent( int delta, double x, double y )
	{
		double fZoomFactor = 1.4;

		SysEnvelope& extent = m_pCurrentMap->GetVisualExtent();

		//地图中心点
		double centerX = 0;
		double centerY = 0;
		extent.Center(centerX,centerY);

		int nZoomMode = 2;

		if ( nZoomMode != 0 )
		{
			if ( nZoomMode == 1 ) //中心点缩放
			{
			}
			else if ( nZoomMode == 2 ) //鼠标点缩放
			{
				//centerX = x + ( centerX - x ) * ( 1.0 / fZoomFactor );
				//centerY = y + ( centerY - y ) * ( 1.0 / fZoomFactor );
			}
		}

		double dbNewIntervalX = 0, dbNewIntervalY = 0;

		if ( delta > 0 )
		{
			dbNewIntervalX = extent.GetWidth() * fZoomFactor;
			dbNewIntervalY = extent.GetHeight() * fZoomFactor;
		}
		else if ( delta < 0 )
		{
			dbNewIntervalX = extent.GetWidth() / fZoomFactor;
			dbNewIntervalY = extent.GetHeight() / fZoomFactor;
		}
		else
		{
			return;
		}

		double dx = centerX - x;
		double dy = centerY - y;

		extent.maxX = x + dbNewIntervalX / 2 ;
		extent.minX = x - dbNewIntervalX / 2 ;
		extent.maxY = y + dbNewIntervalY / 2 ;
		extent.minY = y - dbNewIntervalY / 2 ;
		extent.Translate(dx,dy);

		UpdateVisualExtent(extent);

	}

	void SysMapManager::ZoomInContent(const SysEnvelope& envNew)
	{
		UpdateVisualExtent(envNew);
	}

	void SysMapManager::ZoomOutContent(const SysEnvelope& envNew,double x,double y)
	{
		SysEnvelope envTmp = envNew;
		//计算新的范围，按照2倍缩小
		double dbCenterX = x;
		double dbCenterY = y;
		//envTmp.Center(dbCenterX,dbCenterY);
		double dbWidth = envTmp.GetWidth();
		double dbHeight = envTmp.GetHeight();
		dbWidth *= 2;
		dbHeight *= 2;
		double dbMinx = dbCenterX - dbWidth * 0.5;
		double dbMaxx = dbCenterX + dbWidth * 0.5;
		double dbMiny = dbCenterY - dbHeight * 0.5;
		double dbMaxy = dbCenterY + dbHeight * 0.5;
		envTmp.Init(dbMinx,dbMaxx,dbMiny,dbMaxy);

		UpdateVisualExtent(envTmp);
	}

	void SysMapManager::ZoomToWholeExtent()
	{
		SysEnvelope envNew = GetCurrentMap()->GetFullExtent();
		UpdateVisualExtent(envNew);
	}

	void SysMapManager::ZoomToLayer(const SysMapLayer* pMapLayer)
	{
		//这里还需要动态投影等计算
		SysEnvelope envView;
		LayerExtentToMapExtent(m_pCurrentMap,(SysMapLayer *)pMapLayer,envView);

		SysEnvelope envNew = pMapLayer->GetLayerExtent();
		UpdateVisualExtent(envView);
	}

	void SysMapManager::ZoomActualSize(const SysMapLayer* pMapLayer)
	{
		if ( NULL == pMapLayer)
		{
			return;
		}

		if (pMapLayer->GetLayerType() == SysDraw::SysMapLayer::VectorLayer)
		{
			return;
		}

		SysRasterLayer* pRasLayer = (SysRasterLayer*)pMapLayer;
		if (NULL == pRasLayer)
		{
			return;
		}

		//这里还需要动态投影等计算

		SysDataSource::IRasterDataLayer* pDataLayer = (SysDataSource::IRasterDataLayer *)pRasLayer->GetDataSource();

		//以当前视图中心点将当前图层等比例缩放
		SysEnvelope envCur = pRasLayer->GetLayerExtent();

		//SysEnvelope envCur = m_pCurrentMap->GetVisualExtent();
		double dbCenterX = 0;
		double dbCenterY = 0;
		envCur.Center(dbCenterX,dbCenterY);

		long nCenterX = 0;
		long nCenterY = 0;
		pDataLayer->WorldToPixel(dbCenterX,dbCenterY,nCenterY,nCenterX);

		SysDisplayTransform& tran = m_pRenderContex->GetDisplayTran();
		RECT rtRect;
		tran.GetExportFrame(rtRect);
		int nWidth = rtRect.right - rtRect.left;
		int nHeight = rtRect.bottom - rtRect.top;

		//确定当前图层影像上的行列号(这里可能超过范围，只是逻辑上的坐标)
		int nStartX = nCenterX - nWidth / 2;
		int nEndX = nCenterX + nWidth / 2;
		int nStartY = nCenterY - nHeight / 2;
		int nEndY = nCenterY + nHeight / 2;

		//然后将行列号转换到地理范围
		double dbX1,dbY1;
		pDataLayer->PixelToWorld_Ex(nStartX,nStartY,dbX1,dbY1);
		double dbX2,dbY2;
		pDataLayer->PixelToWorld_Ex(nEndX,nEndY,dbX2,dbY2);

		SysEnvelope envNew;
		envNew.Init(dbX1,dbX2,dbY1,dbY2);

		SysSRS::CCoordTransformation* pForwardTran = pMapLayer->GetForwardCoordTran();
		if (pForwardTran != NULL)
		{
			SysSrsMapToLayer::TransExtent(pForwardTran,envNew);
		}

		UpdateVisualExtent(envNew);
	}

	void SysMapManager::UpDateView(bool bReDraw ,bool bCopyBackGround , bool bRefresh )
	{
		//m_bCopyBack = bCopyBack;
		m_bReDraw = bReDraw;
		m_bCopyBack = bCopyBackGround;
		m_bRefresh = bRefresh;

		if (m_pFun != NULL)
		{
			m_pFun(m_pPara);
		}

		/*for (int i = 0; i < m_vecPara.size(); i ++)
		{
			if (m_vecFun[i] != NULL)
			{
				m_vecFun[i](m_vecPara[i]);
			}
		}*/
	}

	void SysMapManager::UpDateView(void *pParamter,bool bReDraw ,bool bCopyBackGround , bool bRefresh )
	{
		m_bReDraw = bReDraw;
		m_bCopyBack = bCopyBackGround;
		m_bRefresh = bRefresh;

		for (int i = 0; i < m_vecPara.size(); i ++)
		{
			if (pParamter == m_vecPara[i])
			{
				if (m_vecFun[i] != NULL)
				{
					m_vecFun[i](m_vecPara[i]);
				}
			}
		}
	}

	void SysMapManager::SetUpdateFunc(UPDATE_FUN pFun,void* pPara)
	{
		m_pFun = pFun;
		m_pPara = pPara;
	}

	void SysMapManager::SetCopyStartPoint(int nXtartX,int nStartY)
	{
		m_nCopyStartX = nXtartX;
		m_nCopyStartY = nStartY;
	}

	void SysMapManager::UpdateVisualExtent(const SysEnvelope& envNewTmp)
	{
		SysEnvelope envNew = envNewTmp;
		SysDisplayTransform& tran = m_pRenderContex->GetDisplayTran();
		tran.SetVisibleBounds(envNew);

		RECT rtRect;
		tran.GetExportFrame(rtRect);

		//计算比例
		double dbScaleX = envNew.GetWidth()/(rtRect.right-rtRect.left);
		double dbScaleY = envNew.GetHeight()/(rtRect.bottom-rtRect.top);
		tran.SetMapUnitsPerPixel(max(dbScaleX,dbScaleY));

		m_pCurrentMap->SetVisualExtent(envNew);

		m_pRenderContex->SetDisplayTran(tran);
		m_pRenderContex->SetExtent(envNew);
		m_pCanvas->SetRenderContext(m_pRenderContex);
	}

	void SysMapManager::CreateFrameBuf()
	{
		glGenFramebuffers(1,&m_nFrameBuffer);
		const GLubyte* errorStr;
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
			printf("%s\n",errorStr);
		}
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,m_nFrameBuffer);
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
			printf("%s\n",errorStr);
		}

		glGenRenderbuffers(1,&m_nRenderBuffer);
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
			printf("%s\n",errorStr);
		}
		glBindRenderbuffer(GL_RENDERBUFFER,m_nRenderBuffer);
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
			printf("%s\n",errorStr);
		}

		glGenRenderbuffers(1,&m_nStencilBuffer);
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
			printf("%s\n",errorStr);
		}
		glBindRenderbuffer(GL_RENDERBUFFER,m_nStencilBuffer);
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
			printf("%s\n",errorStr);
		}

	}

	void SysMapManager::DrawFrameBuf()
	{
		
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,m_nFrameBuffer);
		const GLubyte* errorStr;
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
			printf("%s\n",errorStr);
		}

		int nWidth = m_nSizeX;
		int nHeight = m_nSizeY;

		glViewport(0,0,nWidth,nHeight);
		glClearColor(1,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
			printf("%s\n",errorStr);
		}
	}

	void SysMapManager::CopyFrameBuf()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER,m_nFrameBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);

		int nWidth = m_nSizeX;
		int nHeight = m_nSizeY;
		glViewport(0,0,nWidth,nHeight);
		glClearColor(1,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glBlitFramebuffer(0,0,nWidth,nHeight,0,0,nWidth,nHeight,GL_COLOR_BUFFER_BIT ,GL_NEAREST);
		//glBlitFramebuffer(0,0,nWidth,nHeight,0,0,nWidth,nHeight, GL_STENCIL_BUFFER_BIT,GL_NEAREST);

		//glDeleteRenderbuffers(1,&m_nRenderBuffer);
	}

	void SysMapManager::InitFrameBuf()
	{
		//使用离屏渲染
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,m_nFrameBuffer);

		const GLubyte* errorStr;
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
			printf("%s804\n",errorStr);
		}

		//渲染缓冲区
		glBindRenderbuffer(GL_RENDERBUFFER,m_nRenderBuffer);
		//glBindRenderbuffer(GL_RENDERBUFFER,m_nStencilBuffer);

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
			printf("%s815\n",errorStr);
		}

		int nWidth = m_nSizeX;
		int nHeight = m_nSizeY;

		//为缓冲区创建存储空间
		//glRenderbufferStorageMultisample(GL_RENDERBUFFER,4,GL_RGBA,nWidth,nHeight);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_RGBA,nWidth,nHeight);
		//将渲染缓冲区附加到帧缓冲区对象上
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,m_nRenderBuffer);
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
			printf("%s829\n",errorStr);
		}

		glBindTexture(GL_TEXTURE_2D,m_nBackTexture);
		//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_nBackTexture,0);

		glBindRenderbuffer(GL_RENDERBUFFER,m_nStencilBuffer);
		//glRenderbufferStorageMultisample(GL_RENDERBUFFER,4,GL_DEPTH24_STENCIL8,nWidth,nHeight);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,nWidth,nHeight);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,m_nStencilBuffer);//绑定深度缓冲
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_STENCIL_ATTACHMENT,GL_RENDERBUFFER,m_nStencilBuffer);

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			errorStr = gluErrorString(error);
			printf("%s844\n",errorStr);
		}
		//glEnable(GL_STENCIL_TEST);

		GLenum eFlag = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
	}

	void SysMapManager::InitMapTool()
	{
		SysMapToolFactory::RegisterMapTool(_T("ZoomInTool"),SysMapTool::CreateMapTool,SysMapTool::DestroyTool);
		SysMapToolFactory::RegisterMapTool(_T("ZoomOutTool"),SysMapTool::CreateMapTool,SysMapTool::DestroyTool);
		SysMapToolFactory::RegisterMapTool(_T("PanTool"),SysMapTool::CreateMapTool,SysMapTool::DestroyTool);
		SysMapToolFactory::RegisterMapTool(_T("WheelTool"),SysMapTool::CreateMapTool,SysMapTool::DestroyTool);
		//SysMapToolFactory::RegisterMapTool(_T("PanTool"),SysMapTool::CreateMapTool,SysMapTool::DestroyTool);
	}

	void SysMapManager::SetBackImagePara( bool bAdjustBack,const BackImagePara& backPara )
	{
		m_bAdjustBack = bAdjustBack;
		m_pBackImgPara->m_fBrightNess = backPara.m_fBrightNess;
		m_pBackImgPara->m_nContrast = backPara.m_nContrast;
		m_pBackImgPara->m_nTranspancy = backPara.m_nTranspancy;
	}

	void SysMapManager::SetRedrawFlag( bool bRedraw )
	{
		m_bReDraw = bRedraw;
	}

	void SysMapManager::SetCopyBackGroundFlag( bool bCopyBackGround )
	{
		m_bCopyBack = bCopyBackGround;
	}

	void SysMapManager::SetUseDrawView( bool bUseDrawView )
	{
		m_bUseDrawView = bUseDrawView;
	}

	void SysMapManager::SetDarwView( const SysEnvelope& envView )
	{
		*m_pDrawView = envView;
	}

	void SysMapManager::CopyBackToFront() const
	{
		//然后将前台缓冲区拷贝到后台帧缓冲区
		/*glBindFramebuffer(GL_READ_FRAMEBUFFER,0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,m_nFrameBuffer);

		int nWidth = m_nSizeX;
		int nHeight = m_nSizeY;

		glViewport(0,0,nWidth,nHeight);
		glClearColor(1,1,1,1);

		glBlitFramebuffer(0,0,nWidth,nHeight,0,0,
			0 + nWidth,(0 + nHeight),GL_COLOR_BUFFER_BIT ,GL_NEAREST);*/

		int nWidth = m_nSizeX;
		int nHeight = m_nSizeY;

		glReadBuffer(GL_FRONT);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glCopyPixels(-m_nCopyStartX,-m_nCopyStartY,nWidth,nHeight,GL_COLOR);
	}

	void SysMapManager::RefreshView( int nX, int nY, double dbScale /*= 1.0*/ ) const
	{
		int nRealY = m_nSizeY - nY;
		glRasterPos2i(nX,nRealY);
		glPixelZoom(dbScale,dbScale);
		glRasterPos2i(0,0);
		glCopyPixels(0,0,m_nSizeX,m_nSizeY,GL_COLOR);
		glPixelZoom(1.0f,1.0f);
		glFlush();
	}

	void SysMapManager::AddUpdateFunc(UPDATE_FUN pFun,void* pPara)
	{
		m_vecFun.push_back(pFun);
		m_vecPara.push_back(pPara);
	}

	void SysMapManager::SetCurrentMap(const SysMap* pMap)
	{
		m_pCurrentMap = (SysMap *)pMap;
	}

}
