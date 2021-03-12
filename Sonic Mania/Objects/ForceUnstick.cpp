#include "../SonicMania.hpp"

ObjectForceUnstick *ForceUnstick;

void ForceUnstick_Update()
{
    EntityPlayer *player = 0;
    EntityForceUnstick *entity = (EntityForceUnstick *)RSDK_sceneInfo->entity;
    while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&player)) {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
            player->collisionMode = CMODE_FLOOR;
            //if (entity->breakClimb && player->state == Player_State_KnuxWallClimb) {
            //    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FLYTIRED, &player->playerAnimData, 0, 2);
            //    player->state = Player_State_KnuxGlideDrop;
            //}
        }
    }
    entity->visible = DebugMode->active;
}

void ForceUnstick_LateUpdate()
{

}

void ForceUnstick_StaticUpdate()
{

}

void ForceUnstick_Draw() { ForceUnstick_DrawSprites(); }

void ForceUnstick_Create(void* data)
{

}

void ForceUnstick_StageLoad()
{
    ForceUnstick->spriteIndex = RSDK.LoadAnimation("Global/ItemBox.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(ForceUnstick->spriteIndex, 2, &ForceUnstick->animData, true, 6);
}

void ForceUnstick_DrawSprites()
{
    Vector2 drawPos;        
    EntityForceUnstick *entity = (EntityForceUnstick *)RSDK_sceneInfo->entity;
    drawPos.x                  = entity->position.x;
    drawPos.y                  = entity->position.y - (entity->height << 19);
    drawPos.x                  = entity->position.x - (entity->width << 19);

    if (!entity->breakClimb)
        ForceUnstick->animData.frameID = 6;
    else
        ForceUnstick->animData.frameID = 9;

    for (int y = 0; y < entity->height + 1; ++y) {
        for (int x = 0; x < entity->width + 1; ++x) {
            RSDK.DrawSprite(&ForceUnstick->animData, &drawPos, false);
            drawPos.x += 0x100000;
        }
        drawPos.x += (-0x100000 - (entity->width << 20));
        drawPos.y += 0x100000;
    }
}

void ForceUnstick_EditorDraw()
{

}

void ForceUnstick_EditorLoad()
{

}

void ForceUnstick_Serialize()
{
    RSDK_EDITABLE_VAR(ForceUnstick, VAR_UINT8, width);
    RSDK_EDITABLE_VAR(ForceUnstick, VAR_UINT8, height);
    RSDK_EDITABLE_VAR(ForceUnstick, VAR_BOOL, breakClimb);
}

