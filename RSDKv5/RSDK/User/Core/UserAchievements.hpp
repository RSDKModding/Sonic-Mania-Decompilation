#ifndef USER_ACHIEVEMENTS_H
#define USER_ACHIEVEMENTS_H

#include <vector>
#include <string>

namespace RSDK
{

namespace SKU
{

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
    virtual void GetAchievementNames(String *names, int32 count) {}
    virtual String *GetAchievementString(String *string) { return NULL; }
    virtual String *GetAchievementName(String *name, uint id) { return NULL; }
    virtual bool32 Unknown8(void) { return false; }
    virtual int32 GetNextAchievementID(void) { return 0; }
    virtual void RemoveLastAchievementID(void) {}
#endif
    virtual void TryUnlockAchievement(const char *name) {}

    bool32 enabled = false;
};

#endif

#if RETRO_REV02
extern UserAchievements *achievements;

inline bool32 GetAchievementsEnabled() { return achievements->enabled; }
inline void SetAchievementsEnabled(bool32 enabled) { achievements->enabled = enabled; }
#endif

#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
inline bool32 CheckAchievementsEnabled(void) { return achievements->CheckAchievementsEnabled(); }
inline void GetAchievementNames(String *names, int32 count) { achievements->GetAchievementNames(names, count); }
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
extern String achievementStrings[2];
extern int achievementStringWidth[2];
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

} // namespace SKU

} // namespace RSDK

#endif // USER_ACHIEVEMENTS_H
