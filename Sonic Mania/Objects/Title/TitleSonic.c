#include "SonicMania.h"

ObjectTitleSonic *TitleSonic;

void TitleSonic_Update(void)
{
    RSDK_THIS(TitleSonic);
    RSDK.ProcessAnimation(&entity->data1);
    if (entity->data1.frameID == entity->data1.frameCount - 1)
        RSDK.ProcessAnimation(&entity->data2);
}

void TitleSonic_LateUpdate(void) {}

void TitleSonic_StaticUpdate(void) {}

void TitleSonic_Draw(void)
{
    RSDK_THIS(TitleSonic);
    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, 160);
    RSDK.DrawSprite(&entity->data1, 0, 0);
    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, RSDK_screens->height);
    if (entity->data1.frameID == entity->data1.frameCount - 1)
        RSDK.DrawSprite(&entity->data2, 0, false);
}

void TitleSonic_Create(void *data)
{
    RSDK_THIS(TitleSonic);
    RSDK.SetSpriteAnimation(TitleSonic->spriteIndex, 0, &entity->data1, true, 0);
    RSDK.SetSpriteAnimation(TitleSonic->spriteIndex, 1, &entity->data2, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible   = false;
        entity->active    = ACTIVE_NEVER;
        entity->drawOrder = 4;
    }
}

void TitleSonic_StageLoad(void) { TitleSonic->spriteIndex = RSDK.LoadSpriteAnimation("Title/Sonic.bin", SCOPE_STAGE); }

void TitleSonic_EditorDraw(void) {}

void TitleSonic_EditorLoad(void) {}

void TitleSonic_Serialize(void) {}
