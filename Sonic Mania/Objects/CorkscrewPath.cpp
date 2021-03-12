#include "../SonicMania.hpp"

ObjectCorkscrewPath *CorkscrewPath;

void CorkscrewPath_Update()
{
    EntityCorkscrewPath *entity = (EntityCorkscrewPath *)RSDK_sceneInfo->entity;
    EntityPlayer *player        = NULL;
    while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&player)) {
        if (abs(entity->position.x - player->position.x) >> 16 > entity->periodShifted) {
            entity->activePlayers &= ~RSDK.GetEntityID(player);
            if (player->playerAnimData.animationID != ANI_SPRINGCS)
                player->direction &= FLIP_X;
        }
        else {
            int x     = entity->periodShifted + ((player->position.x - entity->position.x) >> 16);
            int frame = 24 * x / entity->period;
            int arc   = entity->amplitude * RSDK.Cos1024((x << 10) / entity->period);
            if (!(RSDK.GetEntityID(player) & entity->activePlayers)) {
                if (abs(arc + entity->position.y - player->position.y) >= 0x100000) {
                    entity->activePlayers &= ~RSDK.GetEntityID(player);
                }
                else {
                    if (abs(player->groundVel) <= 0x40000 || player->velocity.y <= -0x40000) {
                        entity->activePlayers &= ~RSDK.GetEntityID(player);
                    }
                    else {
                        player->position.y = arc + entity->position.y;
                        player->velocity.y = 0;
                        player->onGround   = true;
                        if (player->playerAnimData.animationID != ANI_JUMP) {
                            if (player->groundVel < 0) {
                                player->direction |= FLIP_Y;
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimData, true,
                                                        CorkscrewPath->frameTable[frame + 1]);
                            }
                            else {
                                player->direction &= FLIP_X;
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimData, true, frame);
                            }
                        }
                    }
                }
            }
            else if (abs(player->groundVel) > 0x40000 && player->groundedStore && abs(arc + player->position.y - player->position.y) < 0x100000) {
                entity->activePlayers |= RSDK.GetEntityID(player);
                player->position.y = arc + entity->position.y;
                player->velocity.y = 0;
                player->onGround   = true;
                if (player->playerAnimData.animationID != ANI_JUMP) {
                    if (player->groundVel < 0) {
                        player->direction |= FLIP_Y;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimData, true,
                                                CorkscrewPath->frameTable[frame + 1]);
                    }
                    else {
                        player->direction &= FLIP_X;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimData, true, frame);
                    }
                }
            }
        }
    }
}

void CorkscrewPath_LateUpdate()
{

}

void CorkscrewPath_StaticUpdate()
{

}

void CorkscrewPath_Draw()
{

}

void CorkscrewPath_Create(void* data)
{
    EntityCorkscrewPath *entity = (EntityCorkscrewPath *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        entity->amplitude <<= 6;
        entity->periodShifted = abs(entity->period) >> 1;
        entity->period        = abs(entity->period);
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = abs(entity->period) << 15;
        entity->updateRange.y = 4 * entity->amplitude;
    }
}

void CorkscrewPath_StageLoad()
{

}

void CorkscrewPath_EditorDraw()
{

}

void CorkscrewPath_EditorLoad()
{

}

void CorkscrewPath_Serialize()
{
    RSDK_EDITABLE_VAR(CorkscrewPath, VAR_ENUM, period);
    RSDK_EDITABLE_VAR(CorkscrewPath, VAR_ENUM, amplitude);
    RSDK_EDITABLE_VAR(CorkscrewPath, VAR_ENUM, angle);
}

