/**
* @file              SysMapManager.h
* @brief             地图管理类，主要实现加载数据，地图浏览等操作
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年7月15日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSMAPMANAGER_H_02E5DADD_44B5_4E54_9DEB_FA46C715756E__
#define __SYSMAPMANAGER_H_02E5DADD_44B5_4E54_9DEB_FA46C715756E__

#include "SysDrawCommon.h"

//全局地图管理器
	#if defined(pManager)
		#undef pManager
	#endif
#define pManager ((SysDraw::SysMapManager *)(SysDraw::SysMapManager::GetInstance()))

struct BackImagePara		//背景图像调整参数
{
	int m_nTranspancy;
	int m_nContrast;
	float m_fBrightNess;
};

namespace SysDraw
{
	class SysMap;
	class SysDrawCanvas;
	class SysRenderContext;
	class SysDisplayTransform;
	class SysRasterLayer;
	class SysFeatureLayer;
	class SysEnvelope;
	class SysMapLayer;

	//地图刷新视图的回调函数定义
	typedef void (* UPDATE_FUN)(void*);

	class SYSDRAW_API SysMapManager
	{
	public:

		/**
		* @brief 获得唯一实例
		* @return SysMapManager*
		* @author 周旭光
		* @date 2015年7月15日
		* @note 
		*/
		static SysMapManager* GetInstance();

		/**
		* @brief 初始化
		* @return void
		* @author 周旭光
		* @date 2015年7月15日
		* @note 
		*/
		void InitDraw();

		/**
		* @brief 获得当前地图
		* @return SysMap* 当前地图指针
		* @author 周旭光
		* @date 2015年7月15日
		* @note 
		*/
		SysMap* GetCurrentMap() const;

		/**
		* @brief 获得当前地图索引
		* @return int 返回当前地图索引，没有找到则返回-1
		* @author 周旭光
		* @date 2015年7月22日
		* @note 
		*/
		int GetCurrentMapIndex() const;

		/**
		* @brief 设置当前地图
		* @param[in] const SysMap* pMap 地图指针
		* @return void
		* @author 周旭光
		* @date 2015年7月15日
		* @note 
		*/
		void SetCurrentMap(const SysMap* pMap);

		/**
		* @brief GetDisplayTran
		* @return SysDisplayTransform&
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		SysDisplayTransform& GetDisplayTran() const;

		/**
		* @brief 地图视口映射
		* @param[in] int x0
		* @param[in] int y0
		* @param[in] int cx
		* @param[in] int cy
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void ViewPort(int x0,int y0,int cx,int cy) /*const*/;

		/**
		* @brief StartDraw
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void StartDraw(SysMap* pMap) /*const*/;

		/**
		* @brief StartDrawLayer
		* @param[in] SysMapLayer* pLayer
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void StartDrawLayer(SysMapLayer* pLayer) /*const*/;

		/**
		* @brief 绘制背景
		* @param[in] bool bBackGround
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void DrawViewGround(bool bBackGround) const;

		/**
		* @brief 打开栅格影像
		* @param[in] std::string strFileName
		* @param[in] DrawProgressFunc ProgressFunc = NULL
		* @param[in] void* pProgressArg = NULL
		* @return SysRasterLayer*
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		SysRasterLayer* OpenRasterFile(std::string strFileName,DrawProgressFunc ProgressFunc = NULL, void* pProgressArg = NULL);

		/**
		* @brief 打开矢量数据
		* @param[in] std::string strFileName
		* @return SysFeatureLayer*
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		SysFeatureLayer* OpenFeatureFile(std::string strFileName);

		/**
		* @brief 增加地图
		* @param[in] SysMap* pMap
		* @return bool
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		bool AddMap(SysMap* pMap);

		/**
		* @brief 移动地图
		* @param[in]  double dx
		* @param[in] double dy
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void MoveContent( double dx, double dy );

		/**
		* @brief 缩放地图
		* @param[in]  int delta
		* @param[in] double x
		* @param[in] double y
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void ZoomContent( int delta, double x, double y );

		/**
		* @brief 缩放到某个范围，如拉框放大
		* @param[in] const SysEnvelope& envNew
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void ZoomInContent(const SysEnvelope& envNew);

		/**
		* @brief 拉框缩小
		* @param[in] const SysEnvelope& envNew 当前视图的范围
		* @param[in] double x 当前拉框的中心点
		* @param[in] double y
		* @return void
		* @author 周旭光
		* @date 2015年8月5日
		* @note 
		*/
		void ZoomOutContent(const SysEnvelope& envNew,double x,double y);

		/**
		* @brief 缩放到全图
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void ZoomToWholeExtent();

		/**
		* @brief 缩放到图层
		* @param[in] const SysMapLayer* pMapLayer
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void ZoomToLayer(const SysMapLayer* pMapLayer);

		/**
		* @brief 缩放到当前栅格图层同样的分辨率，即1:1显示
		* @param[in] const SysMapLayer* pMapLayer
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void ZoomActualSize(const SysMapLayer* pMapLayer);

		/**
		* @brief 刷新地图视图
		* @param[in] bool bReDraw 是否重绘地图
		* @param[in] bool bCopyBackGround 是否将绘制完成的影像当作背景
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void UpDateView(bool bReDraw = true,bool bCopyBackGround = true, bool bRefresh = false);

		void UpDateView(void *pParamter,bool bReDraw = true,bool bCopyBackGround = true, bool bRefresh = false);

		void RefreshView(int nX, int nY, double dbScale = 1.0) const;

		/**
		* @brief 设置地图刷新的回调函数
		* @param[in] UPDATE_FUN pFun
		* @param[in] void* pPara
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void SetUpdateFunc(UPDATE_FUN pFun,void* pPara);

		void AddUpdateFunc(UPDATE_FUN pFun,void* pPara);

		void SetCopyStartPoint(int nXtartX,int nStartY);

		void DrawFrameBuf();

		void CopyFrameBuf();

		void CreateFrameBuf();

		void InitFrameBuf();

		void SetBackImagePara(bool bAdjustBack,const BackImagePara& backPara);

		static void InitMapTool();

		/**
		* @brief 设置是否重绘的标记
		* @param[in] bool bRedraw
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void SetRedrawFlag(bool bRedraw);

		/**
		* @brief 设置是否使用背景图像的标记
		* @param[in] bool bCopyBackGround
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void SetCopyBackGroundFlag(bool bCopyBackGround);

		void SetUseDrawView(bool bUseDrawView);

		void SetDarwView(const SysEnvelope& envView);

		void CopyBackToFront() const;

	private:
		SysMapManager(void);
		~SysMapManager(void);
		SysMapManager(const SysMapManager& rhs);
		SysMapManager& operator = (const SysMapManager& rhs);

		static SysMapManager* m_pInstance;

		int m_nMapCount;		//地图数量
		SysMap** m_poMaps;		//地图数组
		SysMap* m_pCurrentMap;	//当前地图

		SysDrawCanvas* m_pCanvas;			//画布
		SysRenderContext* m_pRenderContex;		//渲染上下文

		//窗口大小等参数
		mutable int m_nStartX;
		mutable int m_nStartY;
		mutable int m_nSizeX;
		mutable int m_nSizeY;

		//绘制背景左下角拷贝起点
		mutable int m_nCopyStartX;
		mutable int m_nCopyStartY;

		bool m_bReDraw;			//是否重绘地图
		bool m_bCopyBack;		//是否拷贝后台缓冲
		int m_nTransparency;	//绘制背景透明度（0-255）

		BackImagePara *m_pBackImgPara;		//背景图调整参数
		bool m_bAdjustBack;					//是否调整背景

		unsigned char* m_pDrawCache;			//绘图的缓存
		unsigned char* m_pDrawCacheTmp;

		unsigned int m_nBackTexture;		//背景图的纹理
		unsigned int m_nFrameBuffer;		//帧缓冲区
		unsigned int m_nRenderBuffer;		//渲染缓冲区
		unsigned int m_nStencilBuffer;		//离线模板缓冲区

		UPDATE_FUN m_pFun;					//刷新视图回调函数
		void* m_pPara;						//刷新视图的参数

		std::vector<UPDATE_FUN> m_vecFun;					//刷新视图回调函数数组
		std::vector<void*> m_vecPara;						//刷新视图的参数数组

		bool m_bUseDrawView;				//是否指定绘制区域
		SysEnvelope *m_pDrawView;			//指定的绘制区域

		bool m_bRefresh;	

		void Init();

		void UnInit();

		void UpdateVisualExtent(const SysEnvelope& envNew);
	};

}

#endif // end of __SYSMAPMANAGER_H_02E5DADD_44B5_4E54_9DEB_FA46C715756E__
