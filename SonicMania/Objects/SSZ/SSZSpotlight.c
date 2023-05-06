// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SSZSpotlight Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSSZSpotlight *SSZSpotlight;

void SSZSpotlight_Update(void)
{
    RSDK_THIS(SSZSpotlight);

    self->angle    = (self->angle + self->speed) & 0x1FF;
    self->rotation = RSDK.Sin512(self->angle) >> 2;

    Vector2 *vertices = self->spotlightVertices;
    for (int32 i = 0; i < 8; ++i) {
        self->drawVertices[i].x = vertices[i].x;
        self->drawVertices[i].y = vertices[i].y;

        int32 distX             = (self->drawVertices[i].x - self->originPos.x) >> 4;
        int32 distY             = (self->drawVertices[i].y - self->originPos.y) >> 4;
        self->drawVertices[i].x = self->originPos.x + (distY * RSDK.Sin1024(self->rotation) >> 6) + (distX * RSDK.Cos1024(self->rotation) >> 6);
        self->drawVertices[i].y = self->originPos.y + (distY * RSDK.Cos1024(self->rotation) >> 6) - (distX * RSDK.Sin1024(self->rotation) >> 6);
    }

    if (self->flashSpeed)
        self->alpha = (RSDK.Sin256((Zone->timer + self->offset) << self->flashSpeed) >> 2) + 0xC0;
}

void SSZSpotlight_LateUpdate(void) {}

void SSZSpotlight_StaticUpdate(void) {}

void SSZSpotlight_Draw(void)
{
    RSDK_THIS(SSZSpotlight);

    Vector2 *drawVertex = self->drawVertices;
    int32 screenX       = self->position.x - (ScreenInfo[SceneInfo->currentScreenID].position.x << 16);

    for (int32 i = 0; i < 6; i += 2) {
        Vector2 vertices[4];
        color colors[4];

        vertices[0].x = screenX + drawVertex[i + 0].x;
        vertices[0].y = drawVertex[i + 0].y;
        colors[0]     = self->colorTable[i];

        vertices[1].x = screenX + drawVertex[i + 1].x;
        vertices[1].y = drawVertex[i + 1].y;
        colors[1]     = self->colorTable[i + 1];

        vertices[2].x = screenX + drawVertex[i + 3].x;
        vertices[2].y = drawVertex[i + 3].y;
        colors[2]     = self->colorTable[i + 3];

        vertices[3].x = screenX + drawVertex[i + 2].x;
        vertices[3].y = drawVertex[i + 2].y;
        colors[3]     = self->colorTable[i + 2];

        RSDK.DrawBlendedFace(vertices, colors, 4, self->alpha, INK_ADD);
    }
}

void SSZSpotlight_Create(void *data)
{
    RSDK_THIS(SSZSpotlight);
    if (!SceneInfo->inEditor) {
        self->visible = true;

        switch (self->drawFlag) {
            default: break;

            case SSZSPOTLIGHT_DRAW_LOW: self->drawGroup = Zone->objectDrawGroup[0]; break;
            case SSZSPOTLIGHT_DRAW_HIGH: self->drawGroup = Zone->objectDrawGroup[1]; break;
            case SSZSPOTLIGHT_DRAW_LOWER: self->drawGroup = Zone->objectDrawGroup[0] - 1; break;
        }

        self->angle = self->offset;

        int32 sizes[3] = { 4, 8, 16 };

        self->alpha       = 0x100;
        self->originPos.y = 272 << 16;

        self->spotlightVertices[0].x = -(sizes[self->size] << 16);
        self->spotlightVertices[0].y = -(192 << 16);

        self->spotlightVertices[1].x = -(sizes[self->size] << 16);
        self->spotlightVertices[1].y = 256 << 16;

        self->spotlightVertices[2].x = -0x4000 * sizes[self->size];
        self->spotlightVertices[2].y = -(192 << 16);

        self->spotlightVertices[3].x = -0x4000 * sizes[self->size];
        self->spotlightVertices[3].y = 256 << 16;

        self->spotlightVertices[4].x = sizes[self->size] << 14;
        self->spotlightVertices[4].y = -(192 << 16);

        self->spotlightVertices[5].x = sizes[self->size] << 14;
        self->spotlightVertices[5].y = 256 << 16;

        self->spotlightVertices[6].x = sizes[self->size] << 16;
        self->spotlightVertices[6].y = -(192 << 16);

        self->spotlightVertices[7].x = sizes[self->size] << 16;
        self->spotlightVertices[7].y = 256 << 16;

        Vector2 *vertex = self->spotlightVertices;
        for (int32 i = 0; i < 8; i += 2) {
            vertex->x = 720 * (vertex->x >> 8);

            vertex += 2;
        }

#if MANIA_USE_PLUS
        if (SceneInfo->filter & FILTER_ENCORE)
            self->colorTable = self->color != SSZSPOTLIGHT_CLR_CYAN ? SSZSpotlight->yellowSpotlightColors : SSZSpotlight->redSpotlightColors;
        else
            self->colorTable = self->color != SSZSPOTLIGHT_CLR_CYAN ? SSZSpotlight->pinkSpotlightColors : SSZSpotlight->cyanSpotlightColors;
#else
        self->colorTable = self->color != SSZSPOTLIGHT_CLR_CYAN ? SSZSpotlight->pinkSpotlightColors : SSZSpotlight->cyanSpotlightColors;
#endif

        self->active        = ACTIVE_XBOUNDS;
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x800000;
    }
}

void SSZSpotlight_StageLoad(void) {}

#if GAME_INCLUDE_EDITOR
void SSZSpotlight_EditorDraw(void)
{
    RSDK_THIS(SSZSpotlight);

    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(PlaneSwitch->aniFrames, 0, &self->animator, true, 4);

    RSDK.DrawSprite(&self->animator, NULL, false);

    RSDK_DRAWING_OVERLAY(true);
    uint32 color[] = { 0x40D080, 0xE850D8 };
    self->rotation = RSDK.Sin512(self->offset & 0x1FF) >> 2;

    uint32 sizes[] = { 0xC00, 0x1200, 0x1800 };

    Vector2 drawPos;
    drawPos.x = (sizes[self->size] * RSDK.Cos1024(self->rotation - 0x100)) + self->position.x;
    drawPos.y = (sizes[self->size] * RSDK.Sin1024(self->rotation - 0x100)) + self->position.y;

    DrawHelpers_DrawArrow(self->position.x, self->position.y, drawPos.x, drawPos.y, color[self->color], INK_NONE, 0xFF);

    RSDK_DRAWING_OVERLAY(false);
}

void SSZSpotlight_EditorLoad(void)
{
    SSZSpotlight->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(SSZSpotlight, color);
    RSDK_ENUM_VAR("Cyan", SSZSPOTLIGHT_CLR_CYAN);
    RSDK_ENUM_VAR("Pink", SSZSPOTLIGHT_CLR_PINK);

    RSDK_ACTIVE_VAR(SSZSpotlight, size);
    RSDK_ENUM_VAR("Small", SSZSPOTLIGHT_SIZE_SMALL);
    RSDK_ENUM_VAR("Medium", SSZSPOTLIGHT_SIZE_MED);
    RSDK_ENUM_VAR("Large", SSZSPOTLIGHT_SIZE_LARGE);

    RSDK_ACTIVE_VAR(SSZSpotlight, drawFlag);
    RSDK_ENUM_VAR("On Object Group (Low)", SSZSPOTLIGHT_DRAW_LOW);
    RSDK_ENUM_VAR("On Object Group (High)", SSZSPOTLIGHT_DRAW_HIGH);
    RSDK_ENUM_VAR("Behind Object Group (Low)", SSZSPOTLIGHT_DRAW_LOWER);
}
#endif

void SSZSpotlight_Serialize(void)
{
    RSDK_EDITABLE_VAR(SSZSpotlight, VAR_UINT8, color);
    RSDK_EDITABLE_VAR(SSZSpotlight, VAR_UINT8, size);
    RSDK_EDITABLE_VAR(SSZSpotlight, VAR_UINT8, drawFlag);
    RSDK_EDITABLE_VAR(SSZSpotlight, VAR_UINT8, speed);
    RSDK_EDITABLE_VAR(SSZSpotlight, VAR_UINT8, offset);
    RSDK_EDITABLE_VAR(SSZSpotlight, VAR_UINT8, flashSpeed);
}
