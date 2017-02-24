LOCAL_PATH := $(call my-dir)
SDL2_PATH  := $(LOCAL_PATH)/../../sdl2
CUGL_PATH  := $(LOCAL_PATH)/../../../../../../
APPL_PATH  := $(LOCAL_PATH)/../../../source

###########################
#
# Main library
#
###########################
include $(CLEAR_VARS)
LOCAL_MODULE := main

LOCAL_C_INCLUDES := $(CUGL_PATH)/include

# Add your application source files here...
LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
	$(SDL2_PATH)/main/SDL_android_main.c \
	$(wildcard $(APPL_PATH)/*.cpp) \
	)

###########################
#
# Link the Libraries
#
###########################
LOCAL_SHARED_LIBRARIES := SDL2-prebuilt
LOCAL_SHARED_LIBRARIES += SDL2_image-prebuilt
LOCAL_SHARED_LIBRARIES += SDL2_ttf-prebuilt
LOCAL_SHARED_LIBRARIES += SDL2_mixer-prebuilt

LOCAL_STATIC_LIBRARIES := CUGL_static

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lGLESv3 -llog -latomic
include $(BUILD_SHARED_LIBRARY)
