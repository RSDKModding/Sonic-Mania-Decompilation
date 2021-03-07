#include "../SonicMania.hpp"

ObjectExplosion *Explosion;

void Explosion_Update()
{
    EntityExplosion *entity = (EntityExplosion *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->data);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->data.frameID == entity->data.frameCount - 1)
        RSDK.DestroyEntity(entity, 0, 0);
}

void Explosion_LateUpdate()
{

}

void Explosion_StaticUpdate()
{

}

void Explosion_Draw()
{
    EntityExplosion *entity = (EntityExplosion *)RSDK_sceneInfo->entity;
    RSDK.DrawSprite(&entity->data, NULL, false);
}

void Explosion_Create(void* data)
{
    EntityExplosion *entity = (EntityExplosion *)RSDK_sceneInfo->entity;
    entity->active  = ACTIVE_NORMAL;
    entity->visible = true;
    if (entity->planeFilter > 0 && ((byte)entity->planeFilter - 1) & 2)
        entity->drawOrder = Zone->drawOrderHigh;
    else
        entity->drawOrder = Zone->drawOrderLow;
    RSDK.SetSpriteAnimation(Explosion->spriteIndex, (int)(size_t)data, &entity->data, true, 0);
}

void Explosion_StageLoad()
{
    Explosion->spriteIndex = RSDK.LoadAnimation("Global/Explosions.bin", SCOPE_STAGE);
    Explosion->sfx_Destroy = RSDK.GetSFX("Global/Destroy.wav");
}

void Explosion_EditorDraw()
{

}

void Explosion_EditorLoad()
{

}

void Explosion_Serialize()
{

}

