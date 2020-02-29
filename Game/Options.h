#ifndef GAME_OPTIONS_H
#define GAME_OPTIONS_H

enum GameModes {
	GAMEMODE_MANIA,
	GAMEMODE_ENCORE,
	GAMEMODE_TATTACK,
	GAMEMODE_COMPETITION,
};

struct GameOptions {
	int GameMode;
	int PlayerID;
	int SpecialCleared;
	int SpecialRingID;
	int BlueSpheresID;
	int BlueSpheresInit;
	int AtlEnabled;
	int AtlEntityCount;
	int AtlEntitySlot[32];
	int AtlEntityData[0x4000];
	int SaveLoaded;
	int SaveRAM[0x4000];
	int SaveSlotID;
	int NoSaveSlot[0x400];
	int MenuParam[0x4000];
	int ItemMode;
	int SuppressTitlecard;
	int SuppressAutoMusic;
	int CompetitionSession[0x4000];
	int MedalMods; //Stuff Like Instashieldand etc (Bonus Stuff that medals unlock)
	int ParallaxOffset[256];
	int EnableIntro;
	int OptionsLoaded;
	int OptionsRAM[128];
	int PresenceID;
	int MedallionDebug;
	int NoSave;
	int NotifiedAutosave;
	int RecallEntities;
	int RestartRings;
	int Restart1UP;
	int RestartShield;
	int RestartPos[8];
	int RestartSlot[4];
	int RestartDir[4];
	int RestartMinutes;
	int RestartSeconds;
	int RestartMilliseconds;
	int TempMinutes;
	int TempSeconds;
	int TempMilliseconds;
	int RestartScore;
	int RestartScore1UP;
	int RestartLives[4];
	int RestartMusicID;
	int RestartFlags;
	int TempFlags;
	int Continues;
	int InitCoolBonus;
	int CoolBonus[4];
	int ReplayWriteBuffer[0x40000];
	int ReplayReadBuffer[0x40000];
	int ReplayTempWBuffer[0x40000];
	int ReplayTempRBuffer[0x40000];
	int ReplayTableID;
	int ReplayTableLoaded;
	int TATableID;
	int TATableLoaded;
	int Stock; //Encore Lives
	int CharacterFlags;
	int VapeMode;
	int Secrets;
	int SuperSecret;
	int SuperMusicEnabled;
	int LastHasPlus;
	int HasPlusInitial;
};

GameOptions Options;

#endif //!GAME_OPTIONS_H