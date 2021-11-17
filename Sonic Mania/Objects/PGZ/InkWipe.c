#include "SonicMania.h"

ObjectInkWipe *InkWipe;

void InkWipe_Update(void)
{
    RSDK_THIS(InkWipe);
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
    RSDK_THIS(InkWipe);

    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void InkWipe_Create(void* data)
{
    RSDK_THIS(InkWipe);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(InkWipe->aniFrames, 0, &entity->animator, true, 0);
    }
}

void InkWipe_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        InkWipe->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/InkWipe.bin", SCOPE_STAGE);
}

#if RETRO_INCLUDE_EDITOR
void InkWipe_EditorDraw(void)
{

}

void InkWipe_EditorLoad(void)
{

}
#endif

void InkWipe_Serialize(void)
{

}

