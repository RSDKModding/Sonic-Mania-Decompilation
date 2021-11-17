#include "SonicMania.h"

ObjectTitle3DSprite *Title3DSprite;

void Title3DSprite_Update(void)
{
    RSDK_THIS(Title3DSprite);
    int32 sinY      = (entity->position.y >> 8) * RSDK.Sin1024(TitleBG->angle);
    int32 cosY      = (entity->position.y >> 8) * RSDK.Cos1024(TitleBG->angle);
    entity->relX    = (-sinY - (entity->position.x >> 8) * RSDK.Cos1024(TitleBG->angle)) >> 10;
    entity->relY    = (cosY - (entity->position.x >> 8) * RSDK.Sin1024(TitleBG->angle)) >> 10;
    entity->depth3D = entity->relY;
}

void Title3DSprite_LateUpdate(void) {}

void Title3DSprite_StaticUpdate(void) {}

void Title3DSprite_Draw(void)
{
    RSDK_THIS(Title3DSprite);
    int32 depth = entity->depth3D + Title3DSprite->baseDepth;
    if (depth && depth >= 256) {
        int32 scale = 0x18000 * Title3DSprite->islandSize / depth;
        if (scale > 0x200)
            scale = 0x200;
        entity->scale.x = scale;
        entity->scale.y = scale;

        Vector2 drawPos;
        drawPos.x = (Title3DSprite->islandSize * entity->relX / depth + ScreenInfo->centerX) << 16;
        drawPos.y = (Title3DSprite->islandSize * Title3DSprite->height / depth + 152) << 16;
        RSDK.DrawSprite(&entity->animator, &drawPos, true);
    }
}

void Title3DSprite_Create(void *data)
{
    RSDK_THIS(Title3DSprite);
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(Title3DSprite->aniFrames, 5, &entity->animator, true, entity->frame);
        entity->position.x -= 0x2000000;
        entity->position.y -= 0x2000000;
        entity->active    = ACTIVE_NORMAL;
        entity->visible   = false;
        entity->drawOrder = 2;
        entity->drawFX    = FX_NONE;
    }
}

void Title3DSprite_StageLoad(void)
{
    Title3DSprite->aniFrames  = RSDK.LoadSpriteAnimation("Title/Background.bin", SCOPE_STAGE);
    Title3DSprite->islandSize = 0x90;
    Title3DSprite->height     = 0x2800;
    Title3DSprite->baseDepth  = 0xA000;
}

#if RETRO_INCLUDE_EDITOR
void Title3DSprite_EditorDraw(void)
{
    RSDK_THIS(Title3DSprite);
    RSDK.SetSpriteAnimation(Title3DSprite->aniFrames, 5, &entity->animator, true, entity->frame);

    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Title3DSprite_EditorLoad(void) { Title3DSprite->aniFrames = RSDK.LoadSpriteAnimation("Title/Background.bin", SCOPE_STAGE); }
#endif

void Title3DSprite_Serialize(void) { RSDK_EDITABLE_VAR(Title3DSprite, VAR_ENUM, frame); }
