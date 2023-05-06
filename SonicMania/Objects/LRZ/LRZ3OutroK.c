// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZ3OutroK Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLRZ3OutroK *LRZ3OutroK;

void LRZ3OutroK_Update(void)
{
    RSDK_THIS(LRZ3OutroK);

    if (!self->activated) {
        LRZ3OutroK_StartCutscene();

        self->activated = true;
    }
}

void LRZ3OutroK_LateUpdate(void) {}

void LRZ3OutroK_StaticUpdate(void) {}

void LRZ3OutroK_Draw(void) {}

void LRZ3OutroK_Create(void *data)
{
    RSDK_THIS(LRZ3OutroK);

    INIT_ENTITY(self);
    CutsceneRules_SetupEntity(self, &self->size, &self->hitbox);
    self->active = ACTIVE_NEVER;
}

void LRZ3OutroK_StageLoad(void)
{
    foreach_all(EggPrison, prison)
    {
        LRZ3OutroK->prison = prison;
        foreach_break;
    }

    foreach_all(SkyTeleporter, teleporter)
    {
        LRZ3OutroK->teleporter = teleporter;
        foreach_break;
    }

    foreach_all(FXRuby, fxRuby)
    {
        LRZ3OutroK->fxRuby = fxRuby;
        foreach_break;
    }

    LRZ3OutroK->sfxWarp = RSDK.GetSfx("LRZ/Warp.wav");
}

void LRZ3OutroK_StartCutscene(void)
{
    RSDK_THIS(MSZCutsceneK);

    CutsceneSeq_StartSequence(self, LRZ3OutroK_Cutscene_RunToTeleporter, LRZ3OutroK_Cutscene_LandOnTeleporter, LRZ3OutroK_Cutscene_UseTeleporter,
                              LRZ3OutroK_Cutscene_TeleporterActivated, StateMachine_None);

#if MANIA_USE_PLUS
    CutsceneSeq_SetSkipType(SKIPTYPE_RELOADSCN, StateMachine_None);
#endif

    HUD_MoveOut();
}

bool32 LRZ3OutroK_Cutscene_RunToTeleporter(EntityCutsceneSeq *host)
{
    RSDK_THIS(LRZ3OutroK);

    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);

    EntitySkyTeleporter *teleporter = LRZ3OutroK->teleporter;

    Vector2 size;
    RSDK.GetLayerSize(Zone->fgLayer[0], &size, true);
    if (!host->timer) {
        LRZ3OutroK->prison->notSolid = true;

        Zone->cameraBoundsT[0] = 0;
        Zone->cameraBoundsT[1] = 0;
        Zone->cameraBoundsR[0] = (self->position.x + self->size.x) >> 16;
        Zone->cameraBoundsR[1] = (self->position.x + self->size.x) >> 16;

        Zone->playerBoundActiveR[0] = false;
        Zone->playerBoundActiveR[1] = false;

        CutsceneSeq_LockAllPlayerControl();

        player1->stateInput = StateMachine_None;
        player1->state      = Player_State_Ground;
        player1->groundVel  = 0;
        player1->right      = true;

        if (player2->classID == Player->classID) {
            player2->state      = Player_State_Ground;
            player2->stateInput = Player_Input_P2_AI;
            player2->groundVel  = 0;
        }
    }

    if (player1->jumpPress)
        player1->jumpPress = false;

    if (player1->onGround && player1->position.x >= teleporter->position.x - 0x500000 && !host->values[0]) {
        player1->jumpPress = true;
        host->values[0]    = true;
        return true;
    }

    return false;
}

bool32 LRZ3OutroK_Cutscene_LandOnTeleporter(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);

    if (player1->jumpPress)
        player1->jumpPress = false;

    if (player1->onGround) {
        CutsceneSeq_LockPlayerControl(player1);
        if (player2->classID == Player->classID && player2->onGround)
            CutsceneSeq_LockPlayerControl(player2);

        return true;
    }

    return false;
}

bool32 LRZ3OutroK_Cutscene_UseTeleporter(EntityCutsceneSeq *host)
{
    RSDK_THIS(LRZ3OutroK);

    EntitySkyTeleporter *teleporter = LRZ3OutroK->teleporter;

    if (host->timer == 30) {
        foreach_active(Player, player)
        {
            player->state           = Player_State_Static;
            player->nextGroundState = StateMachine_None;
            player->nextAirState    = StateMachine_None;

            RSDK.SetSpriteAnimation(SkyTeleporter->aniFrames, 1, &teleporter->animator, true, 0);

            if (player->characterID == ID_KNUCKLES)
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_GLIDE_DROP, &player->animator, true, 3);
            else
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_DIAGONAL, &player->animator, false, 0);

            self->playerPos[player->playerID].x = player->position.x;
            self->playerPos[player->playerID].y = player->position.y;
        }

        RSDK.PlaySfx(LRZ3OutroK->sfxWarp, false, 255);
    }

    if (host->timer == 60) {
        foreach_active(Player, player) { RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_TWIRL, &player->animator, false, 3); }
    }

    int32 x = teleporter->position.x;
    int32 y = teleporter->position.y - 0x740000;

    if (host->timer >= 30) {
        int32 angle = 255 * (host->timer - 30) / 60;

        foreach_active(Player, player)
        {
            if (angle <= 0) {
                player->position.x = self->playerPos[player->playerID].x;
                player->position.y = self->playerPos[player->playerID].y;
            }
            else {
                if (angle < 0x100) {
                    player->position.x = self->playerPos[player->playerID].x;
                    player->position.y = self->playerPos[player->playerID].y;
                    player->position.x += ((RSDK.Sin512(angle + 0x180) >> 2) + 0x80) * ((x - self->playerPos[player->playerID].x) >> 8);
                    player->position.y += ((RSDK.Sin512(angle + 0x180) >> 2) + 0x80) * ((y - self->playerPos[player->playerID].y) >> 8);
                }
                else {
                    player->position.x = x;
                    player->position.y = y;
                }
            }

            player->position.x &= 0xFFFF0000;
            player->position.y &= 0xFFFF0000;
        }

        if (teleporter->timer < 64)
            teleporter->timer += 2;
    }

    if (host->timer == 90) {
        foreach_active(Player, player)
        {
            player->position.x = x;
            player->position.y = y;
        }

        return true;
    }

    return false;
}

bool32 LRZ3OutroK_Cutscene_TeleporterActivated(EntityCutsceneSeq *host)
{
    if (host->timer >= 60) {
        if (host->timer == 60)
            RSDK.PlaySfx(LRZ3OutroK->sfxWarp, false, 255);

        if (LRZ3OutroK->fxRuby->fadeWhite >= 512) {
            if (LRZ3OutroK->fxRuby->fadeBlack < 512)
                LRZ3OutroK->fxRuby->fadeBlack += 16;
        }
        else {
            LRZ3OutroK->fxRuby->fadeWhite += 16;
        }

        if (host->timer == 180) {
            RSDK.LoadScene();
            return true;
        }
    }

    return false;
}

#if GAME_INCLUDE_EDITOR
void LRZ3OutroK_EditorDraw(void)
{
    RSDK_THIS(LRZ3OutroK);

    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void LRZ3OutroK_EditorLoad(void) {}
#endif

void LRZ3OutroK_Serialize(void) { RSDK_EDITABLE_VAR(LRZ3OutroK, VAR_VECTOR2, size); }
