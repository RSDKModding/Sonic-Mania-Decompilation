// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SSZSpotlight Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectSSZSpotlight *SSZSpotlight;

void SSZSpotlight_Update(void)
{
    RSDK_THIS(SSZSpotlight);
    self->angle        = (self->angle + self->speed) & 0x1FF;
    self->rotation     = RSDK.Sin512(self->angle) >> 2;
    Vector2 *vertStore = self->vertStore;

    for (int32 i = 0; i < 8; ++i) {
        self->vertPos[i].x = vertStore[i].x;
        self->vertPos[i].y = vertStore[i].y;

        int32 xOff         = (self->vertPos[i].x - self->offsetPos.x) >> 4;
        int32 yOff         = (self->vertPos[i].y - self->offsetPos.y) >> 4;
        self->vertPos[i].x = (yOff * RSDK.Sin1024(self->rotation) >> 6) + (xOff * RSDK.Cos1024(self->rotation) >> 6) + self->offsetPos.x;
        self->vertPos[i].y = (yOff * RSDK.Cos1024(self->rotation) >> 6) - (xOff * RSDK.Sin1024(self->rotation) >> 6) + self->offsetPos.y;
    }

    if (self->flashSpeed)
        self->alpha = (RSDK.Sin256((Zone->timer + self->offset) << self->flashSpeed) >> 2) + 0xC0;
}

void SSZSpotlight_LateUpdate(void) {}

void SSZSpotlight_StaticUpdate(void) {}

void SSZSpotlight_Draw(void)
{
    RSDK_THIS(SSZSpotlight);
    Vector2 *vertPosPtr = self->vertPos;
    Vector2 vertPos[4];
    color vertClrs[4];

    int32 screenX = self->position.x - (ScreenInfo[SceneInfo->currentScreenID].position.x << 16);

    for (int32 i = 0; i < 6; i += 2) {
        vertPos[0].x = screenX + vertPosPtr[i + 0].x;
        vertPos[0].y = vertPosPtr[i + 0].y;
        vertClrs[0]  = self->vertClrPtrs[i];

        vertPos[1].x = screenX + vertPosPtr[i + 1].x;
        vertPos[1].y = vertPosPtr[i + 1].y;
        vertClrs[1]  = self->vertClrPtrs[i + 1];

        vertPos[2].x = screenX + vertPosPtr[i + 3].x;
        vertPos[2].y = vertPosPtr[i + 3].y;
        vertClrs[2]  = self->vertClrPtrs[i + 3];

        vertPos[3].x = screenX + vertPosPtr[i + 2].x;
        vertPos[3].y = vertPosPtr[i + 2].y;
        vertClrs[3]  = self->vertClrPtrs[i + 2];

        RSDK.DrawBlendedQuad(vertPos, vertClrs, 4, self->alpha, INK_ADD);
    }
}

void SSZSpotlight_Create(void *data)
{
    RSDK_THIS(SSZSpotlight);
    if (!SceneInfo->inEditor) {
        self->visible = true;

        switch (self->drawFlag) {
            default: break;
            case 0: self->drawOrder = Zone->objectDrawLow; break;
            case 1: self->drawOrder = Zone->objectDrawHigh; break;
            case 2: self->drawOrder = Zone->objectDrawLow - 1; break;
        }

        self->angle = self->offset;

        int32 sizes[3];
        sizes[0]   = 4;
        sizes[1]   = 8;
        sizes[2]   = 16;
        int32 size = -sizes[self->size];
        size <<= 16;
        self->alpha          = 0x100;
        self->offsetPos.y    = 0x1100000;
        self->vertStore[0].x = size;
        self->vertStore[1].x = size;

        self->vertStore[2].x = -0x4000 * sizes[self->size];
        self->vertStore[3].x = -0x4000 * sizes[self->size];

        self->vertStore[4].x = sizes[self->size] << 14;
        self->vertStore[5].x = sizes[self->size] << 14;

        self->vertStore[6].x = sizes[self->size] << 16;
        self->vertStore[7].x = sizes[self->size] << 16;

        Vector2 *vertPtr = self->vertStore;
        for (int32 i = 0; i < 4; ++i) {
            int32 store = vertPtr->x;
            vertPtr->x  = 720 * (store >> 8);
            vertPtr += 2;
        }

        self->vertStore[0].y = -0xC00000;
        self->vertStore[2].y = -0xC00000;
        self->vertStore[4].y = -0xC00000;
        self->vertStore[6].y = -0xC00000;
        self->vertStore[1].y = 0x1000000;
        self->vertStore[3].y = 0x1000000;
        self->vertStore[5].y = 0x1000000;
        self->vertStore[7].y = 0x1000000;
#if RETRO_USE_PLUS
        if (SceneInfo->filter & FILTER_ENCORE) {
            if (self->color)
                self->vertClrPtrs = SSZSpotlight->colorsEncoreB;
            else
                self->vertClrPtrs = SSZSpotlight->colorsEncoreA;
        }
        else {
            if (self->color)
                self->vertClrPtrs = SSZSpotlight->colorsManiaB;
            else
                self->vertClrPtrs = SSZSpotlight->colorsManiaA;
        }
#else
        if (self->color)
            self->vertClrPtrs = SSZSpotlight->colorsManiaB;
        else
            self->vertClrPtrs = SSZSpotlight->colorsManiaA;
#endif

        self->active        = ACTIVE_XBOUNDS;
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x800000;
    }
}

void SSZSpotlight_StageLoad(void) {}

#if RETRO_INCLUDE_EDITOR
void SSZSpotlight_EditorDraw(void) {}

void SSZSpotlight_EditorLoad(void) {}
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
