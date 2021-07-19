#ifndef GAME_OPTIONS_H
#define GAME_OPTIONS_H

typedef enum {
#if !RETRO_USE_PLUS
    MODE_NOSAVE,
#endif
    MODE_MANIA,
#if RETRO_USE_PLUS
    MODE_ENCORE,
#endif
    MODE_TIMEATTACK,
    MODE_COMPETITION,
} GameModes;

typedef enum { MEDIA_DEMO } CategoryIDS;

typedef enum {
    ID_NONE     = 0x00,
    ID_SONIC    = 0x01,
    ID_TAILS    = 0x02,
    ID_KNUCKLES = 0x04,
#if RETRO_USE_PLUS
    ID_MIGHTY = 0x08,
    ID_RAY    = 0x10,
#endif
    ID_TAILS_ASSIST    = ID_TAILS << 8,
    ID_KNUCKLES_ASSIST = ID_KNUCKLES << 8, //custom-added, can be used to check if & knux is active
    ID_DEFAULT_PLAYER  = ID_SONIC | ID_TAILS_ASSIST,
} PlayerIDs;

typedef enum { ITEMS_FIXED, ITEMS_RANDOM, ITEMS_TELEPORT } ItemModes;

typedef enum { MEDAL_DEBUGMODE, MEDAL_ANDKNUCKLES, MEDAL_PEELOUT, MEDAL_INSTASHIELD, MEDAL_NODROPDASH, MEDAL_NOTIMEOVER } MedalMods;
#define getMod(medal) (1 << medal)

typedef enum { FORCE_SPLIT = 2 } ScreenSplit;

typedef enum { WIDE_SCR_XSIZE = 424, WIDE_SCR_XCENTER = 212 } ScreenSizes;

typedef enum { NO_SAVE_SLOT = 255 } SaveSlots;

#if RETRO_USE_PLUS
typedef enum { FILTER_MANIA = 2, FILTER_ENCORE = 4 } ModeFilters;
typedef enum { SCN_FILTER_MANIA = 3, SCN_FILTER_ENCORE = 5 } SceneFilters;
#endif

typedef enum {
    SLOT_PLAYER1     = 0,
    SLOT_PLAYER2     = 1,
    SLOT_PLAYER3     = 2,
    SLOT_PLAYER4     = 3,
    SLOT_BSS_SETUP   = 8,
    SLOT_PBL_SETUP   = 8,
    SLOT_UFO_SETUP   = 8,
    SLOT_MUSIC       = 9,
    SLOT_BSS_HUD     = 10,
    SLOT_UFO_CAMERA  = 10,
    SLOT_PBL_CAMERA  = 10,
    SLOT_BSS_MESSAGE = 11,
    SLOT_UFO_HUD     = 11,
#if RETRO_USE_PLUS
    SLOT_ZONE = 12,
#else
    SLOT_ZONE = 8,
#endif
    SLOT_CUTSCENESEQ         = 15,
    SLOT_PAUSEMENU           = 16,
    SLOT_GAMEOVER            = 16,
    SLOT_ACTCLEAR            = 16,
    SLOT_PAUSEMENU_UICONTROL = 17,
    SLOT_PAUSEMENU_BUTTON1   = 18,
    SLOT_PAUSEMENU_BUTTON2   = 19,
    SLOT_PAUSEMENU_BUTTON3   = 20,
    SLOT_DIALOG              = 21,
    SLOT_DIALOG_UICONTROL    = 22,
    SLOT_BSS_HORIZON         = 32,
    SLOT_UFO_SPEEDLINES      = 34,
    SLOT_UFO_PLASMA          = 36,
    SLOT_REPLAYRECORDER_R    = 36,
    SLOT_REPLAYRECORDER_W    = 37,
    SLOT_CAMERA1             = 60,
    SLOT_CAMERA2             = 61,
    SLOT_CAMERA3             = 62,
    SLOT_CAMERA4             = 63,
} ReservedEntities;

typedef enum {
    CS_Unknown0,
    CS_Unknown1,
    CS_Unknown2,
    CS_Unknown3,
    CS_Unknown4,
    CS_Unknown5,
    CS_Unknown6,
    CS_Unknown7,
    CS_Unknown8,
    CS_Unknown9,
    CS_Unknown10,
    CS_Unknown11,
    CS_Unknown12,
    CS_Unknown13,
    CS_Unknown14,
    CS_Unknown15,
    CS_Unknown16,
    CS_Unknown17,
    CS_Unknown18,
    CS_Unknown19,
    CS_Unknown20,
    CS_Unknown21,
    CS_inMatch,
    CS_PlayerCount,
    CS_LevelIndex,
    CS_ZoneID,
    CS_ActID,
    CS_MatchID,
    CS_MatchCount,
    CS_MonitorMode,
    CS_Unknown29,
    // flags for each zone
    CS_ZoneUnknown30, //GHZ
    CS_ZoneUnknown31, //CPZ
    CS_ZoneUnknown32, //SPZ
    CS_ZoneUnknown33, //FBZ
    CS_ZoneUnknown34, //PGZ
    CS_ZoneUnknown35, //SSZ
    CS_ZoneUnknown36, //HCZ
    CS_ZoneUnknown37, //MSZ
    CS_ZoneUnknown38, //OOZ
    CS_ZoneUnknown39, //LRZ
    CS_ZoneUnknown40, //MMZ
    CS_ZoneUnknown41, //TMZ
    CS_CharacterFlags,
    CS_WinnerFlag1,
    CS_WinnerFlag2,
    CS_WinnerFlag3,
    CS_RingsP1,
    CS_RingsP2,
    CS_RingsP3,
    CS_RingsP4,
    CS_ScoreP1,
    CS_ScoreP2,
    CS_ScoreP3,
    CS_ScoreP4,
    CS_ItemsP1,
    CS_ItemsP2,
    CS_ItemsP3,
    CS_ItemsP4,
    CS_TimeMinutesP1,
    CS_TimeSecondsP1,
    CS_TimeMillisecondsP1,
    CS_TimeMinutesP2,
    CS_TimeSecondsP2,
    CS_TimeMillisecondsP2,
    CS_TimeMinutesP3,
    CS_TimeSecondsP3,
    CS_TimeMillisecondsP3,
    CS_TimeMinutesP4,
    CS_TimeSecondsP4,
    CS_TimeMillisecondsP4,
    CS_FinishFlags,
    CS_TotalRingsP1,
    CS_TotalRingsP2,
    CS_TotalRingsP3,
    CS_TotalRingsP4,
    CS_WinsP1,
    CS_WinsP2,
    CS_WinsP3,
    CS_WinsP4,
    CS_LivesP1,
    CS_LivesP2,
    CS_LivesP3,
    CS_LivesP4,
    CS_Unknown84,
    CS_Unknown85,
    CS_Unknown86,
    CS_Unknown87,
    CS_ReadOnlyDisplayMode,
    CS_Unknown89,
    CS_Unknown90,
    CS_Unknown91,
    CS_Unknown92,
    CS_Unknown93,
    CS_Unknown94,
    CS_Unknown95,
    CS_Unknown96,
    CS_Unknown97,
    CS_Unknown98,
    CS_Unknown99,
}CompetitionSessionIDs;

typedef enum {
    PRESENCE_GENERIC,
    PRESENCE_MENU,
    PRESENCE_MANIA,
#if RETRO_USE_PLUS
    PRESENCE_ENCORE,
#endif
    PRESENCE_TA,
    PRESENCE_COMP,
    PRESENCE_TITLE,
} PresenceTypes;

typedef struct {
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
    int medalMods;
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
    int restartPowerups;
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
    int stock;
    int characterFlags;
    int vapeMode;
    int secrets;
    int superSecret;
    int superMusicEnabled;
    int lastHasPlus;
    int hasPlusInitial;
#endif
} GlobalVariables;

extern Hitbox defaultHitbox;

extern GlobalVariables *globals;

#endif //! GAME_OPTIONS_H
