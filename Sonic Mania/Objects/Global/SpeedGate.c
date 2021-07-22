#include "SonicMania.h"

ObjectSpeedGate *SpeedGate;

void SpeedGate_Update(void)
{
    RSDK_THIS(SpeedGate);
    foreach_active(Player, player)
    {
        int vel = 0;
        if (player->onGround)
            vel = player->groundVel;
        else
            vel = player->velocity.x;
        int spd = vel >> 15;
        if (spd) {
            if (Player_CheckCollisionTouch(player, entity, &SpeedGate->hitbox)) {
                int s = spd - 2;
                if (spd >= 0)
                    s = spd + 2;
                if (abs(s) > abs(entity->field_70))
                    entity->field_70 = s;
                entity->field_74 = 0;
            }
        }
    }

    entity->angle += entity->field_70;
    if (entity->field_70 <= 0) {
        if (entity->angle <= -0x200) {
            ++entity->field_74;
            entity->angle += 512;

            entity->field_70 += 4;
            if (entity->field_70 > -2)
                entity->field_70 = -2;

            if (entity->field_70 == -2) {
                entity->field_70 = 0;
                entity->angle    = 0;
            }
        }
    }
    else {
        if (entity->angle >= 0x200) {
            ++entity->field_74;
            entity->angle -= 0x200;
            entity->field_70 -= 4;
            if (entity->field_70 < 2)
                entity->field_70 = 2;

            if (entity->field_70 == 2) {
                entity->field_70 = 0;
                entity->angle    = 0;
            }
        }
    }

    StateMachine_Run(entity->state);
}

void SpeedGate_LateUpdate(void) {}

void SpeedGate_StaticUpdate(void) {}

void SpeedGate_Draw(void) {}

void SpeedGate_Create(void *data)
{
    RSDK_THIS(SpeedGate);
    RSDK.SetSpriteAnimation(SpeedGate->spriteIndex, 0, &entity->baseData, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->spriteIndex, 1, &entity->topData, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->spriteIndex, 2, &entity->timerData, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->spriteIndex, 5, &entity->bubbleData, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->playerDrawHigh;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        entity->scale.y       = 0x200;
        if (entity->timer < 1) {
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->timerData, true, 0);
            RSDK.SetSpriteAnimation(SpeedGate->spriteIndex, 4, &entity->finsData, true, 0);
            entity->posUnknown.x = entity->position.x;
            entity->posUnknown.y = entity->position.y - 0x200000;
        }
        else {
            RSDK.SetSpriteAnimation(SpeedGate->spriteIndex, 3, &entity->finsData, true, 0);
            entity->posUnknown.x = entity->position.x;
            entity->state        = SpeedGate_State_Unknown1;
            entity->posUnknown.y = entity->position.y - 0x200000;
        }
    }
}

void SpeedGate_StageLoad(void)
{
    SpeedGate->spriteIndex   = RSDK.LoadSpriteAnimation("Global/SpeedGate.bin", SCOPE_STAGE);
    SpeedGate->sfx_StarPost  = RSDK.GetSFX("Global/StarPost.wav");
    SpeedGate->hitbox.left   = -8;
    SpeedGate->hitbox.top    = -44;
    SpeedGate->hitbox.right  = 8;
    SpeedGate->hitbox.bottom = 20;
}

void SpeedGate_State_Unknown1(void)
{
    RSDK_THIS(SpeedGate);

    int playerBit = 1;
    foreach_active(Player, player)
    {
        if (!(playerBit & entity->activePlayers)) {
            if (Player_CheckCollisionTouch(player, entity, &SpeedGate->hitbox)) {
                entity->state     = SpeedGate_State_Unknown2;
                entity->active    = ACTIVE_BOUNDS;
                entity->playerPtr = player;
                entity->field_80  = entity->position.x - player->position.x;
                entity->field_84  = entity->position.y - player->position.y;

                entity->timer2 = 0;
                if (entity->field_70 < 0)
                    entity->timer2 = 0x200;
                RSDK.PlaySFX(SpeedGate->sfx_StarPost, 0, 255);
                foreach_return;
            }
        }
        playerBit <<= 1;
    }
}

void SpeedGate_State_Unknown2(void)
{
    RSDK_THIS(SpeedGate);

    if (entity->timer2 > 0x100) {
        entity->timer2 -= 8;
    }
    else if (entity->timer2 < 0x100) {
        entity->timer2 += 8;
    }

    if (entity->timer3 < 256)
        entity->timer3 += 0x10;

    if (entity->timer <= 0) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->timerData, true, 0);
        entity->active = 4;
        entity->state  = 0;
    }
    else {
        entity->timer--;
        EntityPlayer *player = entity->playerPtr;
        if (entity->playerPtr) {
            int vel = 0x100000;
            if (entity->playerPtr->direction == FLIP_X)
                vel = -0x100000;

            entity->velocity.x += +((int)(vel - entity->field_80) >> 6) - (entity->velocity.x >> 4);
            entity->velocity.y += ((int)(-0x200000 - entity->field_84) >> 6) - (entity->velocity.y >> 4);

            entity->field_80 += entity->velocity.x;
            entity->field_84 += entity->velocity.y;
            entity->posUnknown.x = entity->field_80;
            entity->posUnknown.y = entity->field_84;
            entity->posUnknown.x += player->position.x;
            entity->posUnknown.y += player->position.y;
            EntitySpeedGate *thisEntity = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, SpeedGate);
            if (Player_CheckCollisionTouch(player, &thisEntity, &SpeedGate->hitbox)) {
                RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->timerData, true, 0);
                entity->state            = 0;
                entity->angle            = 0;
                entity->active           = ACTIVE_BOUNDS;
                thisEntity->velocity.x   = player->velocity.x;
                thisEntity->velocity.y   = -0x40000;
                thisEntity->posUnknown.x = entity->posUnknown.x;
                thisEntity->posUnknown.y = entity->posUnknown.y;
                thisEntity->timer3       = entity->timer3;
                thisEntity->active       = ACTIVE_NORMAL;
                thisEntity->state        = SpeedGate_State_Unknown3;
                RSDK.SetSpriteAnimation(SpeedGate->spriteIndex, 2, &thisEntity->timerData, true, 0);
                thisEntity->timer2 = 0;
                if (thisEntity->field_70 < 0)
                    thisEntity->timer2 = 0x200;
                RSDK.PlaySFX(SpeedGate->sfx_StarPost, 0, 255);
            }
        }
    }
}

void SpeedGate_State_Unknown3(void)
{
    RSDK_THIS(SpeedGate);
    int y = entity->position.y - 0x200000;

    entity->velocity.x += ((RSDK_sceneInfo->entity->position.x - entity->posUnknown.x) >> 5) - (entity->velocity.x >> 3);
    entity->velocity.y += ((y - entity->posUnknown.y) >> 5) - (entity->velocity.y >> 3);
    entity->posUnknown.x += entity->velocity.x;
    entity->posUnknown.y += entity->velocity.y;

    if (entity->timer2 > 0x100) {
        entity->timer2 -= 8;
    }
    else if (entity->timer2 < 0x100) {
        entity->timer2 += 8;
    }

    if (entity->timer3 < 512) {
        entity->timer3 += 16;
    }
    if (++entity->timer == 48) {
        entity->posUnknown.x = entity->position.x;
        entity->timer        = 0;
        entity->posUnknown.y = y;
        entity->active       = ACTIVE_BOUNDS;
        entity->state        = StateMachine_None;
    }
}

void SpeedGate_EditorDraw(void) {}

void SpeedGate_EditorLoad(void) {}

void SpeedGate_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpeedGate, VAR_ENUM, id);
    RSDK_EDITABLE_VAR(SpeedGate, VAR_ENUM, timer);
    RSDK_EDITABLE_VAR(SpeedGate, VAR_ENUM, prize);
    RSDK_EDITABLE_VAR(SpeedGate, VAR_UINT8, direction);
}
