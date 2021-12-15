// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Aquis Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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

    if (self->animator1.animationID == 4) {
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }
    else {
        RSDK.DrawSprite(&self->animator1, NULL, false);
        RSDK.DrawSprite(&self->animator2, NULL, false);
    }
}

void Aquis_Create(void *data)
{
    RSDK_THIS(Aquis);
    self->visible   = true;
    self->drawOrder = Zone->drawOrderLow;
    self->drawFX |= FX_FLIP;
    self->startPos = self->position;
    self->startDir = self->direction;
    self->timer    = 128;
    self->field_60 = 0;

    if (data) {
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x200000;
        self->updateRange.y = 0x200000;
        RSDK.SetSpriteAnimation(Aquis->aniFrames, 2, &self->animator1, true, 0);
        self->state = Aquis_State_Shot;
    }
    else {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->timer2        = 3;
        RSDK.SetSpriteAnimation(Aquis->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(Aquis->aniFrames, 3, &self->animator2, true, 0);
        self->state = Aquis_State_Setup;
    }
}

void Aquis_StageLoad(void)
{
    if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2"))
        Aquis->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Aquis.bin", SCOPE_STAGE);
    Aquis->hitbox1.left   = -10;
    Aquis->hitbox1.top    = -16;
    Aquis->hitbox1.right  = 10;
    Aquis->hitbox1.bottom = 16;
    Aquis->hitbox2.left   = -96;
    Aquis->hitbox2.top    = -64;
    Aquis->hitbox2.right  = 64;
    Aquis->hitbox2.bottom = 128;
    Aquis->hitbox3.left   = -3;
    Aquis->hitbox3.top    = -3;
    Aquis->hitbox3.right  = 3;
    Aquis->hitbox3.bottom = 3;
    Aquis->sfxShot        = RSDK.GetSfx("Stage/Shot.wav");
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
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Aquis_CheckPlayerCollisions(void)
{
    RSDK_THIS(Aquis);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Aquis->hitbox1))
            Player_CheckBadnikBreak(self, player, true);
    }
}

void Aquis_CheckOnScreen(void)
{
    RSDK_THIS(Aquis);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        Aquis_Create(NULL);
    }
}

void Aquis_State_Setup(void)
{
    RSDK_THIS(Aquis);
    self->active = ACTIVE_NORMAL;
    self->timer  = 32;
    self->state  = Aquis_Unknown5;
    Aquis_Unknown5();
}

void Aquis_Unknown5(void)
{
    RSDK_THIS(Aquis);
    self->timer--;
    if (!self->timer) {
        int32 timer = self->timer2;
        self->timer2--;
        if (timer) {
            self->velocity.y = -0x10000;
            self->timer      = 128;
            self->field_60   = 0;
            self->state      = Aquis_Unknown6;
        }
        else {
            self->velocity.y = 0;
            if (self->direction == FLIP_X) {
                self->velocity.x = 0;
                self->state      = Aquis_Unknown8;
                RSDK.SetSpriteAnimation(Aquis->aniFrames, 4, &self->animator1, true, 0);
            }
            else {
                self->velocity.x = -0x20000;
                self->state      = Aquis_Unknown9;
            }
        }
    }
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    Aquis_CheckPlayerCollisions();
    Aquis_CheckOnScreen();
}

void Aquis_Unknown6(void)
{
    RSDK_THIS(Aquis);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    EntityPlayer *playerPtr = Player_GetNearestPlayer();
    if (playerPtr) {
        bool32 flag = false;
        if (playerPtr->position.x >= self->position.x) {
            self->velocity.x += 4096;
            if (self->velocity.x > 0x10000)
                self->velocity.x = 0x10000;
            flag = self->direction == FLIP_NONE;
        }
        else {
            self->velocity.x -= 4096;
            if (self->velocity.x < -0x10000)
                self->velocity.x = -0x10000;
            flag = self->direction == FLIP_X;
        }

        if (flag) {
            self->state = Aquis_Unknown8;
            RSDK.SetSpriteAnimation(Aquis->aniFrames, 4, &self->animator1, true, 0);
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

    if (self->state != Aquis_Unknown8) {
        int32 timer = self->timer2;
        self->timer--;
        if (!timer) {
            self->timer      = 32;
            self->state      = Aquis_Unknown5;
            self->velocity.x = 0;
            self->velocity.y = 0;
        }

        if (!self->field_60) {
            foreach_active(Player, player)
            {
                if (player->velocity.y < 0 && !player->onGround)
                    continue;

                if (Player_CheckCollisionTouch(player, self, &Aquis->hitbox2)) {
                    self->field_60 = 1;
                    self->timer    = 64;
                    RSDK.SetSpriteAnimation(Aquis->aniFrames, 1, &self->animator1, true, 0);
                    self->state = Aquis_Unknown7;
                    if (player->position.x >= self->position.x) {
                        if (player->velocity.x <= (self->position.x - player->position.x) >> 5) {
                            RSDK.SetSpriteAnimation(Aquis->aniFrames, 4, &self->animator1, true, 0);
                            self->state = Aquis_Unknown8;
                        }
                    }
                    else if (player->velocity.x >= (self->position.x - player->position.x) >> 5) {
                        RSDK.SetSpriteAnimation(Aquis->aniFrames, 4, &self->animator1, true, 0);
                        self->state = Aquis_Unknown8;
                    }
                }
            }
        }
    }
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    Aquis_CheckPlayerCollisions();
    Aquis_CheckOnScreen();
}

void Aquis_Unknown7(void)
{
    RSDK_THIS(Aquis);
    self->timer--;
    if (self->timer) {
        if (self->timer == 33) {
            EntityAquis *shot = CREATE_ENTITY(Aquis, intToVoid(true), self->position.x, self->position.y);
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
        self->timer      = 32;
        self->timer2     = 0;
        self->state      = Aquis_Unknown5;
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    Aquis_CheckPlayerCollisions();
    Aquis_CheckOnScreen();
}

void Aquis_Unknown8(void)
{
    RSDK_THIS(Aquis);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID == self->animator1.frameCount - 1) {
        RSDK.SetSpriteAnimation(Aquis->aniFrames, 0, &self->animator1, true, 0);
        self->direction ^= 1;
        if (self->timer2 < 0) {
            self->velocity.x = -0x20000;
            self->state      = Aquis_Unknown9;
            Aquis_Unknown9();
        }
        else if (self->field_60) {
            self->timer = 64;
            RSDK.SetSpriteAnimation(Aquis->aniFrames, 1, &self->animator1, true, 0);
            self->state = Aquis_Unknown7;
            Aquis_Unknown7();
        }
        else {
            self->state = Aquis_Unknown6;
            Aquis_Unknown6();
        }
    }
    else {
        if (self->timer > 1)
            self->timer--;
        Aquis_CheckPlayerCollisions();
        Aquis_CheckOnScreen();
    }
}

void Aquis_Unknown9(void)
{
    RSDK_THIS(Aquis);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    RSDK.ProcessAnimation(&self->animator2);
    Aquis_CheckPlayerCollisions();
    Aquis_CheckOnScreen();
}

void Aquis_State_Shot(void)
{
    RSDK_THIS(Aquis);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (RSDK.CheckOnScreen(self, 0)) {
        RSDK.ProcessAnimation(&self->animator1);
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Aquis->hitbox3))
                Player_CheckProjectileHit(player, self);
        }
    }
    else {
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void Aquis_EditorDraw(void)
{
    RSDK_THIS(Aquis);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
}

void Aquis_EditorLoad(void) { Aquis->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Aquis.bin", SCOPE_STAGE); }
#endif

void Aquis_Serialize(void) {}
