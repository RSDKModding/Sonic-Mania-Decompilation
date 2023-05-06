// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TMZ2Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTMZ2Setup *TMZ2Setup;

void TMZ2Setup_Update(void) {}

void TMZ2Setup_LateUpdate(void) {}

void TMZ2Setup_StaticUpdate(void)
{
    if (!(Zone->timer & 1))
        RSDK.RotatePalette(0, 204, 207, false);

    if (!(Zone->timer & 7))
        RSDK.RotatePalette(0, 250, 252, true);

    RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(2 * Zone->timer) >> 1) + 0x80, 192, 197);
    RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(2 * Zone->timer + 0x80) >> 1) + 0x80, 198, 203);
    RSDK.SetLimitedFade(4, 1, 2, (RSDK.Sin256(2 * Zone->timer) >> 1) + 0x80, 192, 197);
    RSDK.SetLimitedFade(4, 1, 2, (RSDK.Sin256(2 * Zone->timer + 0x80) >> 1) + 0x80, 198, 203);

    if (TMZ2Setup->palDuration <= 0) {
        TMZ2Setup->palTimer += 32;
        TMZ2Setup->palTimer = TMZ2Setup->palTimer & 0xFF;

        RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(TMZ2Setup->palTimer) >> 1) + 0x80, 184, 190);

        if (TMZ2Setup->palTimer == 160)
            TMZ2Setup->palDuration = RSDK.Rand(2, 60);
    }
    else {
        TMZ2Setup->palDuration--;
    }
}

void TMZ2Setup_Draw(void) {}

void TMZ2Setup_Create(void *data) {}

void TMZ2Setup_StageLoad(void)
{
    TMZ2Setup->aniTiles = RSDK.LoadSpriteSheet("TMZ1/AniTiles.gif", SCOPE_STAGE);
    TMZ2Setup->dynTiles = RSDK.LoadSpriteSheet("TMZ1/DynTiles.gif", SCOPE_STAGE);

    TMZ2Setup->palDuration = RSDK.Rand(2, 60);
    TMZ2Setup->palTimer    = 192;

    if (!TMZ2Setup->paletteInit) {
#if MANIA_USE_PLUS
        if ((SceneInfo->filter & FILTER_ENCORE))
            RSDK.LoadPalette(0, "EncoreTMZ2.act", 0b0000000011111111);
#endif

        for (int32 i = 0; i < 256; ++i) RSDK.SetPaletteEntry(5, i, 0);

        RSDK.CopyPalette(0, 128, 4, 128, 128);

        TMZ2Setup->paletteInit = true;
    }

    Animals->animalTypes[0] = ANIMAL_TOCKY;
    Animals->animalTypes[1] = ANIMAL_PICKY;

    RSDK.SetDrawGroupProperties(0, false, TMZ2Setup_DrawHook_ApplyDynTilesPalette);
    RSDK.SetDrawGroupProperties(1, false, TMZ2Setup_DrawHook_RemoveDynTilesPalette);

    if (isMainGameMode()) {
        if (globals->atlEnabled && !CutsceneRules_CheckStageReload()) {
            Zone_ReloadStoredEntities(480 << 16, 7920 << 16, true);
            CREATE_ENTITY(TMZ1Outro, NULL, 0, 0);
        }
    }
}

void TMZ2Setup_DrawHook_ApplyDynTilesPalette(void) { RSDK.SetActivePalette(4, 0, ScreenInfo->size.y); }
void TMZ2Setup_DrawHook_RemoveDynTilesPalette(void) { RSDK.SetActivePalette(0, 0, ScreenInfo->size.y); }

void TMZ2Setup_DrawDynTiles_Eggman(void)
{
    RSDK.DrawAniTiles(TMZ2Setup->dynTiles, 604, 64, 0, 96, 128);
    RSDK.DrawAniTiles(TMZ2Setup->dynTiles, 983, 16, 0, 48, 16);
    RSDK.DrawAniTiles(TMZ2Setup->dynTiles, 986, 0, 16, 64, 96);
    RSDK.DrawAniTiles(TMZ2Setup->dynTiles, 1010, 16, 112, 48, 16);
}
void TMZ2Setup_DrawDynTiles_Ruby(void)
{
    RSDK.DrawAniTiles(TMZ2Setup->dynTiles, 604, 64, 128, 96, 128);
    RSDK.DrawAniTiles(TMZ2Setup->dynTiles, 983, 16, 128, 48, 16);
    RSDK.DrawAniTiles(TMZ2Setup->dynTiles, 986, 0, 144, 64, 96);
    RSDK.DrawAniTiles(TMZ2Setup->dynTiles, 1010, 16, 240, 48, 16);
}

#if GAME_INCLUDE_EDITOR
void TMZ2Setup_EditorDraw(void) {}

void TMZ2Setup_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(WarpDoor, effect);
    RSDK_ENUM_VAR("None", WARPDOOR_EFFECT_NONE);
    RSDK_ENUM_VAR("Ruby Mist", TMZ2_WARPDOOR_EFFECT_MIST);
}
#endif

void TMZ2Setup_Serialize(void) {}
