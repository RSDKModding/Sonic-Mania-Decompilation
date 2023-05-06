// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DERobot Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDERobot *DERobot;

void DERobot_Update(void)
{
    RSDK_THIS(DERobot);
    StateMachine_Run(self->state);
}

void DERobot_LateUpdate(void) {}

void DERobot_StaticUpdate(void) {}

void DERobot_Draw(void)
{
    RSDK_THIS(DERobot);
    StateMachine_Run(self->stateDraw);
}

void DERobot_Create(void *data)
{
    RSDK_THIS(DERobot);
    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->drawGroup     = Zone->objectDrawGroup[0];
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            int32 slotID        = RSDK.GetEntitySlot(self);
            if (data)
                self->aniID = VOID_TO_INT(data);

            switch (self->aniID) {
                case DEROBOT_BODY:
                    self->active        = ACTIVE_BOUNDS;
                    self->legs[0]       = RSDK_GET_ENTITY(slotID - 7, DERobot);
                    self->legs[1]       = RSDK_GET_ENTITY(slotID - 6, DERobot);
                    self->legs[2]       = RSDK_GET_ENTITY(slotID - 5, DERobot);
                    self->arms[0]       = RSDK_GET_ENTITY(slotID - 4, DERobot);
                    self->arms[1]       = RSDK_GET_ENTITY(slotID - 3, DERobot);
                    self->eggman        = RSDK_GET_ENTITY(slotID - 2, Eggman);
                    self->head          = RSDK_GET_ENTITY(slotID - 1, DERobot);
                    self->legs[3]       = RSDK_GET_ENTITY(slotID + 1, DERobot);
                    self->legs[4]       = RSDK_GET_ENTITY(slotID + 2, DERobot);
                    self->legs[5]       = RSDK_GET_ENTITY(slotID + 3, DERobot);
                    self->shoulderFront = RSDK_GET_ENTITY(slotID + 4, DERobot);
                    self->arms[2]       = RSDK_GET_ENTITY(slotID + 5, DERobot);
                    self->arms[3]       = RSDK_GET_ENTITY(slotID + 6, DERobot);
                    self->health        = 8;
                    self->state         = DERobot_State_SetupArena;
                    self->stateDraw     = DERobot_Draw_Simple;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->mainAnimator, true, self->frameID);
                    break;

                case DEROBOT_HEAD:
                    self->parent    = (Entity *)RSDK_GET_ENTITY(slotID + 1, DERobot);
                    self->stateDraw = DERobot_Draw_RelativeToParent;
                    self->drawFX    = FX_ROTATE;
                    self->offset.x  = -0x160000;
                    self->offset.y  = -0x240000;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->mainAnimator, true, self->frameID);
                    break;

                case DEROBOT_ARM:
                    if (self->frameID == 2) {
                        RSDK.SetSpriteAnimation(DERobot->aniFrames, 5, &self->altAnimator, true, 0);
                        RSDK.SetSpriteAnimation(DERobot->aniFrames, 4, &self->armAnimator, true, 0);
                        self->stateDraw = DERobot_Draw_Arm;
                        self->drawFX    = FX_ROTATE;
                        self->state     = DERobot_State_ArmIdle;
                    }
                    else if (self->frameID) {
                        self->stateDraw = DERobot_Draw_Simple;
                    }
                    else {
                        self->parent    = (Entity *)RSDK_GET_ENTITY(slotID - 4, DERobot);
                        self->stateDraw = DERobot_Draw_RelativeToParent;
                        self->offset.x  = -0xC0000;
                        self->offset.y  = -0x100000;
                    }
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->mainAnimator, true, self->frameID);
                    break;

                case DEROBOT_LEG:
                    if (self->frameID) {
                        self->stateDraw = DERobot_Draw_Simple;
                    }
                    else {
                        RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->altAnimator, true, 1);
                        self->stateDraw = DERobot_Draw_FrontLeg;
                    }

                    if (self->frameID > 1)
                        self->drawFX = FX_ROTATE;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->mainAnimator, true, self->frameID);
                    break;

                case DEROBOT_TARGET_EDGE:
                    self->active     = ACTIVE_NORMAL;
                    self->visible    = true;
                    self->drawFX     = FX_FLIP;
                    self->drawGroup  = Zone->objectDrawGroup[1];
                    self->velocity.x = 0x20000;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, 7, &self->altAnimator, true, 0);
                    self->state     = DERobot_State_Target;
                    self->stateDraw = DERobot_Draw_Target;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->mainAnimator, true, self->frameID);
                    break;

                case DEROBOT_BOMB:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->state     = DERobot_State_BombLaunched;
                    self->stateDraw = DERobot_Draw_Simple;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->mainAnimator, true, self->frameID);
                    break;

                case DEROBOT_BODY_CUTSCENE:
                    self->active        = ACTIVE_BOUNDS;
                    self->visible       = true;
                    self->stateDraw     = DERobot_Draw_Simple;
                    self->shoulderFront = RSDK_GET_ENTITY(slotID + 4, DERobot);
                    self->arms[0]       = RSDK_GET_ENTITY(slotID + 1, DERobot);
                    self->arms[1]       = RSDK_GET_ENTITY(slotID + 2, DERobot);
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->mainAnimator, true, self->frameID);
                    break;

                default:
                    self->stateDraw = DERobot_Draw_Simple;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->mainAnimator, true, self->frameID);
                    break;
            }
        }
        else {
            destroyEntity(self);
        }
    }
}

void DERobot_StageLoad(void)
{
    DERobot->aniFrames = RSDK.LoadSpriteAnimation("GHZ/DERobot.bin", SCOPE_STAGE);

    DERobot->hitboxBody.left   = -32;
    DERobot->hitboxBody.top    = -40;
    DERobot->hitboxBody.right  = 32;
    DERobot->hitboxBody.bottom = 32;

    DERobot->hitboxHand.left   = -9;
    DERobot->hitboxHand.top    = -9;
    DERobot->hitboxHand.right  = 9;
    DERobot->hitboxHand.bottom = 9;

    DERobot->sfxHit        = RSDK.GetSfx("Stage/BossHit.wav");
    DERobot->sfxExplosion  = RSDK.GetSfx("Stage/Explosion2.wav");
    DERobot->sfxImpact     = RSDK.GetSfx("Stage/Impact2.wav");
    DERobot->sfxTargeting  = RSDK.GetSfx("Stage/Targeting1.wav");
    DERobot->sfxLedgeBreak = RSDK.GetSfx("Stage/LedgeBreak.wav");
    DERobot->sfxBuzzsaw    = RSDK.GetSfx("Stage/Buzzsaw.wav");
    DERobot->sfxDrop       = RSDK.GetSfx("Stage/Drop.wav");
    DERobot->sfxButton2    = RSDK.GetSfx("Stage/Button2.wav");
    DERobot->sfxHullClose  = RSDK.GetSfx("Stage/HullClose.wav");

    RSDK.SetPaletteEntry(0, 236, 0x282028);
    RSDK.SetPaletteEntry(0, 237, 0x383040);
    RSDK.SetPaletteEntry(0, 238, 0x484868);
    RSDK.SetPaletteEntry(0, 239, 0x587090);
    RSDK.SetPaletteEntry(0, 244, 0x000000);

    // Bug Details:
    // palette entry 244 is overwritten in GHZ2Cutscene during plus
    // this is due to the additional colours in the tileset
    // the simple fix to this should be to move DERobot above CutsceneHBH in the scene/stageconfig
    // this will allow the CutsceneHBH object to properly store the 0x000000 colour set here when it does its palette swaps
    // therefore fixing the orange DERobot bug
    // alternatively, have GHZ2Outro_Cutscene_HoleSceneFadeIn call CutsceneHBH_StorePalette to store the updated palette
}

void DERobot_HandleScreenBounds(void)
{
    RSDK_THIS(DERobot);
    int32 x = (self->position.x >> 16) - ScreenInfo->center.x + 128;
    if (x > Zone->cameraBoundsL[0]) {
        Zone->cameraBoundsL[0]                         = x;
        Zone->cameraBoundsR[0]                         = ScreenInfo->size.x + 96 + x;
        Zone->playerBoundsL[0]                         = Zone->cameraBoundsL[0] << 16;
        Zone->playerBoundsR[0]                         = Zone->cameraBoundsR[0] << 16;
        Zone->playerBoundActiveB[0]                    = 0;
        RSDK_GET_ENTITY(SLOT_CAMERA1, Camera)->boundsL = Zone->cameraBoundsL[0];
    }
}

void DERobot_HandleLegMovement(int32 offset)
{
    RSDK_THIS(DERobot);
    EntityDERobot *knee = self->legs[offset + 0];
    EntityDERobot *leg  = self->legs[offset + 1];
    EntityDERobot *foot = self->legs[offset + 2];

    knee->position.x = 0x440 * RSDK.Sin1024(knee->angle) + self->position.x;
    knee->position.y = 0x440 * RSDK.Cos1024(knee->angle) + self->position.y + 0x190000;
    leg->position.x  = knee->position.x;
    leg->position.y  = knee->position.y;
    leg->angle       = MAX(knee->angle >> 2, 0);
    leg->rotation    = -(leg->angle >> 1);
    foot->position.x = 0xA00 * RSDK.Sin1024(leg->angle) + leg->position.x;
    foot->position.y = 0xA00 * RSDK.Cos1024(leg->angle) + leg->position.y;
    foot->onGround   = false;
    int32 storeX     = foot->position.x;
    int32 storeY     = foot->position.y;
    if (RSDK.ObjectTileGrip(foot, Zone->collisionLayers, CMODE_FLOOR, 0, -0x100000, 0x100000, 32)) {
        foot->position.y = storeY;
        if (RSDK.ObjectTileGrip(foot, Zone->collisionLayers, CMODE_FLOOR, 0, 0x100000, 0x100000, 32))
            foot->rotation = 2 * RSDK.ATan2(32, (foot->position.y >> 16) - (foot->position.y >> 16));
    }
    foot->position.x = storeX;
    foot->position.y = storeY;
    if (RSDK.ObjectTileCollision(foot, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x110000, true))
        foot->onGround = true;
}

void DERobot_HandleLegMovement2(int32 offset)
{
    RSDK_THIS(DERobot);
    EntityDERobot *leg  = self->legs[offset + 0];
    EntityDERobot *foot = self->legs[offset + 1];
    EntityDERobot *knee = self->legs[offset - 1];
    leg->position.x     = foot->position.x - 0xA00 * RSDK.Sin1024(leg->angle);
    leg->position.y     = foot->position.y - 0xA00 * RSDK.Cos1024(leg->angle);
    leg->angle -= leg->angle >> 3;
    leg->rotation -= leg->rotation >> 3;
    knee->position.x = leg->position.x;
    knee->position.y = leg->position.y;
    self->position.x = knee->position.x - 0x440 * RSDK.Sin1024(knee->angle);
    self->position.y = knee->position.y - (0x440 * RSDK.Cos1024(knee->angle)) - 0x190000;
}

void DERobot_HandleArmMovement(int32 offset)
{
    RSDK_THIS(DERobot);
    EntityDERobot *arm  = self->arms[offset + 0];
    EntityDERobot *hand = self->arms[offset + 1];
    arm->position.x     = 0x600 * (RSDK.Sin1024(arm->angle) - 0x200) + self->position.x;
    arm->position.y     = 0x600 * (RSDK.Cos1024(arm->angle) - 0x300) + self->position.y;
    hand->position.x    = arm->position.x + 0x600 * RSDK.Cos1024(MIN(arm->angle, 0));
    hand->position.y    = arm->position.y - 0x600 * RSDK.Sin1024(MIN(arm->angle, 0));
}

void DERobot_HandleTerrainDestruction(void)
{
    RSDK_THIS(DERobot);
    int32 tx = (self->position.x + 0xC0000) >> 20;
    if (tx > self->destroyedTerrainX) {
        self->destroyedTerrainX = tx;
        int32 ty                = (self->position.y >> 20) - 16;

        int32 spawnX = (tx << 20) + 0x80000;
        int32 spawnY = (ty << 20) + 0x80000;

        bool32 playSfx = false;
        for (int32 i = 0; i < 32; ++i) {
            uint16 tile = RSDK.GetTile(Zone->fgLayer[1], tx, ty);
            if (tile != (uint16)-1) {
                RSDK.SetTile(Zone->fgLayer[1], tx, ty, -1);

                EntityBreakableWall *wall = CREATE_ENTITY(BreakableWall, INT_TO_VOID(BREAKWALL_TILE_FIXED), spawnX, spawnY);
                wall->drawGroup           = Zone->objectDrawGroup[1];
                wall->visible             = true;
                wall->tileInfo            = tile;
                wall->velocity.x          = RSDK.Rand(-0x20000, 0x20000);
                wall->velocity.y          = RSDK.Rand(-0x20000, 0x20000);
                wall->drawFX              = FX_ROTATE | FX_FLIP;
                playSfx                   = true;
            }
            ++ty;
            spawnY += 0x100000;
        }

        if (playSfx)
            RSDK.PlaySfx(DERobot->sfxLedgeBreak, false, 255);
    }
}

void DERobot_DestroyTerrainFinal(void)
{
    RSDK_THIS(DERobot);

    int32 tx = (self->position.x >> 20) - 4;
    int32 ty = (self->position.y >> 20) - 16;

    int32 spawnX = (tx << 20) + 0x80000;
    for (int32 y = 0; y < 8; ++y) {
        int32 spawnY = (ty << 20) + 0x80000;
        for (int32 x = 0; x < 32; ++x) {
            uint16 tile = RSDK.GetTile(Zone->fgLayer[0], tx, ty);
            if (tile != (uint16)-1) {
                RSDK.SetTile(Zone->fgLayer[0], tx, ty, -1);

                EntityBreakableWall *wall = CREATE_ENTITY(BreakableWall, INT_TO_VOID(BREAKWALL_TILE_FIXED), spawnX, spawnY);
                wall->drawGroup           = Zone->objectDrawGroup[1];
                wall->visible             = true;
                wall->tileInfo            = tile;
                wall->velocity.x          = RSDK.Rand(-0x20000, 0x20000);
                wall->velocity.y          = RSDK.Rand(-0x20000, 0x20000);
                wall->drawFX              = FX_ROTATE | FX_FLIP;
            }
            ++ty;
            spawnY += 0x100000;
        }
        spawnX += 0x100000;
        ty -= 32;
        ++tx;
    }

    tx     = (self->position.x >> 20) - 4;
    ty     = (self->position.y >> 20) - 16;
    spawnX = (tx << 20) + 0x80000;
    for (int32 y = 0; y < 32; ++y) {
        int32 spawnY = (ty << 20) + 0x80000;
        for (int32 x = 0; x < 32; ++x) {
            uint16 tile = RSDK.GetTile(Zone->fgLayer[1], tx, ty);
            if (tile != (uint16)-1) {
                RSDK.SetTile(Zone->fgLayer[1], tx, ty, -1);
                EntityBreakableWall *wall = CREATE_ENTITY(BreakableWall, INT_TO_VOID(BREAKWALL_TILE_FIXED), spawnX, spawnY);
                wall->drawGroup           = Zone->objectDrawGroup[1];
                wall->visible             = true;
                wall->tileInfo            = tile;
                wall->velocity.x          = RSDK.Rand(-0x20000, 0x20000);
                wall->velocity.y          = RSDK.Rand(-0x20000, 0x20000);
                wall->drawFX              = FX_ROTATE | FX_FLIP;
            }
            ++ty;
            spawnY += 0x100000;
        }
        spawnX += 0x100000;
        ty -= 32;
        ++tx;
    }
}

void DERobot_Hit(void)
{
    RSDK_THIS(DERobot);

    if (--self->health <= 0) {
        self->timer            = 0;
        self->state            = DERobot_State_Explode;
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        self->arms[1]->state = DERobot_State_ArmDestroyed;
        self->arms[3]->state = DERobot_State_ArmDestroyed;
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.PlaySfx(DERobot->sfxHit, false, 255);
        if (self->health == 2) {
            self->kneeAngleVel  = 16;
            self->angleVelStart = 0x20000;
            self->angleMaxInc   = -0x4000;
            self->legs[0]->angleVel *= 2;
            self->legs[0]->angleInc *= 4;
            self->legs[3]->angleVel *= 2;
            self->legs[3]->angleInc *= 4;
        }
    }
}

void DERobot_Explode(void)
{
    RSDK_THIS(DERobot);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(DERobot->sfxExplosion, false, 0xFF);

        if ((Zone->timer & 4)) {
            int32 x                    = self->position.x + (RSDK.Rand(-48, 48) << 16);
            int32 y                    = self->position.y + (RSDK.Rand(-48, 48) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawGroup       = Zone->objectDrawGroup[1];
        }
    }
}

void DERobot_CheckPlayerCollisions_Body(void)
{
    RSDK_THIS(DERobot);

    if (self->invincibilityTimer > 0) {
        self->invincibilityTimer--;

        if ((self->invincibilityTimer & 2)) {
            RSDK.SetPaletteEntry(0, 236, 0xC0C0C0);
            RSDK.SetPaletteEntry(0, 237, 0xD0D0D0);
            RSDK.SetPaletteEntry(0, 238, 0xE0E0E0);
            RSDK.SetPaletteEntry(0, 239, 0xF0F0F0);
            RSDK.SetPaletteEntry(0, 244, 0xA0A0A0);
        }
        else {
            RSDK.SetPaletteEntry(0, 236, 0x282028);
            RSDK.SetPaletteEntry(0, 237, 0x383040);
            RSDK.SetPaletteEntry(0, 238, 0x484868);
            RSDK.SetPaletteEntry(0, 239, 0x587090);
            RSDK.SetPaletteEntry(0, 244, 0x000000);
        }
    }

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &DERobot->hitboxBody) && Player_CheckBossHit(player, self)) {
            DERobot_Hit();
            // DERobot_Hit should already play sfxHit, not sure why its here too
            RSDK.PlaySfx(DERobot->sfxHit, false, 255);
            player->velocity.x = 0x60000;
        }
    }
}

void DERobot_CheckPlayerCollisions_ArmExtend(void)
{
    RSDK_THIS(DERobot);

    int32 extend = ((self->armExtend + 22) << 16) >> 8;

    int32 angle  = -(self->angle >> 3);
    int32 left   = (-0xE00 * RSDK.Sin256(angle)) + (0x1600 * RSDK.Cos256(angle));
    int32 top    = (-0xE00 * RSDK.Cos256(angle)) - (0x1600 * RSDK.Sin256(angle));
    int32 right  = (-0xE00 * RSDK.Sin256(angle)) + extend * RSDK.Cos256(angle);
    int32 bottom = (((-0xE00 * RSDK.Cos256(angle)) - (extend * RSDK.Sin256(angle))) >> 16) - (top >> 16);

    self->hitbox.left  = (left >> 16);
    self->hitbox.right = (right >> 16);

    foreach_active(Player, player)
    {
        int32 dist = (player->position.x - self->position.x) >> 16;
        if (dist >= left >> 16) {
            if (dist > (right >> 16))
                dist = (right >> 16);
        }
        else {
            dist = left >> 16;
        }
        self->hitbox.top    = (top >> 16) + bottom * (dist - (left >> 16)) / ((right >> 16) - (left >> 16));
        self->hitbox.bottom = self->hitbox.top + 16;
        if (Player_CheckCollisionPlatform(player, self, &self->hitbox))
            player->position.y += 0x40000;
    }
}

void DERobot_CheckPlayerCollisions_Hand(void)
{
    RSDK_THIS(DERobot);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    Vector2 pivot;
    pivot.x = self->position.x;
    pivot.y = self->position.y;
    self->position.x += (self->armExtend + 35) << 16;
    Zone_RotateOnPivot(&self->position, &pivot, -(self->angle >> 3));

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &DERobot->hitboxHand)) {
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x400, false, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }
    }

    DERobot_HandleTerrainDestruction();
    self->position.x = storeX;
    self->position.y = storeY;
}

bool32 DERobot_CheckRubyGrabbed(void)
{
    RSDK_THIS(DERobot);

    bool32 grabbedRuby = false;
    int32 storeX       = self->position.x;
    int32 storeY       = self->position.y;

    Vector2 pivot;
    pivot.x = self->position.x;
    pivot.y = self->position.y;
    self->position.x += (self->armExtend + 48) << 16;
    Zone_RotateOnPivot(&self->position, &pivot, -(self->angle >> 3));

    foreach_active(PhantomRuby, ruby)
    {
        if (abs(self->position.x - ruby->position.x) < 0x80000) {
            if (abs(self->position.y - ruby->position.y) < 0x80000) {
                grabbedRuby      = true;
                ruby->position.x = self->position.x;
                ruby->position.y = self->position.y;
                ruby->drawGroup  = Zone->objectDrawGroup[0];
                ruby->state      = PhantomRuby_State_RotateToOrigin;
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
    return grabbedRuby;
}

void DERobot_CheckPlayerCollisions_Bomb(void)
{
    RSDK_THIS(DERobot);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &DERobot->hitboxHand))
            Player_Hurt(player, self);
    }
}

void DERobot_Draw_RelativeToParent(void)
{
    RSDK_THIS(DERobot);

    self->position.x = self->parent->position.x + self->offset.x;
    self->position.y = self->parent->position.y + self->offset.y;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

void DERobot_Draw_Arm(void)
{
    RSDK_THIS(DERobot);

    self->rotation = self->angle >> 2;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    if (self->armExtend > 0) {
        SpriteFrame *frame = RSDK.GetFrame(DERobot->aniFrames, self->altAnimator.animationID, self->altAnimator.frameID);
        frame->width       = self->armExtend;
        frame->sprX        = 4 * (Zone->timer & 3);
        RSDK.DrawSprite(&self->altAnimator, NULL, false);
    }

    RSDK.GetFrame(DERobot->aniFrames, self->armAnimator.animationID, self->armAnimator.frameID)->pivotX = self->armExtend + 22;
    RSDK.DrawSprite(&self->armAnimator, NULL, false);
}

void DERobot_Draw_Simple(void)
{
    RSDK_THIS(DERobot);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

void DERobot_Draw_FrontLeg(void)
{
    RSDK_THIS(DERobot);

    self->drawFX   = FX_NONE;
    self->rotation = -(self->angle >> 1);
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    self->drawFX = FX_ROTATE;
    RSDK.DrawSprite(&self->altAnimator, NULL, false);
}

void DERobot_Draw_Target(void)
{
    RSDK_THIS(DERobot);

    Vector2 drawPos;

    self->mainAnimator.frameID = 0;
    self->inkEffect            = INK_ALPHA;
    self->direction            = FLIP_NONE;
    drawPos.x                  = self->position.x - self->offset.x;
    drawPos.y                  = self->position.y - self->offset.y;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    self->direction = FLIP_X;
    drawPos.x       = self->position.x + self->offset.x;
    drawPos.y       = self->position.y - self->offset.y;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    self->mainAnimator.frameID = 1;
    self->direction            = FLIP_NONE;
    drawPos.x                  = self->position.x - self->offset.x;
    drawPos.y                  = self->position.y + self->offset.y;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    self->direction = FLIP_X;
    drawPos.x       = self->position.x + self->offset.x;
    drawPos.y       = self->position.y + self->offset.y;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    self->inkEffect = INK_NONE;
    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->altAnimator, NULL, false);
    RSDK.DrawSprite(&self->armAnimator, NULL, false);
}

void DERobot_State_ArmIdle(void)
{
    RSDK_THIS(DERobot);

    RSDK.ProcessAnimation(&self->armAnimator);
    if (self->armAnimator.speed > 0x40)
        self->armAnimator.speed -= 4;

    DERobot_CheckPlayerCollisions_Hand();
}

void DERobot_State_ArmExtendPrepare(void)
{
    RSDK_THIS(DERobot);

    RSDK.ProcessAnimation(&self->armAnimator);

    if (self->armAnimator.speed == 240)
        RSDK.PlaySfx(DERobot->sfxBuzzsaw, false, 255);

    if (self->armAnimator.speed >= 0x100)
        self->state = DERobot_State_ArmExtending;
    else
        self->armAnimator.speed += 4;

    DERobot_CheckPlayerCollisions_Hand();
}

void DERobot_State_ArmExtending(void)
{
    RSDK_THIS(DERobot);

    RSDK.ProcessAnimation(&self->altAnimator);
    RSDK.ProcessAnimation(&self->armAnimator);
    if (self->armExtend < 192)
        self->armExtend += 8;

    DERobot_CheckPlayerCollisions_ArmExtend();
    DERobot_CheckPlayerCollisions_Hand();

    if (++self->timer == 120) {
        self->timer = 0;
        self->state = DERobot_State_ArmRetracting;
    }
}

void DERobot_State_ArmRetracting(void)
{
    RSDK_THIS(DERobot);

    RSDK.ProcessAnimation(&self->altAnimator);
    RSDK.ProcessAnimation(&self->armAnimator);

    if (self->armExtend <= 0) {
        self->state = DERobot_State_ArmIdle;
    }
    else {
        DERobot_CheckPlayerCollisions_ArmExtend();
        self->armExtend -= 8;
    }

    DERobot_CheckPlayerCollisions_Hand();
}

void DERobot_State_ArmDestroyed(void)
{
    RSDK_THIS(DERobot);

    RSDK.ProcessAnimation(&self->altAnimator);
    RSDK.ProcessAnimation(&self->armAnimator);

    if (self->armExtend > 0)
        self->armExtend -= 8;
}

void DERobot_Cutscene_ActivateArm(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&self->armAnimator);

    if (self->angle > -96)
        self->angle -= 4;

    if (self->armAnimator.speed < 0x80)
        self->armAnimator.speed += 4;

    if (self->timer++ == -1)
        RSDK.PlaySfx(DERobot->sfxButton2, false, 255);

    if (self->timer == 30) {
        self->timer = 0;
        self->state = DERobot_Cutscene_ReachForRuby;
        RSDK.PlaySfx(DERobot->sfxBuzzsaw, false, 255);
    }
}

void DERobot_Cutscene_ReachForRuby(void)
{
    RSDK_THIS(DERobot);

    RSDK.ProcessAnimation(&self->altAnimator);
    RSDK.ProcessAnimation(&self->armAnimator);

    if (DERobot_CheckRubyGrabbed() || self->armExtend >= 192)
        self->state = DERobot_Cutscene_GrabbedRuby;
    else
        self->armExtend += 8;
}

void DERobot_Cutscene_GrabbedRuby(void)
{
    RSDK_THIS(DERobot);

    RSDK.ProcessAnimation(&self->altAnimator);
    RSDK.ProcessAnimation(&self->armAnimator);

    if (self->armExtend <= 0) {
        RSDK.StopSfx(DERobot->sfxBuzzsaw);
        self->state = DERobot_Cutscene_ArmDeactivate;
    }
    else {
        self->armExtend -= 8;
        DERobot_CheckRubyGrabbed();
    }
}

void DERobot_Cutscene_ArmDeactivate(void)
{
    RSDK_THIS(DERobot);

    RSDK.ProcessAnimation(&self->armAnimator);

    if (self->armAnimator.speed)
        self->armAnimator.speed--;
}

void DERobot_State_CloseHeadHatch(void)
{
    RSDK_THIS(DERobot);

    self->timer++;
    if (self->timer == 30) {
        RSDK.PlaySfx(DERobot->sfxButton2, false, 255);
    }

    if (self->timer >= 30) {
        if (self->rotation >= 0) {
            self->timer  = 0;
            self->state  = 0;
            self->drawFX = FX_NONE;
            RSDK.PlaySfx(DERobot->sfxHullClose, false, 255);
        }
        else {
            self->rotation += 8;
        }
    }
}

void DERobot_State_BombLaunched(void)
{
    RSDK_THIS(DERobot);

    self->angle += 7;
    if (self->angle > 480) {
        self->velocity.y += 0x4000;
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
    }
    else {
        self->velocity.x = self->position.x;
        self->position.x = self->offset.x - self->scale.x * RSDK.Cos1024(self->angle);
        self->velocity.x = self->position.x - self->velocity.x;
        self->velocity.y = self->position.y;
        self->position.y = self->offset.y - self->scale.y * RSDK.Sin1024(self->angle);
        self->velocity.y = self->position.y - self->velocity.y;
    }

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xE0000, true))
        self->state = DERobot_State_BombLanded;

    DERobot_CheckPlayerCollisions_Bomb();
}

void DERobot_State_BombLanded(void)
{
    RSDK_THIS(DERobot);

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (self->mainAnimator.speed >= 0x80) {
        self->visible              = false;
        self->state                = DERobot_State_BombExplode;
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSSPUFF), self->position.x, self->position.y - 0x80000);
        explosion->drawGroup       = Zone->objectDrawGroup[1];
        RSDK.PlaySfx(DERobot->sfxExplosion, false, 255);
    }
    else {
        self->mainAnimator.speed++;
    }

    DERobot_CheckPlayerCollisions_Bomb();
}

void DERobot_State_BombExplode(void)
{
    RSDK_THIS(DERobot);

    self->position.y -= 0x20000;

    ++self->timer;
    if (self->timer < 16)
        DERobot_CheckPlayerCollisions_Bomb();

    if (!(self->timer & 7)) {
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSSPUFF), self->position.x, self->position.y);
        explosion->drawGroup       = Zone->objectDrawGroup[1];
    }

    if (self->timer == 32)
        destroyEntity(self);
}

void DERobot_State_SetupArena(void)
{
    RSDK_THIS(DERobot);

    if (++self->timer >= 8) {
        self->timer = 0;

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player1->position.y <= self->position.y + 0x200000 && player1->state != Player_State_TubeRoll) {
            for (int32 i = 0; i < Player->playerCount; ++i) {
                Zone->cameraBoundsL[i]      = (self->position.x >> 16) - ScreenInfo->center.x + 128;
                Zone->cameraBoundsR[i]      = ScreenInfo->center.x + 128 + (self->position.x >> 16);
                Zone->cameraBoundsB[i]      = self->position.y >> 16;
                Zone->playerBoundsB[i]      = Zone->cameraBoundsB[i] << 16;
                Zone->playerBoundActiveL[i] = true;
                Zone->playerBoundActiveR[i] = true;
                Zone->playerBoundActiveB[i] = false;
            }
            self->position.y -= 0x1800000;
            self->active = ACTIVE_NORMAL;
            self->state  = DERobot_State_SetupBoss;
        }
    }
}

void DERobot_State_SetupBoss(void)
{
    RSDK_THIS(DERobot);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (self->timer) {
        self->timer++;
        if (self->timer == 60) {
            CREATE_ENTITY(DERobot, INT_TO_VOID(DEROBOT_TARGET_EDGE), self->position.x, 0x3080000);
            RSDK.PlaySfx(DERobot->sfxTargeting, false, 255);
            Music_TransitionTrack(TRACK_EGGMAN1, 0.0125);
        }

        if (self->timer == 160) {
            self->timer   = 0;
            self->visible = true;
            self->state   = DERobot_State_SurpriseFall;
            if (player1->characterID == ID_TAILS)
                player1->stateAbility = Player_JumpAbility_Tails;

            EntityDERobot *kneeBack = self->legs[0];
            kneeBack->active        = ACTIVE_NORMAL;
            kneeBack->visible       = true;

            EntityDERobot *legBack = self->legs[1];
            legBack->active        = ACTIVE_NORMAL;
            legBack->visible       = true;

            EntityDERobot *footBack = self->legs[2];
            footBack->active        = ACTIVE_NORMAL;
            footBack->visible       = true;

            EntityDERobot *kneeFront = self->legs[3];
            kneeFront->active        = ACTIVE_NORMAL;
            kneeFront->visible       = true;

            EntityDERobot *legFront = self->legs[4];
            legFront->active        = ACTIVE_NORMAL;
            legFront->visible       = true;

            EntityDERobot *footFront = self->legs[5];
            footFront->active        = ACTIVE_NORMAL;
            footFront->visible       = true;

            EntityDERobot *head = self->head;
            head->active        = ACTIVE_NORMAL;
            head->visible       = true;
            head->rotation      = -128;

            EntityDERobot *shoulder = self->shoulderFront;
            shoulder->active        = ACTIVE_NORMAL;
            shoulder->visible       = true;

            EntityDERobot *armBack = self->arms[0];
            armBack->active        = ACTIVE_NORMAL;
            armBack->visible       = true;

            EntityDERobot *handBack = self->arms[1];
            handBack->active        = ACTIVE_NORMAL;
            handBack->visible       = true;

            EntityDERobot *armFront = self->arms[2];
            armFront->active        = ACTIVE_NORMAL;
            armFront->visible       = true;

            EntityDERobot *handFront = self->arms[3];
            handFront->active        = ACTIVE_NORMAL;
            handFront->visible       = true;

            EntityEggman *eggman = self->eggman;
            RSDK.ResetEntity(eggman, Eggman->classID, self);
            eggman->state    = Eggman_State_ProcessAnimation;
            eggman->animID   = 0;
            eggman->offset.x = -0x40000;
            eggman->offset.y = -0x200000;
        }
    }
    else if (player1->position.x > 0x800000 + self->position.x && player1->onGround) {
        RSDK_GET_ENTITY(SLOT_CAMERA1, Camera)->boundsOffset.x = 1;

        ++self->timer;
        if (player1->characterID == ID_TAILS)
            player1->stateAbility = StateMachine_None;
    }

    foreach_active(Player, player)
    {
        if (player->state == Player_State_TubeRoll || player->state == Player_State_TubeAirRoll)
            player->state = Player_State_Air;
    }
}

void DERobot_State_Target(void)
{
    RSDK_THIS(DERobot);

    RSDK.ProcessAnimation(&self->altAnimator);
    RSDK.ProcessAnimation(&self->armAnimator);

    if (self->parent) {
        self->position.x = self->parent->position.x;
        self->position.y = self->parent->position.y;
    }

    self->alpha += 0x20;
    self->offset.x -= self->velocity.x;
    if (self->offset.x <= 0xC0000) {
        self->alpha    = 0;
        self->offset.x = 0x2C0000;
    }

    self->offset.y = self->offset.x;
    if (++self->timer == 60)
        RSDK.SetSpriteAnimation(DERobot->aniFrames, 8, &self->armAnimator, true, 0);

    if (self->timer == 96) {
        self->parent = 0;
        foreach_active(DERobot, robot)
        {
            if (!robot->aniID) {
                robot->offset.x = self->position.x;
                robot->offset.y = self->position.y;
            }
        }
    }

    if (self->timer == 128)
        destroyEntity(self);
}

void DERobot_State_SurpriseFall(void)
{
    RSDK_THIS(DERobot);

    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;

    DERobot_HandleLegMovement(0);
    DERobot_HandleLegMovement(3);

    if (self->legs[2]->onGround) {
        self->angleVel = -0x10000;
        self->state    = DERobot_State_FallLand;
        RSDK.SetSpriteAnimation(Eggman->aniFrames, 2, &self->eggman->animator, true, 0);
        self->eggman->state = Eggman_State_ProcessThenSet;
        Camera_ShakeScreen(0, 0, 8);
        RSDK.PlaySfx(DERobot->sfxLedgeBreak, false, 255);
    }

    DERobot_HandleArmMovement(0);
    DERobot_HandleArmMovement(2);
}

void DERobot_State_FallLand(void)
{
    RSDK_THIS(DERobot);

    EntityDERobot *kneeBack  = self->legs[0];
    EntityDERobot *kneeFront = self->legs[3];

    kneeBack->angle += self->angleVel >> 12;
    if (kneeBack->angle <= 0) {
        self->angleVel += 2048;
    }
    else {
        kneeBack->angle     = 0;
        self->angleVel      = 0;
        self->movingSide    = 0;
        kneeBack->angleVel  = 0x10000;
        kneeBack->angleInc  = 0;
        kneeFront->angleVel = 0x10000;
        kneeFront->angleInc = 0;
        self->kneeAngleVel  = 8;
        self->angleVelStart = 0x10000;
        self->angleMaxInc   = -0x1000;
        RSDK.SetSpriteAnimation(Eggman->aniFrames, 3, &self->eggman->animator, true, 0);
        self->eggman->state = Eggman_State_ProcessThenSet;
        self->head->state   = DERobot_State_CloseHeadHatch;
        self->state         = DERobot_State_Walk;
    }
    kneeFront->angle = kneeBack->angle;

    DERobot_HandleLegMovement2(1);
    DERobot_HandleLegMovement2(4);
    DERobot_HandleArmMovement(0);
    DERobot_HandleArmMovement(2);

    DERobot_CheckPlayerCollisions_Body();
}

void DERobot_State_Walk(void)
{
    RSDK_THIS(DERobot);

    int32 id = self->movingSide;

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    DERobot_HandleScreenBounds();

    ++self->timer;
    if (self->timer == 240) {
        EntityDERobot *robotPart = CREATE_ENTITY(DERobot, INT_TO_VOID(DEROBOT_TARGET_EDGE), self->position.x, 0x3080000);
        robotPart->parent        = (Entity *)player1;
        RSDK.PlaySfx(DERobot->sfxTargeting, false, 0xFF);
    }
    else if (self->timer == 340) {
        self->timer = 0;
        if (self->offset.x - self->position.x <= 0xD00000) {
            self->arms[3]->state = DERobot_State_ArmExtendPrepare;
            self->state          = DERobot_State_ArmAttack;
        }
        else {
            EntityDERobot *robotPart = CREATE_ENTITY(DERobot, INT_TO_VOID(DEROBOT_BOMB), self->position.x - 0x360000, self->position.y - 0x60000);
            robotPart->offset.x      = (self->offset.x >> 1) + ((self->position.x - 0x360000) >> 1);
            robotPart->offset.y      = robotPart->position.y;
            robotPart->scale.y       = 0x2000;
            robotPart->scale.x       = (self->offset.x - self->position.x + 0x360000) >> 11;
            RSDK.PlaySfx(DERobot->sfxDrop, false, 255);
        }
    }

    if (self->movingSide == 3) {
        self->arms[0]->angle += (-112 - self->arms[0]->angle) >> 4;
        self->arms[1]->angle += (64 - self->arms[1]->angle) >> 4;
        if (self->timer >= 240 && player1->position.x - self->position.x <= 0xD00000) {
            int32 armAngle =
                4 * RSDK.ATan2((player1->position.x - self->arms[3]->position.x) >> 16, (player1->position.y - self->arms[3]->position.y) >> 16);

            if (armAngle > 0x200)
                armAngle = armAngle - 0x400;

            armAngle        = CLAMP(armAngle, -0x60, 0x80);
            int32 armAngle2 = MIN(self->arms[2]->angle + (-(self->arms[2]->angle + armAngle) >> 3), 0x80);

            self->arms[2]->angle = armAngle2;
            self->arms[3]->angle += (armAngle - self->arms[3]->angle) >> 3;
        }
        else {
            self->arms[2]->angle += (0x70 - self->arms[2]->angle) >> 4;
            self->arms[3]->angle += (-0x20 - self->arms[3]->angle) >> 4;
        }
    }
    else {
        self->arms[0]->angle += (112 - self->arms[0]->angle) >> 4;
        self->arms[1]->angle += (-32 - self->arms[1]->angle) >> 4;
        if (self->timer >= 240 && player1->position.x - self->position.x <= 0xD00000) {
            int32 armAngle =
                4 * RSDK.ATan2((player1->position.x - self->arms[3]->position.x) >> 16, (player1->position.y - self->arms[3]->position.y) >> 16);
            if (armAngle > 0x200)
                armAngle = armAngle - 0x400;
            armAngle = MIN(armAngle, 0x80);

            int32 armAngle2 = MIN(self->arms[2]->angle + (-(self->arms[2]->angle + armAngle) >> 3), 0x80);

            self->arms[2]->angle = armAngle2;
            self->arms[3]->angle += (armAngle - self->arms[3]->angle) >> 3;
        }
        else {
            self->arms[2]->angle += (-112 - self->arms[2]->angle) >> 4;
            self->arms[3]->angle += (64 - self->arms[3]->angle) >> 4;
        }
    }

    DERobot_HandleArmMovement(0);
    DERobot_HandleArmMovement(2);

    EntityDERobot *knee = self->legs[id];
    EntityDERobot *leg  = self->legs[((id + 3) % 6) + 0];
    EntityDERobot *foot = self->legs[((id + 3) % 6) + 2];
    knee->angle -= self->kneeAngleVel;
    DERobot_HandleLegMovement2(id + 1);

    leg->angle += leg->angleVel >> 12;
    leg->angleVel += leg->angleInc;
    if (leg->angle > 176)
        leg->angleInc = self->angleMaxInc;

    DERobot_HandleLegMovement((id + 3) % 6);

    if (foot->onGround && (-knee->angle >> 6) > 0) {
        self->movingSide = (self->movingSide + 3) % 6;
        leg->angleVel    = self->angleVelStart;
        leg->angleInc    = 0;
        Camera_ShakeScreen(0, 0, 4);
        RSDK.PlaySfx(DERobot->sfxImpact, false, 255);
        if (self->position.x > 0x4D800000) {
            self->health = 1;
            DERobot_Hit();
            foreach_active(CollapsingPlatform, platform) { platform->stoodPos.x = platform->position.x; }
        }
    }

    DERobot_CheckPlayerCollisions_Body();
}

void DERobot_State_ArmAttack(void)
{
    RSDK_THIS(DERobot);

    DERobot_HandleScreenBounds();
    EntityDERobot *armBack = self->arms[0];
    if (self->movingSide == 3) {
        armBack->angle += (-112 - armBack->angle) >> 4;
        self->arms[1]->angle += (64 - self->arms[1]->angle) >> 4;
    }
    else {
        armBack->angle += (112 - armBack->angle) >> 4;
        self->arms[1]->angle += (-32 - self->arms[1]->angle) >> 4;
    }

    if (self->arms[3]->state == DERobot_State_ArmRetracting)
        self->state = DERobot_State_Walk;

    DERobot_HandleArmMovement(0);
    DERobot_HandleArmMovement(2);

    EntityDERobot *knee = self->legs[self->movingSide];
    knee->angle -= self->kneeAngleVel;
    DERobot_HandleLegMovement2(self->movingSide + 1);

    EntityDERobot *leg  = self->legs[((self->movingSide + 3) % 6) + 0];
    EntityDERobot *foot = self->legs[((self->movingSide + 3) % 6) + 2];

    leg->angle += leg->angleVel >> 12;
    leg->angleVel += leg->angleInc;
    if (leg->angle > 176)
        leg->angleInc = self->angleMaxInc;

    DERobot_HandleLegMovement((self->movingSide + 3) % 6);

    if (foot->onGround && (-knee->angle >> 6) > 0) {
        self->movingSide = (self->movingSide + 3) % 6;
        leg->angleVel    = self->angleVelStart;
        leg->angleInc    = 0;
        Camera_ShakeScreen(0, 0, 4);
        RSDK.PlaySfx(DERobot->sfxImpact, false, 0xFF);
        if (self->position.x > 0x4E000000) {
            self->health = 1;
            DERobot_Hit();
        }
    }

    DERobot_CheckPlayerCollisions_Body();
}

void DERobot_State_Explode(void)
{
    RSDK_THIS(DERobot);

    DERobot_Explode();

    if (++self->timer == 90) {
        foreach_active(DERobot, robot)
        {
            switch (robot->aniID) {
                case DEROBOT_BODY: break;
                case DEROBOT_HEAD:
                    robot->velocity.x = -0x10000;
                    robot->velocity.y = -0x40000;
                    robot->angleVel   = -8;
                    robot->drawFX     = FX_ROTATE;
                    robot->state      = DERobot_State_DebrisFall;
                    robot->stateDraw  = DERobot_Draw_Simple;
                    break;

                case DEROBOT_ARM:
                case DEROBOT_LEG:
                case DEROBOT_SPIKES:
                case DEROBOT_ARM_THREAD:
                case DEROBOT_TARGET_EDGE:
                case DEROBOT_TARGET_CENTER:
                    robot->velocity.x = RSDK.Rand(-0x40000, 0x40000);
                    robot->velocity.y = RSDK.Rand(-0x40000, -0x10000);
                    robot->angleVel   = RSDK.Rand(-16, 16);
                    robot->drawFX     = FX_ROTATE;
                    robot->state      = DERobot_State_DebrisFall;
                    if (robot->stateDraw == DERobot_Draw_RelativeToParent)
                        robot->stateDraw = DERobot_Draw_Simple;
                    break;
            }
        }

        RSDK.SetSpriteAnimation(Eggman->aniFrames, 4, &self->eggman->animator, true, 0);
        self->timer      = 0;
        self->offset.x   = 0;
        self->velocity.y = -0x40000;
        self->state      = DERobot_State_ExplodeTerrain;
        RSDK.PlaySfx(DERobot->sfxLedgeBreak, false, 0xFF);
    }
}

void DERobot_State_ExplodeTerrain(void)
{
    RSDK_THIS(DERobot);

    DERobot_Explode();

    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x280000, true)) {
        DERobot_DestroyTerrainFinal();
        RSDK.PlaySfx(DERobot->sfxLedgeBreak, false, 255);
        self->state      = DERobot_State_Finish;
        self->velocity.y = -0x10000 - (self->velocity.y >> 1);
        Camera_ShakeScreen(0, 0, 8);
    }

    if (self->offset.x < 0x480000)
        self->offset.x += 0x40000;

    foreach_active(Player, player)
    {
        if (player->position.x < self->offset.x + self->position.x) {
            player->position.x = self->offset.x + self->position.x;
            player->velocity.x = 0x40000;
            player->groundVel  = 0x40000;
        }
    }
}

void DERobot_State_Finish(void)
{
    RSDK_THIS(DERobot);

    self->velocity.y += 0x2000;
    self->position.y += self->velocity.y;

    if (++self->timer == 96)
        RSDK.PlaySfx(DERobot->sfxDrop, false, 255);

    if (self->timer == 180) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);

        EntityEggPrison *prison = (EntityEggPrison *)self->eggman;
        RSDK.ResetEntity(prison, EggPrison->classID, INT_TO_VOID(EGGPRISON_FLYING));
        prison->position.x          = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
        prison->checkTileCollisions = true;
        prison->position.y          = (ScreenInfo->position.y - 48) << 16;

        foreach_all(BoundsMarker, marker) { destroyEntity(marker); }
        self->state = DERobot_State_FinishBounds;
    }

    if (!(Zone->timer & 7)) {
        int32 x                    = (RSDK.Rand(-48, 48) << 16) + self->position.x;
        int32 y                    = (RSDK.Rand(-48, 48) << 16) + self->position.y;
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSSPUFF), x, y);
        explosion->drawGroup       = Zone->objectDrawGroup[1];
    }

    foreach_active(Player, player)
    {
        if (player->position.x < self->position.x + 0x480000) {
            player->position.x = self->position.x + 0x480000;
            player->velocity.x = 0;
            player->groundVel  = 0;
            player->pushing    = false;
        }
    }
}

void DERobot_State_DebrisFall(void)
{
    RSDK_THIS(DERobot);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    self->rotation += self->angleVel;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void DERobot_State_FinishBounds(void)
{
    RSDK_THIS(DERobot);

    foreach_active(Player, player)
    {
        if (player->position.x < self->position.x + 0x480000) {
            player->position.x = self->position.x + 0x480000;
            player->velocity.x = 0;
            player->groundVel  = 0;
            player->pushing    = false;
        }

        if (player->position.x > 0x4E800000) {
            player->position.x = 0x4E800000;
            player->velocity.x = 0;
            player->groundVel  = 0;
            player->pushing    = false;
        }
    }
}

void DERobot_State_CutsceneExplode(void)
{
    RSDK_THIS(DERobot);

    if (!(Zone->timer & 0x3F)) {
        int32 x                    = (RSDK.Rand(-32, 32) << 16) + self->position.x;
        int32 y                    = (RSDK.Rand(-32, 32) << 16) + self->position.y;
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSSPUFF), x, y);
        explosion->drawGroup       = Zone->objectDrawGroup[1];
    }
}

#if GAME_INCLUDE_EDITOR
void DERobot_EditorDraw(void)
{
    RSDK_THIS(DERobot);

    self->drawFX = FX_NONE;
    self->parent = (Entity *)self;

    switch (self->aniID) {
        case DEROBOT_BODY:
            self->stateDraw = DERobot_Draw_Simple;
            RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->mainAnimator, true, self->frameID);

            if (showGizmos()) {
                RSDK_DRAWING_OVERLAY(true);
                DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER + 128, -SCREEN_YSIZE, WIDE_SCR_XCENTER + 128, 0, 1 | 0 | 4 | 8, 0x00C0F0);

                int32 slot = RSDK.GetEntitySlot(self);
                for (int32 i = -7; i < 7; ++i) {
                    if (i == 0) // thats this object lol
                        continue;

                    EntityDERobot *child = RSDK_GET_ENTITY(slot + i, DERobot);

                    if (child)
                        DrawHelpers_DrawArrow(self->position.x, self->position.y, child->position.x, child->position.y, 0xFFFF00, INK_NONE, 0xFF);
                }

                RSDK_DRAWING_OVERLAY(false);
            }
            break;

        case DEROBOT_HEAD:
            self->stateDraw = DERobot_Draw_RelativeToParent;
            self->drawFX    = FX_ROTATE;
            self->offset.x  = -0x160000;
            self->offset.y  = -0x240000;
            RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->mainAnimator, true, self->frameID);
            break;

        case DEROBOT_ARM:
            if (self->frameID == 2) {
                RSDK.SetSpriteAnimation(DERobot->aniFrames, 5, &self->altAnimator, true, 0);
                RSDK.SetSpriteAnimation(DERobot->aniFrames, 4, &self->armAnimator, true, 0);
                self->stateDraw = DERobot_Draw_Arm;
                self->drawFX    = FX_ROTATE;
                self->state     = DERobot_State_ArmIdle;
            }
            else if (self->frameID) {
                self->stateDraw = DERobot_Draw_Simple;
            }
            else {
                self->stateDraw = DERobot_Draw_RelativeToParent;
                self->offset.x  = -0xC0000;
                self->offset.y  = -0x100000;
            }
            RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->mainAnimator, true, self->frameID);
            break;

        case DEROBOT_LEG:
            if (self->frameID) {
                self->stateDraw = DERobot_Draw_Simple;
            }
            else {
                RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->altAnimator, true, 1);
                self->stateDraw = DERobot_Draw_FrontLeg;
            }

            if (self->frameID > 1)
                self->drawFX = FX_ROTATE;
            RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->mainAnimator, true, self->frameID);
            break;

        case DEROBOT_TARGET_EDGE:
            self->drawFX = FX_FLIP;
            RSDK.SetSpriteAnimation(DERobot->aniFrames, 7, &self->altAnimator, true, 0);
            self->stateDraw = DERobot_Draw_Target;
            RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->mainAnimator, true, self->frameID);
            break;

        default:
            self->stateDraw = DERobot_Draw_Simple;
            RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->mainAnimator, true, self->frameID);
            break;
    }

    StateMachine_Run(self->stateDraw);
}

void DERobot_EditorLoad(void)
{
    DERobot->aniFrames = RSDK.LoadSpriteAnimation("GHZ/DERobot.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(DERobot, aniID);
    RSDK_ENUM_VAR("Body", DEROBOT_BODY);
    RSDK_ENUM_VAR("Head", DEROBOT_HEAD);
    RSDK_ENUM_VAR("Arm", DEROBOT_ARM);
    RSDK_ENUM_VAR("Leg", DEROBOT_LEG);
    RSDK_ENUM_VAR("Spikes", DEROBOT_SPIKES);
    RSDK_ENUM_VAR("Arm Thread", DEROBOT_ARM_THREAD);
    RSDK_ENUM_VAR("Target Edge", DEROBOT_TARGET_EDGE);
    RSDK_ENUM_VAR("Target Center", DEROBOT_TARGET_CENTER);
    RSDK_ENUM_VAR("Target Lock", DEROBOT_TARGET_LOCK);
    RSDK_ENUM_VAR("Bomb", DEROBOT_BOMB);
    RSDK_ENUM_VAR("Body (Cutscene)", DEROBOT_BODY_CUTSCENE);
}
#endif

void DERobot_Serialize(void)
{
    RSDK_EDITABLE_VAR(DERobot, VAR_ENUM, aniID);
    RSDK_EDITABLE_VAR(DERobot, VAR_ENUM, frameID);
}
