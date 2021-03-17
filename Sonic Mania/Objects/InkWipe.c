#include "../SonicMania.h"

ObjectInkWipe *InkWipe;

void InkWipe_Update()
{
    EntityInkWipe *entity = (EntityInkWipe *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->data);
}

void InkWipe_LateUpdate()
{

}

void InkWipe_StaticUpdate()
{

}

void InkWipe_Draw()
{
    EntityInkWipe *entity = (EntityInkWipe *)RSDK_sceneInfo->entity;
    RSDK.DrawSprite(&entity->data, NULL, 0);
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
        RSDK.SetSpriteAnimation(InkWipe->spriteIndex, 0, &entity->data, true, 0);
    }
}

void InkWipe_StageLoad()
{
    if (RSDK.CheckStageFolder("PSZ1"))
        InkWipe->spriteIndex = RSDK.LoadSpriteAnimation("PSZ1/InkWipe.bin", SCOPE_STAGE);
}

void InkWipe_EditorDraw()
{

}

void InkWipe_EditorLoad()
{

}

void InkWipe_Serialize()
{

}

