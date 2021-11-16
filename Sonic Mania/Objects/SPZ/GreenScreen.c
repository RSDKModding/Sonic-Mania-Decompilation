#include "SonicMania.h"

ObjectGreenScreen *GreenScreen;

void GreenScreen_Update(void) {}

void GreenScreen_LateUpdate(void) {}

void GreenScreen_StaticUpdate(void)
{
    int count = 0;
    foreach_active(GreenScreen, screen)
    {
        RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(screen));
        ++count;
    }
    GreenScreen->greenScreenActive = count > 0;
}

void GreenScreen_Draw(void) { GreenScreen_DrawSprites(); }

void GreenScreen_Create(void *data)
{
    RSDK_THIS(GreenScreen);

    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->field_6C      = true;
    if (!entity->paraYFactor)
        entity->paraYFactor = 32;
    GreenScreen->hitbox.left   = -104;
    GreenScreen->hitbox.top    = -64;
    GreenScreen->hitbox.right  = 104;
    GreenScreen->hitbox.bottom = 64;
}

void GreenScreen_StageLoad(void)
{
    GreenScreen->aniFrames     = RSDK.LoadSpriteAnimation("SPZ2/GreenScreen.bin", SCOPE_STAGE);
    GreenScreen->hitbox.left   = -104;
    GreenScreen->hitbox.top    = -64;
    GreenScreen->hitbox.right  = 104;
    GreenScreen->hitbox.bottom = 64;
    GreenScreen->field_C[0]    = 0;
    GreenScreen->field_C[1]    = 42;
    GreenScreen->field_C[2]    = 64;
    GreenScreen->field_C[3]    = 96;
    GreenScreen->field_C[4]    = 112;
    GreenScreen->field_20[0]   = 0x100000;
    GreenScreen->field_20[1]   = 0xB40000;
    GreenScreen->field_20[2]   = 0xBE0000;
    GreenScreen->field_20[3]   = 0xC00000;
    GreenScreen->field_20[4]   = 0xC00000;
}

void GreenScreen_Unknown1(int fieldC, uint8 id, int a3, int a4)
{
    RSDK_THIS(GreenScreen);
    Vector2 drawPos;

    int offset = fieldC * (-a3 / 128);
    while (offset >= 0) {
        offset -= GreenScreen->field_20[id];
    }

    drawPos = entity->position;
    drawPos.y += (entity->paraYFactor * (-a4 / 128));
    drawPos.x += offset + (GreenScreen->hitbox.left << 16);

    int sizwX = entity->position.x + (GreenScreen->hitbox.right << 16);
    RSDK.SetSpriteAnimation(GreenScreen->aniFrames, 1, &entity->animator, true, id);
    while (drawPos.x < sizwX) {
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        drawPos.x += GreenScreen->field_20[id];
    }
}

void GreenScreen_Unknown2(void)
{
    RSDK_THIS(GreenScreen);

    int clipX1 = 0, clipY1 = 0, clipX2 = 0, clipY2 = 0;
    if (!RSDK_sceneInfo->inEditor) {
        clipX1 = RSDK_screens->clipBound_X1;
        clipY1 = RSDK_screens->clipBound_Y1;
        clipX2 = RSDK_screens->clipBound_X2;
        clipY2 = RSDK_screens->clipBound_Y2;
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, (entity->position.x >> 16) + GreenScreen->hitbox.left - RSDK_screens->position.x,
                           (entity->position.y >> 16) + GreenScreen->hitbox.top - RSDK_screens->position.y,
                           (entity->position.x >> 16) + GreenScreen->hitbox.right - RSDK_screens->position.x,
                           (entity->position.y >> 16) + GreenScreen->hitbox.bottom - RSDK_screens->position.y);
    }

    for (int i = 0; i < 5; ++i) {
        GreenScreen_Unknown1(GreenScreen->field_C[i], i, entity->position.x - entity->startPos.x + entity->paraOffset.x,
                             entity->position.y - entity->startPos.y + entity->paraOffset.y);
    }

    if (!RSDK_sceneInfo->inEditor)
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, clipX1, clipY1, clipX2, clipY2);
}

void GreenScreen_DrawSprites(void)
{
    RSDK_THIS(GreenScreen);

    if ((RSDK_sceneInfo->currentDrawGroup != Zone->drawOrderHigh && entity->field_6C) || (RSDK_sceneInfo->inEditor && entity->paraPreview))
        GreenScreen_Unknown2();

    if (RSDK_sceneInfo->currentDrawGroup == Zone->drawOrderHigh || RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(GreenScreen->aniFrames, 0, &entity->animator, true, 0);
        RSDK.DrawSprite(&entity->animator, NULL, false);
    }
}

#if RETRO_INCLUDE_EDITOR
void GreenScreen_EditorDraw(void) { GreenScreen_DrawSprites(); }

void GreenScreen_EditorLoad(void)
{
    GreenScreen->aniFrames   = RSDK.LoadSpriteAnimation("SPZ2/GreenScreen.bin", SCOPE_STAGE);
    GreenScreen->field_C[0]  = 0;
    GreenScreen->field_C[1]  = 42;
    GreenScreen->field_C[2]  = 64;
    GreenScreen->field_C[3]  = 96;
    GreenScreen->field_C[4]  = 112;
    GreenScreen->field_20[0] = 0x100000;
    GreenScreen->field_20[1] = 0xB40000;
    GreenScreen->field_20[2] = 0xBE0000;
    GreenScreen->field_20[3] = 0xC00000;
    GreenScreen->field_20[4] = 0xC00000;
}
#endif

void GreenScreen_Serialize(void)
{
    RSDK_EDITABLE_VAR(GreenScreen, VAR_VECTOR2, paraOffset);
    RSDK_EDITABLE_VAR(GreenScreen, VAR_ENUM, paraYFactor);
    RSDK_EDITABLE_VAR(GreenScreen, VAR_BOOL, paraPreview);
}
