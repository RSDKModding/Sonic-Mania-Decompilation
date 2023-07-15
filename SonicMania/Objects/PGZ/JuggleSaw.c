// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: JuggleSaw Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectJuggleSaw *JuggleSaw;

// 🦀 crab in da code

void JuggleSaw_Update(void)
{
    RSDK_THIS(JuggleSaw);

    StateMachine_Run(self->state);
}

void JuggleSaw_LateUpdate(void) {}

void JuggleSaw_StaticUpdate(void) {}

void JuggleSaw_Draw(void)
{
    RSDK_THIS(JuggleSaw);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void JuggleSaw_Create(void *data)
{
    RSDK_THIS(JuggleSaw);

    self->visible = true;
    self->drawFX |= FX_FLIP;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0xC00000;
    self->updateRange.y = 0xC00000;

    if (!SceneInfo->inEditor) {
        if (data) {
            RSDK.SetSpriteAnimation(JuggleSaw->aniFrames, 6, &self->animator, true, 0);
            self->state = JuggleSaw_StateSaw_Handle;
        }
        else {
            self->startPos = self->position;
            self->startDir = self->direction;

            switch (self->direction) {
                case FLIP_NONE: RSDK.SetSpriteAnimation(JuggleSaw->aniFrames, self->hasSaw == JUGGLESAW_HAS_SAW, &self->animator, true, 0); break;

                case FLIP_X:
                    self->direction = FLIP_Y;
                    RSDK.SetSpriteAnimation(JuggleSaw->aniFrames, self->hasSaw == JUGGLESAW_HAS_SAW, &self->animator, true, 0);
                    break;

                case FLIP_Y:
                    self->direction = FLIP_NONE;
                    RSDK.SetSpriteAnimation(JuggleSaw->aniFrames, 4 - (self->hasSaw != JUGGLESAW_HAS_SAW), &self->animator, true, 0);
                    break;

                case FLIP_XY:
                    self->direction = FLIP_X;
                    RSDK.SetSpriteAnimation(JuggleSaw->aniFrames, 4 - (self->hasSaw != JUGGLESAW_HAS_SAW), &self->animator, true, 0);
                    break;

                default: break;
            }

            if (!self->sawSpeed)
                self->sawSpeed = 8;

            self->sawSpeed <<= 7;
            self->state = JuggleSaw_StateCrab_Setup;
        }
    }
}

void JuggleSaw_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        JuggleSaw->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/JuggleSaw.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("PSZ2"))
        JuggleSaw->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/JuggleSaw.bin", SCOPE_STAGE);

    JuggleSaw->hitboxBadnik.left   = -12;
    JuggleSaw->hitboxBadnik.top    = -18;
    JuggleSaw->hitboxBadnik.right  = 12;
    JuggleSaw->hitboxBadnik.bottom = 18;

    JuggleSaw->hitboxFriendRange.left   = -512;
    JuggleSaw->hitboxFriendRange.top    = -512;
    JuggleSaw->hitboxFriendRange.right  = 512;
    JuggleSaw->hitboxFriendRange.bottom = 512;

    JuggleSaw->hitboxSaw.left   = -12;
    JuggleSaw->hitboxSaw.top    = -12;
    JuggleSaw->hitboxSaw.right  = 12;
    JuggleSaw->hitboxSaw.bottom = 12;

    JuggleSaw->hitboxGrabV.left   = -24;
    JuggleSaw->hitboxGrabV.top    = -38;
    JuggleSaw->hitboxGrabV.right  = -16;
    JuggleSaw->hitboxGrabV.bottom = -30;

    JuggleSaw->hitboxGrabH.left   = 38;
    JuggleSaw->hitboxGrabH.top    = -24;
    JuggleSaw->hitboxGrabH.right  = 30;
    JuggleSaw->hitboxGrabH.bottom = -16;

    DEBUGMODE_ADD_OBJ(JuggleSaw);

    JuggleSaw->sfxExplode = RSDK.GetSfx("Stage/Explosion.wav"); // not actually used it seems
    JuggleSaw->sfxJuggle  = RSDK.GetSfx("PSZ/Juggle.wav");
    JuggleSaw->sfxThrow   = RSDK.GetSfx("PSZ/JuggleThrow.wav");
}

void JuggleSaw_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityJuggleSaw *badnik = CREATE_ENTITY(JuggleSaw, NULL, self->position.x, self->position.y);
    badnik->sawDelay        = 30;
}

void JuggleSaw_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(JuggleSaw->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void JuggleSaw_CheckPlayerCollisions(void)
{
    RSDK_THIS(JuggleSaw);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &JuggleSaw->hitboxBadnik) && Player_CheckBadnikBreak(player, self, false)) {
            if (self->hasSaw == JUGGLESAW_HAS_SAW) {
                int32 debrisX = self->position.x;
                int32 debrisY = self->position.y;
                if (self->startDir >= FLIP_Y)
                    debrisX += 0x200000 * ((self->direction & FLIP_X) ? -1 : 1);
                else
                    debrisY += 0x200000 * ((self->direction & FLIP_Y) ? 1 : -1);
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, debrisX, debrisY);

                RSDK.SetSpriteAnimation(JuggleSaw->aniFrames, 6, &debris->animator, true, 0);

                int32 minVelX = -4, maxVelX = 5, minVelY = -4, maxVelY = 5;
                if (self->startDir >= FLIP_Y)
                    minVelY = (self->direction & FLIP_X) ? -1 : -4;
                else
                    minVelY = (self->direction & FLIP_Y) ? -4 : -1;

                debris->velocity.x      = RSDK.Rand(minVelX, maxVelX) << 16;
                debris->velocity.y      = RSDK.Rand(minVelY, maxVelY) << 16;
                debris->gravityStrength = 0x3800;
                debris->drawGroup       = self->drawGroup;
                debris->updateRange.x   = 0x400000;
                debris->updateRange.y   = 0x400000;
            }

            destroyEntity(self);
        }
    }
}

void JuggleSaw_CheckOffScreen(void)
{
    RSDK_THIS(JuggleSaw);

    if (self->hasSaw != JUGGLESAW_AWAITING_SAW && !RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->sawSpeed  = self->sawSpeed >> 7;
        self->position  = self->startPos;
        self->direction = self->startDir;
        JuggleSaw_Create(NULL);
    }
}

void JuggleSaw_StateCrab_Setup(void)
{
    RSDK_THIS(JuggleSaw);

    self->active     = ACTIVE_NORMAL;
    self->friends[0] = NULL;
    self->angle      = 0;
    self->sawTimer   = 0;

    self->state = JuggleSaw_StateCrab_Handle;
    JuggleSaw_StateCrab_Handle();
}

void JuggleSaw_StateCrab_Handle(void)
{
    RSDK_THIS(JuggleSaw);

    // this was the worst func to decompile so far. -rmg
    RSDK.ProcessAnimation(&self->animator);

    switch (self->startDir) {
        case FLIP_NONE:
        case FLIP_X: self->position.x = (RSDK.Cos256(self->angle) << 10) + self->startPos.x; break;

        case FLIP_Y:
        case FLIP_XY: self->position.y = (RSDK.Cos256(self->angle) << 10) + self->startPos.y; break;

        default: break;
    }

    if (self->hasSaw != JUGGLESAW_HAS_SAW) {
        self->angle += 2;
    }
    else {
        self->angle += 4;

        if (self->animator.frameID == self->animator.frameCount - 1) {
            RSDK.PlaySfx(JuggleSaw->sfxJuggle, false, 0xFF);
            if (self->startDir >= FLIP_Y)
                self->direction ^= FLIP_Y;
            else
                self->direction ^= FLIP_X;
        }
        else {
            if (self->sawTimer == self->sawDelay - 1)
                --self->sawTimer;
        }

        if (++self->sawTimer >= self->sawDelay) {
            bool32 throwSaw = true;

            if (!self->friends[0] || self->friends[0]->classID != JuggleSaw->classID) {
                self->friendCount = 0;
                foreach_active(JuggleSaw, newFriend)
                {
                    if (newFriend != self && newFriend->hasSaw == JUGGLESAW_NO_SAW && newFriend->setID == self->setID
                        && self->friendCount < JUGGLESAW_MAX_FRIENDS) {
                        if (RSDK.CheckObjectCollisionTouchBox(newFriend, &JuggleSaw->hitboxBadnik, self, &JuggleSaw->hitboxFriendRange))
                            self->friends[self->friendCount++] = newFriend;
                    }
                }

                if (self->friendCount) {
                    self->friends[0]  = self->friends[RSDK.Rand(0, self->friendCount)];

                    if (self->startDir >= FLIP_Y) {
                        if (self->position.y < self->friends[0]->position.y) {
                            if ((self->direction & FLIP_Y))
                                --self->sawTimer;
                        }
                        else {
                            if (!(self->direction & FLIP_Y))
                                --self->sawTimer;
                        }
                    }
                    else {
                        if (self->position.x < self->friends[0]->position.x) {
                            if ((self->direction & FLIP_X))
                                --self->sawTimer;
                        }
                        else {
                            if (!(self->direction & FLIP_X))
                                --self->sawTimer;
                        }
                    }

                    throwSaw = false;
                    if (self->sawTimer >= self->sawDelay)
                        throwSaw = true;
                }
                else {
                    throwSaw = false;
                }
            }

            if (throwSaw) {
                RSDK.SetSpriteAnimation(JuggleSaw->aniFrames, self->startDir >= FLIP_Y ? 5 : 2, &self->animator, true, 0);
                self->direction ^= (self->startDir >= FLIP_Y) ? FLIP_Y : FLIP_X;
                self->state = JuggleSaw_StateCrab_ThrowSaw;
            }
        }
    }

    JuggleSaw_CheckPlayerCollisions();
    JuggleSaw_CheckOffScreen();
}

void JuggleSaw_StateCrab_ThrowSaw(void)
{
    RSDK_THIS(JuggleSaw);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == 3) {
        EntityJuggleSaw *reciever = self->friends[0];

        if (reciever->classID == JuggleSaw->classID) {
            RSDK.PlaySfx(JuggleSaw->sfxThrow, false, 0xFF);
            reciever->hasSaw = JUGGLESAW_AWAITING_SAW;
            reciever->active = ACTIVE_NORMAL;

            EntityJuggleSaw *saw = CREATE_ENTITY(JuggleSaw, INT_TO_VOID(true), self->position.x, self->position.y);
            int32 sx = 0x2C0000, sy = -0xE0000;
            if (self->startDir >= FLIP_Y) {
                sx = 0xE0000;
                sy = 0x2C0000;
            }

            saw->position.x += sx * ((self->direction & FLIP_X) ? -1 : 1);
            saw->position.y += sy * ((self->direction & FLIP_Y) ? -1 : 1);

            int32 targetX = 0;
            int32 targetY = 0;
            int32 sawDir  = 0;
            if (reciever->startDir >= FLIP_Y) {
                targetX = reciever->position.x + 0x220000 * ((reciever->direction & FLIP_X) ? -1 : 1);
                targetY = reciever->position.y + 0x140000 * (reciever->startPos.y >= self->startPos.y ? 1 : -1);
                sawDir  = (reciever->direction & FLIP_X) | (reciever->startPos.y >= self->startPos.y ? FLIP_Y : 0);
            }
            else {
                targetX = reciever->position.x + 0x140000 * (reciever->startPos.x >= self->startPos.x ? 1 : -1);
                targetY = reciever->position.y + 0x220000 * ((reciever->direction & FLIP_Y) ? 1 : -1);
                sawDir  = (reciever->direction & FLIP_Y) | (reciever->startPos.x >= self->startPos.x ? FLIP_X : FLIP_NONE);
            }

            saw->direction    = sawDir;
            int32 targetAngle = RSDK.ATan2(targetX - saw->position.x, targetY - saw->position.y);
            saw->velocity.x   = self->sawSpeed * RSDK.Cos256(targetAngle);
            saw->velocity.y   = self->sawSpeed * RSDK.Sin256(targetAngle);
            saw->friends[0]   = reciever;
            saw->hasSaw       = JUGGLESAW_HAS_SAW;
            saw->active       = ACTIVE_NORMAL;
            self->sawTimer    = 0;
            self->hasSaw      = JUGGLESAW_NO_SAW;
        }
        else {
            RSDK.SetSpriteAnimation(JuggleSaw->aniFrames, self->startDir >= FLIP_Y ? 4 : 1, &self->animator, true, 0);
            self->state = JuggleSaw_StateCrab_Handle;
        }

        self->friends[0] = NULL;
    }

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(JuggleSaw->aniFrames, self->startDir >= FLIP_Y ? 3 : 0, &self->animator, true, 0);
        self->state = JuggleSaw_StateCrab_Handle;
    }

    JuggleSaw_CheckPlayerCollisions();
    JuggleSaw_CheckOffScreen();
}

void JuggleSaw_StateSaw_Handle(void)
{
    RSDK_THIS(JuggleSaw);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    EntityJuggleSaw *reciever = self->friends[0];
    int32 oldDir              = reciever->direction;
    reciever->direction       = self->direction;

    Hitbox *hitboxGrab = &JuggleSaw->hitboxGrabV;
    if (reciever->startDir >= FLIP_Y)
        hitboxGrab = &JuggleSaw->hitboxGrabH;

    // I have decided to imortalize this bit of code that the below code replaces because this cost RMG and I at least 4 hours of our lives
    // collectively.
    // reciever->direction = oldDir;
    // if (RSDK.CheckObjectCollisionTouchBox(reciever, hitboxGrab, self, &JuggleSaw->hitboxSaw)) {

    bool32 collided     = RSDK.CheckObjectCollisionTouchBox(reciever, hitboxGrab, self, &JuggleSaw->hitboxSaw);
    reciever->direction = oldDir;

    if (collided) {
        int32 newDir = 0;
        if (reciever->startDir >= FLIP_Y) {
            if (self->velocity.y > 0)
                newDir = oldDir | FLIP_Y;
            else if (self->velocity.y < 0)
                newDir = oldDir & ~FLIP_Y;
        }
        else {
            if (self->velocity.x > 0)
                newDir = oldDir | FLIP_X;
            else if (self->velocity.x < 0)
                newDir = oldDir & ~FLIP_X;
        }

        reciever->direction = newDir;

        reciever->hasSaw = JUGGLESAW_HAS_SAW;
        RSDK.SetSpriteAnimation(JuggleSaw->aniFrames, reciever->startDir >= FLIP_Y ? 4 : 1, &reciever->animator, true, 0);
        destroyEntity(self);
    }
    else if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        RSDK.ProcessAnimation(&self->animator);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &JuggleSaw->hitboxSaw)) {
#if MANIA_USE_PLUS
                if (Player_CheckMightyShellHit(player, self, -0x400, -0x600)) {
                    self->interaction = false;
                    self->state       = JuggleSaw_StateSaw_Debris;
                }
                else
#endif
                    Player_Hurt(player, self);
            }
        }
    }
    else if (!self->friends[0] || self->friends[0]->classID != JuggleSaw->classID)
        destroyEntity(self);
}

void JuggleSaw_StateSaw_Debris(void)
{
    RSDK_THIS(JuggleSaw);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->visible ^= true;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void JuggleSaw_EditorDraw(void)
{
    RSDK_THIS(JuggleSaw);

    int32 dir = self->direction;

    switch (self->direction) {
        case FLIP_NONE: RSDK.SetSpriteAnimation(JuggleSaw->aniFrames, self->hasSaw == JUGGLESAW_HAS_SAW, &self->animator, true, 0); break;

        case FLIP_X:
            self->direction = FLIP_Y;
            RSDK.SetSpriteAnimation(JuggleSaw->aniFrames, self->hasSaw == JUGGLESAW_HAS_SAW, &self->animator, true, 0);
            break;

        case FLIP_Y:
            self->direction = FLIP_NONE;
            RSDK.SetSpriteAnimation(JuggleSaw->aniFrames, 4 - (self->hasSaw != JUGGLESAW_HAS_SAW), &self->animator, true, 0);
            break;

        case FLIP_XY:
            self->direction = FLIP_X;
            RSDK.SetSpriteAnimation(JuggleSaw->aniFrames, 4 - (self->hasSaw != JUGGLESAW_HAS_SAW), &self->animator, true, 0);
            break;

        default: break;
    }

    JuggleSaw_Draw();

    if (self->hasSaw == JUGGLESAW_HAS_SAW && showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &JuggleSaw->hitboxFriendRange, self->direction, 0xFF0000);

        RSDK_DRAWING_OVERLAY(false);
    }

    self->direction = dir;
}

void JuggleSaw_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        JuggleSaw->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/JuggleSaw.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("PSZ2"))
        JuggleSaw->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/JuggleSaw.bin", SCOPE_STAGE);

    JuggleSaw->hitboxFriendRange.left   = -512;
    JuggleSaw->hitboxFriendRange.top    = -512;
    JuggleSaw->hitboxFriendRange.right  = 512;
    JuggleSaw->hitboxFriendRange.bottom = 512;

    RSDK_ACTIVE_VAR(JuggleSaw, direction);
    RSDK_ENUM_VAR("No Flip (Vertical)", FLIP_NONE);
    RSDK_ENUM_VAR("Flipped (Vertical)", FLIP_X);
    RSDK_ENUM_VAR("No Flip (Horizontal)", FLIP_Y);
    RSDK_ENUM_VAR("Flipped (Horizontal)", FLIP_XY);
}
#endif

void JuggleSaw_Serialize(void)
{
    RSDK_EDITABLE_VAR(JuggleSaw, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(JuggleSaw, VAR_UINT16, sawDelay);
    RSDK_EDITABLE_VAR(JuggleSaw, VAR_ENUM, sawSpeed);
    RSDK_EDITABLE_VAR(JuggleSaw, VAR_BOOL, hasSaw); // liars.
    RSDK_EDITABLE_VAR(JuggleSaw, VAR_UINT16, setID);
}
