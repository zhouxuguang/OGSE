LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := Math3D
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libMath3D.so

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../Math3d/

include $(PREBUILT_SHARED_LIBRARY)