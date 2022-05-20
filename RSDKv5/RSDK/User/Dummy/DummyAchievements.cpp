#include "RSDK/Core/RetroEngine.hpp"

using namespace RSDK;

// Start Dummy Achievements
std::vector<RSDK::SKU::AchievementInfo> RSDK::SKU::achievementList;
std::vector<int> RSDK::SKU::achievementStack;
// End Dummy Achievements

#if RETRO_REV02
void RSDK::SKU::DummyAchievements::TryUnlockAchievement(const char *name)
{
    if (enabled) {
        PrintLog(PRINT_NORMAL, "DUMMY TryUnlockAchievement(%s)", name);

        int i = 0;
        for (; i < (int)achievementList.size(); ++i) {
            if (achievementList[i].identifier == name) {
                if (!achievementList[i].achieved) {
                    achievementStack.push_back(i);
                    PrintLog(PRINT_NORMAL, "Unlocked Achievement: (%s, %d)", name, i);
                    achievementList[i].achieved = true;
                    SaveUserData();
                }
                break;
            }
        }

        if (i == achievementList.size())
            PrintLog(PRINT_NORMAL, "Failed to Unlock Achievement: (%s)", name);
    }
    else {
        std::string str = __FILE__;
        str += ": TryUnlockAchievement() # Tried to unlock achievement, but achievements are disabled. \r\n";
        PrintLog(PRINT_NORMAL, str.c_str());
    }
}

void RSDK::SKU::DummyAchievements::GetAchievementNames(String *names, int32 count)
{
    int i = 0;
    for (; i < count && i < (int)RSDK::SKU::achievementStack.size(); ++i) {
        InitString(&names[i], (char *)RSDK::SKU::achievementList[i].name.c_str(), 0);
    }
    for (; i < count; ++i) {
        InitString(&names[i], (char *)"Dummy Achievement", 0);
    }
}

String *RSDK::SKU::DummyAchievements::GetAchievementString(String *string)
{
    InitString(string, (char *)"Achievement!", 0);
    return string;
}
String *RSDK::SKU::DummyAchievements::GetAchievementName(String *name, uint id)
{
    id--;
    if (id <= achievementList.size())
        InitString(name, (char *)achievementList[id].name.c_str(), 0);
    return name;
}

int RSDK::SKU::DummyAchievements::GetNextAchievementID()
{
    if (achievementStack.size() > 0)
        return achievementStack[0] + 1;
    else
        return 0;
}

void RSDK::SKU::DummyAchievements::RemoveLastAchievementID()
{
    if (achievementStack.size() > 0)
        achievementStack.erase(achievementStack.begin());
}
#endif