#include "../SonicMania.h"

ObjectInkWipe *InkWipe;

void InkWipe_Update(void)
{
    EntityInkWipe *entity = (EntityInkWipe *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->animator);
}

void InkWipe_LateUpdate(void)
{

}

void InkWipe_StaticUpdate(void)
{

}

void InkWipe_Draw(void)
{
    EntityInkWipe *entity = (EntityInkWipe *)RSDK_sceneInfo->entity;
    RSDK.DrawSprite(&entity->animator, NULL, 0);
}

void InkWipe_Create(void* data)
{
    EntityInkWipe *entity  = (EntityInkWipe *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(InkWipe->spriteIndex, 0, &entity->animator, true, 0);
    }
}

void InkWipe_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        InkWipe->spriteIndex = RSDK.LoadSpriteAnimation("PSZ1/InkWipe.bin", SCOPE_STAGE);
}

void InkWipe_EditorDraw(void)
{

}

void InkWipe_EditorLoad(void)
{

}

void InkWipe_Serialize(void)
{

}

