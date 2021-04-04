#include "../SonicMania.h"

ObjectDust *Dust;

void Dust_Update(void)
{

}

void Dust_LateUpdate(void)
{
    RSDK_THIS(Dust);
    if (entity->state)
        entity->state();
}

void Dust_StaticUpdate(void)
{

}

void Dust_Draw(void)
{
    RSDK_THIS(Dust);
    RSDK.DrawSprite(&entity->data, NULL, false);
}

void Dust_Create(void* data)
{
    RSDK_THIS(Dust);
    RSDK.SetSpriteAnimation(Dust->spriteIndex, 0, &entity->data, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_NORMAL;
        entity->visible   = true;
        entity->drawFX    = FX_FLIP | FX_ROTATE;
        entity->drawOrder = Zone->drawOrderHigh;
        entity->parent    = (Entity *)data;
    }
}

void Dust_StageLoad(void) { Dust->spriteIndex = RSDK.LoadSpriteAnimation("Global/Dust.bin", SCOPE_STAGE); }

void Dust_State_DropDash(void)
{
    RSDK_THIS(Dust);
    entity->position.x += RSDK_sceneInfo->entity->velocity.x;
    entity->position.y += entity->velocity.y;
    RSDK.ProcessAnimation(&entity->data);
    if (entity->data.frameID == entity->data.frameCount - 1)
        RSDK.ResetEntityPtr(entity, 0, 0);
}
void Dust_State_HammerDrop(void)
{
    RSDK_THIS(Dust);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    RSDK.ObjectTileGrip(entity, entity->collisionLayers, entity->collisionMode, entity->collisionPlane, 0, 0, 8);
    RSDK.ProcessAnimation(&entity->data);
    if (entity->data.frameID == entity->data.frameCount - 1)
        RSDK.ResetEntityPtr(entity, 0, 0);
}
void Dust_State_GlideSlide(void)
{
    RSDK_THIS(Dust);
    EntityPlayer *player = (EntityPlayer *)entity->parent;
    if (!player) {
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }
    else {
        entity->visible = false;
        if (entity->timer == 0 && player->onGround) {
            Hitbox *playerHitbox = Player_GetHitbox(player);
            EntityDust *dust     = (EntityDust *)RSDK.CreateEntity(Dust->objectID, entity, player->position.x, player->position.y - 0x40000);
            dust->state          = Dust_State_DropDash;
            dust->position.y += playerHitbox->bottom << 16;
            dust->drawOrder = player->drawOrder;
        }
        entity->timer = ((byte)entity->timer + 1) & 7;
        if (player->playerAnimData.animationID != ANI_FLYLIFTTIRED || !player->groundVel)
            RSDK.ResetEntityPtr(entity, 0, 0);
    }
}
void Dust_State_Skid(void)
{
    RSDK_THIS(Dust);
    EntityPlayer *player = (EntityPlayer *)entity->parent;
    if (!player) {
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }
    else {
        entity->visible = false;
        if (entity->timer == 0 && player->onGround) {
            Hitbox *playerHitbox = Player_GetHitbox(player);
            EntityDust *dust     = (EntityDust *)RSDK.CreateEntity(Dust->objectID, entity, player->position.x, player->position.y);
            dust->state          = Dust_State_DropDash;
            dust->position.y += playerHitbox->bottom << 16;
            dust->drawOrder = player->drawOrder;
        }
        entity->timer = ((byte)entity->timer + 1) & 7;
        if (player->playerAnimData.animationID != ANI_SKID)
            RSDK.ResetEntityPtr(entity, 0, 0);
    }
}
void Dust_State_Spindash(void)
{
    RSDK_THIS(Dust);
    EntityPlayer *player = (EntityPlayer *)entity->parent;
    if (!player) {
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }
    else {
        Hitbox *playerHitbox    = Player_GetHitbox(player);
        entity->position.x = player->position.x;
        entity->position.y = player->position.y;
        int bottom                      = playerHitbox->bottom << 16;
        if (player->invertGravity)
            entity->position.y -= bottom;
        else
            entity->position.y += bottom;
        entity->direction = player->direction;
        entity->drawOrder = player->drawOrder;
        entity->rotation  = player->rotation;
        RSDK.ProcessAnimation(&entity->data);
        if (player->state != Player_State_Spindash)
            RSDK.ResetEntityPtr(entity, 0, 0);
    }
}
void Dust_State_EggLoco(void)
{
    RSDK_THIS(Dust);
    RSDK.ProcessAnimation(&entity->data);
    entity->velocity.x -= 0x2000;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->data.frameID == entity->data.frameCount - 1)
        RSDK.ResetEntityPtr(entity, 0, 0);
}

void Dust_EditorDraw(void)
{

}

void Dust_EditorLoad(void)
{

}

void Dust_Serialize(void)
{

}

