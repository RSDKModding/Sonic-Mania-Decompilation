// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FBZFan Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFBZFan *FBZFan;

void FBZFan_Update(void)
{
    RSDK_THIS(FBZFan);

    FBZFan->hitboxFan.top    = (RSDK.Sin256(2 * Zone->timer) >> 5) - 80;
    FBZFan->hitboxFan.bottom = FBZFan->hitboxFan.top + 96;

    self->hitboxTrigger.top    = -96;
    self->hitboxTrigger.bottom = -8;

    foreach_active(Player, player)
    {
        bool32 isFanning = false;

        int32 playerID = RSDK.GetEntitySlot(player);
        Player_CheckCollisionBox(player, self, &FBZFan->hitboxSolid);

        if (player->state != Player_State_Static && Player_CheckValidState(player) && player->animator.animationID != ANI_HURT
            && RSDK.CheckObjectCollisionTouchBox(self, &FBZFan->hitboxFan, player, &FBZFan->hitboxPlayer)) {
            isFanning = true;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
            player->state    = Player_State_Air;
            player->onGround = false;

            int32 vel = (self->position.y + (FBZFan->hitboxFan.top << 16) - player->position.y) >> 4;
            if (player->velocity.y <= vel) {
                player->velocity.y = vel;
            }
            else {
                player->velocity.y += ((self->position.y + (FBZFan->hitboxFan.top << 16) - player->position.y) >> 6)
                                      + ((self->position.y + (FBZFan->hitboxFan.top << 16) - player->position.y) >> 5);

                if (player->velocity.y < vel)
                    player->velocity.y = vel;
            }

            if (!((1 << playerID) & FBZFan->activePlayers)) {
                if (player->velocity.y > -0x40000 && player->velocity.y < 0)
                    player->velocity.x += (32 * player->velocity.x / 31) >> 5;

                FBZFan->activePlayers |= 1 << playerID;
            }
        }

        if (RSDK.CheckObjectCollisionTouchBox(self, &self->hitboxTrigger, player, &FBZFan->hitboxPlayer)) {
            if (!((1 << playerID) & self->activePlayers) && isFanning) {
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
        self->drawGroup     = Zone->objectDrawGroup[0] + 1;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        self->hitboxTrigger.left  = -64;
        self->hitboxTrigger.right = 64;
    }
}

void FBZFan_StageLoad(void)
{
    FBZFan->aniFrames = RSDK.LoadSpriteAnimation("FBZ/FBZFan.bin", SCOPE_STAGE);

    FBZFan->sfxFan = RSDK.GetSfx("FBZ/FBZFan.wav");

    RSDK.SetSpriteAnimation(FBZFan->aniFrames, 0, &FBZFan->baseAnimator, true, 0);
    RSDK.SetSpriteAnimation(FBZFan->aniFrames, 1, &FBZFan->fanAnimator, true, 0);
    RSDK.SetSpriteAnimation(FBZFan->aniFrames, 2, &FBZFan->fan2Animator, true, 0);

    FBZFan->hitboxSolid.left   = -64;
    FBZFan->hitboxSolid.top    = -16;
    FBZFan->hitboxSolid.right  = 64;
    FBZFan->hitboxSolid.bottom = 16;

    FBZFan->hitboxFan.left  = -64;
    FBZFan->hitboxFan.right = 64;

    FBZFan->hitboxPlayer.left   = -1;
    FBZFan->hitboxPlayer.top    = -1;
    FBZFan->hitboxPlayer.right  = 1;
    FBZFan->hitboxPlayer.bottom = 1;
}

#if GAME_INCLUDE_EDITOR
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
