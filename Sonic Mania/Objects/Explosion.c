#include "../SonicMania.h"

ObjectExplosion *Explosion;

void Explosion_Update(void)
{
    RSDK_THIS(Explosion);
    RSDK.ProcessAnimation(&entity->data);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->data.frameID == entity->data.frameCount - 1)
        destroyEntity(entity);
}

void Explosion_LateUpdate(void)
{

}

void Explosion_StaticUpdate(void)
{

}

void Explosion_Draw(void)
{
    RSDK_THIS(Explosion);
    RSDK.DrawSprite(&entity->data, NULL, false);
}

void Explosion_Create(void* data)
{
    RSDK_THIS(Explosion);
    entity->active  = ACTIVE_NORMAL;
    entity->visible = true;
    if (entity->planeFilter > 0 && ((byte)entity->planeFilter - 1) & 2)
        entity->drawOrder = Zone->drawOrderHigh;
    else
        entity->drawOrder = Zone->drawOrderLow;
    RSDK.SetSpriteAnimation(Explosion->spriteIndex, voidToInt(data), &entity->data, true, 0);
}

void Explosion_StageLoad(void)
{
    Explosion->spriteIndex = RSDK.LoadSpriteAnimation("Global/Explosions.bin", SCOPE_STAGE);
    Explosion->sfx_Destroy = RSDK.GetSFX("Global/Destroy.wav");
}

void Explosion_EditorDraw(void)
{

}

void Explosion_EditorLoad(void)
{

}

void Explosion_Serialize(void)
{

}

