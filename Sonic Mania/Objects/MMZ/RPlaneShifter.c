// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: RPlaneShifter Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectRPlaneShifter *RPlaneShifter;

void RPlaneShifter_Update(void)
{
    RSDK_THIS(RPlaneShifter);

    self->prevActivePlayers1 = self->activePlayers1;
    if (self->animator.animationID == 1)
        RSDK.ProcessAnimation(&self->animator);
    StateMachine_Run(self->state);
}

void RPlaneShifter_LateUpdate(void) {}

void RPlaneShifter_StaticUpdate(void)
{
    foreach_active(RPlaneShifter, shifter) { RSDK.AddDrawListRef(Zone->playerDrawHigh + 1, RSDK.GetEntityID(shifter)); }
}

void RPlaneShifter_Draw(void) { RPlaneShifter_DrawSprites(); }

void RPlaneShifter_Create(void *data)
{
    RSDK_THIS(RPlaneShifter);
    self->visible   = true;
    self->drawOrder = Zone->drawOrderLow;
    self->active    = ACTIVE_BOUNDS;
    self->angle     = 0;
    if (SceneInfo->inEditor && !self->height)
        self->height = 10;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(RPlaneShifter->aniFrames, 1, &self->animator, true, 0);
    RSDK.SetSpriteAnimation(RPlaneShifter->aniFrames, 2, &RPlaneShifter->animator, true, 0);
    self->state = RPlaneShifter_State_Setup;
}

void RPlaneShifter_StageLoad(void)
{
    RPlaneShifter->active     = ACTIVE_ALWAYS;
    RPlaneShifter->aniFrames  = RSDK.LoadSpriteAnimation("MMZ/RPlaneShifter.bin", SCOPE_STAGE);
    RPlaneShifter->sfxTicTock = RSDK.GetSfx("MMZ/TicTock.wav");
}

void RPlaneShifter_DrawSprites(void)
{
    RSDK_THIS(RPlaneShifter);
    uint8 poleAngles[3];
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->inEditor || SceneInfo->currentDrawGroup == Zone->playerDrawHigh + 1)
        RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.y -= 0x80000;
    poleAngles[0] = self->angle + 21;
    poleAngles[1] = self->angle + 106;
    poleAngles[2] = self->angle - 64;
    for (int32 i = 0; i < self->height; ++i) {
        drawPos.y -= 0x100000;
        for (int32 p = 0; p < 3; ++p) {
            drawPos.x = self->position.x;
            if (SceneInfo->inEditor) {
                drawPos.x += 0x1C00 * RSDK.Cos256(poleAngles[p]);
                RSDK.DrawSprite(&RPlaneShifter->animator, &drawPos, false);
            }
            else if (SceneInfo->currentDrawGroup != Zone->playerDrawHigh + 1) {
                if ((poleAngles[p] >= 0 && poleAngles[p] < 0x80) || SceneInfo->currentDrawGroup == Zone->playerDrawHigh + 1) {
                    drawPos.x += 0x1C00 * RSDK.Cos256(poleAngles[p]);
                    RSDK.DrawSprite(&RPlaneShifter->animator, &drawPos, false);
                }
            }
            else if (poleAngles[p] >= 0x80) {
                drawPos.x += 0x1C00 * RSDK.Cos256(poleAngles[p]);
                RSDK.DrawSprite(&RPlaneShifter->animator, &drawPos, false);
            }
        }
    }

    drawPos.x = self->position.x;
    drawPos.y -= 0x80000;
    if (SceneInfo->inEditor || SceneInfo->currentDrawGroup == Zone->playerDrawHigh + 1)
        RSDK.DrawSprite(&self->animator, &drawPos, false);
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
    if (player->sidekick && (!player->sidekick || (self->prevActivePlayers1 & 1))) {
        player->collisionPlane = player1->collisionPlane;
        player->drawOrder      = player1->drawOrder - 1;
        player->direction      = player->direction == FLIP_NONE;
    }
    else {
        if (self->flags) {
            player->drawOrder = player->collisionPlane ? Zone->playerDrawLow : Zone->playerDrawHigh;
            if (!player->collisionPlane)
                player->collisionPlane = 1;
            else
                player->collisionPlane = 0;
        }
        else {
            player->drawOrder = !player->collisionPlane ? Zone->playerDrawLow : Zone->playerDrawHigh;
            if (player->collisionPlane)
                player->collisionPlane = 0;
            else
                player->collisionPlane = 1;
        }

        player->direction = player->direction == FLIP_NONE;
    }
}

void RPlaneShifter_State_Setup(void)
{
    RSDK_THIS(RPlaneShifter);

    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->hitbox.left   = -2;
    self->field_6C      = 0;
    self->hitbox.right  = 2;
    self->hitbox.bottom = -8;
    self->hitbox.top    = -8 - (16 * self->height);
    RSDK.SetSpriteAnimation(RPlaneShifter->aniFrames, 0, &self->animator, true, 0);
    self->state = RPlaneShifter_Unknown3;
    RPlaneShifter_Unknown3();
}

void RPlaneShifter_Unknown3(void)
{
    RSDK_THIS(RPlaneShifter);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);

        if (!((1 << playerID) & self->activePlayers1) && !((1 << playerID) & self->activePlayers2)) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox) && player->onGround && !player->isChibi) {
                self->activePlayers1 |= 1 << playerID;
                self->activePlayers2 |= 1 << playerID;

                player->groundVel  = 0;
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->position.x = self->position.x;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_IDLE, &player->animator, false, 0);
                player->state           = Player_State_None;
                player->nextGroundState = StateMachine_None;
                player->nextAirState    = StateMachine_None;
                player->onGround        = true;
                if (!player->sidekick) {
                    self->field_74 = self->angle << 16;
                    self->field_78 = 0;
                    RSDK.SetSpriteAnimation(RPlaneShifter->aniFrames, 1, &self->animator, true, 0);
                    RSDK.PlaySfx(RPlaneShifter->sfxTicTock, false, 255);
                    self->state = RPlaneShifter_Unknown4;
                }
                else {
                    player->abilityValues[0] = player->left;
                    player->abilityValues[1] = player->right;
                }
            }
        }

        if (player->sidekick) {
            if (self->activePlayers1 & (1 << playerID)) {
                if (player->jumpPress) {
                    self->activePlayers1 &= ~(1 << playerID);
                    self->activePlayers2 &= ~(1 << playerID);
                    Player_StartJump(player);
                }
                if (!player->abilityValues[0] && player->left) {
                    self->activePlayers1 &= ~(1 << playerID);
                    self->activePlayers2 &= ~(1 << playerID);
                    player->state = Player_State_Ground;
                }
                if (!player->abilityValues[1] && player->right) {
                    self->activePlayers1 &= ~(1 << playerID);
                    self->activePlayers2 &= ~(1 << playerID);
                    player->state = Player_State_Ground;
                }
                player->abilityValues[0] = player->left;
                player->abilityValues[1] = player->right;
            }
        }

        if (Player_CheckCollisionTouch(player, self, &self->hitbox))
            self->activePlayers2 |= (1 << playerID);
        else
            self->activePlayers2 &= ~(1 << playerID);
    }
}

void RPlaneShifter_Unknown4(void)
{
    RSDK_THIS(RPlaneShifter);

    if (abs(self->field_78 >> 16) < 128) {
        bool32 flag = false;
        if (self->field_64 && self->field_78 >> 16 > -65) {
            flag = false;
        }
        else if (!self->field_64 && self->field_78 >> 16 < 65) {
            flag = false;
        }
        else {
            flag = true;
        }

        int32 speed = 0;
        if (!self->field_64)
            speed = 0x3800;
        else
            speed = -0x3800;

        if (flag) {
            self->field_70 -= speed;
            if (self->field_64 && self->field_70 > -0x3800) {
                self->field_70 = speed;
                flag             = 2;
            }
            else if (!self->field_64 && self->field_70 < 0x3800) {
                self->field_70 = speed;
                flag             = 2;
            }
        }
        else {
            self->field_70 += speed;
        }
        if (self->field_64 && self->field_70 > -0x3800 && flag != 2) {
            self->field_70 = speed;
        }

        self->field_78 += self->field_70;
        self->angle = (self->field_74 + self->field_78) >> 16;

        if (self->angle < 0)
            self->angle += 0x100;
        self->angle &= 0xFF;
    }

    if (abs(self->field_78 >> 16) >= 128) {
        self->field_70 = 0;
        self->field_78 = 128 << 16;
        if (!self->field_64)
            self->field_78 = -self->field_78;

        self->angle = (self->field_74 + self->field_78) >> 16;
        if (self->angle < 0)
            self->angle += 0x100;
        self->angle &= 0xFF;
        RSDK.SetSpriteAnimation(RPlaneShifter->aniFrames, 0, &self->animator, true, 0);
        self->state = RPlaneShifter_Unknown5;
    }

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);

        if (!((1 << playerID) & self->activePlayers1) && !((1 << playerID) & self->activePlayers2)
            && Player_CheckCollisionTouch(player, self, &self->hitbox) && player->onGround && !player->isChibi) {
            self->activePlayers1 |= 1 << playerID;
            player->groundVel  = 0;
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->position.x = self->position.x;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_IDLE, &player->animator, false, 0);
            player->state           = Player_State_None;
            player->nextGroundState = 0;
            player->nextAirState    = 0;
            player->onGround        = true;
        }

        if ((1 << playerID) & self->activePlayers1) {
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_TWISTER, &player->animator, false, 0);

            int32 rot = (self->field_78 >> 16) & 0xFF;
            int32 val = 0xFF - rot;
            if (player->direction)
                val = rot;

            int32 frame = 0;
            switch (val >> 6) {
                case 0:
                case 1: frame = 9 * val / 128; break;
                case 2:
                case 3: frame = 16 * (val - 128) / 128 + 9; break;
                default: break;
            }
            player->animator.frameID        = frame % 24;
            player->animator.speed = 0;
        }

        if (Player_CheckCollisionTouch(player, self, &self->hitbox))
            self->activePlayers2 |= (1 << playerID);
        else
            self->activePlayers2 &= ~(1 << playerID);
    }
}

void RPlaneShifter_Unknown5(void)
{
    RSDK_THIS(RPlaneShifter);
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);

        if ((1 << playerID) & self->activePlayers1) {
            self->activePlayers1 &= ~(1 << playerID);
            RPlaneShifter_HandlePlaneShift(player);
        }

        if (Player_CheckCollisionTouch(player, self, &self->hitbox))
            self->activePlayers2 |= (1 << playerID);
        else
            self->activePlayers2 &= ~(1 << playerID);

        if (!player->sidekick && !((1 << playerID) & self->activePlayers1) && !((1 << playerID) & self->activePlayers2)) {
            self->activePlayers1 = 0;
            self->field_78       = 0;
            self->field_74       = 0;
            self->field_64       = self->field_64 == 0;
            self->state          = RPlaneShifter_Unknown3;
        }
    }
}

void RPlaneShifter_EditorDraw(void) { RPlaneShifter_DrawSprites(); }

void RPlaneShifter_EditorLoad(void) { RPlaneShifter->aniFrames = RSDK.LoadSpriteAnimation("MMZ/RPlaneShifter.bin", SCOPE_STAGE); }

void RPlaneShifter_Serialize(void)
{
    RSDK_EDITABLE_VAR(RPlaneShifter, VAR_UINT32, height);
    RSDK_EDITABLE_VAR(RPlaneShifter, VAR_UINT8, flags);
}
