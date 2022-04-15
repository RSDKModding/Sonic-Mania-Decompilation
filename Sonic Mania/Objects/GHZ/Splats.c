// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Splats Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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

    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->startDir      = self->direction;
    self->visible       = true;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    if (self->bounceCount < 1)
        self->bounceCount = 1;

    if (Splats->state == Splats_State_BounceAround) {
        self->drawFX |= FX_FLIP;
        self->drawOrder = Zone->drawOrderLow;
        RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &self->mainAnimator, true, 0);
        self->state = Splats_State_Setup;
    }
    else {
        switch (voidToInt(data)) {
            case SPLATS_SPAWNER:
                self->drawOrder = Zone->drawOrderHigh;
                self->delay     = 0;
                RSDK.SetSpriteAnimation(Splats->aniFrames, 1, &self->mainAnimator, true, 0);
                self->state = Splats_State_Setup;
                break;

            case SPLATS_INKSPLATS:
                self->drawFX |= FX_FLIP;
                self->drawOrder = Zone->drawOrderLow;
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
                self->drawOrder = Zone->drawOrderHigh;
                self->active    = ACTIVE_NORMAL;
                RSDK.SetSpriteAnimation(Splats->aniFrames, 3, &self->mainAnimator, true, 0);
                self->state = Splats_State_InkSplat;
                break;
        }
    }
}

void Splats_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ")) {
        Splats->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Splats.bin", SCOPE_STAGE);
        Splats->state     = Splats_State_BounceAround;
    }
    else if (RSDK.CheckStageFolder("PSZ1")) {
        Splats->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Splats.bin", SCOPE_STAGE);
        Splats->state     = Splats_State_InkJarSpawner;
    }

    Splats->hitboxGHZBadnik.left   = -10;
    Splats->hitboxGHZBadnik.top    = -20;
    Splats->hitboxGHZBadnik.right  = 6;
    Splats->hitboxGHZBadnik.bottom = 20;

    Splats->hitboxJar.left   = -18;
    Splats->hitboxJar.top    = -26;
    Splats->hitboxJar.right  = 18;
    Splats->hitboxJar.bottom = 17;

    Splats->hitboxPGZBadnik.left   = -18;
    Splats->hitboxPGZBadnik.top    = -146;
    Splats->hitboxPGZBadnik.right  = 18;
    Splats->hitboxPGZBadnik.bottom = -18;

    DEBUGMODE_ADD_OBJ(Splats);
    Splats->sfxSplatsSpawn = RSDK.GetSfx("PSZ/SplatsSpawn.wav");
    Splats->sfxSplatsLand  = RSDK.GetSfx("PSZ/SplatsLand.wav");
}

void Splats_DebugDraw(void)
{
    if (Splats->state == Splats_State_BounceAround)
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
        if (Player_CheckBadnikTouch(player, self, &Splats->hitboxGHZBadnik)) {
            if (Splats->state == Splats_State_BounceAround) {
                Player_CheckBadnikBreak(player, self, true);
            }
            else {
                if (Player_CheckBadnikBreak(player, self, false)) {
                    EntitySplats *parent = (EntitySplats *)self->parent;
                    if (parent) {
                        if (parent->objectID == Splats->objectID)
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
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        self->isOnScreen = false;
        Splats_Create(NULL);
    }
}

void Splats_State_Setup(void)
{
    RSDK_THIS(Splats);
    self->active     = ACTIVE_NORMAL;
    self->velocity.x = -0x10000;
    self->state      = Splats->state;
    StateMachine_Run(self->state);
}

void Splats_State_BounceAround(void)
{
    RSDK_THIS(Splats);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.y > 0 && RSDK.ObjectTileCollision(self, Zone->fgLayers, 0, 0, 0, 0x100000, true)) {
        if (self->bounceCount) {
            if (++self->activeCount >= self->bounceCount) {
                self->activeCount = 0;
                self->direction ^= FLIP_X;
                self->velocity.x = -self->velocity.x;
            }
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
    self->state      = Splats_State_InkJarSpawner;
    Splats_State_InkJarSpawner();
}

void Splats_State_InkJarSpawner(void)
{
    RSDK_THIS(Splats);
    if (!self->isOnScreen) {
        if (!self->activeScreens)
            return;
        self->isOnScreen = true;
    }
    RSDK.ProcessAnimation(&self->splashAnimator);

    bool32 disableSpawn = false;
    foreach_active(Player, player)
    {
        Player_CheckCollisionBox(player, self, &Splats->hitboxJar);
        disableSpawn |= Player_CheckCollisionTouch(player, self, &Splats->hitboxPGZBadnik);
        disableSpawn |= (player->animator.animationID == ANI_SPINDASH);
    }

    if (--self->delay <= 0) {
        if (disableSpawn || (self->activeCount >= self->numActive)) {
            self->delay++;
        }
        else {
            RSDK.SetSpriteAnimation(Splats->aniFrames, 2, &self->splashAnimator, true, 0);
            self->delay = self->minDelay;
            RSDK.PlaySfx(Splats->sfxSplatsSpawn, false, 255);
            EntitySplats *child = CREATE_ENTITY(Splats, intToVoid(SPLATS_INKSPLATS), self->position.x, self->position.y - 0x60000);
            child->parent       = (Entity *)self;
            child->bounceCount  = self->bounceCount;
            child->direction    = self->direction;
            child->velocity.y   = -0x60000;
            if (self->direction == FLIP_NONE)
                child->velocity.x = -0x10000;
            else
                child->velocity.x = 0x10000;
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
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, 0, 0, 0, 0x120000, true)) {
            RSDK.PlaySfx(Splats->sfxSplatsLand, false, 255);
            EntitySplats *splats = CREATE_ENTITY(Splats, intToVoid(SPLATS_SPLAT), self->position.x, self->position.y);
            splats->direction    = self->direction;
            self->delay          = 4;
            self->state          = Splats_State_HandleLanding;
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
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, 0, 0, 0, 0x120000, true)) {
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

            EntitySplats *splats = CREATE_ENTITY(Splats, intToVoid(SPLATS_SPLAT), self->position.x, self->position.y);
            splats->direction    = self->direction;
        }
    }

    if (RSDK.CheckOnScreen(self, NULL)) {
        Splats_CheckPlayerCollisions();
    }
    else {
        EntitySplats *parent = (EntitySplats *)self->parent;
        if (parent) {
            if (parent->objectID == Splats->objectID)
                --parent->activeCount;
        }
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
        EntitySplats *parent = (EntitySplats *)self->parent;
        if (parent) {
            if (parent->objectID == Splats->objectID)
                --parent->activeCount;
        }
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
        EntitySplats *parent = (EntitySplats *)self->parent;
        if (parent) {
            if (parent->objectID == Splats->objectID)
                --parent->activeCount;
        }
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

#if RETRO_INCLUDE_EDITOR
void Splats_EditorDraw(void)
{
    RSDK_THIS(Splats);

    if (RSDK.CheckStageFolder("GHZ")) {
        self->drawFX |= FX_FLIP;
        self->drawOrder = Zone->drawOrderLow;
        RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &self->mainAnimator, true, 0);
    }
    else {
        self->drawFX    = FX_NONE;
        self->drawOrder = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(Splats->aniFrames, 1, &self->mainAnimator, true, 0);
    }
    Splats_Draw();
}

void Splats_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        Splats->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Splats.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("PSZ1"))
        Splats->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Splats.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Splats, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void Splats_Serialize(void)
{
    RSDK_EDITABLE_VAR(Splats, VAR_UINT8, bounceCount);
    RSDK_EDITABLE_VAR(Splats, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Splats, VAR_UINT8, numActive);
    RSDK_EDITABLE_VAR(Splats, VAR_UINT16, minDelay);
}
