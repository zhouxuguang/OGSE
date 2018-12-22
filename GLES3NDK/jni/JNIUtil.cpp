/*
 * JNIUtil.cpp
 *
 *  Created on: 2016-11-9
 *      Author: zhouxuguang
 */

#include "JNIUtil.h"
#include <stdio.h>

#include <memory>
#include <math.h>

JNIUtil::JNIUtil() {
	// TODO Auto-generated constructor stub

}

JNIUtil::~JNIUtil() {
	// TODO Auto-generated destructor stub
}

void JNIUtil::MakeCString(JNIEnv* env, jstring str,std::string& outString)
{
    if (str == NULL) return;

    jsize length = env->GetStringUTFLength(str);
    const char* src = env->GetStringUTFChars(str, NULL);
    outString = src;
    env->ReleaseStringUTFChars(str, src);
}

jstring JNIUtil::MakeJString(JNIEnv* env, const std::string& str)
{
	return env->NewStringUTF(str.c_str());
}

void JNIUtil::GetIntArray(JNIEnv* env, jintArray intArr,std::vector<jint>& outArr)
{
	outArr.clear();
	if(NULL == intArr) return;

    //1. 获取数组长度
    jsize arr_len = env->GetArrayLength(intArr);
    if(arr_len <= 0) return;

    //2. 申请空间
    outArr.resize(arr_len);

    //3. 拷贝Java数组中的所有元素到缓冲区中
    env->GetIntArrayRegion(intArr,0,arr_len,&outArr[0]);
}

void JNIUtil::GetFloatArray(JNIEnv* env, jfloatArray floatArr,std::vector<jfloat>& outArr)
{
	outArr.clear();
	if(NULL == floatArr) return;

    //1. 获取数组长度
    jsize arr_len = env->GetArrayLength(floatArr);
    if(arr_len <= 0) return;

    //2. 申请空间
    outArr.resize(arr_len);

    //3. 拷贝Java数组中的所有元素到缓冲区中
    env->GetFloatArrayRegion(floatArr,0,arr_len,&outArr[0]);
}

void JNIUtil::Test()
{
	//std::shared_ptr<int> aa = std::make_shared<int>(12);

	std::vector<int> iVec;
	iVec.data();

	round(10.6);

	//auto ii = 10;
}

