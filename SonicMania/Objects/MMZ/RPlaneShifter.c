// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: RPlaneShifter Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectRPlaneShifter *RPlaneShifter;

void RPlaneShifter_Update(void)
{
    RSDK_THIS(RPlaneShifter);

    self->prevActivePlayers = self->activePlayers;

    if (self->baseAnimator.animationID == 1)
        RSDK.ProcessAnimation(&self->baseAnimator);

    StateMachine_Run(self->state);
}

void RPlaneShifter_LateUpdate(void) {}

void RPlaneShifter_StaticUpdate(void)
{
    foreach_active(RPlaneShifter, shifter) { RSDK.AddDrawListRef(Zone->playerDrawGroup[1] + 1, RSDK.GetEntitySlot(shifter)); }
}

void RPlaneShifter_Draw(void) { RPlaneShifter_DrawSprites(); }

void RPlaneShifter_Create(void *data)
{
    RSDK_THIS(RPlaneShifter);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->active    = ACTIVE_BOUNDS;
    self->angle     = 0;

    if (SceneInfo->inEditor && !self->height)
        self->height = 10;

    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    RSDK.SetSpriteAnimation(RPlaneShifter->aniFrames, 1, &self->baseAnimator, true, 0);
    RSDK.SetSpriteAnimation(RPlaneShifter->aniFrames, 2, &RPlaneShifter->barAnimator, true, 0);
    self->state = RPlaneShifter_State_Init;
}

void RPlaneShifter_StageLoad(void)
{
    RPlaneShifter->active = ACTIVE_ALWAYS;

    RPlaneShifter->aniFrames = RSDK.LoadSpriteAnimation("MMZ/RPlaneShifter.bin", SCOPE_STAGE);

    RPlaneShifter->sfxTicTock = RSDK.GetSfx("MMZ/TicTock.wav");
}

void RPlaneShifter_DrawSprites(void)
{
    RSDK_THIS(RPlaneShifter);

    Vector2 drawPos = self->position;
    if (SceneInfo->inEditor || SceneInfo->currentDrawGroup == Zone->playerDrawGroup[1] + 1)
        RSDK.DrawSprite(&self->baseAnimator, &drawPos, false);

    drawPos.y -= 0x80000;

    uint8 poleAngles[3];
    poleAngles[0] = self->angle + 21;
    poleAngles[1] = self->angle + 106;
    poleAngles[2] = self->angle - 64;

    for (uint32 i = 0; i < self->height; ++i) {
        drawPos.y -= 0x100000;
        for (int32 p = 0; p < 3; ++p) {
            drawPos.x = self->position.x;

            if (SceneInfo->inEditor) {
                drawPos.x += 0x1C00 * RSDK.Cos256(poleAngles[p]);
                RSDK.DrawSprite(&RPlaneShifter->barAnimator, &drawPos, false);
            }
            else if (SceneInfo->currentDrawGroup != Zone->playerDrawGroup[1] + 1) {
                if ((poleAngles[p] >= 0 && poleAngles[p] < 0x80) || SceneInfo->currentDrawGroup == Zone->playerDrawGroup[1] + 1) {
                    drawPos.x += 0x1C00 * RSDK.Cos256(poleAngles[p]);
                    RSDK.DrawSprite(&RPlaneShifter->barAnimator, &drawPos, false);
                }
            }
            else if (poleAngles[p] >= 0x80) {
                drawPos.x += 0x1C00 * RSDK.Cos256(poleAngles[p]);
                RSDK.DrawSprite(&RPlaneShifter->barAnimator, &drawPos, false);
            }
        }
    }

    drawPos.x = self->position.x;
    drawPos.y -= 0x80000;
    if (SceneInfo->inEditor || SceneInfo->currentDrawGroup == Zone->playerDrawGroup[1] + 1)
        RSDK.DrawSprite(&self->baseAnimator, &drawPos, false);
}

void RPlaneShifter_HandlePlaneShift(EntityPlayer *player)
{
    RSDK_THIS(RPlaneShifter);

    player->state           = Player_State_Ground;
    player->onGround        = true;
    player->nextGroundState = StateMachine_None;
    player->nextAirState    = StateMachine_None;
    player->groundVel       = 0;

    RSDK.SetSpriteAnimation(player->aniFrames, ANI_IDLE, &player->animator, false, 0);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player->sidekick && (!player->sidekick || (self->prevActivePlayers & 1))) {
        player->collisionPlane = player1->collisionPlane;
        player->drawGroup      = player1->drawGroup - 1;
        player->direction      = player->direction == FLIP_NONE;
    }
    else {
        if (self->flags) {
            player->drawGroup      = player->collisionPlane ? Zone->playerDrawGroup[0] : Zone->playerDrawGroup[1];
            player->collisionPlane = player->collisionPlane ? 0 : 1;
        }
        else {
            player->drawGroup      = player->collisionPlane ? Zone->playerDrawGroup[1] : Zone->playerDrawGroup[0];
            player->collisionPlane = player->collisionPlane ? 0 : 1;
        }

        player->direction = player->direction == FLIP_NONE;
    }
}

void RPlaneShifter_State_Init(void)
{
    RSDK_THIS(RPlaneShifter);

    self->active  = ACTIVE_BOUNDS;
    self->visible = true;

    self->hitbox.left   = -2;
    self->hitbox.top    = -8 - (16 * self->height);
    self->hitbox.right  = 2;
    self->hitbox.bottom = -8;

    self->unused = 0;
    RSDK.SetSpriteAnimation(RPlaneShifter->aniFrames, 0, &self->baseAnimator, true, 0);

    self->state = RPlaneShifter_State_AwaitPlayer;
    RPlaneShifter_State_AwaitPlayer();
}

void RPlaneShifter_State_AwaitPlayer(void)
{
    RSDK_THIS(RPlaneShifter);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (!((1 << playerID) & self->activePlayers) && !((1 << playerID) & self->stoodPlayers)) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox) && player->onGround && !player->isChibi) {
                self->activePlayers |= 1 << playerID;
                self->stoodPlayers |= 1 << playerID;

                player->groundVel  = 0;
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->position.x = self->position.x;

                RSDK.SetSpriteAnimation(player->aniFrames, ANI_IDLE, &player->animator, false, 0);
                player->state           = Player_State_Static;
                player->nextGroundState = StateMachine_None;
                player->nextAirState    = StateMachine_None;
                player->onGround        = true;

                if (!player->sidekick) {
                    self->startAngle = self->angle << 16;
                    self->spinAngle  = 0;
                    RSDK.SetSpriteAnimation(RPlaneShifter->aniFrames, 1, &self->baseAnimator, true, 0);
                    RSDK.PlaySfx(RPlaneShifter->sfxTicTock, false, 255);
                    self->state = RPlaneShifter_State_Spinning;
                }
                else {
                    player->abilityValues[0] = player->left;
                    player->abilityValues[1] = player->right;
                }
            }
        }

        if (player->sidekick) {
            if (self->activePlayers & (1 << playerID)) {
                if (player->jumpPress) {
                    self->activePlayers &= ~(1 << playerID);
                    self->stoodPlayers &= ~(1 << playerID);
                    Player_Action_Jump(player);
                }

                if (!player->abilityValues[0] && player->left) {
                    self->activePlayers &= ~(1 << playerID);
                    self->stoodPlayers &= ~(1 << playerID);
                    player->state = Player_State_Ground;
                }

                if (!player->abilityValues[1] && player->right) {
                    self->activePlayers &= ~(1 << playerID);
                    self->stoodPlayers &= ~(1 << playerID);
                    player->state = Player_State_Ground;
                }

                player->abilityValues[0] = player->left;
                player->abilityValues[1] = player->right;
            }
        }

        if (Player_CheckCollisionTouch(player, self, &self->hitbox))
            self->stoodPlayers |= 1 << playerID;
        else
            self->stoodPlayers &= ~(1 << playerID);
    }
}

void RPlaneShifter_State_Spinning(void)
{
    RSDK_THIS(RPlaneShifter);

    if (abs(self->spinAngle >> 16) < 128) {
        uint8 updateSpinSpeed = 1;
        if (self->rotateDir && self->spinAngle >> 16 > -65)
            updateSpinSpeed = 0;
        else if (!self->rotateDir && self->spinAngle >> 16 < 65)
            updateSpinSpeed = 0;

        int32 speed = self->rotateDir ? -0x3800 : 0x3800;

        if (updateSpinSpeed) {
            self->spinSpeed -= speed;
            if (self->rotateDir && self->spinSpeed > -0x3800) {
                self->spinSpeed = speed;
                updateSpinSpeed = 2;
            }
            else if (!self->rotateDir && self->spinSpeed < 0x3800) {
                self->spinSpeed = speed;
                updateSpinSpeed = 2;
            }
        }
        else {
            self->spinSpeed += speed;
        }

        if (self->rotateDir && self->spinSpeed > -0x3800 && updateSpinSpeed != 2) {
            self->spinSpeed = speed;
        }

        self->spinAngle += self->spinSpeed;
        self->angle = (self->startAngle + self->spinAngle) >> 16;

        if (self->angle < 0)

            self->angle += 0x100;
        self->angle &= 0xFF;
    }

    if (abs(self->spinAngle >> 16) >= 128) {
        self->spinSpeed = 0;
        self->spinAngle = 128 << 16;
        if (!self->rotateDir)
            self->spinAngle = -self->spinAngle;

        self->angle = (self->startAngle + self->spinAngle) >> 16;
        if (self->angle < 0)
            self->angle += 0x100;
        self->angle &= 0xFF;

        RSDK.SetSpriteAnimation(RPlaneShifter->aniFrames, 0, &self->baseAnimator, true, 0);
        self->state = RPlaneShifter_State_FinishSpin;
    }

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (!((1 << playerID) & self->activePlayers) && !((1 << playerID) & self->stoodPlayers)
            && Player_CheckCollisionTouch(player, self, &self->hitbox) && player->onGround && !player->isChibi) {
            self->activePlayers |= 1 << playerID;
            player->groundVel  = 0;
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->position.x = self->position.x;

            RSDK.SetSpriteAnimation(player->aniFrames, ANI_IDLE, &player->animator, false, 0);
            player->state           = Player_State_Static;
            player->nextGroundState = StateMachine_None;
            player->nextAirState    = StateMachine_None;
            player->onGround        = true;
        }

        if ((1 << playerID) & self->activePlayers) {
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_TWISTER, &player->animator, false, 0);

            int32 angle    = (self->spinAngle >> 16) & 0xFF;
            int32 negAngle = 0xFF - angle;
            if (player->direction)
                negAngle = angle;

            int32 frame = 0;
            switch (negAngle >> 6) {
                case 0:
                case 1: frame = 9 * negAngle / 128; break;
                case 2:
                case 3: frame = 16 * (negAngle - 128) / 128 + 9; break;
                default: break;
            }

            player->animator.frameID = frame % 24;
            player->animator.speed   = 0;
        }

        if (Player_CheckCollisionTouch(player, self, &self->hitbox))
            self->stoodPlayers |= 1 << playerID;
        else
            self->stoodPlayers &= ~(1 << playerID);
    }
}

void RPlaneShifter_State_FinishSpin(void)
{
    RSDK_THIS(RPlaneShifter);
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if ((1 << playerID) & self->activePlayers) {
            self->activePlayers &= ~(1 << playerID);
            RPlaneShifter_HandlePlaneShift(player);
        }

        if (Player_CheckCollisionTouch(player, self, &self->hitbox))
            self->stoodPlayers |= 1 << playerID;
        else
            self->stoodPlayers &= ~(1 << playerID);

        // wait for P1 to get out of the spinner
        if (!player->sidekick && !((1 << playerID) & self->activePlayers) && !((1 << playerID) & self->stoodPlayers)) {
            self->activePlayers = 0;
            self->spinAngle     = 0;
            self->startAngle    = 0;
            self->rotateDir     = !self->rotateDir;
            self->state         = RPlaneShifter_State_AwaitPlayer;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void RPlaneShifter_EditorDraw(void) { RPlaneShifter_DrawSprites(); }

void RPlaneShifter_EditorLoad(void)
{
    RPlaneShifter->aniFrames = RSDK.LoadSpriteAnimation("MMZ/RPlaneShifter.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(RPlaneShifter, flags);
    RSDK_ENUM_VAR("Plane A -> Low Draw Group, Plane B -> High Draw Group", 0);
    RSDK_ENUM_VAR("Plane A -> High Draw Group, Plane B -> Low Draw Group", 1);
}
#endif

void RPlaneShifter_Serialize(void)
{
    RSDK_EDITABLE_VAR(RPlaneShifter, VAR_UINT32, height);
    RSDK_EDITABLE_VAR(RPlaneShifter, VAR_UINT8, flags);
}
