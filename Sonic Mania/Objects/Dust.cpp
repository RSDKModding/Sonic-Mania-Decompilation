#include "../SonicMania.hpp"

ObjectDust *Dust;

void Dust_Update()
{

}

void Dust_LateUpdate()
{
    EntityDust *entity = (EntityDust *)RSDK_sceneInfo->entity;
    if (entity->state)
        entity->state();
}

void Dust_StaticUpdate()
{

}

void Dust_Draw()
{
    EntityDust *entity = (EntityDust *)RSDK_sceneInfo->entity;
    RSDK.DrawSprite(&entity->data, NULL, false);
}

void Dust_Create(void* data)
{
    EntityDust *entity = (EntityDust *)RSDK_sceneInfo->entity;
    RSDK.SetSpriteAnimation(Dust->spriteIndex, 0, &entity->data, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_NORMAL;
        entity->visible   = 1;
        entity->drawFX    = FX_FLIP | FX_ROTATE;
        entity->drawOrder = Zone->drawOrderHigh;
        entity->parent    = (Entity *)data;
    }
}

void Dust_StageLoad() { Dust->spriteIndex = RSDK.LoadAnimation("Global/Dust.bin", SCOPE_STAGE); }

void Dust_State_DropDash()
{
    EntityDust *entity = (EntityDust *)RSDK_sceneInfo->entity;
    entity->position.x += RSDK_sceneInfo->entity->velocity.x;
    entity->position.y += entity->velocity.y;
    RSDK.ProcessAnimation(&entity->data);
    if (entity->data.frameID == entity->data.frameCount - 1)
        RSDK.DestroyEntity(entity, 0, 0);
}
void Dust_State_HammerDrop()
{
    EntityDust *entity = (EntityDust *)RSDK_sceneInfo->entity;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    RSDK.ObjectTileGrip(entity, entity->collisionLayers, entity->collisionMode, entity->collisionPlane, 0, 0, 8);
    RSDK.ProcessAnimation(&entity->data);
    if (entity->data.frameID == entity->data.frameCount - 1)
        RSDK.DestroyEntity(entity, 0, 0);
}
void Dust_State_GlideSlide()
{
    EntityDust *entity   = (EntityDust *)RSDK_sceneInfo->entity;
    EntityPlayer *player = (EntityPlayer *)entity->parent;
    if (!player) {
        RSDK.DestroyEntity(entity, 0, 0);
        return;
    }
    else {
        entity->visible = false;
        if (entity->timer == 0 && player->onGround) {
            Hitbox *playerHitbox = Player_GetHitbox(player);
            EntityDust *dust     = (EntityDust *)RSDK.SpawnEntity(Dust->objectID, entity, player->position.x, player->position.y - 0x40000);
            dust->state          = Dust_State_DropDash;
            dust->position.y += playerHitbox->bottom << 16;
            dust->drawOrder = player->drawOrder;
        }
        entity->timer = ((byte)entity->timer + 1) & 7;
        if (player->playerAnimData.animationID != ANI_FLYLIFTTIRED || !player->groundVel)
            RSDK.DestroyEntity(entity, 0, 0);
    }
}
void Dust_State_Skid()
{
    EntityDust *entity   = (EntityDust *)RSDK_sceneInfo->entity;
    EntityPlayer *player = (EntityPlayer *)entity->parent;
    if (!player) {
        RSDK.DestroyEntity(entity, 0, 0);
        return;
    }
    else {
        entity->visible = 0;
        if (entity->timer == 0 && player->onGround) {
            Hitbox *playerHitbox = Player_GetHitbox(player);
            EntityDust *dust     = (EntityDust *)RSDK.SpawnEntity(Dust->objectID, entity, player->position.x, player->position.y);
            dust->state          = Dust_State_DropDash;
            dust->position.y += playerHitbox->bottom << 16;
            dust->drawOrder = player->drawOrder;
        }
        entity->timer = ((byte)entity->timer + 1) & 7;
        if (player->playerAnimData.animationID != ANI_SKID)
            RSDK.DestroyEntity(entity, 0, 0);
    }
}
void Dust_State_Spindash()
{
    EntityDust *entity   = (EntityDust *)RSDK_sceneInfo->entity;
    EntityPlayer *player = (EntityPlayer *)entity->parent;
    if (!player) {
        RSDK.DestroyEntity(entity, 0, 0);
    }
    else {
        Hitbox *playerHitbox    = Player_GetHitbox(player);
        entity->position.x = player->position.x;
        entity->position.y = player->position.y;
        int bottom                      = playerHitbox->bottom << 16;
        if (player->flipY)
            entity->position.y -= bottom;
        else
            entity->position.y += bottom;
        entity->direction = player->direction;
        entity->drawOrder = player->drawOrder;
        entity->rotation  = player->rotation;
        RSDK.ProcessAnimation(&entity->data);
        if (player->state != Player_State_Spindash)
            RSDK.DestroyEntity(entity, 0, 0);
    }
}
void Dust_State_EggLoco()
{
    EntityDust *entity = (EntityDust *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->data);
    entity->velocity.x -= 0x2000;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->data.frameID == entity->data.frameCount - 1)
        RSDK.DestroyEntity(entity, 0, 0);
}

void Dust_EditorDraw()
{

}

void Dust_EditorLoad()
{

}

void Dust_Serialize()
{

}

