#ifndef USERDATA_H
#define USERDATA_H

struct DummyCore {
    void (*unknown1)();
    void (*unknown2)();
    void (*unknown3)();
    void (*unknown4)();
    void (*SetupDebugValues)();
    void (*UserInitUnknown)();
    void (*unknown7)();
    int (*GetUserLanguage)();
    void (*unknown9)();
    void (*unknown10)();
    int (*GetConfirmButtonFlip)();
    void (*LaunchManual)();
    void (*ExitGame)();
    void (*unknown14)();
    void (*unknown15)();
    void (*CheckDLC)();
    void (*unknown17)();

    int values[8];
    byte debugValCnt;
    ushort field_25;
    byte field_27;
};

struct DummyAchievements {
    void (*unknown1)();
    void (*SetDebugValues)();
    void (*unknown3)();
    void (*UnlockAchievement)(const char* name);

    int active;
};

struct DummyLeaderboards {
    void (*SetDebugValues)(void);
    void (*sub_5E9D70)(void);
    void (*unknown3)(void);
    int (*unknown4)(void);
    void (*FetchLeaderboard)(int a2, int a3);
    void (*unknown5)(void);
    void (*TrackScore)(int a2, int a3, int a4);
    int (*unknown7)(void);

    byte field_4;
    byte field_5;
    byte field_6;
    byte field_7;
    byte field_8;
    byte field_9;
    byte field_A;
    byte field_B;
    byte field_C;
    byte field_D;
    byte field_E;
    byte field_F;
    byte field_10;
    ushort field_11;
    byte field_13;
    byte field_14;
    byte field_15;
    byte field_16;
    byte field_17;
    byte field_18;
    byte field_19;
    byte field_1A;
    byte field_1B;
    byte field_1C;
    byte field_1D;
    byte field_1E;
    byte field_1F;
    byte field_20[848];
};

struct DummyRichPresence {
    void (*SetDebugValues)(void);
    void (*unknown2)(void);
    void (*unknown3)(void);
    void (*SetPresence)(byte a2, TextInfo *info);

    int active;
};

struct DummyStats {
    void (*SetDebugValues)(void);
    void (*Unknown2)(void);
    void (*Unknown3)(void);
    void (*TrackStat)(void);

    int active;
};

extern DummyCore *dummmyCore;
extern DummyCore *userCore;

extern DummyAchievements *achievements;
extern DummyLeaderboards *leaderboards;
extern DummyRichPresence *richPresence;
extern DummyStats *stats;

void initUserData();
void releaseUserData();

int getUserLanguage();
int getConfirmButtonFlip();
void launchManual();
void exitGame();
inline int checkDLC(byte dlcID)
{
    if (dlcID < 8)
        return userCore->values[dlcID];
    else
        return 0;
}

void unlockAchievement(const char *name);

void fetchLeaderboard(int a2, int a3);
void trackScore(int a2, int a3, int a4);

void setPresence(byte a2, TextInfo *info);

void setupUserDebugValues();

extern bool settingsChanged;

int GetSettingsValue(int id);
void SetSettingsValue(int id, int val);

void readSettings();
void writeSettings(bool writeToFile);

#endif
