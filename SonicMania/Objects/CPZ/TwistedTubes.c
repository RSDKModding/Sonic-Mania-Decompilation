// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TwistedTubes Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTwistedTubes *TwistedTubes;

void TwistedTubes_Update(void)
{
    RSDK_THIS(TwistedTubes);

    StateMachine_Run(self->state);
}

void TwistedTubes_LateUpdate(void) {}

void TwistedTubes_StaticUpdate(void) {}

void TwistedTubes_Draw(void)
{
    RSDK_THIS(TwistedTubes);

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    RSDK.DrawSprite(&self->entranceAnimator, &drawPos, false);

    drawPos.y += 0x400000;
    RSDK.DrawSprite(&self->tubeAnimator, &drawPos, false);

    RSDK.DrawSprite(&self->topGlassAnimator, &drawPos, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->topGlassAnimator, &drawPos, false);

    drawPos.y += 0x200000;
    for (int32 h = 0; h < self->height; ++h) {
        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->tubeAnimator, &drawPos, false);

        RSDK.DrawSprite(&self->sideAnimator, &drawPos, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->sideAnimator, &drawPos, false);

        drawPos.y += 0x200000;
    }

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->tubeAnimator, &drawPos, false);

    RSDK.DrawSprite(&self->bottomGlassAnimator, &drawPos, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->bottomGlassAnimator, &drawPos, false);

    self->direction = FLIP_NONE;
}

void TwistedTubes_Create(void *data)
{
    RSDK_THIS(TwistedTubes);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        if (data) {
            self->active = ACTIVE_NORMAL;
            self->player = (EntityPlayer *)data;
            if (self->player->position.x > self->position.x)
                self->state = TwistedTubes_State_PlayerEntryR;
            else
                self->state = TwistedTubes_State_PlayerEntryL;
        }
        else {
            self->visible       = true;
            self->drawGroup     = Zone->objectDrawGroup[1];
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x800000;
            self->updateRange.y = (self->height + 4) << 21;

            RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->entranceAnimator, true, 0);
            RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->tubeAnimator, true, 1);
            RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->topGlassAnimator, true, 2);
            RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->sideAnimator, true, 3);
            RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->bottomGlassAnimator, true, 4);
            self->state = TwistedTubes_State_HandleInteractions;
        }
    }
}

void TwistedTubes_StageLoad(void)
{
    TwistedTubes->aniFrames = RSDK.LoadSpriteAnimation("CPZ/TwistedTubes.bin", SCOPE_STAGE);

    TwistedTubes->hitboxSolid.left   = -32;
    TwistedTubes->hitboxSolid.top    = -16;
    TwistedTubes->hitboxSolid.right  = 32;
    TwistedTubes->hitboxSolid.bottom = 16;

    TwistedTubes->hitboxEntryL.left   = -56;
    TwistedTubes->hitboxEntryL.top    = 24;
    TwistedTubes->hitboxEntryL.right  = -32;
    TwistedTubes->hitboxEntryL.bottom = 64;

    TwistedTubes->hitboxEntryR.left   = 32;
    TwistedTubes->hitboxEntryR.top    = 24;
    TwistedTubes->hitboxEntryR.right  = 56;
    TwistedTubes->hitboxEntryR.bottom = 64;

    TwistedTubes->sfxTravel = RSDK.GetSfx("Tube/Travel.wav");
}

void TwistedTubes_State_HandleInteractions(void)
{
    RSDK_THIS(TwistedTubes);

    foreach_active(Player, player)
    {
        if (player->state != Player_State_TransportTube) {
            Player_CheckCollisionBox(player, self, &TwistedTubes->hitboxSolid);
            int32 playerID = RSDK.GetEntitySlot(player);

            bool32 entered = false;
            if (Player_CheckCollisionTouch(player, self, &TwistedTubes->hitboxEntryL)) {
                EntityTwistedTubes *tube = CREATE_ENTITY(TwistedTubes, player, self->position.x, self->position.y + 0x300000);
                tube->isPermanent        = true;
                tube->height             = self->height + 2;
                entered                  = true;
            }
            else if (Player_CheckCollisionTouch(player, self, &TwistedTubes->hitboxEntryR)) {
                EntityTwistedTubes *tube = CREATE_ENTITY(TwistedTubes, player, self->position.x, self->position.y + 0x300000);
                tube->isPermanent        = true;
                tube->height             = self->height + 3;
                entered                  = true;
            }

            if (entered) {
                player->state                        = Player_State_TransportTube;
                player->drawGroup                    = 1;
                player->interaction                  = false;
                player->tileCollisions               = TILECOLLISION_NONE;
                player->onGround                     = false;
                player->velocity.x                   = 0;
                player->velocity.y                   = 0;
                player->nextAirState                 = StateMachine_None;
                player->nextGroundState              = StateMachine_None;
                TwistedTubes->playerActive[playerID] = true;
                RSDK.PlaySfx(TwistedTubes->sfxTravel, false, 255);
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                player->animator.speed = 240;
            }
        }
    }
}

void TwistedTubes_State_PlayerEntryL(void)
{
    RSDK_THIS(TwistedTubes);

    EntityPlayer *player = self->player;
    player->position.x += (self->position.x - player->position.x - 0x380000) >> 3;
    player->position.y += (self->position.y - player->position.y - 0x80000) >> 3;
    if (++self->timer == 8) {
        self->timer     = 0;
        self->direction = FLIP_NONE;
        self->state     = TwistedTubes_State_FirstLoopL;
    }
}

void TwistedTubes_State_PlayerEntryR(void)
{
    RSDK_THIS(TwistedTubes);

    EntityPlayer *player = self->player;
    player->position.x += (self->position.x - player->position.x + 0x380000) >> 3;
    player->position.y += (self->position.y - player->position.y - 0x80000) >> 3;

    if (++self->timer == 8) {
        self->timer     = 0;
        self->direction = FLIP_X;
        self->state     = TwistedTubes_State_FirstLoopR;
    }
}

void TwistedTubes_State_FirstLoopR(void)
{
    RSDK_THIS(TwistedTubes);

    EntityPlayer *player = self->player;
    int32 playerID       = RSDK.GetEntitySlot(player);

    self->angle += 8;
    player->position.x = self->position.x + 0x3800 * RSDK.Cos256(self->angle);
    player->position.y = self->position.y - (RSDK.Cos256(self->angle) << 12);
    player->velocity.x = -0x400000;
    player->velocity.y = 0x10000;

    if (self->angle >= 0x80) {
        self->position.y += 0x100000;
        self->angle = 0;
        self->state = TwistedTubes_State_TubeLoops;
    }

    if (!Player_CheckValidState(player)) {
        TwistedTubes->playerActive[playerID] = false;
        destroyEntity(self);
    }
}

void TwistedTubes_State_TubeLoops(void)
{
    RSDK_THIS(TwistedTubes);

    EntityPlayer *player = self->player;
    int32 playerID       = RSDK.GetEntitySlot(player);

    self->angle += 8;
    player->position.x = self->position.x - 0x3800 * RSDK.Cos256(self->angle);
    player->position.y = self->position.y - (RSDK.Cos256(self->angle) << 9);
    player->velocity.x = 0x40000;
    player->velocity.y = 0x10000;

    if (self->angle < 0x80) {
        if (self->angle >= 0x40 && self->direction == FLIP_X && self->height == 1) {
            player->position.y                   = self->position.y;
            player->velocity.x                   = 0xC0000;
            player->velocity.y                   = 0;
            self->state                          = TwistedTubes_State_ExitR;
            TwistedTubes->playerActive[playerID] = false;
        }
    }
    else {
        self->angle = 0;
        if (--self->height) {
            self->position.y += 0x100000;
            self->state = TwistedTubes_State_FirstLoopR;
        }
        else {
            player->position.y = self->position.y;
            player->velocity.y = 0;
            if (self->direction == FLIP_X)
                self->state = TwistedTubes_State_ExitR;
            else
                self->state = TwistedTubes_State_ExitL;
            TwistedTubes->playerActive[playerID] = false;
        }
    }

    if (!Player_CheckValidState(player)) {
        TwistedTubes->playerActive[playerID] = false;
        destroyEntity(self);
    }
}

void TwistedTubes_State_FirstLoopL(void)
{
    RSDK_THIS(TwistedTubes);

    EntityPlayer *player = self->player;

    self->angle += 8;
    player->position.x = self->position.x - (RSDK.Cos256(self->angle) * 0x3800);
    player->position.y = self->position.y - (RSDK.Cos256(self->angle) << 12);
    player->velocity.x = 0x40000;
    player->velocity.y = 0x10000;

    if (self->angle >= 0x80) {
        self->position.y += 0x200000;
        self->angle = 0;
        self->state = TwistedTubes_State_FirstLoopR;
    }

    if (!Player_CheckValidState(player)) {
        TwistedTubes->playerActive[RSDK.GetEntitySlot(player)] = false;
        destroyEntity(self);
    }
}

void TwistedTubes_State_ExitL(void)
{
    RSDK_THIS(TwistedTubes);

    EntityPlayer *player = self->player;
    player->position.y   = self->position.y;

    if (player->velocity.x <= -0x80000)
        destroyEntity(self);
    else
        player->velocity.x -= 0x8000;

    if (!Player_CheckValidState(player))
        destroyEntity(self);
}

void TwistedTubes_State_ExitR(void)
{
    RSDK_THIS(TwistedTubes);

    EntityPlayer *player = self->player;
    player->position.y   = self->position.y;

    if (player->velocity.x >= 0x80000)
        destroyEntity(self);
    else
        player->velocity.x += 0x8000;
    if (!Player_CheckValidState(player))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void TwistedTubes_EditorDraw(void)
{
    RSDK_THIS(TwistedTubes);

    self->updateRange.y = (self->height + 4) << 21;
    self->updateRange.x = 0x800000;

    RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->entranceAnimator, true, 0);
    RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->tubeAnimator, true, 1);
    RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->topGlassAnimator, true, 2);
    RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->sideAnimator, true, 3);
    RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->bottomGlassAnimator, true, 4);

    TwistedTubes_Draw();
}

void TwistedTubes_EditorLoad(void) { TwistedTubes->aniFrames = RSDK.LoadSpriteAnimation("CPZ/TwistedTubes.bin", SCOPE_STAGE); }
#endif

void TwistedTubes_Serialize(void) { RSDK_EDITABLE_VAR(TwistedTubes, VAR_UINT8, height); }
