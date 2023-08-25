// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PullSwitch Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPullSwitch *PullSwitch;

void PullSwitch_Update(void)
{
    RSDK_THIS(PullSwitch);

    if (!self->activated) {
        if (self->pullPos > 0) {
            self->position.y -= 0x20000;
            self->pullPos -= 0x20000;
        }
    }
    else if (self->pullPos < 0x300000) {
        self->position.y += 0x20000;
        self->pullPos += 0x20000;
    }
    else {
        if (OOZSetup->smogTimer > 16 || Smog->forceEnabled)
            RSDK.PlaySfx(PullSwitch->sfxSmogClear, false, 0xFF);

        OOZSetup->smogTimer = 0;
        Smog->forceEnabled  = false;
    }

    self->activated = false;
    foreach_active(Player, player)
    {
        int32 playerID       = RSDK.GetEntitySlot(player);
        Hitbox *playerHitbox = Player_GetHitbox(player);

        if (self->playerTimers[playerID]) {
            self->playerTimers[playerID]--;
        }
        else if (((1 << playerID) & self->activePlayers)) {
            player->position.x = self->position.x;
            player->position.y = self->position.y - (playerHitbox->top << 16) + 0x1A0000;
            self->activated    = true;

            if (player->state == Player_State_Static) {
                if (player->jumpPress) {
                    self->activePlayers &= ~(1 << playerID);
                    self->playerTimers[playerID] = 60;
                    player->velocity.y           = -0x40000;
                    player->onGround             = false;
                    player->groundedStore        = false;
                    player->jumpAbilityState     = 1;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->animator.speed = 48;
                    player->state          = Player_State_Air;
                }
            }
            else {
                self->activePlayers &= ~(1 << playerID);
                self->playerTimers[playerID] = 60;
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, self, &PullSwitch->hitbox) && player->velocity.y >= 0) {
                self->activePlayers |= 1 << playerID;
                player->state           = Player_State_Static;
                player->nextGroundState = StateMachine_None;
                player->nextAirState    = StateMachine_None;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->animator, false, 0);

                player->velocity.x  = 0;
                player->velocity.y  = 0;
                player->groundVel   = 0;
                player->controlLock = 0;
                player->position.x  = self->position.x;
                player->position.y  = self->position.y - (playerHitbox->top << 16) + 0x1A0000;
                self->activated     = true;
            }
        }
    }
}

void PullSwitch_LateUpdate(void) {}

void PullSwitch_StaticUpdate(void) {}

void PullSwitch_Draw(void)
{
    RSDK_THIS(PullSwitch);

    SpriteFrame *frame = RSDK.GetFrame(PullSwitch->aniFrames, 0, 1);

    frame->height = (self->position.y - self->handlePos.y) >> 16;
    frame->sprY   = self->sprY + self->sprHeight - frame->height;
    RSDK.DrawSprite(&self->chainAnimator, &self->handlePos, false);
    RSDK.DrawSprite(&self->handleAnimator, NULL, false);
    RSDK.DrawSprite(&self->dispenserAnimator, &self->handlePos, false);
}

void PullSwitch_Create(void *data)
{
    RSDK_THIS(PullSwitch);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawFX        = FX_FLIP;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->handlePos     = self->position;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        RSDK.SetSpriteAnimation(PullSwitch->aniFrames, 0, &self->handleAnimator, true, 2);
        RSDK.SetSpriteAnimation(PullSwitch->aniFrames, 0, &self->dispenserAnimator, true, 0);
        RSDK.SetSpriteAnimation(PullSwitch->aniFrames, 0, &self->chainAnimator, true, 1);

        SpriteFrame *frame = RSDK.GetFrame(PullSwitch->aniFrames, 0, 1);
        self->sprY         = frame->sprY;
        self->sprHeight    = frame->height;
    }
}

void PullSwitch_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("OOZ1") || RSDK.CheckSceneFolder("OOZ2"))
        PullSwitch->aniFrames = RSDK.LoadSpriteAnimation("OOZ/PullSwitch.bin", SCOPE_STAGE);

    PullSwitch->hitbox.left   = -16;
    PullSwitch->hitbox.top    = 20;
    PullSwitch->hitbox.right  = 16;
    PullSwitch->hitbox.bottom = 28;

    PullSwitch->sfxButton    = RSDK.GetSfx("Stage/Button.wav");
    PullSwitch->sfxSmogClear = RSDK.GetSfx("OOZ/SmogClear.wav");
}

#if GAME_INCLUDE_EDITOR
void PullSwitch_EditorDraw(void)
{
    RSDK_THIS(PullSwitch);

    self->handlePos = self->position;

    RSDK.SetSpriteAnimation(PullSwitch->aniFrames, 0, &self->handleAnimator, false, 2);
    RSDK.SetSpriteAnimation(PullSwitch->aniFrames, 0, &self->dispenserAnimator, false, 0);
    RSDK.SetSpriteAnimation(PullSwitch->aniFrames, 0, &self->chainAnimator, false, 1);

    RSDK.DrawSprite(&self->chainAnimator, &self->handlePos, false);
    RSDK.DrawSprite(&self->dispenserAnimator, &self->handlePos, false);

    self->handlePos.y += 0x300000;
    RSDK.DrawSprite(&self->handleAnimator, &self->handlePos, false);
}

void PullSwitch_EditorLoad(void) { PullSwitch->aniFrames = RSDK.LoadSpriteAnimation("OOZ/PullSwitch.bin", SCOPE_STAGE); }
#endif

void PullSwitch_Serialize(void) {}
