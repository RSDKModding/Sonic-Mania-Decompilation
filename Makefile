.DEFAULT_GOAL := all

PKGCONFIG	=  pkg-config
STRIP		?= strip

STATIC		?= 0
DEBUG		?= 1
VERBOSE		?= 0
PROFILE		?= 0

GAME_NAME   ?= SonicMania
GAME_SUFFIX ?= .so
GAME_ALLC   ?= 1

GAME_CFLAGS  =
GAME_LDFLAGS = -shared
GAME_LIBS    =

GAME_PREBUILD =
GAME_PRELINK  =
GAME_POSTLINK =

DEFINES      =

RETRO_REVISION ?= 3
RSDK_REVISION  ?= $(RETRO_REVISION)

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

ifneq ("$(wildcard makefiles/$(PLATFORM).cfg)","")
	include makefiles/$(PLATFORM).cfg
endif

OUTDIR = bin/$(PLATFORM)
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

DEFINES += -DRETRO_STANDALONE=1
DEFINES += -DRETRO_REVISION=$(RSDK_REVISION)

CFLAGS_ALL += $(CFLAGS) \
			   -fsigned-char 
		
CXXFLAGS_ALL += $(CXXFLAGS) \
			   -std=c++17 \
			   -fsigned-char \
			   -fpermissive 

LDFLAGS_ALL = $(LDFLAGS)

GAME_INCLUDES = \
	-I./$(GAME_NAME)/   		\
	-I./$(GAME_NAME)/Objects/

GAME_SOURCES = \
	$(GAME_NAME)/Game

ifeq ($(GAME_ALLC),1)
GAME_SOURCES += $(GAME_NAME)/Objects/All
else
# execute Game/objectmake.py?
include $(GAME_NAME)/Objects.cfg
endif

$(shell mkdir -p $(OUTDIR))

GAME_OBJECTS += $(addprefix $(GAME_OBJDIR)/, $(addsuffix .o, $(GAME_SOURCES)))
GAME_PATH = $(OUTDIR)/$(GAME_NAME)$(GAME_SUFFIX)
$(shell mkdir -p $(GAME_OBJDIR))

$(GAME_OBJDIR)/%.o: $(GAME_PREBUILD) %.c
	@mkdir -p $(@D)
	@echo compiling $<...
	$(CC) -c -fPIC $(CFLAGS_ALL) $(GAME_FLAGS) $(GAME_INCLUDES) $(DEFINES) $< -o $@
	@echo done $<

$(GAME_PATH): $(GAME_PRELINK) $(GAME_OBJECTS)
	@echo linking game...
	$(CXX) $(CXXFLAGS_ALL) $(LDFLAGS_ALL) $(GAME_LDFLAGS) $(GAME_OBJECTS) $(GAME_LIBS) -o $@ 
 	$(STRIP) $@
	@echo done linking game

all: $(GAME_POSTLINK) $(GAME_PATH)

clean:
	rm -rf $(GAME_OBJDIR) && rm -rf $(GAME_PATH)
