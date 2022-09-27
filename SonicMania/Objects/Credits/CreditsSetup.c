// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CreditsSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCreditsSetup *CreditsSetup;

void CreditsSetup_Update(void) {}

void CreditsSetup_LateUpdate(void) {}

void CreditsSetup_StaticUpdate(void)
{
    if (CreditsSetup->started) {
        CreditsSetup->scrollPos += 0x1000;

        EntityFXFade *fade = CreditsSetup->fxFade;

        if (!CreditsSetup->skipped) {
            // Check if we should play the next track
            if (!RSDK.ChannelActive(0))
                Music_PlayTrack(++CreditsSetup->creditsTrack);

            if (ControllerInfo->keyStart.press || (CreditsSetup->creditsSize && CreditsSetup->creditsPos >= CreditsSetup->creditsSize)) {
                CreditsSetup->skipped = true;

                fade->state    = FXFade_State_FadeOut;
                fade->speedIn  = 8;
                fade->wait     = 64;
                fade->speedOut = 8;
                fade->timer    = 0;
                fade->oneWay   = false;
                Music_FadeOut(0.0125);
            }
        }
        else {
            if (fade->state == FXFade_State_Wait && fade->wait == 1) {
                EntityMenuParam *param = MenuParam_GetParam();

#if MANIA_USE_PLUS
                if (param->creditsReturnToMenu) {
                    RSDK.SetScene("Presentation", "Menu");
                }
                else if (SaveGame_GetSaveRAM()->collectedEmeralds < 0b01111111) {
                    globals->gameMode == MODE_ENCORE ? RSDK.SetScene("Cutscenes", "Try Again Encore") : RSDK.SetScene("Cutscenes", "Try Again");
                }
                else if (globals->gameMode == MODE_ENCORE) {
                    RSDK.SetScene("Cutscenes", "Mirage Saloon Encore End");
                }
                else {
                    API.CheckDLC(DLC_PLUS) ? RSDK.SetScene("Presentation", "Game Summary") : RSDK.SetScene("Presentation", "Menu");
                }
#else
                if (param->creditsReturnToMenu)
                    RSDK.SetScene("Presentation", "Menu");
                else if (SaveGame_GetSaveRAM()->collectedEmeralds < 0b01111111)
                    RSDK.SetScene("Cutscenes", "Try Again");
                else
                    RSDK.SetScene("Presentation", "Menu");
#endif

                RSDK.LoadScene();
            }
        }

        int32 scrollSpeed = 0x10000;
        if (ControllerInfo->keyA.down || ControllerInfo->keyB.down || ControllerInfo->keyC.down)
            scrollSpeed = 0x80000;

        foreach_all(UICreditsText, text)
        {
            text->drawGroup = Zone->playerDrawGroup[1];
            text->position.y -= scrollSpeed;
        }

        CreditsSetup->creditsPos += scrollSpeed;
    }
    else {
        foreach_all(Player, player) { destroyEntity(player); }
        foreach_all(Camera, camera) { destroyEntity(camera); }

        CreditsSetup->creditsSize = 0;
        CreditsSetup->creditsPos  = 0;
        CreditsSetup_LoadCreditsStrings();
        CreditsSetup->started = true;
    }
}

void CreditsSetup_Draw(void) {}

void CreditsSetup_Create(void *data) {}

void CreditsSetup_StageLoad(void)
{
    CreditsSetup->started = false;

    foreach_all(FXFade, fxFade) { CreditsSetup->fxFade = fxFade; }
}

void CreditsSetup_LoadCreditsStrings(void)
{
    String string;
    String stringList;

    INIT_STRING(stringList);
    INIT_STRING(string);
    RSDK.LoadStringList(&stringList, "Credits.txt", 8);
    RSDK.InitString(&string, "", 0x80);

    int32 offset = (ScreenInfo->size.y + 128) << 16;
    for (int32 i = 0; RSDK.SplitStringList(&string, &stringList, i, 1); ++i) {
        if (string.length <= 4) {
            offset += 0x200000;
        }
        else {
            int32 type      = string.chars[1] - '0';
            bool32 hasShape = string.chars[2] == 'U';

            string.length -= 3;
            for (int32 c = 0; c < string.length; ++c) string.chars[c] = string.chars[c + 3];

            EntityUICreditsText *text = RSDK_GET_ENTITY(i + 0x100, UICreditsText);
            RSDK.ResetEntity(text, UICreditsText->classID, 0);
            text->hasShape   = hasShape;
            text->position.x = 0x1000000;
            text->position.y = offset;
            UICreditsText_SetText(type, text, &string);

            SpriteFrame *frame = RSDK.GetFrame(UICreditsText->aniFrames, type, 0);
            if (frame)
                offset += (frame->height + 8) << 16;
            if (type == 7)
                offset -= 0x200000;
        }
    }

    CreditsSetup->creditsSize = offset + (ScreenInfo->size.y << 15);
}

#if RETRO_INCLUDE_EDITOR
void CreditsSetup_EditorDraw(void) {}

void CreditsSetup_EditorLoad(void) {}
#endif

void CreditsSetup_Serialize(void) {}
