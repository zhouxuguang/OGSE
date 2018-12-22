package com.example.gles3ndk;

import android.content.Context;
import android.opengl.GLSurfaceView;

public class MySurfaceView extends GLSurfaceView {
	
	private GlRenderer mRenderer;
	private float mPreviousX;
	private float mPreviousY;
	private final float TOUCH_SCALE_FACTOR = 180.0f / 320;
	
	public MySurfaceView(Context context) {
		super(context);
		mRenderer = new GlRenderer();
		
		setEGLContextClientVersion(3);
		this.setRenderer(mRenderer);
		this.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
	}
}
