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

ifeq ($(DEBUG),1)
	CXXFLAGS += -g
	STRIP = :
else
	CXXFLAGS += -O3
endif


ifeq ($(STATIC),1)
	CXXFLAGS += -static
	STATICGAME = 1
endif

CXXFLAGS_ALL = `$(PKGCONFIG) --cflags --static sdl2 vorbisfile vorbis theora theoradec zlib`
LIBS_ALL = `$(PKGCONFIG) --libs --static sdl2 vorbisfile vorbis theora theoradec zlib`

CXXFLAGS_ALL += $(CXXFLAGS) \
               -DBASE_PATH='"$(BASE_PATH)"' \
               --std=c++17 \
               -fsigned-char \
			   -fpermissive

LDFLAGS_ALL = $(LDFLAGS)
LIBS_ALL += -pthread $(LIBS)

ifeq ($(PROFILE),1)
	CXXFLAGS_ALL += -pg -g -fno-inline-functions -fno-inline-functions-called-once -fno-optimize-sibling-calls -fno-default-inline
endif

ifeq ($(VERBOSE),0)
	CC := @$(CC)
	CXX := @$(CXX)
endif

ifeq ($(STATICGAME),0)
	CXXFLAGS_ALL += -DRETRO_STANDALONE=1
else
	CXXFLAGS_ALL += -DRETRO_STANDALONE=0
endif

RSDK_INCLUDES  += \
    -I./RSDKv5/ 					\
    -I./dependencies/all/ 			\
    -I./dependencies/all/tinyxml2/ 	\
	-I./dependencies/all/iniparser/ \
	-I./dependencies/all/theoraplay/ 

RSDK_INCLUDES += $(LIBS)

# Main Sources
RSDK_SOURCES = \
    RSDKv5/Animation    					\
    RSDKv5/Audio        					\
    RSDKv5/Collision    					\
    RSDKv5/Debug        					\
    RSDKv5/DefaultObject        			\
    RSDKv5/DevOutput        				\
    RSDKv5/Drawing      					\
    RSDKv5/Input        					\
    RSDKv5/Link        						\
    RSDKv5/Math         					\
    RSDKv5/ModAPI       					\
    RSDKv5/Object       					\
    RSDKv5/Palette      					\
    RSDKv5/Reader       					\
    RSDKv5/RetroEngine  					\
    RSDKv5/Scene        					\
    RSDKv5/Scene3D      					\
    RSDKv5/Shader       					\
    RSDKv5/Sprite       					\
    RSDKv5/Storage       					\
    RSDKv5/Text         					\
    RSDKv5/Video     						\
    RSDKv5/User/Core/UserAchievements     	\
    RSDKv5/User/Core/UserCore     			\
    RSDKv5/User/Core/UserLeaderboards     	\
    RSDKv5/User/Core/UserPresence     		\
    RSDKv5/User/Core/UserStats     			\
    RSDKv5/User/Core/UserStorage     		\
    RSDKv5/main 							\
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
	$(CXX) -c $(CXXFLAGS_ALL) -w $(GAME_INCLUDES) $(DEFINES) $< -o $@
	@echo done $<

$(RSDK_OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo compiling $<...
	$(CXX) -c $(CXXFLAGS_ALL) $(RSDK_INCLUDES) $(DEFINES) $< -o $@
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