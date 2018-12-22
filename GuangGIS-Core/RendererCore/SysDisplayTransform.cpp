#include "SysDisplayTransform.h"
#include "SysCoordinate.h"

#include <GL/glew.h>


namespace SysDraw
{
	void OpenGLViewPort(const SysEnvelope& envVis,const RECT& rectView)
	{
		int height = abs(rectView.bottom - rectView.top);
		int width = abs(rectView.right - rectView.left);
		if ( height == 0 )  
		{    
			height = 1;  
		} 

		int nWidth = width < height ? width : height;
		glViewport( 0, 0, (GLint)width, (GLint)height ); 

		//设置为正射投影
		glMatrixMode( GL_PROJECTION );  
		glLoadIdentity();
		glOrtho(envVis.minX,envVis.maxX,envVis.minY,envVis.maxY,-1.0f,1.0f);

		glMatrixMode( GL_MODELVIEW );  
		glLoadIdentity();
	}

	void CalViewRect(const SysEnvelope& envGeo,RECT& rectView,double dbPixelsPerMapUnit)
	{
		//首先计算中心屏幕坐标
		long centerX = rectView.left + (rectView.right-rectView.left)/2;
		long centerY = rectView.top + (rectView.bottom-rectView.top)/2;
		
		//计算地理范围长度和宽度的一半
		double dbWidth = envGeo.GetWidth()/2;
		double dbHeight = envGeo.GetHeight()/2;
		long lLeft = centerX - dbWidth*dbPixelsPerMapUnit;
		long lRight = (long)(centerX + dbWidth*dbPixelsPerMapUnit + 0.5);
		long lTop = centerY - dbHeight*dbPixelsPerMapUnit;
		long lBottom = (long)(centerY + dbHeight*dbPixelsPerMapUnit + 0.5);
		rectView.left = lLeft;
		rectView.right = lRight;
		rectView.bottom = lBottom;
		rectView.top = lTop;

		return;
	}

	SysDisplayTransform::SysDisplayTransform()
	{
		m_DeviceFrame.left = 0;
		m_DeviceFrame.right = 0;
		m_DeviceFrame.top = 0;
		m_DeviceFrame.bottom = 0;
		m_dbDegree = 0;

		//设置可见地理范围(虚拟范围x-y都是从-1到1之间)
		m_VisibleBounds.minX = -1;
		m_VisibleBounds.maxX = 1;
		m_VisibleBounds.minY = -1;
		m_VisibleBounds.maxY = 1;
		m_dbMapUnitsPerPixel = 1;
		m_dbPixelsPerMapUnit = 1;
	}

	SysDisplayTransform::SysDisplayTransform(
		double dbMinx, 
		double dbMaxx, 
		double dbMiny, 
		double dbMaxy, 
		double dbMapUnitsPerPixel)
	{
		m_DeviceFrame.left = 0;
		m_DeviceFrame.right = 0;
		m_DeviceFrame.top = 0;
		m_DeviceFrame.bottom = 0;
		m_dbDegree = 0;

		m_ViewRect.left = 0;
		m_ViewRect.right = 0;
		m_ViewRect.top = 0;
		m_ViewRect.bottom = 0;

		//设置可见地理范围
		m_VisibleBounds.minX = dbMinx;
		m_VisibleBounds.maxX = dbMaxx;
		m_VisibleBounds.minY = dbMiny;
		m_VisibleBounds.maxY = dbMaxy;
		m_dbMapUnitsPerPixel = dbMapUnitsPerPixel;
		m_dbPixelsPerMapUnit = 1.0/m_dbMapUnitsPerPixel;
	}


	SysDisplayTransform::SysDisplayTransform(
		SysDraw::SysEnvelope& VisualBounds,
		double dbMapUnitsPerPixel)
	{
		m_DeviceFrame.left = 0;
		m_DeviceFrame.right = 0;
		m_DeviceFrame.top = 0;
		m_DeviceFrame.bottom = 0;
		m_dbDegree = 0;

		m_ViewRect.left = 0;
		m_ViewRect.right = 0;
		m_ViewRect.top = 0;
		m_ViewRect.bottom = 0;

		//设置可见地理范围
		m_VisibleBounds = VisualBounds;
		m_dbMapUnitsPerPixel = dbMapUnitsPerPixel;
		m_dbPixelsPerMapUnit = 1.0/m_dbMapUnitsPerPixel;
	}

	SysDisplayTransform::SysDisplayTransform(const SysDisplayTransform& OtherTran)
	{
		m_Bounds = OtherTran.m_Bounds;				//全图地理坐标范围
		m_VisibleBounds = OtherTran.m_VisibleBounds; //“可见”地理坐标范围

		m_DeviceFrame = OtherTran.m_DeviceFrame;	//“可见”地理范围的设备坐标范围
		m_dbDegree = OtherTran.m_dbDegree;						//旋转角度
		m_dbDeviceWidth = OtherTran.m_dbDeviceWidth;
		m_dbDeviceHeight = OtherTran.m_dbDeviceHeight;

		m_ViewRect = OtherTran.m_ViewRect;

		m_dbMapUnitsPerPixel = OtherTran.m_dbMapUnitsPerPixel;
		m_dbPixelsPerMapUnit = OtherTran.m_dbPixelsPerMapUnit;

		m_lViewWidth = OtherTran.m_lViewWidth;
		m_lViewHeight = OtherTran.m_lViewHeight;
	}

	SysDisplayTransform& SysDisplayTransform::operator=(const SysDisplayTransform& OtherTran)
	{
		m_Bounds = OtherTran.m_Bounds;				//全图地理坐标范围
		m_VisibleBounds = OtherTran.m_VisibleBounds; //“可见”地理坐标范围

		m_DeviceFrame = OtherTran.m_DeviceFrame;	//“可见”地理范围的设备坐标范围
		m_dbDegree = OtherTran.m_dbDegree;						//旋转角度
		m_dbDeviceWidth = OtherTran.m_dbDeviceWidth;
		m_dbDeviceHeight = OtherTran.m_dbDeviceHeight;

		m_ViewRect = OtherTran.m_ViewRect;

		m_dbMapUnitsPerPixel = OtherTran.m_dbMapUnitsPerPixel;
		m_dbPixelsPerMapUnit = OtherTran.m_dbPixelsPerMapUnit;

		m_lViewWidth = OtherTran.m_lViewWidth;
		m_lViewHeight = OtherTran.m_lViewHeight;

		return *this;
	}

	SysDisplayTransform::~SysDisplayTransform(void)
	{

	}

	void SysDisplayTransform::GetBounds(SysDraw::SysEnvelope& Bounds) const
	{
		Bounds = m_Bounds;
	}

	void SysDisplayTransform::SetBounds(const SysDraw::SysEnvelope& Bounds)
	{
		m_Bounds = Bounds;
	}

	void SysDisplayTransform::GetVisibleBounds(SysDraw::SysEnvelope& Bounds) const
	{
		Bounds = m_VisibleBounds;
	}

	void SysDisplayTransform::SetVisibleBounds(/*const*/ SysDraw::SysEnvelope& Bounds)
	{
		//if(m_VisibleBounds.equals(&Bounds)) return;
		if(fabs(Bounds.GetWidth())<FLOAT_ZERO) return;
		if(fabs(Bounds.GetHeight())<FLOAT_ZERO) return;

		m_VisibleBounds = Bounds;
		CalculateVisibleExtents();

		OpenGLViewPort(m_VisibleBounds,m_ViewRect);

		Bounds = m_VisibleBounds;
	}

	void SysDisplayTransform::SetVisibleBoundsExt(const SysDraw::SysEnvelope& Bounds)
	{
		//if(m_VisibleBounds.equals(&Bounds)) return;
		if(fabs(Bounds.GetWidth())<FLOAT_ZERO) return;
		if(fabs(Bounds.GetHeight())<FLOAT_ZERO) return;

		m_VisibleBounds = Bounds;

		CalViewRect(m_VisibleBounds,m_ViewRect,m_dbPixelsPerMapUnit);

		OpenGLViewPort(m_VisibleBounds,m_ViewRect);
	}

	void SysDisplayTransform::GetExportFrame(RECT& rtFrame) const
	{
		rtFrame.left = m_DeviceFrame.left;
		rtFrame.right = m_DeviceFrame.right;
		rtFrame.top = m_DeviceFrame.top;
		rtFrame.bottom = m_DeviceFrame.bottom;

		/*rtFrame.left = m_ViewRect.left;
		rtFrame.right = m_ViewRect.right;
		rtFrame.top = m_ViewRect.top;
		rtFrame.bottom = m_ViewRect.bottom;*/
	}

	void SysDisplayTransform::SetExportFrame(const RECT& rtFrame)
	{
		m_DeviceFrame.left = rtFrame.left;
		m_DeviceFrame.right = rtFrame.right;
		m_DeviceFrame.top = rtFrame.top;
		m_DeviceFrame.bottom = rtFrame.bottom;

		m_lViewWidth = abs(rtFrame.right - rtFrame.left);
		m_lViewHeight = abs(rtFrame.bottom - rtFrame.top);

		m_ViewRect = m_DeviceFrame;
		if (!m_VisibleBounds.IsNull())
		{
			//CalViewRect(m_VisibleBounds,m_ViewRect,m_dbPixelsPerMapUnit);
			CalculateVisibleExtents();
		}
		OpenGLViewPort(m_VisibleBounds,m_ViewRect);
	}

	void SysDisplayTransform::SetViewRect(const RECT& viewRect)
	{
		m_ViewRect = viewRect;
	}

	void SysDisplayTransform::SetMapUnitsPerPixel(double dbMapUnitsPerPixel)
	{
		m_dbMapUnitsPerPixel = dbMapUnitsPerPixel;
		m_dbPixelsPerMapUnit = 1.0f/m_dbMapUnitsPerPixel;
	}

	double SysDisplayTransform::GetMapUnitsPerPixel() const
	{
		return m_dbMapUnitsPerPixel;
	}

	void SysDisplayTransform::ToMapPoint(long lX, long lY, double& x, double& y) const
	{
		double left = m_VisibleBounds.minX;
		double right = m_VisibleBounds.maxX;
		double bottom = m_VisibleBounds.minY;
		double top = m_VisibleBounds.maxY;

		//Screen To Map
		x = left + lX * m_dbMapUnitsPerPixel;
		y = top - lY * m_dbMapUnitsPerPixel;
	}

	void SysDisplayTransform::ToMapPoint(const POINT& screenPoint, SysCoordinate &geoPoint) const
	{
		double dbX = 0;
		double dbY = 0;
		ToMapPoint(screenPoint.x,screenPoint.y,dbX,dbY);
		geoPoint.x = dbX;
		geoPoint.y = dbY;
	}

	void SysDisplayTransform::ToMapRect(const RECT& viewRect, SysEnvelope& envGeo) const
	{
		double dbX[2];
		double dbY[2];

		ToMapPoint(viewRect.left,viewRect.top,dbX[0],dbY[0]);
		ToMapPoint(viewRect.right,viewRect.bottom,dbX[1],dbY[1]);
		envGeo.minX = min(dbX[0],dbX[1]);
		envGeo.maxX = max(dbX[0],dbX[1]);
		envGeo.minY = min(dbY[0],dbY[1]);
		envGeo.maxY = max(dbY[0],dbY[1]);

		return;
	}

	bool SysDisplayTransform::ToScreenPoint(double dbGeoX, double dbGeoY,long& lX, long& lY) const
	{
		if((m_DeviceFrame.right - m_DeviceFrame.left) == 0 ||(m_DeviceFrame.bottom - m_DeviceFrame.top) == 0)
		{
			return 0;
		}
		double dMinX = m_VisibleBounds.minX;          
		double dMaxY = m_VisibleBounds.maxY; 

		lX = (long)((dbGeoX - dMinX) * m_dbPixelsPerMapUnit + 0.5 + m_ViewRect.left);
		lY = (long)((dMaxY - dbGeoY) * m_dbPixelsPerMapUnit + 0.5 + m_ViewRect.top);
		return 1;
	}

	bool SysDisplayTransform::ToScreenPoint(const SysCoordinate& geoPoint,POINT& screenPoint) const
	{
		long x = 0;
		long y = 0;
		ToScreenPoint(geoPoint.x,geoPoint.y,x,y);
		screenPoint.x = x;
		screenPoint.y = y;

		return true;
	}

	bool SysDisplayTransform::ToScreenRect(const SysEnvelope &envGeo,RECT &rectView) const
	{
		if((m_DeviceFrame.right - m_DeviceFrame.left) == 0 ||(m_DeviceFrame.bottom - m_DeviceFrame.top) == 0)
		{
			return 0;
		}
		double dMinX = m_VisibleBounds.minX;          
		double dMaxY = m_VisibleBounds.maxY; 

		//左上角点
		long lX1 = (long)((envGeo.minX - dMinX) * m_dbPixelsPerMapUnit + 0.5 + m_ViewRect.left);
		long lY1 = (long)((dMaxY - envGeo.maxY) * m_dbPixelsPerMapUnit + 0.5 + m_ViewRect.top);

		//右下角点
		long lX2 = (long)((envGeo.maxX - dMinX) * m_dbPixelsPerMapUnit + 0.5 + m_ViewRect.left);
		long lY2 = (long)((dMaxY - envGeo.minY) * m_dbPixelsPerMapUnit + 0.5 + m_ViewRect.top);
		
		rectView.left = lX1;
		rectView.right = lX2;
		rectView.bottom = lY2;
		rectView.top = lY1;

		return 0;
	}

	bool SysDisplayTransform::ToScreenRect( const SysEnvelope &envGeo,RECTD &rectView ) const
	{
		if((m_DeviceFrame.right - m_DeviceFrame.left) == 0 ||(m_DeviceFrame.bottom - m_DeviceFrame.top) == 0)
		{
			return 0;
		}
		double dMinX = m_VisibleBounds.minX;          
		double dMaxY = m_VisibleBounds.maxY; 

		//左上角点
		double lX1 = ((envGeo.minX - dMinX) * m_dbPixelsPerMapUnit  + m_ViewRect.left);
		double lY1 = ((dMaxY - envGeo.maxY) * m_dbPixelsPerMapUnit  + m_ViewRect.top);

		//右下角点
		double lX2 = ((envGeo.maxX - dMinX) * m_dbPixelsPerMapUnit + m_ViewRect.left);
		double lY2 = ((dMaxY - envGeo.minY) * m_dbPixelsPerMapUnit + m_ViewRect.top);

		rectView.left = lX1;
		rectView.right = lX2;
		rectView.bottom = lY2;
		rectView.top = lY1;

		return 0;
	}

	void SysDisplayTransform::CalculateVisibleExtents()
	{
		double left,right,bottom,top;
		left = m_VisibleBounds.minX;
		right = m_VisibleBounds.maxX;
		bottom = m_VisibleBounds.minY;
		top = m_VisibleBounds.maxY;

		double xrange = right - left;
		double yrange = top - bottom;
		double xextent, yextent;
		double xadjust, yadjust;
		long viewWidth = m_DeviceFrame.right - m_DeviceFrame.left + 1;
		long viewHeight = m_DeviceFrame.bottom - m_DeviceFrame.top + 1;
		if (xrange > yrange)
		{
			yextent = (xrange / viewWidth) * viewHeight;
			xextent = xrange;
			yadjust = (yextent - yrange) * .5;
			xadjust = 0;
			if( yextent < yrange )
			{
				yextent = yrange;
				xextent = (yrange / viewHeight) * viewWidth;
				yadjust = 0;
				xadjust = (xextent - xrange) * .5;
			}
		}
		else
		{
			xextent = (yrange / viewHeight) * viewWidth;
			yextent = yrange;
			xadjust = (xextent - xrange) * .5;
			yadjust = 0;

			if( xextent < xrange )
			{
				xextent = xrange;
				yextent = (xrange / viewWidth) * viewHeight;
				xadjust = 0;
				yadjust = (yextent - yrange) * .5;
			}
		}

		left = left - xadjust;
		right = left + xextent;
		bottom = bottom - yadjust;
		top = bottom + yextent;
		xrange = right - left;
		yrange = top - bottom;

		m_VisibleBounds.Init(left,right,bottom,top);
		//m_DeviceFrame = m_ViewRect;

		//ToScreenRect(m_VisibleBounds,m_ViewRect);
		//CalViewRect(m_VisibleBounds,m_ViewRect,m_dbMapUnitsPerPixel);

		/*double x1,y1;
		ToMapPoint(m_ViewRect.left,m_ViewRect.top,x1,y1);
		double x2,y2;
		ToMapPoint(m_ViewRect.right,m_ViewRect.bottom,x2,y2);*/

		//ToMapRect(m_ViewRect,m_VisibleBounds);

		/*m_VisibleBounds.minX = x1;
		m_VisibleBounds.maxX = x2;
		m_VisibleBounds.minY = y2;
		m_VisibleBounds.maxY = y1;*/
		//m_DeviceFrame = m_ViewRect;

		//CalViewRect(m_VisibleBounds,m_DeviceFrame,m_dbMapUnitsPerPixel);

		/*viewWidth = m_ViewRect.right - m_ViewRect.left + 1;
		viewHeight = m_ViewRect.bottom - m_ViewRect.top + 1;

		double dbScaleX = xrange/viewWidth;
		double dbScaleY = yrange/viewHeight;
		double dbScale = max(dbScaleX,dbScaleY);

		
		SetMapUnitsPerPixel(dbScale);*/

		
	}

	long SysDisplayTransform::GetViewWidth() const
	{
		return m_lViewWidth;
	}

	long SysDisplayTransform::GetViewHeight() const
	{
		return m_lViewHeight;
	}

}
