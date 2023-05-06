// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Summary Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectSummary *Summary;

void Summary_Update(void)
{
    RSDK_THIS(Summary);

    StateMachine_Run(self->state);

    ScreenInfo->position.x = 0x100 - ScreenInfo->center.x;
}

void Summary_LateUpdate(void) {}

void Summary_StaticUpdate(void)
{
    if (--Summary->bgAniDuration <= 0) {
        ++Summary->bgAniFrame;
        Summary->bgAniFrame &= 3;
        Summary->bgAniDuration = Summary->bgAniDurationTable[Summary->bgAniFrame];

        TileLayer *background = RSDK.GetTileLayer(0);
        background->scrollPos = (background->scrollPos + 0x1000000) & 0x7FF0000;
    }
}

void Summary_Draw(void)
{
    RSDK_THIS(Summary);

    StateMachine_Run(self->stateDraw);
}

void Summary_Create(void *data)
{
    RSDK_THIS(Summary);

    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_ALWAYS;
        self->visible   = true;
        self->drawGroup = 12;
        self->state     = Summary_State_SetupText;
        self->stateDraw = Summary_State_Draw;
        self->timer     = 640;
    }
}

void Summary_StageLoad(void)
{
    RSDK.ResetEntitySlot(0, Summary->classID, NULL);

    UIPicture->aniFrames = RSDK.LoadSpriteAnimation("LSelect/Icons.bin", SCOPE_STAGE);
    UIText->aniFrames    = RSDK.LoadSpriteAnimation("LSelect/Text.bin", SCOPE_STAGE);
}

void Summary_State_Draw(void)
{
    RSDK_THIS(Summary);

    RSDK.FillScreen(0x000000, self->timer, self->timer - 128, self->timer - 256);
}

void Summary_State_SetupText(void)
{
    RSDK_THIS(Summary);

    self->unusedPtr     = NULL;
    self->gameModeLabel = NULL;
    self->saveFileLabel = NULL;

    foreach_all(UIText, text)
    {
        switch (text->align) {
            default:
            case UITEXT_ALIGN_LEFT: break;

            case UITEXT_ALIGN_CENTER:
                if (self->gameModeLabel)
                    self->saveFileLabel = text;
                else
                    self->gameModeLabel = text;
                break;

            case UITEXT_ALIGN_RIGHT:
                if (text->data1 == 0xFF) {
                    self->totalTime = text;
                }
                else {
                    if (text->data0 > -1 && text->data0 < 32)
                        self->zoneLabels[text->data0] = text;
                }
                break;
        }
    }

    self->leaderCharacterID   = HUD_CharacterIndexFromID(GET_CHARACTER_ID(1)) + 1;
    self->sidekickCharacterID = HUD_CharacterIndexFromID(GET_CHARACTER_ID(2)) + 1;

    foreach_all(UIPicture, picture)
    {
        if (picture->listID == 3) {
            if (picture->frameID == 1)
                self->player2Icon = picture;
            else
                self->player1Icon = picture;
        }
    }

    self->player2Icon->animator.frameID = self->leaderCharacterID;
    self->player1Icon->animator.frameID = self->sidekickCharacterID;

    Summary_LoadTimes();
    self->state = Summary_State_FadeIn;
}

void Summary_State_FadeIn(void)
{
    RSDK_THIS(Summary);

    if (self->timer <= 0) {
        self->timer     = 0;
        self->state     = Summary_State_Wait;
        self->stateDraw = StateMachine_None;
    }
    else {
        self->timer -= 16;
    }
}

void Summary_State_Wait(void)
{
    RSDK_THIS(Summary);

    if (ControllerInfo->keyStart.press || (API_GetConfirmButtonFlip() ? ControllerInfo->keyB.press : ControllerInfo->keyA.press)) {
        RSDK.SetScene("Presentation", "Menu");
        self->timer     = 0;
        self->state     = Summary_State_FadeOut;
        self->stateDraw = Summary_State_Draw;
        Music_FadeOut(0.01);
    }
    else {
        self->timer = 0;
    }
}

void Summary_State_FadeOut(void)
{
    RSDK_THIS(Summary);

    if (self->timer >= 0x400)
        RSDK.LoadScene();
    else
        self->timer += 0x10;
}

void Summary_SetTextString(uint8 anim, EntityUIText *text, const char *str)
{
    if (!UIWidgets || UIText->aniFrames)
        RSDK.SetSpriteAnimation(UIText->aniFrames, anim, &text->animator, true, 0);
    else
        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, anim, &text->animator, true, 0);

    RSDK.SetString(&text->text, str);
    text->listID = anim;

    if (!UIWidgets || UIText->aniFrames)
        RSDK.SetSpriteString(UIText->aniFrames, text->listID, &text->text);
    else
        RSDK.SetSpriteString(UIWidgets->uiFrames, text->listID, &text->text);
}
void Summary_GetPlayTime(char *buffer, int32 time)
{
    if (!time) {
        sprintf(buffer, "--'--\"--");
    }
    else {
        int32 packedTime   = 100 * time;
        int32 hours        = 0;
        int32 minutes      = 0;
        int32 seconds      = 0;
        int32 milliseconds = 0;

        while (packedTime >= 6000) {
            ++seconds;
            packedTime -= MIN(packedTime, 6025);
        }

        if (seconds >= 60) {
            minutes = (seconds - 60) / 60 + 1;
            seconds -= 60 * minutes;
        }

        milliseconds = (packedTime / 60) & 0xFF;

        if (minutes >= 60) {
            hours = (minutes - 60) / 60 + 1;
            minutes -= 60 * hours;

            sprintf(buffer, "%d:%02d'%02d\"%02d", hours, minutes, seconds, milliseconds);
        }
        else {
            sprintf(buffer, "%02d'%02d\"%02d", minutes, seconds, milliseconds);
        }
    }
}

void Summary_LoadTimes(void)
{
    RSDK_THIS(Summary);

    if (globals->gameMode == MODE_ENCORE) {
        if (Localization->language == LANGUAGE_JP || sku_region == REGION_JP)
            Summary_SetTextString(1, self->gameModeLabel, "STORY MODE PLUS");
        else
            Summary_SetTextString(1, self->gameModeLabel, "ENCORE MODE");
    }
    else {
        if (Localization->language == LANGUAGE_JP || sku_region == REGION_JP) {
            Summary_SetTextString(1, self->gameModeLabel, "STORY MODE");
        }
        else {
            Summary_SetTextString(1, self->gameModeLabel, "MANIA MODE");
        }
    }

    if (globals->saveSlotID == NO_SAVE_SLOT) {
        Summary_SetTextString(0, self->saveFileLabel, "NO SAVE");
    }
    else {
        char text[0x20];
        sprintf_s(text, (int32)sizeof(text), "%s %d", "SAVE SLOT", globals->saveSlotID);

        Summary_SetTextString(0, self->saveFileLabel, text);
    }

    SaveRAM *saveRAM = SaveGame_GetSaveRAM();
    char playTime[0x100];
    memset(playTime, 0, 0x100);
    int32 totalTime = 0;

    for (int32 i = 0; i < 0x20; ++i) {
        if (self->zoneLabels[i]) {
            EntityUIText *text = self->zoneLabels[i];

            Summary_GetPlayTime(playTime, saveRAM->zoneTimes[text->data0]);

            int32 newTotal = saveRAM->zoneTimes[text->data0] + totalTime;
            if (newTotal < totalTime)
                newTotal = -1;

            totalTime = newTotal;
            Summary_SetTextString(0, text, playTime);
        }
    }

    Summary_GetPlayTime(playTime, totalTime);
    Summary_SetTextString(0, self->totalTime, playTime);
}

#if GAME_INCLUDE_EDITOR
void Summary_EditorDraw(void) {}

void Summary_EditorLoad(void)
{
    if (UIPicture)
        UIPicture->aniFrames = RSDK.LoadSpriteAnimation("LSelect/Icons.bin", SCOPE_STAGE);

    if (UIText)
        UIText->aniFrames = RSDK.LoadSpriteAnimation("LSelect/Text.bin", SCOPE_STAGE);
}
#endif

void Summary_Serialize(void) {}
#endif
