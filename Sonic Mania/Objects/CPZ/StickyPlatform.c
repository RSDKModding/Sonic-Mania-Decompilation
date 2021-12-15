// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: StickyPlatform Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectStickyPlatform *StickyPlatform;

void StickyPlatform_Update(void)
{
    RSDK_THIS(StickyPlatform);
    self->active = ACTIVE_NORMAL;
    StateMachine_Run(self->state);
    self->position.x = self->amplitude.x * RSDK.Cos1024(self->angle) + self->startPos.x;
    self->position.y = self->amplitude.y * RSDK.Cos1024(self->angle) + self->startPos.y;
    StickyPlatform_Interact();
    RSDK.ProcessAnimation(&self->animator);
    int32 animState = self->animator.animationID % 3;
    if (animState) {
        if (animState == 2 && self->animator.frameID == self->animator.frameCount - 1) {
            RSDK.SetSpriteAnimation(StickyPlatform->animID, 3 * (self->type >> 1), &self->animator, false, 0);
            self->animator.speed = 0;
        }
    }
    else if (self->animator.frameID == self->animator.frameCount - 3) {
        RSDK.SetSpriteAnimation(StickyPlatform->animID, 3 * (self->type >> 1) + 1, &self->animator, false, 0);
    }

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->angle     = 0;
        self->playerBits = 0;
        self->state     = self->oscillate ? StickyPlatform_State_AddSpeed : StickyPlatform_State_HandleMovement;
    }
}

void StickyPlatform_LateUpdate(void) {}

void StickyPlatform_StaticUpdate(void) {}

void StickyPlatform_Draw(void)
{
    RSDK_THIS(StickyPlatform);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void StickyPlatform_Create(void *data)
{
    RSDK_THIS(StickyPlatform);
    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->active        = ACTIVE_BOUNDS;
        self->drawOrder     = Zone->drawOrderHigh - 2;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        int16 left = 0, top = 0, right = 0, bottom = 0;
        RSDK.SetSpriteAnimation(StickyPlatform->animID, self->type > STICKYPLATFORM_DOWN ? 3 : 0, &self->animator, true, 0);
        switch (self->type) {
            case STICKYPLATFORM_UP:
                self->direction = FLIP_NONE;
                self->rotation  = 0;

                left   = -16;
                top    = -16;
                right  = 16;
                bottom = 8;
                break;
            case STICKYPLATFORM_DOWN:
                self->direction = FLIP_Y;
                self->rotation  = 0x80;

                left   = -16;
                top    = -8;
                right  = 16;
                bottom = 16;
                break;
            case STICKYPLATFORM_LEFT:
                self->direction = FLIP_NONE;
                self->rotation  = 0x40;

                left   = -8;
                top    = -16;
                right  = 16;
                bottom = 16;
                break;
            case STICKYPLATFORM_RIGHT:
                self->direction = FLIP_X;
                self->rotation  = 0xC0;

                left   = -16;
                top    = -16;
                right  = 8;
                bottom = 16;
                break;
            default: break;
        }
        self->hitbox.left   = left;
        self->hitbox.top    = top;
        self->hitbox.bottom = bottom;
        self->hitbox.right  = right;

        self->animator.speed = 0;

        self->startPos.x = self->position.x;
        self->position.x += self->amplitude.x;
        self->startPos.y = self->position.y;
        self->position.y += self->amplitude.y;
        self->amplitude.x >>= 10;
        self->amplitude.y >>= 10;

        if (!self->speed)
            self->speed = 4;

        self->state = self->oscillate ? StickyPlatform_State_AddSpeed : StickyPlatform_State_HandleMovement;
    }
}

void StickyPlatform_StageLoad(void)
{
    StickyPlatform->animID = RSDK.LoadSpriteAnimation("CPZ/StickyPlatform.bin", SCOPE_STAGE);
    StickyPlatform->sfxID  = RSDK.GetSfx("Puyo/Land.wav");
}

void StickyPlatform_Interact(void)
{
    RSDK_THIS(StickyPlatform);
    if (Player->playerCount > 0) {
        for (int32 i = 0; i < Player->playerCount; i++) {
            EntityPlayer *player = RSDK.GetEntityByID(i);
            if (Player_CheckValidState(player) && player->interaction) {
                if (((1 << i) & self->playerBits)) {
                    if (player->state == Player_State_None) {
                        if (player->jumpPress) {
                            player->angle     = self->rotation;
                            player->groundVel = 0;
                            Player_StartJump(player);
                            player->velocity.x = (320 * player->velocity.x) >> 8;
                            player->velocity.y = (320 * player->velocity.y) >> 8;
                            self->playerBits &= ~(1 << i);
                            self->cooldowns[i] = 16;
                            RSDK.SetSpriteAnimation(StickyPlatform->animID, 3 * (self->type >> 1) + 2, &self->animator, false, 0);
                        }
                        else {
                            Hitbox *playerbox = Player_GetHitbox(player);
                            uint16 animID     = 0;
                            switch (self->type) {
                                case STICKYPLATFORM_UP:
                                    player->position.x = self->position.x;
                                    player->position.y = self->position.y - (playerbox->bottom << 16);
                                    animID             = 10;
                                    break;
                                case STICKYPLATFORM_DOWN:
                                    player->position.x = self->position.x;
                                    player->position.y = self->position.y - (playerbox->top << 16);
                                    animID             = 10;
                                    break;
                                case STICKYPLATFORM_LEFT:
                                    player->position.y = self->position.y;
                                    player->position.x = self->position.x + ((8 - playerbox->left) << 16);
                                    player->direction  = FLIP_NONE;
                                    animID             = 36;
                                    break;
                                case STICKYPLATFORM_RIGHT:
                                    player->position.y = self->position.y;
                                    player->position.x = self->position.x - ((playerbox->right + 8) << 16);
                                    player->direction  = FLIP_X;
                                    animID             = 36;
                                    break;
                                default: break;
                            }
                            RSDK.SetSpriteAnimation(player->aniFrames, animID, &player->animator, false, 0);
                        }
                    }
                    else
                        self->playerBits &= ~(1 << i);
                }
                else {
                    if (!Player_CheckCollisionTouch(player, self, &self->hitbox) || self->cooldowns[i]) {
                        if (self->cooldowns[i])
                            self->cooldowns[i]--;
                        continue;
                    }

                    self->playerBits |= 1 << i;
                    RSDK.SetSpriteAnimation(StickyPlatform->animID, 3 * (self->type >> 1), &self->animator, true, 0);
                    self->animator.speed = 1;
                    player->state                   = Player_State_None;
                    player->velocity.x              = 0;
                    player->velocity.y              = 0;
                    player->groundVel               = 0;
                    player->rotation                = 0;
                    RSDK.PlaySfx(StickyPlatform->sfxID, false, 255);
                }
            }
        }
    }
}

void StickyPlatform_State_HandleMovement(void)
{
    RSDK_THIS(StickyPlatform);
    if (self->angle) {
        if (!self->playerBits)
            self->state = StickyPlatform_State_MoveBackForth;
    }
    else if (self->playerBits > 0) {
        self->state = StickyPlatform_State_MoveBack;
    }
}

void StickyPlatform_State_MoveBack(void)
{
    RSDK_THIS(StickyPlatform);
    self->angle += self->speed;
    if (self->angle >= 0x200) {
        self->angle = 0x200;
        self->state = StickyPlatform_State_HandleMovement;
    }
}

void StickyPlatform_State_MoveBackForth(void)
{
    RSDK_THIS(StickyPlatform);
    self->angle += self->speed;
    if (self->angle >= 0x400) {
        self->angle = 0;
        self->state = StickyPlatform_State_HandleMovement;
    }
}

void StickyPlatform_State_AddSpeed(void)
{
    RSDK_THIS(StickyPlatform);
    self->angle = (self->angle + self->speed) & 0x3FF;
}

#if RETRO_INCLUDE_EDITOR
void StickyPlatform_EditorDraw(void)
{
    RSDK_THIS(StickyPlatform);
    RSDK.SetSpriteAnimation(StickyPlatform->animID, self->type > 1 ? 3 : 0, &self->animator, true, 0);
    switch (self->type) {
        case STICKYPLATFORM_UP: self->direction = FLIP_NONE; break;
        case STICKYPLATFORM_DOWN: self->direction = FLIP_Y; break;
        case STICKYPLATFORM_LEFT: self->direction = FLIP_NONE; break;
        case STICKYPLATFORM_RIGHT: self->direction = FLIP_X; break;
        default: break;
    }

    StickyPlatform_Draw();
}

void StickyPlatform_EditorLoad(void)
{
    StickyPlatform->animID = RSDK.LoadSpriteAnimation("CPZ/StickyPlatform.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(StickyPlatform, type);
    RSDK_ENUM_VAR("Up", STICKYPLATFORM_UP);
    RSDK_ENUM_VAR("Down", STICKYPLATFORM_DOWN);
    RSDK_ENUM_VAR("Left", STICKYPLATFORM_LEFT);
    RSDK_ENUM_VAR("Right", STICKYPLATFORM_RIGHT);
}
#endif

void StickyPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(StickyPlatform, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(StickyPlatform, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(StickyPlatform, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(StickyPlatform, VAR_BOOL, oscillate);
}