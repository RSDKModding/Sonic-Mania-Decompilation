#include "SonicMania.h"

ObjectCollapsingPlatform *CollapsingPlatform;

void CollapsingPlatform_Update(void)
{
    RSDK_THIS(CollapsingPlatform);
    self->visible = false;
    if (DebugMode)
        self->visible = DebugMode->debugActive;

    if (self->collapseDelay) {
#if RETRO_USE_PLUS
        if (Player) {
            foreach_active(Player, player)
            {
                if (Player_CheckCollisionTouch(player, self, &self->hitbox) && player->characterID == ID_MIGHTY && player->jumpAbilityTimer > 1) {
                    StateMachine_Run(self->state);
                    RSDK.PlaySfx(CollapsingPlatform->sfxCrumble, 0, 255);
                    if (self->respawn) {
                        self->collapseDelay = 0;
                        self->playerPos.x   = 0;
                    }
                    else {
                        destroyEntity(self);
                    }
                    foreach_return;
                }
            }
        }
#endif
        if (--self->collapseDelay == 0) {
            StateMachine_Run(self->state);
            RSDK.PlaySfx(CollapsingPlatform->sfxCrumble, 0, 255);
            if (self->respawn) {
                self->collapseDelay = 0;
                self->playerPos.x   = 0;
            }
            else {
                destroyEntity(self);
            }
        }
    }
    else if (Player) {
        self->direction = FLIP_NONE;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)
#if RETRO_USE_PLUS
                && (!self->mightyOnly || (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop))
#endif
                && !player->sidekick && player->onGround && !player->collisionMode && !self->eventOnly && self->delay < 0xFFFF) {
                self->playerPos.x = player->position.x;
#if RETRO_USE_PLUS
                if (player->characterID == ID_MIGHTY && player->jumpAbilityTimer > 1) {
                    StateMachine_Run(self->state);
                    RSDK.PlaySfx(CollapsingPlatform->sfxCrumble, 0, 255);
                    if (self->respawn) {
                        self->collapseDelay = 0;
                        self->playerPos.x   = 0;
                    }
                    else {
                        destroyEntity(self);
                    }
                    foreach_break;
                }
#endif
            }
        }

        if (self->playerPos.x) {
            self->collapseDelay = self->delay;
            if (!self->delay) {
                StateMachine_Run(self->state);
                RSDK.PlaySfx(CollapsingPlatform->sfxCrumble, 0, 255);
                if (self->respawn) {
                    self->collapseDelay = 0;
                    self->playerPos.x   = 0;
                }
                else {
                    destroyEntity(self);
                }
            }
        }
    }
    else if (self->playerPos.x) {
        self->collapseDelay = self->delay;
        if (!self->delay) {
            StateMachine_Run(self->state);
            RSDK.PlaySfx(CollapsingPlatform->sfxCrumble, 0, 255);
            if (self->respawn) {
                self->collapseDelay = 0;
                self->playerPos.x   = 0;
            }
            else {
                destroyEntity(self);
            }
        }
    }
}

void CollapsingPlatform_LateUpdate(void) {}

void CollapsingPlatform_StaticUpdate(void) {}

void CollapsingPlatform_Draw(void)
{
    RSDK_THIS(CollapsingPlatform);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= self->size.x >> 1;
    drawPos.y -= self->size.y >> 1;
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + self->size.x, drawPos.y - 0x10000, 0xE0E0E0, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, self->size.y + drawPos.y, drawPos.x + self->size.x, self->size.y + drawPos.y, 0xE0E0E0, 0, INK_NONE,
                  false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + self->size.y, 0xE0E0E0, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + self->size.x, drawPos.y - 0x10000, drawPos.x + self->size.x, drawPos.y + self->size.y, 0xE0E0E0, 0, INK_NONE,
                  false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&CollapsingPlatform->animator, &drawPos, false);

    drawPos.x += self->size.x;
    self->direction = FLIP_X;
    RSDK.DrawSprite(&CollapsingPlatform->animator, &drawPos, false);

    drawPos.y += self->size.y;
    self->direction = FLIP_XY;
    RSDK.DrawSprite(&CollapsingPlatform->animator, &drawPos, false);

    drawPos.x -= self->size.x;
    self->direction = FLIP_Y;
    RSDK.DrawSprite(&CollapsingPlatform->animator, &drawPos, false);
}

void CollapsingPlatform_Create(void *data)
{
    RSDK_THIS(CollapsingPlatform);

    self->visible = true;
    self->position.x &= 0xFFF80000;
    self->position.y &= 0xFFF80000;
    self->drawFX |= FX_FLIP;
    self->drawOrder = Zone->drawOrderLow;
    if (self->targetLayer == 0) {
        self->targetLayer = Zone->fgLow;
        self->drawOrder   = Zone->drawOrderLow;
    }
    else {
        self->targetLayer = Zone->fgHigh;
        self->drawOrder   = Zone->drawOrderHigh;
    }

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        int32 xOff              = (self->position.x >> 20) - (self->size.x >> 21);
        int32 yOff              = (self->position.y >> 20) - (self->size.y >> 21);

        if ((self->size.y & 0xFFF00000) && !(self->size.y & 0xFFF00000 & 0x80000000)) {
            int32 sx = self->size.x >> 20;
            int32 sy = self->size.y >> 20;
            for (int32 y = 0; y < sy; ++y) {
                for (int32 x = 0; x < sx; ++x) {
                    self->storedTiles[x + y * (self->size.x >> 20)] = RSDK.GetTileInfo(self->targetLayer, x + xOff, y + yOff);
                }
            }
        }

        self->hitbox.right  = self->size.x >> 17;
        self->hitbox.left   = -(self->size.x >> 17);
        self->hitbox.bottom = self->size.y >> 17;
        self->hitbox.top    = -16 - self->hitbox.bottom;
    }
    switch (self->type) {
        default:
        case 0: self->state = CollapsingPlatform_State_Left; break;
        case 1: self->state = CollapsingPlatform_State_Right; break;
        case 2: self->state = CollapsingPlatform_State_Center; break;
        case 3: self->state = CollapsingPlatform_State_LeftOrRight; break;
        case 4: self->state = CollapsingPlatform_State_PlayerPos; break;
    }
}

void CollapsingPlatform_StageLoad(void)
{
    CollapsingPlatform->aniFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(CollapsingPlatform->aniFrames, 0, &CollapsingPlatform->animator, true, 0);
    if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2"))
        CollapsingPlatform->shift = 1;

    if (RSDK.CheckStageFolder("FBZ") || RSDK.CheckStageFolder("HCZ") || RSDK.CheckStageFolder("LRZ1") || RSDK.CheckStageFolder("LRZ2")
        || RSDK.CheckStageFolder("AIZ")) {
        CollapsingPlatform->sfxCrumble = RSDK.GetSFX("Stage/LedgeBreak3.wav");
    }
    else {
        CollapsingPlatform->sfxCrumble = RSDK.GetSFX("Stage/LedgeBreak.wav");
    }
}

void CollapsingPlatform_State_Left(void)
{
    RSDK_THIS(CollapsingPlatform);

    uint16 *tiles = self->storedTiles;
    int32 startTX   = (self->position.x >> 20) - (self->size.x >> 21);
    int32 startTY   = (self->position.y >> 20) - (self->size.y >> 21);
    int32 tx        = self->position.x - (self->size.x >> 1) + 0x80000;
    int32 ty        = (self->position.y - (self->size.y >> 1)) + 0x80000;

    int32 sx = self->size.x >> 20;
    int32 sy = self->size.y >> 20;

    for (int32 y = 0; y < sy; ++y) {
        for (int32 x = 0; x < sx; ++x) {
            EntityBreakableWall *tileChunk = CREATE_ENTITY(BreakableWall, intToVoid(2), tx, ty);
            tx += 0x100000;
            tileChunk->layerID   = self->targetLayer;
            tileChunk->tileInfo  = *tiles;
            tileChunk->drawOrder = self->drawOrder;
            tileChunk->tilePos.y = y + startTY;
            tileChunk->tilePos.x = x + startTX;
            int32 timerX           = x >> CollapsingPlatform->shift;
            int32 timerY           = y >> CollapsingPlatform->shift;
            tileChunk->timer     = 3 * (sy + 2 * timerX - timerY);
            ++tiles;
        }
        tx -= self->size.x;
        ty += 0x100000;
    }
}
void CollapsingPlatform_State_Right(void)
{
    RSDK_THIS(CollapsingPlatform);

    uint16 *tiles = self->storedTiles;
    int32 startTX   = (self->position.x >> 20) - (self->size.x >> 21);
    int32 startTY   = (self->position.y >> 20) - (self->size.y >> 21);
    int32 tx        = self->position.x - (self->size.x >> 1) + 0x80000;
    int32 ty        = (self->position.y - (self->size.y >> 1)) + 0x80000;

    int32 timerSX   = self->size.x >> CollapsingPlatform->shift >> 20;

    int32 sx = self->size.x >> 20;
    int32 sy = self->size.y >> 20;
    for (int32 y = 0; y < sy; ++y) {
        for (int32 x = 0; x < sx; ++x) {
            EntityBreakableWall *tileChunk = CREATE_ENTITY(BreakableWall, intToVoid(2), tx, ty);
            tx += 0x100000;
            tileChunk->layerID   = self->targetLayer;
            tileChunk->tileInfo  = *tiles;
            tileChunk->drawOrder = self->drawOrder;
            tileChunk->tilePos.y = y + startTY;
            tileChunk->tilePos.x = x + startTX;
            int32 timerX           = x >> CollapsingPlatform->shift;
            int32 timerY           = y >> CollapsingPlatform->shift;
            tileChunk->timer     = 3 * (sy + 2 * (timerSX - timerX) - timerY);
            ++tiles;
        }
        tx -= self->size.x;
        ty += 0x100000;
    }
}
void CollapsingPlatform_State_Center(void)
{
    RSDK_THIS(CollapsingPlatform);

    uint16 *tiles = self->storedTiles;
    int32 startTX   = (self->position.x >> 20) - (self->size.x >> 21);
    int32 startTY   = (self->position.y >> 20) - (self->size.y >> 21);
    int32 tx        = self->position.x - (self->size.x >> 1) + 0x80000;
    int32 ty        = (self->position.y - (self->size.y >> 1)) + 0x80000;

    int32 timerSX = self->size.x >> CollapsingPlatform->shift >> 20;
    int32 timerSY = self->size.y >> CollapsingPlatform->shift >> 20;

    int32 sx = self->size.x >> 20;
    int32 sy = self->size.y >> 20;
    for (int32 y = 0; y < sy; ++y) {
        for (int32 x = 0; x < sx; ++x) {
            EntityBreakableWall *tileChunk = CREATE_ENTITY(BreakableWall, intToVoid(2), tx, ty);
            tx += 0x100000;
            tileChunk->layerID   = self->targetLayer;
            tileChunk->tileInfo  = *tiles;
            tileChunk->drawOrder = self->drawOrder;
            tileChunk->tilePos.x = x + startTX;
            tileChunk->tilePos.y = y + startTY;
            int32 timerX           = abs((timerSX >> 1) - (x >> CollapsingPlatform->shift));
            int32 timerY           = y >> CollapsingPlatform->shift;
            tileChunk->timer     = 3 * (timerSY + 2 * timerX - timerY);
            if (!(timerSX & 1) && x >> CollapsingPlatform->shift < (timerSX >> 1))
                tileChunk->timer -= 6;
            ++tiles;
        }
        tx -= self->size.x;
        ty += 0x100000;
    }
}
void CollapsingPlatform_State_LeftOrRight(void)
{
    RSDK_THIS(CollapsingPlatform);
    int32 px = self->playerPos.x;
    int32 x  = self->position.x;

    if (px < x)
        CollapsingPlatform_State_Left();
    else
        CollapsingPlatform_State_Right();
}
void CollapsingPlatform_State_PlayerPos(void)
{
    RSDK_THIS(CollapsingPlatform);
    int32 px = self->playerPos.x;
    int32 x  = self->position.x;

    if (abs(px - x) < self->size.x / 6) {
        CollapsingPlatform_State_Center();
    }
    else {
        if (px < x)
            CollapsingPlatform_State_Left();
        else
            CollapsingPlatform_State_Right();
    }
}

void CollapsingPlatform_EditorDraw(void)
{
    RSDK_THIS(CollapsingPlatform);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= self->size.x >> 1;
    drawPos.y -= self->size.y >> 1;
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + self->size.x, drawPos.y - 0x10000, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, self->size.y + drawPos.y, drawPos.x + self->size.x, self->size.y + drawPos.y, 0xFFFF00, 0, INK_NONE,
                  false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + self->size.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + self->size.x, drawPos.y - 0x10000, drawPos.x + self->size.x, drawPos.y + self->size.y, 0xFFFF00, 0, INK_NONE,
                  false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&CollapsingPlatform->animator, &drawPos, false);

    drawPos.x += self->size.x;
    self->direction = FLIP_X;
    RSDK.DrawSprite(&CollapsingPlatform->animator, &drawPos, false);

    drawPos.y += self->size.y;
    self->direction = FLIP_XY;
    RSDK.DrawSprite(&CollapsingPlatform->animator, &drawPos, false);

    drawPos.x -= self->size.x;
    self->direction = FLIP_Y;
    RSDK.DrawSprite(&CollapsingPlatform->animator, &drawPos, false);
}

void CollapsingPlatform_EditorLoad(void)
{
    CollapsingPlatform->aniFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(CollapsingPlatform->aniFrames, 0, &CollapsingPlatform->animator, true, 0);
}

void CollapsingPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(CollapsingPlatform, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(CollapsingPlatform, VAR_BOOL, respawn);
    RSDK_EDITABLE_VAR(CollapsingPlatform, VAR_UINT16, targetLayer);
    RSDK_EDITABLE_VAR(CollapsingPlatform, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(CollapsingPlatform, VAR_ENUM, delay);
    RSDK_EDITABLE_VAR(CollapsingPlatform, VAR_BOOL, eventOnly);
    RSDK_EDITABLE_VAR(CollapsingPlatform, VAR_BOOL, mightyOnly);
}
