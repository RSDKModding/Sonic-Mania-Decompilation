#include "RetroEngine.hpp"

// Start Dummy Achievements
std::vector<AchievementInfo> achievementList;
std::vector<int> achievementStack;
//End Dummy Achievements

#if RETRO_REV02
DummyAchievements *achievements = NULL;
#endif

void ClearAchievements() { PrintLog(PRINT_NORMAL, "DUMMY ClearAchievements()"); }

void TryUnlockAchievement(const char *name)
{
    if (achievements->enabled) {
        PrintLog(PRINT_NORMAL, "DUMMY TryUnlockAchievement(%s)", name);

        int i = 0;
        for (; i < (int)achievementList.size(); ++i) {
            if (achievementList[i].identifier == name) {
                if (!achievementList[i].achieved) {
                    achievementStack.push_back(i);
                    PrintLog(PRINT_NORMAL, "Unlocked Achievement: (%s, %d)", name, i);
                    achievementList[i].achieved = true;
                    saveUserData();
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

void GetAchievementNames(TextInfo *names, int count)
{
    int i = 0;
    for (; i < count && i < (int)achievementStack.size(); ++i) {
        SetText(&names[i], (char *)achievementList[i].name.c_str(), 0);
    }
    for (; i < count; ++i) {
        SetText(&names[i], (char *)"Dummy Achievement", 0);
    }
}

TextInfo *GetAchievementText(TextInfo *info)
{
    SetText(info, (char *)"Achievement!", 0);
    return info;
}
TextInfo *GetAchievementName(TextInfo *info, uint id)
{
    id--;
    if (id <= achievementList.size())
        SetText(info, (char *)achievementList[id].name.c_str(), 0);
    return info;
}

int GetNextAchievementID(void)
{
    if (achievementStack.size() > 0)
        return achievementStack[0] + 1;
    else
        return 0;
}

void RemoveLastAchievementID(void)
{
    if (achievementStack.size() > 0)
        achievementStack.erase(achievementStack.begin());
}



#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
bool32 achievementsEnabled = true;
ushort achievementAniFrames[2];
Animator achievementAnimator[2];
TextInfo achievementText[2];
int achievementTextWidth[2];
int achievementID             = 0;
int achievementsDelay         = 0;
int achievementsDrawn         = 0;
int achievementStrW           = 0;
int achievementStrX           = 0;
bool32 achievementsLoaded     = false;
bool32 achievementDrawFlag    = false;
bool32 achievementUnknownFlag = false;

void LoadAchievementAssets()
{
    if (achievementsEnabled) {
        if (achievements) {
            if (achievements->CheckAchievementsEnabled()) {
                if (achievements->Unknown8()) {
                    if (achievementUnknownFlag) {
                        achievementUnknownFlag = false;
                        achievementID          = 0;
                        achievementDrawFlag    = false;
                        achievementsDelay      = 0;
                        achievementsDrawn      = 0;
                    }
                    else {
                        if (achievementID)
                            achievementsDelay = 180;
                    }

                    achievementsLoaded = !(CheckSceneFolder("Logos") || CheckSceneFolder("Title"));

                    if (achievementsLoaded) {
                        achievementAniFrames[0] = LoadSpriteAnimation("UI/SmallFont.bin", SCOPE_STAGE);
                        SetSpriteAnimation(achievementAniFrames[0], 0, &achievementAnimator[0], true, 0);

                        achievementAniFrames[1] = LoadSpriteAnimation("UI/SmallFont.bin", SCOPE_STAGE);
                        SetSpriteAnimation(achievementAniFrames[1], 0, &achievementAnimator[1], true, 0);
                    }
                }
                else {
                    achievementsLoaded = false;
                }
            }
        }
    }
}
void ProcessAchievements()
{
    if (achievementsEnabled && achievements && achievements->CheckAchievementsEnabled()) {
        if (achievementsLoaded) {
            if (!achievementID) {
                achievementID = achievements->GetNextAchievementID();
                if (achievementID) {
                    achievementDrawFlag = true;
                    achievementsDelay   = 180;
                    achievementsDrawn   = false;
                    achievements->RemoveLastAchievementID();

                    TextInfo buffer;
                    CopyString(&achievementText[0], achievements->GetAchievementText(&buffer));
                    CopyString(&achievementText[1], achievements->GetAchievementName(&buffer, achievementID));
                    if (curSKU.language == LANGUAGE_JP) {
                        achievementTextWidth[0] = 13 * achievementText[0].length;
                        achievementTextWidth[1] = 13 * achievementText[1].length;
                    }
                    else {
                        achievementTextWidth[0] =
                            GetStringWidth(achievementAniFrames[0], 0, &achievementText[0], 0, achievementText[0].length, 0);
                        achievementTextWidth[1] =
                            GetStringWidth(achievementAniFrames[1], 0, &achievementText[1], 0, achievementText[1].length, 0);
                    }

                    achievementStrW = maxVal(achievementTextWidth[0], achievementTextWidth[1]) + 16;
                    achievementStrX = achievementTextWidth[1] > achievementTextWidth[0] ? 20 : 0;
                }
            }

            if (achievementsDrawn) {
                if (!--achievementsDelay) {
                    achievementID       = 0;
                    achievementDrawFlag = false;
                }
            }
        }
    }
}
void DrawAchievements()
{
    if (achievementsEnabled && achievements && achievements->CheckAchievementsEnabled()) {
        if (achievementsLoaded && achievementDrawFlag && achievementID) {
            Vector2 drawPos;

            TextInfo buffer;
            CopyString(&achievementText[0], achievements->GetAchievementText(&buffer));
            CopyString(&achievementText[1], achievements->GetAchievementName(&buffer, achievementID));

            int drawX = achievementStrX + currentScreen->size.x - achievementStrW;
            DrawRectangle(drawX, currentScreen->size.y - 40, achievementStrW - achievementStrX, 40, 0xFF107C, 255, INK_NONE, true);

            Vector2 vertices[3];
            vertices[0].x = (drawX - 40) << 16;
            vertices[1].y = (currentScreen->size.y - 40) << 16;
            vertices[0].y = currentScreen->size.y << 16;
            vertices[1].x = drawX << 16;
            vertices[2].x = drawX << 16;
            vertices[2].y = currentScreen->size.y << 16;
            DrawFace(vertices, 3, 255, 16, 124, 255, INK_NONE);

            drawPos.x = (drawX - achievementStrX + achievementStrW - 8) << 16;
            drawPos.y = vertices[1].y + 0xA0000;
            SetSpriteString(achievementAniFrames[0], 0, &achievementText[0]);
            DrawText(&achievementAnimator[0], &drawPos, &achievementText[0], 0, achievementText[0].length, ALIGN_CENTER, 0, 0, NULL, true);

            vertices[1].y += 0x1C0000;

            drawPos.x = (drawX - achievementStrX + achievementStrW - 8) << 16;
            drawPos.y = vertices[1].y;
            SetSpriteString(achievementAniFrames[1], 0, &achievementText[1]);
            DrawText(&achievementAnimator[1], &drawPos, &achievementText[1], 0, achievementText[1].length, ALIGN_CENTER, 0, 0, NULL, true);

            achievementsDrawn = true;
        }
    }
}

#endif