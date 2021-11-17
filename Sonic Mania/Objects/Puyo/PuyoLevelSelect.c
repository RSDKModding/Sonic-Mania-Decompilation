#include "SonicMania.h"

ObjectPuyoLevelSelect *PuyoLevelSelect;

void PuyoLevelSelect_Update(void)
{
    RSDK_THIS(PuyoLevelSelect);
    if (entity->flag) {
        entity->visible = !entity->ready;

        int32 controllerID  = entity->playerID + 1;

#if RETRO_USE_TOUCH_CONTROLS
        for (int32 t = 0; t < TouchInfo->count; ++t) {
            int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
            int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

            if (TouchInfo->down[t]) {
                if (tx >= 0 && ty >= 96 && tx <= ScreenInfo->centerX && ty <= ScreenInfo->height) {
                    int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
                    int32 ty = (TouchInfo->y[t] * ScreenInfo->height);
                    tx -= 64;
                    ty -= 192;

                    switch (((RSDK.ATan2(tx, ty) + 32) & 0xFF) >> 6) {
                        case 0:
                            break;
                        case 1:
                            ControllerInfo->keyDown.down |= true;
                            ControllerInfo[controllerID].keyDown.down = true;
                            break;
                        case 2:
                            break;
                        case 3:
                            ControllerInfo->keyUp.down |= true;
                            ControllerInfo[controllerID].keyUp.down = true;
                            break;
                    }
                    break;
                }
            }
        }
        if (!getBit(entity->touchFlags, 0)) {
            ControllerInfo->keyUp.press |= ControllerInfo->keyUp.down;
            ControllerInfo[controllerID].keyUp.press |= ControllerInfo[controllerID].keyUp.down;
        }
        if (!getBit(entity->touchFlags, 1)) {
            ControllerInfo->keyDown.press |= ControllerInfo->keyDown.down;
            ControllerInfo[controllerID].keyDown.press |= ControllerInfo[controllerID].keyDown.down;
        }
        setBit(entity->touchFlags, ControllerInfo[controllerID].keyUp.down, 0);
        setBit(entity->touchFlags, ControllerInfo[controllerID].keyDown.down, 1);

        int32 halfX = ScreenInfo->centerX / 2;
        for (int32 t = 0; t < TouchInfo->count; ++t) {
            int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
            int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

            if (TouchInfo->down[t]) {
                if (tx >= ScreenInfo->centerX && ty >= 96 && tx <= (ScreenInfo->width - halfX) && ty <= ScreenInfo->height) {
                    ControllerInfo->keyB.down |= true;
                    ControllerInfo[controllerID].keyB.down = true;
                    break;
                }
                else if (tx >= (ScreenInfo->centerX + halfX) && ty >= 96 && tx <= ScreenInfo->width && ty <= ScreenInfo->height) {
                    ControllerInfo->keyA.down |= true;
                    ControllerInfo[controllerID].keyA.down = true;
                    break;
                }
            }
        }

        if (!getBit(entity->touchFlags, 2)) {
            ControllerInfo->keyA.press |= ControllerInfo->keyA.down;
            ControllerInfo[controllerID].keyA.press |= ControllerInfo[controllerID].keyA.down;
        }
        if (!getBit(entity->touchFlags, 3)) {
            ControllerInfo->keyB.press |= ControllerInfo->keyB.down;
            ControllerInfo[controllerID].keyB.press |= ControllerInfo[controllerID].keyB.down;
        }
        setBit(entity->touchFlags, ControllerInfo[controllerID].keyA.down, 2);
        setBit(entity->touchFlags, ControllerInfo[controllerID].keyB.down, 3);
#endif

        entity->up      = ControllerInfo[controllerID].keyUp.press || AnalogStickInfoL[controllerID].keyUp.press;
        entity->down    = ControllerInfo[controllerID].keyDown.press || AnalogStickInfoL[controllerID].keyDown.press;

        if (API_GetConfirmButtonFlip()) {
            entity->confirmPress = ControllerInfo[controllerID].keyB.press;
            entity->backPress    = ControllerInfo[controllerID].keyA.press;
        }
        else {
            entity->confirmPress = ControllerInfo[controllerID].keyA.press;
            entity->backPress    = ControllerInfo[controllerID].keyB.press;
        }
        PuyoLevelSelect_HandleMenuMovement();
    }
    else {
        entity->visible = false;
    }
}

void PuyoLevelSelect_LateUpdate(void) {}

void PuyoLevelSelect_StaticUpdate(void) {}

void PuyoLevelSelect_Draw(void) { PuyoLevelSelect_DrawSprites(); }

void PuyoLevelSelect_Create(void *data)
{
    RSDK_THIS(PuyoLevelSelect);
    entity->active        = ACTIVE_NORMAL;
    entity->drawOrder     = 10;
    entity->visible       = true;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
}

void PuyoLevelSelect_StageLoad(void)
{
    PuyoLevelSelect->aniFrames     = RSDK.LoadSpriteAnimation("Puyo/PuyoUI.bin", SCOPE_STAGE);
    PuyoLevelSelect->sfxMenuBleep  = RSDK.GetSFX("Global/MenuBleep.wav");
    PuyoLevelSelect->sfxMenuAccept = RSDK.GetSFX("Global/MenuAccept.wav");
}

void PuyoLevelSelect_DrawSprites(void)
{
    RSDK_THIS(PuyoLevelSelect);
    Vector2 drawPos;

    drawPos.x         = entity->position.x + 0x30000;
    drawPos.y         = entity->position.y + 0x30000;
    entity->inkEffect = INK_BLEND;
    entity->alpha     = 0x7F;
    RSDK.SetSpriteAnimation(PuyoLevelSelect->aniFrames, 0, &entity->animator1, true, 1);
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.x -= 0x30000;
    drawPos.y -= 0x30000;
    entity->inkEffect = INK_NONE;
    RSDK.SetSpriteAnimation(PuyoLevelSelect->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y - 0x500000;
    RSDK.SetSpriteAnimation(PuyoLevelSelect->aniFrames, 1, &entity->animator2, true, 1);
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y - 0x310000;

    for (int i = 0; i < 5; ++i) {
        if (i != entity->optionID || (!(Zone->timer & 4))) {
            RSDK.SetSpriteAnimation(PuyoLevelSelect->aniFrames, (i != entity->optionID) + 4, &entity->animator2, true, i + 1);
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        }
        drawPos.y += 0x180000;
    }
}

void PuyoLevelSelect_HandleMenuMovement(void)
{
    RSDK_THIS(PuyoLevelSelect);

    if (entity->ready) {
        if (entity->backPress)
            entity->ready = false;
    }
    else {
        if (entity->up || entity->down) {
            if (entity->up)
                --entity->optionID;
            if (entity->down)
                ++entity->optionID;

            if (entity->optionID >= 5)
                entity->optionID -= 5;
            if (entity->optionID < 0)
                entity->optionID += 5;

            RSDK.PlaySfx(PuyoLevelSelect->sfxMenuBleep, false, 255);
        }
        else if (entity->confirmPress) {
            entity->ready = true;
            RSDK.PlaySfx(PuyoLevelSelect->sfxMenuAccept, false, 255);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void PuyoLevelSelect_EditorDraw(void) { PuyoLevelSelect_DrawSprites(); }

void PuyoLevelSelect_EditorLoad(void) { PuyoLevelSelect->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoUI.bin", SCOPE_STAGE); }
#endif

void PuyoLevelSelect_Serialize(void) { RSDK_EDITABLE_VAR(PuyoLevelSelect, VAR_ENUM, playerID); }
