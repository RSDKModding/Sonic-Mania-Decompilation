LOCAL_PATH := $(call my-dir)

OGG_DIR := dependencies/android/libogg
THEORA_DIR := dependencies/android/libtheora

OGG_INCLUDES    := $(LOCAL_PATH)/$(OGG_DIR)/include
THEORA_INCLUDES := $(LOCAL_PATH)/$(THEORA_DIR)/include \
	                 $(LOCAL_PATH)/$(THEORA_DIR)/lib

######################################################################
# OGG
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
LOCAL_MODULE   := libogg
LOCAL_CFLAGS   := -ffast-math -fsigned-char -O2 -fPIC -DPIC \
                  -DBYTE_ORDER=LITTLE_ENDIAN -D_ARM_ASSEM_

LOCAL_C_INCLUDES := $(OGG_INCLUDES)

WILDCARD_SETUP := \
  $(wildcard $(LOCAL_PATH)/$(OGG_DIR)/src/*.c)


LOCAL_SRC_FILES := \
	$(subst jni/src/, , $(WILDCARD_SETUP))

include $(BUILD_STATIC_LIBRARY)

######################################################################
# THEORA
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
LOCAL_MODULE   := libtheora
LOCAL_CFLAGS   := -ffast-math -fsigned-char -O2 -fPIC -DPIC \
                  -DBYTE_ORDER=LITTLE_ENDIAN -D_ARM_ASSEM_

LOCAL_C_INCLUDES := $(OGG_INCLUDES) $(THEORA_INCLUDES)

WILDCARD_SETUP := \
  $(wildcard $(LOCAL_PATH)/$(THEORA_DIR)/lib/*.c)

LOCAL_SRC_FILES := \
	$(subst jni/src/, , $(WILDCARD_SETUP))

include $(BUILD_STATIC_LIBRARY)

#######################################
#GAME
include $(CLEAR_VARS)

LOCAL_MODULE := Game

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/Game \
    $(LOCAL_PATH)/Game/Objects

WILDCARD_SETUP := \
  $(wildcard $(LOCAL_PATH)/Game/*.c) \
  $(wildcard $(LOCAL_PATH)/Game/Objects/All.c)

LOCAL_SRC_FILES := \
	$(subst jni/, , $(WILDCARD_SETUP))

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

######################################################################
#RSDK

include $(CLEAR_VARS)

LOCAL_MODULE := RSDK

LOCAL_CFLAGS := -fexceptions -frtti -DRSDK_USE_GL3

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/RSDKv5/ 					                \
	$(LOCAL_PATH)/dependencies/android/libogg/include/ 	    \
	$(LOCAL_PATH)/dependencies/android/libtheora/include/ 	\
	$(LOCAL_PATH)/dependencies/android/                  	\
	$(LOCAL_PATH)/dependencies/all/ 			            \
	$(LOCAL_PATH)/dependencies/all/tinyxml2/ 	            \
	$(LOCAL_PATH)/dependencies/all/iniparser/

LOCAL_SRC_FILES += \
	RSDKv5/main.cpp 							\
	RSDKv5/RSDK/Core/RetroEngine.cpp  			\
	RSDKv5/RSDK/Core/Math.cpp         			\
	RSDKv5/RSDK/Core/Reader.cpp       			\
	RSDKv5/RSDK/Core/Link.cpp        			\
	RSDKv5/RSDK/Core/ModAPI.cpp       			\
	RSDKv5/RSDK/Dev/Debug.cpp        			\
	RSDKv5/RSDK/Storage/Storage.cpp       		\
	RSDKv5/RSDK/Storage/Text.cpp         		\
	RSDKv5/RSDK/Graphics/Drawing.cpp      		\
	RSDKv5/RSDK/Graphics/Scene3D.cpp      		\
	RSDKv5/RSDK/Graphics/Animation.cpp    		\
	RSDKv5/RSDK/Graphics/Sprite.cpp       		\
	RSDKv5/RSDK/Graphics/Palette.cpp      		\
	RSDKv5/RSDK/Graphics/Video.cpp     			\
	RSDKv5/RSDK/Audio/Audio.cpp        			\
	RSDKv5/RSDK/Input/Input.cpp        			\
	RSDKv5/RSDK/Scene/Scene.cpp        			\
	RSDKv5/RSDK/Scene/Collision.cpp    			\
	RSDKv5/RSDK/Scene/Object.cpp       			\
	RSDKv5/RSDK/Scene/Objects/DefaultObject.cpp \
	RSDKv5/RSDK/Scene/Objects/DevOutput.cpp     \
	RSDKv5/RSDK/User/Core/UserAchievements.cpp  \
	RSDKv5/RSDK/User/Core/UserCore.cpp     		\
	RSDKv5/RSDK/User/Core/UserLeaderboards.cpp  \
	RSDKv5/RSDK/User/Core/UserPresence.cpp     	\
	RSDKv5/RSDK/User/Core/UserStats.cpp     	\
	RSDKv5/RSDK/User/Core/UserStorage.cpp     	\
	dependencies/all/tinyxml2/tinyxml2.cpp 		\
	dependencies/all/iniparser/iniparser.cpp 	\
	dependencies/all/iniparser/dictionary.cpp   \
	dependencies/all/miniz/miniz.c              \
	dependencies/android/androidHelpers.cpp


LOCAL_SHARED_LIBRARIES := libogg libtheora Game
LOCAL_LDLIBS := -lGLESv3 -lEGL -llog -lz -landroid -ljnigraphics
LOCAL_SRC_FILES := $(subst jni/, , $(LOCAL_SRC_FILES))

include $(BUILD_SHARED_LIBRARY)

######################################################################
