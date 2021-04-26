/*
 * com_example_ndkgles_GLinterface.cpp
 *
 *  Created on: 2016-11-7
 *      Author: zhouxuguang
 */
#include "com_example_gles3ndk_GLinterface.h"
#include "NDKGL2.h"
#include "DrawFile/BasicShader/BasicShader.h"
#include "DrawFile/BasicShader/UniformShader.h"
#include "DrawFile/BasicShader/DiffuseShader.h"
#include "DrawFile/BasicShader/ADSShader.h"
#include "DrawFile/BasicShader/TwoSideShader.h"

#include "DrawFile/Light/scenemultilight.h"
#include "DrawFile/Texture/scenetexture.h"
#include "DrawFile/Texture/scenenormalmap.h"
#include "DrawFile/Texture/sceneprojtex.h"
#include "DrawFile/Texture/sceneshadow.h"

#include <android/asset_manager_jni.h>
#include <android/log.h>

#include "JNIUtil.h"

#include <android/bitmap.h>

SceneShadowMap* pBasicShder = NULL;

/*
 * Class:     com_example_ndkgles_GLinterface
 * Method:    onDrawFrame
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_gles3ndk_GLinterface_onDrawFrame(JNIEnv *env, jclass clsObj)
{
	//OnDrawGlFrame();
	pBasicShder->Draw();
}

/*
 * Class:     com_example_ndkgles_GLinterface
 * Method:    onSurfaceChanged
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_example_gles3ndk_GLinterface_onSurfaceChanged(JNIEnv *env, jclass clsObj, jint width, jint height)
{
	//OnResize(width,height);
	pBasicShder->Resize(width, height);
}

/*
 * Class:     com_example_ndkgles_GLinterface
 * Method:    onSurfaceCreated
 * Signature: (Ljavax/microedition/khronos/egl/EGLConfig;)V
 */
JNIEXPORT void JNICALL Java_com_example_gles3ndk_GLinterface_onSurfaceCreated(JNIEnv *env, jclass clsObj, jobject obj)
{
	//OnInited();
	pBasicShder = new SceneShadowMap();
	pBasicShder->Init();
}

#include "GLSLProgram.h"

 extern std::string strVert;
 extern std::string strFrag;

void GetShaderFile(AAssetManager* mgr,const char* vertFile,const char* fragFile)
{
	//打开顶点shader
	AAsset* asset = AAssetManager_open(mgr,vertFile,AASSET_MODE_UNKNOWN);
	off_t lenght = AAsset_getLength(asset);
	__android_log_print(ANDROID_LOG_INFO,"GLES2","length = %ld",lenght);
	strVert.resize(lenght+1,0);
	memcpy((char*)strVert.data(),AAsset_getBuffer(asset),lenght);
	__android_log_print(ANDROID_LOG_INFO,"GLES2","content = %s",strVert.c_str());
	AAsset_close(asset);

	asset = AAssetManager_open(mgr,fragFile,AASSET_MODE_UNKNOWN);
	lenght = AAsset_getLength(asset);
	__android_log_print(ANDROID_LOG_INFO,"GLES2","length = %ld",lenght);
	strFrag.resize(lenght+1,0);
	memcpy((char*)strFrag.data(),AAsset_getBuffer(asset),lenght);
	__android_log_print(ANDROID_LOG_INFO,"GLES2","content = %s",strFrag.c_str());
	AAsset_close(asset);
}

AAssetManager* mgr = NULL;

JNIEXPORT void JNICALL Java_com_example_gles3ndk_GLinterface_initializeAssetManager(JNIEnv *env, jclass clsObj, jobject assetManager)
{
	mgr = AAssetManager_fromJava(env, assetManager);

    std::string *s2 = NULL;
    std::string s1 = "周旭光ssssssssssssssssssssssssssssssssssssss";
    s2 = new std::string();
    *s2 = s1;
    s2->size();

    std::string *eventId = new(std::nothrow) std::string(s1.c_str());

    if(s2->c_str() == s1.c_str())
    {
    		__android_log_print(ANDROID_LOG_INFO,"", "指针地址一样");
    }

    if (eventId->c_str() == s1.c_str())
    {
    		__android_log_print(ANDROID_LOG_INFO,"", "eventId -----指针地址一样");
    }

	//GetShaderFile(mgr,"uniformBlock.vsh","uniformBlock.fsh");
	//GetShaderFile(mgr,"ADS.vsh","ADS.fsh");
	//GetShaderFile(mgr,"twoSide.vsh","twoSide.fsh");
	//GetShaderFile(mgr,"multiLight.vsh","multiLight.fsh");
	//GetShaderFile(mgr,"Texture.vsh","Texture.fsh");
	//GetShaderFile(mgr,"normalmap.vs","normalmap.fs");
    GetShaderFile(mgr,"shadowmap.vert","shadowmap1.frag");
}

std::string mediaPath;


#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

#include <sys/system_properties.h>
#include <android/configuration.h>
#include <android/native_window_jni.h>

#define  LOG_TAG    "GLES2"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

JNIEXPORT void JNICALL Java_com_example_gles3ndk_GLinterface_setInitName
  (JNIEnv *env, jclass clsObj, jstring str)
{
	JNIUtil::MakeCString(env,str,mediaPath);
	__android_log_print(ANDROID_LOG_INFO,"GLES2","strName = %s",mediaPath.c_str());

	//读取序号
	char m_szDevID[PROP_VALUE_MAX];
	__system_property_get("ro.serialno",m_szDevID);
	__android_log_print(ANDROID_LOG_INFO,"GLES2","m_szDevID = %s",m_szDevID);

	//读取机型
	char m_szDevName[PROP_VALUE_MAX];
	__system_property_get("ro.product.model",m_szDevName);
	__android_log_print(ANDROID_LOG_INFO,"GLES2","m_szDevName = %s",m_szDevName);

	//读取sdk版本
	char m_szSdkVer[PROP_VALUE_MAX];
	__system_property_get("ro.build.version.sdk", m_szSdkVer);
	__android_log_print(ANDROID_LOG_INFO,"GLES2","m_szSdkVer = %s",m_szSdkVer);

	AConfiguration* conf = AConfiguration_new();
	AConfiguration_fromAssetManager(conf,mgr);

	int scale = AConfiguration_getScreenSize(conf);
	int width = AConfiguration_getScreenWidthDp(conf);
	int height = AConfiguration_getScreenHeightDp(conf);
	int32_t dpi = AConfiguration_getDensity(conf);

	__android_log_print(ANDROID_LOG_INFO,"GLES2","dpi = %d, width = %d, height = %d",dpi,width,height);

	int index = 0;
	while(true)
	{
		const prop_info *info = __system_property_find_nth(index);
		if(NULL == info)
		{
			break;
		}

		char szName[PROP_NAME_MAX];
		char szValue[PROP_VALUE_MAX];
		__system_property_read(info,szName,szValue);
		__android_log_print(ANDROID_LOG_INFO,"GLES2","%s = %s",szName,szValue);

		index++;
	}

	//ANativeWindow* ANativeWindow_fromSurface(JNIEnv* env, jobject surface);

	//chmod("/dev/graphics/fb0",O_RDWR);
//	struct fb_var_screeninfo fb_var;
//
//	int fd = open("/dev/graphics/fb0",O_RDWR);
//	if(-1 == fd)
//	{
//		__android_log_print(ANDROID_LOG_INFO,"GLES2","file not found : /dev/graphics/fb0");
//	}
//	//
//	//screen w , h , bpp
//	//get screen information
//	ioctl(fd,FBIOGET_VSCREENINFO,&fb_var);
//	int w = fb_var.xres;
//	int h = fb_var.yres;
//	int bpp = fb_var.bits_per_pixel;
//
//	__android_log_print(ANDROID_LOG_INFO,"GLES2","width = %d,height =%d, bpp = %d",w,h,bpp);
//
//	//fbmem = Framebuffer address
//	void *fbmem = mmap (0,w*h*bpp/8,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
}

JNIEXPORT void JNICALL Java_com_example_gles3ndk_GLinterface_setSurface(JNIEnv *env, jclass cls, jobject surface)
{
	ANativeWindow* aWindow = ANativeWindow_fromSurface(env, surface);
	if (aWindow != NULL)
	{
		int32_t width = ANativeWindow_getWidth(aWindow);
		int32_t height = ANativeWindow_getHeight(aWindow);

		__android_log_print(ANDROID_LOG_INFO,"GLES2","surface width = %d , height =  %d",width,height);
		ANativeWindow_release(aWindow);
	}
}

/*
 * Class:     com_example_gles3ndk_GLinterface
 * Method:    setBitmap
 * Signature: (Landroid/graphics/Bitmap;)V
 */
JNIEXPORT void JNICALL Java_com_example_gles3ndk_GLinterface_setBitmap
  (JNIEnv *env, jclass clsObj, jobject jBitmap)
{
	AndroidBitmapInfo bmpInfo;
	int ret = AndroidBitmap_getInfo(env,jBitmap,&bmpInfo);
	if (ret < 0)
	{
		//
	}

	//JAVA端是ARGB，c端是RGBA

	unsigned char* pImage = NULL;
	AndroidBitmap_lockPixels(env,jBitmap,(void **)&pImage);

	if (ANDROID_BITMAP_FORMAT_RGB_565 == bmpInfo.format)
	{
//		uint16_t buf;
//		for (int nRow = 0; nRow < bmpInfo.height; nRow ++)
//		{
//			unsigned char * pLine = pImage + nRow * bmpInfo.stride;
//			for (int nCol = 0; nCol < bmpInfo.width; nCol ++)
//			{
//				memcpy(&buf,pLine+nCol,2);
//				unsigned char red = (buf & 0xf800) >> 11;
//				unsigned char green = (buf & 0x07e0) >> 5;
//				unsigned char blue = buf & 0x001f;
//			}
//		}
	}

	else if (ANDROID_BITMAP_FORMAT_RGBA_8888 == bmpInfo.format)
	{
//		for (int nRow = 0; nRow < bmpInfo.height; nRow ++)
//		{
//			unsigned char * pLine = pImage + nRow * bmpInfo.stride;
//			for (int nCol = 0; nCol < bmpInfo.width; nCol ++)
//			{
//				pLine[nCol*4] = 255;		//R
//				pLine[nCol*4+1] = 0;
//				pLine[nCol*4+2] = 255;
//				pLine[nCol*4+3] = 255;	//A
//			}
//		}

//		img.height = bmpInfo.height;
//		img.width = bmpInfo.width;
//		img.data = pImage;
	}

	AndroidBitmap_unlockPixels(env,jBitmap);
}


