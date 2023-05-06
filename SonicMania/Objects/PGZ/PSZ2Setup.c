// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PSZ2Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPSZ2Setup *PSZ2Setup;

void PSZ2Setup_Update(void) {}

void PSZ2Setup_LateUpdate(void) {}

void PSZ2Setup_StaticUpdate(void)
{
    if (--PSZ2Setup->petalAniDuration < 1) {
        ++PSZ2Setup->petalAniFrame;
        PSZ2Setup->petalAniFrame &= 7;
        PSZ2Setup->petalAniDuration = PSZ2Setup->petalAniDurationTable[PSZ2Setup->petalAniFrame];

        int32 sheetY = 32 * PSZ2Setup->petalAniFrame;
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles1, 260, 0, sheetY, 64, 32);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles1, 268, 80, sheetY, 48, 16);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles1, 271, 64, sheetY + 16, 64, 16);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles2, 159, 0, sheetY, 16, 32);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles2, 276, 16, sheetY, 32, 16);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles2, 281, 16, sheetY + 16, 32, 16);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles2, 279, 48, sheetY, 32, 16);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles2, 286, 48, sheetY + 16, 32, 16);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles2, 284, 80, sheetY, 32, 16);
        RSDK.DrawAniTiles(PSZ2Setup->aniTiles2, 289, 80, sheetY + 16, 32, 16);
    }

    RSDK.SetLimitedFade(0, 1, 2, abs(RSDK.Sin256(16 * Zone->timer)), 224, 227);

    if (PSZ2Setup->petalBehaviourActive) {
        if (PSZ2Setup->petalTimer <= 0) {
            foreach_active(Player, player)
            {
                Hitbox *playerHitbox = Player_GetHitbox(player);

                uint16 tile = RSDK.GetTile(Zone->fgLayer[0], player->position.x >> 20, (player->position.y + (playerHitbox->bottom << 16)) >> 20);
                bool32 isLowLayer = true;
                if (tile == (uint16)-1) {
                    tile       = RSDK.GetTile(Zone->fgLayer[1], player->position.x >> 20, (player->position.y + (playerHitbox->bottom << 16)) >> 20);
                    isLowLayer = false;
                }

                if (RSDK.GetTileFlags(tile, player->collisionPlane)) {
                    if (abs(player->groundVel) >= 0x60000 || player->state == Player_State_DropDash) {
                        RSDK_THIS(PSZ2Setup); // not sure what this is meant to be since this is a StaticUpdate event...

                        EntityPetalPile *pile = CREATE_ENTITY(PetalPile, self, player->position.x, player->position.y + (playerHitbox->bottom << 16));
                        pile->leafPattern     = PETALPILE_PATTERN_4;
                        pile->tileLayer       = isLowLayer;
                        pile->pileSize.x      = 0x40000;
                        pile->pileSize.y      = 0x40000;
                        pile->noRemoveTiles   = true;
                        pile->petalRadius     = 0xB5555;
                        pile->petalDir        = 2 * (player->direction != FLIP_NONE) - 1;
                        pile->petalVel        = player->groundVel >> 1;
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
    PSZ2Setup->petalBehaviourActive = false;

    GenericTrigger->callbacks[GENERICTRIGGER_PSZ2_PETALSINACTIVE] = PSZ2Setup_Trigger_DeactivatePetalBehaviour;
    GenericTrigger->callbacks[GENERICTRIGGER_PSZ2_PETALSACTIVE]   = PSZ2Setup_Trigger_ActivatePetalBehaviour;

    PSZ2Setup->aniTiles1 = RSDK.LoadSpriteSheet("PSZ2/AniTiles.gif", SCOPE_STAGE);
    PSZ2Setup->aniTiles2 = RSDK.LoadSpriteSheet("PSZ2/AniTiles2.gif", SCOPE_STAGE);

    if (!isMainGameMode() || !globals->atlEnabled || CutsceneRules_CheckStageReload()) {
        Zone->cameraBoundsL[0] = 1024;
        Zone->cameraBoundsL[1] = 1024;
        Zone->cameraBoundsL[2] = 1024;
        Zone->cameraBoundsL[3] = 1024;
        FXFade_StopAll();
    }
    else {
        Zone->cameraBoundsB[0] = 1556;
        Zone->cameraBoundsB[1] = 1556;
        Zone->cameraBoundsB[2] = 1556;
        Zone->cameraBoundsB[3] = 1556;
        PSZ2Setup_ActTransitionLoad();
    }

    if (isMainGameMode() && CutsceneRules_IsAct2())
        Zone->stageFinishCallback = PSZ2Setup_StageFinish_EndAct2;

#if MANIA_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE)
        RSDK.LoadPalette(0, "EncorePSZ2.act", 0b0000000011111111);

    // Fun Fact: Pre-Plus didn't have animal types set for PGZ! It'd always be flickies due to that being the default value!
    Animals->animalTypes[0] = ANIMAL_POCKY;
    Animals->animalTypes[1] = ANIMAL_BECKY;
#endif
}

void PSZ2Setup_Trigger_ActivatePetalBehaviour(void) { PSZ2Setup->petalBehaviourActive = true; }

void PSZ2Setup_Trigger_DeactivatePetalBehaviour(void) { PSZ2Setup->petalBehaviourActive = false; }

void PSZ2Setup_ActTransitionLoad(void)
{
    SaveGame_LoadPlayerState();
    Zone_ReloadStoredEntities(472 << 16, 1556 << 16, false);

    globals->recallEntities      = false;
    globals->restartMilliseconds = 0;
    globals->restartSeconds      = 0;
    globals->restartMinutes      = 0;
    memset(globals->atlEntityData, 0, TEMPENTITY_START * sizeof(int32));

    Zone->cameraBoundsL[0] = 0;
    Zone->cameraBoundsR[0] = 944;
    Zone->cameraBoundsT[0] = 0;
    Zone->cameraBoundsB[0] = 1556;
    Zone->cameraBoundsL[1] = 0;
    Zone->cameraBoundsR[1] = 944;
    Zone->cameraBoundsT[1] = 0;
    Zone->cameraBoundsB[1] = 1556;
    Zone->cameraBoundsL[2] = 0;
    Zone->cameraBoundsR[2] = 944;
    Zone->cameraBoundsT[2] = 0;
    Zone->cameraBoundsB[2] = 1556;
    Zone->cameraBoundsL[3] = 0;
    Zone->cameraBoundsR[3] = 944;
    Zone->cameraBoundsT[3] = 0;
    Zone->cameraBoundsB[3] = 1556;
}

void PSZ2Setup_StageFinish_EndAct2(void) { CREATE_ENTITY(PSZ2Outro, NULL, 0, 0); }

#if GAME_INCLUDE_EDITOR
void PSZ2Setup_EditorDraw(void) {}

void PSZ2Setup_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(GenericTrigger, triggerID);
    RSDK_ENUM_VAR("Stop Petal Behaviour", GENERICTRIGGER_PSZ2_PETALSINACTIVE);
    RSDK_ENUM_VAR("Start Petal Behaviour", GENERICTRIGGER_PSZ2_PETALSACTIVE);
}
#endif

void PSZ2Setup_Serialize(void) {}
