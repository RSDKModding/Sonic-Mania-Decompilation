// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Press Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPress *Press;

void Press_Update(void)
{
    RSDK_THIS(Press);

    StateMachine_Run(self->state);
}

void Press_LateUpdate(void) {}

void Press_StaticUpdate(void)
{
#if MANIA_USE_PLUS
    Press->canSuper      = true;
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player->classID == Player->classID) {
        foreach_active(Press, press)
        {
            Hitbox hitboxRange;
            hitboxRange.top    = -4 * press->size;
            hitboxRange.bottom = 4 * press->size;
            hitboxRange.left   = -112;
            hitboxRange.right  = 112;
            if (Player_CheckCollisionTouch(player, press, &hitboxRange)) {
                Press->canSuper = false;
                foreach_break;
            }
        }
    }
    else
        Press->canSuper = false;
#endif
}

void Press_Draw(void)
{
    RSDK_THIS(Press);

    if (self->rotation <= 0x100)
        Press_DrawHandle();

    // threads
    Vector2 drawPos    = self->drawPos;
    SpriteFrame *frame = RSDK.GetFrame(Press->aniFrames, 1, self->threadAnimator.frameID);
    frame->height      = 56;
    frame->sprY        = (self->threadSprY >> 8) + 182;
    for (uint32 i = 0; i < self->threads; ++i) {
        RSDK.DrawSprite(&self->threadAnimator, &drawPos, false);
        drawPos.y += 0x380000;
    }
    frame->height = self->height;
    RSDK.DrawSprite(&self->threadAnimator, &drawPos, false);

    // crusher platforms
    drawPos = self->position;
    drawPos.y += self->offTop;
    self->crusherAnimator.frameID = 3;
    RSDK.DrawSprite(&self->crusherAnimator, &drawPos, false);

    self->crusherAnimator.frameID = 4;
    RSDK.DrawSprite(&self->crusherAnimator, &drawPos, false);

    drawPos.y -= self->offTop;
    drawPos.y += self->offBottom;
    self->crusherAnimator.frameID = 3;
    RSDK.DrawSprite(&self->crusherAnimator, &drawPos, false);

    self->crusherAnimator.frameID = 5;
    RSDK.DrawSprite(&self->crusherAnimator, &drawPos, false);

    // bumper
    drawPos.y = self->drawPos.y - 0x80000;
    RSDK.DrawSprite(&self->bumperAnimator, &drawPos, false);

    drawPos.y += (self->size + 16) << 16;
    RSDK.DrawSprite(&self->bumperAnimator, &drawPos, false);

    if (self->rotation > 0x100)
        Press_DrawHandle();
}

void Press_Create(void *data)
{
    RSDK_THIS(Press);

    self->active    = ACTIVE_BOUNDS;
    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];

    if (!SceneInfo->inEditor) {
        self->size *= 8;
        int32 size = self->size;
        self->speed <<= 15;
        int32 step    = size / 7;
        int32 count   = (size - step) / 2;
        self->threads = (step + count) / 32;
        self->scale.y = 0x200;
        self->height  = size - 0x38 * self->threads;

        size <<= 15;
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x380000 * self->threads + 0x1000000;
        self->drawPos.x     = self->position.x;
        self->drawPos.y     = self->position.y - size;
        self->offTop        = (self->offTop << 16) - size;
        self->offBottom     = (self->offBottom << 16) - size + 0xFFFF;

        RSDK.SetSpriteAnimation(Press->aniFrames, 0, &self->crusherAnimator, true, 0);
        RSDK.SetSpriteAnimation(Press->aniFrames, 1, &self->threadAnimator, true, 0);
        RSDK.SetSpriteAnimation(Press->aniFrames, 2, &self->bumperAnimator, true, 0);
        self->state = Press_State_Crush;
    }
}

void Press_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        Press->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Press.bin", SCOPE_STAGE);

    Press->hitbox.left   = -112;
    Press->hitbox.top    = -16;
    Press->hitbox.right  = 112;
    Press->hitbox.bottom = 16;

#if MANIA_USE_PLUS
    Player->canSuperCB = Press_CheckCanSuper;
#endif

    Press->sfxImpact = RSDK.GetSfx("Stage/Impact2.wav");
    Press->sfxPress  = RSDK.GetSfx("PSZ/Press.wav");
}

#if MANIA_USE_PLUS
bool32 Press_CheckCanSuper(bool32 isHUD) { return Press->canSuper; }
#endif

void Press_DrawHandle(void)
{
    RSDK_THIS(Press);

    Vector2 drawPos = self->drawPos;

    self->scale.x = abs(RSDK.Cos512(self->rotation)) + 1;
    int32 scaleX  = abs(RSDK.Sin512(self->rotation)) + 1;

    drawPos.x += 0x2500 * RSDK.Cos512(self->rotation);
    drawPos.y -= 0x80000;
    self->crusherAnimator.frameID = 0;
    RSDK.DrawSprite(&self->crusherAnimator, &drawPos, false);

    self->drawFX |= FX_SCALE;
    drawPos.x = 0x1B80 * RSDK.Cos512(self->rotation) + self->drawPos.x;

    switch (self->rotation >> 7) {
        case 0:
        case 2:
            drawPos.x += (scaleX << 9);
            self->crusherAnimator.frameID = 2;
            RSDK.DrawSprite(&self->crusherAnimator, &drawPos, false);

            drawPos.x += -0xC00 * self->scale.x - (scaleX << 9);
            break;

        case 1:
        case 3:
            drawPos.x -= (scaleX << 9);
            self->crusherAnimator.frameID = 2;
            RSDK.DrawSprite(&self->crusherAnimator, &drawPos, false);

            drawPos.x += (scaleX + 2 * (3 * self->scale.x - 32)) << 9;
            break;

        default: break;
    }

    self->scale.x                 = scaleX;
    self->crusherAnimator.frameID = 1;
    RSDK.DrawSprite(&self->crusherAnimator, &drawPos, false);

    self->drawFX &= ~FX_SCALE;
}

void Press_Move(void)
{
    RSDK_THIS(Press);

    self->stoodPlayersRoof  = 0;
    self->stoodPlayersFloor = 0;

    int32 playerID = 0;
    foreach_active(Player, player)
    {
        self->position.y += self->offBottom;

        if (Player_CheckCollisionBox(player, self, &Press->hitbox) == C_TOP) {
            if (self->state == Press_State_Crush && !player->sidekick) {
                if (abs(self->position.x - player->position.x) <= 0x600000) {
                    RSDK.PlaySfx(Press->sfxPress, false, 255);
                    self->state  = Press_HandleMovement;
                    self->active = ACTIVE_NORMAL;
                }
            }

            self->stoodPlayersFloor |= 1 << playerID;
        }

        self->position.y += self->offTop - self->offBottom;

        int32 collide = Player_CheckCollisionBox(player, self, &Press->hitbox);
        if (collide == C_BOTTOM)
            player->collisionFlagV |= 2;
        else if (collide == C_TOP)
            self->stoodPlayersRoof |= 1 << playerID;

        ++playerID;
        self->position.y -= self->offTop;
    }
}

void Press_State_Crush(void)
{
    // fun press fact!
    Press_Move();
}
void Press_State_FinalCrush(void)
{
    // every second you don't move the press only gets closer
    Press_Move();
}
void Press_HandleMovement(void)
{
    RSDK_THIS(Press);

    RSDK.ProcessAnimation(&self->threadAnimator);
    RSDK.ProcessAnimation(&self->bumperAnimator);

    Press_Move();

    int32 oldBottom = self->offBottom;
    int32 newBottom = oldBottom - self->speed;
    self->offBottom = oldBottom - self->speed;

    self->threadSprY += (self->speed >> 11);
    self->threadSprY &= 0x7FF;

    int32 oldTop = self->offTop;
    int32 newTop = self->speed + self->offTop + self->topOffset;

    self->rotation  = (self->rotation - (self->speed >> 15)) & 0x1FF;
    self->offTop    = newTop;
    self->topOffset = 0;

    if (newTop + 0x100000 >= newBottom - 0x100000) {
        int32 diff = newTop - newBottom + 0x200000;
        if (diff > 0) {
            diff >>= 1;
            self->offTop    = newTop - diff;
            self->offBottom = newBottom + diff;
        }

        RSDK.PlaySfx(Press->sfxImpact, false, 255);
        self->active = ACTIVE_BOUNDS;
        Camera_ShakeScreen(0, 0, 5);
        self->state = Press_State_FinalCrush;
    }

    uint32 waitTime = 0;
    self->topOffset = self->offTop;

    bool32 top = false, bottom = false;
    int32 floorOffset = (oldBottom & 0xFFFF0000) - (self->offBottom & 0xFFFF0000);
    int32 actualPos   = self->position.y;

    foreach_active(Crate, crate)
    {
        self->position.y += self->offBottom;

        if (MathHelpers_CheckBoxCollision(self, &Press->hitbox, crate, &crate->hitbox) == C_TOP) {
            bottom = true;
            Crate_MoveY(crate, -floorOffset);
        }

        self->position.y += self->offTop - self->offBottom;
        if (MathHelpers_CheckBoxCollision(crate, &crate->hitbox, self, &Press->hitbox) == C_TOP) {
            top = true;

            switch (crate->frameID) {
                default: break;
                case 0: waitTime += 60; break;
                case 1: waitTime += 30; break;
                case 2: waitTime += 90; break;
                case 3: waitTime += 60; break;
            }
        }

        self->offTop     = self->position.y - actualPos;
        self->position.y = actualPos;
    }

    self->topOffset -= self->offTop;

    if (bottom && top) {
        RSDK.PlaySfx(Press->sfxImpact, false, 255);
        Camera_ShakeScreen(0, 0, 3);

        self->state      = Press_State_HandleCrates;
        self->timerStart = waitTime;
        self->timer      = waitTime;
    }

    int32 playerID   = 1;
    int32 roofOffset = (oldTop & 0xFFFF0000) - (self->offTop & 0xFFFF0000);
    foreach_active(Player, player)
    {
        if (playerID & self->stoodPlayersFloor)
            player->position.y -= floorOffset;
        if (playerID & self->stoodPlayersRoof)
            player->position.y -= roofOffset;

        playerID <<= 1;
    }
}

void Press_State_HandleCrates(void)
{
    RSDK_THIS(Press);

    Press_Move();

    if (self->timer <= 0) {
        foreach_active(Crate, crate)
        {
            self->position.y += self->offTop + 0x80000;

            if (RSDK.CheckObjectCollisionTouchBox(self, &Press->hitbox, crate, &crate->hitbox))
                Crate_Break(crate);

            self->position.y += -0x80000 - self->offTop;
        }

        self->state = Press_HandleMovement;
    }
    else {
        int32 percentDone = ((self->timerStart - self->timer) << 16) / self->timerStart;
        int32 crateOff    = 0;

        foreach_active(Crate, crate)
        {
            self->position.y += self->offTop + 0x80000;
            if (RSDK.CheckObjectCollisionTouchBox(self, &Press->hitbox, crate, &crate->hitbox) && percentDone > 0x8000) {
                int32 percent  = percentDone >> 9;
                int32 percent2 = percent * percent;
                int32 angle    = crateOff + ((percent * self->timerStart * (percent2 >> 8)) >> 8);

                crateOff += 0x100;
                crate->drawPos.x = (RSDK.Sin512(angle & 0x1FF) << 7) + crate->centerPos.x;
            }

            self->position.y += -0x80000 - self->offTop;
        }

        --self->timer;
    }
}

#if GAME_INCLUDE_EDITOR
void Press_EditorDraw(void)
{
    RSDK_THIS(Press);

    int32 sizeStore = self->size;
    int32 offT      = self->offTop;
    int32 offB      = self->offBottom;

    self->size *= 8;
    int32 size = self->size;

    int32 step    = size / 7;
    int32 count   = (size - step) / 2;
    self->threads = (step + count) / 32;
    self->scale.y = 0x200;
    self->height  = size - 0x38 * self->threads;

    size <<= 15;
    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x380000 * self->threads + 0x1000000;
    self->drawPos.x     = self->position.x;
    self->drawPos.y     = self->position.y - size;
    self->offTop        = (self->offTop << 16) - size;
    self->offBottom     = (self->offBottom << 16) - size + 0xFFFF;

    RSDK.SetSpriteAnimation(Press->aniFrames, 0, &self->crusherAnimator, true, 0);
    RSDK.SetSpriteAnimation(Press->aniFrames, 1, &self->threadAnimator, true, 0);
    RSDK.SetSpriteAnimation(Press->aniFrames, 2, &self->bumperAnimator, true, 0);

    Press_Draw();

    self->size      = sizeStore;
    self->offTop    = offT;
    self->offBottom = offB;
}

void Press_EditorLoad(void) { Press->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Press.bin", SCOPE_STAGE); }
#endif

void Press_Serialize(void)
{
    RSDK_EDITABLE_VAR(Press, VAR_UINT16, size);
    RSDK_EDITABLE_VAR(Press, VAR_UINT32, speed);
    RSDK_EDITABLE_VAR(Press, VAR_ENUM, offTop);
    RSDK_EDITABLE_VAR(Press, VAR_ENUM, offBottom);
}
