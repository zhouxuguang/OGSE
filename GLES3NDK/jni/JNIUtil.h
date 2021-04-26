/*
 * JNIUtil.h
 *
 *  Created on: 2016-11-9
 *      Author: zhouxuguang
 */

#ifndef OGSESDK_JNIUTIL_INCLUDE_H_
#define OGSESDK_JNIUTIL_INCLUDE_H_

#include <jni.h>
#include <string>
#include <vector>

struct ImageData
{
	int width;
	int height;
	void* data;
};

class JNIUtil {
private:
	JNIUtil();
	~JNIUtil();

public:
	static void MakeCString(JNIEnv* env, jstring str,std::string& outString);

	static jstring MakeJString(JNIEnv* env, const std::string& str);

	static void GetIntArray(JNIEnv* env, jintArray intArr,std::vector<jint>& outArr);

	static void GetFloatArray(JNIEnv* env, jfloatArray floatArr,std::vector<jfloat>& outArr);

	//获得当前Android程序的路�?
	//static std::string GetCurrentDir(GLES2);

	//获得Java的类以及对象相关的函�?
//	static bool getStaticMethodInfo(JniMethodInfo &methodinfo,
//	                                    const char *className,
//	                                    const char *methodName,
//	                                    const char *paramCode);

	void Test();
};

#endif /* OGSESDK_JNIUTIL_INCLUDE_H_ */
