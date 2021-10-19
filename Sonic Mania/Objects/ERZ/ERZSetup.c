#include "SonicMania.h"

ObjectERZSetup *ERZSetup;

void ERZSetup_Update(void) {}

void ERZSetup_LateUpdate(void) {}

void ERZSetup_StaticUpdate(void)
{
    ERZSetup->timer += 0x10000;
    ERZSetup->timer &= 0x7FFFFFFF;
    if (!(Zone->timer & 3)) {
        ERZSetup->aniTileTimer += 128;
        ERZSetup->aniTileTimer &= 0xFFF;
        RSDK.DrawAniTiles(ERZSetup->skySheet, 520, ERZSetup->aniTileTimer & 0x1FF, (ERZSetup->aniTileTimer >> 2) & 0xFF80, 128, 128);
    }
}

void ERZSetup_Draw(void) {}

void ERZSetup_Create(void *data) {}

void ERZSetup_StageLoad(void)
{
    ERZSetup->skySheet                      = RSDK.LoadSpriteSheet("Phantom/Sky.gif", SCOPE_STAGE);
    RSDK.GetSceneLayer(1)->scanlineCallback = ERZSetup_ScanlineCallback;
    Animals->animalTypes[0]                 = ANIMAL_POCKY;
    Animals->animalTypes[1]                 = ANIMAL_ROCKY;
    for (int32 p = 1; p < Player->playerCount; ++p) {
        RSDK.ResetEntitySlot(p, 0, NULL);
    }
    Player->playerCount = 1;
}

void ERZSetup_ScanlineCallback(ScanlineInfo *scanlines)
{
    ScreenInfo *screen = &RSDK_screens[RSDK_sceneInfo->currentScreenID];
    RSDK.SetClipBounds(0, 0, 0, screen->width, 136);
    int32 a           = 0x1000000;
    int32 posX        = screen->position.x << 14;
    int32 sineAngle   = RSDK.Sin256(0);
    int32 cosineAngle = RSDK.Cos256(0);
    int32 timer       = ERZSetup->timer;
    for (int32 i = 160; i > 16; --i) {
        int32 angle           = a / (8 * i);
        int32 sin             = sineAngle * angle;
        int32 cos             = cosineAngle * angle;
        scanlines->deform.y   = sin >> 7;
        scanlines->deform.x   = -cos >> 7;
        scanlines->position.x = sin - screen->centerX * (-cos >> 7) - posX;
        scanlines->position.y = timer + 2 * cos - screen->centerX * (sin >> 7);
        a -= 0xC000;
        scanlines++;
    }
}

void ERZSetup_EditorDraw(void) {}

void ERZSetup_EditorLoad(void) {}

void ERZSetup_Serialize(void) {}
