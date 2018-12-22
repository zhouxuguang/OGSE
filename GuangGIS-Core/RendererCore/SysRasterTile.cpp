#include "SysRasterTile.h"
#include "SysImagePreProcess.h"

namespace SysDraw
{

	SysRasterTile::SysRasterTile(const SysDisplayTransform& dispTran,const SysEnvelope& env)
	{
		glEnable(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);

		glGenTextures(1,&m_nTexId);
		glBindTexture(GL_TEXTURE_2D,m_nTexId);

		//使用纹理渲染图像
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256,
			256, 0, GL_RGBA, GL_UNSIGNED_BYTE,NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

		//初始化
		m_DispTran = dispTran;
		m_TileEnv = env;
	}

	SysRasterTile::~SysRasterTile(void)
	{
		glDeleteTextures(1,&m_nTexId);
	}

	void SysRasterTile::Draw() const
	{
		glBindTexture(GL_TEXTURE_2D,m_nTexId);

		//将地理坐标转换为屏幕坐标
		RECT rectView;
		m_DispTran.ToScreenRect(m_TileEnv,rectView);

		//将屏幕坐标转换为OpenGL世界坐标
		double winMinx,winminy,winMaxx,winmaxy,dbZ;
		ScreenToWorld(rectView.left,rectView.top,&winMinx,&winmaxy,&dbZ);
		ScreenToWorld(rectView.right,rectView.bottom,&winMaxx,&winminy,&dbZ);

		glBegin(GL_QUADS);
			glVertex3f(winMinx,winminy,0);glTexCoord2f(0.0,0.0);
			glVertex3f(winMinx,winmaxy,0);glTexCoord2f(1.0,0.0);
			glVertex3f(winMaxx,winmaxy,0);glTexCoord2f(1.0,1.0);
			glVertex3f(winMaxx,winminy,0);glTexCoord2f(0.0,1.0);
		glEnd();
	}

	void SysRasterTile::UpdateSubDate(int nXoffset,int nYoffset,int nWidth,int nHeight,const void* pPixels)
	{
		glBindTexture(GL_TEXTURE_2D,m_nTexId);
		glTexSubImage2D(GL_TEXTURE_2D,0,nXoffset,nYoffset,nWidth,nHeight,GL_RGBA,GL_UNSIGNED_BYTE,pPixels);
	}

	void SysRasterTile::SetDispTran(const SysDisplayTransform &dispTran)
	{
		m_DispTran = dispTran;
	}

	void SysRasterTile::SetEnvlope(const SysEnvelope& env)
	{
		m_TileEnv = env;
	}

}
