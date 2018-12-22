LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libBaseLib
OGSE_PATH := $(LOCAL_PATH)/../../..

#ͷ�ļ�Ŀ¼					
LOCAL_C_INCLUDES := $(OGSE_PATH)/GuangGIS-SDK/include/BaseLib

#$(warning "the value of HOST_OS is $(HOST_OS)")
	
#Դ�ļ�Ŀ¼				
MY_FILES_PATH  :=  $(OGSE_PATH)/GuangGIS-Core/BaseLib

MY_FILES_SUFFIX := %.cpp %.c

My_All_Files := $(foreach src_path,$(MY_FILES_PATH), $(shell find "$(src_path)" -type f) ) 
My_All_Files := $(My_All_Files:$(MY_CPP_PATH)/./%=$(MY_CPP_PATH)%)
MY_SRC_LIST  := $(filter $(MY_FILES_SUFFIX),$(My_All_Files)) 
MY_SRC_LIST  := $(MY_SRC_LIST:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES := $(MY_SRC_LIST)

MY_FILES_PATH  :=  $(MY_FILES_PATH:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES :=  $(MY_FILES_PATH)/AtomicOps.cpp \
					$(MY_FILES_PATH)/AlignedMalloc.cpp \
					$(MY_FILES_PATH)/Base64.cpp \
					$(MY_FILES_PATH)/ByteEncode.cpp \
					$(MY_FILES_PATH)/BytesSwap.cpp \
					$(MY_FILES_PATH)/Condition.cpp \
					$(MY_FILES_PATH)/ConvertUTF.cpp \
					$(MY_FILES_PATH)/Crc32Code.cpp \
					$(MY_FILES_PATH)/DataCompress.cpp \
					$(MY_FILES_PATH)/DateTime.cpp \
					$(MY_FILES_PATH)/DynamicLib.cpp \
					$(MY_FILES_PATH)/EnvironmentUtility.cpp \
					$(MY_FILES_PATH)/FileUtil.cpp \
					$(MY_FILES_PATH)/FileName.cpp \
					$(MY_FILES_PATH)/GuidGenerator.cpp \
					$(MY_FILES_PATH)/HashFunction.cpp \
					$(MY_FILES_PATH)/HashTable.cpp \
					$(MY_FILES_PATH)/JenkinsHash.cpp \
					$(MY_FILES_PATH)/Log.cpp \
					$(MY_FILES_PATH)/LogService.cpp \
					$(MY_FILES_PATH)/MutexLock.cpp \
					$(MY_FILES_PATH)/Pugixml.cpp \
					$(MY_FILES_PATH)/UrlEncode.cpp \
					$(MY_FILES_PATH)/Random.cpp \
					$(MY_FILES_PATH)/Reference.cpp \
					$(MY_FILES_PATH)/RWLock.cpp \
					$(MY_FILES_PATH)/Semaphore.cpp \
					$(MY_FILES_PATH)/SpinLock.cpp \
					$(MY_FILES_PATH)/StringConverter.cpp \
					$(MY_FILES_PATH)/StringUtil.cpp \
					$(MY_FILES_PATH)/SwapPtr.cpp \
					$(MY_FILES_PATH)/SystemInfo.cpp\
					$(MY_FILES_PATH)/TaskQueue.cpp \
					$(MY_FILES_PATH)/TaskRunner.cpp \
					$(MY_FILES_PATH)/Thread.cpp \
					$(MY_FILES_PATH)/ThreadPool.cpp \
					$(MY_FILES_PATH)/ThreadUtil.cpp \
					$(MY_FILES_PATH)/TimeCost.cpp \
					$(MY_FILES_PATH)/Timer.cpp \
					$(MY_FILES_PATH)/ZigZag.cpp \
					$(MY_FILES_PATH)/md5/md5.cpp \
					$(MY_FILES_PATH)/lz4/lz4.c \
					$(MY_FILES_PATH)/CryptoHash.cpp

$(warning "the value of MY_SRC_LIST is $(LOCAL_SRC_FILES)")

#$(warning "the value of LOCAL_C_INCLUDES is $(LOCAL_C_INCLUDES)")
				
# Compilation flags
LOCAL_CFLAGS :=-Wdeprecated-declarations -D__GXX_EXPERIMENTAL_CXX0X__ -Werror=attributes -fvisibility=hidden

LOCAL_CPPFLAGS += -fvisibility=hidden
LOCAL_CFLAGS += -fvisibility=hidden -fvisibility-inlines-hidden
LOCAL_CFLAGS += -ffunction-sections -fdata-sections
LOCAL_LDFLAGS += -Wl,--gc-sections

LOCAL_WHOLE_STATIC_LIBRARIES += android_support

LOCAL_LDLIBS    := -llog


include $(BUILD_SHARED_LIBRARY)
$(call import-module, android/support)