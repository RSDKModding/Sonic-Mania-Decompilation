// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MSZCutsceneK Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMSZCutsceneK *MSZCutsceneK;

void MSZCutsceneK_Update(void)
{
    RSDK_THIS(MSZCutsceneK);

    if (!self->activated) {
        MSZCutsceneK_StartCutscene();
        self->activated = true;
    }
}

void MSZCutsceneK_LateUpdate(void) {}

void MSZCutsceneK_StaticUpdate(void) {}

void MSZCutsceneK_Draw(void) {}

void MSZCutsceneK_Create(void *data)
{
    RSDK_THIS(MSZCutsceneK);

    INIT_ENTITY(self);
    CutsceneRules_SetupEntity(self, &self->size, &self->hitbox);
    self->active = ACTIVE_BOUNDS;
}

void MSZCutsceneK_StageLoad(void)
{
    MSZCutsceneK->playerFrames = RSDK.LoadSpriteAnimation("Players/KnuxCutsceneAIZ.bin", SCOPE_STAGE);

    MSZCutsceneK->sfxImpact = RSDK.GetSfx("Stage/Impact5.wav");
    MSZCutsceneK->sfxDrop   = RSDK.GetSfx("Stage/Drop.wav");

    MSZCutsceneK->mystic  = NULL;
    MSZCutsceneK->tornado = NULL;

    foreach_all(HeavyMystic, mystic)
    {
        MSZCutsceneK->mystic = mystic;
        foreach_break;
    }

    foreach_all(Tornado, tornado)
    {
        MSZCutsceneK->tornado = tornado;
        foreach_break;
    }
}

#if MANIA_USE_PLUS
void MSZCutsceneK_Cutscene_SkipCB(void)
{
    RSDK.SetScene("Mania Mode", "");
    SceneInfo->listPos += TimeAttackData_GetManiaListPos(ZONE_MSZ, ACT_1, CHAR_KNUX);
}
#endif

void MSZCutsceneK_StartCutscene(void)
{
    RSDK_THIS(MSZCutsceneK);

    CutsceneSeq_StartSequence(self, MSZCutsceneK_Cutscene_RidingTornado, MSZCutsceneK_Cutscene_KnockedOffTornado, StateMachine_None);

#if MANIA_USE_PLUS
    CutsceneSeq_SetSkipTypeCallback(MSZCutsceneK_Cutscene_SkipCB);
#endif
}

void MSZCutsceneK_SetupP2(int32 x, int32 y)
{
    Player->sonicFrames = RSDK.LoadSpriteAnimation("Players/Sonic.bin", SCOPE_STAGE);
    Player->superFrames = RSDK.LoadSpriteAnimation("Players/SuperSonic.bin", SCOPE_STAGE);

    RSDK.ResetEntitySlot(SLOT_PLAYER2, Player->classID, NULL);

    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    ++Player->playerCount;
    player2->characterID  = ID_SONIC;
    player2->position.x   = x;
    player2->position.y   = y;
    player2->aniFrames    = Player->sonicFrames;
    player2->tailFrames   = -1;
    player2->jumpOffset   = 0x50000;
    player2->stateAbility = Player_JumpAbility_Sonic;
    player2->sensorY      = 0x140000;
    player2->stateInput   = StateMachine_None;
    player2->state        = Player_State_Static;
    RSDK.SetSpriteAnimation(Player->sonicFrames, ANI_RIDE, &player2->animator, true, 0);
}

bool32 MSZCutsceneK_Cutscene_RidingTornado(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    EntityHeavyMystic *mystic = MSZCutsceneK->mystic;
    EntityTornado *tornado    = MSZCutsceneK->tornado;
    if (!host->timer) {
        SceneInfo->timeEnabled  = false;
        SceneInfo->milliseconds = 0;
        player1->state          = Player_State_Static;
        player1->stateInput     = StateMachine_None;
        CutsceneSeq_LockAllPlayerControl();
        player1->velocity.x = 0;
        player1->velocity.y = 0;
        RSDK.SetSpriteAnimation(MSZCutsceneK->playerFrames, 6, &player1->animator, true, 0);
        MSZCutsceneK_SetupP2(0xCC0000, 0x29E0000);
    }

    if (mystic->position.x > tornado->position.x - 0x100000) {
        MSZCutsceneK->tornadoDistanceP1.x = player1->position.x - tornado->position.x;
        MSZCutsceneK->tornadoDistanceP1.y = player1->position.y - tornado->position.y;
        MSZCutsceneK->tornadoDistanceP2.x = player2->position.x - tornado->position.x;
        MSZCutsceneK->tornadoDistanceP2.y = player2->position.y - tornado->position.y;
        return true;
    }

    return false;
}

bool32 MSZCutsceneK_Cutscene_KnockedOffTornado(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    EntityTornado *tornado = MSZCutsceneK->tornado;

    if (!host->timer) {
        Music_Stop();
        RSDK.PlaySfx(MSZCutsceneK->sfxImpact, false, 0xFF);
        tornado->velocity.y = -0x10000;
        if (!host->values[0]) {
            host->values[0] = true;
            RSDK.PlaySfx(MSZCutsceneK->sfxDrop, false, 0xFF);
            RSDK.SetSpriteAnimation(MSZCutsceneK->playerFrames, 4, &player1->animator, true, 0);
            player1->velocity.x = -0x10000;
            player1->velocity.y = -0x30000;
        }
    }

    tornado->position.x += tornado->velocity.x;
    tornado->position.y += tornado->velocity.y;

    if (tornado->position.y >= tornado->movePos.y) {
        tornado->velocity.y = 0;
        tornado->position.y = tornado->movePos.y;
    }
    else {
        tornado->velocity.y += 0x3800;
    }

    if (host->values[0]) {
        if (!host->values[2])
            player1->velocity.y += 0x3800;
    }
    else {
        player1->position.x = tornado->position.x + MSZCutsceneK->tornadoDistanceP1.x;
        player1->position.y = tornado->position.y + MSZCutsceneK->tornadoDistanceP1.y;
    }

    if (host->values[1]) {
        if (host->timer - host->storedTimer == 15) {
            globals->suppressTitlecard = true;
            globals->suppressAutoMusic = true;
            globals->enableIntro       = true;
            RSDK.SetScene("Mania Mode", "");
            SceneInfo->listPos += TimeAttackData_GetManiaListPos(ZONE_MSZ, ACT_1, CHAR_KNUX);
            Zone_StartFadeOut(10, 0x000000);
        }
        else if (host->timer - host->storedTimer == 60) {
            player1->velocity.y = 0;
            host->values[2]     = true;
        }
    }
    else if (player1->position.y > tornado->position.y) {
        TornadoPath->camera = NULL;
        host->values[1]     = true;
        host->storedTimer   = host->timer;
        MSZCutsceneK->pos3  = camera->position;
        MSZCutsceneK->pos3.y += 0x1E00000;
        Camera_SetupLerp(CAMERA_LERP_NORMAL, 0, MSZCutsceneK->pos3.x, MSZCutsceneK->pos3.y, 3);
    }

    player2->position.x = tornado->position.x + MSZCutsceneK->tornadoDistanceP2.x;
    player2->position.y = tornado->position.y + MSZCutsceneK->tornadoDistanceP2.y;
    return false;
}

#if GAME_INCLUDE_EDITOR
void MSZCutsceneK_EditorDraw(void)
{
    RSDK_THIS(MSZCutsceneK);

    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void MSZCutsceneK_EditorLoad(void) {}
#endif

void MSZCutsceneK_Serialize(void) { RSDK_EDITABLE_VAR(MSZCutsceneK, VAR_VECTOR2, size); }
