// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CutsceneSeq Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCutsceneSeq *CutsceneSeq;

void CutsceneSeq_Update(void)
{
#if MANIA_USE_PLUS
    RSDK_THIS(CutsceneSeq);

    CutsceneSeq_CheckSkip(self->skipType, self, self->skipCallback);
#endif
}

void CutsceneSeq_LateUpdate(void)
{
    RSDK_THIS(CutsceneSeq);

    self->currentState = self->cutsceneStates[self->stateID];
    if (self->currentState) {
        SceneInfo->entity    = self->activeEntity;
        bool32 finishedState = self->currentState(self);
        SceneInfo->entity    = (Entity *)self;

        ++self->timer;
        if (finishedState) {
            LogHelpers_Print("State completed");
            CutsceneSeq_NewState(self->stateID + 1, self);
        }

        if (!self->cutsceneStates[self->stateID]) {
            LogHelpers_Print("Sequence completed");
            destroyEntity(self);
        }
    }

    self->visible = false;
    for (int32 p = 0; p < CUTSCENESEQ_POINT_COUNT; ++p) {
        if (self->points[p].x && self->points[p].y)
            self->visible = true;
    }

    if (self->fadeWhite > 0 || self->fadeBlack > 0)
        self->visible = true;
}

void CutsceneSeq_StaticUpdate(void) {}

void CutsceneSeq_Draw(void)
{
    RSDK_THIS(CutsceneSeq);

    color colors[CUTSCENESEQ_POINT_COUNT];
    colors[0] = 0x00FF00;
    colors[1] = 0xFF0000;
    colors[2] = 0x0000FF;
    colors[3] = 0xFF00FF;
    colors[4] = 0xFFFF00;
    colors[5] = 0x00FFFF;
    colors[6] = 0x9933FF;
    colors[7] = 0xFF9900;

    for (int32 p = 0; p < CUTSCENESEQ_POINT_COUNT; ++p) {
        Vector2 *point = &self->points[p];
        if (point->x || point->y) {
#if MANIA_USE_PLUS
            LogHelpers_PrintVector2("Draw poi ", *point);
#endif
            DrawHelpers_DrawCross(point->x, point->y, TO_FIXED(32), TO_FIXED(32), colors[p]);
        }
    }

    if (self->fadeWhite > 0)
        RSDK.FillScreen(0xFFF0F0, self->fadeWhite, self->fadeWhite - 256, self->fadeWhite - 256);

    if (self->fadeBlack > 0)
        RSDK.FillScreen(0x000000, self->fadeBlack, self->fadeBlack - 128, self->fadeBlack - 256);
}

void CutsceneSeq_Create(void *data)
{
    RSDK_THIS(CutsceneSeq);

    self->active    = ACTIVE_NORMAL;
    self->visible   = false;
    self->fadeWhite = 0;
    self->fadeBlack = 0;
    self->drawGroup = Zone->hudDrawGroup + 1;

#if MANIA_USE_PLUS
    CutsceneSeq_CheckSkip(self->skipType, self, self->skipCallback);
#endif
}

void CutsceneSeq_StageLoad(void) {}

void CutsceneSeq_NewState(int32 nextState, EntityCutsceneSeq *seq)
{
    seq->stateID     = nextState;
    seq->timer       = 0;
    seq->storedTimer = 0;

    for (int32 p = 0; p < CUTSCENESEQ_POINT_COUNT; ++p) {
        seq->values[p]   = 0;
        seq->points[p].x = 0;
        seq->points[p].y = 0;
    }
}
#if MANIA_USE_PLUS
void CutsceneSeq_SetSkipType(uint8 type, void (*callback)(void))
{
    EntityCutsceneSeq *seq = RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq);
    if (seq->classID) {
        seq->skipType     = type;
        seq->skipCallback = callback;
    }
}

void CutsceneSeq_CheckSkip(uint8 skipType, EntityCutsceneSeq *seq, void (*skipCallback)(void))
{
    bool32 skipPress = ControllerInfo->keyStart.press;

    if (skipType && skipPress && (SceneInfo->state & ENGINESTATE_REGULAR)) {
        bool32 loadNewScene = false;

        if (skipType == SKIPTYPE_NEXTSCENE) {
            ++SceneInfo->listPos;
            loadNewScene = true;
        }
        else {
            if (skipType == SKIPTYPE_CALLBACK) {
                StateMachine_Run(skipCallback);
            }

            loadNewScene = seq && (seq->skipType == SKIPTYPE_CALLBACK || seq->skipType == SKIPTYPE_RELOADSCN);
        }

        if (loadNewScene) {
            globals->suppressTitlecard = false;
            globals->suppressAutoMusic = false;
            globals->enableIntro       = false;

            RSDK.SetEngineState(ENGINESTATE_FROZEN);

            Zone_StartFadeOut(20, 0x000000);
            Music_FadeOut(0.03);
        }
    }
}
#endif

Entity *CutsceneSeq_GetEntity(int32 type)
{
    foreach_all_type(type, entity) { foreach_return entity; }

    return NULL;
}

void CutsceneSeq_LockPlayerControl(EntityPlayer *player)
{
    player->up        = false;
    player->down      = false;
    player->left      = false;
    player->right     = false;
    player->jumpHold  = false;
    player->jumpPress = false;

    Player->upState        = 0;
    Player->downState      = 0;
    Player->leftState      = 0;
    Player->rightState     = 0;
    Player->jumpPressState = 0;
    Player->jumpHoldState  = 0;
}

void CutsceneSeq_LockAllPlayerControl(void)
{
    for (int32 p = 0; p < Player->playerCount; ++p) {
        CutsceneSeq_LockPlayerControl(RSDK_GET_ENTITY(p, Player));
    }
}
void CutsceneSeq_StartSequence(void *manager, ...)
{
    if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->classID)
        return;

    RSDK.ResetEntitySlot(SLOT_CUTSCENESEQ, CutsceneSeq->classID, NULL);
    EntityCutsceneSeq *cutsceneSeq = RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq);

    cutsceneSeq->position.x    = 0;
    cutsceneSeq->position.y    = 0;
    cutsceneSeq->activeEntity  = SceneInfo->entity;
    cutsceneSeq->managerEntity = manager;

    va_list list;
    va_start(list, manager);

    int32 count = 0;
    for (count = 0;; ++count) {
        void *state = va_arg(list, void *);
        if (!state)
            break;

        LogHelpers_Print("state = %x", state);
        cutsceneSeq->cutsceneStates[count] = (bool32(*)(EntityCutsceneSeq *))state;
    }
    va_end(list);

    LogHelpers_Print("Starting sequence with %d states", count);
}

#if RETRO_INCLUDE_EDITOR
void CutsceneSeq_EditorDraw(void) {}

void CutsceneSeq_EditorLoad(void) {}
#endif

void CutsceneSeq_Serialize(void) {}
