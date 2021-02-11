#ifndef GAME_OPTIONS_H
#define GAME_OPTIONS_H

enum GameModes {
    MODE_MANIA,
    MODE_ENCORE,
    MODE_TIMEATTACK,
    MODE_COMPETITION,
};

enum CategoryIDS { MEDIA_DEMO };

enum PlayerIDs {
    MAX_PLAYERS       = 4,
    ID_SONIC          = 0x01,
    ID_TAILS          = 0x02,
    ID_KNUCKLES       = 0x04,
    ID_MIGHTY         = 0x08,
    ID_RAY            = 0x10,
    ID_TAILS_ASSIST   = 0x200,
    ID_DEFAULT_PLAYER = 0x201,
};

enum ItemModes { ITEMS_FIXED, ITEMS_RANDOM, ITEMS_TELEPORT };

enum MedalMods { MEDAL_DEBUGMODE, MEDAL_ANDKNUCKLES, MEDAL_PEELOUT, MEDAL_INSTASHIELD, MEDAL_DROPDASH };

enum ScreenSplit { FORCE_SPLIT = 2 };

enum ScreenSizes { WIDE_SCR_XSIZE = 424, WIDE_SCR_XCENTER = 212 };

enum SaveSlots { NO_SAVE_SLOT = 255 };

enum ModeFilters { FILTER_MANIA = 2, FILTER_ENCORE = 4 };

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
};

extern Hitbox defaultHitbox;

extern GameOptions *options;

#endif //!GAME_OPTIONS_H