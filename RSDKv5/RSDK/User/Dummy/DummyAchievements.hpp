#include <vector>
#include <string>

#if RETRO_REV02
// This is the "dummy" struct, it serves as the base in the event a suitable API isn't loaded (such as in this decomp)
// This struct should never be removed, other structs such as "SteamAchievements" would be added and "achievements" would be set to that instead
struct DummyAchievements : UserAchievements {
    DummyAchievements() {}

#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
    inline bool32 CheckAchievementsEnabled() { return true; }
    void SetAchievementNames(String **names, int32 count);
    String *GetAchievementString(String *string);
    String *GetAchievementName(String *name, uint32 id);
    inline bool32 CheckAchievementPopupEnabled() { return true; }
    int32 GetNextAchievementID();
    void RemoveLastAchievementID();
#endif
    void TryUnlockAchievement(AchievementID *id);

#if RETRO_VER_EGS

#endif
};
#endif
