LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := FreeImage
LOCAL_SRC_FILES := libFreeImage.so

#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../Math3d/

include $(PREBUILT_SHARED_LIBRARY)