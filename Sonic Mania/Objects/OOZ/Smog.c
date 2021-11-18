#include "SonicMania.h"

ObjectSmog *Smog;

void Smog_Update(void)
{
    RSDK_THIS(Smog);
    if (Smog->field_4 == 1) {
        OOZSetup->fadeTimer = 0;
        self->alpha     = 128;
        RSDK.SetLimitedFade(0, 1, 2, 224, 0, 255);
    }
    else {
        if (OOZSetup->fadeTimer <= 600) {
            if (self->alpha > 0) {
                RSDK.SetLimitedFade(0, 1, 2, 0, 0, 255);
                self->alpha -= 8;
            }
            self->timer = 0;
        }
        else {
            RSDK.SetLimitedFade(0, 1, 2, (OOZSetup->fadeTimer - 600) >> 2, 0, 255);
            if (self->alpha < 0x80)
                self->alpha++;
        }
    }

    if (OOZSetup->fadeTimer > 1800) {
        ++self->timer;
        foreach_active(Player, player)
        {
            if (player->superState != SUPERSTATE_SUPER) {
                foreach_all(HUD, hud)
                {
                    if (hud)
                        hud->enableRingFlash = true;
                    foreach_break;
                }
                if (self->timer == 60 && !player->sidekick && player->rings > 0)
                    Player_GiveRings(-1, player, true);
            }
        }
        if (self->timer == 60)
            self->timer = 0;
    }
}

void Smog_LateUpdate(void) {}

void Smog_StaticUpdate(void)
{
    globals->tempFlags = OOZSetup->flags;
    if (Smog->starPostID != StarPost->postIDs[0]) {
        Smog->starPostID      = StarPost->postIDs[0];
        globals->restartFlags = OOZSetup->flags;
    }
}

void Smog_Draw(void)
{
    RSDK_THIS(Smog);
    ScanlineInfo *scanlinePtr = Smog->scanlines;

    int32 y       = (Zone->timer + 2 * ScreenInfo->position.y) << 14;
    uint8 defPos = ((ScreenInfo->position.y >> 1) + 2 * Zone->timer);

    ScanlineInfo *scanline = NULL;
    for (int32 i = 0; i < ScreenInfo->height; ++i) {
        scanline  = (ScanlineInfo *)&Smog->scanlineData[defPos++ * sizeof(ScanlineInfo)];
        scanlinePtr->position.x = scanline->position.x + (ScreenInfo->position.x << 16);
        scanlinePtr->position.y = y;
        scanlinePtr->deform.x   = scanline->deform.x;
        scanlinePtr->deform.y   = 0;

        ScanlineInfo *data = (ScanlineInfo *)&Smog->scanlineData[defPos * sizeof(ScanlineInfo)];
        y += data->deform.y;
        scanlinePtr++;
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
        self->drawOrder = Zone->hudDrawOrder - 1;
        OOZSetup->fadeTimer = 0;
        if (!(SceneInfo->milliseconds || SceneInfo->seconds || SceneInfo->minutes)) {
            Smog->starPostID      = 0;
            globals->restartFlags = 1;
            globals->tempFlags    = 1;
            OOZSetup->flags       = 1;
        }
        else {
            if (isMainGameMode() && globals->enableIntro) {
                if (!PlayerHelpers_CheckStageReload()) {
                    Smog->starPostID      = 0;
                    globals->restartFlags = 1;
                    globals->tempFlags    = 1;
                    OOZSetup->flags       = 1;
                    return;
                }
            }

            if (SceneInfo->minutes != globals->tempMinutes || SceneInfo->seconds != globals->tempSeconds
                || SceneInfo->milliseconds != globals->tempMilliseconds) {
                OOZSetup->flags = globals->restartFlags;
                Zone_Unknown2();
            }
            else {
                OOZSetup->flags = globals->tempFlags;
                Zone_Unknown2();
            }
        }

    }
}

void Smog_StageLoad(void)
{
    Smog->aniFrames = RSDK.LoadSpriteSheet("OOZ/Smog.gif", SCOPE_STAGE);
    Smog->scanlines = RSDK.GetScanlines();

    ScanlineInfo *scanline = (ScanlineInfo *)Smog->scanlineData;
    int32 angle              = 0;
    for (int32 i = 0; i < 0x100; ++i) {
        scanline[i].deform.x   = (RSDK.Sin256(angle >> 1) << 6) + 0x10000;
        scanline[i].deform.y   = (RSDK.Sin256(angle >> 1) << 5) + 0x10000;
        scanline[i].position.x = (RSDK.Sin256(angle) << 10) - scanline->deform.x * ScreenInfo->centerX;
        scanline[i].position.y = 0;
        angle += 2;
    }

    RSDK.CopyPalette(0, 0, 1, 0, 128);
    RSDK.CopyPalette(0, 0, 2, 0, 128);
    RSDK.SetLimitedFade(2, 1, 3, 64, 0, 127);
    RSDK.SetLimitedFade(0, 1, 2, 0, 0, 255);
    RSDK.SetDrawLayerProperties(Zone->hudDrawOrder - 1, false, Smog_DrawLayerCB);
}

void Smog_DrawLayerCB(void) { RSDK.CopyPalette(1, 0, 0, 0, 128); }

#if RETRO_INCLUDE_EDITOR
void Smog_EditorDraw(void) {}

void Smog_EditorLoad(void) {}
#endif

void Smog_Serialize(void) {}
