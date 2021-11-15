#include "SonicMania.h"

ObjectEggJankenPart *EggJankenPart = NULL;

void EggJankenPart_Update(void)
{
    RSDK_THIS(EggJankenPart);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->rotation += entity->angle;
    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

void EggJankenPart_LateUpdate(void) {}

void EggJankenPart_StaticUpdate(void) {}

void EggJankenPart_Draw(void)
{
    RSDK_THIS(EggJankenPart);

    if (Zone->timer & 1)
        RSDK.DrawSprite(&entity->animator, NULL, false);
}

void EggJankenPart_Create(void *data)
{
    RSDK_THIS(EggJankenPart);

    RSDK.SetSpriteAnimation(EggJankenPart->aniFrames, 6, &entity->animator, true, 0);
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow + 1;
    entity->drawFX |= FX_ROTATE | FX_FLIP;
    entity->animator.frameID = voidToInt(data);
}

void EggJankenPart_StageLoad(void) { EggJankenPart->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/EggJanken.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void EggJankenPart_EditorDraw(void)
{
    RSDK_THIS(EggJankenPart);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void EggJankenPart_EditorLoad(void) { EggJankenPart->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/EggJanken.bin", SCOPE_STAGE); }
#endif

void EggJankenPart_Serialize(void) {}
