#include "../SonicMania.h"

ObjectTitleBG *TitleBG;

void TitleBG_Update()
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

void TitleBG_LateUpdate() {}

void TitleBG_StaticUpdate()
{
    TitleBG->timer += 0x8000;
    TitleBG->timer &= 0x7FFFFFFF;
    ++TitleBG->angle;
    TitleBG->angle &= 0x3FF;
    if (++TitleBG->palTimer == 6) {
        TitleBG->palTimer = 0;
        RSDK.RotatePalette(0, 140, 143, 0);
    }
}

void TitleBG_Draw()
{
    RSDK_THIS(TitleBG);
    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, RSDK_screens->height);
    RSDK.DrawSprite(&entity->data, 0, 0);
}

void TitleBG_Create(void *data)
{
    RSDK_THIS(TitleBG);
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(TitleBG->spriteIndex, entity->type, &entity->data, true, 0);
        entity->active    = ACTIVE_NORMAL;
        entity->visible   = 0;
        entity->drawOrder = 1;
        entity->alpha     = 0xFF;
        entity->drawFX    = FX_FLIP;
        switch (entity->type - 1) {
            case 0: entity->inkEffect = INK_BLEND; break;
            case 1:
            case 2:
                entity->inkEffect = INK_ADD;
                entity->alpha     = 0x80;
                break;
            case 3:
                entity->drawOrder = 4;
                entity->inkEffect = INK_MASKED;
                break;
            default: return;
        }
    }
}

void TitleBG_StageLoad()
{
    TitleBG->spriteIndex = RSDK.LoadSpriteAnimation("Title/Background.bin", SCOPE_STAGE);
    RSDK.SetPaletteEntry(0, 55, 0x202030);
}

void TitleBG_SetupFX()
{
    RSDK.GetSceneLayer(0)->drawLayer[0] = 16;
    RSDK.GetSceneLayer(1)->drawLayer[0] = 0;

    TileLayer *bg1        = RSDK.GetSceneLayer(2);
    bg1->drawLayer[0]     = 0;
    bg1->scanlineCallback = TitleBG_ScanlineCallbackA;

    TileLayer *bg2        = RSDK.GetSceneLayer(3);
    bg2->drawLayer[0]     = 1;
    bg2->scanlineCallback = TitleBG_ScanlineCallbackB;

    Entity *entity = NULL;
    while (RSDK.GetEntities(TitleBG->objectID, &entity)) entity->visible = true;
    entity = NULL;
    while (RSDK.GetEntities(Title3DSprite->objectID, &entity)) entity->visible = true;

    RSDK.SetPaletteEntry(0, 55, 0xFF00);
    RSDK.SetPaletteMask(0xFF00u);
    RSDK.SetDrawLayerProperties(2, true, 0);
}

void TitleBG_ScanlineCallbackA(ScanlineInfo *scanlines)
{
    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, SCREEN_YSIZE / 2);
    int sine   = RSDK.Sin256(0);
    int cosine = RSDK.Cos256(0);

    int off = 0x1000000;
    for (int i = 0xA0; i > 0x20; --i) {
        int val               = off / (8 * i);
        int sVal              = sine * val;
        int cVal              = cosine * val;
        scanlines->deform.x   = (-cVal >> 7);
        scanlines->deform.y   = sVal >> 7;
        scanlines->position.x = sVal - RSDK_screens->centerX * (-cVal >> 7);
        scanlines->position.y = TitleBG->timer + 2 * cVal - RSDK_screens->centerX * (sVal >> 7);

        off -= 0x4000;
        scanlines++;
    }
}

void TitleBG_ScanlineCallbackB(ScanlineInfo *scanlines)
{
    RSDK.SetClipBounds(0, 0, 168, RSDK_screens->width, SCREEN_YSIZE);
    int sine            = RSDK.Sin1024(-TitleBG->angle) >> 2;
    int cosine          = RSDK.Cos1024(-TitleBG->angle) >> 2;
    ScanlineInfo *scnln = &scanlines[168];

    for (int i = 16; i < 88; ++i) {
        int val           = 0xA00000 / (8 * i);
        int sVal          = sine * val;
        int cVal          = cosine * val;
        scnln->deform.y   = sVal >> 7;
        scnln->deform.x   = -cVal >> 7;
        scnln->position.y = cVal - RSDK_screens->centerX * scnln->deform.y - 0xA000 * cosine + 0x2000000;
        scnln->position.x = sVal - RSDK_screens->centerX * scnln->deform.x - 0xA000 * sine + 0x2000000;
        ++scnln;
    }
}

void TitleBG_EditorDraw() {}

void TitleBG_EditorLoad() {}

void TitleBG_Serialize() { RSDK_EDITABLE_VAR(TitleBG, VAR_ENUM, type); }
