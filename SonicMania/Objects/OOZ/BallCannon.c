// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BallCannon Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBallCannon *BallCannon;

void BallCannon_Update(void)
{
    RSDK_THIS(BallCannon);

    StateMachine_Run(self->state);
}

void BallCannon_LateUpdate(void) {}

void BallCannon_StaticUpdate(void) {}

void BallCannon_Draw(void)
{
    RSDK_THIS(BallCannon);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void BallCannon_Create(void *data)
{
    RSDK_THIS(BallCannon);

    self->drawGroup = Zone->playerDrawGroup[0];
    self->visible   = true;

    if (!SceneInfo->inEditor) {
        if (data) {
            self->visible       = true;
            self->drawFX        = FX_ROTATE;
            self->active        = ACTIVE_NORMAL;
            self->rotationSpeed = RSDK.Rand(-8, 8);

            RSDK.SetSpriteAnimation(BallCannon->aniFrames, 3, &self->animator, true, VOID_TO_INT(data));
            self->state = BallCannon_State_CorkDebris;
        }
        else {
            self->drawFX        = FX_ROTATE | FX_FLIP;
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x400000;

            switch (self->type) {
                case BALLCANNON_CANNON:
                    if (self->angle >= 4)
                        self->direction = FLIP_X;

                    self->rotation = (self->angle + self->direction + 1) << 7;

                    switch (self->angle) {
                        case BALLCANNON_DIR_RIGHT_CW: // Right -> Down
                        case BALLCANNON_DIR_LEFT_CCW: // Left -> Down
                            self->velocity.y = 0x100000;
                            break;

                        case BALLCANNON_DIR_DOWN_CW: // Down -> Left
                        case BALLCANNON_DIR_UP_CCW:  // Up -> Left
                            self->velocity.x = -0x100000;
                            break;

                        case BALLCANNON_DIR_LEFT_CW:   // Left -> Up
                        case BALLCANNON_DIR_RIGHT_CCW: // Right -> Up
                            self->velocity.y = -0x100000;
                            break;

                        case BALLCANNON_DIR_UP_CW:    // Up -> Right
                        case BALLCANNON_DIR_DOWN_CCW: // Down -> Right
                            self->velocity.x = 0x100000;
                            break;

                        default: break;
                    }
                    RSDK.SetSpriteAnimation(BallCannon->aniFrames, 0, &self->animator, true, 0);
                    self->state = BallCannon_State_Idle;
                    break;

                case BALLCANNON_CORKV:
                    RSDK.SetSpriteAnimation(BallCannon->aniFrames, 3, &self->animator, true, 0);
                    self->velocity.y = -0x80000;
                    self->state      = BallCannon_State_CorkBlocked;
                    break;

                case BALLCANNON_CORKH:
                    RSDK.SetSpriteAnimation(BallCannon->aniFrames, 4, &self->animator, true, 0);
                    self->velocity.x = 0x80000;
                    self->state      = BallCannon_State_CorkBlocked;
                    break;
            }
        }
    }
}

void BallCannon_StageLoad(void)
{
    BallCannon->aniFrames = RSDK.LoadSpriteAnimation("OOZ/BallCannon.bin", SCOPE_STAGE);

    BallCannon->hitboxCannon.top    = -4;
    BallCannon->hitboxCannon.left   = -4;
    BallCannon->hitboxCannon.right  = 4;
    BallCannon->hitboxCannon.bottom = 4;

    BallCannon->hitboxCorkBlock.top    = -16;
    BallCannon->hitboxCorkBlock.left   = -16;
    BallCannon->hitboxCorkBlock.right  = 16;
    BallCannon->hitboxCorkBlock.bottom = 16;

    BallCannon->hitboxCorkEntry.top    = -4;
    BallCannon->hitboxCorkEntry.left   = -8;
    BallCannon->hitboxCorkEntry.right  = 8;
    BallCannon->hitboxCorkEntry.bottom = 4;

    BallCannon->sfxLedgeBreak = RSDK.GetSfx("Stage/LedgeBreak.wav");
    BallCannon->sfxFire       = RSDK.GetSfx("Stage/CannonFire.wav");
}

void BallCannon_CheckPlayerEntry(void)
{
    RSDK_THIS(BallCannon);

    if (RSDK.CheckOnScreen(self, NULL)) {
        foreach_all(Player, player)
        {
            if (Player_CheckValidState(player)) {
                int32 playerID = RSDK.GetEntitySlot(player);

                if (self->playerTimers[playerID]) {
                    self->playerTimers[playerID]--;
                }
                else {
                    if ((1 << playerID) & self->activePlayers) {
                        if (player->state != Player_State_Static)
                            self->activePlayers &= ~(1 << playerID);
                    }
                    else {
                        if (Player_CheckCollisionTouch(player, self, &BallCannon->hitboxCannon)) {
                            RSDK.PlaySfx(Player->sfxRoll, false, 0xFF);
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);

                            player->position.x     = self->position.x;
                            player->position.y     = self->position.y;
                            player->velocity.x     = 0;
                            player->velocity.y     = 0;
                            player->tileCollisions = TILECOLLISION_NONE;
                            player->interaction    = false;
                            player->blinkTimer     = 0;
                            player->visible        = false;
                            player->state          = Player_State_Static;
                            self->activePlayers |= 1 << playerID;
                            self->active   = ACTIVE_NORMAL;
                            self->rotation = (self->angle + self->direction + 1) << 7;
                            self->drawFX   = FX_ROTATE;
                            RSDK.SetSpriteAnimation(BallCannon->aniFrames, 0, &self->animator, true, 0);
                            self->state = BallCannon_State_Inserted;
                        }
                    }
                }
            }
        }
    }
    else {
        self->active   = ACTIVE_BOUNDS;
        self->rotation = (self->angle + self->direction + 1) << 7;
    }
}

void BallCannon_State_Idle(void) { BallCannon_CheckPlayerEntry(); }

void BallCannon_State_Inserted(void)
{
    RSDK_THIS(BallCannon);

    BallCannon_CheckPlayerEntry();

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(BallCannon->aniFrames, 1, &self->animator, true, 0);
        self->drawFX = FX_FLIP;
        self->state  = BallCannon_State_Turning;
    }
}

void BallCannon_State_Turning(void)
{
    RSDK_THIS(BallCannon);

    BallCannon_CheckPlayerEntry();

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(BallCannon->aniFrames, 2, &self->animator, true, 0);
        self->drawFX   = FX_ROTATE;
        self->state    = BallCannon_State_EjectPlayer;
        self->rotation = (self->angle - self->direction + 2) << 7;
    }
}

void BallCannon_State_EjectPlayer(void)
{
    RSDK_THIS(BallCannon);

    BallCannon_CheckPlayerEntry();

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        foreach_all(Player, player)
        {
            if (Player_CheckValidState(player)) {
                int32 playerID = RSDK.GetEntitySlot(player);

                if (((1 << playerID) & self->activePlayers)) {
                    RSDK.PlaySfx(BallCannon->sfxFire, false, 0xFF);

                    player->velocity = self->velocity;
                    player->visible  = true;

                    if (self->exit) {
                        player->onGround       = false;
                        player->applyJumpCap   = false;
                        player->state          = Player_State_Air;
                        player->tileCollisions = TILECOLLISION_DOWN;
                        player->interaction    = true;
                    }

                    self->activePlayers &= ~(1 << playerID);
                    self->playerTimers[playerID] = 15;
                }
            }
        }

        self->state = BallCannon_State_Idle;
    }
}

void BallCannon_State_CorkBlocked(void)
{
    RSDK_THIS(BallCannon);

    foreach_active(Player, player)
    {
        Animator animator;

        memcpy(&animator, &player->animator, sizeof(Animator));
        int32 storeX    = player->position.x;
        int32 storeY    = player->position.y;
        int32 storeVelX = player->velocity.x;
        int32 storeVelY = player->velocity.y;

        if (Player_CheckCollisionBox(player, self, &BallCannon->hitboxCorkBlock) == C_TOP) {
            if (player->animator.animationID == ANI_JUMP || player->state == Player_State_DropDash
#if MANIA_USE_PLUS
                || player->state == Player_State_MightyHammerDrop
#endif
            ) {
                if (storeVelY >= 0 && !player->groundedStore) {
                    for (int32 i = 0; i < 16; ++i) {
                        EntityBallCannon *debris =
                            CREATE_ENTITY(BallCannon, INT_TO_VOID((i & 3) + 1), self->position.x + BallCannon->corkDebrisOffset[(i * 2) + 0],
                                          self->position.y + BallCannon->corkDebrisVelocity[(i * 2) + 1]);
                        debris->velocity.x = BallCannon->corkDebrisVelocity[(i * 2) + 0];
                        debris->velocity.y = BallCannon->corkDebrisVelocity[(i * 2) + 1];
                    }

                    RSDK.PlaySfx(BallCannon->sfxLedgeBreak, false, 0xFF);

                    memcpy(&player->animator, &animator, sizeof(Animator));
                    player->velocity.x = storeVelX;
                    player->velocity.y = storeVelY;
                    player->position.x = storeX;
                    player->position.y = storeY;
                    player->onGround   = false;
                    self->active       = ACTIVE_NORMAL;
                    self->visible      = false;
                    self->state        = BallCannon_State_CorkOpened;

                    foreach_break;
                }
            }
        }
    }
}

void BallCannon_State_CorkOpened(void)
{
    RSDK_THIS(BallCannon);

    if (RSDK.CheckOnScreen(self, NULL)) {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntitySlot(player);

            if (self->playerTimers[playerID]) {
                self->playerTimers[playerID]--;
            }
            else {
                if (Player_CheckCollisionTouch(player, self, &BallCannon->hitboxCorkEntry)) {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    RSDK.PlaySfx(BallCannon->sfxFire, false, 0xFF);

                    player->state                = Player_State_Static;
                    player->nextGroundState      = StateMachine_None;
                    player->nextAirState         = StateMachine_None;
                    player->position             = self->position;
                    player->velocity             = self->velocity;
                    player->tileCollisions       = TILECOLLISION_NONE;
                    player->interaction          = false;
                    player->onGround             = false;
                    self->playerTimers[playerID] = 15;
                }
            }
        }
    }
    else {
        self->visible = true;
        for (int32 i = 0; i < Player->playerCount; ++i) self->playerTimers[i] = 0;
        self->state = BallCannon_State_CorkBlocked;
    }
}

void BallCannon_State_CorkDebris(void)
{
    RSDK_THIS(BallCannon);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->rotation += self->rotationSpeed;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void BallCannon_EditorDraw(void)
{
    RSDK_THIS(BallCannon);

    self->drawFX        = FX_ROTATE | FX_FLIP;
    self->rotation      = 0;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    self->velocity.x    = 0;
    self->velocity.y    = 0;

    switch (self->type) {
        default: break;

        case BALLCANNON_CANNON:
            if (self->angle >= 4)
                self->direction = FLIP_X;

            self->rotation = (self->angle + self->direction + 1) << 7;

            switch (self->angle) {
                case BALLCANNON_DIR_RIGHT_CW: // Right -> Down
                case BALLCANNON_DIR_LEFT_CCW: // Left -> Down
                    self->velocity.y = 0x200000;
                    break;

                case BALLCANNON_DIR_DOWN_CW: // Down -> Left
                case BALLCANNON_DIR_UP_CCW:  // Up -> Left
                    self->velocity.x = -0x200000;
                    break;

                case BALLCANNON_DIR_LEFT_CW:   // Left -> Up
                case BALLCANNON_DIR_RIGHT_CCW: // Right -> Up
                    self->velocity.y = -0x200000;
                    break;

                case BALLCANNON_DIR_UP_CW:    // Up -> Right
                case BALLCANNON_DIR_DOWN_CCW: // Down -> Right
                    self->velocity.x = 0x200000;
                    break;

                default: break;
            }

            RSDK.SetSpriteAnimation(BallCannon->aniFrames, 0, &self->animator, true, 0);
            break;

        case BALLCANNON_CORKV: RSDK.SetSpriteAnimation(BallCannon->aniFrames, 3, &self->animator, true, 0); break;

        case BALLCANNON_CORKH: RSDK.SetSpriteAnimation(BallCannon->aniFrames, 4, &self->animator, true, 0); break;
    }

    BallCannon_Draw();

    if (showGizmos() && self->type == BALLCANNON_CANNON) {
        self->rotation = (self->angle - self->direction + 2) << 7;

        self->inkEffect = INK_BLEND;

        BallCannon_Draw();

        self->inkEffect = INK_NONE;

        // Draw the direction the player will be shot from (the names are a little confusing on their own)
        DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x + self->velocity.x, self->position.y + self->velocity.y, 0x00FF00,
                              INK_NONE, 0xFF);
    }
}

void BallCannon_EditorLoad(void)
{
    BallCannon->aniFrames = RSDK.LoadSpriteAnimation("OOZ/BallCannon.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(BallCannon, type);
    RSDK_ENUM_VAR("Cannon", BALLCANNON_CANNON);
    RSDK_ENUM_VAR("Cork V", BALLCANNON_CORKV);
    RSDK_ENUM_VAR("Cork H", BALLCANNON_CORKH);

    RSDK_ACTIVE_VAR(BallCannon, angle);
    RSDK_ENUM_VAR("Right (Rotates Clockwise)", BALLCANNON_DIR_RIGHT_CW);
    RSDK_ENUM_VAR("Down (Rotates Clockwise)", BALLCANNON_DIR_DOWN_CW);
    RSDK_ENUM_VAR("Left (Rotates Clockwise)", BALLCANNON_DIR_LEFT_CW);
    RSDK_ENUM_VAR("Up (Rotates Clockwise)", BALLCANNON_DIR_UP_CW);
    RSDK_ENUM_VAR("Down (Rotates Anti-Clockwise)", BALLCANNON_DIR_DOWN_CCW);
    RSDK_ENUM_VAR("Left (Rotates Anti-Clockwise)", BALLCANNON_DIR_LEFT_CCW);
    RSDK_ENUM_VAR("Up (Rotates Anti-Clockwise)", BALLCANNON_DIR_UP_CCW);
    RSDK_ENUM_VAR("Right (Rotates Anti-Clockwise)", BALLCANNON_DIR_RIGHT_CCW);
}
#endif

void BallCannon_Serialize(void)
{
    RSDK_EDITABLE_VAR(BallCannon, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(BallCannon, VAR_ENUM, angle);
    RSDK_EDITABLE_VAR(BallCannon, VAR_BOOL, exit);
}
