// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Smog Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSmog *Smog;

void Smog_Update(void)
{
    RSDK_THIS(Smog);

    if (Smog->forceEnabled) {
        OOZSetup->smogTimer = 0;
        self->alpha         = 0x80;
        RSDK.SetLimitedFade(0, 1, 2, 224, 0, 255);
    }
    else {
        if (OOZSetup->smogTimer <= 600) {
            if (self->alpha > 0) {
                RSDK.SetLimitedFade(0, 1, 2, 0, 0, 255);
                self->alpha -= 8;
            }

            self->timer = 0;
        }
        else {
            RSDK.SetLimitedFade(0, 1, 2, (OOZSetup->smogTimer - 600) >> 2, 0, 255);

            if (self->alpha < 0x80)
                self->alpha++;
        }
    }

    if (OOZSetup->smogTimer > 1800) {
        ++self->timer;

        foreach_active(Player, player)
        {
            if (player->superState != SUPERSTATE_SUPER) {
                HUD_EnableRingFlash();

                if (self->timer == 60 && !player->sidekick && player->rings > 0)
                    Player_GiveRings(player, -1, true);
            }
        }

        if (self->timer == 60)
            self->timer = 0;
    }
}

void Smog_LateUpdate(void) {}

void Smog_StaticUpdate(void)
{
    globals->tempFlags = OOZSetup->useSmogEffect;

    if (Smog->starPostID != StarPost->postIDs[0]) {
        Smog->starPostID      = StarPost->postIDs[0];
        globals->restartFlags = OOZSetup->useSmogEffect;
    }
}

void Smog_Draw(void)
{
    RSDK_THIS(Smog);

    int32 y          = (Zone->timer + (ScreenInfo->position.y << 1)) << 14;
    uint8 scanlineID = ((ScreenInfo->position.y >> 1) + 2 * Zone->timer);

    ScanlineInfo *scanline = Smog->scanlines;
    for (int32 i = 0; i < ScreenInfo->size.y; ++i) {
        scanline->position.x = TO_FIXED(ScreenInfo->position.x) + Smog->scanlineList[scanlineID].position.x;
        scanline->position.y = y;
        scanline->deform.x   = Smog->scanlineList[scanlineID].deform.x;
        scanline->deform.y   = 0;
    
        y += Smog->scanlineList[(scanlineID + 1) & 0xFF].deform.y;
        scanline++;
        scanlineID++;
    }

    if (self->alpha >= 0x80)
        RSDK.DrawDeformedSprite(Smog->aniFrames, INK_BLEND, 0xE0);
    else
        RSDK.DrawDeformedSprite(Smog->aniFrames, INK_ALPHA, self->alpha);
}

void Smog_Create(void *data)
{
    RSDK_THIS(Smog);

    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_NORMAL;
        self->visible   = true;
        self->inkEffect = INK_ALPHA;
        self->drawGroup = Zone->hudDrawGroup - 1;

        OOZSetup->smogTimer = 0;

        if (!SceneInfo->milliseconds && !SceneInfo->seconds && !SceneInfo->minutes) {
            Smog->starPostID        = 0;
            globals->restartFlags   = 1;
            globals->tempFlags      = 1;
            OOZSetup->useSmogEffect = true;
        }
        else {
            if (isMainGameMode() && globals->enableIntro) {
                if (CutsceneRules_CheckStageReload()) {
                    if (SceneInfo->minutes == globals->tempMinutes && SceneInfo->seconds == globals->tempSeconds
                        && SceneInfo->milliseconds == globals->tempMilliseconds) {
                        OOZSetup->useSmogEffect = globals->tempFlags;
                    }
                    else {
                        OOZSetup->useSmogEffect = globals->restartFlags;
                    }

                    EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);
                    zone->fadeColor  = 0x000000;
                    zone->timer      = 0;
                    zone->visible    = true;
                    zone->drawGroup  = DRAWGROUP_COUNT - 1;
                    zone->stateDraw  = Zone_Draw_Fade;
                }
                else {
                    Smog->starPostID        = 0;
                    globals->restartFlags   = 1;
                    globals->tempFlags      = 1;
                    OOZSetup->useSmogEffect = true;
                }
            }
            else {
                if (SceneInfo->minutes == globals->tempMinutes && SceneInfo->seconds == globals->tempSeconds
                    && SceneInfo->milliseconds == globals->tempMilliseconds) {
                    OOZSetup->useSmogEffect = globals->tempFlags;
                }
                else {
                    OOZSetup->useSmogEffect = globals->restartFlags;
                }

                EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);
                zone->fadeColor  = 0x000000;
                zone->timer      = 0;
                zone->visible    = true;
                zone->drawGroup  = DRAWGROUP_COUNT - 1;
                zone->stateDraw  = Zone_Draw_Fade;
            }
        }
    }
}

void Smog_StageLoad(void)
{
    Smog->aniFrames = RSDK.LoadSpriteSheet("OOZ/Smog.gif", SCOPE_STAGE);
    Smog->scanlines = RSDK.GetScanlines();

    ScanlineInfo *scanline = (ScanlineInfo *)Smog->scanlineList;
    int32 angle            = 0;
    for (int32 i = 0; i < 0x100; ++i) {
        scanline[i].deform.x   = (RSDK.Sin256(angle >> 1) << 6) + 0x10000;
        scanline[i].deform.y   = (RSDK.Sin256(angle >> 1) << 5) + 0x10000;
        scanline[i].position.x = (RSDK.Sin256(angle) << 10) - scanline[i].deform.x * ScreenInfo->center.x;
        scanline[i].position.y = 0;

        angle += 2;
    }

    RSDK.CopyPalette(0, 0, 1, 0, 128);
    RSDK.CopyPalette(0, 0, 2, 0, 128);
    RSDK.SetLimitedFade(2, 1, 3, 64, 0, 127);
    RSDK.SetLimitedFade(0, 1, 2, 0, 0, 255);
    RSDK.SetDrawGroupProperties(Zone->hudDrawGroup - 1, false, Smog_DrawHook_ApplySmogPalette);
}

void Smog_DrawHook_ApplySmogPalette(void) { RSDK.CopyPalette(1, 0, 0, 0, 128); }

#if GAME_INCLUDE_EDITOR
void Smog_EditorDraw(void) {}

void Smog_EditorLoad(void) {}
#endif

void Smog_Serialize(void) {}
