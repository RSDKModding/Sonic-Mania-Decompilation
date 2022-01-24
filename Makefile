.DEFAULT_GOAL := all

NAME		=  RSDKv5
SUFFIX		= 
PKGCONFIG	=  pkg-config
DEBUG		?= 0
STATIC		?= 1
VERBOSE		?= 0
PROFILE		?= 0
STRIP		?= strip
DEFINES     =

STATICGAME 	?= 0
GAMENAME    ?= Game

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

ifdef EMSCRIPTEN
	PLATFORM = Emscripten
endif

PLATFORM ?= Unknown

# =============================================================================

OUTDIR = bin/$(PLATFORM)
RSDK_OBJDIR = RSDKv5/obj/$(PLATFORM)
GAME_OBJDIR = Game/obj/$(PLATFORM)

include makefiles/$(PLATFORM).cfg

# =============================================================================
CFLAGS ?= $(CXXFLAGS)

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
	STATICGAME = 1
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
	CXXFLAGS += -DRETRO_STANDALONE=1
	CFLAGS += -DRETRO_STANDALONE=1
else
	CXXFLAGS += -DRETRO_STANDALONE=0
	CFLAGS += -DRETRO_STANDALONE=0
endif

CFLAGS += `$(PKGCONFIG) --cflags --static sdl2 vorbisfile vorbis theora theoradec zlib`
CXXFLAGS += `$(PKGCONFIG) --cflags --static sdl2 vorbisfile vorbis theora theoradec zlib`
LIBS += `$(PKGCONFIG) --libs --static sdl2 vorbisfile vorbis theora theoradec zlib`

CFLAGS_ALL += $(CFLAGS) \
			   -DBASE_PATH='"$(BASE_PATH)"' \
               -fsigned-char 
		
CXXFLAGS_ALL += $(CXXFLAGS) \
               -DBASE_PATH='"$(BASE_PATH)"' \
               -std=c++17 \
               -fsigned-char \
			   -fpermissive

LDFLAGS_ALL = $(LDFLAGS)
LIBS_ALL += -pthread $(LIBS)


RSDK_INCLUDES  += \
    -I./RSDKv5/ 					\
    -I./dependencies/all/ 			\
    -I./dependencies/all/tinyxml2/ 	\
	-I./dependencies/all/iniparser/ \
	-I./dependencies/all/theoraplay/ 

RSDK_INCLUDES += $(LIBS)

# Main Sources
RSDK_SOURCES = \
    RSDKv5/main 							\
    RSDKv5/RSDK/Graphics/Animation    					\
    RSDKv5/RSDK/Audio/Audio        					\
    RSDKv5/RSDK/Scene/Collision    					\
    RSDKv5/RSDK/Dev/Debug        					\
    RSDKv5/RSDK/Scene/Objects/DefaultObject        			\
    RSDKv5/RSDK/Scene/Objects/DevOutput        				\
    RSDKv5/RSDK/Graphics/Drawing      					\
    RSDKv5/RSDK/Input/Input        					\
    RSDKv5/RSDK/Core/Link        						\
    RSDKv5/RSDK/Core/Math         					\
    RSDKv5/RSDK/Core/ModAPI       					\
    RSDKv5/RSDK/Scene/Object       					\
    RSDKv5/RSDK/Graphics/Palette      					\
    RSDKv5/RSDK/Core/Reader       					\
    RSDKv5/RSDK/Core/RetroEngine  					\
    RSDKv5/RSDK/Scene/Scene        					\
    RSDKv5/RSDK/Graphics/Scene3D      					\
    RSDKv5/RSDK/Graphics/Shader       					\
    RSDKv5/RSDK/Graphics/Sprite       					\
    RSDKv5/RSDK/Storage/Storage       					\
    RSDKv5/RSDK/Storage/Text         					\
    RSDKv5/RSDK/Graphics/Video     						\
    RSDKv5/RSDK/User/Core/UserAchievements     	\
    RSDKv5/RSDK/User/Core/UserCore     			\
    RSDKv5/RSDK/User/Core/UserLeaderboards     	\
    RSDKv5/RSDK/User/Core/UserPresence     		\
    RSDKv5/RSDK/User/Core/UserStats     			\
    RSDKv5/RSDK/User/Core/UserStorage     		\
    RSDKv5/RSDK/User/Dummy/DummyAchievements     	\
    RSDKv5/RSDK/User/Dummy/DummyCore     			\
    RSDKv5/RSDK/User/Dummy/DummyLeaderboards     	\
    RSDKv5/RSDK/User/Dummy/DummyPresence     		\
    RSDKv5/RSDK/User/Dummy/DummyStats     			\
    RSDKv5/RSDK/User/Dummy/DummyStorage     		\
    dependencies/all/tinyxml2/tinyxml2 		\
	dependencies/all/theoraplay/theoraplay 	\
	dependencies/all/iniparser/iniparser 	\
	dependencies/all/iniparser/dictionary

GAME_INCLUDES = \
	-I./Game/   	\
	-I./Game/Objects/

GAME_SOURCES = \
	Game/GameObjects	\
	Game/GameVariables 

include Game/Objects.cfg


RSDKPATH = $(OUTDIR)/$(NAME)$(SUFFIX)
GAMEPATH = $(OUTDIR)/$(GAMENAME)$(SUFFIX)

PKGSUFFIX ?= $(SUFFIX)
PKGPATH = $(OUTDIR)/$(NAME)$(PKGSUFFIX)

RSDK_OBJECTS += $(addprefix $(RSDK_OBJDIR)/, $(addsuffix .o, $(RSDK_SOURCES)))
GAME_OBJECTS += $(addprefix $(GAME_OBJDIR)/, $(addsuffix .o, $(GAME_SOURCES)))


$(shell mkdir -p $(OUTDIR))
$(shell mkdir -p $(RSDK_OBJDIR))
$(shell mkdir -p $(GAME_OBJDIR))


$(GAME_OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo compiling $<...
	$(CC) -c $(CFLAGS_ALL) $(GAME_INCLUDES) $(DEFINES) $< -o $@
	@echo done $<

$(RSDK_OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo compiling $<...
	$(CC) -c $(CFLAGS_ALL) $(RSDK_INCLUDES) $(DEFINES) $< -o $@
	@echo done $<

$(RSDK_OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@echo compiling $<...
	$(CXX) -c $(CXXFLAGS_ALL) $(RSDK_INCLUDES) $(DEFINES) $< -o $@
	@echo done $<

ifeq ($(STATICGAME),1)
$(RSDKPATH): $(RSDK_OBJECTS) $(GAME_OBJECTS)
	@echo -n linking...
	$(CXX) $(CXXFLAGS_ALL) $(LDFLAGS_ALL) $(RSDK_OBJECTS) $(GAME_OBJECTS) -o $@ $(LIBS_ALL)
	@echo done
	$(STRIP) $@
else
$(RSDKPATH): $(RSDK_OBJECTS) $(GAME_OBJECTS)
	@echo -n linking RSDK...
	$(CXX) $(CXXFLAGS_ALL) $(LDFLAGS_ALL) $(RSDK_OBJECTS) -o $@ $(LIBS_ALL)
	@echo done
	@echo -n linking game...
	$(CXX) $(CXXFLAGS_ALL) $(LDFLAGS_ALL) $(GAME_OBJECTS) -o $(GAMEPATH) $(LIBS_ALL)
	@echo done
	$(STRIP) $@
	$(STRIP) $(GAMEPAHT)
endif


ifeq ($(RSDKPATH),$(PKGPATH))
all: $(RSDKPATH)
else
all: $(PKGPATH)
endif

clean:
	rm -rf $(RSDK_OBJDIR) && rm -rf $(GAME_OBJDIR) && rm -rf $(RSDKPATH)

clean-rsdk:
	rm -rf $(RSDK_OBJDIR) && rm -rf $(RSDKPATH)

clean-game:
	rm -rf $(GAME_OBJDIR) && rm -rf $(RSDKPATH)
