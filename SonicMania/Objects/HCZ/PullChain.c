// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PullChain Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPullChain *PullChain;

void PullChain_Update(void)
{
    RSDK_THIS(PullChain);

    if (!self->decorMode) {
        if (self->currentlyActive)
            self->currentlyActive = false;

        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntitySlot(player);

            if (self->grabDelay[playerID] > 0)
                self->grabDelay[playerID]--;

            if (!player->sidekick) {
                if (((1 << playerID) & self->activePlayers)) {
                    if (self->chainOffset < 0x100000)
                        self->chainOffset += 0x8000;

                    if (self->chainOffset > 0x100000)
                        self->chainOffset = 0x100000;

                    if (self->chainOffset == 0x100000) {
                        if (!self->down) {
                            self->currentlyActive = true;
                            self->activated       = true;
                            self->toggled         = !self->toggled;
                        }

                        self->down = true;
                    }
                }

                self->position.y = self->basePos.y + self->chainOffset;
            }

            if (!((1 << playerID) & self->activePlayers)) {
                if (!(self->releasedPlayers & (1 << playerID))) {
                    if (!Current || !((1 << playerID) & Current->activePlayers)) {
                        int32 x = abs(player->position.x - self->position.x);
                        int32 y = abs((player->position.y - 0x180000) - self->position.y);

                        if (MathHelpers_SquareRoot((x >> 16) * (x >> 16) + (y >> 16) * (y >> 16)) <= 8 && player->state != Player_State_Static
                            && !self->grabDelay[playerID]) {
                            self->activePlayers |= 1 << playerID;
                            self->releasedPlayers |= 1 << playerID;

                            RSDK.PlaySfx(Player->sfxGrab, false, 0xFF);

                            if (!player->sidekick)
                                RSDK.PlaySfx(PullChain->sfxPullChain, false, 0xFF);

                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->animator, true, 6);
                            player->nextGroundState = StateMachine_None;
                            player->nextAirState    = StateMachine_None;
                            player->velocity.x      = 0;
                            player->velocity.y      = 0;
                            player->state           = Player_State_Static;

                            // Reset the dunky code inputs if a proper player grabs it
                            if (!player->sidekick) {
                                for (int32 i = 0; i < 18; ++i) self->cheatCodeInputs[i] = 0;
                            }
                        }
                    }
                }
            }

            if ((1 << playerID) & self->activePlayers) {
                player->position.x = self->position.x;
                player->position.y = self->position.y + 0x1C0000;

                // R.I.P dunkey mode, you are very missed :(
#if GAME_VERSION == VER_100
                if (!player->sidekick && PullChain_HandleDunkeyCode(player)) {
                    HandLauncher->dunkeyMode = true;
                    RSDK.PlaySfx(Ring->sfxRing, false, 0xFF);
                }
#endif

                if (player->jumpPress || player->animator.animationID != ANI_HANG || player->velocity.x || player->velocity.y) {
                    self->activePlayers &= ~(1 << playerID);
                    if (player->jumpPress) {
                        if (self->chainOffset < 0x100000 && !player->sidekick)
                            RSDK.StopSfx(PullChain->sfxPullChain);

                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
                        player->velocity.x = 0;
                        player->velocity.y = -0x20000;
                        player->state      = Player_State_Air;
                    }

                    self->grabDelay[playerID] = 30;
                }

                if ((1 << playerID) & self->activePlayers)
                    continue;
            }

            // Make sure we're far enough away before unsetting this
            // This controls if players can grab the hook
            if (self->releasedPlayers & (1 << playerID)) {
                int32 x = abs(player->position.x - self->position.x);
                int32 y = abs(player->position.y - 0x180000 - self->position.y);
                if (MathHelpers_SquareRoot((x >> 16) * (x >> 16) + (y >> 16) * (y >> 16)) > 4)
                    self->releasedPlayers &= ~(1 << playerID);
            }
        }

        if (!self->activePlayers) {
            self->down = false;

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

    RSDK.DrawSprite(&self->hookAnimator, NULL, false);

    Vector2 drawPos = self->position;
    for (int32 i = 0; i < self->length; ++i) {
        RSDK.DrawSprite(&self->chainAnimator, &drawPos, false);
        drawPos.y -= 0x80000;
    }

    for (int32 i = 0; i < self->chainOffset; i += 0x80000) {
        RSDK.DrawSprite(&self->chainAnimator, &drawPos, false);
        drawPos.y -= 0x80000;
    }
}

void PullChain_Create(void *data)
{
    RSDK_THIS(PullChain);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = self->decorMode ? Zone->objectDrawGroup[0] : Zone->playerDrawGroup[0];
    self->basePos       = self->position;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    self->hitbox.left   = -10;
    self->hitbox.top    = 3;
    self->hitbox.right  = 10;
    self->hitbox.bottom = 12;

    RSDK.SetSpriteAnimation(PullChain->aniFrames, 0, &self->hookAnimator, true, self->decorMode);
    RSDK.SetSpriteAnimation(PullChain->aniFrames, 1, &self->chainAnimator, true, self->decorMode);
}

void PullChain_StageLoad(void)
{
    PullChain->aniFrames = RSDK.LoadSpriteAnimation("HCZ/PullChain.bin", SCOPE_STAGE);

    PullChain->sfxPullChain = RSDK.GetSfx("HCZ/PullChain.wav");
}

#if GAME_VERSION == VER_100
bool32 PullChain_HandleDunkeyCode(EntityPlayer *player)
{
    RSDK_THIS(PullChain);

    if (HandLauncher->dunkeyMode)
        return false;

    uint8 buttonMasks = PULLCHAIN_INPUT_NONE;
    if (player->left)
        buttonMasks = 1;

    if (player->right)
        buttonMasks |= 2;

    if (player->up)
        buttonMasks |= 4;

    if (player->down)
        buttonMasks |= 8;

    if (buttonMasks == self->codeButtonMasks)
        return false;

    for (int32 i = 1; i < 18; ++i) self->cheatCodeInputs[i - 1] = self->cheatCodeInputs[i];

    self->cheatCodeInputs[17] = PULLCHAIN_INPUT_NONE;
    if (player->left)
        self->cheatCodeInputs[17] = PULLCHAIN_INPUT_LEFT;
    else if (player->right)
        self->cheatCodeInputs[17] = PULLCHAIN_INPUT_RIGHT;
    else if (player->up)
        self->cheatCodeInputs[17] = PULLCHAIN_INPUT_UP;
    else if (player->down)
        self->cheatCodeInputs[17] = PULLCHAIN_INPUT_DOWN;

    bool32 activatedCheatCode = true;
    for (int32 i = 0; i < 18; ++i) activatedCheatCode &= (self->cheatCodeInputs[i] == PullChain->dunkeyCode[i]);
    self->codeButtonMasks = buttonMasks;

    return activatedCheatCode;
}
#endif

#if GAME_INCLUDE_EDITOR
void PullChain_EditorDraw(void)
{
    RSDK_THIS(PullChain);

    self->drawGroup = self->decorMode ? Zone->objectDrawGroup[0] : Zone->playerDrawGroup[0];
    RSDK.SetSpriteAnimation(PullChain->aniFrames, 0, &self->hookAnimator, true, self->decorMode);
    RSDK.SetSpriteAnimation(PullChain->aniFrames, 1, &self->chainAnimator, true, self->decorMode);

    PullChain_Draw();
}

void PullChain_EditorLoad(void)
{
    PullChain->aniFrames = RSDK.LoadSpriteAnimation("HCZ/PullChain.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PullChain, type);
    RSDK_ENUM_VAR("Normal", PULLCHAIN_NORMAL);
}
#endif

void PullChain_Serialize(void)
{
    RSDK_EDITABLE_VAR(PullChain, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(PullChain, VAR_BOOL, walkOnto);
    RSDK_EDITABLE_VAR(PullChain, VAR_UINT8, tag);
    RSDK_EDITABLE_VAR(PullChain, VAR_UINT32, length);
    RSDK_EDITABLE_VAR(PullChain, VAR_BOOL, decorMode);
}
