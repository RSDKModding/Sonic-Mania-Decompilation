#include "SonicMania.h"

ObjectPSZ2Setup *PSZ2Setup;

void PSZ2Setup_Update(void) {}

void PSZ2Setup_LateUpdate(void) {}

void PSZ2Setup_StaticUpdate(void)
{
    if (--PSZ2Setup->aniTileDelay < 1) {
        ++PSZ2Setup->aniTileFrame;
        PSZ2Setup->aniTileFrame &= 7;
        PSZ2Setup->aniTileDelay = PSZ2Setup->aniTileDelays[PSZ2Setup->aniTileFrame];

        int tileY = 32 * PSZ2Setup->aniTileFrame;
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles1, 260, 0, tileY, 64, 32);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles1, 268, 80, tileY, 48, 16);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles1, 271, 64, tileY + 16, 64, 16);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles2, 159, 0, tileY, 16, 32);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles2, 276, 16, tileY, 32, 16);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles2, 281, 16, tileY + 16, 32, 16);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles2, 279, 48, tileY, 32, 16);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles2, 286, 48, tileY + 16, 32, 16);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles2, 284, 80, tileY, 32, 16);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles2, 289, 80, tileY + 16, 32, 16);
    }

    if (RSDK.Sin256(16 * Zone->timer) < 0)
        RSDK.SetLimitedFade(0, 1, 2, -RSDK.Sin256(16 * Zone->timer), 224, 227);
    else
        RSDK.SetLimitedFade(0, 1, 2, -RSDK.Sin256(16 * Zone->timer), 224, 227);

    if (PSZ2Setup->flag) {
        if (PSZ2Setup->petalTimer <= 0) {
            foreach_active(Player, player)
            {
                Hitbox *playerHitbox = Player_GetHitbox(player);
                ushort tile    = RSDK.GetTileInfo(Zone->fgLow, player->position.x >> 20, (player->position.y + (playerHitbox->bottom << 16)) >> 20);
                bool32 lowFlag = true;
                if (tile == 0xFFFF) {
                    tile    = RSDK.GetTileInfo(Zone->fgHigh, player->position.x >> 20, (player->position.y + (playerHitbox->bottom << 16)) >> 20);
                    lowFlag = false;
                }

                if (RSDK.GetTileBehaviour(tile, player->collisionPlane)) {
                    if (abs(player->groundVel) >= 0x60000 || player->state == Player_State_DropDash) {
                        EntityPetalPile *pile = (EntityPetalPile *)RSDK.CreateEntity(PetalPile->objectID, RSDK_sceneInfo->entity, player->position.x,
                                                                                     player->position.y + (playerHitbox->bottom << 16));
                        pile->leafPattern     = 4;
                        pile->tileLayer       = lowFlag;
                        pile->pileSize.x      = 0x40000;
                        pile->pileSize.y      = 0x40000;
                        pile->flag            = 1;
                        pile->field_98        = 0xB5555;
                        pile->field_94        = 2 * (player->direction != FLIP_NONE) - 1;
                        pile->field_8C        = player->groundVel >> 1;
                        PSZ2Setup->petalTimer = 3;
                    }
                }
            }
        }
        else {
            PSZ2Setup->petalTimer--;
        }
    }
}

void PSZ2Setup_Draw(void) {}

void PSZ2Setup_Create(void *data) {}

void PSZ2Setup_StageLoad(void)
{
    PSZ2Setup->flag              = false;
    GenericTrigger->callbacks[0] = PSZ2Setup_TriggerCB1;
    GenericTrigger->callbacks[1] = PSZ2Setup_TriggerCB2;
    PSZ2Setup->aniTiles1         = RSDK.LoadSpriteSheet("PSZ2/AniTiles.gif", SCOPE_STAGE);
    PSZ2Setup->aniTiles2         = RSDK.LoadSpriteSheet("PSZ2/AniTiles2.gif", SCOPE_STAGE);

    if (isMainGameMode() || !globals->atlEnabled || PlayerHelpers_CheckStageReload()) {
        Zone->screenBoundsL1[0] = 1024;
        Zone->screenBoundsL1[1] = 1024;
#if RETRO_USE_PLUS
        Zone->screenBoundsL1[2] = 1024;
        Zone->screenBoundsL1[3] = 1024;
#endif
        FXFade_StopAll();
    }
    else {
        Zone->screenBoundsB1[0] = 1556;
        Zone->screenBoundsB1[1] = 1556;
#if RETRO_USE_PLUS
        Zone->screenBoundsB1[2] = 1556;
        Zone->screenBoundsB1[3] = 1556;
#endif
        PSZ2Setup_ActTransitionLoad();
    }

    if (isMainGameMode() && PlayerHelpers_CheckAct2())
        Zone->stageFinishCallback = PSZ2Setup_StageFinishCB;

#if RETRO_USE_PLUS
    if (RSDK_sceneInfo->filter & FILTER_ENCORE)
        RSDK.LoadPalette(0, "EncorePSZ2.act", 0xFF);
    Animals->animalTypes[0] = ANIMAL_POCKY;
    Animals->animalTypes[1] = ANIMAL_BECKY;
#endif
}

void PSZ2Setup_TriggerCB2(void) { PSZ2Setup->flag = true; }

void PSZ2Setup_TriggerCB1(void) { PSZ2Setup->flag = false; }

void PSZ2Setup_ActTransitionLoad(void)
{
    SaveGame_LoadPlayerState();
    Zone_ReloadStoredEntities(0x6140000, 0x1D80000, false);
    globals->recallEntities      = false;
    globals->restartMilliseconds = 0;
    globals->restartSeconds      = 0;
    globals->restartMinutes      = 0;
    memset(globals->atlEntityData, 0, TEMPENTITY_START * sizeof(int));

    Zone->screenBoundsL1[0] = 0;
    Zone->screenBoundsR1[0] = 944;
    Zone->screenBoundsT1[0] = 0;
    Zone->screenBoundsB1[0] = 1556;
    Zone->screenBoundsL1[0] = 0;
    Zone->screenBoundsR1[0] = 944;
    Zone->screenBoundsT1[0] = 0;
    Zone->screenBoundsB1[1] = 1556;
    Zone->screenBoundsL1[0] = 0;
    Zone->screenBoundsR1[0] = 944;
    Zone->screenBoundsT1[0] = 0;
    Zone->screenBoundsB1[2] = 1556;
    Zone->screenBoundsL1[0] = 0;
    Zone->screenBoundsR1[0] = 944;
    Zone->screenBoundsT1[0] = 0;
    Zone->screenBoundsB1[3] = 1556;
}

void PSZ2Setup_StageFinishCB(void) { RSDK.CreateEntity(PSZ2Outro->objectID, NULL, 0, 0); }

void PSZ2Setup_EditorDraw(void) {}

void PSZ2Setup_EditorLoad(void) {}

void PSZ2Setup_Serialize(void) {}
