LOCAL_PATH:= $(call my-dir)/../

include $(CLEAR_VARS)

LOCAL_MODULE    := libMath3D
LOCAL_CFLAGS    := -Werror
LOCAL_SRC_FILES := AxisAlignedBox.cpp \
				AlphaBlend.cpp \
				BitHacks.cpp \
				ColorRGBA.cpp \
				CommonStuff.cpp \
				EulerAngles.cpp	\
				FrameBuffer.cpp	\
				GeometryObject.cpp	\
				GlobalApi.cpp	\
				Math3DCommon.cpp	\
				MathUtil.cpp	\
				Matrix3x3.cpp	\
				Matrix4x3.cpp	\
				Matrix4x4.cpp	\
				ModelTransform.cpp	\
				ModelProject.cpp \
				Plane3D.cpp	\
				Quaternion.cpp	\
				Ray3D.cpp	\
				Triangle3D.cpp \
				RotationMatrix.cpp	\
				ShadeRec.cpp	\
				Sphere.cpp	\
				TriangleRender.cpp	\
				Vector2.cpp \
				Vector3.cpp	\
				Vector4.cpp	\
				ViewPort.cpp \
				Light.cpp \
				Lighting.cpp \
				Material.cpp \
				AntiLine.cpp \
				poly2tri/triangulate.cpp	\

#LOCAL_LDLIBS    := -llog -lGLESv2

LOCAL_STATIC_LIBRARIES += cpufeatures
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_CFLAGS := -DHAVE_NEON=1
    LOCAL_ARM_NEON := true
    #LOCAL_SRC_FILES += MathUtilNeon.h.neon
endif


include $(BUILD_SHARED_LIBRARY)

$(call import-module, android/cpufeatures)
