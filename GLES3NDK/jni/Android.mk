LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := NdkGLES

#头文件目录					
LOCAL_C_INCLUDES := $(LOCAL_PATH)/thirdparty/libjpeg \
					$(LOCAL_PATH)/thirdparty/libpng \
					$(LOCAL_PATH)/thirdparty/libskia \
					$(LOCAL_PATH)/thirdparty/libnoise
	
#源文件目录				
MY_FILES_PATH  :=  $(LOCAL_PATH)

MY_FILES_SUFFIX := %.cpp %.c

#My_All_Files := $(foreach src_path,$(MY_FILES_PATH), $(shell find "$(src_path)" -type f) ) 
#My_All_Files := $(My_All_Files:$(MY_CPP_PATH)/./%=$(MY_CPP_PATH)%)
#MY_SRC_LIST  := $(filter $(MY_FILES_SUFFIX),$(My_All_Files)) 
#MY_SRC_LIST  := $(MY_SRC_LIST:$(LOCAL_PATH)/%=%)
#LOCAL_SRC_FILES := $(MY_SRC_LIST)

#windows下脚本

#ifeq ($(HOST_OS),windows)

# 递归遍历目录下的所有的文件
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# 获取相应的源文件
MY_ALL_FILES := $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*.*) ) 
MY_ALL_FILES := $(MY_ALL_FILES:$(MY_CPP_PATH)/./%=$(MY_CPP_PATH)%)
MY_SRC_LIST  := $(filter $(MY_FILES_SUFFIX),$(MY_ALL_FILES)) 
MY_SRC_LIST  := $(MY_SRC_LIST:$(LOCAL_PATH)/%=%)

# 去除字串的重复单词
define uniq =
  $(eval seen :=)
  $(foreach _,$1,$(if $(filter $_,${seen}),,$(eval seen += $_)))
  ${seen}
endef

# 递归遍历获取所有目录
MY_ALL_DIRS := $(dir $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*/) ) )
MY_ALL_DIRS := $(call uniq,$(MY_ALL_DIRS))

# 赋值给NDK编译系统
LOCAL_SRC_FILES  := $(MY_SRC_LIST)
LOCAL_C_INCLUDES := $(MY_ALL_DIRS)

$(warning "the value of My_All_Files is $(MY_SRC_LIST)")

#LOCAL_SRC_FILES := com_example_gles3ndk_GLinterface.cpp \
				GLConfiguration.cpp \
				GLPrecompile.cpp \
				GLSLProgram.cpp \
				ImageLoad.cpp \
				JNIUtil.cpp \
				RenderTarget.cpp \
				skybox.cpp \
				vbocube.cpp \
				vbomesh.cpp \
				vboplane.cpp \
				vboteapot.cpp \
				vbotorus.cpp \
				DrawFile/Texture/sceneprojtex.cpp

#$(warning "the value of LOCAL_C_INCLUDES is $(src_path)")
				
# Compilation flags
LOCAL_CFLAGS := -Werror -D__GXX_EXPERIMENTAL_CXX0X__
#LOCAL_CPPFLAGS  := -std=c++11

LOCAL_SHARED_LIBRARIES := Math3D \
						FreeImage

# Static libraries to link with
LOCAL_LDLIBS := -llog -landroid -lGLESv1_CM -lGLESv2 -lGLESv3 -ljnigraphics -lz

include $(BUILD_SHARED_LIBRARY)

include $(LOCAL_PATH)/prebuilt/Android.mk
#include $(LOCAL_PATH)/BaseLib.mk

#include $(LOCAL_PATH)/freeimage/Android.mk