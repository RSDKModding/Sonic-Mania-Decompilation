#include "../SonicMania.hpp"

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
        RSDK.DrawAniTile(ERZSetup->skySheet, 520, ERZSetup->aniTileTimer & 0x1FF, (ERZSetup->aniTileTimer >> 2) & 0xFF80, 128, 128);
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
        RSDK.ResetEntity(p, 0, NULL);
    }
    Player->playerCount = 1;
}

void ERZSetup_ScanlineCallback(ScanlineInfo* scanlines) {
    RSDK.SetClipBounds(0, 0, 0, RSDK_screens[RSDK_sceneInfo->currentScreenID].width, 136);
    int a             = 0x1000000;
    int b             = 0x1000000;
    int curScreenID   = RSDK_sceneInfo->currentScreenID;
    int posX          = RSDK_screens[curScreenID].position.x << 14;
    int screenCenterX = RSDK_screens[curScreenID].centerX;
    int sineAngle     = RSDK.Sin256(0);
    int cnt           = 160;
    int cosineAngle   = RSDK.Cos256(0);
    int timer         = ERZSetup->timer;
    for (int i = 160; i > 16; --i) {
        int v6                = a / (8 * i);
        int v7                = sineAngle * v6;
        int v8                = cosineAngle * v6;
        scanlines->deform.y   = v7 >> 7;
        scanlines->deform.x   = -v8 >> 7;
        scanlines->position.x = v7 - screenCenterX * (-v8 >> 7) - posX;
        a                     = b - 0xC000;
        scanlines->position.y = timer + 2 * v8 - screenCenterX * (v7 >> 7);
        b -= 0xC000;
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

