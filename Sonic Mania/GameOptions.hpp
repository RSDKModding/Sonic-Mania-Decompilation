#ifndef GAME_OPTIONS_H
#define GAME_OPTIONS_H

enum GameModes {
#if !RETRO_USE_PLUS
	MODE_NOSAVE,
#endif
    MODE_MANIA,
#if RETRO_USE_PLUS
    MODE_ENCORE,
#endif
    MODE_TIMEATTACK,
    MODE_COMPETITION,
};

enum CategoryIDS { MEDIA_DEMO };

enum PlayerIDs {
    MAX_PLAYERS       = 4,
    ID_SONIC          = 0x01,
    ID_TAILS          = 0x02,
    ID_KNUCKLES = 0x04,
#if RETRO_USE_PLUS
    ID_MIGHTY         = 0x08,
    ID_RAY            = 0x10,
#endif
    ID_TAILS_ASSIST   = 0x200,
    ID_DEFAULT_PLAYER = 0x201,
};

enum ItemModes { ITEMS_FIXED, ITEMS_RANDOM, ITEMS_TELEPORT };

enum MedalMods { MEDAL_DEBUGMODE, MEDAL_ANDKNUCKLES, MEDAL_PEELOUT, MEDAL_INSTASHIELD, MEDAL_DROPDASH, MEDAL_NOTIMEOVER };
#define getMod(medal) (1 << medal)

enum ScreenSplit { FORCE_SPLIT = 2 };

enum ScreenSizes { WIDE_SCR_XSIZE = 424, WIDE_SCR_XCENTER = 212 };

enum SaveSlots { NO_SAVE_SLOT = 255 };

#if RETRO_USE_PLUS
enum ModeFilters { FILTER_MANIA = 2, FILTER_ENCORE = 4 };
#endif

enum ReservedEntities {
    SLOT_PLAYER1        = 0,
    SLOT_PLAYER2        = 1,
    SLOT_PLAYER3        = 2,
    SLOT_PLAYER4        = 3,
    SLOT_MUSIC          = 9,
    SLOT_BSS_HUD        = 10,
    SLOT_BSS_MESSAGE    = 11,
    SLOT_ZONE           = 12,
    SLOT_CUTSCENESEQ    = 15,
    SLOT_PAUSEMENU      = 16,
    SLOT_DIALOG         = 21,
    SLOT_BSS_HORIZON    = 32,
    SLOT_REPLAYRECORDER = 37,
    SLOT_CAMERA1        = 60,
    SLOT_CAMERA2        = 61,
    SLOT_CAMERA3        = 62,
    SLOT_CAMERA4        = 63,
};

enum SettingsValues {
    SETTINGS_WINDOWED        = 0x0,
    SETTINGS_BORDERED        = 0x1,
    SETTINGS_EXCLUSIVEFS     = 0x2,
    SETTINGS_VSYNC           = 0x3,
    SETTINGS_TRIPLEBUFFERED  = 0x4,
    SETTINGS_WINDOW_WIDTH    = 0x5,
    SETTINGS_WINDOW_HEIGHT   = 0x6,
    SETTINGS_FSWIDTH         = 0x7,
    SETTINGS_FSHEIGHT        = 0x8,
    SETTINGS_REFRESHRATE     = 0x9,
    SETTINGS_SHADERSUPPORT   = 0xA,
    SETTINGS_SHADERID        = 0xB,
    SETTINGS_SCREENCOUNT               = 0xC,
    SETTINGS_DIMTIMER        = 0xD,
    SETTINGS_STREAMSENABLED  = 0xE,
    SETTINGS_STREAM_VOL      = 0xF,
    SETTINGS_SFX_VOL         = 0x10,
    SETTINGS_LANGUAGE        = 0x11,
    SETTINGS_12              = 0x12,
    SETTINGS_13              = 0x13,
    SETTINGS_14              = 0x14,
    SETTINGS_SETTINGSCHANGED = 0x15,
    SETTINGS_WRITE           = 0x16,
};


struct GameOptions {
	int gameMode;
	int playerID;
	int specialCleared;
	int specialRingID;
	int blueSpheresID;
	int blueSpheresInit;
	int atlEnabled;
	int atlEntityCount;
	int atlEntitySlot[32];
	int atlEntityData[0x4000];
	int saveLoaded;
	int saveRAM[0x4000];
	int saveSlotID;
	int noSaveSlot[0x400];
	int menuParam[0x4000];
	int itemMode;
	int suppressTitlecard;
	int suppressAutoMusic;
	int competitionSession[0x4000];
	int medalMods; //Stuff Like Instashieldand etc (Bonus Stuff that medals unlock)
	int parallaxOffset[256];
	int enableIntro;
	int optionsLoaded;
	int optionsRAM[128];
	int presenceID;
	int medallionDebug;
	int noSave;
	int notifiedAutosave;
	int recallEntities;
	int restartRings;
	int restart1UP;
	int restartShield;
	int restartPos[8];
	int restartSlot[4];
	int restartDir[4];
	int restartMinutes;
	int restartSeconds;
	int restartMilliseconds;
	int tempMinutes;
	int tempSeconds;
	int tempMilliseconds;
	int restartScore;
	int restartScore1UP;
	int restartLives[4];
	int restartMusicID;
	int restartFlags;
	int tempFlags;
	int continues;
	int initCoolBonus;
	int coolBonus[4];
#if RETRO_USE_PLUS
	int replayWriteBuffer[0x40000];
	int replayReadBuffer[0x40000];
	int replayTempWBuffer[0x40000];
	int replayTempRBuffer[0x40000];
	int replayTableID;
	int replayTableLoaded;
	int taTableID;
	int taTableLoaded;
	int stock; //Encore Lives
	int characterFlags;
	int vapeMode;
	int secrets;
	int superSecret;
	int superMusicEnabled;
	int lastHasPlus;
	int hasPlusInitial;
#endif
};

extern Hitbox defaultHitbox;

extern GameOptions *options;

#endif //!GAME_OPTIONS_H