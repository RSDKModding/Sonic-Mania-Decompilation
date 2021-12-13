#ifndef USER_ACHIEVEMENTS_H
#define USER_ACHIEVEMENTS_H

#include <vector>
#include <string>

#if RETRO_REV02
// This is the base struct, it serves as the base for any API-specific stats
// This struct should never be removed
struct UserAchievements {
    UserAchievements() {}

    virtual void FrameInit(void) {}
    virtual void StageLoad(void) {}
    virtual void OnUnknownEvent(void) {}
#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
    virtual bool32 CheckAchievementsEnabled(void) { return false; }
    virtual void GetAchievementNames(TextInfo *names, int32 count) {}
    virtual TextInfo *GetAchievementText(TextInfo *info) { return NULL; }
    virtual TextInfo *GetAchievementName(TextInfo *info, uint id) { return NULL; }
    virtual bool32 Unknown8(void) { return false; }
    virtual int32 GetNextAchievementID(void) { return 0; }
    virtual void RemoveLastAchievementID(void) {}
#endif
    virtual void TryUnlockAchievement(const char *name) {}

    bool32 enabled = false;
};

// This is the "dummy" struct, it serves as the base in the event a suitable API isn't loaded (such as in this decomp)
// This struct should never be removed, other structs such as "SteamAchievements" would be added and "achievements" would be set to that instead
struct DummyAchievements : UserAchievements {
    DummyAchievements() {}

#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
    inline bool32 CheckAchievementsEnabled(void) { return true; }
    void GetAchievementNames(TextInfo *names, int32 count);
    TextInfo *GetAchievementText(TextInfo *info);
    TextInfo *GetAchievementName(TextInfo *info, uint id);
    inline bool32 Unknown8(void) { return true; }
    int32 GetNextAchievementID(void);
    void RemoveLastAchievementID(void);
#endif
    void TryUnlockAchievement(const char *name);

#if RETRO_VER_EGS

#endif
};
#endif

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

#if RETRO_REV02
extern UserAchievements *achievements;

inline bool32 GetAchievementsEnabled() { return achievements->enabled; }
inline void SetAchievementsEnabled(bool32 enabled) { achievements->enabled = enabled; }
#endif

#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
inline bool32 CheckAchievementsEnabled(void) { return achievements->CheckAchievementsEnabled(); }
inline void GetAchievementNames(TextInfo *names, int32 count) { achievements->GetAchievementNames(names, count); }
#endif

#if RETRO_REV02
inline void TryUnlockAchievement(const char *name) { achievements->TryUnlockAchievement(name); }
#else
void TryUnlockAchievement(const char *name);
#endif
void ClearAchievements();

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
