package com.example.gles3ndk;

import java.util.UUID;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.support.v4.view.GestureDetectorCompat;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;



public class MainActivity extends Activity {
	
	private MySurfaceView mGlView;
	
	private GestureDetectorCompat mDetector;
	
	static
	{
		System.loadLibrary("NdkGLES");
	}

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        
        AssetManager assetManager = getAssets();
        GLinterface.initializeAssetManager(assetManager);
        
        UUID uuid = java.util.UUID.randomUUID();
        String str = uuid.toString();
        
        final ApplicationInfo applicationInfo = this.getApplicationInfo();
        GLinterface.setInitName(str);
        
//        Bitmap bmp = BitmapFactory.decodeFile("/sdcard/texture/brick1.jpg");
//        GLinterface.setBitmap(bmp);
        
        mGlView = new MySurfaceView(this);
        
        //设置手势
//        mGlView.setOnTouchListener(new TouchListener() {
//            public boolean onTouch(View v, MotionEvent event) {
//                // ... Respond to touch events
//                return true;
//            }
//        });
        
        setContentView(mGlView);
    }
    
    protected void onPause() 
    {
		super.onPause();
		mGlView.onPause();
	}

	protected void onResume() 
	{
		super.onResume();
		mGlView.onResume();
	}
    
    public boolean onTouchEvent(MotionEvent event){
		return false;

//        int action = MotionEventCompat.getActionMasked(event);
//        
//        String DEBUG_TAG = "Geoture";
//
//        switch(action) {
//            case (MotionEvent.ACTION_DOWN) :
//                Log.d(DEBUG_TAG,"Action was DOWN");
//                return true;
//            case (MotionEvent.ACTION_MOVE) :
//                Log.d(DEBUG_TAG,"Action was MOVE");
//                return true;
//            case (MotionEvent.ACTION_UP) :
//                Log.d(DEBUG_TAG,"Action was UP");
//                return true;
//            case (MotionEvent.ACTION_CANCEL) :
//                Log.d(DEBUG_TAG,"Action was CANCEL");
//                return true;
//            case (MotionEvent.ACTION_OUTSIDE) :
//                Log.d(DEBUG_TAG,"Movement occurred outside bounds " +
//                        "of current screen element");
//                return true;
//            default :
//                return super.onTouchEvent(event);
//        }
    }
}
