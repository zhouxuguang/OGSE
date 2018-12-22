package com.example.gles3ndk;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;

public class GlRenderer implements GLSurfaceView.Renderer{

	@Override
	public void onDrawFrame(GL10 gl) {
		// TODO Auto-generated method stub
		GLinterface.onDrawFrame();
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		// TODO Auto-generated method stub
		GLinterface.onSurfaceChanged(width, height);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		// TODO Auto-generated method stub
		GLinterface.onSurfaceCreated(config);
	}

}
