#include "SonicMania.h"

ObjectRPlaneShifter *RPlaneShifter;

void RPlaneShifter_Update(void)
{
    RSDK_THIS(RPlaneShifter);

    entity->prevActivePlayers1 = entity->activePlayers1;
    if (entity->animator.animationID == 1)
        RSDK.ProcessAnimation(&entity->animator);
    StateMachine_Run(entity->state);
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
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    entity->active    = ACTIVE_BOUNDS;
    entity->angle     = 0;
    if (RSDK_sceneInfo->inEditor && !entity->height)
        entity->height = 10;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(RPlaneShifter->aniFrames, 1, &entity->animator, true, 0);
    RSDK.SetSpriteAnimation(RPlaneShifter->aniFrames, 2, &RPlaneShifter->animator, true, 0);
    entity->state = RPlaneShifter_State_Setup;
}

void RPlaneShifter_StageLoad(void)
{
    RPlaneShifter->active     = ACTIVE_ALWAYS;
    RPlaneShifter->aniFrames  = RSDK.LoadSpriteAnimation("MMZ/RPlaneShifter.bin", SCOPE_STAGE);
    RPlaneShifter->sfxTicTock = RSDK.GetSFX("MMZ/TicTock.wav");
}

void RPlaneShifter_DrawSprites(void)
{
    RSDK_THIS(RPlaneShifter);
    uint8 poleAngles[3];
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (RSDK_sceneInfo->inEditor || RSDK_sceneInfo->currentDrawGroup == Zone->playerDrawHigh + 1)
        RSDK.DrawSprite(&entity->animator, &drawPos, false);

    drawPos.y -= 0x80000;
    poleAngles[0] = entity->angle + 21;
    poleAngles[1] = entity->angle + 106;
    poleAngles[2] = entity->angle - 64;
    for (int32 i = 0; i < entity->height; ++i) {
        drawPos.y -= 0x100000;
        for (int32 p = 0; p < 3; ++p) {
            drawPos.x = entity->position.x;
            if (RSDK_sceneInfo->inEditor) {
                drawPos.x += 0x1C00 * RSDK.Cos256(poleAngles[p]);
                RSDK.DrawSprite(&RPlaneShifter->animator, &drawPos, false);
            }
            else if (RSDK_sceneInfo->currentDrawGroup != Zone->playerDrawHigh + 1) {
                if ((poleAngles[p] >= 0 && poleAngles[p] < 0x80) || RSDK_sceneInfo->currentDrawGroup == Zone->playerDrawHigh + 1) {
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

    drawPos.x = entity->position.x;
    drawPos.y -= 0x80000;
    if (RSDK_sceneInfo->inEditor || RSDK_sceneInfo->currentDrawGroup == Zone->playerDrawHigh + 1)
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
}

void RPlaneShifter_HandlePlaneShift(EntityPlayer *player)
{
    RSDK_THIS(RPlaneShifter);

    player->state           = Player_State_Ground;
    player->onGround        = true;
    player->nextGroundState = StateMachine_None;
    player->nextAirState    = StateMachine_None;
    player->groundVel       = 0;
    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_IDLE, &player->playerAnimator, false, 0);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player->sidekick && (!player->sidekick || (entity->prevActivePlayers1 & 1))) {
        player->collisionPlane = player1->collisionPlane;
        player->drawOrder      = player1->drawOrder - 1;
        player->direction      = player->direction == FLIP_NONE;
    }
    else {
        if (entity->flags) {
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

    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->hitbox.left   = -2;
    entity->field_6C      = 0;
    entity->hitbox.right  = 2;
    entity->hitbox.bottom = -8;
    entity->hitbox.top    = -8 - (16 * entity->height);
    RSDK.SetSpriteAnimation(RPlaneShifter->aniFrames, 0, &entity->animator, true, 0);
    entity->state = RPlaneShifter_Unknown3;
    RPlaneShifter_Unknown3();
}

void RPlaneShifter_Unknown3(void)
{
    RSDK_THIS(RPlaneShifter);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);

        if (!((1 << playerID) & entity->activePlayers1) && !((1 << playerID) & entity->activePlayers2)) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox) && player->onGround && !player->isChibi) {
                entity->activePlayers1 |= 1 << playerID;
                entity->activePlayers2 |= 1 << playerID;

                player->groundVel  = 0;
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->position.x = entity->position.x;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_IDLE, &player->playerAnimator, false, 0);
                player->state           = Player_State_None;
                player->nextGroundState = StateMachine_None;
                player->nextAirState    = StateMachine_None;
                player->onGround        = true;
                if (!player->sidekick) {
                    entity->field_74 = entity->angle << 16;
                    entity->field_78 = 0;
                    RSDK.SetSpriteAnimation(RPlaneShifter->aniFrames, 1, &entity->animator, true, 0);
                    RSDK.PlaySfx(RPlaneShifter->sfxTicTock, false, 255);
                    entity->state = RPlaneShifter_Unknown4;
                }
                else {
                    player->abilityValues[0] = player->left;
                    player->abilityValues[1] = player->right;
                }
            }
        }

        if (player->sidekick) {
            if (entity->activePlayers1 & (1 << playerID)) {
                if (player->jumpPress) {
                    entity->activePlayers1 &= ~(1 << playerID);
                    entity->activePlayers2 &= ~(1 << playerID);
                    Player_StartJump(player);
                }
                if (!player->abilityValues[0] && player->left) {
                    entity->activePlayers1 &= ~(1 << playerID);
                    entity->activePlayers2 &= ~(1 << playerID);
                    player->state = Player_State_Ground;
                }
                if (!player->abilityValues[1] && player->right) {
                    entity->activePlayers1 &= ~(1 << playerID);
                    entity->activePlayers2 &= ~(1 << playerID);
                    player->state = Player_State_Ground;
                }
                player->abilityValues[0] = player->left;
                player->abilityValues[1] = player->right;
            }
        }

        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox))
            entity->activePlayers2 |= (1 << playerID);
        else
            entity->activePlayers2 &= ~(1 << playerID);
    }
}

void RPlaneShifter_Unknown4(void)
{
    RSDK_THIS(RPlaneShifter);

    if (abs(entity->field_78 >> 16) < 128) {
        bool32 flag = false;
        if (entity->field_64 && entity->field_78 >> 16 > -65) {
            flag = false;
        }
        else if (!entity->field_64 && entity->field_78 >> 16 < 65) {
            flag = false;
        }
        else {
            flag = true;
        }

        int32 speed = 0;
        if (!entity->field_64)
            speed = 0x3800;
        else
            speed = -0x3800;

        if (flag) {
            entity->field_70 -= speed;
            if (entity->field_64 && entity->field_70 > -0x3800) {
                entity->field_70 = speed;
                flag             = 2;
            }
            else if (!entity->field_64 && entity->field_70 < 0x3800) {
                entity->field_70 = speed;
                flag             = 2;
            }
        }
        else {
            entity->field_70 += speed;
        }
        if (entity->field_64 && entity->field_70 > -0x3800 && flag != 2) {
            entity->field_70 = speed;
        }

        entity->field_78 += entity->field_70;
        entity->angle = (entity->field_74 + entity->field_78) >> 16;

        if (entity->angle < 0)
            entity->angle += 0x100;
        entity->angle &= 0xFF;
    }

    if (abs(entity->field_78 >> 16) >= 128) {
        entity->field_70 = 0;
        entity->field_78 = 128 << 16;
        if (!entity->field_64)
            entity->field_78 = -entity->field_78;

        entity->angle = (entity->field_74 + entity->field_78) >> 16;
        if (entity < 0)
            entity->angle += 0x100;
        entity->angle &= 0xFF;
        RSDK.SetSpriteAnimation(RPlaneShifter->aniFrames, 0, &entity->animator, true, 0);
        entity->state = RPlaneShifter_Unknown5;
    }

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);

        if (!((1 << playerID) & entity->activePlayers1) && !((1 << playerID) & entity->activePlayers2)
            && Player_CheckCollisionTouch(player, entity, &entity->hitbox) && player->onGround && !player->isChibi) {
            entity->activePlayers1 |= 1 << playerID;
            player->groundVel  = 0;
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->position.x = entity->position.x;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_IDLE, &player->playerAnimator, false, 0);
            player->state           = Player_State_None;
            player->nextGroundState = 0;
            player->nextAirState    = 0;
            player->onGround        = true;
        }

        if ((1 << playerID) & entity->activePlayers1) {
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_TWISTER, &player->playerAnimator, false, 0);

            int32 rot = (entity->field_78 >> 16) & 0xFF;
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
            player->playerAnimator.frameID        = frame % 24;
            player->playerAnimator.animationSpeed = 0;
        }

        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox))
            entity->activePlayers2 |= (1 << playerID);
        else
            entity->activePlayers2 &= ~(1 << playerID);
    }
}

void RPlaneShifter_Unknown5(void)
{
    RSDK_THIS(RPlaneShifter);
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);

        if ((1 << playerID) & entity->activePlayers1) {
            entity->activePlayers1 &= ~(1 << playerID);
            RPlaneShifter_HandlePlaneShift(player);
        }

        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox))
            entity->activePlayers2 |= (1 << playerID);
        else
            entity->activePlayers2 &= ~(1 << playerID);

        if (!player->sidekick && !((1 << playerID) & entity->activePlayers1) && !((1 << playerID) & entity->activePlayers2)) {
            entity->activePlayers1 = 0;
            entity->field_78       = 0;
            entity->field_74       = 0;
            entity->field_64       = entity->field_64 == 0;
            entity->state          = RPlaneShifter_Unknown3;
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
