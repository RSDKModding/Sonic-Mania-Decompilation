// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BuzzBomber Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBuzzBomber *BuzzBomber;

void BuzzBomber_Update(void)
{
    RSDK_THIS(BuzzBomber);
    StateMachine_Run(self->state);
}

void BuzzBomber_LateUpdate(void) {}

void BuzzBomber_StaticUpdate(void) {}

void BuzzBomber_Draw(void)
{
    RSDK_THIS(BuzzBomber);

    if (self->inkEffect == INK_ADD) {
        RSDK.DrawSprite(&self->animator, NULL, false);
    }
    else {
        RSDK.DrawSprite(&self->animator, NULL, false);

        self->inkEffect = INK_ALPHA;
        RSDK.DrawSprite(&self->wingAnimator, NULL, false);

        self->inkEffect = INK_NONE;
        RSDK.DrawSprite(&self->thrustAnimator, NULL, false);
    }
}

void BuzzBomber_Create(void *data)
{
    RSDK_THIS(BuzzBomber);
    self->visible = true;
    self->drawFX |= FX_FLIP;
    self->drawGroup      = Zone->objectDrawGroup[0];
    self->startPos.x     = self->position.x;
    self->startPos.y     = self->position.y;
    self->startDir       = self->direction;
    self->timer          = 128;
    self->detectedPlayer = false;
    self->projectile     = NULL;

    if (!self->shotRange)
        self->shotRange = 96;

    self->hitboxRange.right  = self->shotRange;
    self->hitboxRange.left   = -self->shotRange;
    self->hitboxRange.top    = -256;
    self->hitboxRange.bottom = 256;

    if (data) {
        self->inkEffect     = INK_ADD;
        self->alpha         = 0xC0;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x200000;
        self->updateRange.y = 0x200000;
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 4, &self->animator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->wingAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->thrustAnimator, true, 0);
        self->state = BuzzBomber_State_ProjectileCharge;
    }
    else {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 0, &self->animator, true, 0);
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 2, &self->wingAnimator, true, 0);
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 3, &self->thrustAnimator, true, 0);
        self->state = BuzzBomber_State_Init;
        self->alpha = 0xC0;
    }
}

void BuzzBomber_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("GHZ"))
        BuzzBomber->aniFrames = RSDK.LoadSpriteAnimation("GHZ/BuzzBomber.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("Blueprint"))
        BuzzBomber->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/BuzzBomber.bin", SCOPE_STAGE);

    BuzzBomber->hitboxBadnik.left   = -24;
    BuzzBomber->hitboxBadnik.top    = -12;
    BuzzBomber->hitboxBadnik.right  = 24;
    BuzzBomber->hitboxBadnik.bottom = 12;

    BuzzBomber->hitboxProjectile.left   = -6;
    BuzzBomber->hitboxProjectile.top    = -6;
    BuzzBomber->hitboxProjectile.right  = 6;
    BuzzBomber->hitboxProjectile.bottom = 6;

    DEBUGMODE_ADD_OBJ(BuzzBomber);
}

void BuzzBomber_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void BuzzBomber_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityBuzzBomber *buzzBomber = CREATE_ENTITY(BuzzBomber, NULL, self->position.x, self->position.y);

    buzzBomber->direction = self->direction;
    buzzBomber->startDir  = self->direction;
}

void BuzzBomber_CheckOffScreen(void)
{
    RSDK_THIS(BuzzBomber);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        BuzzBomber_Create(NULL);
    }
}

void BuzzBomber_CheckPlayerCollisions(void)
{
    RSDK_THIS(BuzzBomber);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &BuzzBomber->hitboxBadnik)) {
            if (Player_CheckBadnikBreak(player, self, true)) {
                if (self->projectile)
                    destroyEntity(self->projectile);
            }
        }
        else if (self->state == BuzzBomber_State_Flying && !self->detectedPlayer) {
            if (Player_CheckCollisionTouch(player, self, &self->hitboxRange)) {
                self->detectedPlayer = true;
                self->timer          = 90;
                RSDK.SetSpriteAnimation(-1, 0, &self->thrustAnimator, true, 0);
                self->state = BuzzBomber_State_DetectedPlayer;
            }
        }
    }
}

void BuzzBomber_State_Init(void)
{
    RSDK_THIS(BuzzBomber);

    self->active = ACTIVE_NORMAL;
    if (!(self->direction & FLIP_X))
        self->velocity.x = -0x40000;
    else
        self->velocity.x = 0x40000;

    self->state = BuzzBomber_State_Flying;
    BuzzBomber_State_Flying();
}

void BuzzBomber_State_Flying(void)
{
    RSDK_THIS(BuzzBomber);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (!--self->timer) {
        self->direction ^= FLIP_X;
        self->timer          = 60;
        self->velocity.x     = -self->velocity.x;
        self->detectedPlayer = false;
        RSDK.SetSpriteAnimation(-1, 0, &self->thrustAnimator, true, 0);
        self->state = BuzzBomber_State_Idle;
    }

    RSDK.ProcessAnimation(&self->animator);
    RSDK.ProcessAnimation(&self->wingAnimator);
    RSDK.ProcessAnimation(&self->thrustAnimator);
    BuzzBomber_CheckPlayerCollisions();
    BuzzBomber_CheckOffScreen();
}

void BuzzBomber_State_Idle(void)
{
    RSDK_THIS(BuzzBomber);

    if (!--self->timer) {
        self->timer = 128;
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 3, &self->thrustAnimator, true, 0);
        self->state = BuzzBomber_State_Flying;
    }

    RSDK.ProcessAnimation(&self->animator);
    RSDK.ProcessAnimation(&self->wingAnimator);

    BuzzBomber_CheckPlayerCollisions();
    BuzzBomber_CheckOffScreen();
}

void BuzzBomber_State_DetectedPlayer(void)
{
    RSDK_THIS(BuzzBomber);

    RSDK.ProcessAnimation(&self->animator);
    RSDK.ProcessAnimation(&self->wingAnimator);
    BuzzBomber_CheckPlayerCollisions();
    BuzzBomber_CheckOffScreen();

    self->timer--;
    if (self->timer == 82) {
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 1, &self->animator, true, 0);
    }
    else if (self->timer == 45) {
        EntityBuzzBomber *projectile = CREATE_ENTITY(BuzzBomber, INT_TO_VOID(true), self->position.x, self->position.y);
        if (self->direction) {
            projectile->position.x += 0x180000;
            projectile->velocity.x = 0x20000;
        }
        else {
            projectile->position.x -= 0x180000;
            projectile->velocity.x = -0x20000;
        }
        projectile->position.y += 0x1C0000;
        projectile->velocity.y = 0x20000;
        projectile->groundVel  = 0;
        projectile->projectile = (Entity *)self;
        projectile->direction  = self->direction;
        projectile->active     = ACTIVE_NORMAL;

        self->projectile = (Entity *)projectile;
    }
    else if (!self->timer) {
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 0, &self->animator, true, 0);
        self->timer = 128;
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 3, &self->thrustAnimator, true, 0);
        self->state = BuzzBomber_State_Flying;
    }
}

void BuzzBomber_State_ProjectileCharge(void)
{
    RSDK_THIS(BuzzBomber);
    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == 6) {
        self->state            = BuzzBomber_State_ProjectileShot;
        EntityBuzzBomber *shot = (EntityBuzzBomber *)self->projectile;
        shot->projectile       = NULL;
    }
}

void BuzzBomber_State_ProjectileShot(void)
{
    RSDK_THIS(BuzzBomber);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (RSDK.CheckOnScreen(self, NULL)) {
        RSDK.ProcessAnimation(&self->animator);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &BuzzBomber->hitboxProjectile))
                Player_ProjectileHurt(player, self);
        }
    }
    else {
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void BuzzBomber_EditorDraw(void)
{
    RSDK_THIS(BuzzBomber);

    BuzzBomber_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        self->hitboxRange.right  = self->shotRange;
        self->hitboxRange.left   = -self->shotRange;
        self->hitboxRange.top    = -256;
        self->hitboxRange.bottom = 256;

        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &self->hitboxRange, FLIP_NONE, 0xFF0000);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void BuzzBomber_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("GHZ"))
        BuzzBomber->aniFrames = RSDK.LoadSpriteAnimation("GHZ/BuzzBomber.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("Blueprint"))
        BuzzBomber->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/BuzzBomber.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(BuzzBomber, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void BuzzBomber_Serialize(void)
{
    RSDK_EDITABLE_VAR(BuzzBomber, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(BuzzBomber, VAR_UINT8, shotRange);
}
