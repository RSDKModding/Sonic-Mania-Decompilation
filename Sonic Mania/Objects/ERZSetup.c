#include "../SonicMania.h"

ObjectERZSetup *ERZSetup;

void ERZSetup_Update()
{

}

void ERZSetup_LateUpdate()
{

}

void ERZSetup_StaticUpdate()
{
    ERZSetup->timer += 0x10000;
    ERZSetup->timer &= 0x7FFFFFFFu;
    if (!(Zone->timer & 3)) {
        ERZSetup->aniTileTimer += 128;
        ERZSetup->aniTileTimer &= 0xFFFu;
        RSDK.DrawAniTiles(ERZSetup->skySheet, 520, ERZSetup->aniTileTimer & 0x1FF, (ERZSetup->aniTileTimer >> 2) & 0xFF80, 128, 128);
    }
}

void ERZSetup_Draw()
{

}

void ERZSetup_Create(void* data)
{

}

void ERZSetup_StageLoad()
{
    ERZSetup->skySheet        = RSDK.LoadSpriteSheet("Phantom/Sky.gif", SCOPE_STAGE);
    RSDK.GetSceneLayer(1)->scanlineCallback = ERZSetup_ScanlineCallback;
    Animals->animalType1           = 2;
    Animals->animalType2           = 6;
    for (int p = 1; p < Player->playerCount; ++p) {
        RSDK.ResetEntitySlot(p, 0, NULL);
    }
    Player->playerCount = 1;
}

void ERZSetup_ScanlineCallback(ScanlineInfo *scanlines)
{
    ScreenInfo *screen = &RSDK_screens[RSDK_sceneInfo->currentScreenID];
    RSDK.SetClipBounds(0, 0, 0, screen->width, 136);
    int a           = 0x1000000;
    int posX        = screen->position.x << 14;
    int sineAngle   = RSDK.Sin256(0);
    int cosineAngle = RSDK.Cos256(0);
    int timer       = ERZSetup->timer;
    for (int i = 160; i > 16; --i) {
        int angle             = a / (8 * i);
        int sin               = sineAngle * angle;
        int cos               = cosineAngle * angle;
        scanlines->deform.y   = sin >> 7;
        scanlines->deform.x   = -cos >> 7;
        scanlines->position.x = sin - screen->centerX * (-cos >> 7) - posX;
        scanlines->position.y = timer + 2 * cos - screen->centerX * (sin >> 7);
        a -= 0xC000;
        scanlines++;
    }
}

void ERZSetup_EditorDraw()
{

}

void ERZSetup_EditorLoad()
{

}

void ERZSetup_Serialize()
{

}

