// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Aquis Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectAquis *Aquis;

void Aquis_Update(void)
{
    RSDK_THIS(Aquis);

    StateMachine_Run(self->state);
}

void Aquis_LateUpdate(void) {}

void Aquis_StaticUpdate(void) {}

void Aquis_Draw(void)
{
    RSDK_THIS(Aquis);

    if (self->mainAnimator.animationID == 4) {
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    }
    else {
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);
        RSDK.DrawSprite(&self->wingAnimator, NULL, false);
    }
}

void Aquis_Create(void *data)
{
    RSDK_THIS(Aquis);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->drawFX |= FX_FLIP;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->timer         = 128;
    self->playerInRange = 0;

    if (data) {
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x200000;
        self->updateRange.y = 0x200000;
        RSDK.SetSpriteAnimation(Aquis->aniFrames, 2, &self->mainAnimator, true, 0);
        self->state = Aquis_State_Shot;
    }
    else {
        self->active         = ACTIVE_BOUNDS;
        self->updateRange.x  = 0x800000;
        self->updateRange.y  = 0x800000;
        self->remainingTurns = 3;
        RSDK.SetSpriteAnimation(Aquis->aniFrames, 0, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(Aquis->aniFrames, 3, &self->wingAnimator, true, 0);
        self->state = Aquis_State_Init;
    }
}

void Aquis_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("OOZ1") || RSDK.CheckSceneFolder("OOZ2"))
        Aquis->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Aquis.bin", SCOPE_STAGE);

    Aquis->hitboxBadnik.left   = -10;
    Aquis->hitboxBadnik.top    = -16;
    Aquis->hitboxBadnik.right  = 10;
    Aquis->hitboxBadnik.bottom = 16;

    Aquis->hitboxRange.left   = -96;
    Aquis->hitboxRange.top    = -64;
    Aquis->hitboxRange.right  = 64;
    Aquis->hitboxRange.bottom = 128;

    Aquis->hitboxProjectile.left   = -3;
    Aquis->hitboxProjectile.top    = -3;
    Aquis->hitboxProjectile.right  = 3;
    Aquis->hitboxProjectile.bottom = 3;

    Aquis->sfxShot = RSDK.GetSfx("Stage/Shot.wav");

    DEBUGMODE_ADD_OBJ(Aquis);
}

void Aquis_DebugSpawn(void)
{
    RSDK_THIS(Aquis);

    EntityAquis *aquis = CREATE_ENTITY(Aquis, NULL, self->position.x, self->position.y);
    aquis->direction   = self->direction;
    aquis->startDir    = self->direction;
}

void Aquis_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Aquis->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Aquis_CheckPlayerCollisions(void)
{
    RSDK_THIS(Aquis);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Aquis->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Aquis_CheckOffScreen(void)
{
    RSDK_THIS(Aquis);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Aquis_Create(NULL);
    }
}

void Aquis_State_Init(void)
{
    RSDK_THIS(Aquis);

    self->active = ACTIVE_NORMAL;
    self->timer  = 32;

    self->state = Aquis_State_Idle;
    Aquis_State_Idle();
}

void Aquis_State_Idle(void)
{
    RSDK_THIS(Aquis);

    if (!--self->timer) {
        if (self->remainingTurns--) {
            self->velocity.y    = -0x10000;
            self->timer         = 128;
            self->playerInRange = false;
            self->state         = Aquis_State_Moving;
        }
        else {
            self->velocity.y = 0;

            if (self->direction == FLIP_X) {
                self->velocity.x = 0;
                self->state      = Aquis_State_Turning;
                RSDK.SetSpriteAnimation(Aquis->aniFrames, 4, &self->mainAnimator, true, 0);
            }
            else {
                self->velocity.x = -0x20000;
                self->state      = Aquis_State_Flee;
            }
        }
    }

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->wingAnimator);

    Aquis_CheckPlayerCollisions();
    Aquis_CheckOffScreen();
}

void Aquis_State_Moving(void)
{
    RSDK_THIS(Aquis);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    EntityPlayer *playerPtr = Player_GetNearestPlayer();
    if (playerPtr) {
        bool32 changeDir = false;
        if (playerPtr->position.x >= self->position.x) {
            self->velocity.x += 0x1000;

            if (self->velocity.x > 0x10000)
                self->velocity.x = 0x10000;

            changeDir = self->direction == FLIP_NONE;
        }
        else {
            self->velocity.x -= 0x1000;

            if (self->velocity.x < -0x10000)
                self->velocity.x = -0x10000;

            changeDir = self->direction == FLIP_X;
        }

        if (changeDir) {
            self->state = Aquis_State_Turning;
            RSDK.SetSpriteAnimation(Aquis->aniFrames, 4, &self->mainAnimator, true, 0);
        }

        if (playerPtr->position.y >= self->position.y) {
            self->velocity.y += 0x1000;

            if (self->velocity.y > 0x10000)
                self->velocity.y = 0x10000;
        }
        else {
            self->velocity.y -= 0x1000;

            if (self->velocity.y < -0x10000)
                self->velocity.y = -0x10000;
        }
    }

    if (self->state != Aquis_State_Turning) {
        if (!self->timer--) {
            self->timer      = 32;
            self->state      = Aquis_State_Idle;
            self->velocity.x = 0;
            self->velocity.y = 0;
        }

        if (!self->playerInRange) {
            foreach_active(Player, player)
            {
                if (player->velocity.y < 0 && !player->onGround)
                    continue;

                if (Player_CheckCollisionTouch(player, self, &Aquis->hitboxRange)) {
                    self->playerInRange = true;
                    self->timer         = 64;
                    RSDK.SetSpriteAnimation(Aquis->aniFrames, 1, &self->mainAnimator, true, 0);
                    self->state = Aquis_State_Shoot;

                    if (player->position.x >= self->position.x) {
                        if (player->velocity.x <= (self->position.x - player->position.x) >> 5) {
                            RSDK.SetSpriteAnimation(Aquis->aniFrames, 4, &self->mainAnimator, true, 0);
                            self->state = Aquis_State_Turning;
                        }
                    }
                    else if (player->velocity.x >= (self->position.x - player->position.x) >> 5) {
                        RSDK.SetSpriteAnimation(Aquis->aniFrames, 4, &self->mainAnimator, true, 0);
                        self->state = Aquis_State_Turning;
                    }
                }
            }
        }
    }

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->wingAnimator);

    Aquis_CheckPlayerCollisions();
    Aquis_CheckOffScreen();
}

void Aquis_State_Shoot(void)
{
    RSDK_THIS(Aquis);

    if (--self->timer) {
        if (self->timer == 33) {
            EntityAquis *shot = CREATE_ENTITY(Aquis, INT_TO_VOID(true), self->position.x, self->position.y);

            if (self->direction) {
                shot->position.x += 0x100000;
                shot->velocity.x = 0x30000;
            }
            else {
                shot->position.x -= 0x100000;
                shot->velocity.x = -0x30000;
            }

            shot->position.y -= 0xA0000;
            shot->velocity.y = 0x20000;
            shot->direction  = self->direction;
            shot->active     = ACTIVE_NORMAL;
            RSDK.PlaySfx(Aquis->sfxShot, false, 255);
        }
    }
    else {
        self->timer          = 32;
        self->remainingTurns = 0;
        self->state          = Aquis_State_Idle;
        self->velocity.x     = 0;
        self->velocity.y     = 0;
    }

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->wingAnimator);

    Aquis_CheckPlayerCollisions();
    Aquis_CheckOffScreen();
}

void Aquis_State_Turning(void)
{
    RSDK_THIS(Aquis);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Aquis->aniFrames, 0, &self->mainAnimator, true, 0);
        self->direction ^= FLIP_X;

        if (self->remainingTurns < 0) {
            self->velocity.x = -0x20000;

            self->state = Aquis_State_Flee;
            Aquis_State_Flee();
        }
        else if (self->playerInRange) {
            self->timer = 64;
            RSDK.SetSpriteAnimation(Aquis->aniFrames, 1, &self->mainAnimator, true, 0);

            self->state = Aquis_State_Shoot;
            Aquis_State_Shoot();
        }
        else {
            self->state = Aquis_State_Moving;
            Aquis_State_Moving();
        }
    }
    else {
        if (self->timer > 1)
            self->timer--;

        Aquis_CheckPlayerCollisions();
        Aquis_CheckOffScreen();
    }
}

void Aquis_State_Flee(void)
{
    RSDK_THIS(Aquis);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    RSDK.ProcessAnimation(&self->wingAnimator);

    Aquis_CheckPlayerCollisions();
    Aquis_CheckOffScreen();
}

void Aquis_State_Shot(void)
{
    RSDK_THIS(Aquis);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (RSDK.CheckOnScreen(self, NULL)) {
        RSDK.ProcessAnimation(&self->mainAnimator);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Aquis->hitboxProjectile))
                Player_ProjectileHurt(player, self);
        }
    }
    else {
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void Aquis_EditorDraw(void)
{
    RSDK_THIS(Aquis);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    RSDK.DrawSprite(&self->wingAnimator, NULL, false);
}

void Aquis_EditorLoad(void) { Aquis->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Aquis.bin", SCOPE_STAGE); }
#endif

void Aquis_Serialize(void) {}
