// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Rexon Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectRexon *Rexon;

void Rexon_Update(void)
{
    RSDK_THIS(Rexon);

    StateMachine_Run(self->state);

    self->positions[REXON_SEGMENT_COUNT].x = self->position.x;
    self->positions[REXON_SEGMENT_COUNT].y = self->position.y;

    if (self->direction)
        self->positions[REXON_SEGMENT_COUNT].x += 0x1C0000;
    else
        self->positions[REXON_SEGMENT_COUNT].x -= 0x1C0000;
    self->positions[REXON_SEGMENT_COUNT].y += 0x110000;

    for (int32 i = REXON_SEGMENT_COUNT - 1; i >= 0; --i) {
        int32 y = self->segmentAmplitude[i] >> 1;
        if (self->direction == FLIP_X)
            y = (0x80 - y) & 0xFF;

        self->positions[i].x = self->segmentMagnitude[i] * RSDK.Cos256(y) + self->positions[i + 1].x;
        self->positions[i].y = self->segmentMagnitude[i] * RSDK.Sin256(y) + self->positions[i + 1].y;
    }
}

void Rexon_LateUpdate(void) {}

void Rexon_StaticUpdate(void) {}

void Rexon_Draw(void)
{
    RSDK_THIS(Rexon);

    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);

    if (self->state != Rexon_State_Destroyed && self->state != Rexon_State_Explode) {
        RSDK.DrawSprite(&self->headAnimator, self->positions, false);
        for (int32 i = 1; i < REXON_SEGMENT_COUNT; ++i) RSDK.DrawSprite(&self->neckAnimator, &self->positions[i], false);
    }
}

void Rexon_Create(void *data)
{
    RSDK_THIS(Rexon);

    self->drawFX |= FX_FLIP;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->visible       = true;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->type          = VOID_TO_INT(data);

    switch (self->type) {
        case REXON_MAIN:
            self->timer = 128;
            if (!self->noMove)
                self->velocity.x = -0x2000;

            for (int32 i = 0; i < REXON_SEGMENT_COUNT; ++i) {
                self->segmentMagnitude[i]  = 0;
                self->segmentAmplitude[i]  = 0x170;
                self->segmentDirections[i] = FLIP_NONE;
            }

            self->segmentID = 0;
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 3, &self->bodyAnimator, true, 0);
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 1, &self->headAnimator, true, 2);
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 2, &self->neckAnimator, true, 0);
            self->state = Rexon_State_Init;
            break;

        case REXON_DEBRIS:
            self->active = ACTIVE_NORMAL;
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 2, &self->bodyAnimator, true, 0);
            self->state = Rexon_State_Debris;
            break;

        case REXON_SHOT:
            self->active = ACTIVE_NORMAL;
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 4, &self->bodyAnimator, true, 0);
            self->state = Rexon_State_Projectile;
            break;
    }
}

void Rexon_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        Rexon->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Rexon.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ2") || RSDK.CheckSceneFolder("LRZ3"))
        Rexon->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Rexon.bin", SCOPE_STAGE);

    Rexon->hitboxBadnik.left   = -8;
    Rexon->hitboxBadnik.top    = -8;
    Rexon->hitboxBadnik.right  = 8;
    Rexon->hitboxBadnik.bottom = 8;

    Rexon->hitboxShell.left   = -16;
    Rexon->hitboxShell.top    = -8;
    Rexon->hitboxShell.right  = 16;
    Rexon->hitboxShell.bottom = 8;

    Rexon->hitboxRange.left   = -160;
    Rexon->hitboxRange.top    = -256;
    Rexon->hitboxRange.right  = 96;
    Rexon->hitboxRange.bottom = 256;

    Rexon->hitboxProjectile.left   = -4;
    Rexon->hitboxProjectile.top    = -4;
    Rexon->hitboxProjectile.right  = 4;
    Rexon->hitboxProjectile.bottom = 4;

    Rexon->sfxShot      = RSDK.GetSfx("Stage/Shot.wav");
    Rexon->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");

    DEBUGMODE_ADD_OBJ(Rexon);
}

void Rexon_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Rexon, NULL, self->position.x, self->position.y);
}

void Rexon_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Rexon->aniFrames, 3, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Rexon_CheckOffScreen(void)
{
    RSDK_THIS(Rexon);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Rexon_Create(NULL);
    }
}

void Rexon_CheckPlayerCollisions(void)
{
    RSDK_THIS(Rexon);

    if (self->state != Rexon_State_Explode) {
        int32 storeX = self->position.x;
        int32 storeY = self->position.y;

        foreach_active(Player, player)
        {
            // Provide a shell box for players to stand on
            Player_CheckCollisionBox(player, self, &Rexon->hitboxShell);

            if (self->state != Rexon_State_Destroyed) {

                // Check if the head's been hit
                self->position.x = self->positions[0].x;
                self->position.y = self->positions[0].y;
                if (Player_CheckBadnikTouch(player, self, &Rexon->hitboxBadnik)) {
                    if (Player_CheckBadnikBreak(player, self, false))
                        Rexon_Destroy(self, false);
                }
                else {
                    // Check hitboxes for all body parts individually, too
                    for (int32 i = 1; i < REXON_SEGMENT_COUNT; ++i) {
                        self->position.x = self->positions[i].x;
                        self->position.y = self->positions[i].y;
                        if (Player_CheckCollisionTouch(player, self, &Rexon->hitboxBadnik))
                            Player_Hurt(player, self);
                    }
                }

                self->position.x = storeX;
                self->position.y = storeY;
            }
        }
    }
}

void Rexon_Destroy(EntityRexon *rexon, bool32 crushed)
{
    if (rexon->state != Rexon_State_Destroyed && rexon->state != Rexon_State_Explode) {
        int32 x = rexon->position.x;
        int32 y = rexon->position.y;

        rexon->position.x = rexon->positions[0].x;
        rexon->position.y = rexon->positions[0].y;

        BadnikHelpers_BadnikBreak(rexon, false, true);

        rexon->position.x = x;
        rexon->position.y = y;

        int32 velocities[] = { -0x8000, 0x10000, -0x10000, 0x8000 };
        for (int32 i = 1; i < REXON_SEGMENT_COUNT; ++i) {
            EntityRexon *debris = CREATE_ENTITY(Rexon, INT_TO_VOID(REXON_DEBRIS), rexon->positions[i].x, rexon->positions[i].y);
            debris->direction   = rexon->direction;
            debris->velocity.x  = velocities[(i - 1) & 3];
        }
    }

    if (crushed) {
        rexon->active       = ACTIVE_NORMAL;
        rexon->destroyDelay = 60;
        rexon->state        = Rexon_State_Explode;
    }
    else {
        rexon->state = Rexon_State_Destroyed;
    }
}

void Rexon_State_Init(void)
{
    RSDK_THIS(Rexon);

    self->active = ACTIVE_NORMAL;

    self->state = Rexon_State_Hidden;
    Rexon_State_Hidden();
}

void Rexon_State_Hidden(void)
{
    RSDK_THIS(Rexon);

    if (self->noMove) {
        self->direction = Player_GetNearestPlayer()->position.x >= self->position.x;
    }
    else {
        self->position.x += self->velocity.x;
        if (--self->timer <= 0) {
            self->direction ^= FLIP_X;
            self->timer      = 128;
            self->velocity.x = -self->velocity.x;
        }
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Rexon->hitboxRange)) {
            self->state     = Rexon_State_Rising;
            self->direction = self->direction = player->position.x >= self->position.x;
            foreach_break;
        }
    }

    Rexon_CheckPlayerCollisions();
    Rexon_CheckOffScreen();
}

void Rexon_State_Rising(void)
{
    RSDK_THIS(Rexon);

    self->segmentMagnitude[self->segmentID] += 640;
    if (self->segmentMagnitude[self->segmentID] >= 0xF00) {
        self->segmentMagnitude[self->segmentID] = 0xF00;
        ++self->segmentID;
    }

    for (int32 i = self->segmentID - 1; i >= 0; --i) {
        self->segmentAmplitude[i] += 2;
    }

    if (self->segmentID == REXON_SEGMENT_COUNT)
        self->state = Rexon_State_Shooting;

    Rexon_CheckPlayerCollisions();
    Rexon_CheckOffScreen();
}

void Rexon_State_Shooting(void)
{
    RSDK_THIS(Rexon);

    RSDK.ProcessAnimation(&self->headAnimator);

    for (int32 i = 0; i < REXON_SEGMENT_COUNT; ++i) {
        if (self->segmentDirections[i]) {
            if (--self->segmentAmplitude[i] <= 0x160)
                self->segmentDirections[i] = FLIP_NONE;
        }
        else if (++self->segmentAmplitude[i] >= 0x1A0) {
            self->segmentDirections[i] = FLIP_X;
        }
    }

    if (self->segmentAmplitude[0] == 0x160) {
        RSDK.PlaySfx(Rexon->sfxShot, false, 255);

        EntityRexon *shot = CREATE_ENTITY(Rexon, INT_TO_VOID(REXON_SHOT), self->positions[0].x, self->positions[0].y);
        if (self->direction) {
            shot->position.x += 0xE0000;
            shot->velocity.x = 0x10000;
        }
        else {
            shot->position.x -= 0xE0000;
            shot->velocity.x = -0x10000;
        }

        shot->position.y += 0x60000;
        shot->velocity.y = 0x8000;
    }
    else if (self->segmentAmplitude[0] == 0x170) {
        if (self->segmentDirections[0] == FLIP_X)
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 0, &self->headAnimator, true, 0);
    }
    else if (self->segmentAmplitude[0] == 0x180 && self->segmentDirections[0] == FLIP_NONE) {
        RSDK.SetSpriteAnimation(Rexon->aniFrames, 1, &self->headAnimator, true, 0);
    }

    Rexon_CheckPlayerCollisions();
    Rexon_CheckOffScreen();
}

void Rexon_State_Destroyed(void)
{
    Rexon_CheckPlayerCollisions();
    Rexon_CheckOffScreen();
}

void Rexon_State_Explode(void)
{
    RSDK_THIS(Rexon);

    self->position.y += 0x4000;
    self->velocity.y = 0x4000;

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(Rexon->sfxExplosion, false, 255);

        if (Zone->timer & 4) {
            int32 x                    = self->position.x + (RSDK.Rand(Rexon->hitboxShell.left, Rexon->hitboxShell.right) << 16);
            int32 y                    = self->position.y + (RSDK.Rand(Rexon->hitboxShell.top, Rexon->hitboxShell.bottom) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawGroup       = Zone->objectDrawGroup[1];
        }
    }

    if (--self->destroyDelay <= 0)
        destroyEntity(self);
}

void Rexon_State_Debris(void)
{
    RSDK_THIS(Rexon);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->visible ^= true;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void Rexon_State_Projectile(void)
{
    RSDK_THIS(Rexon);

    RSDK.ProcessAnimation(&self->bodyAnimator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Rexon->hitboxProjectile)) {
            Player_ProjectileHurt(player, self);
        }
    }

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, Rexon->hitboxProjectile.bottom << 13, 4)
        || RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, Rexon->hitboxProjectile.left << 13, 0, 4)
        || RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_ROOF, 0, 0, Rexon->hitboxProjectile.top << 13, 4)
        || RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, Rexon->hitboxProjectile.right << 13, 0, 4)
        || !RSDK.CheckOnScreen(self, NULL)) {
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void Rexon_EditorDraw(void)
{
    RSDK_THIS(Rexon);

    for (int32 i = 0; i < REXON_SEGMENT_COUNT; ++i) {
        self->segmentMagnitude[i] = 0xF00;
        self->segmentAmplitude[i] = 0x170;
    }

    self->segmentID = 0;
    RSDK.SetSpriteAnimation(Rexon->aniFrames, 3, &self->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(Rexon->aniFrames, 1, &self->headAnimator, true, 2);
    RSDK.SetSpriteAnimation(Rexon->aniFrames, 2, &self->neckAnimator, true, 0);

    self->positions[REXON_SEGMENT_COUNT].x = self->position.x;
    self->positions[REXON_SEGMENT_COUNT].y = self->position.y;

    if (self->direction)
        self->positions[REXON_SEGMENT_COUNT].x += 0x1C0000;
    else
        self->positions[REXON_SEGMENT_COUNT].x -= 0x1C0000;
    self->positions[REXON_SEGMENT_COUNT].y += 0x110000;

    for (int32 i = REXON_SEGMENT_COUNT - 1; i >= 0; --i) {
        int32 y = self->segmentAmplitude[i] >> 1;
        if (self->direction == FLIP_X)
            y = (0x80 - y) & 0xFF;

        self->positions[i].x = self->segmentMagnitude[i] * RSDK.Cos256(y) + self->positions[i + 1].x;
        self->positions[i].y = self->segmentMagnitude[i] * RSDK.Sin256(y) + self->positions[i + 1].y;
    }

    Rexon_Draw();
}

void Rexon_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        Rexon->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Rexon.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ2") || RSDK.CheckSceneFolder("LRZ3"))
        Rexon->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Rexon.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Rexon, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void Rexon_Serialize(void)
{
    RSDK_EDITABLE_VAR(Rexon, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Rexon, VAR_BOOL, noMove);
}
