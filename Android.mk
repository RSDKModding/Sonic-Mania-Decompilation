LOCAL_PATH := $(call my-dir)
GAME_PATH := $(LOCAL_PATH)
include $(CLEAR_VARS)

# Change this or pass to ndk-build manually
GAME_NAME := SonicMania
LOCAL_MODULE := Game

LOCAL_C_INCLUDES := \
    $(GAME_PATH)/$(GAME_NAME) \
    $(GAME_PATH)/$(GAME_NAME)/Objects

LOCAL_SRC_FILES := \
    $(GAME_PATH)/$(GAME_NAME)/Game.c \
    $(GAME_PATH)/$(GAME_NAME)/Objects/All.c

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
