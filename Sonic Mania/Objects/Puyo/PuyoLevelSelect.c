#include "SonicMania.h"

ObjectPuyoLevelSelect *PuyoLevelSelect;

void PuyoLevelSelect_Update(void)
{
    RSDK_THIS(PuyoLevelSelect);
    if (entity->flag) {
        entity->visible = !entity->ready;

        int32 controllerID  = entity->playerID + 1;

#if RETRO_USE_TOUCH_CONTROLS
        for (int32 t = 0; t < RSDK_touchMouse->count; ++t) {
            int32 tx = (RSDK_touchMouse->x[t] * RSDK_screens->width);
            int32 ty = (RSDK_touchMouse->y[t] * RSDK_screens->height);

            if (RSDK_touchMouse->down[t]) {
                if (tx >= 0 && ty >= 96 && tx <= RSDK_screens->centerX && ty <= RSDK_screens->height) {
                    int32 tx = (RSDK_touchMouse->x[t] * RSDK_screens->width);
                    int32 ty = (RSDK_touchMouse->y[t] * RSDK_screens->height);
                    tx -= 64;
                    ty -= 192;

                    switch (((RSDK.ATan2(tx, ty) + 32) & 0xFF) >> 6) {
                        case 0:
                            break;
                        case 1:
                            RSDK_controller->keyDown.down |= true;
                            RSDK_controller[controllerID].keyDown.down = true;
                            break;
                        case 2:
                            break;
                        case 3:
                            RSDK_controller->keyUp.down |= true;
                            RSDK_controller[controllerID].keyUp.down = true;
                            break;
                    }
                    break;
                }
            }
        }
        if (!getBit(entity->touchFlags, 0)) {
            RSDK_controller->keyUp.press |= RSDK_controller->keyUp.down;
            RSDK_controller[controllerID].keyUp.press |= RSDK_controller[controllerID].keyUp.down;
        }
        if (!getBit(entity->touchFlags, 1)) {
            RSDK_controller->keyDown.press |= RSDK_controller->keyDown.down;
            RSDK_controller[controllerID].keyDown.press |= RSDK_controller[controllerID].keyDown.down;
        }
        setBit(entity->touchFlags, RSDK_controller[controllerID].keyUp.down, 0);
        setBit(entity->touchFlags, RSDK_controller[controllerID].keyDown.down, 1);

        int32 halfX = RSDK_screens->centerX / 2;
        for (int32 t = 0; t < RSDK_touchMouse->count; ++t) {
            int32 tx = (RSDK_touchMouse->x[t] * RSDK_screens->width);
            int32 ty = (RSDK_touchMouse->y[t] * RSDK_screens->height);

            if (RSDK_touchMouse->down[t]) {
                if (tx >= RSDK_screens->centerX && ty >= 96 && tx <= (RSDK_screens->width - halfX) && ty <= RSDK_screens->height) {
                    RSDK_controller->keyB.down |= true;
                    RSDK_controller[controllerID].keyB.down = true;
                    break;
                }
                else if (tx >= (RSDK_screens->centerX + halfX) && ty >= 96 && tx <= RSDK_screens->width && ty <= RSDK_screens->height) {
                    RSDK_controller->keyA.down |= true;
                    RSDK_controller[controllerID].keyA.down = true;
                    break;
                }
            }
        }

        if (!getBit(entity->touchFlags, 2)) {
            RSDK_controller->keyA.press |= RSDK_controller->keyA.down;
            RSDK_controller[controllerID].keyA.press |= RSDK_controller[controllerID].keyA.down;
        }
        if (!getBit(entity->touchFlags, 3)) {
            RSDK_controller->keyB.press |= RSDK_controller->keyB.down;
            RSDK_controller[controllerID].keyB.press |= RSDK_controller[controllerID].keyB.down;
        }
        setBit(entity->touchFlags, RSDK_controller[controllerID].keyA.down, 2);
        setBit(entity->touchFlags, RSDK_controller[controllerID].keyB.down, 3);
#endif

        entity->up      = RSDK_controller[controllerID].keyUp.press || RSDK_stickL[controllerID].keyUp.press;
        entity->down    = RSDK_controller[controllerID].keyDown.press || RSDK_stickL[controllerID].keyDown.press;

        if (API_GetConfirmButtonFlip()) {
            entity->confirmPress = RSDK_controller[controllerID].keyB.press;
            entity->backPress    = RSDK_controller[controllerID].keyA.press;
        }
        else {
            entity->confirmPress = RSDK_controller[controllerID].keyA.press;
            entity->backPress    = RSDK_controller[controllerID].keyB.press;
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
