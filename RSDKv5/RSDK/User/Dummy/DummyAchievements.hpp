#ifndef DUMMY_ACHIEVEMENTS_H
#define DUMMY_ACHIEVEMENTS_H

#include <vector>
#include <string>

namespace RSDK
{

namespace SKU
{

#if RETRO_REV02
// This is the "dummy" struct, it serves as the base in the event a suitable API isn't loaded (such as in this decomp)
// This struct should never be removed, other structs such as "SteamAchievements" would be added and "achievements" would be set to that instead
struct DummyAchievements : UserAchievements {
    DummyAchievements() {}

#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
    inline bool32 CheckAchievementsEnabled(void) { return true; }
    void GetAchievementNames(String *names, int32 count);
    String *GetAchievementString(String *string);
    String *GetAchievementName(String *name, uint id);
    inline bool32 Unknown8() { return true; }
    int32 GetNextAchievementID();
    void RemoveLastAchievementID();
#endif
    void TryUnlockAchievement(const char *name);

#if RETRO_VER_EGS

#endif
};
#endif

// Start Dummy Achievment code
struct AchievementInfo {
    std::string identifier;
    std::string name;
    std::string description;
    bool32 achieved;
};

extern std::vector<AchievementInfo> achievementList;
extern std::vector<int> achievementStack;

// End Dummy Achievement code

} // namespace SKU

} // namespace RSDK

#endif // DUMMY_ACHIEVEMENTS_H
