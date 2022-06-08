.DEFAULT_GOAL := all

PKGCONFIG	=  pkg-config
STRIP		?= strip

STATIC		?= 1
DEBUG		?= 1
VERBOSE		?= 0
PROFILE		?= 0

RSDK_NAME    = RSDKv5
RSDK_SUFFIX  = 
USERTYPE    ?= Dummy

GAME_NAME   ?= Game
GAME_SUFFIX ?= .so
GAME_ALLC   ?= 1
STATICGAME 	?= 0

RSDK_CFLAGS  =
RSDK_LDFLAGS =
RSDK_LIBS    =

GAME_CFLAGS  =
GAME_LDFLAGS = -shared
GAME_LIBS    =

DEFINES      =


# =============================================================================
# Detect default platform if not explicitly specified
# =============================================================================

ifeq ($(OS),Windows_NT)
	PLATFORM ?= Windows
else
	UNAME_S := $(shell uname -s)

	ifeq ($(UNAME_S),Linux)
		PLATFORM ?= Linux
	endif

	ifeq ($(UNAME_S),Darwin)
		PLATFORM ?= macOS
	endif

endif

PLATFORM ?= Unknown

# =============================================================================

RSDK_SOURCES =

include makefiles/$(PLATFORM).cfg

DEFINES += -DRSDK_USE_$(SUBSYSTEM)

OUTDIR = bin/$(PLATFORM)/$(SUBSYSTEM)
RSDK_OBJDIR = bin/obj/$(PLATFORM)/$(SUBSYSTEM)/RSDKv5
GAME_OBJDIR = bin/obj/$(PLATFORM)/$(GAME_NAME)


# =============================================================================

CFLAGS ?= $(CXXFLAGS)
DEFINES += -DBASE_PATH='"$(BASE_PATH)"'

ifeq ($(DEBUG),1)
	CXXFLAGS += -g
	CFLAGS += -g
	STRIP = :
else
	CXXFLAGS += -O3
	CFLAGS += -O3
endif

ifeq ($(STATIC),1)
	CXXFLAGS += -static
	CFLAGS += -static
endif

ifeq ($(PROFILE),1)
	CXXFLAGS += -pg -g -fno-inline-functions -fno-inline-functions-called-once -fno-optimize-sibling-calls -fno-default-inline
	CFLAGS += -pg -g -fno-inline-functions -fno-inline-functions-called-once -fno-optimize-sibling-calls -fno-default-inline
endif

ifeq ($(VERBOSE),0)
	CC := @$(CC)
	CXX := @$(CXX)
endif

ifeq ($(STATICGAME),0)
	DEFINES += -DRETRO_STANDALONE=1
else
	DEFINES += -DRETRO_STANDALONE=0
endif

CFLAGS_ALL += $(CFLAGS) \
			   -fsigned-char 
		
CXXFLAGS_ALL += $(CXXFLAGS) \
			   -std=c++17 \
			   -fsigned-char \
			   -fpermissive 

LDFLAGS_ALL = $(LDFLAGS)

RSDK_INCLUDES  += \
	-I./RSDKv5/ 					\
	-I./dependencies/all/ 			\
	-I./dependencies/all/tinyxml2/ 	\
	-I./dependencies/all/iniparser/

# Main Sources
RSDK_SOURCES += \
	RSDKv5/main 							\
	RSDKv5/RSDK/Core/RetroEngine  			\
	RSDKv5/RSDK/Core/Math         			\
	RSDKv5/RSDK/Core/Reader       			\
	RSDKv5/RSDK/Core/Link        			\
	RSDKv5/RSDK/Core/ModAPI       			\
	RSDKv5/RSDK/Dev/Debug        			\
	RSDKv5/RSDK/Storage/Storage       		\
	RSDKv5/RSDK/Storage/Text         		\
	RSDKv5/RSDK/Graphics/Drawing      		\
	RSDKv5/RSDK/Graphics/Scene3D      		\
	RSDKv5/RSDK/Graphics/Animation    		\
	RSDKv5/RSDK/Graphics/Sprite       		\
	RSDKv5/RSDK/Graphics/Palette      		\
	RSDKv5/RSDK/Graphics/Video     			\
	RSDKv5/RSDK/Audio/Audio        			\
	RSDKv5/RSDK/Input/Input        			\
	RSDKv5/RSDK/Scene/Scene        			\
	RSDKv5/RSDK/Scene/Collision    			\
	RSDKv5/RSDK/Scene/Object       			\
	RSDKv5/RSDK/Scene/Objects/DefaultObject \
	RSDKv5/RSDK/Scene/Objects/DevOutput     \
	RSDKv5/RSDK/User/Core/UserAchievements  \
	RSDKv5/RSDK/User/Core/UserCore     		\
	RSDKv5/RSDK/User/Core/UserLeaderboards  \
	RSDKv5/RSDK/User/Core/UserPresence     	\
	RSDKv5/RSDK/User/Core/UserStats     	\
	RSDKv5/RSDK/User/Core/UserStorage     	\
	dependencies/all/tinyxml2/tinyxml2 		\
	dependencies/all/iniparser/iniparser 	\
	dependencies/all/iniparser/dictionary   \
	dependencies/all/miniz/miniz   


GAME_INCLUDES = \
	-I./$(GAME_NAME)/   		\
	-I./$(GAME_NAME)/Objects/

GAME_SOURCES = \
	$(GAME_NAME)/GameObjects	\
	$(GAME_NAME)/GameVariables 

ifeq ($(GAME_ALLC),1)
GAME_SOURCES += $(GAME_NAME)/Objects/All
else
# execute Game/objectmake.py?
include $(GAME_NAME)/Objects.cfg
endif

RSDK_PATH = $(OUTDIR)/$(RSDK_NAME)$(RSDK_SUFFIX)
GAME_PATH = $(OUTDIR)/$(GAME_NAME)$(GAME_SUFFIX)

PKG_NAME 	?= $(RSDK_NAME)
PKG_SUFFIX 	?= $(RSDK_SUFFIX)
PKG_PATH 	 = $(OUTDIR)/$(PKG_NAME)$(PKG_SUFFIX)

RSDK_OBJECTS += $(addprefix $(RSDK_OBJDIR)/, $(addsuffix .o, $(RSDK_SOURCES)))
GAME_OBJECTS += $(addprefix $(GAME_OBJDIR)/, $(addsuffix .o, $(GAME_SOURCES)))

$(shell mkdir -p $(OUTDIR))
$(shell mkdir -p $(RSDK_OBJDIR))
$(shell mkdir -p $(GAME_OBJDIR))

$(GAME_OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo compiling $<...
	$(CC) -c -fPIC $(CFLAGS_ALL) $(GAME_FLAGS) $(GAME_INCLUDES) $(DEFINES) $< -o $@
	@echo done $<

$(RSDK_OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo compiling $<...
	$(CC) -c $(CFLAGS_ALL) $(RSDK_CFLAGS) $(RSDK_INCLUDES) $(DEFINES) $< -o $@
	@echo done $<

$(RSDK_OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@echo compiling $<...
	$(CXX) -c $(CXXFLAGS_ALL) $(RSDK_CFLAGS) $(RSDK_INCLUDES) $(DEFINES) $< -o $@
	@echo done $<

ifeq ($(STATICGAME),1)
$(RSDK_PATH): $(RSDK_OBJECTS) $(GAME_OBJECTS)
	@echo -n linking...
	$(CXX) $(CXXFLAGS_ALL) $(LDFLAGS_ALL) $(RSDK_LDFLAGS) $(RSDK_OBJECTS) $(GAME_OBJECTS) $(RSDK_LIBS) $(GAME_LIBS) -o $@ 
	@echo done
	$(STRIP) $@
else
$(RSDK_PATH): $(RSDK_OBJECTS)
	@echo linking RSDK...
	$(CXX) $(CXXFLAGS_ALL) $(LDFLAGS_ALL) $(RSDK_LDFLAGS) $(RSDK_OBJECTS) $(RSDK_LIBS) -o $@ 
	$(STRIP) $@
	@echo done linking RSDK
$(GAME_PATH): $(GAME_OBJECTS)
	@echo linking game...
	$(CXX) $(CXXFLAGS_ALL) $(LDFLAGS_ALL) $(GAME_LDFLAGS) $(GAME_OBJECTS) $(GAME_LIBS) -o $@ 
 	$(STRIP) $@
	@echo done linking game
endif


ifeq ($(RSDK_PATH),$(PKG_PATH))

ifeq ($(STATICGAME),1)
all: $(RSDK_PATH) 
else
all: $(RSDK_PATH) $(GAME_PATH)
endif # STATICGAME

else
all: $(PKG_PATH)
endif

clean:
	rm -rf $(RSDK_OBJDIR) && rm -rf $(GAME_OBJDIR) && rm -rf $(RSDK_PATH)

clean-rsdk:
	rm -rf $(RSDK_OBJDIR) && rm -rf $(RSDK_PATH)

clean-game:
	rm -rf $(GAME_OBJDIR) && rm -rf $(RSDK_PATH)
