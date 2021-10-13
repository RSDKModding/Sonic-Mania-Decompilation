#include "SonicMania.h"

ObjectCreditsSetup *CreditsSetup;

void CreditsSetup_Update(void) {}

void CreditsSetup_LateUpdate(void) {}

void CreditsSetup_StaticUpdate(void)
{
    if (CreditsSetup->started) {
        CreditsSetup->unknown += 0x1000;
        EntityFXFade *fade = (EntityFXFade *)CreditsSetup->fxFade;
        if (!CreditsSetup->skipFlag) {
            if (!RSDK.ChannelActive(0))
                Music_PlayTrack(++CreditsSetup->creditsTrack);

            if (RSDK_controller->keyStart.press || (CreditsSetup->creditsSize && CreditsSetup->creditsPos >= CreditsSetup->creditsSize)) {
                CreditsSetup->skipFlag = true;

                fade->state    = FXFade_State_FadeIn;
                fade->speedIn  = 8;
                fade->wait     = 64;
                fade->speedOut = 8;
                fade->timer    = 0;
                fade->oneWay   = 0;
                Music_FadeOut(0.0125);
            }
#if RETRO_USE_TOUCH_CONTROLS
            else if (RSDK_touchMouse->count) {
                CreditsSetup->skipFlag = true;

                fade->state    = FXFade_State_FadeIn;
                fade->speedIn  = 8;
                fade->wait     = 64;
                fade->speedOut = 8;
                fade->timer    = 0;
                fade->oneWay   = 0;
                Music_FadeOut(0.0125);
            }
#endif
        }
        else {
            if (fade->state == FXFade_State_Wait && fade->wait == 1) {
                EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
                if (param->field_1A8) {
                    RSDK.SetScene("Presentation", "Menu");
                }
                else if (SaveGame->saveRAM->chaosEmeralds < 0x7F) {
#if RETRO_USE_PLUS
                    if (globals->gameMode == MODE_ENCORE)
                        RSDK.SetScene("Cutscenes", "Try Again Encore");
                    else
#endif
                        RSDK.SetScene("Cutscenes", "Try Again");
                }
#if RETRO_USE_PLUS
                else if (globals->gameMode == MODE_ENCORE) {
                    RSDK.SetScene("Cutscenes", "Mirage Saloon Encore End");
                }
#endif
                else {
#if RETRO_USE_PLUS
                    if (API.CheckDLC(DLC_PLUS))
                        RSDK.SetScene("Presentation", "Game Summary");
                    else
#endif
                        RSDK.SetScene("Presentation", "Menu");
                }
                RSDK.LoadScene();
            }
        }

        int32 move = 0x10000;
        if (RSDK_controller->keyA.down || RSDK_controller->keyB.down || RSDK_controller->keyC.down)
            move = 0x80000;
        foreach_all(UICreditsText, text)
        {
            text->drawOrder = Zone->playerDrawHigh;
            text->position.y -= move;
        }
        CreditsSetup->creditsPos += move;
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
    foreach_all(FXFade, fxFade) { CreditsSetup->fxFade = (Entity *)fxFade; }
}

void CreditsSetup_LoadCreditsStrings(void)
{
    TextInfo info;
    TextInfo buffer;

    INIT_TEXTINFO(buffer);
    INIT_TEXTINFO(info);
    RSDK.LoadStrings(&buffer, "Credits.txt", 8);
    RSDK.SetText(&info, "", 0x80);

    int32 offset = (RSDK_screens->height + 128) << 16;
    for (int32 i = 0; RSDK.SplitStringList(&info, &buffer, i, 1); ++i) {
        if (info.textLength <= 4) {
            offset += 0x200000;
        }
        else {
            int32 type = info.text[1] - '0';
            bool32 hasShape = info.text[2] == 'U';

            info.textLength -= 3;
            for (int32 c = 0; c < info.textLength; ++c) {
                info.text[c] = info.text[c + 3];
            }

            EntityUICreditsText *text = RSDK_GET_ENTITY(i + 0x100, UICreditsText);
            RSDK.ResetEntityPtr(text, UICreditsText->objectID, 0);
            text->hasShape   = hasShape;
            text->position.x = 0x1000000;
            text->position.y = offset;
            UICreditsText_SetText(type, text, &info);
            SpriteFrame *frame = RSDK.GetFrame(UICreditsText->aniFrames, type, 0);
            if (frame)
                offset += (frame->height + 8) << 16;
            if (type == 7)
                offset -= 0x200000;
        }
    }

    CreditsSetup->creditsSize = offset + (RSDK_screens->height << 15);
}

void CreditsSetup_EditorDraw(void) {}

void CreditsSetup_EditorLoad(void) {}

void CreditsSetup_Serialize(void) {}
