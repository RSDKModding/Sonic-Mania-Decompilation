LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# Change this or pass to ndk-build manually
GAME_NAME := SonicMania

LOCAL_MODULE := Game

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/$(GAME_NAME) \
    $(LOCAL_PATH)/$(GAME_NAME)/Objects

WILDCARD_SETUP := \
  $(wildcard $(LOCAL_PATH)/$(GAME_NAME)/*.c) \
  $(wildcard $(LOCAL_PATH)/$(GAME_NAME)/Objects/All.c)

LOCAL_SRC_FILES := \
	$(subst jni/Game/, , $(WILDCARD_SETUP))

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
