package com.example.gles3ndk;

import javax.microedition.khronos.egl.EGLConfig;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.view.Surface;

public class GLinterface {
	
	public native static void onDrawFrame();
	
	public native static void onSurfaceChanged(int width, int height);

	public native static void onSurfaceCreated(EGLConfig config);
	
	public native static void initializeAssetManager(AssetManager assetManager);
	
	public native static void setInitName(String name);
	
	public native static void setBitmap(Bitmap bmp);
	
	public native static void setSurface(Surface face);

}
