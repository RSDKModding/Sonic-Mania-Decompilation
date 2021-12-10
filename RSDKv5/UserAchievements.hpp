#ifndef USER_ACHIEVEMENTS_H
#define USER_ACHIEVEMENTS_H

#include <vector>

struct DummyAchievements {
    void (*InitUnknown1)(void);
    void (*SetDebugValues)(void);
    void (*InitUnknown2)(void);
#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
    bool32 (*CheckAchievementsEnabled)(void);
    void (*GetAchievementNames)(TextInfo *names, int count);
    TextInfo *(*GetAchievementText)(TextInfo *info);
    TextInfo *(*GetAchievementName)(TextInfo *info, uint id);
    bool32 (*Unknown8)(void);
    int (*GetNextAchievementID)(void);
    void (*RemoveLastAchievementID)(void);
#endif
    void (*UnlockAchievement)(const char *name);

    int status;
#if RETRO_VER_EGS

#endif
};

//Start Dummy Achievment code
struct AchievementInfo {
    std::string identifier;
    std::string name;
    std::string description;
    bool32 achieved;
};

extern std::vector<AchievementInfo> achievementList;
extern std::vector<int> achievementStack;

//End Dummy Achievement code

extern DummyAchievements *achievements;

#if RETRO_REV02
inline int GetAchievementsStatus() { return achievements->status; }
inline void SetAchievementsStatus(int status) { achievements->status = status; }
#endif
inline bool32 CheckAchievementsEnabled() { return true; }
inline bool32 AchivementsUnknown8() { return true; }
void GetAchievementNames(TextInfo *names, int count);
TextInfo *GetAchievementText(TextInfo *info);
TextInfo *GetAchievementName(TextInfo *info, uint id);
int GetNextAchievementID(void);
void RemoveLastAchievementID(void);
void ClearAchievements();
void TryUnlockAchievement(const char *name);

#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
extern bool32 achievementsEnabled;
extern ushort achievementAniFrames[2];
extern Animator achievementAnimator[2];
extern TextInfo achievementText[2];
extern int achievementTextWidth[2];
extern int achievementID;
extern int achievementsDelay;
extern int achievementsDrawn;
extern int achievementStrW;
extern int achievementStrX;
extern bool32 achievementsLoaded;
extern bool32 achievementDrawFlag;
extern bool32 achievementUnknownFlag;

void LoadAchievementAssets();
void ProcessAchievements();
void DrawAchievements();
#endif

#endif // USER_ACHIEVEMENTS_H
