// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CollapsingPlatform Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCollapsingPlatform *CollapsingPlatform;

void CollapsingPlatform_Update(void)
{
    RSDK_THIS(CollapsingPlatform);

    self->visible = false;
    if (DebugMode)
        self->visible = DebugMode->debugActive;

    bool32 runState = false;

    if (self->collapseDelay) {
#if MANIA_USE_PLUS
        if (Player) {
            foreach_active(Player, player)
            {
                if (Player_CheckCollisionTouch(player, self, &self->hitboxTrigger) && player->characterID == ID_MIGHTY
                    && player->jumpAbilityState > 1) {
                    runState = true;
                    foreach_break;
                }
            }
        }
#endif

        if (!runState && --self->collapseDelay == 0)
            runState = true;
    }
    else {
        if (Player) {
            self->direction = FLIP_NONE;
            foreach_active(Player, player)
            {
                if (Player_CheckCollisionTouch(player, self, &self->hitboxTrigger)
#if MANIA_USE_PLUS
                    && (!self->mightyOnly || (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop))
#endif
                    && !player->sidekick && player->onGround && !player->collisionMode && !self->eventOnly && self->delay < 0xFFFF) {
                    self->stoodPos.x = player->position.x;
#if MANIA_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->jumpAbilityState > 1) {
                        runState = true;
                        foreach_break;
                    }
#endif
                }
            }
        }

        if (!runState && self->stoodPos.x) {
            self->collapseDelay = self->delay;
            if (!self->delay)
                runState = true;
        }
    }

    if (runState) {
        StateMachine_Run(self->state);

        RSDK.PlaySfx(CollapsingPlatform->sfxCrumble, false, 0xFF);
        if (self->respawn) {
            self->collapseDelay = 0;
            self->stoodPos.x    = 0;
        }
        else {
            destroyEntity(self);
        }
    }
}

void CollapsingPlatform_LateUpdate(void) {}

void CollapsingPlatform_StaticUpdate(void) {}

void CollapsingPlatform_Draw(void)
{
    RSDK_THIS(CollapsingPlatform);
    Vector2 drawPos;

    drawPos.x = self->position.x - (self->size.x >> 1);
    drawPos.y = self->position.y - (self->size.y >> 1);
    RSDK.DrawLine(drawPos.x - TO_FIXED(1), drawPos.y - TO_FIXED(1), drawPos.x + self->size.x, drawPos.y - TO_FIXED(1), 0xE0E0E0, 0x00, INK_NONE,
                  false);
    RSDK.DrawLine(drawPos.x - TO_FIXED(1), self->size.y + drawPos.y, drawPos.x + self->size.x, self->size.y + drawPos.y, 0xE0E0E0, 0x00, INK_NONE,
                  false);
    RSDK.DrawLine(drawPos.x - TO_FIXED(1), drawPos.y - TO_FIXED(1), drawPos.x - TO_FIXED(1), drawPos.y + self->size.y, 0xE0E0E0, 0x00, INK_NONE,
                  false);
    RSDK.DrawLine(drawPos.x + self->size.x, drawPos.y - TO_FIXED(1), drawPos.x + self->size.x, drawPos.y + self->size.y, 0xE0E0E0, 0x00, INK_NONE,
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
    self->drawGroup = Zone->objectDrawGroup[0];

    if (self->targetLayer == COLLAPSEPLAT_TARGET_LOW) {
        self->targetLayer = Zone->fgLayer[0];
        self->drawGroup   = Zone->objectDrawGroup[0];
    }
    else {
        self->targetLayer = Zone->fgLayer[1];
        self->drawGroup   = Zone->objectDrawGroup[1];
    }

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = TO_FIXED(128);
        self->updateRange.y = TO_FIXED(128);
        int32 xOff          = (self->position.x >> 20) - (self->size.x >> 21);
        int32 yOff          = (self->position.y >> 20) - (self->size.y >> 21);

        if ((self->size.y & 0xFFF00000) && !(self->size.y & 0xFFF00000 & 0x80000000)) {
            int32 sx = self->size.x >> 20;
            int32 sy = self->size.y >> 20;
            for (int32 y = 0; y < sy; ++y) {
                for (int32 x = 0; x < sx; ++x) {
                    self->storedTiles[x + y * (self->size.x >> 20)] = RSDK.GetTile(self->targetLayer, x + xOff, y + yOff);
                }
            }
        }

        self->hitboxTrigger.left   = -(self->size.x >> 17);
        self->hitboxTrigger.top    = -16 - (self->size.y >> 17);
        self->hitboxTrigger.right  = self->size.x >> 17;
        self->hitboxTrigger.bottom = self->size.y >> 17;
    }

    switch (self->type) {
        default:
        case COLLAPSEPLAT_LEFT: self->state = CollapsingPlatform_State_Left; break;
        case COLLAPSEPLAT_RIGHT: self->state = CollapsingPlatform_State_Right; break;
        case COLLAPSEPLAT_CENTER: self->state = CollapsingPlatform_State_Center; break;
        case COLLAPSEPLAT_LR: self->state = CollapsingPlatform_State_LeftRight; break;
        case COLLAPSEPLAT_LRC: self->state = CollapsingPlatform_State_LeftRightCenter; break;
    }
}

void CollapsingPlatform_StageLoad(void)
{
    CollapsingPlatform->aniFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(CollapsingPlatform->aniFrames, 0, &CollapsingPlatform->animator, true, 0);

    if (RSDK.CheckSceneFolder("OOZ1") || RSDK.CheckSceneFolder("OOZ2"))
        CollapsingPlatform->shift = 1;

    if (RSDK.CheckSceneFolder("FBZ") || RSDK.CheckSceneFolder("HCZ") || RSDK.CheckSceneFolder("LRZ1") || RSDK.CheckSceneFolder("LRZ2")
        || RSDK.CheckSceneFolder("AIZ")) {
        CollapsingPlatform->sfxCrumble = RSDK.GetSfx("Stage/LedgeBreak3.wav");
    }
    else {
        CollapsingPlatform->sfxCrumble = RSDK.GetSfx("Stage/LedgeBreak.wav");
    }
}

void CollapsingPlatform_State_Left(void)
{
    RSDK_THIS(CollapsingPlatform);

    uint16 *tiles = self->storedTiles;
    int32 startTX = (self->position.x >> 20) - (self->size.x >> 21);
    int32 startTY = (self->position.y >> 20) - (self->size.y >> 21);
    int32 tx      = self->position.x - (self->size.x >> 1) + TO_FIXED(8);
    int32 ty      = self->position.y - (self->size.y >> 1) + TO_FIXED(8);

    int32 sx = self->size.x >> 20;
    int32 sy = self->size.y >> 20;

    for (int32 y = 0; y < sy; ++y) {
        for (int32 x = 0; x < sx; ++x) {
            EntityBreakableWall *tile = CREATE_ENTITY(BreakableWall, INT_TO_VOID(BREAKWALL_TILE_DYNAMIC), tx, ty);
            tile->targetLayer         = self->targetLayer;
            tile->tileInfo            = *tiles;
            tile->drawGroup           = self->drawGroup;
            tile->tilePos.x           = x + startTX;
            tile->tilePos.y           = y + startTY;
            int32 timerX              = x >> CollapsingPlatform->shift;
            int32 timerY              = y >> CollapsingPlatform->shift;
            tile->timer               = 3 * (sy + 2 * timerX - timerY);

            ++tiles;
            tx += 0x100000;
        }

        tx -= self->size.x;
        ty += 0x100000;
    }
}
void CollapsingPlatform_State_Right(void)
{
    RSDK_THIS(CollapsingPlatform);

    uint16 *tiles = self->storedTiles;
    int32 startTX = (self->position.x >> 20) - (self->size.x >> 21);
    int32 startTY = (self->position.y >> 20) - (self->size.y >> 21);
    int32 tx      = self->position.x - (self->size.x >> 1) + TO_FIXED(8);
    int32 ty      = self->position.y - (self->size.y >> 1) + TO_FIXED(8);

    int32 timerSX = self->size.x >> CollapsingPlatform->shift >> 20;

    int32 sx = self->size.x >> 20;
    int32 sy = self->size.y >> 20;

    for (int32 y = 0; y < sy; ++y) {
        for (int32 x = 0; x < sx; ++x) {
            EntityBreakableWall *tile = CREATE_ENTITY(BreakableWall, INT_TO_VOID(BREAKWALL_TILE_DYNAMIC), tx, ty);
            tile->targetLayer         = self->targetLayer;
            tile->tileInfo            = *tiles;
            tile->drawGroup           = self->drawGroup;
            tile->tilePos.x           = x + startTX;
            tile->tilePos.y           = y + startTY;
            int32 timerX              = x >> CollapsingPlatform->shift;
            int32 timerY              = y >> CollapsingPlatform->shift;
            tile->timer               = 3 * (sy + 2 * (timerSX - timerX) - timerY);

            ++tiles;
            tx += 0x100000;
        }

        tx -= self->size.x;
        ty += 0x100000;
    }
}
void CollapsingPlatform_State_Center(void)
{
    RSDK_THIS(CollapsingPlatform);

    uint16 *tiles = self->storedTiles;
    int32 startTX = (self->position.x >> 20) - (self->size.x >> 21);
    int32 startTY = (self->position.y >> 20) - (self->size.y >> 21);
    int32 tx      = self->position.x - (self->size.x >> 1) + TO_FIXED(8);
    int32 ty      = self->position.y - (self->size.y >> 1) + TO_FIXED(8);

    int32 timerSX = self->size.x >> CollapsingPlatform->shift >> 20;
    int32 timerSY = self->size.y >> CollapsingPlatform->shift >> 20;

    int32 sx = self->size.x >> 20;
    int32 sy = self->size.y >> 20;

    for (int32 y = 0; y < sy; ++y) {
        for (int32 x = 0; x < sx; ++x) {
            EntityBreakableWall *tile = CREATE_ENTITY(BreakableWall, INT_TO_VOID(BREAKWALL_TILE_DYNAMIC), tx, ty);
            tile->targetLayer         = self->targetLayer;
            tile->tileInfo            = *tiles;
            tile->drawGroup           = self->drawGroup;
            tile->tilePos.x           = x + startTX;
            tile->tilePos.y           = y + startTY;
            int32 timerX              = abs((timerSX >> 1) - (x >> CollapsingPlatform->shift));
            int32 timerY              = y >> CollapsingPlatform->shift;
            tile->timer               = 3 * (timerSY + 2 * timerX - timerY);

            if (!(timerSX & 1) && x >> CollapsingPlatform->shift < (timerSX >> 1))
                tile->timer -= 6;

            ++tiles;
            tx += 0x100000;
        }

        tx -= self->size.x;
        ty += 0x100000;
    }
}
void CollapsingPlatform_State_LeftRight(void)
{
    RSDK_THIS(CollapsingPlatform);

    int32 px = self->stoodPos.x;
    int32 x  = self->position.x;

    if (px < x)
        CollapsingPlatform_State_Left();
    else
        CollapsingPlatform_State_Right();
}
void CollapsingPlatform_State_LeftRightCenter(void)
{
    RSDK_THIS(CollapsingPlatform);

    int32 px = self->stoodPos.x;
    int32 x  = self->position.x;

    if (abs(px - x) < self->size.x / 6)
        CollapsingPlatform_State_Center();
    else if (px < x)
        CollapsingPlatform_State_Left();
    else
        CollapsingPlatform_State_Right();
}

#if GAME_INCLUDE_EDITOR
void CollapsingPlatform_EditorDraw(void)
{
    RSDK_THIS(CollapsingPlatform);

    DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->size.x, self->size.y, 0xFFFF00);

    Vector2 drawPos;
    drawPos.x = self->position.x - (self->size.x >> 1);
    drawPos.y = self->position.y - (self->size.y >> 1);

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

    RSDK_ACTIVE_VAR(CollapsingPlatform, type);
    RSDK_ENUM_VAR("Left", COLLAPSEPLAT_LEFT);
    RSDK_ENUM_VAR("Right", COLLAPSEPLAT_RIGHT);
    RSDK_ENUM_VAR("Center", COLLAPSEPLAT_CENTER);
    RSDK_ENUM_VAR("Left or Right", COLLAPSEPLAT_LR);
    RSDK_ENUM_VAR("Base on Player Position", COLLAPSEPLAT_LRC);

    RSDK_ACTIVE_VAR(CollapsingPlatform, targetLayer);
    RSDK_ENUM_VAR("Low", COLLAPSEPLAT_TARGET_LOW);
    RSDK_ENUM_VAR("High", COLLAPSEPLAT_TARGET_HIGH);
}
#endif

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
