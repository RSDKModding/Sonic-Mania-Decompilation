#include "../SonicMania.h"

ObjectCutsceneSeq *CutsceneSeq;

void CutsceneSeq_Update(void) {}

void CutsceneSeq_LateUpdate(void)
{
    EntityCutsceneSeq *entity = (EntityCutsceneSeq *)RSDK_sceneInfo->entity;
    entity->currentState      = (bool32(*)(Entity *))entity->cutsceneStates[entity->stateID];
    if (entity->currentState) {
        EntityCutsceneSeq *cutsceneSeq = (EntityCutsceneSeq *)RSDK_sceneInfo->entity;
        RSDK_sceneInfo->entity         = entity->cutsceneCurEntity;
        bool32 stateComplete           = entity->currentState((Entity*)cutsceneSeq);
        RSDK_sceneInfo->entity         = (Entity *)cutsceneSeq;
        ++entity->timer;
        if (stateComplete) {
            Game_Print("State completed");
            CutsceneSeq_NewState(entity->stateID + 1, entity);
        }

        if (!entity->cutsceneStates[entity->stateID]) {
            Game_Print("Sequence completed");
            RSDK.ResetEntityPtr(entity, 0, 0);
        }
    }

    for (int i = 0; i < 8; ++i) {
        Vector2 *point = &entity->points[i];
        if (point->x && point->y) {
            break;
        }
        else {
            entity->visible = true;
        }
    }
    if (entity->fillTimerA <= 0 && entity->fillTimerB <= 0) {
        entity->visible = false;
    }
}

void CutsceneSeq_StaticUpdate(void) {}

void CutsceneSeq_Draw(void)
{
    EntityCutsceneSeq *entity = (EntityCutsceneSeq *)RSDK_sceneInfo->entity;

    colour colours[8];
    colours[0] = 0xFF00;
    colours[1] = 0xFF0000;
    colours[2] = 0xFF;
    colours[3] = 0xFF00FF;
    colours[4] = 0xFFFF00;
    colours[5] = 0xFFFF;
    colours[6] = 0x9933FF;
    colours[7] = 0xFF9900;

    for (int i = 0; i < 8; ++i) {
        Vector2 *point = &entity->points[i];
        if (point->x || point->y) {
#if RETRO_USE_PLUS
            RSDK.PrintVector2(0, "Draw poi ", point->x, point->y);
#endif
            RSDK.DrawLine(point->x - 0x100000, point->y - 0x100000, point->x + 0x100000, point->y + 0x100000, colours[i], 0x7F, INK_NONE, 0);
            RSDK.DrawLine(point->x + 0x100000, point->y - 0x100000, point->x - 0x100000, point->y + 0x100000, colours[i], 0x7F, INK_NONE, 0);
        }
    }

    if (entity->fillTimerA > 0)
        RSDK.FillScreen(0xFFF0F0, entity->fillTimerA, entity->fillTimerA - 256, entity->fillTimerA - 256);
    if (entity->fillTimerB > 0)
        RSDK.FillScreen(0x000000, entity->fillTimerB, entity->fillTimerB - 128, entity->fillTimerB - 256);
}

void CutsceneSeq_Create(void *data)
{
    EntityCutsceneSeq *entity = (EntityCutsceneSeq *)RSDK_sceneInfo->entity;
    entity->active            = ACTIVE_NORMAL;
    entity->visible           = 0;
    entity->fillTimerA        = 0;
    entity->drawOrder         = Zone->uiDrawHigh + 1;
    entity->fillTimerB        = 0;
    CutsceneSeq_CheckSkip(entity->skipState, entity, entity->skipCallback);
}

void CutsceneSeq_StageLoad(void) {}

Entity *CutsceneSeq_GetEntity(int type)
{
    Entity *entity = NULL;
    if (RSDK.GetEntities(type, (Entity **)&entity)) {
        RSDK.BreakForeachLoop();
        return entity;
    }
    return NULL;
}

void CutsceneSeq_LockAllPlayerControl(void)
{
    for (int i = 0; i < Player->playerCount; ++i) {
        CutsceneSeq_LockPlayerControl(RSDK.GetEntityByID(i));
    }
}
void CutsceneSeq_LockPlayerControl(void *plr)
{
    EntityPlayer *player   = (EntityPlayer *)plr;
    player->up             = false;
    player->down           = false;
    player->left           = false;
    player->right          = false;
    player->jumpHold       = false;
    player->jumpPress      = false;
    Player->upState        = 0;
    Player->downState      = 0;
    Player->leftState      = 0;
    Player->rightState     = 0;
    Player->jumpPressState = 0;
    Player->jumpHoldState  = 0;
}

void CutsceneSeq_CheckSkip(byte skipState, EntityCutsceneSeq *entity, void (*skipCallback)(void))
{
    if (skipState && RSDK_controller->keyStart.press && RSDK_sceneInfo->state & 1) {
        if (skipState == 2) {
            ++RSDK_sceneInfo->listPos;
        }

        if (skipCallback && skipState == 3)
            skipCallback();

        if (skipState == 2 || skipState != 3 || entity->skipState == 3 || entity->skipState)
            globals->suppressTitlecard = 0;
        globals->suppressAutoMusic = 0;
        globals->enableIntro       = 0;
        RSDK.SetGameMode(ENGINESTATE_FROZEN);
        Zone_Unknown1(20, 0);
        Music_FadeOut(0.029999999);
    }
}
void CutsceneSeq_NewState(int nextState, EntityCutsceneSeq *CutsceneSeq)
{
    CutsceneSeq->stateID     = nextState;
    CutsceneSeq->timer       = 0;
    CutsceneSeq->field_68    = 0;
    CutsceneSeq->field_6C[0] = 0;
    CutsceneSeq->points[0].x = 0;
    CutsceneSeq->points[0].y = 0;
    CutsceneSeq->field_6C[1] = 0;
    CutsceneSeq->points[1].x = 0;
    CutsceneSeq->points[1].y = 0;
    CutsceneSeq->field_6C[2] = 0;
    CutsceneSeq->points[2].x = 0;
    CutsceneSeq->points[2].y = 0;
    CutsceneSeq->field_6C[3] = 0;
    CutsceneSeq->points[3].x = 0;
    CutsceneSeq->points[3].y = 0;
    CutsceneSeq->field_6C[4] = 0;
    CutsceneSeq->points[4].x = 0;
    CutsceneSeq->points[4].y = 0;
    CutsceneSeq->field_6C[5] = 0;
    CutsceneSeq->points[5].x = 0;
    CutsceneSeq->points[5].y = 0;
    CutsceneSeq->field_6C[6] = 0;
    CutsceneSeq->points[6].x = 0;
    CutsceneSeq->points[6].y = 0;
    CutsceneSeq->field_6C[7] = 0;
    CutsceneSeq->points[7].x = 0;
    CutsceneSeq->points[7].y = 0;
}
void CutsceneSeq_StartSequence(Entity *host, void *states)
{
    EntityCutsceneSeq *cutsceneSeq;
    if (((Entity*)RSDK.GetEntityByID(SLOT_CUTSCENESEQ))->objectID)
        return;
    RSDK.ResetEntitySlot(SLOT_CUTSCENESEQ, CutsceneSeq->objectID, 0);
    cutsceneSeq                    = (EntityCutsceneSeq *)RSDK.GetEntityByID(SLOT_CUTSCENESEQ);
    cutsceneSeq->position.x   = 0;
    cutsceneSeq->position.y   = 0;
    cutsceneSeq->cutsceneCurEntity = RSDK_sceneInfo->entity;
    cutsceneSeq->hostEntity        = host;
    int id                         = 0;
    if (states) {
        void **curState = &states;
        while (*curState) {
            Game_Print("state = %x", states);
            cutsceneSeq->cutsceneStates[id++] = *curState;
        }
    }
    Game_Print("Starting sequence with %d states", id);
}

void CutsceneSeq_EditorDraw(void) {}

void CutsceneSeq_EditorLoad(void) {}

void CutsceneSeq_Serialize(void) {}
