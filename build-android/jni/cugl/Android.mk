LOCAL_PATH := $(call my-dir)
SDL2_PATH  := $(LOCAL_PATH)/../../sdl2
CUGL_PATH  := $(LOCAL_PATH)/../../../cugl
###########################
#
# Pre-Built SDL2
#
###########################

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2-prebuilt
LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,,$(SDL2_PATH)/$(TARGET_ARCH_ABI)/libSDL2.so)
LOCAL_EXPORT_C_INCLUDES := $(SDL2_PATH)/includes/SDL2
include $(PREBUILT_SHARED_LIBRARY)

###########################
#
# Pre-Built SDL2 Image
#
###########################

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_image-prebuilt
LOCAL_SRC_FILES := 	$(subst $(LOCAL_PATH)/,,$(SDL2_PATH)/$(TARGET_ARCH_ABI)/libSDL2_image.so)
LOCAL_EXPORT_C_INCLUDES := $(SDL2_PATH)/includes/SDL2
LOCAL_EXPORT_C_INCLUDES += $(SDL2_PATH)/includes/SDL2_image
include $(PREBUILT_SHARED_LIBRARY)

###########################
#
# Pre-Built SDL2 TTF
#
###########################

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_ttf-prebuilt
LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,,$(SDL2_PATH)/$(TARGET_ARCH_ABI)/libSDL2_ttf.so)
LOCAL_EXPORT_C_INCLUDES := $(SDL2_PATH)/includes/SDL2
LOCAL_EXPORT_C_INCLUDES += $(SDL2_PATH)/includes/SDL2_ttf
include $(PREBUILT_SHARED_LIBRARY)

###########################
#
# Pre-Built SDL2 Mixer
#
###########################

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_mixer-prebuilt
LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,,$(SDL2_PATH)/$(TARGET_ARCH_ABI)/libSDL2_mixer.so)
LOCAL_EXPORT_C_INCLUDES := $(SDL2_PATH)/includes/SDL2
LOCAL_EXPORT_C_INCLUDES += $(SDL2_PATH)/includes/SDL2_mixer
include $(PREBUILT_SHARED_LIBRARY)

###########################
#
# CUGL static library
#
###########################
include $(CLEAR_VARS)

LOCAL_MODULE := CUGL_static

LOCAL_MODULE_FILENAME := libCUGL

LOCAL_C_INCLUDES := $(CUGL_PATH)/include

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

