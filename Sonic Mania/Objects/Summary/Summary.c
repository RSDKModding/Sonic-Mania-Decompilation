#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectSummary *Summary;

void Summary_Update(void)
{
    RSDK_THIS(Summary);
    StateMachine_Run(entity->state);
    RSDK_screens->position.x = 0x100 - RSDK_screens->centerX;
}

void Summary_LateUpdate(void) {}

void Summary_StaticUpdate(void)
{
    if (--Summary->timer < 1) {
        ++Summary->aniFrameID;
        Summary->aniFrameID &= 3;
        Summary->timer = Summary->aniFrameDelay[Summary->aniFrameID];
        TileLayer *bg  = RSDK.GetSceneLayer(0);
        bg->scrollPos  = (bg->scrollPos + 0x1000000) & 0x7FF0000;
    }
}

void Summary_Draw(void)
{
    RSDK_THIS(Summary);
    StateMachine_Run(entity->stateDraw);
}

void Summary_Create(void *data)
{
    RSDK_THIS(Summary);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_ALWAYS;
        entity->visible   = true;
        entity->drawOrder = 12;
        entity->state     = Summary_State_SetupText;
        entity->stateDraw = Summary_State_Draw;
        entity->timer     = 640;
    }
}

void Summary_StageLoad(void)
{
    RSDK.ResetEntitySlot(0, Summary->objectID, 0);

    UIPicture->spriteIndex = RSDK.LoadSpriteAnimation("LSelect/Icons.bin", SCOPE_STAGE);
    UIText->spriteIndex    = RSDK.LoadSpriteAnimation("LSelect/Text.bin", SCOPE_STAGE);
}

void Summary_State_Draw(void)
{
    RSDK_THIS(Summary);
    RSDK.FillScreen(0x000000, entity->timer, entity->timer - 128, entity->timer - 256);
}

void Summary_State_SetupText(void)
{
    RSDK_THIS(Summary);
    entity->unknownPtr   = NULL;
    entity->gameModeText = NULL;
    entity->saveText     = NULL;

    foreach_all(UIText, text)
    {
        int32 align = text->align;
        if (align == ALIGN_RIGHT) {
            if (entity->gameModeText)
                entity->saveText = text;
            else
                entity->gameModeText = text;
        }
        else if (align == ALIGN_CENTER) {
            if (text->data1 == 0xFF) {
                entity->totalTime = text;
            }
            else {
                if (text->data0 > -1 && text->data0 < 32)
                    entity->textEntities[text->data0] = text;
            }
        }
    }

    int32 id = 0;
    for (int32 i = globals->playerID & 0xFF; i > 0; ++id) i >>= 1;
    entity->player1ID = id;

    id = 0;
    for (int32 i = globals->playerID >> 8; i > 0; ++id) i >>= 1;
    entity->player2ID = id;

    foreach_all(UIPicture, picture)
    {
        if (picture->listID == 3) {
            if (picture->frameID == 1)
                entity->pictureA = picture;
            else
                entity->pictureB = picture;
        }
    }

    picture               = entity->pictureA;
    picture->animator.frameID = entity->player1ID;
    picture               = entity->pictureB;
    picture->animator.frameID = entity->player2ID;

    Summary_LoadTimes();
    entity->state = Summary_State_Unknown;
}

void Summary_State_Unknown(void)
{
    RSDK_THIS(Summary);

    if (entity->timer <= 0) {
        entity->timer     = 0;
        entity->state     = Summary_State_Unknown2;
        entity->stateDraw = StateMachine_None;
    }
    else {
        entity->timer -= 16;
    }
}

void Summary_State_Unknown2(void)
{
    RSDK_THIS(Summary);
    if (RSDK_controller->keyStart.press || (API_GetConfirmButtonFlip() ? RSDK_controller->keyB.press : RSDK_controller->keyA.press)) {
        RSDK.SetScene("Presentation", "Menu");
        entity->timer     = 0;
        entity->state     = Summary_State_Unknown3;
        entity->stateDraw = Summary_State_Draw;
        Music_FadeOut(0.01);
    }
#if RETRO_USE_TOUCH_CONTROLS
    else if (RSDK_touchMouse->count) {
        RSDK.SetScene("Presentation", "Menu");
        entity->timer     = 0;
        entity->state     = Summary_State_Unknown3;
        entity->stateDraw = Summary_State_Draw;
        Music_FadeOut(0.01);
    }
#endif
    else {
        entity->timer = 0;
    }
}

void Summary_State_Unknown3(void)
{
    RSDK_THIS(Summary);
    if (entity->timer >= 1024) {
        RSDK.LoadScene();
    }
    else {
        entity->timer += 0x10;
    }
}

void Summary_SetTextString(uint8 anim, void *text, const char *str)
{
    EntityUIText *uiText = (EntityUIText *)text;
    if (!UIWidgets || UIText->spriteIndex)
        RSDK.SetSpriteAnimation(UIText->spriteIndex, anim, &uiText->animator, true, 0);
    else
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, anim, &uiText->animator, true, 0);

    RSDK.PrependText(&uiText->text, str);
    uiText->listID = anim;
    if (!UIWidgets || UIText->spriteIndex)
        RSDK.SetSpriteString(UIText->spriteIndex, uiText->listID, &uiText->text);
    else
        RSDK.SetSpriteString(UIWidgets->uiSpriteIndex, uiText->listID, &uiText->text);
}
void Summary_SetStageTime(char *buffer, int32 time)
{
    if (!time) {
        sprintf(buffer, "--'--\"--");
    }
    else {
        int32 tm = 100 * time;
        int32 mins = 0;
        int32 secs = 0;
        uint8 ms  = 0;

        if (tm >= 6000) {
            do {
                int32 t = tm;
                if (tm > 6025)
                    t = 6025;
                ++secs;
                tm -= t;
            } while (tm >= 6000);
            if (secs > 59) {
                mins = (secs - 60) / 60 + 1;
                secs -= 60 * mins;
            }
        }
        ms = tm / 60;

        if (mins >= 60)
            sprintf(buffer, "%d:%02d'%02d\"%02d", (mins - 60) / 60 + 1, mins - 60 * ((mins - 60) / 0x3Cu + 1), secs, ms);
        else
            sprintf(buffer, "%02d'%02d\"%02d", mins, secs, ms);
    }
}

void Summary_LoadTimes(void)
{
    RSDK_THIS(Summary);
    if (globals->gameMode == MODE_ENCORE) {
        if (Localization->language == LANGUAGE_JP || sku_region == REGION_JP)
            Summary_SetTextString(1, entity->gameModeText, "STORY MODE PLUS");
        else
            Summary_SetTextString(1, entity->gameModeText, "ENCORE MODE");
    }
    else if (Localization->language == LANGUAGE_JP || sku_region == REGION_JP) {
        Summary_SetTextString(1, entity->gameModeText, "STORY MODE");
    }
    else {
        Summary_SetTextString(1, entity->gameModeText, "MANIA MODE");
    }
    if (globals->saveSlotID == 255) {
        Summary_SetTextString(0, entity->saveText, "NO SAVE");
    }
    else {
        char buf[0x20];
        sprintf(buf, "%s %d", "SAVE SLOT", globals->saveSlotID);

        Summary_SetTextString(0, entity->saveText, "SAVE SLOT ");
    }

    EntitySaveGame *saveRAM = SaveGame->saveRAM;
    char textBuf[0x100];
    memset(textBuf, 0, 0x100);
    int32 totalTime = 0;

    for (int32 i = 0; i < 0x20; ++i) {
        if (entity->textEntities[i]) {
            EntityUIText *text = (EntityUIText *)entity->textEntities[i];

            Summary_SetStageTime(textBuf, saveRAM->zoneTimes[text->data0]);
            int32 time = saveRAM->zoneTimes[text->data0] + totalTime;
            if (saveRAM->zoneTimes[text->data0] + totalTime < totalTime)
                time = -1;
            totalTime = time;
            Summary_SetTextString(0, text, textBuf);
        }
    }

    Summary_SetStageTime(textBuf, totalTime);
    Summary_SetTextString(0, entity->totalTime, textBuf);
}

#if RETRO_INCLUDE_EDITOR
void Summary_EditorDraw(void) {}

void Summary_EditorLoad(void) {}
#endif

void Summary_Serialize(void) {}
#endif
