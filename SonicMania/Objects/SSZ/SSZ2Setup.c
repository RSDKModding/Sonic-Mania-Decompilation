// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SSZ2Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSSZ2Setup *SSZ2Setup;

void SSZ2Setup_Update(void) {}

void SSZ2Setup_LateUpdate(void) {}

void SSZ2Setup_StaticUpdate(void)
{
    if (SSZ2Setup->sparkTimer <= 0) {
        foreach_active(Player, player)
        {
            Hitbox *hitbox = Player_GetHitbox(player);

            uint16 tile = RSDK.GetTile(Zone->fgLayer[0], player->position.x >> 20, ((hitbox->bottom << 16) + player->position.y - 0x10000) >> 20);
            if (tile == (uint16)-1)
                tile = RSDK.GetTile(Zone->fgLayer[0], player->position.x >> 20, ((hitbox->bottom << 16) + player->position.y - 0x10000) >> 20);

            if (RSDK.GetTileFlags(tile, player->collisionPlane) != SSZ2_TFLAGS_NORMAL && (abs(player->groundVel) > 0x80000 && player->onGround)) {
                int32 x              = player->position.x;
                int32 y              = player->position.y + (hitbox->bottom << 16);
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x, y);

                RSDK.SetSpriteAnimation(SparkRail->aniFrames, 0, &debris->animator, true, 0);
                debris->drawFX     = FX_FLIP;
                debris->direction  = player->velocity.x < 0;
                debris->drawGroup  = Zone->objectDrawGroup[0];
                debris->position.x = player->position.x;
                debris->position.y = player->position.y + (hitbox->bottom << 16);
                debris->timer      = 30;

                RSDK.PlaySfx(SSZ2Setup->sfxSpark, false, 255);
                SSZ2Setup->sparkTimer = 3;
            }
        }
    }
    else {
        SSZ2Setup->sparkTimer--;
    }
}

void SSZ2Setup_Draw(void) {}

void SSZ2Setup_Create(void *data) {}

void SSZ2Setup_StageLoad(void)
{
    SSZ2Setup->sfxSpark = RSDK.GetSfx("SSZ2/Spark.wav");

    Animals->animalTypes[0] = ANIMAL_FLICKY;
    Animals->animalTypes[1] = ANIMAL_PICKY;

    SSZ2Setup->towerID = RSDK.GetTileLayerID("Tower");
    if (SSZ2Setup->towerID < LAYER_COUNT) {
        SSZ2Setup->towerLayer                   = RSDK.GetTileLayer(SSZ2Setup->towerID);
        SSZ2Setup->towerLayer->scanlineCallback = SSZ2Setup_Scanline_BGTower;

        RSDK.SetDrawGroupProperties(1, false, SSZ2Setup_DrawHook_PrepareDrawingFX);

        RSDK.SetLimitedFade(3, 0, 4, 96, 0, 256);
        if (globals->suppressTitlecard >= true) {
            SaveGame_LoadPlayerState();
            Zone_StartFadeIn(10, 0x000000);
        }

        CREATE_ENTITY(SSZ3Cutscene, INT_TO_VOID(false), 0, 0);

#if MANIA_USE_PLUS
        Zone->stageFinishCallback = SSZ2Setup_StageFinish_EndAct2;
#endif
    }

#if MANIA_USE_PLUS
    if ((SceneInfo->filter & FILTER_ENCORE))
        RSDK.LoadPalette(0, "EncoreSSZ2.act", 0b0000000011111111);
#endif

    GenericTrigger->callbacks[GENERICTRIGGER_SSZ2_DESTROYHOTARUMKII] = SSZ2Setup_Trigger_DestroyHotaruMKII;
    GenericTrigger->callbacks[GENERICTRIGGER_SSZ2_ACHIEVEMENT]       = SSZ2Setup_Trigger_AwardAchievement;
    GenericTrigger->callbacks[GENERICTRIGGER_SSZ2_ACTTRANSITION]     = SSZ2Setup_Trigger_SSZ2BTransition;
}

#if MANIA_USE_PLUS
void SSZ2Setup_StageFinish_EndAct2(void) { CREATE_ENTITY(SSZ3Cutscene, INT_TO_VOID(true), 0, 0); }
#endif

void SSZ2Setup_DrawHook_PrepareDrawingFX(void)
{
    RSDK.SetActivePalette(0, 0, ScreenInfo->size.y);

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->size.x, ScreenInfo->size.y);
}

void SSZ2Setup_Scanline_BGTower(ScanlineInfo *scanlines)
{
    RSDK.SetClipBounds(0, ScreenInfo->center.x - 144, 0, ScreenInfo->center.x + 144, ScreenInfo->size.y);
    RSDK.ProcessParallax(SSZ2Setup->towerLayer);
    RSDK.SetActivePalette(3, 0, ScreenInfo->size.y);

    ScanlineInfo *scanlinePtr = &scanlines[ScreenInfo->center.x - 64];
    int32 x1                  = scanlinePtr->position.x;
    int32 offset              = 0x10000;
    for (int32 i = 2; i - 2 < 80;) {
        scanlinePtr -= 5;
        scanlinePtr[5].position.x = x1 & 0x1FFFFFF;

        int32 x2                  = x1 - offset;
        offset                    = (i - 2) * (i - 2) + offset;
        scanlinePtr[4].position.x = x2 & 0x1FFFFFF;

        int32 x3                  = x2 - offset;
        offset                    = (i - 1) * (i - 1) + offset;
        scanlinePtr[3].position.x = x3 & 0x1FFFFFF;

        int32 x4                  = x3 - offset;
        offset                    = i * i + offset;
        scanlinePtr[2].position.x = x4 & 0x1FFFFFF;

        int32 x5                  = x4 - offset;
        offset                    = (i + 1) * (i + 1) + offset;
        scanlinePtr[1].position.x = x5 & 0x1FFFFFF;

        x1     = x5 - offset;
        offset = (i + 2) * (i + 2) + offset;

        i += 5;
    }

    scanlinePtr = &scanlines[ScreenInfo->center.x + 64];
    x1          = scanlinePtr->position.x;
    offset      = 0x10000;

    for (int32 i = 2; i - 2 < 80;) {
        scanlinePtr += 5;
        scanlinePtr[-5].position.x = x1 & 0x1FFFFFF;

        int32 x2                   = x1 + offset;
        offset                     = (i - 2) * (i - 2) + offset;
        scanlinePtr[-4].position.x = x2 & 0x1FFFFFF;

        int32 x3                   = x2 + offset;
        offset                     = (i - 1) * (i - 1) + offset;
        scanlinePtr[-3].position.x = x3 & 0x1FFFFFF;

        int32 x4                   = x3 + offset;
        offset                     = i * i + offset;
        scanlinePtr[-2].position.x = x4 & 0x1FFFFFF;

        int32 x5                   = x4 + offset;
        offset                     = (i + 1) * (i + 1) + offset;
        scanlinePtr[-1].position.x = x5 & 0x1FFFFFF;

        x1     = x5 + offset;
        offset = (i + 2) * (i + 2) + offset;

        i += 5;
    }
}

void SSZ2Setup_Trigger_DestroyHotaruMKII(void)
{
    foreach_active(HotaruMKII, hotaru)
    {
        if (!hotaru->type) {
            BadnikHelpers_BadnikBreak(hotaru, true, true);
        }
    }
}

void SSZ2Setup_Trigger_AwardAchievement(void)
{
    if (!SSZ2Setup->hasAchievement) {
        if (!SceneInfo->minutes) {
            API_UnlockAchievement(&achievementList[ACH_SSZ]);
            SSZ2Setup->hasAchievement = true;
        }
    }
}

void SSZ2Setup_Trigger_SSZ2BTransition(void)
{
    RSDK_THIS(GenericTrigger);

    if (isMainGameMode()) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player1->stateInput) {
            player1->stateInput = StateMachine_None;
            player1->left       = false;
            player1->right      = true;

            Zone->cameraBoundsR[0] = ScreenInfo->center.x + (self->position.x >> 16);
            Zone->cameraBoundsR[1] = ScreenInfo->center.x + (self->position.x >> 16);
#if MANIA_USE_PLUS
            Zone->cameraBoundsR[2] = ScreenInfo->center.x + (self->position.x >> 16);
            Zone->cameraBoundsR[3] = ScreenInfo->center.x + (self->position.x >> 16);
#endif

            for (int32 p = 0; p < Player->playerCount; ++p) StarPost->postIDs[p] = 0;

            SaveGame_SavePlayerState();
            globals->suppressAutoMusic = true;
            globals->suppressTitlecard = true;

            ++SceneInfo->listPos;
            if (!RSDK.CheckValidScene())
                RSDK.SetScene("Presentation", "Title Screen");
            Zone_StartFadeOut(10, 0x000000);
        }

        if (player1->superState == SUPERSTATE_SUPER || player1->state == Player_State_Transform)
            globals->restartPowerups |= 0x80;

#if GAME_VERSION != VER_100
        globals->restartMusicID = Music->activeTrack;
#endif
    }
}

#if RETRO_INCLUDE_EDITOR
void SSZ2Setup_EditorDraw(void) {}

void SSZ2Setup_EditorLoad(void)
{

    RSDK_ACTIVE_VAR(GenericTrigger, triggerID);
    RSDK_ENUM_VAR("Destroy Active HotaruMKIIs", GENERICTRIGGER_SSZ2_DESTROYHOTARUMKII);
    RSDK_ENUM_VAR("Handle SSZ2 Achievement", GENERICTRIGGER_SSZ2_ACHIEVEMENT);
    RSDK_ENUM_VAR("Act Transition", GENERICTRIGGER_SSZ2_ACTTRANSITION);
}
#endif

void SSZ2Setup_Serialize(void) {}
