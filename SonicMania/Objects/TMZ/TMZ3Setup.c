// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TMZ3Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTMZ3Setup *TMZ3Setup;

void TMZ3Setup_Update(void) {}

void TMZ3Setup_LateUpdate(void) {}

void TMZ3Setup_StaticUpdate(void)
{
    if (!(Zone->timer & 3)) {
        TMZ3Setup->bannerAniFrame += 32;
        TMZ3Setup->bannerAniFrame = TMZ3Setup->bannerAniFrame & 0xFF;

        RSDK.DrawAniTiles(TMZ3Setup->aniTiles, 34, TMZ3Setup->bannerAniFrame, 0, 32, 112);
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

    TMZ3Setup->palDuration = RSDK.Rand(2, 60);
    TMZ3Setup->palTimer    = 192;

    ++Zone->objectDrawGroup[0];

    Animals->animalTypes[0] = ANIMAL_TOCKY;
    Animals->animalTypes[1] = ANIMAL_PICKY;

    if (globals->suppressTitlecard >= true) {
        SaveGame_LoadPlayerState();
        Zone_StartFadeIn(10, 0xF0F0F0);
    }

    RSDK.SetDrawGroupProperties(0, false, TMZ3Setup_DrawHook_ApplyDynTilesPalette);
    RSDK.SetDrawGroupProperties(1, false, TMZ3Setup_DrawHook_RemoveDynTilesPalette);
}

void TMZ3Setup_DrawHook_ApplyDynTilesPalette(void) { RSDK.SetActivePalette(1, 0, ScreenInfo->size.y); }
void TMZ3Setup_DrawHook_RemoveDynTilesPalette(void) { RSDK.SetActivePalette(0, 0, ScreenInfo->size.y); }

#if GAME_INCLUDE_EDITOR
void TMZ3Setup_EditorDraw(void) {}

void TMZ3Setup_EditorLoad(void) {}
#endif

void TMZ3Setup_Serialize(void) {}
