#include "SonicMania.h"

ObjectConstellation *Constellation;

void Constellation_Update(void)
{
    RSDK_THIS(Constellation);
    RSDK.ProcessAnimation(&entity->animator);
}

void Constellation_LateUpdate(void) {}

void Constellation_StaticUpdate(void) {}

void Constellation_Draw(void)
{
    RSDK_THIS(Constellation);
    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];
    Vector2 drawPos;

    drawPos.y = entity->startPos.y - ((screen->position.y * Constellation->bg1->parallaxFactor / 256) << 16);
    drawPos.x = ((((entity->position.x >> 16) - screen->position.x - (screen->width >> 1)) << 7) / (256 - entity->scrollInfo->parallaxFactor)) << 16;
    drawPos.x += screen->width << 15;
    RSDK.DrawSprite(&entity->animator, &drawPos, true);
}

void Constellation_Create(void *data)
{
    RSDK_THIS(Constellation);

    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = 1;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x3000000;
    if (!SceneInfo->inEditor)
        Constellation_SetupInfo();
    RSDK.SetSpriteAnimation(Constellation->aniFrames, entity->shape, &entity->animator, true, 0);
}

void Constellation_StageLoad(void)
{
    Constellation->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Constellation.bin", SCOPE_STAGE);
    Constellation->bg1       = RSDK.GetSceneLayer(RSDK.GetSceneLayerID("Background 1"));
}

void Constellation_SetupInfo(void)
{
    RSDK_THIS(Constellation);
    TileLayer *bgLayer = RSDK.GetSceneLayer(RSDK.GetSceneLayerID("Background 1"));

    int id = minVal(entity->startPos.y >> 3, 20);
    entity->scrollInfo = &bgLayer->scrollInfo[id];
    if (entity->scrollInfo) {
        int factor = entity->scrollInfo->parallaxFactor;
        if (factor > 0) {
            entity->updateRange.x += ((ScreenInfo->width << 8) / factor) << 16;
        }
    }
}

void Constellation_EditorDraw(void)
{
    RSDK_THIS(Constellation);
    RSDK.SetSpriteAnimation(Constellation->aniFrames, entity->shape, &entity->animator, true, 0);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Constellation_EditorLoad(void) { Constellation->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Constellation.bin", SCOPE_STAGE); }

void Constellation_Serialize(void)
{
    RSDK_EDITABLE_VAR(Constellation, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Constellation, VAR_UINT8, shape);
}
