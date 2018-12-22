/**
* @file              SysRasterTile.h
* @brief             栅格文件tile，用于显示缓存256*256
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年7月31日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSRASTERTILE_H_01C56D2A_F45A_493D_B3B0_E96522650C49__
#define __SYSRASTERTILE_H_01C56D2A_F45A_493D_B3B0_E96522650C49__

#include <GL/glew.h>
#include "SysDisplayTransform.h"

namespace SysDraw
{

	class SysRasterTile
	{
	public:
		SysRasterTile(const SysDisplayTransform &dispTran,const SysEnvelope& env);
		~SysRasterTile(void);

		//绘制
		void Draw() const;

		//更新一部分数据
		void UpdateSubDate(int nXoffset,int nYoffset,int nWidth,int nHeight,const void* pPixels);

		void SetDispTran(const SysDisplayTransform &dispTran);

		void SetEnvlope(const SysEnvelope& env);

	private:

		GLuint m_nTexId;			//纹理ID
		SysEnvelope m_TileEnv;		//当前TILE的地理范围
		SysDisplayTransform m_DispTran;
	};

}

#endif // end of __SYSRASTERTILE_H_01C56D2A_F45A_493D_B3B0_E96522650C49__
