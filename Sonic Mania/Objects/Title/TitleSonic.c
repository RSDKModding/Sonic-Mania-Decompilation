#include "SonicMania.h"

ObjectTitleSonic *TitleSonic;

void TitleSonic_Update(void)
{
    RSDK_THIS(TitleSonic);
    RSDK.ProcessAnimation(&entity->animatorSonic);
    if (entity->animatorSonic.frameID == entity->animatorSonic.frameCount - 1)
        RSDK.ProcessAnimation(&entity->animatorFinger);
}

void TitleSonic_LateUpdate(void) {}

void TitleSonic_StaticUpdate(void) {}

void TitleSonic_Draw(void)
{
    RSDK_THIS(TitleSonic);
    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, 160);
    RSDK.DrawSprite(&entity->animatorSonic, NULL, false);
    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ScreenInfo->height);
    if (entity->animatorSonic.frameID == entity->animatorSonic.frameCount - 1)
        RSDK.DrawSprite(&entity->animatorFinger, NULL, false);
}

void TitleSonic_Create(void *data)
{
    RSDK_THIS(TitleSonic);
    RSDK.SetSpriteAnimation(TitleSonic->aniFrames, 0, &entity->animatorSonic, true, 0);
    RSDK.SetSpriteAnimation(TitleSonic->aniFrames, 1, &entity->animatorFinger, true, 0);
    if (!SceneInfo->inEditor) {
        entity->visible   = false;
        entity->active    = ACTIVE_NEVER;
        entity->drawOrder = 4;
    }
}

void TitleSonic_StageLoad(void) { TitleSonic->aniFrames = RSDK.LoadSpriteAnimation("Title/Sonic.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void TitleSonic_EditorDraw(void)
{
    RSDK_THIS(TitleSonic);
    entity->animatorSonic.frameID = entity->animatorSonic.frameCount - 1;

    RSDK.DrawSprite(&entity->animatorSonic, NULL, false);
    RSDK.DrawSprite(&entity->animatorFinger, NULL, false);
}

void TitleSonic_EditorLoad(void) { TitleSonic->aniFrames = RSDK.LoadSpriteAnimation("Title/Sonic.bin", SCOPE_STAGE); }
#endif

void TitleSonic_Serialize(void) {}
