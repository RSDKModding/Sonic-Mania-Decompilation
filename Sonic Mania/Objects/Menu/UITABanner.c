#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectUITABanner *UITABanner;

void UITABanner_Update(void)
{
    RSDK_THIS(UITABanner);
    entity->active = ACTIVE_NORMAL;
    if (++entity->timer >= 192)
        entity->timer -= 192;
}

void UITABanner_LateUpdate(void) {}

void UITABanner_StaticUpdate(void) {}

void UITABanner_Draw(void)
{
    RSDK_THIS(UITABanner);
    EntityUIControl *control = entity->parent;
    if (control) {
        if (control->active == ACTIVE_ALWAYS) {
            int32 zone = entity->zoneID % 0xC;
            if (entity->isEncore)
                RSDK.CopyPalette((zone >> 3) + 4, (32 * zone), 0, 224, 32);
            else
                RSDK.CopyPalette((zone >> 3) + 1, (32 * zone), 0, 224, 32);
            RSDK.SetSpriteAnimation(UITABanner->aniFrames, 11, &entity->animator1, true, zone);
        }
        UITABanner_Unknown2(entity->position.x, entity->position.y, entity->isEncore);
        UITABanner_Unknown5(entity->actID, entity->zoneID % 0xC, entity->characterID, entity->isEncore, entity->position.x, entity->position.y);
    }
}

void UITABanner_Create(void *data)
{
    RSDK_THIS(UITABanner);
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = 2;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x300000;
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetText(&entity->text, "", 0);
        RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &entity->labelAnimator, true, 0);
    }
    RSDK.SetSpriteAnimation(UITABanner->aniFrames, 10, &entity->animator2, false, 0);
}

void UITABanner_StageLoad(void) { UITABanner->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UITABanner_Unknown1(uint8 characterID, EntityUITABanner *banner, uint8 zoneID, uint8 actID, bool32 isEncore)
{
    RSDK.SetText(&banner->text, "", 0);
    Localization_GetZoneName(&banner->text, zoneID);
    if (zoneID == 5 || zoneID == 10)
        RSDK.SetSpriteString(UIWidgets->uiSpriteIndex, 5, &banner->text);
    else
        RSDK.SetSpriteString(UIWidgets->uiSpriteIndex, 3, &banner->text);
    banner->actID       = actID;
    banner->characterID = characterID;
    banner->zoneID      = zoneID;
    banner->isEncore    = isEncore;
}

void UITABanner_Unknown2(int32 drawX, int32 drawY, bool32 isEncore)
{
    if (!RSDK_sceneInfo->inEditor)
        RSDK.DrawRect(drawX - 0x990000, drawY - 0x1E8000, 0x1320000, 0x3D0000, 0xFFFFFF, 127, INK_BLEND, false);

    UIWidgets_Unknown5(88, -58, 112, 224, drawX + 0x790000, drawY + 0x1D8000);
    RSDK.DrawRect(drawX + 0x790000, (drawY + 0x1D8000) - 0x3D0000, 0x200000, 0x3D0000, 0x5870E0, 255, INK_NONE, false);
    RSDK.DrawRect(drawX - 0x990000, drawY - 0x1E8000, 0x1320000, 0x220000, 0, 255, INK_NONE, false);

    uint32 colour = 0;
    if (isEncore)
        colour = 0xF26C4F;
    else
        colour = 0x5FA0B0;
    UIWidgets_Unknown5((colour >> 16) & 0xFF, -58, (colour >> 8) & 0xFF, colour & 0xFF, drawX + 0x990000, drawY + 0x1D8000);

    if (!RSDK_sceneInfo->inEditor)
        UIWidgets_Unknown3(67, 312, drawX + 0x30000, drawY + 0x30000);
    UIWidgets_Unknown2(67, 312, drawX, drawY);
}

void UITABanner_Unknown3(uint8 actID, uint8 characterID, bool32 isEncore, int32 drawX, int32 drawY)
{
    RSDK_THIS(UITABanner);
    Vector2 drawPos;

    colour colours[3];
    if (!isEncore)
        colours[0] = 0x5870E0;
    else
        colours[0] = 0xF26C4F;
    colours[1] = 0xF0D808;
    colours[2] = 0x10101;

    int32 widths[3];
    widths[0] = 60;
    widths[1] = 52;
    widths[2] = 70;
    drawPos.x = drawX - 0x390000;
    drawPos.y = drawY + 0x118000;

    Vector2 drawOffsets[3];
    for (int32 i = 0; i < 3; ++i) {
        RSDK.DrawRect(drawPos.x, drawPos.y, widths[i] << 16, 0xD0000, colours[i], 255, INK_NONE, false);

        drawOffsets[i].x = drawPos.x + 0x10000;
        drawOffsets[i].y = drawPos.y + 0xC0000;
        drawPos.x += (widths[i] << 16);
        if (i < 2) {
            UIWidgets_Unknown5((colours[i] >> 16) & 0xFF, 13, (colours[i] >> 8) & 0xFF, colours[i] & 0xFF, drawPos.x, drawPos.y);

            drawPos.x += 0xE0000;
            UIWidgets_Unknown5((colours[i + 1] >> 16) & 0xFF, -13, (colours[i + 1] >> 8) & 0xFF, colours[i + 1] & 0xFF, drawPos.x,
                               drawPos.y + 0xC0000);
        }
    }

    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 16, &entity->animator3, true, isEncore + 15);
    drawPos.x = drawOffsets[0].x + 0x1E0000;
    drawPos.y = drawOffsets[0].y - 0x70000;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 16, &entity->animator3, true, actID + 5);
    drawPos.x = drawOffsets[1].x + 0x1A0000;
    drawPos.y = drawOffsets[1].y - 0x70000;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 8, &entity->animator3, true, characterID - 1);
    drawPos.x = drawOffsets[2].x + 0x230000;
    drawPos.y = drawOffsets[2].y - 0x60000;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);
}

void UITABanner_Unknown4(int32 drawX, int32 drawY, int32 zoneID)
{
    RSDK_THIS(UITABanner);
    Vector2 drawPos;
    EntityUIControl *control = entity->parent;

    drawPos.y = drawY;
    drawPos.x = drawX - 0x690000;
    if (!RSDK_sceneInfo->inEditor && (control->active == ACTIVE_ALWAYS || control->dialogHasFocus == 1 || control->popoverHasFocus == 1)) {
        int32 clipY  = (drawY >> 16) - RSDK_screens->position.y;
        int32 clipX  = ((drawX - 0x690000) >> 16) - RSDK_screens->position.x;
        int32 clipX1 = RSDK_screens->clipBound_X1;
        int32 clipY1 = RSDK_screens->clipBound_Y1;
        int32 clipX2 = RSDK_screens->clipBound_X2;
        int32 clipY2 = RSDK_screens->clipBound_Y2;
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, clipX - 48, clipY - 27, clipX + 48, clipY + 27);

        SpriteFrame *frame = RSDK.GetFrame(UITABanner->aniFrames, 11, zoneID);
        frame->pivotX      = -45;
        frame->width       = 90;
        frame->sprX        = entity->timer;
        if (entity->timer <= 102) {
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);
        }
        else {
            int32 width    = entity->timer - 102;
            frame->width = 90 - width;
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);

            frame->pivotX += frame->width;
            frame->sprX  = 0;
            frame->width = width;
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);
        }
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, clipX1, clipY1, clipX2, clipY2);
    }
    else {
        entity->direction = entity->startDir;
        entity->drawFX    = FX_FLIP;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        entity->direction = FLIP_NONE;
        entity->drawFX    = FX_NONE;
    }
    UIWidgets_Unknown2(61, 96, drawPos.x, drawPos.y);
}

void UITABanner_Unknown5(uint8 actID, uint8 zoneID, uint8 characterID, bool32 isEncore, int32 drawX, int32 drawY)
{
    RSDK_THIS(UITABanner);
    Vector2 drawPos;
    UITABanner_Unknown3(actID, characterID, isEncore, drawX, drawY);
    UITABanner_Unknown4(drawX, drawY, zoneID);
    drawPos.y = drawY - 0x100000;
    drawPos.x = drawX - 0x390000;
    if (!RSDK_sceneInfo->inEditor) {
        if (zoneID == 5 || zoneID == 10) {
            RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 5, &entity->animator5, true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 3, &entity->animator5, true, 0);
        }
        RSDK.DrawText(&entity->animator5, &drawPos, &entity->text, 0, entity->text.textLength, ALIGN_LEFT, 0, 2, 0, false);
    }
}

void UITABanner_EditorDraw(void) {}

void UITABanner_EditorLoad(void) {}

void UITABanner_Serialize(void) {}
#endif
