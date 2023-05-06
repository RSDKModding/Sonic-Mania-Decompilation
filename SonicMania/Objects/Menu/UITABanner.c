// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UITABanner Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectUITABanner *UITABanner;

void UITABanner_Update(void)
{
    RSDK_THIS(UITABanner);

    self->active = ACTIVE_NORMAL;

    if (++self->timer >= 192)
        self->timer -= 192;
}

void UITABanner_LateUpdate(void) {}

void UITABanner_StaticUpdate(void) {}

void UITABanner_Draw(void)
{
    RSDK_THIS(UITABanner);

    EntityUIControl *control = self->parent;
    if (control) {
        if (control->active == ACTIVE_ALWAYS) {
            int32 zone = self->zoneID % 12;
            if (self->isEncore)
                RSDK.CopyPalette((zone >> 3) + 4, (32 * zone), 0, 224, 32);
            else
                RSDK.CopyPalette((zone >> 3) + 1, (32 * zone), 0, 224, 32);

            RSDK.SetSpriteAnimation(UITABanner->aniFrames, 11, &self->zoneIconAnimator, true, zone);
        }

        UITABanner_DrawBGShapes(self->position.x, self->position.y, self->isEncore);
        UITABanner_DrawBannerInfo(self->position.x, self->position.y, self->zoneID % 12, self->actID, self->characterID, self->isEncore);
    }
}

void UITABanner_Create(void *data)
{
    RSDK_THIS(UITABanner);

    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = 2;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x300000;

    if (!SceneInfo->inEditor) {
        RSDK.InitString(&self->zoneName, "", 0);
        RSDK.SetSpriteAnimation(UIWidgets->fontFrames, 0, &self->labelAnimator, true, 0);
    }

    RSDK.SetSpriteAnimation(UITABanner->aniFrames, 10, &self->fuzzAnimator, false, 0);
}

void UITABanner_StageLoad(void) { UITABanner->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UITABanner_SetupDetails(EntityUITABanner *banner, uint8 zoneID, uint8 actID, uint8 characterID, bool32 isEncore)
{
    RSDK.InitString(&banner->zoneName, "", 0);
    Localization_GetZoneName(&banner->zoneName, zoneID);

    if (zoneID == 5 || zoneID == 10)
        RSDK.SetSpriteString(UIWidgets->uiFrames, 5, &banner->zoneName);
    else
        RSDK.SetSpriteString(UIWidgets->uiFrames, 3, &banner->zoneName);

    banner->actID       = actID;
    banner->characterID = characterID;
    banner->zoneID      = zoneID;
    banner->isEncore    = isEncore;
}

void UITABanner_DrawBGShapes(int32 drawX, int32 drawY, bool32 isEncore)
{
    if (!SceneInfo->inEditor)
        RSDK.DrawRect(drawX - 0x990000, drawY - 0x1E8000, 0x1320000, 0x3D0000, 0xFFFFFF, 127, INK_BLEND, false);

    UIWidgets_DrawRightTriangle(drawX + 0x790000, drawY + 0x1D8000, -58, 88, 112, 224);

    RSDK.DrawRect(drawX + 0x790000, (drawY + 0x1D8000) - 0x3D0000, 0x200000, 0x3D0000, 0x5870E0, 255, INK_NONE, false);
    RSDK.DrawRect(drawX - 0x990000, drawY - 0x1E8000, 0x1320000, 0x220000, 0, 255, INK_NONE, false);

    uint32 color = isEncore ? 0xF26C4F : 0x5FA0B0;
    UIWidgets_DrawRightTriangle(drawX + 0x990000, drawY + 0x1D8000, -58, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);

    if (!SceneInfo->inEditor)
        UIWidgets_DrawRectOutline_Blended(drawX + 0x30000, drawY + 0x30000, 312, 67);

    UIWidgets_DrawRectOutline_Black(drawX, drawY, 312, 67);
}

void UITABanner_DrawStageInfo(int32 drawX, int32 drawY, uint8 actID, uint8 characterID, bool32 isEncore)
{
    RSDK_THIS(UITABanner);

    Vector2 drawPos;

    color colors[3];
    colors[0] = isEncore ? 0xF26C4F : 0x5870E0;
    colors[1] = 0xF0D808;
    colors[2] = 0x010101;

    int32 widths[3];
    widths[0] = 60;
    widths[1] = 52;
    widths[2] = 70;
    drawPos.x = drawX - 0x390000;
    drawPos.y = drawY + 0x118000;

    Vector2 drawOffsets[3];
    for (int32 i = 0; i < 3; ++i) {
        RSDK.DrawRect(drawPos.x, drawPos.y, widths[i] << 16, 0xD0000, colors[i], 255, INK_NONE, false);

        drawOffsets[i].x = drawPos.x + 0x10000;
        drawOffsets[i].y = drawPos.y + 0xC0000;
        drawPos.x += (widths[i] << 16);

        if (i < 2) {
            UIWidgets_DrawRightTriangle(drawPos.x, drawPos.y, 13, (colors[i] >> 16) & 0xFF, (colors[i] >> 8) & 0xFF, colors[i] & 0xFF);

            drawPos.x += 0xE0000;
            UIWidgets_DrawRightTriangle(drawPos.x, drawPos.y + 0xC0000, -13, (colors[i + 1] >> 16) & 0xFF, (colors[i + 1] >> 8) & 0xFF,
                                        colors[i + 1] & 0xFF);
        }
    }

    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 16, &self->textAnimator, true, isEncore + 15);
    drawPos.x = drawOffsets[0].x + 0x1E0000;
    drawPos.y = drawOffsets[0].y - 0x70000;
    RSDK.DrawSprite(&self->textAnimator, &drawPos, false);

    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 16, &self->textAnimator, true, actID + 5);
    drawPos.x = drawOffsets[1].x + 0x1A0000;
    drawPos.y = drawOffsets[1].y - 0x70000;
    RSDK.DrawSprite(&self->textAnimator, &drawPos, false);

    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 8, &self->textAnimator, true, characterID - 1);
    drawPos.x = drawOffsets[2].x + 0x230000;
    drawPos.y = drawOffsets[2].y - 0x60000;
    RSDK.DrawSprite(&self->textAnimator, &drawPos, false);
}

void UITABanner_DrawZoneIcon(int32 drawX, int32 drawY, int32 zoneID)
{
    RSDK_THIS(UITABanner);

    Vector2 drawPos;
    EntityUIControl *control = self->parent;

    drawPos.y = drawY;
    drawPos.x = drawX - 0x690000;
    if (!SceneInfo->inEditor && (control->active == ACTIVE_ALWAYS || control->dialogHasFocus == 1 || control->popoverHasFocus == 1)) {
        int32 clipY  = (drawY >> 16) - ScreenInfo->position.y;
        int32 clipX  = ((drawX - 0x690000) >> 16) - ScreenInfo->position.x;
        int32 clipX1 = ScreenInfo->clipBound_X1;
        int32 clipY1 = ScreenInfo->clipBound_Y1;
        int32 clipX2 = ScreenInfo->clipBound_X2;
        int32 clipY2 = ScreenInfo->clipBound_Y2;
        RSDK.SetClipBounds(SceneInfo->currentScreenID, clipX - 48, clipY - 27, clipX + 48, clipY + 27);

        SpriteFrame *frame = RSDK.GetFrame(UITABanner->aniFrames, 11, zoneID);
        frame->pivotX      = -45;
        frame->width       = 90;
        frame->sprX        = self->timer;

        if (self->timer <= 102) {
            RSDK.DrawSprite(&self->zoneIconAnimator, &drawPos, false);
        }
        else {
            int32 width  = self->timer - 102;
            frame->width = 90 - width;
            RSDK.DrawSprite(&self->zoneIconAnimator, &drawPos, false);

            frame->pivotX += frame->width;
            frame->sprX  = 0;
            frame->width = width;
            RSDK.DrawSprite(&self->zoneIconAnimator, &drawPos, false);
        }

        RSDK.SetClipBounds(SceneInfo->currentScreenID, clipX1, clipY1, clipX2, clipY2);
    }
    else {
        self->direction = self->startDir;
        self->drawFX    = FX_FLIP;
        RSDK.DrawSprite(&self->fuzzAnimator, &drawPos, false);

        self->direction = FLIP_NONE;
        self->drawFX    = FX_NONE;
    }

    UIWidgets_DrawRectOutline_Black(drawPos.x, drawPos.y, 96, 61);
}

void UITABanner_DrawBannerInfo(int32 drawX, int32 drawY, uint8 zoneID, uint8 actID, uint8 characterID, bool32 isEncore)
{
    RSDK_THIS(UITABanner);

    Vector2 drawPos;
    UITABanner_DrawStageInfo(drawX, drawY, actID, characterID, isEncore);
    UITABanner_DrawZoneIcon(drawX, drawY, zoneID);

    drawPos.y = drawY - 0x100000;
    drawPos.x = drawX - 0x390000;
    if (!SceneInfo->inEditor) {
        if (zoneID == 5 || zoneID == 10)
            RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 5, &self->zoneNameAnimator, true, 0);
        else
            RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 3, &self->zoneNameAnimator, true, 0);

        RSDK.DrawText(&self->zoneNameAnimator, &drawPos, &self->zoneName, 0, self->zoneName.length, ALIGN_LEFT, 0, INT_TO_VOID(2), NULL, false);
    }
}

#if GAME_INCLUDE_EDITOR
void UITABanner_EditorDraw(void)
{
    RSDK_THIS(UITABanner);

    RSDK.SetSpriteAnimation(UITABanner->aniFrames, 11, &self->zoneIconAnimator, true, self->zoneID % 12);

    UITABanner_DrawBGShapes(self->position.x, self->position.y, self->isEncore);
    UITABanner_DrawBannerInfo(self->position.x, self->position.y, self->zoneID % 12, self->actID, self->characterID, self->isEncore);
}

void UITABanner_EditorLoad(void) { UITABanner->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }
#endif

void UITABanner_Serialize(void) {}
#endif
