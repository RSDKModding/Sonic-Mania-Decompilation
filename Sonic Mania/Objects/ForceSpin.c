#include "../SonicMania.h"

ObjectForceSpin *ForceSpin;

void ForceSpin_Update()
{
    EntityPlayer *player      = NULL;
    EntityForceSpin *entity = (EntityForceSpin *)RSDK_sceneInfo->entity;
    while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&player)) {
        int x     = (player->position.x - entity->position.x) >> 8;
        int y     = (player->position.y - entity->position.y) >> 8;
        int scanX = (y * RSDK.Sin256(entity->negAngle)) + (x * RSDK.Cos256(entity->negAngle)) + entity->position.x;
        int scanY = (y * RSDK.Cos256(entity->negAngle)) - (x * RSDK.Sin256(entity->negAngle)) + entity->position.y;
        int pos   = ((player->velocity.y >> 8) * RSDK.Sin256(entity->negAngle)) + (player->velocity.x >> 8) * RSDK.Cos256(entity->negAngle);
        RSDK.Cos256(entity->negAngle);
        RSDK.Sin256(entity->negAngle);

        int xDif = abs(scanX - entity->position.x);
        int yDif = abs(scanY - entity->position.y);

        if (xDif < 0x180000 && yDif < entity->size << 19) {
            if (scanX + pos >= entity->position.x) {
                if (entity->direction) {
                    if (player->state == Player_State_ForceRoll || player->state == Player_State_RollLock) {
                        if (player->onGround)
                            player->state = Player_State_Roll;
                        else
                            player->state = Player_State_Air;
                        player->nextGroundState = NULL;
                        player->nextAirState    = NULL;
                    }
                }
                else {
                    ForceSpin_SetPlayerState(player);
                }
            }
            else {
                if (!entity->direction) {
                    if (player->state == Player_State_ForceRoll || player->state == Player_State_RollLock) {
                        if (player->onGround)
                            player->state = Player_State_Roll;
                        else
                            player->state = Player_State_Air;
                        player->nextGroundState = NULL;
                        player->nextAirState    = NULL;
                    }
                }
                else {
                    ForceSpin_SetPlayerState(player);
                }
            }
        }
    }
    entity->visible = DebugMode->debugActive;
}

void ForceSpin_LateUpdate()
{

}

void ForceSpin_StaticUpdate()
{

}

void ForceSpin_Draw() { ForceSpin_DrawSprites(); }

void ForceSpin_Create(void* data)
{
    EntityForceSpin *entity = (EntityForceSpin *)RSDK_sceneInfo->entity;
    RSDK.SetSpriteAnimation(ForceSpin->spriteIndex, 0, &entity->data, true, 0);
    entity->drawFX |= FX_FLIP;
    entity->data.frameID = 4;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active = ACTIVE_BOUNDS;

        int x = 0;
        if (entity->size * RSDK.Sin256(entity->angle) << 11 >= 0)
            x = entity->size * RSDK.Sin256(entity->angle) << 11;
        else
            x = -0x800 * entity->size * RSDK.Sin256(entity->angle);
        entity->updateRange.x = x + 0x200000;

        int y = 0;
        if (entity->size * RSDK.Cos256(entity->angle) << 11 >= 0)
            y = entity->size * RSDK.Cos256(entity->angle) << 11;
        else
            y = -0x800 * entity->size * RSDK.Cos256(entity->angle);
        entity->visible       = false;
        entity->updateRange.y = y + 0x200000;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->negAngle      = (byte) - (byte)entity->angle;
    }
}

void ForceSpin_StageLoad() { ForceSpin->spriteIndex = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }

void ForceSpin_DrawSprites()
{
    Vector2 drawPos;

    EntityForceSpin *entity = (EntityForceSpin *)RSDK_sceneInfo->entity;
    drawPos.x = RSDK_sceneInfo->entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.y -= entity->size << 19;
    Zone_Unknown3((Entity *)entity, &drawPos, entity->angle);

    for (int i = 0; i < entity->size; ++i) {
        RSDK.DrawSprite(&entity->data, &drawPos, 0);
        drawPos.x += RSDK.Sin256(entity->angle) << 12;
        drawPos.y += RSDK.Cos256(entity->angle) << 12;
    }
}
void ForceSpin_SetPlayerState(void *plr)
{
    EntityPlayer *player    = (EntityPlayer *)plr;
    EntityForceSpin *entity = (EntityForceSpin *)RSDK_sceneInfo->entity;
    if (player->state != Player_State_ForceRoll && player->state != Player_State_RollLock) {
        if (player->playerAnimData.animationID != ANI_JUMP) {
            RSDK.PlaySFX(Player->sfx_Roll, 0, 255);
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimData, 0, 0);
            if (!player->collisionMode && player->onGround)
                player->position.y += player->cameraOffset;
            player->pushing = 0;
        }

        player->nextAirState = NULL;
        if (player->onGround)
            player->state = Player_State_ForceRoll;
        else
            player->state = Player_State_RollLock;

        player->nextGroundState = NULL;
        if (abs(player->groundVel) < 0x10000) {
            if (entity->direction & FLIP_X)
                player->groundVel = -0x40000;
            else
                player->groundVel = 0x40000;
        }
    }
}

void ForceSpin_EditorDraw()
{

}

void ForceSpin_EditorLoad()
{

}

void ForceSpin_Serialize()
{
    RSDK_EDITABLE_VAR(ForceSpin, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(ForceSpin, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(ForceSpin, VAR_INT32, angle);
}

