// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: JacobsLadder Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectJacobsLadder *JacobsLadder;

void JacobsLadder_Update(void)
{
    RSDK_THIS(JacobsLadder);

    bool32 showElectricity = self->showElectricity;

    self->electricFadeOut = false;
    self->showElectricity = false;
    self->direction       = self->flip ? FLIP_Y : FLIP_NONE;

    int32 timer = (self->intervalOffset + Zone->timer) % self->interval;

    if (timer < self->duration) {
        if (!showElectricity && self->onScreen == 1)
            RSDK.PlaySfx(JacobsLadder->sfxLadder, false, 255);

        self->showElectricity = true;
    }
    else if (timer < self->duration + 12)
        self->electricFadeOut = true;

    if (self->electricFadeOut) {
        self->electricPos = self->height << 20;
        RSDK.SetSpriteAnimation(JacobsLadder->aniFrames, 3, &self->electricAnimator, true, 10);
    }

    if (self->showElectricity) {
        int32 storeX      = self->position.x;
        int32 storeY      = self->position.y;
        self->electricPos = timer * ((self->height << 20) / self->duration);
        self->position.y += (2 * self->flip - 1) * (self->electricPos + 0x300000);

        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntitySlot(player);

            if (((1 << playerID) & self->activePlayers)) {
                if (player->state == Player_State_Static && player->shield == SHIELD_LIGHTNING) {
                    if (player->jumpPress) {
                        self->playerTimers[playerID] = 15;
                        self->activePlayers &= ~(1 << playerID);
                        player->jumpAbilityState = 1;
                        player->state            = Player_State_Air;

                        SceneInfo->entity = (Entity *)player;
                        StateMachine_Run(player->stateAbility);
                        SceneInfo->entity = (Entity *)self;
                    }
                    else {
                        player->position.x = self->position.x + self->playerPositions[playerID].x;
                        player->position.y = self->position.y + self->playerPositions[playerID].y;
                    }
                }
                else {
                    self->activePlayers &= ~(1 << playerID);
                }
            }
            else {
                if (self->playerTimers[playerID]) {
                    self->playerTimers[playerID]--;
                }
                else {
                    if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                        if (player->shield == SHIELD_LIGHTNING) {
                            if (player->state != Player_State_Static) {
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                                player->state           = Player_State_Static;
                                player->nextAirState    = StateMachine_None;
                                player->nextGroundState = StateMachine_None;

                                self->playerPositions[playerID].x = player->position.x - self->position.x;
                                self->playerPositions[playerID].y = player->position.y - self->position.y;

                                player->velocity.x = 0;
                                player->velocity.y = 0;
                                player->groundVel  = 0;

                                self->activePlayers |= 1 << playerID;
                            }
                        }
                        else {
                            Player_Hurt(player, self);
                        }
                    }
                }
            }
        }

        self->position.x = storeX;
        self->position.y = storeY;

        RSDK.ProcessAnimation(&self->electricAnimator);
    }
    else {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntitySlot(player);

            if ((1 << playerID) & self->activePlayers) {
                self->activePlayers &= ~(1 << playerID);

                if (player->state == Player_State_Static) {
                    player->velocity.x       = 0;
                    player->velocity.y       = 0;
                    player->groundVel        = 0;
                    player->jumpAbilityState = 1;
                    player->state            = Player_State_Air;
                }
            }
        }
    }
}

void JacobsLadder_LateUpdate(void) {}

void JacobsLadder_StaticUpdate(void) {}

void JacobsLadder_Draw(void)
{
    RSDK_THIS(JacobsLadder);

    self->drawFX    = FX_FLIP;
    self->inkEffect = INK_NONE;
    self->alpha     = 0xFF;

    // what a line of code tbh
    int32 direction = -(-(int32)self->flip != false) & 0xFFFFFF02;

    int32 yOff = 0x300000 * (2 * (self->flip != false) - 1);
    int32 side = 0;
    for (int32 i = 0; i < 2; ++i) {
        self->direction = direction ^ side;

        Vector2 drawPos;
        drawPos.x = self->position.x + ((2 * (side != false) - 1) << 21);
        drawPos.y = self->position.y;
        RSDK.SetSpriteAnimation(JacobsLadder->aniFrames, 0, &self->sideAnimator, true, 0);
        RSDK.DrawSprite(&self->sideAnimator, &drawPos, false);

        drawPos.y += yOff;
        for (int32 y = 0; y < self->height; ++y) {
            RSDK.SetSpriteAnimation(JacobsLadder->aniFrames, 1, &self->sideAnimator, true, 0);
            RSDK.DrawSprite(&self->sideAnimator, &drawPos, false);
            drawPos.y += (2 * (self->flip != false) - 1) << 20;
        }

        RSDK.SetSpriteAnimation(JacobsLadder->aniFrames, 2, &self->sideAnimator, true, 0);
        RSDK.DrawSprite(&self->sideAnimator, &drawPos, false);
        side++;
    }

    self->direction = direction;
    if (self->showElectricity || self->electricFadeOut) {
        self->inkEffect = INK_ADD;
        if (self->electricFadeOut)
            self->alpha = 21 * (self->duration - (self->intervalOffset + Zone->timer) % self->interval) + 255;

        Vector2 drawPos;
        drawPos.x = self->position.x;
        drawPos.y = self->position.y + (2 * (self->flip != false) - 1) * (self->electricPos + 0x300000);
        RSDK.DrawSprite(&self->electricAnimator, &drawPos, false);
        RSDK.DrawSprite(&self->electricAnimator, &drawPos, false);
    }
}

void JacobsLadder_Create(void *data)
{
    RSDK_THIS(JacobsLadder);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->visible       = true;
    self->updateRange.x = 0x800000;
    self->updateRange.y = (self->height + 8) << 20;
    self->drawFX        = FX_FLIP;
    self->direction     = self->flip ? FLIP_Y : FLIP_NONE;

    if (!self->interval)
        self->interval = 120;

    if (!self->duration)
        self->duration = 60;

    RSDK.SetSpriteAnimation(JacobsLadder->aniFrames, 3, &self->electricAnimator, true, 0);

    self->hitbox.left   = -35;
    self->hitbox.top    = -4;
    self->hitbox.right  = 35;
    self->hitbox.bottom = 4;
}

void JacobsLadder_StageLoad(void)
{
    JacobsLadder->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/JacobsLadder.bin", SCOPE_STAGE);

    JacobsLadder->sfxLadder = RSDK.GetSfx("TMZ1/JacobsLadder.wav");
}

#if GAME_INCLUDE_EDITOR
void JacobsLadder_EditorDraw(void)
{
    JacobsLadder_Create(NULL);
    JacobsLadder_Draw();
}

void JacobsLadder_EditorLoad(void) { JacobsLadder->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/JacobsLadder.bin", SCOPE_STAGE); }
#endif

void JacobsLadder_Serialize(void)
{
    RSDK_EDITABLE_VAR(JacobsLadder, VAR_UINT32, height);
    RSDK_EDITABLE_VAR(JacobsLadder, VAR_ENUM, interval);
    RSDK_EDITABLE_VAR(JacobsLadder, VAR_ENUM, intervalOffset);
    RSDK_EDITABLE_VAR(JacobsLadder, VAR_ENUM, duration);
    RSDK_EDITABLE_VAR(JacobsLadder, VAR_BOOL, flip);
}
