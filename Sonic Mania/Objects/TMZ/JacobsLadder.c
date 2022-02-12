// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: JacobsLadder Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectJacobsLadder *JacobsLadder;

void JacobsLadder_Update(void)
{
    RSDK_THIS(JacobsLadder);
    self->field_84  = 0;
    self->field_80  = false;
    self->direction = self->flip ? FLIP_Y : FLIP_NONE;

    int32 val = (self->intervalOffset + Zone->timer) % self->interval;
    if (val >= self->duration) {
        if (val < self->duration + 12)
            self->field_84 = 1;
    }
    else {
        if (!self->field_80 && self->activeScreens == 1)
            RSDK.PlaySfx(JacobsLadder->sfxLadder, false, 255);
        self->field_80 = true;
    }

    if (self->field_84) {
        self->field_7C = self->height << 20;
        RSDK.SetSpriteAnimation(JacobsLadder->aniFrames, 3, &self->animator, true, 10);
    }

    if (self->field_80) {
        int32 storeX       = self->position.x;
        int32 storeY       = self->position.y;
        self->field_7C = val * ((self->height << 20) / self->duration);
        self->position.y += (2 * self->flip - 1) * (self->field_7C + 0x300000);

        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntityID(player);

            if (((1 << playerID) & self->activePlayers)) {
                if (player->state == Player_State_None && player->shield == SHIELD_LIGHTNING) {
                    if (player->jumpPress) {
                        self->playerTimers[playerID] = 15;
                        self->activePlayers &= ~(1 << playerID);
                        player->jumpAbilityState = 1;
                        player->state            = Player_State_Air;
                        SceneInfo->entity   = (Entity *)player;
                        player->stateAbility();
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
                            if (player->state != Player_State_None) {
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                                player->state                       = Player_State_None;
                                player->nextAirState                = StateMachine_None;
                                player->nextGroundState             = StateMachine_None;
                                self->playerPositions[playerID].x = player->position.x - self->position.x;
                                self->playerPositions[playerID].y = player->position.y - self->position.y;
                                player->velocity.x                  = 0;
                                player->velocity.y                  = 0;
                                player->groundVel                   = 0;
                                self->activePlayers |= 1 << playerID;
                            }
                        }
                        else {
                            Player_CheckHit(player, self);
                        }
                    }
                }
            }
        }

        self->position.x = storeX;
        self->position.y = storeY;
        RSDK.ProcessAnimation(&self->animator);
    }
    else {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntityID(player);

            if (((1 << playerID) & self->activePlayers)) {
                self->activePlayers &= ~(1 << playerID);
                if (player->state == Player_State_None) {
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
    Vector2 drawPos;
    self->drawFX    = FX_FLIP;
    self->inkEffect = INK_NONE;
    self->alpha     = 255;
    int32 val           = -(int32)self->flip;
    val               = -(val != 0);
    int32 val2          = val & 0xFFFFFF02;

    int32 yOff = 0x300000 * (2 * (self->flip != false) - 1);
    int32 id   = 0;
    for (int32 i = 0; i < 2; ++i) {
        self->direction = val2 ^ id;
        drawPos.y         = self->position.y;
        drawPos.x         = ((2 * (id != 0) - 1) << 21) + self->position.x;
        RSDK.SetSpriteAnimation(JacobsLadder->aniFrames, 0, &self->animator2, true, 0);
        RSDK.DrawSprite(&self->animator2, &drawPos, false);
        drawPos.y += yOff;
        for (int32 y = 0; y < self->height; ++y) {
            RSDK.SetSpriteAnimation(JacobsLadder->aniFrames, 1, &self->animator2, true, 0);
            RSDK.DrawSprite(&self->animator2, &drawPos, false);
            drawPos.y += (2 * (self->flip != false) - 1) << 20;
        }
        RSDK.SetSpriteAnimation(JacobsLadder->aniFrames, 2, &self->animator2, true, 0);
        RSDK.DrawSprite(&self->animator2, &drawPos, false);
        id++;
    }

    self->direction = val2;
    if (self->field_80 || self->field_84) {
        self->inkEffect = INK_ADD;
        if (self->field_84)
            self->alpha = 21 * (self->duration - (self->intervalOffset + Zone->timer) % self->interval) + 255;
        drawPos.x = self->position.x;
        drawPos.y = (2 * (self->flip != false) - 1) * (self->field_7C + 0x300000) + self->position.y;
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        RSDK.DrawSprite(&self->animator, &drawPos, false);
    }
}

void JacobsLadder_Create(void *data)
{
    RSDK_THIS(JacobsLadder);
    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->field_70      = self->position;
    self->visible       = true;
    self->updateRange.x = 0x800000;
    self->updateRange.y = (self->height + 8) << 20;
    self->drawFX        = FX_FLIP;
    self->direction     = self->flip ? FLIP_Y : FLIP_NONE;
    if (!self->interval)
        self->interval = 120;
    if (!self->duration)
        self->duration = 60;
    RSDK.SetSpriteAnimation(JacobsLadder->aniFrames, 3, &self->animator, true, 0);
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

#if RETRO_INCLUDE_EDITOR
void JacobsLadder_EditorDraw(void) { JacobsLadder_Draw(); }

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
