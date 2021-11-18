#include "SonicMania.h"

ObjectPushSpring *PushSpring;

void PushSpring_Update(void)
{
    RSDK_THIS(PushSpring);
    StateMachine_Run(self->state);
    self->field_78 = false;
    StateMachine_Run(self->stateLate);
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
        self->drawOrder     = Zone->drawOrderLow;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(PushSpring->aniFrames, self->type, &self->animator, true, 0);
        if (self->type) {
            if (self->direction) {
                self->stateDraw = PushSpring_StateDraw_Left;
                self->stateLate = PushSpring_HandlePlayerCollisions_Left;
            }
            else {
                self->stateDraw = PushSpring_StateDraw_Right;
                self->stateLate = PushSpring_HandlePlayerCollisions_Right;
            }
            self->groundVel     = 0x4000;
            self->hitbox.left   = -20;
            self->hitbox.top    = -16;
            self->hitbox.right  = 20;
            self->hitbox.bottom = 16;
            self->state         = PushSpring_Unknown9;
        }
        else {
            if (self->direction) {
                self->direction = FLIP_Y;
                self->stateDraw = PushSpring_StateDraw_Bottom;
                self->stateLate = PushSpring_HandlePlayerCollisions_Bottom;
            }
            else {
                self->stateDraw = PushSpring_StateDraw_Top;
                self->stateLate = PushSpring_HandlePlayerCollisions_Top;
            }
            self->groundVel     = 0x20000;
            self->hitbox.left   = -16;
            self->hitbox.top    = -20;
            self->hitbox.right  = 16;
            self->hitbox.bottom = 20;
            self->state         = PushSpring_Unknown9;
        }
    }
}

void PushSpring_StageLoad(void)
{
    if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2"))
        PushSpring->aniFrames = RSDK.LoadSpriteAnimation("OOZ/PushSpring.bin", SCOPE_STAGE);
    PushSpring->sfxPush   = RSDK.GetSFX("Stage/Push.wav");
    PushSpring->sfxSpring = RSDK.GetSFX("Global/Spring.wav");
}

void PushSpring_HandlePlayerCollisions_Top(void)
{
    RSDK_THIS(PushSpring);
    self->hitbox.top = (self->pushOffset >> 16) - 22;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {
            player->position.y += 0x20000;
            self->field_78 |= 1;
            if (self->pushOffset >= 0x120000) {
                player->collisionMode = 0;
                player->onGround      = false;
                player->state         = Player_State_Air;
                player->velocity.y    = -0xA0000;
                int32 anim              = player->playerAnimator.animationID;
                if (anim == ANI_WALK || (anim > ANI_AIRWALK && anim <= ANI_DASH))
                    player->storedAnim = player->playerAnimator.animationID;
                else
                    player->storedAnim = ANI_WALK;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGDIAGONAL, &player->playerAnimator, true, 0);
                RSDK.PlaySfx(PushSpring->sfxSpring, false, 255);
                self->state = PushSpring_Unknown11;
            }
        }
    }
}

void PushSpring_HandlePlayerCollisions_Bottom(void)
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
                self->field_78 |= 1;
            }
            if (self->pushOffset >= 0x120000) {
                player->collisionMode = 0;
                player->onGround      = false;
                player->state         = Player_State_Air;
                player->velocity.y    = 0xA0000;
                RSDK.PlaySfx(PushSpring->sfxSpring, false, 255);
                self->state = PushSpring_Unknown11;
            }
        }
    }
}

void PushSpring_HandlePlayerCollisions_Left(void)
{
    RSDK_THIS(PushSpring);
    self->hitbox.right = 22 - (self->pushOffset >> 16);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_LEFT) {
            if (player->direction == FLIP_NONE)
                player->position.x += 0x20000;
            if (player->right == true)
                self->field_78 |= 1u;
            if (self->state == PushSpring_Unknown11) {
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

void PushSpring_HandlePlayerCollisions_Right(void)
{
    RSDK_THIS(PushSpring);
    self->hitbox.right = 22 - (self->pushOffset >> 16);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_RIGHT) {
            if (player->direction == FLIP_X)
                player->position.x -= 0x20000;
            if (player->left == true)
                self->field_78 |= 1;
            if (self->state == PushSpring_Unknown11) {
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

void PushSpring_StateDraw_Top(void)
{
    RSDK_THIS(PushSpring);

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.y += self->pushOffset;
    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.y                = self->position.y + 2 * self->pushOffset / 3;
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.y                = self->position.y + self->pushOffset / 3;
    self->animator.frameID = 3;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 4;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PushSpring_StateDraw_Bottom(void)
{
    RSDK_THIS(PushSpring);

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.y -= self->pushOffset;
    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);
    drawPos.y                = self->position.y - 2 * self->pushOffset / 3;
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);
    drawPos.y                = self->position.y - self->pushOffset / 3;
    self->animator.frameID = 3;
    RSDK.DrawSprite(&self->animator, &drawPos, false);
    self->animator.frameID = 4;
    RSDK.DrawSprite(&self->animator, 0, false);
}

void PushSpring_StateDraw_Left(void)
{
    RSDK_THIS(PushSpring);

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x += self->pushOffset;
    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x                = self->position.x + 2 * self->pushOffset / 3;
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x                = self->position.x + self->pushOffset / 3;
    self->animator.frameID = 3;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 4;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PushSpring_StateDraw_Right(void)
{
    RSDK_THIS(PushSpring);

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= self->pushOffset;
    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x                = self->position.x - 2 * self->pushOffset / 3;
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x                = self->position.x - self->pushOffset / 3;
    self->animator.frameID = 3;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 4;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PushSpring_Unknown9(void)
{
    RSDK_THIS(PushSpring);
    if (self->field_78) {
        self->pushTimer = 0;
        self->state     = PushSpring_Unknown10;
    }
}

void PushSpring_Unknown10(void)
{
    RSDK_THIS(PushSpring);
    if (self->field_78) {
        self->pushOffset += self->groundVel;
        if (self->pushOffset < 0x120000) {
            if (!(self->pushTimer % 10))
                RSDK.PlaySfx(PushSpring->sfxPush, false, 255);
            ++self->pushTimer;
        }
        else {
            ++self->pushTimer;
            self->pushOffset = 0x120000;
        }
    }
    else {
        self->state = PushSpring_Unknown11;
    }
}

void PushSpring_Unknown11(void)
{
    RSDK_THIS(PushSpring);
    if (self->field_78) {
        self->pushTimer = 0;
        self->state     = PushSpring_Unknown10;
    }
    else {
        self->pushOffset -= 0x20000;
        if (self->pushOffset <= 0) {
            self->pushOffset = 0;
            self->state      = PushSpring_Unknown9;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void PushSpring_EditorDraw(void)
{
    RSDK_THIS(PushSpring);

    int dir = self->direction;

    RSDK.SetSpriteAnimation(PushSpring->aniFrames, self->type, &self->animator, false, 0);
    if (!self->type  &&self->direction) {
        self->direction = FLIP_Y;
    }

    RSDK.DrawSprite(&self->animator, NULL, false);

    self->direction = dir;
}

void PushSpring_EditorLoad(void) { PushSpring->aniFrames = RSDK.LoadSpriteAnimation("OOZ/PushSpring.bin", SCOPE_STAGE); }
#endif

void PushSpring_Serialize(void)
{
    RSDK_EDITABLE_VAR(PushSpring, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(PushSpring, VAR_UINT8, direction);
}
