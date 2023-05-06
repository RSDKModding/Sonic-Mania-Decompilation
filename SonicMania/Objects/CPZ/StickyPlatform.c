// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: StickyPlatform Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectStickyPlatform *StickyPlatform;

void StickyPlatform_Update(void)
{
    RSDK_THIS(StickyPlatform);

    self->active = ACTIVE_NORMAL;
    StateMachine_Run(self->state);

    self->position.x = self->amplitude.x * RSDK.Cos1024(self->angle) + self->centerPos.x;
    self->position.y = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.y;

    StickyPlatform_Interact();

    RSDK.ProcessAnimation(&self->animator);

    int32 anim = self->animator.animationID % 3;
    if (anim) {
        if (anim == 2 && self->animator.frameID == self->animator.frameCount - 1) {
            RSDK.SetSpriteAnimation(StickyPlatform->aniFrames, 3 * (self->type >> 1), &self->animator, false, 0);
            self->animator.speed = 0;
        }
    }
    else if (self->animator.frameID == self->animator.frameCount - 3) {
        RSDK.SetSpriteAnimation(StickyPlatform->aniFrames, 3 * (self->type >> 1) + 1, &self->animator, false, 0);
    }

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->centerPos, &self->updateRange)) {
        self->position      = self->centerPos;
        self->angle         = 0;
        self->activePlayers = 0;
        self->state         = self->oscillate ? StickyPlatform_State_Oscillating : StickyPlatform_State_HandleMovement;
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
        self->drawGroup     = Zone->objectDrawGroup[1] - 2;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        RSDK.SetSpriteAnimation(StickyPlatform->aniFrames, self->type > STICKYPLATFORM_DOWN ? 3 : 0, &self->animator, true, 0);
        switch (self->type) {
            case STICKYPLATFORM_UP:
                self->direction = FLIP_NONE;
                self->rotation  = 0;

                self->hitbox.left   = -16;
                self->hitbox.top    = -16;
                self->hitbox.right  = 16;
                self->hitbox.bottom = 8;
                break;

            case STICKYPLATFORM_DOWN:
                self->direction = FLIP_Y;
                self->rotation  = 0x80;

                self->hitbox.left   = -16;
                self->hitbox.top    = -8;
                self->hitbox.right  = 16;
                self->hitbox.bottom = 16;
                break;

            case STICKYPLATFORM_RIGHT:
                self->direction = FLIP_NONE;
                self->rotation  = 0x40;

                self->hitbox.left   = -8;
                self->hitbox.top    = -16;
                self->hitbox.right  = 16;
                self->hitbox.bottom = 16;
                break;

            case STICKYPLATFORM_LEFT:
                self->direction = FLIP_X;
                self->rotation  = 0xC0;

                self->hitbox.left   = -16;
                self->hitbox.top    = -16;
                self->hitbox.right  = 8;
                self->hitbox.bottom = 16;
                break;

            default: break;
        }

        self->animator.speed = 0;

        self->centerPos.x = self->position.x;
        self->centerPos.y = self->position.y;
        self->position.x += self->amplitude.x;
        self->position.y += self->amplitude.y;
        self->amplitude.x >>= 10;
        self->amplitude.y >>= 10;

        if (!self->speed)
            self->speed = 4;

        self->state = self->oscillate ? StickyPlatform_State_Oscillating : StickyPlatform_State_HandleMovement;
    }
}

void StickyPlatform_StageLoad(void)
{
    StickyPlatform->aniFrames = RSDK.LoadSpriteAnimation("CPZ/StickyPlatform.bin", SCOPE_STAGE);

    StickyPlatform->sfxLand = RSDK.GetSfx("Puyo/Land.wav");
}

void StickyPlatform_Interact(void)
{
    RSDK_THIS(StickyPlatform);

    if (Player->playerCount > 0) {
        for (int32 i = 0; i < Player->playerCount; i++) {
            EntityPlayer *player = RSDK_GET_ENTITY(i, Player);

            if (Player_CheckValidState(player) && player->interaction) {
                if (((1 << i) & self->activePlayers)) {
                    if (player->state == Player_State_Static) {
                        if (player->jumpPress) {
                            player->angle     = self->rotation;
                            player->groundVel = 0;
                            Player_Action_Jump(player);

                            player->velocity.x = (320 * player->velocity.x) >> 8;
                            player->velocity.y = (320 * player->velocity.y) >> 8;
                            self->activePlayers &= ~(1 << i);
                            self->playerTimer[i] = 16;
                            RSDK.SetSpriteAnimation(StickyPlatform->aniFrames, 3 * (self->type >> 1) + 2, &self->animator, false, 0);
                        }
                        else {
                            Hitbox *playerHitbox = Player_GetHitbox(player);

                            uint16 animID = 0;
                            switch (self->type) {
                                case STICKYPLATFORM_UP:
                                    player->position.x = self->position.x;
                                    player->position.y = self->position.y - (playerHitbox->bottom << 16);
                                    animID             = 10;
                                    break;

                                case STICKYPLATFORM_DOWN:
                                    player->position.x = self->position.x;
                                    player->position.y = self->position.y - (playerHitbox->top << 16);
                                    animID             = 10;
                                    break;

                                case STICKYPLATFORM_RIGHT:
                                    player->position.y = self->position.y;
                                    player->position.x = self->position.x + ((8 - playerHitbox->left) << 16);
                                    player->direction  = FLIP_NONE;
                                    animID             = 36;
                                    break;

                                case STICKYPLATFORM_LEFT:
                                    player->position.y = self->position.y;
                                    player->position.x = self->position.x - ((playerHitbox->right + 8) << 16);
                                    player->direction  = FLIP_X;
                                    animID             = 36;
                                    break;

                                default: break;
                            }
                            RSDK.SetSpriteAnimation(player->aniFrames, animID, &player->animator, false, 0);
                        }
                    }
                    else {
                        self->activePlayers &= ~(1 << i);
                    }
                }
                else {
                    if (!Player_CheckCollisionTouch(player, self, &self->hitbox) || self->playerTimer[i]) {
                        if (self->playerTimer[i])
                            self->playerTimer[i]--;
                        continue;
                    }

                    self->activePlayers |= 1 << i;
                    RSDK.SetSpriteAnimation(StickyPlatform->aniFrames, 3 * (self->type >> 1), &self->animator, true, 0);
                    self->animator.speed = 1;
                    player->state        = Player_State_Static;
                    player->velocity.x   = 0;
                    player->velocity.y   = 0;
                    player->groundVel    = 0;
                    player->rotation     = 0;

                    RSDK.PlaySfx(StickyPlatform->sfxLand, false, 255);
                }
            }
        }
    }
}

void StickyPlatform_State_HandleMovement(void)
{
    RSDK_THIS(StickyPlatform);

    if (self->angle) {
        if (!self->activePlayers)
            self->state = StickyPlatform_State_MoveBackForth;
    }
    else if (self->activePlayers > 0) {
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

void StickyPlatform_State_Oscillating(void)
{
    RSDK_THIS(StickyPlatform);

    self->angle = (self->angle + self->speed) & 0x3FF;
}

#if GAME_INCLUDE_EDITOR
void StickyPlatform_EditorDraw(void)
{
    RSDK_THIS(StickyPlatform);

    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    RSDK.SetSpriteAnimation(StickyPlatform->aniFrames, self->type > STICKYPLATFORM_DOWN ? 3 : 0, &self->animator, true, 0);
    switch (self->type) {
        case STICKYPLATFORM_UP: self->direction = FLIP_NONE; break;
        case STICKYPLATFORM_DOWN: self->direction = FLIP_Y; break;
        case STICKYPLATFORM_RIGHT: self->direction = FLIP_NONE; break;
        case STICKYPLATFORM_LEFT: self->direction = FLIP_X; break;
        default: break;
    }

    StickyPlatform_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        self->centerPos = self->position;
        Vector2 amplitude;
        amplitude.x = self->amplitude.x >> 10;
        amplitude.y = self->amplitude.y >> 10;

        self->inkEffect = INK_BLEND;

        // start pos
        self->position.x = amplitude.x * RSDK.Cos1024(self->angle) + self->centerPos.x;
        self->position.y = amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.y;
        StickyPlatform_Draw();

        // right max
        self->position.x = amplitude.x * RSDK.Cos1024(0x000) + self->centerPos.x;
        self->position.y = amplitude.y * RSDK.Cos1024(0x000) + self->centerPos.y;
        Vector2 drawPos  = self->position;
        StickyPlatform_Draw();

        // left max
        self->position.x = amplitude.x * RSDK.Cos1024(0x200) + self->centerPos.x;
        self->position.y = amplitude.y * RSDK.Cos1024(0x200) + self->centerPos.y;
        StickyPlatform_Draw();

        DrawHelpers_DrawArrow(drawPos.x, drawPos.y, self->position.x, self->position.y, 0x00FF00, INK_NONE, 0xFF);
        DrawHelpers_DrawArrow(self->position.x, self->position.y, drawPos.x, drawPos.y, 0x00FF00, INK_NONE, 0xFF);

        self->inkEffect = INK_NONE;

        self->position = self->centerPos;
        RSDK_DRAWING_OVERLAY(false);
    }
}

void StickyPlatform_EditorLoad(void)
{
    StickyPlatform->aniFrames = RSDK.LoadSpriteAnimation("CPZ/StickyPlatform.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(StickyPlatform, type);
    RSDK_ENUM_VAR("Up", STICKYPLATFORM_UP);
    RSDK_ENUM_VAR("Down", STICKYPLATFORM_DOWN);
    RSDK_ENUM_VAR("Right", STICKYPLATFORM_RIGHT);
    RSDK_ENUM_VAR("Left", STICKYPLATFORM_LEFT);
}
#endif

void StickyPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(StickyPlatform, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(StickyPlatform, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(StickyPlatform, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(StickyPlatform, VAR_BOOL, oscillate);
}