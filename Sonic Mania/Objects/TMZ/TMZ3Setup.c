#include "SonicMania.h"

ObjectTMZ3Setup *TMZ3Setup;

void TMZ3Setup_Update(void) {}

void TMZ3Setup_LateUpdate(void) {}

void TMZ3Setup_StaticUpdate(void)
{
    if (!(Zone->timer & 3)) {
        TMZ3Setup->aniTileFrame += 32;
        TMZ3Setup->aniTileFrame = TMZ3Setup->aniTileFrame & 0xFF;
        RSDK.DrawAniTiles(TMZ3Setup->aniTiles, 34u, TMZ3Setup->aniTileFrame, 0, 32, 112);
    }

    if (!(Zone->timer & 1))
        RSDK.RotatePalette(0, 204, 207, false);
    if (!(Zone->timer & 7))
        RSDK.RotatePalette(0, 250, 252, true);

    RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(Zone->timer) >> 1) + 128, 192, 197);
    RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(Zone->timer + 128) >> 1) + 128, 198, 203);
}

void TMZ3Setup_Draw(void) {}

void TMZ3Setup_Create(void *data) {}

void TMZ3Setup_StageLoad(void)
{
    TMZ3Setup->aniTiles = RSDK.LoadSpriteSheet("TMZ1/AniTiles.gif", SCOPE_STAGE);
    TMZ3Setup->value    = RSDK.Rand(2, 60);
    TMZ3Setup->dword8   = 192;
    ++Zone->drawOrderLow;
    Animals->animalTypes[0] = ANIMAL_TOCKY;
    Animals->animalTypes[1] = ANIMAL_PICKY;
    if (globals->suppressTitlecard >= true) {
        SaveGame_LoadPlayerState();
        Zone_StartFadeOut(16, 0xF0F0F0);
    }
    RSDK.SetDrawLayerProperties(0, false, TMZ3Setup_DrawLayerCB_A);
    RSDK.SetDrawLayerProperties(1, false, TMZ3Setup_DrawLayerCB_B);
}

void TMZ3Setup_DrawLayerCB_A(void) { RSDK.SetActivePalette(1, 0, RSDK_screens->height); }
void TMZ3Setup_DrawLayerCB_B(void) { RSDK.SetActivePalette(0, 0, RSDK_screens->height); }

void TMZ3Setup_EditorDraw(void) {}

void TMZ3Setup_EditorLoad(void) {}

void TMZ3Setup_Serialize(void) {}
