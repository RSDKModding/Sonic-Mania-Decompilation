#include "SonicMania.h"

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
    self->drawOrder     = Zone->drawOrderLow;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0xC00000;
    self->updateRange.y = 0xC00000;
    if (!SceneInfo->inEditor) {
        if (data) {
            RSDK.SetSpriteAnimation(JuggleSaw->animID, 6, &self->animator, true, 0);
            self->state = JuggleSaw_Saw_Handle;
        }
        else {
            self->spawnPos.x = self->position.x;
            self->spawnPos.y = self->position.y;
            self->spawnDir   = self->direction;
            switch (self->direction) {
                case FLIP_NONE: RSDK.SetSpriteAnimation(JuggleSaw->animID, self->hasSaw == JSAW_HAS_SAW, &self->animator, true, 0); break;
                case FLIP_X:
                    self->direction = FLIP_Y;
                    RSDK.SetSpriteAnimation(JuggleSaw->animID, self->hasSaw == JSAW_HAS_SAW, &self->animator, true, 0);
                    break;
                case FLIP_Y:
                    self->direction = FLIP_NONE;
                    RSDK.SetSpriteAnimation(JuggleSaw->animID, 4 - (self->hasSaw != JSAW_HAS_SAW), &self->animator, true, 0);
                    break;
                case FLIP_XY:
                    self->direction = FLIP_X;
                    RSDK.SetSpriteAnimation(JuggleSaw->animID, 4 - (self->hasSaw != JSAW_HAS_SAW), &self->animator, true, 0);
                    break;
                default: break;
            }
            if (!self->sawSpeed)
                self->sawSpeed = 8;
            self->sawSpeed <<= 7;
            self->state = JuggleSaw_Crab_Create;
        }
    }
}

void JuggleSaw_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        JuggleSaw->animID = RSDK.LoadSpriteAnimation("PSZ1/JuggleSaw.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("PSZ2"))
        JuggleSaw->animID = RSDK.LoadSpriteAnimation("PSZ2/JuggleSaw.bin", SCOPE_STAGE);

    JuggleSaw->hitbox.left   = -12;
    JuggleSaw->hitbox.top    = -18;
    JuggleSaw->hitbox.right  = 12;
    JuggleSaw->hitbox.bottom = 18;

    JuggleSaw->friendbox.left   = -0x200;
    JuggleSaw->friendbox.top    = -0x200;
    JuggleSaw->friendbox.right  = 0x200;
    JuggleSaw->friendbox.bottom = 0x200;

    JuggleSaw->sawHitbox.left   = -12;
    JuggleSaw->sawHitbox.top    = -12;
    JuggleSaw->sawHitbox.right  = 12;
    JuggleSaw->sawHitbox.bottom = 12;

    JuggleSaw->grabboxFloor.left   = -24;
    JuggleSaw->grabboxFloor.top    = -38;
    JuggleSaw->grabboxFloor.right  = -16;
    JuggleSaw->grabboxFloor.bottom = -30;

    // yes im SURE this is accurate
    JuggleSaw->grabboxWall.left   = 38;
    JuggleSaw->grabboxWall.top    = -24;
    JuggleSaw->grabboxWall.right  = 30;
    JuggleSaw->grabboxWall.bottom = -16;

    DEBUGMODE_ADD_OBJ(JuggleSaw);
    JuggleSaw->explodeSFX = RSDK.GetSfx("Stage/Explosion.wav");
    JuggleSaw->juggleSFX  = RSDK.GetSfx("PSZ/Juggle.wav");
    JuggleSaw->throwSFX   = RSDK.GetSfx("PSZ/JuggleThrow.wav");
}

void JuggleSaw_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(JuggleSaw, NULL, self->position.x, self->position.y)->sawDelay = 30;
}

void JuggleSaw_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(JuggleSaw->animID, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void JuggleSaw_Crab_Collide(void)
{
    RSDK_THIS(JuggleSaw);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &JuggleSaw->hitbox) && Player_CheckBadnikBreak(self, player, false)) {
            if (self->hasSaw == JSAW_HAS_SAW) {
                int32 debrisX = self->position.x;
                int32 debrisY = self->position.y;
                if (self->spawnDir >= FLIP_Y)
                    debrisX += 0x200000 * ((self->direction & FLIP_X) ? -1 : 1);
                else
                    debrisY += 0x200000 * ((self->direction & FLIP_Y) ? 1 : -1);
                EntityDebris *debris = CREATE_ENTITY(Debris, (void *)Debris_State_FallAndFlicker, debrisX, debrisY);
                RSDK.SetSpriteAnimation(JuggleSaw->animID, 6, &debris->animator, true, 0);
                int32 vx1 = -4, vx2 = 5, vy1 = -4, vy2 = 5;
                if (self->spawnDir >= FLIP_Y) {
                    if (self->direction & FLIP_X)
                        vx2 = -1;
                    else
                        vx1 = -4;
                }
                else {
                    if (self->direction & FLIP_Y)
                        vy1 = -4;
                    else
                        vy2 = -1;
                }
                debris->velocity.x    = RSDK.Rand(vx1, vx2) << 16;
                debris->velocity.y    = RSDK.Rand(vy1, vy2) << 16;
                debris->gravity       = 0x3800;
                debris->drawOrder     = self->drawOrder;
                debris->updateRange.x = 0x400000;
                debris->updateRange.y = 0x400000;
            }
            destroyEntity(self);
        }
    }
}

void JuggleSaw_Crab_CheckOnScreen(void)
{
    RSDK_THIS(JuggleSaw);
    if (self->hasSaw != JSAW_AWAITING_SAW && !RSDK.CheckOnScreen(self, 0) && !RSDK.CheckPosOnScreen(&self->spawnPos, &self->updateRange)) {
        self->sawSpeed   = self->sawSpeed >> 7;
        self->position.x = self->spawnPos.x;
        self->position.y = self->spawnPos.y;
        self->direction  = self->spawnDir;
        JuggleSaw_Create(NULL);
    }
}

void JuggleSaw_Crab_Handle(void)
{
    // this was the worst func to decompile so far. -rmg
    RSDK_THIS(JuggleSaw);
    RSDK.ProcessAnimation(&self->animator);
    switch (self->spawnDir) {
        case FLIP_NONE:
        case FLIP_X: self->position.x = (RSDK.Cos256(self->angle) << 10) + self->spawnPos.x; break;
        case FLIP_Y:
        case FLIP_XY: self->position.y = (RSDK.Cos256(self->angle) << 10) + self->spawnPos.y; break;
        default: break;
    }

    if (self->hasSaw != JSAW_HAS_SAW) {
        self->angle += 2;
    }
    else {
        self->angle += 4;
        if (self->animator.frameID == self->animator.frameCount - 1) {
            RSDK.PlaySfx(JuggleSaw->juggleSFX, 0, 255);
            if (self->spawnDir >= FLIP_Y)
                self->direction ^= FLIP_Y;
            else
                self->direction ^= FLIP_X;
        }
        else {
            if (self->sawTimer == self->sawDelay - 1)
                --self->sawTimer;
        }

        if (++self->sawTimer >= self->sawDelay) {
            bool32 flag = true;

            if (!self->friends[0] || self->friends[0]->objectID != self->objectID) {
                self->friendCount = 0;
                foreach_active(JuggleSaw, newFriend)
                {
                    if (newFriend != self && newFriend->hasSaw == JSAW_NO_SAW && newFriend->setID == self->setID
                        && self->friendCount < JuggleSaw_MaxFriends) {
                        if (RSDK.CheckObjectCollisionTouchBox(newFriend, &JuggleSaw->hitbox, self, &JuggleSaw->friendbox))
                            self->friends[self->friendCount++] = (Entity *)newFriend;
                    }
                }

                if (self->friendCount) {
                    self->friends[0] = self->friends[RSDK.Rand(0, self->friendCount)];
                    bool32 throwCheck  = false;
                    if (self->spawnDir >= FLIP_Y) {
                        if (self->friends[0]->position.y < self->position.y) {
                            if (!(self->direction & FLIP_Y))
                                --self->sawTimer;
                        }
                        else
                            throwCheck = !(self->direction & FLIP_Y);
                    }
                    else {
                        if (self->friends[0]->position.x < self->position.x) {
                            if (!(self->direction & FLIP_X))
                                --self->sawTimer;
                        }
                        else
                            throwCheck = !(self->direction & FLIP_X);
                    }

                    flag = false;
                    if (throwCheck && self->sawTimer >= self->sawDelay) {
                        flag = true;
                    }
                }
                else {
                    flag = false;
                }
            }

            if (flag) {
                RSDK.SetSpriteAnimation(JuggleSaw->animID, self->spawnDir >= FLIP_Y ? 5 : 2, &self->animator, true, 0);
                if (self->spawnDir >= FLIP_Y)
                    self->direction ^= FLIP_Y;
                else
                    self->direction ^= FLIP_X;
                self->state = JuggleSaw_Crab_ThrowSaw;
            }
        }
    }
    JuggleSaw_Crab_Collide();
    JuggleSaw_Crab_CheckOnScreen();
}

void JuggleSaw_Crab_Create(void)
{
    RSDK_THIS(JuggleSaw);
    self->active     = ACTIVE_NORMAL;
    self->friends[0] = NULL;
    self->angle      = 0;
    self->sawTimer   = 0;
    self->state      = JuggleSaw_Crab_Handle;
    JuggleSaw_Crab_Handle();
}

void JuggleSaw_Crab_ThrowSaw(void)
{
    RSDK_THIS(JuggleSaw);
    Animator *animator = &self->animator;
    RSDK.ProcessAnimation(animator);
    if (animator->frameID == 3) {
        EntityJuggleSaw *reciever = (EntityJuggleSaw *)self->friends[0];
        if (reciever->objectID == JuggleSaw->objectID) {
            RSDK.PlaySfx(JuggleSaw->throwSFX, 0, 255);
            reciever->hasSaw    = JSAW_AWAITING_SAW;
            reciever->active     = ACTIVE_NORMAL;
            EntityJuggleSaw *saw = CREATE_ENTITY(JuggleSaw, intToVoid(1), self->position.x, self->position.y);
            int32 sx = 0x2C0000, sy = -0xE0000;
            if (self->spawnDir >= FLIP_Y) {
                sx = 0xE0000;
                sy = 0x2C0000;
            }
            saw->position.x += sx * ((self->direction & FLIP_X) ? -1 : 1);
            saw->position.y += sy * ((self->direction & FLIP_Y) ? -1 : 1);

            int32 recieverX   = reciever->position.x, targetX;
            int32 recieverY   = reciever->position.y, targetY;
            int32 recieverDir = reciever->direction, sawDir = 0;
            if (reciever->spawnDir >= FLIP_Y) {
                targetX = recieverX + 0x220000 * ((recieverDir & FLIP_X) ? -1 : 1);
                targetY = recieverY + 0x140000 * (reciever->spawnPos.y >= self->spawnPos.y ? 1 : -1);
                sawDir  = (recieverDir & FLIP_X) | (reciever->spawnPos.y >= self->spawnPos.y ? FLIP_Y : 0);
            }
            else {
                targetX = recieverX + 0x140000 * (reciever->spawnPos.x >= self->spawnPos.x ? 1 : -1);
                targetY = recieverY + 0x220000 * ((recieverDir & FLIP_Y) ? 1 : -1);
                sawDir  = (recieverDir & FLIP_Y) | (reciever->spawnPos.x >= self->spawnPos.x ? FLIP_X : FLIP_NONE);
            }

            saw->direction   = sawDir;
            int32 targetAngle  = RSDK.ATan2(targetX - saw->position.x, targetY - saw->position.y);
            saw->velocity.x  = self->sawSpeed * RSDK.Cos256(targetAngle);
            saw->velocity.y  = self->sawSpeed * RSDK.Sin256(targetAngle);
            saw->friends[0]  = (Entity*)reciever;
            saw->hasSaw     = JSAW_HAS_SAW;
            saw->active      = ACTIVE_NORMAL;
            self->sawTimer = 0;
            self->hasSaw  = JSAW_NO_SAW;
        }
        else {
            RSDK.SetSpriteAnimation(JuggleSaw->animID, self->spawnDir >= FLIP_Y ? 4 : 1, animator, true, 0);
            self->state = JuggleSaw_Crab_Handle;
        }
        self->friends[0] = NULL;
    }
    if (animator->frameID == animator->frameCount - 1) {
        RSDK.SetSpriteAnimation(JuggleSaw->animID, self->spawnDir >= FLIP_Y ? 3 : 0, animator, true, 0);
        self->state = JuggleSaw_Crab_Handle;
    }
    JuggleSaw_Crab_Collide();
    JuggleSaw_Crab_CheckOnScreen();
}

void JuggleSaw_Saw_Handle(void)
{
    RSDK_THIS(JuggleSaw);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    EntityJuggleSaw *reciever = (EntityJuggleSaw *)self->friends[0];
    int32 oldDir                = reciever->direction;
    reciever->direction       = self->direction;
    Hitbox *grabbox           = &JuggleSaw->grabboxFloor;
    if (reciever->spawnDir >= FLIP_Y)
        grabbox = &JuggleSaw->grabboxWall;

    //I have decided to imortalize this bit of code that the below code replaces because this cost RMG and I at least 4 hours of our lives collectively.
    //reciever->direction = oldDir;
    //if (RSDK.CheckObjectCollisionTouchBox(reciever, grabbox, self, &JuggleSaw->sawHitbox)) {

    bool32 collided     = RSDK.CheckObjectCollisionTouchBox(reciever, grabbox, self, &JuggleSaw->sawHitbox);
    reciever->direction = oldDir;
    if (collided) {
        int32 newDir = 0;
        if (reciever->spawnDir >= FLIP_Y) {
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

        reciever->hasSaw = JSAW_HAS_SAW;
        RSDK.SetSpriteAnimation(JuggleSaw->animID, reciever->spawnDir >= FLIP_Y ? 4 : 1, &reciever->animator, true, 0);
        destroyEntity(self);
    }
    else if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        RSDK.ProcessAnimation(&self->animator);
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &JuggleSaw->sawHitbox)) {
#if RETRO_USE_PLUS
                if (Player_CheckMightyShellHit(player, self, -0x400, -0x600)) {
                    self->interaction = false;
                    self->state       = JuggleSaw_Saw_Knocked;
                }
                else
#endif
                    Player_CheckHit(player, self);
            }
        }
    }
    else if (!self->friends[0] || self->friends[0]->objectID != JuggleSaw->objectID)
        destroyEntity(self);
}

void JuggleSaw_Saw_Knocked(void)
{
    RSDK_THIS(JuggleSaw);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    self->visible ^= true;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void JuggleSaw_EditorDraw(void)
{
    RSDK_THIS(JuggleSaw);

    int32 dir = self->direction;

    switch (self->direction) {
        case FLIP_NONE: RSDK.SetSpriteAnimation(JuggleSaw->animID, self->hasSaw == JSAW_HAS_SAW, &self->animator, true, 0); break;
        case FLIP_X:
            self->direction = FLIP_Y;
            RSDK.SetSpriteAnimation(JuggleSaw->animID, self->hasSaw == JSAW_HAS_SAW, &self->animator, true, 0);
            break;
        case FLIP_Y:
            self->direction = FLIP_NONE;
            RSDK.SetSpriteAnimation(JuggleSaw->animID, 4 - (self->hasSaw != JSAW_HAS_SAW), &self->animator, true, 0);
            break;
        case FLIP_XY:
            self->direction = FLIP_X;
            RSDK.SetSpriteAnimation(JuggleSaw->animID, 4 - (self->hasSaw != JSAW_HAS_SAW), &self->animator, true, 0);
            break;
        default: break;
    }

    JuggleSaw_Draw();

    self->direction = dir;
}

void JuggleSaw_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        JuggleSaw->animID = RSDK.LoadSpriteAnimation("PSZ1/JuggleSaw.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("PSZ2"))
        JuggleSaw->animID = RSDK.LoadSpriteAnimation("PSZ2/JuggleSaw.bin", SCOPE_STAGE);

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
