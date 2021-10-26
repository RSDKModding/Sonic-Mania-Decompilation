#include "SonicMania.h"

ObjectExplosion *Explosion;

void Explosion_Update(void)
{
    RSDK_THIS(Explosion);
    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->animator.frameID == entity->animator.frameCount - 1)
        destroyEntity(entity);
}

void Explosion_LateUpdate(void) {}

void Explosion_StaticUpdate(void) {}

void Explosion_Draw(void)
{
    RSDK_THIS(Explosion);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Explosion_Create(void *data)
{
    RSDK_THIS(Explosion);
    entity->active  = ACTIVE_NORMAL;
    entity->visible = true;
    if (entity->planeFilter > 0 && ((uint8)entity->planeFilter - 1) & 2)
        entity->drawOrder = Zone->drawOrderHigh;
    else
        entity->drawOrder = Zone->drawOrderLow;
    RSDK.SetSpriteAnimation(Explosion->aniFrames, voidToInt(data), &entity->animator, true, 0);
}

void Explosion_StageLoad(void)
{
    Explosion->aniFrames  = RSDK.LoadSpriteAnimation("Global/Explosions.bin", SCOPE_STAGE);
    Explosion->sfxDestroy = RSDK.GetSFX("Global/Destroy.wav");
}

#if RETRO_INCLUDE_EDITOR
void Explosion_EditorDraw(void) {}

void Explosion_EditorLoad(void) {}
#endif

void Explosion_Serialize(void) {}
