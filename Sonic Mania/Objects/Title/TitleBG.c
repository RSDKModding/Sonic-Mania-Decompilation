#include "SonicMania.h"

ObjectTitleBG *TitleBG;

void TitleBG_Update(void)
{
    RSDK_THIS(TitleBG);

    if (entity->type == 4) {
        ++entity->timer;
        entity->position.y += 0x10000;
        if (entity->timer == 32) {
            entity->timer = 0;
            entity->position.y -= 0x200000;
        }
    }
    else {
        entity->position.x -= 0x10000;
        if (entity->position.x < -0x800000)
            entity->position.x += 0x3000000;
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

    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, RSDK_screens->height);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void TitleBG_Create(void *data)
{
    RSDK_THIS(TitleBG);

    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(TitleBG->aniFrames, entity->type, &entity->animator, true, 0);
        entity->active    = ACTIVE_NORMAL;
        entity->visible   = false;
        entity->drawOrder = 1;
        entity->alpha     = 0xFF;
        entity->drawFX    = FX_FLIP;
        switch (entity->type) {
            case TITLEBG_MOUNTAIN2: entity->inkEffect = INK_BLEND; break;
            case TITLEBG_REFLECTION:
            case TITLEBG_WATERSPARKLE:
                entity->inkEffect = INK_ADD;
                entity->alpha     = 0x80;
                break;
            case TITLEBG_WINGSHINE:
                entity->drawOrder = 4;
                entity->inkEffect = INK_MASKED;
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
    RSDK.GetSceneLayer(0)->drawLayer[0] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(1)->drawLayer[0] = 0;

    TileLayer *bg1        = RSDK.GetSceneLayer(2);
    bg1->drawLayer[0]     = 0;
    bg1->scanlineCallback = TitleBG_ScanlineCallback_Clouds;

    TileLayer *bg2        = RSDK.GetSceneLayer(3);
    bg2->drawLayer[0]     = 1;
    bg2->scanlineCallback = TitleBG_ScanlineCallback_Island;

    foreach_all(TitleBG, titleBG) titleBG->visible = true;
    foreach_all(Title3DSprite, title3DSprite) title3DSprite->visible = true;

    RSDK.SetPaletteEntry(0, 55, 0x00FF00);
    RSDK.SetPaletteMask(0x00FF00);
    RSDK.SetDrawLayerProperties(2, true, NULL);
}

void TitleBG_ScanlineCallback_Clouds(ScanlineInfo *scanlines)
{
    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, SCREEN_YSIZE / 2);
    int32 sine   = RSDK.Sin256(0);
    int32 cosine = RSDK.Cos256(0);

    int32 off = 0x1000000;
    for (int32 i = 0xA0; i > 0x20; --i) {
        int32 val               = off / (8 * i);
        int32 sVal              = sine * val;
        int32 cVal              = cosine * val;
        scanlines->deform.x   = (-cVal >> 7);
        scanlines->deform.y   = sVal >> 7;
        scanlines->position.x = sVal - RSDK_screens->centerX * (-cVal >> 7);
        scanlines->position.y = TitleBG->timer + 2 * cVal - RSDK_screens->centerX * (sVal >> 7);

        off -= 0x4000;
        scanlines++;
    }
}

void TitleBG_ScanlineCallback_Island(ScanlineInfo *scanlines)
{
    RSDK.SetClipBounds(0, 0, 168, RSDK_screens->width, SCREEN_YSIZE);
    int32 sine            = RSDK.Sin1024(-TitleBG->angle) >> 2;
    int32 cosine          = RSDK.Cos1024(-TitleBG->angle) >> 2;
    ScanlineInfo *scnln = &scanlines[168];

    for (int32 i = 16; i < 88; ++i) {
        int32 val           = 0xA00000 / (8 * i);
        int32 sVal          = sine * val;
        int32 cVal          = cosine * val;
        scnln->deform.y   = sVal >> 7;
        scnln->deform.x   = -cVal >> 7;
        scnln->position.y = cVal - RSDK_screens->centerX * scnln->deform.y - 0xA000 * cosine + 0x2000000;
        scnln->position.x = sVal - RSDK_screens->centerX * scnln->deform.x - 0xA000 * sine + 0x2000000;
        ++scnln;
    }
}

#if RETRO_INCLUDE_EDITOR
void TitleBG_EditorDraw(void)
{
    RSDK_THIS(TitleBG);
    RSDK.SetSpriteAnimation(TitleBG->aniFrames, entity->type, &entity->animator, true, 0);
    entity->alpha  = 0xFF;
    entity->drawFX = FX_FLIP;
    entity->inkEffect = INK_NONE;
    switch (entity->type) {
        case TITLEBG_MOUNTAIN2: entity->inkEffect = INK_BLEND; break;
        case TITLEBG_REFLECTION:
        case TITLEBG_WATERSPARKLE:
            entity->inkEffect = INK_ADD;
            entity->alpha     = 0x80;
            break;
        case TITLEBG_WINGSHINE:
            entity->drawOrder = 4;
            entity->inkEffect = INK_MASKED;
            break;
        default: break;
    }

    RSDK.DrawSprite(&entity->animator, NULL, false);
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
