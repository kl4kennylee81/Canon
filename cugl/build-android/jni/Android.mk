LOCAL_PATH := $(call my-dir)
CUGL_PATH  := $(LOCAL_PATH)/../../

###########################
#
# CUGL static library
#
###########################

include $(CLEAR_VARS)

LOCAL_MODULE := CUGL_static

LOCAL_MODULE_FILENAME := libCUGL

LOCAL_C_INCLUDES = $(CUGL_PATH)/include

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
	$(wildcard $(CUGL_PATH)/src/base/*.cpp) \
	$(wildcard $(CUGL_PATH)/src/base/platform/*.cpp) \
	$(wildcard $(CUGL_PATH)/src/util/*.cpp) \
	$(wildcard $(CUGL_PATH)/src/math/*.cpp) \
	$(wildcard $(CUGL_PATH)/src/math/polygon/*.cpp) \
	$(wildcard $(CUGL_PATH)/src/input/*.cpp) \
	$(wildcard $(CUGL_PATH)/src/input/gestures/*.cpp) \
	$(wildcard $(CUGL_PATH)/src/io/*.cpp) \
	$(wildcard $(CUGL_PATH)/src/renderer/*.cpp) \
	$(wildcard $(CUGL_PATH)/src/audio/*.cpp) \
	$(wildcard $(CUGL_PATH)/src/audio/platform/CUAudioEngine-SDL.cpp) \
	$(wildcard $(CUGL_PATH)/src/assets/*.cpp) \
	$(wildcard $(CUGL_PATH)/src/2d/*.cpp) \
	$(wildcard $(CUGL_PATH)/src/2d/physics/*.cpp) \
	$(wildcard $(CUGL_PATH)/external/cJSON/*.c) \
	$(wildcard $(CUGL_PATH)/external/box2d/src/Box2D/Collision/*.cpp) \
	$(wildcard $(CUGL_PATH)/external/box2d/src/Box2D/Collision/Shapes/*.cpp) \
	$(wildcard $(CUGL_PATH)/external/box2d/src/Box2D/Common/*.cpp) \
	$(wildcard $(CUGL_PATH)/external/box2d/src/Box2D/Dynamics/*.cpp) \
	$(wildcard $(CUGL_PATH)/external/box2d/src/Box2D/Dynamics/Contacts/*.cpp) \
	$(wildcard $(CUGL_PATH)/external/box2d/src/Box2D/Dynamics/Joints/*.cpp) \
	$(wildcard $(CUGL_PATH)/external/box2d/src/Box2D/Rope/*.cpp))

LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES

LOCAL_LDLIBS := 
LOCAL_EXPORT_LDLIBS := -Wl,--undefined=Java_org_libsdl_app_SDLActivity_nativeInit -ldl -lGLESv1_CM -lGLESv2 -lGLESv3 -llog -landroid

include $(BUILD_STATIC_LIBRARY)
