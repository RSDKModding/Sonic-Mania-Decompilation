#include "SonicMania.h"

ObjectPullChain *PullChain;

void PullChain_Update(void)
{
    RSDK_THIS(PullChain);
    if (!self->decorMode) {
        if (self->currentlyActive)
            self->currentlyActive = false;
        foreach_active(Player, player)
        {
            int32 plrID = RSDK.GetEntityID(player);
            if (self->timer[plrID] > 0)
                self->timer[plrID]--;
            if (!player->sidekick) {
                if (((1 << plrID) & self->activePlayers1)) {
                    if (self->chainOffset < 0x100000)
                        self->chainOffset += 0x8000;
                    if (self->chainOffset > 0x100000) 
                        self->chainOffset = 0x100000;

                    if (self->chainOffset == 0x100000) {
                        if (!self->down) {
                            self->currentlyActive = true;
                            self->activated = true;
                            self->toggled = !self->toggled;
                        }
                        self->down = true;
                    }
                }
                self->position.y = self->basePos.y + self->chainOffset;
            }

            int32 playerID = 1 << plrID;
            if (!((1 << plrID) & self->activePlayers1)) {
                if (!(self->activePlayers2 & playerID)) {
                    if (!Current || !(playerID & Current->activePlayers)) {
                        int32 x = abs(player->position.x - self->position.x);
                        int32 y = abs((player->position.y - 0x180000) - self->position.y);
                        if (MathHelpers_SquareRoot((y >> 16) * (y >> 16) + (x >> 16) * (x >> 16)) <= 8 && player->state != Player_State_None
                            && !self->timer[plrID]) {
                            self->activePlayers1 |= (1 << plrID);
                            self->activePlayers2 |= (1 << plrID);
                            RSDK.PlaySfx(Player->sfxGrab, 0, 255);
                            if (!player->sidekick)
                                RSDK.PlaySfx(PullChain->sfxPullChain, 0, 255);
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->animator, true, 6);
                            player->nextGroundState = StateMachine_None;
                            player->nextAirState    = StateMachine_None;
                            player->velocity.x      = 0;
                            player->velocity.y      = 0;
                            player->state           = Player_State_None;
                            if (!player->sidekick) {
                                for (int32 i = 0; i < 18; ++i) {
                                    self->field_D0[i] = 0;
                                }
                            }
                        }
                    }
                }
            }

            if ((playerID & self->activePlayers1)) {
                player->position.x = self->position.x;
                player->position.y = self->position.y;
                player->position.y += 0x1C0000;
#if RETRO_GAMEVER == VER_100
                if (!player->sidekick && PullChain_HandleDunkeyCode(player)) {
                    HandLauncher->dunkeyMode = true;
                    RSDK.PlaySfx(Ring->sfxRing, false, 0xFF);
                }
#endif
                if (player->jumpPress || player->animator.animationID != ANI_HANG || player->velocity.x || player->velocity.y) {
                    self->activePlayers1 &= ~(1 << plrID);
                    if (player->jumpPress) {
                        if (self->chainOffset < 0x100000 && !player->sidekick)
                            RSDK.StopSFX(PullChain->sfxPullChain);
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
                        player->velocity.x = 0;
                        player->velocity.y = -0x20000;
                        player->state      = Player_State_Air;
                    }
                    self->timer[plrID] = 30;
                }
                if ((playerID & self->activePlayers1))
                    continue;
            }

            if ((self->activePlayers2 & playerID)) {
                int32 x = abs(player->position.x - self->position.x);
                int32 y = abs(player->position.y - 0x180000 - self->position.y);
                if (MathHelpers_SquareRoot((x >> 16) * (x >> 16) + (y >> 16) * (y >> 16)) > 4)
                    self->activePlayers2 &= ~(1 << plrID);
            }
        }

        if (!self->activePlayers1) {
            self->down = 0;
            if (self->chainOffset > 0)
                self->chainOffset -= 0x8000;
            if (self->chainOffset < 0)
                self->chainOffset = 0;
        }
    }
}

void PullChain_LateUpdate(void) {}

void PullChain_StaticUpdate(void) {}

void PullChain_Draw(void)
{
    RSDK_THIS(PullChain);
    RSDK.DrawSprite(&self->animator, NULL, false);

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    for (int32 i = 0; i < self->length; ++i) {
        RSDK.DrawSprite(&self->animator2, &drawPos, false);
        drawPos.y -= 0x80000;
    }

    for (int32 i = 0; i < self->chainOffset; i += 0x80000) {
        RSDK.DrawSprite(&self->animator2, &drawPos, false);
        drawPos.y -= 0x80000;
    }
}

void PullChain_Create(void *data)
{
    RSDK_THIS(PullChain);
    self->active = ACTIVE_BOUNDS;
    if (!self->decorMode)
        self->drawOrder = Zone->playerDrawLow;
    else
        self->drawOrder = Zone->drawOrderLow;
    self->basePos       = self->position;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->hitbox.left   = -10;
    self->hitbox.top    = 3;
    self->hitbox.right  = 10;
    self->hitbox.bottom = 12;
    RSDK.SetSpriteAnimation(PullChain->aniFrames, 0, &self->animator, true, self->decorMode);
    RSDK.SetSpriteAnimation(PullChain->aniFrames, 1, &self->animator2, true, self->decorMode);
}

void PullChain_StageLoad(void)
{
    PullChain->aniFrames    = RSDK.LoadSpriteAnimation("HCZ/PullChain.bin", SCOPE_STAGE);
    PullChain->sfxPullChain = RSDK.GetSFX("HCZ/PullChain.wav");
}

#if RETRO_GAMEVER == VER_100
bool32 PullChain_HandleDunkeyCode(EntityPlayer *player)
{
    RSDK_THIS(PullChain);

    if (HandLauncher->dunkeyMode)
        return false;

    uint8 inputFlags = 0;

    if (player->left)
        inputFlags = 1;
    if (player->right)
        inputFlags |= 2;

    if (player->up)
        inputFlags |= 4;
    if (player->down)
        inputFlags |= 8;

    if (inputFlags == self->codeInputFlags)
        return false;

    for (int32 i = 0; i < 17; ++i) {
        self->field_D0[i] = self->field_D0[i + 1];
    }

    if (player->left) {
        self->field_D0[17] = 1;
    }
    else if (player->right) {
        self->field_D0[17] = 2;
    }
    else if (player->up) {
        self->field_D0[17] = 3;
    }
    else if (player->down) {
        self->field_D0[17] = 4;
    }
    else {
        self->field_D0[17] = 0;
    }
    
    bool32 flag = true;
    for (int32 i = 0; i < 18; ++i) {
        flag &= (self->field_D0[i] == PullChain->dunkeyCode[i]);
    }
    self->codeInputFlags = inputFlags;
    return flag;

}
#endif

#if RETRO_INCLUDE_EDITOR
void PullChain_EditorDraw(void)
{
    RSDK_THIS(PullChain);
    if (!self->decorMode)
        self->drawOrder = Zone->playerDrawLow;
    else
        self->drawOrder = Zone->drawOrderLow;
    RSDK.SetSpriteAnimation(PullChain->aniFrames, 0, &self->animator, true, self->decorMode);
    RSDK.SetSpriteAnimation(PullChain->aniFrames, 1, &self->animator2, true, self->decorMode);

    PullChain_Draw();
}

void PullChain_EditorLoad(void) { PullChain->aniFrames = RSDK.LoadSpriteAnimation("HCZ/PullChain.bin", SCOPE_STAGE); }
#endif

void PullChain_Serialize(void)
{
    RSDK_EDITABLE_VAR(PullChain, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(PullChain, VAR_BOOL, walkOnto);
    RSDK_EDITABLE_VAR(PullChain, VAR_UINT8, tag);
    RSDK_EDITABLE_VAR(PullChain, VAR_UINT32, length);
    RSDK_EDITABLE_VAR(PullChain, VAR_BOOL, decorMode);
}
