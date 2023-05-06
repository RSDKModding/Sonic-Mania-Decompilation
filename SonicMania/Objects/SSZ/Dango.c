// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Dango Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDango *Dango;

void Dango_Update(void)
{
    RSDK_THIS(Dango);

    StateMachine_Run(self->state);

    if (self->rollDelay > 0)
        self->rollDelay--;
}

void Dango_LateUpdate(void) {}

void Dango_StaticUpdate(void) {}

void Dango_Draw(void)
{
    RSDK_THIS(Dango);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Dango_Create(void *data)
{
    RSDK_THIS(Dango);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->drawFX |= FX_FLIP;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    RSDK.SetSpriteAnimation(Dango->aniFrames, 1, &self->animator, true, 0);
    self->state = Dango_State_Init;
}

void Dango_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        Dango->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Dango.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SSZ2"))
        Dango->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Dango.bin", SCOPE_STAGE);

    Dango->hitboxBadnik.top    = -14;
    Dango->hitboxBadnik.left   = -14;
    Dango->hitboxBadnik.right  = 14;
    Dango->hitboxBadnik.bottom = 14;

    Dango->hitboxCurlRange.top    = -128;
    Dango->hitboxCurlRange.left   = -112;
    Dango->hitboxCurlRange.right  = -18;
    Dango->hitboxCurlRange.bottom = 128;

    Dango->sfxBumper = RSDK.GetSfx("Stage/Bumper3.wav");

    DEBUGMODE_ADD_OBJ(Dango);
}

void Dango_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityDango *dango = CREATE_ENTITY(Dango, NULL, self->position.x, self->position.y);
    RSDK.SetSpriteAnimation(Dango->aniFrames, 2, &dango->animator, true, 0);

    dango->state     = Dango_State_Falling_Curled;
    dango->groundVel = dango->direction == FLIP_NONE ? -0x20000 : 0x20000;
}

void Dango_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Dango->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Dango_CheckOffScreen(void)
{
    RSDK_THIS(Dango);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Dango_Create(NULL);
    }
}

void Dango_CheckPlayerCollisions(void)
{
    RSDK_THIS(Dango);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Dango->hitboxBadnik)) {
            int32 anim = player->animator.animationID;

            bool32 bumpPlayer = anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH;
            bumpPlayer &= self->state == Dango_State_Rolling;
#if MANIA_USE_PLUS
            if (player->characterID == ID_MIGHTY)
                bumpPlayer |= anim == ANI_CROUCH || player->jumpAbilityState > 1;
            bumpPlayer &= player->state != Player_State_MightyHammerDrop;
#endif

            if (bumpPlayer) {
                if (anim != ANI_JUMP)
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                RSDK.PlaySfx(Dango->sfxBumper, false, 0xFF);

                if (player->velocity.x <= 0) {
                    if (self->groundVel > player->velocity.x) {
                        if (self->groundVel > -0x20000)
                            self->groundVel = -0x20000;
                    }
                }
                else if (self->groundVel < player->velocity.x) {
                    self->groundVel = player->velocity.x;
                    if (self->groundVel < 0x20000)
                        self->groundVel = 0x20000;
                }

                self->velocity.x = self->groundVel;

#if MANIA_USE_PLUS
                if (!(player->characterID == ID_MIGHTY && player->jumpAbilityState > 1)) {
#endif
                    player->groundVel  = (player->position.x - self->position.x) >> 2;
                    player->velocity.x = player->groundVel;
                    player->velocity.y = (player->position.y - self->position.y) >> 2;
                    player->onGround   = false;
#if MANIA_USE_PLUS
                }
#endif

                if ((self->position.x > player->position.x && self->velocity.x < 0)
                    || (self->position.x < player->position.x && self->velocity.x > 0)) {
                    self->groundVel  = -self->groundVel;
                    self->velocity.x = -self->velocity.x;
                }
            }
            else {
                Player_CheckBadnikBreak(player, self, true);
            }
        }
    }
}

bool32 Dango_HandleMovement(StateMachine(nextState), uint8 anim)
{
    RSDK_THIS(Dango);

    bool32 changeState = false;

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;
    self->position.x += self->groundVel;

    bool32 collidedWall = false;
    if (self->groundVel <= 0)
        collidedWall = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_RWALL, 0, Dango->hitboxBadnik.left << 16, 0, 4);
    else
        collidedWall = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_LWALL, 0, Dango->hitboxBadnik.right << 16, 0, 4);

    if (RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xD0000, 2)) {
        uint16 tile = RSDK.GetTile(Zone->fgLayer[0], self->position.x >> 16, (self->position.y + 0xD0000) >> 16);
        self->angle = RSDK.GetTileAngle(tile, 0, 0);

        if (self->groundVel <= 0) {
            if (self->angle < 0x80 && self->angle > 0x10)
                changeState = true;
            else
                self->velocity.x = (self->groundVel >> 8) * RSDK.Cos256(self->angle);
        }
        else {
            if ((uint32)(self->angle - 0x81) <= 0x6E)
                changeState = true;
            else
                self->velocity.x = (self->groundVel >> 8) * RSDK.Cos256(self->angle);
        }
    }
    else {
        changeState = true;
    }

    if (collidedWall)
        changeState = true;

    if (self->preventStateChange)
        changeState = false;

    if (changeState) {
        self->position.x = storeX;
        self->position.y = storeY;
        RSDK.SetSpriteAnimation(Dango->aniFrames, anim, &self->animator, true, 0);

        self->state              = nextState;
        self->preventStateChange = true;
    }

    return changeState;
}

void Dango_State_Init(void)
{
    RSDK_THIS(Dango);

    self->active     = ACTIVE_NORMAL;
    self->groundVel  = self->direction == FLIP_NONE ? -0x6000 : 0x6000;
    self->velocity.y = 0;

    self->state = Dango_State_Walking;
    Dango_State_Walking();
}

void Dango_State_Walking(void)
{
    RSDK_THIS(Dango);

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xD0000, false)) {
        RSDK.ProcessAnimation(&self->animator);

        Dango_CheckPlayerCollisions();
        Dango_CheckOffScreen();

        if (!Dango_HandleMovement(Dango_State_Turning, 5) && self->rollDelay <= 0) {
            foreach_active(Player, player)
            {
                if (Player_CheckCollisionTouch(player, self, &Dango->hitboxCurlRange)) {
                    RSDK.SetSpriteAnimation(Dango->aniFrames, 3, &self->animator, true, 0);
                    self->state = Dango_State_Curling;
                }
            }
        }
    }
    else {
        self->state = Dango_State_Falling_Uncurled;
        Dango_State_Falling_Uncurled();
    }
}

void Dango_State_Turning(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 1, &self->animator, true, 0);
        self->preventStateChange = false;
        self->groundVel          = -self->groundVel;
        self->velocity.x         = -self->velocity.x;

        self->state = Dango_State_Walking;
        self->direction ^= FLIP_X;
    }
    else {
        Dango_CheckPlayerCollisions();
        Dango_CheckOffScreen();
    }
}

void Dango_State_Falling_Uncurled(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->groundVel;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xD0000, true)) {
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(Dango->aniFrames, 1, &self->animator, true, 0);

        self->state = Dango_State_Walking;
    }
    else {
        Dango_CheckPlayerCollisions();
        Dango_CheckOffScreen();
    }
}

void Dango_State_Curling(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 2, &self->animator, true, 0);
        self->state     = Dango_State_Rolling;
        self->groundVel = self->direction == FLIP_NONE ? -0x20000 : 0x20000;
    }
    else {
        Dango_CheckPlayerCollisions();
        Dango_CheckOffScreen();
    }
}

void Dango_State_Rolling(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xD0000, false)) {
        Dango_CheckPlayerCollisions();
        Dango_CheckOffScreen();

        Dango_HandleMovement(Dango_State_Uncurling, 4);
    }
    else {
        self->state = Dango_State_Falling_Curled;
        Dango_State_Falling_Curled();
    }
}

void Dango_State_Falling_Curled(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->groundVel;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xD0000, true)) {
        self->velocity.y = 0;
        self->state      = Dango_State_Rolling;
    }
    else {
        Dango_CheckPlayerCollisions();
        Dango_CheckOffScreen();
    }
}

void Dango_State_Uncurling(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 1, &self->animator, true, 0);
        self->preventStateChange = false;
        self->rollDelay          = 30;
        self->groundVel          = self->direction == FLIP_NONE ? -0x6000 : 0x6000;
        self->state              = Dango_State_Walking;
    }
    else {
        Dango_CheckPlayerCollisions();
        Dango_CheckOffScreen();
    }
}

#if MANIA_USE_PLUS
void Dango_StateTaunt_Setup(void)
{
    RSDK_THIS(Dango);

    if (++self->timer == 180) {
        self->timer     = 0;
        self->groundVel = 0x60000;
        self->state     = Dango_StateTaunt_RollIn;

        RSDK.SetSpriteAnimation(Dango->aniFrames, 2, &self->animator, true, 0);
    }
}

void Dango_StateTaunt_RollIn(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->groundVel;
    RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xE0000, 14);

    foreach_active(PhantomRuby, ruby)
    {
        if (abs(self->position.x - ruby->position.x) < 0x120000) {
            RSDK.PlaySfx(Dango->sfxBumper, false, 255);
            self->state      = Dango_StateTaunt_KnockedRuby;
            ruby->state      = PhantomRuby_State_MoveGravity;
            ruby->velocity.x = self->groundVel;
            ruby->velocity.y = -0x80000;

            RSDK.SetSpriteAnimation(Dango->aniFrames, 4, &self->animator, true, 0);
        }
    }
}

void Dango_StateTaunt_KnockedRuby(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);

    self->groundVel -= self->groundVel >> 2;
    self->position.x += self->groundVel;

    if (self->groundVel < 0x8000) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 6, &self->animator, true, 0);
        self->state = Dango_StateTaunt_Taunting;
    }
}

void Dango_StateTaunt_Taunting(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1)
        ++self->timer;

    if (self->timer == 32) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 5, &self->animator, true, 0);
        self->timer = 0;
        self->state = Dango_StateTaunt_Turning;
    }
}

void Dango_StateTaunt_Turning(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 3, &self->animator, true, 0);
        self->direction = FLIP_NONE;
        self->state     = Dango_StateTaunt_RollOut;
    }
}

void Dango_StateTaunt_RollOut(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.animationID == 3) {
        if (self->animator.frameID == self->animator.frameCount - 1)
            RSDK.SetSpriteAnimation(Dango->aniFrames, 2, &self->animator, true, 0);
    }
    else {
        self->position.x -= 0x40000;
    }

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}
#endif

#if GAME_INCLUDE_EDITOR
void Dango_EditorDraw(void) { Dango_Draw(); }

void Dango_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        Dango->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Dango.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SSZ2"))
        Dango->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Dango.bin", SCOPE_STAGE);
}
#endif

void Dango_Serialize(void) {}
