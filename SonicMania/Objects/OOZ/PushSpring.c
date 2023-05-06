// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PushSpring Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPushSpring *PushSpring;

void PushSpring_Update(void)
{
    RSDK_THIS(PushSpring);

    StateMachine_Run(self->state);

    self->beingPushed = false;
    StateMachine_Run(self->stateCollide);
}

void PushSpring_LateUpdate(void) {}

void PushSpring_StaticUpdate(void) {}

void PushSpring_Draw(void)
{
    RSDK_THIS(PushSpring);

    if (self->pushOffset) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        self->animator.frameID = 0;
        RSDK.DrawSprite(&self->animator, NULL, false);
    }
}

void PushSpring_Create(void *data)
{
    RSDK_THIS(PushSpring);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        RSDK.SetSpriteAnimation(PushSpring->aniFrames, self->type, &self->animator, true, 0);

        if (self->type != PUSHSPRING_V) {
            if (self->direction) {
                self->stateDraw    = PushSpring_Draw_Left;
                self->stateCollide = PushSpring_Collide_Left;
            }
            else {
                self->stateDraw    = PushSpring_Draw_Right;
                self->stateCollide = PushSpring_Collide_Right;
            }

            self->groundVel = 0x4000;

            self->hitbox.left   = -20;
            self->hitbox.top    = -16;
            self->hitbox.right  = 20;
            self->hitbox.bottom = 16;

            self->state = PushSpring_State_WaitForPushed;
        }
        else {
            if (self->direction) {
                self->direction    = FLIP_Y;
                self->stateDraw    = PushSpring_Draw_Bottom;
                self->stateCollide = PushSpring_Collide_Bottom;
            }
            else {
                self->stateDraw    = PushSpring_Draw_Top;
                self->stateCollide = PushSpring_Collide_Top;
            }

            self->groundVel = 0x20000;

            self->hitbox.left   = -16;
            self->hitbox.top    = -20;
            self->hitbox.right  = 16;
            self->hitbox.bottom = 20;

            self->state = PushSpring_State_WaitForPushed;
        }
    }
}

void PushSpring_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("OOZ1") || RSDK.CheckSceneFolder("OOZ2"))
        PushSpring->aniFrames = RSDK.LoadSpriteAnimation("OOZ/PushSpring.bin", SCOPE_STAGE);

    PushSpring->sfxPush   = RSDK.GetSfx("Stage/Push.wav");
    PushSpring->sfxSpring = RSDK.GetSfx("Global/Spring.wav");
}

void PushSpring_Collide_Top(void)
{
    RSDK_THIS(PushSpring);

    self->hitbox.top = (self->pushOffset >> 16) - 22;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {
            player->position.y += 0x20000;
            self->beingPushed |= true;

            if (self->pushOffset >= 0x120000) {
                player->collisionMode = 0;
                player->onGround      = false;
                player->state         = Player_State_Air;
                player->velocity.y    = -0xA0000;

                int32 anim = player->animator.animationID;
                if (anim == ANI_WALK || (anim > ANI_AIR_WALK && anim <= ANI_DASH))
                    player->animationReserve = player->animator.animationID;
                else
                    player->animationReserve = ANI_WALK;

                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_DIAGONAL, &player->animator, true, 0);
                RSDK.PlaySfx(PushSpring->sfxSpring, false, 255);
                self->state = PushSpring_State_PushRecoil;
            }
        }
    }
}

void PushSpring_Collide_Bottom(void)
{
    RSDK_THIS(PushSpring);

    self->hitbox.top = (self->pushOffset >> 16) - 22;

    foreach_active(Player, player)
    {
        int32 yvel = player->velocity.y;

        if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_BOTTOM) {
            if (yvel < 0) {
                player->velocity.y = yvel + 0x3800;
                player->position.y -= 0x20000;
                self->beingPushed |= true;
            }

            if (self->pushOffset >= 0x120000) {
                player->collisionMode = 0;
                player->onGround      = false;
                player->state         = Player_State_Air;
                player->velocity.y    = 0xA0000;
                RSDK.PlaySfx(PushSpring->sfxSpring, false, 255);
                self->state = PushSpring_State_PushRecoil;
            }
        }
    }
}

void PushSpring_Collide_Left(void)
{
    RSDK_THIS(PushSpring);

    self->hitbox.right = 22 - (self->pushOffset >> 16);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_LEFT) {
            if (player->direction == FLIP_NONE)
                player->position.x += 0x20000;

            if (player->right)
                self->beingPushed |= true;

            if (self->state == PushSpring_State_PushRecoil) {
                if (self->pushOffset > 0x10000) {
                    player->groundVel     = -12 * self->pushOffset / 18;
                    player->velocity.x    = player->groundVel;
                    player->collisionMode = 0;
                    player->controlLock   = 16;
                    player->pushing       = false;
                    player->direction     = self->direction;
                    player->state         = Player_State_Ground;
                    RSDK.PlaySfx(PushSpring->sfxSpring, false, 255);
                }
            }
        }
    }
}

void PushSpring_Collide_Right(void)
{
    RSDK_THIS(PushSpring);

    self->hitbox.right = 22 - (self->pushOffset >> 16);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_RIGHT) {
            if (player->direction == FLIP_X)
                player->position.x -= 0x20000;

            if (player->left)
                self->beingPushed |= true;

            if (self->state == PushSpring_State_PushRecoil) {
                if (self->pushOffset > 0x10000) {
                    player->groundVel     = 12 * self->pushOffset / 18;
                    player->velocity.x    = player->groundVel;
                    player->collisionMode = 0;
                    player->controlLock   = 16;
                    player->pushing       = false;
                    player->direction     = self->direction;
                    player->state         = Player_State_Ground;
                    RSDK.PlaySfx(PushSpring->sfxSpring, false, 255);
                }
            }
        }
    }
}

void PushSpring_Draw_Top(void)
{
    RSDK_THIS(PushSpring);

    Vector2 drawPos = self->position;
    drawPos.y += self->pushOffset;
    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.y              = self->position.y + 2 * self->pushOffset / 3;
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.y              = self->position.y + self->pushOffset / 3;
    self->animator.frameID = 3;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 4;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PushSpring_Draw_Bottom(void)
{
    RSDK_THIS(PushSpring);

    Vector2 drawPos = self->position;
    drawPos.y -= self->pushOffset;
    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.y              = self->position.y - 2 * self->pushOffset / 3;
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.y              = self->position.y - self->pushOffset / 3;
    self->animator.frameID = 3;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 4;
    RSDK.DrawSprite(&self->animator, 0, false);
}

void PushSpring_Draw_Left(void)
{
    RSDK_THIS(PushSpring);

    Vector2 drawPos = self->position;
    drawPos.x += self->pushOffset;
    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x              = self->position.x + 2 * self->pushOffset / 3;
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x              = self->position.x + self->pushOffset / 3;
    self->animator.frameID = 3;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 4;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PushSpring_Draw_Right(void)
{
    RSDK_THIS(PushSpring);

    Vector2 drawPos = self->position;
    drawPos.x -= self->pushOffset;
    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x              = self->position.x - 2 * self->pushOffset / 3;
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x              = self->position.x - self->pushOffset / 3;
    self->animator.frameID = 3;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 4;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PushSpring_State_WaitForPushed(void)
{
    RSDK_THIS(PushSpring);

    if (self->beingPushed) {
        self->timer = 0;
        self->state = PushSpring_State_BeingPushed;
    }
}

void PushSpring_State_BeingPushed(void)
{
    RSDK_THIS(PushSpring);

    if (self->beingPushed) {
        self->pushOffset += self->groundVel;

        if (self->pushOffset < 0x120000) {
            if (!(self->timer % 10))
                RSDK.PlaySfx(PushSpring->sfxPush, false, 0xFF);
        }
        else {
            self->pushOffset = 0x120000;
        }

        ++self->timer;
    }
    else {
        self->state = PushSpring_State_PushRecoil;
    }
}

void PushSpring_State_PushRecoil(void)
{
    RSDK_THIS(PushSpring);

    if (self->beingPushed) {
        self->timer = 0;
        self->state = PushSpring_State_BeingPushed;
    }
    else {
        self->pushOffset -= 0x20000;

        if (self->pushOffset <= 0) {
            self->pushOffset = 0;
            self->state      = PushSpring_State_WaitForPushed;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void PushSpring_EditorDraw(void)
{
    RSDK_THIS(PushSpring);

    int32 dir = self->direction;

    RSDK.SetSpriteAnimation(PushSpring->aniFrames, self->type, &self->animator, false, 0);

    if (self->type == PUSHSPRING_V)
        self->direction *= FLIP_Y;

    RSDK.DrawSprite(&self->animator, NULL, false);

    self->direction = dir;
}

void PushSpring_EditorLoad(void)
{
    PushSpring->aniFrames = RSDK.LoadSpriteAnimation("OOZ/PushSpring.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PushSpring, type);
    RSDK_ENUM_VAR("Vertical", PUSHSPRING_V);
    RSDK_ENUM_VAR("Horizontal", PUSHSPRING_H);

    RSDK_ACTIVE_VAR(PushSpring, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flipped", FLIP_X);
}
#endif

void PushSpring_Serialize(void)
{
    RSDK_EDITABLE_VAR(PushSpring, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(PushSpring, VAR_UINT8, direction);
}
