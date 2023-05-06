// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Splats Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSplats *Splats;

void Splats_Update(void)
{
    RSDK_THIS(Splats);
    StateMachine_Run(self->state);
}

void Splats_LateUpdate(void) {}

void Splats_StaticUpdate(void) {}

void Splats_Draw(void)
{
    RSDK_THIS(Splats);

    RSDK.DrawSprite(&self->splashAnimator, NULL, false);
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

void Splats_Create(void *data)
{
    RSDK_THIS(Splats);

    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->visible       = true;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (self->bounceCount < 1)
        self->bounceCount = 1;

    if (Splats->initialState == Splats_State_BounceAround) {
        self->drawFX |= FX_FLIP;
        self->drawGroup = Zone->objectDrawGroup[0];
        RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &self->mainAnimator, true, 0);
        self->state = Splats_State_Init;
    }
    else {
        switch (VOID_TO_INT(data)) {
            case SPLATS_SPAWNER:
                self->drawGroup = Zone->objectDrawGroup[1];
                self->delay     = 0;
                RSDK.SetSpriteAnimation(Splats->aniFrames, 1, &self->mainAnimator, true, 0);
                self->state = Splats_State_Init;
                break;

            case SPLATS_INKSPLATS:
                self->drawFX |= FX_FLIP;
                self->drawGroup = Zone->objectDrawGroup[0];
                self->active    = ACTIVE_NORMAL;
                RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &self->mainAnimator, true, 0);
                self->mainAnimator.loopIndex  = 1;
                self->state                   = Splats_State_JumpOutOfJar;
                self->mainAnimator.frameCount = 2;
                break;

            case SPLATS_SPLAT:
                self->drawFX |= FX_FLIP;
                self->inkEffect |= INK_ALPHA;
                self->alpha     = 256;
                self->drawGroup = Zone->objectDrawGroup[1];
                self->active    = ACTIVE_NORMAL;
                RSDK.SetSpriteAnimation(Splats->aniFrames, 3, &self->mainAnimator, true, 0);
                self->state = Splats_State_InkSplat;
                break;
        }
    }
}

void Splats_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("GHZ")) {
        Splats->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Splats.bin", SCOPE_STAGE);

        Splats->initialState = Splats_State_BounceAround;
    }
    else if (RSDK.CheckSceneFolder("PSZ1")) {
        Splats->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Splats.bin", SCOPE_STAGE);

        Splats->initialState = Splats_State_InkJarSpawner;
    }

    Splats->hitboxBadnikGHZ.left   = -10;
    Splats->hitboxBadnikGHZ.top    = -20;
    Splats->hitboxBadnikGHZ.right  = 6;
    Splats->hitboxBadnikGHZ.bottom = 20;

    Splats->hitboxJar.left   = -18;
    Splats->hitboxJar.top    = -26;
    Splats->hitboxJar.right  = 18;
    Splats->hitboxJar.bottom = 17;

    Splats->hitboxBadnikPGZ.left   = -18;
    Splats->hitboxBadnikPGZ.top    = -146;
    Splats->hitboxBadnikPGZ.right  = 18;
    Splats->hitboxBadnikPGZ.bottom = -18;

    DEBUGMODE_ADD_OBJ(Splats);

    Splats->sfxSplatsSpawn = RSDK.GetSfx("PSZ/SplatsSpawn.wav");
    Splats->sfxSplatsLand  = RSDK.GetSfx("PSZ/SplatsLand.wav");
}

void Splats_DebugDraw(void)
{
    if (Splats->initialState == Splats_State_BounceAround)
        RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &DebugMode->animator, true, 0);
    else
        RSDK.SetSpriteAnimation(Splats->aniFrames, 1, &DebugMode->animator, true, 0);

    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Splats_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Splats, NULL, self->position.x, self->position.y);
}

void Splats_CheckPlayerCollisions(void)
{
    RSDK_THIS(Splats);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Splats->hitboxBadnikGHZ)) {
            if (Splats->initialState == Splats_State_BounceAround) {
                Player_CheckBadnikBreak(player, self, true);
            }
            else {
                if (Player_CheckBadnikBreak(player, self, false)) {
                    EntitySplats *parent = self->parent;
                    if (parent) {
                        if (parent->classID == Splats->classID)
                            --parent->activeCount;
                    }
                    destroyEntity(self);
                    foreach_break;
                }
            }
        }
    }
}

void Splats_CheckOffScreen(void)
{
    RSDK_THIS(Splats);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position   = self->startPos;
        self->direction  = self->startDir;
        self->isOnScreen = false;
        Splats_Create(NULL);
    }
}

void Splats_State_Init(void)
{
    RSDK_THIS(Splats);
    self->active     = ACTIVE_NORMAL;
    self->velocity.x = -0x10000;

    self->state = Splats->initialState;
    StateMachine_Run(self->state);
}

void Splats_State_BounceAround(void)
{
    RSDK_THIS(Splats);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.y > 0 && RSDK.ObjectTileCollision(self, Zone->collisionLayers, 0, 0, 0, 0x100000, true)) {
        if (self->bounceCount && ++self->activeCount >= self->bounceCount) {
            self->activeCount = 0;
            self->direction ^= FLIP_X;
            self->velocity.x = -self->velocity.x;
        }
        self->position.y -= 0x80000;
        self->velocity.y = -0x40000;
    }

    self->mainAnimator.frameID = self->velocity.y < 0;

    Splats_CheckPlayerCollisions();
    Splats_CheckOffScreen();
}

void Splats_State_SetupInkJar(void)
{
    RSDK_THIS(Splats);

    self->isOnScreen = false;
    self->delay      = 0;
    self->active     = ACTIVE_NORMAL;

    self->state = Splats_State_InkJarSpawner;
    Splats_State_InkJarSpawner();
}

void Splats_State_InkJarSpawner(void)
{
    RSDK_THIS(Splats);

    if (!self->isOnScreen) {
        if (!self->onScreen)
            return;
        self->isOnScreen = true;
    }

    RSDK.ProcessAnimation(&self->splashAnimator);

    bool32 disableSpawn = false;
    foreach_active(Player, player)
    {
        Player_CheckCollisionBox(player, self, &Splats->hitboxJar);
        disableSpawn |= Player_CheckCollisionTouch(player, self, &Splats->hitboxBadnikPGZ);
        disableSpawn |= (player->animator.animationID == ANI_SPINDASH);
    }

    if (--self->delay <= 0) {
        if (disableSpawn || (self->activeCount >= self->numActive)) {
            self->delay++;
        }
        else {
            RSDK.SetSpriteAnimation(Splats->aniFrames, 2, &self->splashAnimator, true, 0);
            self->delay = self->minDelay;
            RSDK.PlaySfx(Splats->sfxSplatsSpawn, false, 0xFF);

            EntitySplats *splats = CREATE_ENTITY(Splats, INT_TO_VOID(SPLATS_INKSPLATS), self->position.x, self->position.y - 0x60000);
            splats->parent       = self;
            splats->bounceCount  = self->bounceCount;
            splats->direction    = self->direction;
            splats->velocity.y   = -0x60000;
            if (self->direction == FLIP_NONE)
                splats->velocity.x = -0x10000;
            else
                splats->velocity.x = 0x10000;

            ++self->activeCount;
        }
    }

    if (!RSDK.CheckOnScreen(self, NULL)) {
        self->state  = Splats_State_SetupInkJar;
        self->active = ACTIVE_NORMAL;
    }
}

void Splats_State_JumpOutOfJar(void)
{
    RSDK_THIS(Splats);

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.y > -0x40000)
        self->position.x += self->velocity.x;

    if (self->velocity.y > 0) {
        self->mainAnimator.loopIndex  = 4;
        self->mainAnimator.frameCount = 5;
        if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, 0, 0, 0, 0x120000, true)) {
            RSDK.PlaySfx(Splats->sfxSplatsLand, false, 255);
            EntitySplats *splat = CREATE_ENTITY(Splats, INT_TO_VOID(SPLATS_SPLAT), self->position.x, self->position.y);
            splat->direction    = self->direction;
            self->delay         = 4;
            self->state         = Splats_State_HandleLanding;
        }
    }
    Splats_CheckPlayerCollisions();
}

void Splats_State_HandleBouncing(void)
{
    RSDK_THIS(Splats);

    RSDK.ProcessAnimation(&self->mainAnimator);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.y > 0) {
        self->mainAnimator.loopIndex  = 4;
        self->mainAnimator.frameCount = 5;
        if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, 0, 0, 0, 0x120000, true)) {
            RSDK.PlaySfx(Splats->sfxSplatsLand, false, 0xFF);
            if (self->bounceCount) {
                if (++self->activeCount < self->bounceCount) {
                    self->delay = 4;
                    self->state = Splats_State_HandleLanding;
                }
                else {
                    RSDK.SetSpriteAnimation(Splats->aniFrames, 4, &self->mainAnimator, true, 0);
                    self->state       = Splats_State_NoMoreJumps;
                    self->activeCount = 0;
                }
            }

            EntitySplats *splat = CREATE_ENTITY(Splats, INT_TO_VOID(SPLATS_SPLAT), self->position.x, self->position.y);
            splat->direction    = self->direction;
        }
    }

    if (RSDK.CheckOnScreen(self, NULL)) {
        Splats_CheckPlayerCollisions();
    }
    else {
        EntitySplats *parent = self->parent;
        if (parent && parent->classID == Splats->classID)
            --parent->activeCount;

        destroyEntity(self);
    }
}

void Splats_State_HandleLanding(void)
{
    RSDK_THIS(Splats);

    if (--self->delay <= 0) {
        RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &self->mainAnimator, true, 0);
        self->mainAnimator.loopIndex  = 1;
        self->mainAnimator.frameCount = 2;
        self->position.y -= 0x80000;
        self->velocity.y = -0x40000;
        self->state      = Splats_State_HandleBouncing;
    }

    if (RSDK.CheckOnScreen(self, NULL)) {
        Splats_CheckPlayerCollisions();
    }
    else {
        EntitySplats *parent = self->parent;
        if (parent && parent->classID == Splats->classID)
            --parent->activeCount;

        destroyEntity(self);
    }
}

void Splats_State_NoMoreJumps(void)
{
    RSDK_THIS(Splats);

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &self->mainAnimator, true, 0);
        self->position.y -= 0x80000;
        self->mainAnimator.frameCount = 2;
        self->mainAnimator.loopIndex  = 1;
        self->direction ^= FLIP_X;
        self->velocity.y = -0x40000;
        self->velocity.x = -self->velocity.x;
        self->state      = Splats_State_HandleBouncing;
    }

    if (RSDK.CheckOnScreen(self, NULL)) {
        Splats_CheckPlayerCollisions();
    }
    else {
        EntitySplats *parent = self->parent;
        if (parent && parent->classID == Splats->classID)
            --parent->activeCount;

        destroyEntity(self);
    }
}

void Splats_State_InkSplat(void)
{
    RSDK_THIS(Splats);
    RSDK.ProcessAnimation(&self->mainAnimator);

    if (self->delay >= 30) {
        self->alpha -= 2;
        if (!self->alpha)
            destroyEntity(self);
    }
    else {
        self->delay++;
    }
}

#if GAME_INCLUDE_EDITOR
void Splats_EditorDraw(void)
{
    RSDK_THIS(Splats);

    if (RSDK.CheckSceneFolder("GHZ")) {
        self->drawFX |= FX_FLIP;
        self->drawGroup = Zone->objectDrawGroup[0];
        RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &self->mainAnimator, true, 0);
    }
    else {
        self->drawFX    = FX_NONE;
        self->drawGroup = Zone->objectDrawGroup[1];
        RSDK.SetSpriteAnimation(Splats->aniFrames, 1, &self->mainAnimator, true, 0);
    }

    Splats_Draw();
}

void Splats_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("GHZ"))
        Splats->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Splats.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("PSZ1"))
        Splats->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Splats.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Splats, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void Splats_Serialize(void)
{
    RSDK_EDITABLE_VAR(Splats, VAR_UINT8, bounceCount);
    RSDK_EDITABLE_VAR(Splats, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Splats, VAR_UINT8, numActive);
    RSDK_EDITABLE_VAR(Splats, VAR_UINT16, minDelay);
}
