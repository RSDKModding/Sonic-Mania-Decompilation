// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Fireworm Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectFireworm *Fireworm;

void Fireworm_Update(void)
{
    RSDK_THIS(Fireworm);
    StateMachine_Run(self->state);
}

void Fireworm_LateUpdate(void) {}

void Fireworm_StaticUpdate(void) {}

void Fireworm_Draw(void)
{
    RSDK_THIS(Fireworm);

    self->drawFX &= ~FX_FLIP;
    Fireworm->animator.frameID = 0;
    RSDK.DrawSprite(&Fireworm->animator, &self->startPos, false);

    self->drawFX |= FX_FLIP;
    if (self->state) {
        for (int i = Fireworm_SegmentCount - 1; i > 0; --i) {
            self->direction = self->directions[i];
            RSDK.DrawSprite(&self->animators1[i], &self->positions[i], false);
            RSDK.DrawSprite(&self->animators2[i], &self->positions[i], false);
        }

        self->direction = self->directions[0];
        RSDK.DrawSprite(&self->animators1[0], NULL, false);
    }
}

void Fireworm_Create(void *data)
{
    RSDK_THIS(Fireworm);
    self->drawFX |= FX_FLIP;
    RSDK.SetSpriteAnimation(Fireworm->aniFrames, 0, self->animators1, true, 0);

    if (!SceneInfo->inEditor) {
        for (int i = 0; i < Fireworm_SegmentCount; ++i) {
            self->positions[i].x = self->position.x;
            self->positions[i].y = self->position.y;
            self->positionsY[i]  = self->position.y;
        }

        self->startPos.x    = self->position.x;
        self->startPos.y    = self->position.y;
        self->startDir      = self->direction;
        self->visible       = true;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->drawOrder     = Zone->drawOrderLow;
        self->state         = Fireworm_State_Setup;
    }
}

void Fireworm_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Fireworm->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Fireworm.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        Fireworm->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Fireworm.bin", SCOPE_STAGE);

    RSDK.SetSpriteAnimation(Fireworm->aniFrames, 6, &Fireworm->animator, true, 0);

    Fireworm->hitbox1.left   = -8;
    Fireworm->hitbox1.top    = -8;
    Fireworm->hitbox1.right  = 8;
    Fireworm->hitbox1.bottom = 8;
    Fireworm->hitbox2.left   = -128;
    Fireworm->hitbox2.top    = -128;
    Fireworm->hitbox2.right  = 128;
    Fireworm->hitbox2.bottom = 128;

    DEBUGMODE_ADD_OBJ(Fireworm);
}

void Fireworm_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Fireworm, NULL, self->position.x, self->position.y);
}

void Fireworm_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Fireworm->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Fireworm_CheckPlayerCollisions(void)
{
    RSDK_THIS(Fireworm);

    foreach_active(Player, player)
    {
        self->position.x = self->positions[0].x;
        self->position.y = self->positions[0].y;
        if (Player_CheckBadnikTouch(player, self, &Fireworm->hitbox1) && Player_CheckBadnikBreak(self, player, false)) {
            for (int i = 1; i < Fireworm_SegmentCount; ++i) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->positions[i].x, self->positions[i].y);
                RSDK.SetSpriteAnimation(Fireworm->aniFrames, 3, &debris->animator, true, 0);
                debris->velocity.x    = RSDK.Rand(-0x40000, 0x40000);
                debris->velocity.y    = RSDK.Rand(-0x40000, 0x40000);
                debris->gravity       = 0x3800;
                debris->drawOrder     = Zone->drawOrderLow;
                debris->updateRange.x = 0x400000;
                debris->updateRange.y = 0x400000;
            }
            self->state = StateMachine_None;
            foreach_break;
        }
        else {
            for (int i = 1; i < Fireworm_SegmentCount; ++i) {
                self->position.x = self->positions[i].x;
                self->position.y = self->positions[i].y;

                if (Player_CheckCollisionTouch(player, self, &Fireworm->hitbox1)) {
#if RETRO_USE_PLUS
                    if (!Player_CheckMightyUnspin(0x300, player, 2, &player->uncurlTimer))
#endif
                        Player_CheckHit(player, self);
                }
            }
        }
    }

    if (self->state) {
        self->position.x = self->positions[0].x;
        self->position.y = self->positions[0].y;
    }
}

void Fireworm_CheckOnScreen(void)
{
    RSDK_THIS(Fireworm);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->timer      = 0;
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        for (int i = 0; i < Fireworm_SegmentCount; ++i) {
            self->velocities[i].x = 0;
            self->angles[i]       = 0;
            self->timers[i]       = 0;
            if (i > 0) {
                RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animators1[i], true, 0);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animators2[i], true, 0);
            }
        }
        Fireworm_Create(NULL);
    }
}

void Fireworm_State_Setup(void)
{
    RSDK_THIS(Fireworm);

    self->active = ACTIVE_NORMAL;
    self->state  = Fireworm_State_Unknown1;
    Fireworm_State_Unknown1();
}

void Fireworm_State_Unknown1(void)
{
    RSDK_THIS(Fireworm);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Fireworm->hitbox2)) {
            for (int i = 0; i < Fireworm_SegmentCount; ++i) {
                self->directions[i] = player->position.x >= self->position.x;
            }

            self->startDir = self->directions[0];
            if (self->startDir) {
                self->velocities[0].x = 0x10000;
                self->field_EC        = self->position.x - 0x3C0000;
                self->field_F0        = self->position.x + 0xBC0000;
            }
            else {
                self->velocities[0].x = -0x10000;
                self->field_EC        = self->position.x - 0xBC0000;
                self->field_F0        = self->position.x + 0x3C0000;
            }
            self->state = Fireworm_State_Unknown2;
            foreach_break;
        }
    }

    if (self->state == Fireworm_State1_Unknown1)
        Fireworm_CheckOnScreen();
}

void Fireworm_State_Unknown2(void)
{
    RSDK_THIS(Fireworm);

    RSDK.ProcessAnimation(&self->animators1[0]);
    if (self->animators1[0].frameID == 3) {
        for (int i = 1; i < Fireworm_SegmentCount; ++i) {
            RSDK.SetSpriteAnimation(Fireworm->aniFrames, 3, &self->animators1[i], true, 0);
            self->timers2[i] = RSDK.Rand(0, 60);
        }
        self->state = Fireworm_State1_Unknown1;
    }
}

void Fireworm_State1_Unknown1(void)
{
    RSDK_THIS(Fireworm);

    ++self->timer;
    for (int i = 0; i < Fireworm_SegmentCount; ++i) {
        if (self->timer == (i * 10) + 4) {
            if (!self->directions[i])
                self->velocities[i].x = -0x10000;
            else
                self->velocities[i].x = 0x10000;

            if (i == Fireworm_SegmentCount - 1) {
                self->timer = 0;
                self->state = Fireworm_State1_Unknown2;
            }
        }
    }

    Fireworm_State1_Unknown2();
}

void Fireworm_State1_Unknown2(void)
{
    RSDK_THIS(Fireworm);

    for (int i = 0; i < Fireworm_SegmentCount; ++i) {
        if (!self->velocities[i].x)
            continue;

        RSDK.ProcessAnimation(&self->animators1[i]);
        RSDK.ProcessAnimation(&self->animators2[i]);

        self->positions[i].x += self->velocities[i].x;

        if (self->velocities[i].x <= 0) {
            if (self->positions[i].x > self->field_EC) {
                self->velocities[i].x -= 0xC00;
                if (self->velocities[i].x < -0x10000)
                    self->velocities[i].x = -0x10000;
            }
            else {
                self->velocities[i].x += 0xC00;
                if (self->velocities[i].x > 0x10000)
                    self->velocities[i].x = 0x10000;

                if (!self->timers[i]) {
                    if (i)
                        RSDK.SetSpriteAnimation(Fireworm->aniFrames, 4, &self->animators1[i], true, 0);
                    else
                        RSDK.SetSpriteAnimation(Fireworm->aniFrames, 2, &self->animators1[i], true, 0);
                    self->directions[i] = FLIP_X;
                    if (!self->startDir)
                        self->velocities[i].y = -0x10000;
                    else
                        self->velocities[i].y = 0x10000;
                    self->timers[i] = 32;
                }
            }
        }
        else {
            if (self->positions[i].x < self->field_F0) {
                self->velocities[i].x += 0xC00;
                if (self->velocities[i].x > 0x10000)
                    self->velocities[i].x = 0x10000;
            }
            else {
                self->velocities[i].x -= 0xC00;
                if (self->velocities[i].x < -0x10000)
                    self->velocities[i].x = -0x10000;

                if (!self->timers[i]) {
                    if (i)
                        RSDK.SetSpriteAnimation(Fireworm->aniFrames, 4, &self->animators1[i], true, 0);
                    else
                        RSDK.SetSpriteAnimation(Fireworm->aniFrames, 2, &self->animators1[i], true, 0);
                    self->directions[i] = FLIP_NONE;
                    if (!self->startDir)
                        self->velocities[i].y = 0x10000;
                    else
                        self->velocities[i].y = -0x10000;
                    self->timers[i] = 32;
                }
            }
        }

        if (self->timers[i]) {
            self->timers[i]--;
            self->positionsY[i] += self->velocities[i].y;
        }
        if (!--self->timers2[i]) {
            RSDK.SetSpriteAnimation(Fireworm->aniFrames, 5, &self->animators2[i], true, 0);
            self->timers2[i] = RSDK.Rand(0, 60) + 32;
        }
        self->angles[i] += 16;
        self->positions[i].y = (RSDK.Sin1024(self->angles[i]) << 8) + self->positionsY[i];
        self->angles[i] %= 0xC00;
    }
    Fireworm_CheckPlayerCollisions();
    Fireworm_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void Fireworm_EditorDraw(void)
{
    RSDK_THIS(Fireworm);

    for (int i = 0; i < Fireworm_SegmentCount; ++i) {
        self->positions[i].x = self->position.x + (self->direction ? 0x100000 : -0x100000) * i;
        self->positions[i].y = self->position.y;
        self->positionsY[i]  = self->position.y;
    }

    self->startPos.x = self->position.x;
    self->startPos.y = self->position.y;
    self->startDir   = self->direction;

    for (int i = 1; i < Fireworm_SegmentCount; ++i) {
        RSDK.SetSpriteAnimation(Fireworm->aniFrames, 3, &self->animators1[i], true, 0);
    }

    Fireworm_Draw();
}

void Fireworm_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Fireworm->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Fireworm.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        Fireworm->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Fireworm.bin", SCOPE_STAGE);

    RSDK.SetSpriteAnimation(Fireworm->aniFrames, 6, &Fireworm->animator, true, 0);

    RSDK_ACTIVE_VAR(Fireworm, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void Fireworm_Serialize(void) { RSDK_EDITABLE_VAR(Fireworm, VAR_UINT8, direction); }
