#ifndef GAME_OPTIONS_H
#define GAME_OPTIONS_H

enum GameModes {
	GAMEMODE_MANIA,
	GAMEMODE_ENCORE,
	GAMEMODE_TATTACK,
	GAMEMODE_COMPETITION,
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

extern GameOptions options;

#endif //!GAME_OPTIONS_H