// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Constellation Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectConstellation *Constellation;

void Constellation_Update(void)
{
    RSDK_THIS(Constellation);

    RSDK.ProcessAnimation(&self->animator);
}

void Constellation_LateUpdate(void) {}

void Constellation_StaticUpdate(void) {}

void Constellation_Draw(void)
{
    RSDK_THIS(Constellation);

    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];
    Vector2 drawPos;

    drawPos.y = self->originPos.y - ((screen->position.y * Constellation->background1->parallaxFactor / 256) << 16);
    drawPos.x = ((((self->position.x >> 16) - screen->position.x - (screen->size.x >> 1)) << 7) / (256 - self->scrollInfo->parallaxFactor)) << 16;
    drawPos.x += screen->size.x << 15;
    RSDK.DrawSprite(&self->animator, &drawPos, true);
}

void Constellation_Create(void *data)
{
    RSDK_THIS(Constellation);

    self->originPos     = self->position;
    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = 1;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x3000000;

    if (!SceneInfo->inEditor)
        Constellation_SetupInfo();

    RSDK.SetSpriteAnimation(Constellation->aniFrames, self->shape, &self->animator, true, 0);
}

void Constellation_StageLoad(void)
{
    Constellation->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Constellation.bin", SCOPE_STAGE);

    Constellation->background1 = RSDK.GetTileLayer(RSDK.GetTileLayerID("Background 1"));
}

void Constellation_SetupInfo(void)
{
    RSDK_THIS(Constellation);

    TileLayer *background1 = RSDK.GetTileLayer(RSDK.GetTileLayerID("Background 1"));

    int32 id         = MIN(self->originPos.y >> 3, 20);
    self->scrollInfo = &background1->scrollInfo[id];

    if (self->scrollInfo) {
        int32 factor = self->scrollInfo->parallaxFactor;
        if (factor > 0)
            self->updateRange.x += ((ScreenInfo->size.x << 8) / factor) << 16;
    }
}

#if GAME_INCLUDE_EDITOR
void Constellation_EditorDraw(void)
{
    RSDK_THIS(Constellation);

    RSDK.SetSpriteAnimation(Constellation->aniFrames, self->shape, &self->animator, true, 0);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Constellation_EditorLoad(void)
{
    Constellation->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Constellation.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Constellation, shape);
    RSDK_ENUM_VAR("Sonic", CONSTELLATION_SONIC);
    RSDK_ENUM_VAR("Vase", CONSTELLATION_VASE);
    RSDK_ENUM_VAR("Berries", CONSTELLATION_BERRIES);
    RSDK_ENUM_VAR("Lyre", CONSTELLATION_LYRE);
    RSDK_ENUM_VAR("Helmet", CONSTELLATION_HELMET);

    RSDK_ACTIVE_VAR(Constellation, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void Constellation_Serialize(void)
{
    RSDK_EDITABLE_VAR(Constellation, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Constellation, VAR_UINT8, shape);
}
