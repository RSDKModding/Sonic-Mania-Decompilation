// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FBZFan Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectFBZFan *FBZFan;

void FBZFan_Update(void)
{
    RSDK_THIS(FBZFan);

    FBZFan->fanHitbox.top    = (RSDK.Sin256(2 * Zone->timer) >> 5) - 80;
    FBZFan->fanHitbox.bottom = FBZFan->fanHitbox.top + 96;
    self->hitbox.top       = -96;
    self->hitbox.bottom    = -8;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        bool32 flag  = false;
        Player_CheckCollisionBox(player, self, &FBZFan->solidHitbox);

        if (player->state != Player_State_None && Player_CheckValidState(player) && player->animator.animationID != ANI_HURT
            && RSDK.CheckObjectCollisionTouchBox(self, &FBZFan->fanHitbox, player, &FBZFan->playerHitbox)) {
            flag = true;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
            player->state    = Player_State_Air;
            player->onGround = false;
            int32 vel          = (self->position.y + (FBZFan->fanHitbox.top << 16) - player->position.y) >> 4;
            if (player->velocity.y <= vel) {
                player->velocity.y = vel;
            }
            else {
                player->velocity.y += ((self->position.y + (FBZFan->fanHitbox.top << 16) - player->position.y) >> 6)
                                      + ((self->position.y + (FBZFan->fanHitbox.top << 16) - player->position.y) >> 5);
                if (player->velocity.y < vel) {
                    player->velocity.y = vel;
                }
            }

            if (!((1 << playerID) & FBZFan->activePlayers)) {
                if (player->velocity.y > -0x40000 && player->velocity.y < 0)
                    player->velocity.x += (32 * player->velocity.x / 31) >> 5;
                FBZFan->activePlayers |= 1 << playerID;
            }
        }

        if (RSDK.CheckObjectCollisionTouchBox(self, &self->hitbox, player, &FBZFan->playerHitbox)) {
            if (!((1 << playerID) & self->activePlayers) && flag) {
                RSDK.PlaySfx(FBZFan->sfxFan, false, 255);
                self->activePlayers |= (1 << playerID);
            }
        }
        else {
            self->activePlayers &= ~(1 << playerID);
        }
    }
}

void FBZFan_LateUpdate(void) {}

void FBZFan_StaticUpdate(void)
{
    RSDK.ProcessAnimation(&FBZFan->fanAnimator);
    RSDK.ProcessAnimation(&FBZFan->fan2Animator);
    FBZFan->activePlayers = 0;
}

void FBZFan_Draw(void)
{
    RSDK.DrawSprite(&FBZFan->baseAnimator, NULL, false);
    RSDK.DrawSprite(&FBZFan->fanAnimator, NULL, false);
    RSDK.DrawSprite(&FBZFan->fan2Animator, NULL, false);
}

void FBZFan_Create(void *data)
{
    RSDK_THIS(FBZFan);
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow + 1;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->hitbox.left   = -64;
        self->hitbox.right  = 64;
    }
}

void FBZFan_StageLoad(void)
{
    FBZFan->aniFrames = RSDK.LoadSpriteAnimation("FBZ/FBZFan.bin", SCOPE_STAGE);
    FBZFan->sfxFan    = RSDK.GetSfx("FBZ/FBZFan.wav");
    RSDK.SetSpriteAnimation(FBZFan->aniFrames, 0, &FBZFan->baseAnimator, true, 0);
    RSDK.SetSpriteAnimation(FBZFan->aniFrames, 1, &FBZFan->fanAnimator, true, 0);
    RSDK.SetSpriteAnimation(FBZFan->aniFrames, 2, &FBZFan->fan2Animator, true, 0);
    FBZFan->solidHitbox.left    = -64;
    FBZFan->solidHitbox.top     = -16;
    FBZFan->solidHitbox.right   = 64;
    FBZFan->solidHitbox.bottom  = 16;
    FBZFan->fanHitbox.left      = -64;
    FBZFan->fanHitbox.right     = 64;
    FBZFan->playerHitbox.left   = -1;
    FBZFan->playerHitbox.top    = -1;
    FBZFan->playerHitbox.right  = 1;
    FBZFan->playerHitbox.bottom = 1;
}

#if RETRO_INCLUDE_EDITOR
void FBZFan_EditorDraw(void) { FBZFan_Draw(); }

void FBZFan_EditorLoad(void)
{
    FBZFan->aniFrames = RSDK.LoadSpriteAnimation("FBZ/FBZFan.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(FBZFan->aniFrames, 0, &FBZFan->baseAnimator, true, 0);
    RSDK.SetSpriteAnimation(FBZFan->aniFrames, 1, &FBZFan->fanAnimator, true, 0);
    RSDK.SetSpriteAnimation(FBZFan->aniFrames, 2, &FBZFan->fan2Animator, true, 0);
}
#endif

void FBZFan_Serialize(void) {}
