// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ERZSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectERZSetup *ERZSetup;

void ERZSetup_Update(void) {}

void ERZSetup_LateUpdate(void) {}

void ERZSetup_StaticUpdate(void)
{
    ERZSetup->timer += 0x10000;
    ERZSetup->timer &= 0x7FFFFFFF;

    if (!(Zone->timer & 3)) {
        ERZSetup->aniTileTimer += 0x80;
        ERZSetup->aniTileTimer &= 0xFFF;
        RSDK.DrawAniTiles(ERZSetup->aniTiles, 520, ERZSetup->aniTileTimer & 0x1FF, (ERZSetup->aniTileTimer >> 2) & 0xFF80, 128, 128);
    }
}

void ERZSetup_Draw(void) {}

void ERZSetup_Create(void *data) {}

void ERZSetup_StageLoad(void)
{
    ERZSetup->aniTiles = RSDK.LoadSpriteSheet("Phantom/Sky.gif", SCOPE_STAGE);

    RSDK.GetTileLayer(1)->scanlineCallback = ERZSetup_Scanline_Sky;

    // Animal Types in ERZ...? why is that? no animals would exist so it'd be safe to not define types
    Animals->animalTypes[0] = ANIMAL_POCKY;
    Animals->animalTypes[1] = ANIMAL_ROCKY;

    // Remove all players except for P1
    for (int32 p = 1; p < Player->playerCount; ++p) destroyEntitySlot(p);
    Player->playerCount = 1;
}

void ERZSetup_Scanline_Sky(ScanlineInfo *scanlines)
{
    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];

    RSDK.SetClipBounds(0, 0, 0, screen->size.x, 136);

    int32 a           = 0x1000000;
    int32 sineAngle   = RSDK.Sin256(0);
    int32 cosineAngle = RSDK.Cos256(0);

    for (int32 i = 160; i > 16; --i) {
        int32 angle = a / (8 * i);
        int32 sin   = sineAngle * angle;
        int32 cos   = cosineAngle * angle;

        scanlines->deform.y   = sin >> 7;
        scanlines->deform.x   = -cos >> 7;
        scanlines->position.x = sin - screen->center.x * (-cos >> 7) - (screen->position.x << 14);
        scanlines->position.y = ERZSetup->timer + 2 * cos - screen->center.x * (sin >> 7);

        a -= 0xC000;
        scanlines++;
    }
}

#if GAME_INCLUDE_EDITOR
void ERZSetup_EditorDraw(void) {}

void ERZSetup_EditorLoad(void) {}
#endif

void ERZSetup_Serialize(void) {}
