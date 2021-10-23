#include "SonicMania.h"

ObjectPuyoLevelSelect *PuyoLevelSelect;

void PuyoLevelSelect_Update(void)
{
    RSDK_THIS(PuyoLevelSelect);
    if (entity->flag) {
        entity->visible = !entity->ready;
        int controller  = entity->playerID + 1;
        entity->up      = RSDK_controller[controller].keyUp.press || RSDK_stickL[controller].keyUp.press;
        entity->down    = RSDK_controller[controller].keyDown.press || RSDK_stickL[controller].keyDown.press;

        if (API_GetConfirmButtonFlip()) {
            entity->confirmPress = RSDK_controller[controller].keyB.press;
            entity->backPress    = RSDK_controller[controller].keyA.press;
        }
        else {
            entity->confirmPress = RSDK_controller[controller].keyA.press;
            entity->backPress    = RSDK_controller[controller].keyB.press;
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
                --entity->optionID;

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

void PuyoLevelSelect_EditorDraw(void) { PuyoLevelSelect_DrawSprites(); }

void PuyoLevelSelect_EditorLoad(void) { PuyoLevelSelect->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoUI.bin", SCOPE_STAGE); }

void PuyoLevelSelect_Serialize(void) { RSDK_EDITABLE_VAR(PuyoLevelSelect, VAR_ENUM, playerID); }
