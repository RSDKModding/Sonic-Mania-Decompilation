// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TitleBG Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTitleBG *TitleBG;

void TitleBG_Update(void)
{
    RSDK_THIS(TitleBG);

    if (self->type == TITLEBG_WINGSHINE) {
        self->position.y += 0x10000;

        ++self->timer;
        if (self->timer == 32) {
            self->timer = 0;
            self->position.y -= 0x200000;
        }
    }
    else {
        self->position.x -= 0x10000;
        if (self->position.x < -0x800000)
            self->position.x += 0x3000000;
    }
}

void TitleBG_LateUpdate(void) {}

void TitleBG_StaticUpdate(void)
{
    TitleBG->timer += 0x8000;
    TitleBG->timer &= 0x7FFFFFFF;

    ++TitleBG->angle;
    TitleBG->angle &= 0x3FF;

    if (++TitleBG->palTimer == 6) {
        TitleBG->palTimer = 0;
        RSDK.RotatePalette(0, 140, 143, false);
    }
}

void TitleBG_Draw(void)
{
    RSDK_THIS(TitleBG);

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->size.x, ScreenInfo->size.y);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void TitleBG_Create(void *data)
{
    RSDK_THIS(TitleBG);

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(TitleBG->aniFrames, self->type, &self->animator, true, 0);

        self->active    = ACTIVE_NORMAL;
        self->visible   = false;
        self->drawGroup = 1;
        self->alpha     = 0xFF;
        self->drawFX    = FX_FLIP;

        switch (self->type) {
            case TITLEBG_MOUNTAIN2: self->inkEffect = INK_BLEND; break;

            case TITLEBG_REFLECTION:
            case TITLEBG_WATERSPARKLE:
                self->inkEffect = INK_ADD;
                self->alpha     = 0x80;
                break;

            case TITLEBG_WINGSHINE:
                self->drawGroup = 4;
                self->inkEffect = INK_MASKED;
                break;
            default: break;
        }
    }
}

void TitleBG_StageLoad(void)
{
    TitleBG->aniFrames = RSDK.LoadSpriteAnimation("Title/Background.bin", SCOPE_STAGE);

    RSDK.SetPaletteEntry(0, 55, 0x202030);
}

void TitleBG_SetupFX(void)
{
    RSDK.GetTileLayer(0)->drawGroup[0] = DRAWGROUP_COUNT;
    RSDK.GetTileLayer(1)->drawGroup[0] = 0;

    TileLayer *cloudLayer        = RSDK.GetTileLayer(2);
    cloudLayer->drawGroup[0]     = 0;
    cloudLayer->scanlineCallback = TitleBG_Scanline_Clouds;

    TileLayer *islandLayer        = RSDK.GetTileLayer(3);
    islandLayer->drawGroup[0]     = 1;
    islandLayer->scanlineCallback = TitleBG_Scanline_Island;

    foreach_all(TitleBG, titleBG) { titleBG->visible = true; }
    foreach_all(Title3DSprite, title3DSprite) { title3DSprite->visible = true; }

    RSDK.SetPaletteEntry(0, 55, 0x00FF00);
    RSDK.SetPaletteMask(0x00FF00);
    RSDK.SetDrawGroupProperties(2, true, StateMachine_None);
}

void TitleBG_Scanline_Clouds(ScanlineInfo *scanlines)
{
    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->size.x, SCREEN_YSIZE / 2);

    int32 sine   = RSDK.Sin256(0);
    int32 cosine = RSDK.Cos256(0);

    int32 off = 0x1000000;
    for (int32 i = 0xA0; i > 0x20; --i) {
        int32 id  = off / (8 * i);
        int32 sin = sine * id;
        int32 cos = cosine * id;

        scanlines->deform.x   = (-cos >> 7);
        scanlines->deform.y   = sin >> 7;
        scanlines->position.x = sin - ScreenInfo->center.x * (-cos >> 7);
        scanlines->position.y = TitleBG->timer + 2 * cos - ScreenInfo->center.x * (sin >> 7);

        off -= 0x4000;
        scanlines++;
    }
}

void TitleBG_Scanline_Island(ScanlineInfo *scanlines)
{
    RSDK.SetClipBounds(0, 0, 168, ScreenInfo->size.x, SCREEN_YSIZE);

    int32 sine   = RSDK.Sin1024(-TitleBG->angle) >> 2;
    int32 cosine = RSDK.Cos1024(-TitleBG->angle) >> 2;

    ScanlineInfo *scanlinePtr = &scanlines[168];
    for (int32 i = 16; i < 88; ++i) {
        int32 id  = 0xA00000 / (8 * i);
        int32 sin = sine * id;
        int32 cos = cosine * id;

        scanlinePtr->deform.y   = sin >> 7;
        scanlinePtr->deform.x   = -cos >> 7;
        scanlinePtr->position.y = cos - ScreenInfo->center.x * scanlinePtr->deform.y - 0xA000 * cosine + 0x2000000;
        scanlinePtr->position.x = sin - ScreenInfo->center.x * scanlinePtr->deform.x - 0xA000 * sine + 0x2000000;
        ++scanlinePtr;
    }
}

#if GAME_INCLUDE_EDITOR
void TitleBG_EditorDraw(void)
{
    RSDK_THIS(TitleBG);

    RSDK.SetSpriteAnimation(TitleBG->aniFrames, self->type, &self->animator, true, 0);
    self->alpha     = 0xFF;
    self->drawFX    = FX_FLIP;
    self->inkEffect = INK_NONE;

    switch (self->type) {
        case TITLEBG_MOUNTAIN2: self->inkEffect = INK_BLEND; break;

        case TITLEBG_REFLECTION:
        case TITLEBG_WATERSPARKLE:
            self->inkEffect = INK_ADD;
            self->alpha     = 0x80;
            break;

        case TITLEBG_WINGSHINE:
            self->drawGroup = 4;
            self->inkEffect = INK_MASKED;
            break;

        default: break;
    }

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void TitleBG_EditorLoad(void)
{
    TitleBG->aniFrames = RSDK.LoadSpriteAnimation("Title/Background.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(TitleBG, type);
    RSDK_ENUM_VAR("Mountain Top 1", TITLEBG_MOUNTAIN1);
    RSDK_ENUM_VAR("Mountain Top 2", TITLEBG_MOUNTAIN2);
    RSDK_ENUM_VAR("Reflection", TITLEBG_REFLECTION);
    RSDK_ENUM_VAR("Water Sparkle", TITLEBG_WATERSPARKLE);
    RSDK_ENUM_VAR("Wing Shine", TITLEBG_WINGSHINE);
}
#endif

void TitleBG_Serialize(void) { RSDK_EDITABLE_VAR(TitleBG, VAR_ENUM, type); }
