#if RETRO_REV02
void DummyAchievements::TryUnlockAchievement(AchievementID *id)
{
    if (enabled) {
        PrintLog(PRINT_NORMAL, "DUMMY TryUnlockAchievement(%s)", id->identifier);

        int32 i = 0;
        for (; i < (int)achievementList.size(); ++i) {
            if (achievementList[i].identifier == id->identifier) {
                if (!achievementList[i].achieved) {
                    achievementStack.push_back(i);
                    PrintLog(PRINT_NORMAL, "Unlocked Achievement: (%s, %d)", id->identifier, i);
                    achievementList[i].achieved = true;
                    SaveUserData();
                }
                break;
            }
        }

        if (i == achievementList.size())
            PrintLog(PRINT_NORMAL, "Failed to Unlock Achievement: (%s)", id->identifier);
    }
    else {
        std::string str = __FILE__;
        str += ": TryUnlockAchievement() # Tried to unlock achievement, but achievements are disabled. \r\n";
        PrintLog(PRINT_NORMAL, str.c_str());
    }
}

void DummyAchievements::SetAchievementNames(String **names, int32 count)
{
    if (count <= 0)
        return;

    char nameBuffer[0x40];
    GetCString(nameBuffer, names[0]);
    achievementText = nameBuffer;

    for (int32 i = 1; i < count && i < (int)achievementList.size(); ++i) {
        GetCString(nameBuffer, names[i]);
        achievementList[i].name = nameBuffer;
    }
}

String *DummyAchievements::GetAchievementString(String *string)
{
    InitString(string, (char *)"Achievement!", 0);
    return string;
}
String *DummyAchievements::GetAchievementName(String *name, uint32 id)
{
    id--;
    if (id <= achievementList.size())
        InitString(name, (char *)achievementList[id].name.c_str(), 0);
    return name;
}

int32 DummyAchievements::GetNextAchievementID()
{
    if (achievementStack.size() > 0)
        return achievementStack[0] + 1;
    else
        return 0;
}

void DummyAchievements::RemoveLastAchievementID()
{
    if (achievementStack.size() > 0)
        achievementStack.erase(achievementStack.begin());
}
#endif