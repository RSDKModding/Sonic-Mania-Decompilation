#include "SonicMania.h"

ObjectCutsceneSeq *CutsceneSeq;

void CutsceneSeq_Update(void)
{
    RSDK_THIS(CutsceneSeq);
    CutsceneSeq_CheckSkip(self->skipType, self, self->skipCallback);
}

void CutsceneSeq_LateUpdate(void)
{
    RSDK_THIS(CutsceneSeq);
    self->currentState      = (bool32(*)(Entity *))self->cutsceneStates[self->stateID];
    if (self->currentState) {
        SceneInfo->entity         = self->cutsceneCurEntity;
        bool32 stateComplete = self->currentState((Entity *)self);
        SceneInfo->entity         = (Entity *)self;
        ++self->timer;
        if (stateComplete) {
            LogHelpers_Print("State completed");
            CutsceneSeq_NewState(self->stateID + 1, self);
        }

        if (!self->cutsceneStates[self->stateID]) {
            LogHelpers_Print("Sequence completed");
            RSDK.ResetEntityPtr(self, TYPE_BLANK, NULL);
        }
    }

    for (int32 i = 0; i < 8; ++i) {
        Vector2 *point = &self->points[i];
        if (point->x && point->y) {
            self->visible = true;
            return;
        }
    }
    if (self->fillTimerA <= 0 && self->fillTimerB <= 0) {
        self->visible = false;
    }
}

void CutsceneSeq_StaticUpdate(void) {}

void CutsceneSeq_Draw(void)
{
    RSDK_THIS(CutsceneSeq);

    colour colours[8];
    colours[0] = 0x00FF00;
    colours[1] = 0xFF0000;
    colours[2] = 0x0000FF;
    colours[3] = 0xFF00FF;
    colours[4] = 0xFFFF00;
    colours[5] = 0x00FFFF;
    colours[6] = 0x9933FF;
    colours[7] = 0xFF9900;

    for (int32 i = 0; i < 8; ++i) {
        Vector2 *point = &self->points[i];
        if (point->x || point->y) {
#if RETRO_USE_PLUS
            RSDK.PrintVector2(0, "Draw poi ", point->x, point->y);
#endif
            RSDK.DrawLine(point->x - 0x100000, point->y - 0x100000, point->x + 0x100000, point->y + 0x100000, colours[i], 0x7F, INK_NONE, false);
            RSDK.DrawLine(point->x + 0x100000, point->y - 0x100000, point->x - 0x100000, point->y + 0x100000, colours[i], 0x7F, INK_NONE, false);
        }
    }

    if (self->fillTimerA > 0)
        RSDK.FillScreen(0xFFF0F0, self->fillTimerA, self->fillTimerA - 256, self->fillTimerA - 256);
    if (self->fillTimerB > 0)
        RSDK.FillScreen(0x000000, self->fillTimerB, self->fillTimerB - 128, self->fillTimerB - 256);
}

void CutsceneSeq_Create(void *data)
{
    RSDK_THIS(CutsceneSeq);
    self->active            = ACTIVE_NORMAL;
    self->visible           = false;
    self->fillTimerA        = 0;
    self->drawOrder         = Zone->hudDrawOrder + 1;
    self->fillTimerB        = 0;
    CutsceneSeq_CheckSkip(self->skipType, self, self->skipCallback);
}

void CutsceneSeq_StageLoad(void) {}

Entity *CutsceneSeq_GetEntity(int32 type)
{
    Entity *entity = NULL;
    if (RSDK.GetEntities(type, (Entity **)&entity)) {
        foreach_return entity;
    }
    return NULL;
}

void CutsceneSeq_LockAllPlayerControl(void)
{
    for (int32 i = 0; i < Player->playerCount; ++i) {
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

void CutsceneSeq_CheckSkip(uint8 skipType, EntityCutsceneSeq *self, void (*skipCallback)(void))
{
    bool32 skipPress = ControllerInfo->keyStart.press;
#if RETRO_USE_TOUCH_CONTROLS
    skipPress |= TouchInfo->count;
#endif

    if (skipType && skipPress && (SceneInfo->state & 1)) {
        bool32 load = false;
        if (skipType == SKIPTYPE_NEXTSCENE) {
            ++SceneInfo->listPos;
            load = true;
        }
        else {
            if (skipCallback && skipType == SKIPTYPE_CALLBACK)
                skipCallback();
            load = self && (self->skipType == SKIPTYPE_CALLBACK || self->skipType == SKIPTYPE_RELOADSCN);
        }

        if (load) {
            globals->suppressTitlecard = false;
            globals->suppressAutoMusic = false;
            globals->enableIntro       = false;
            RSDK.SetGameMode(ENGINESTATE_FROZEN);
            Zone_StartFadeOut(20, 0x000000);
            Music_FadeOut(0.03);
        }
    }
}
void CutsceneSeq_NewState(int32 nextState, EntityCutsceneSeq *CutsceneSeq)
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
void CutsceneSeq_StartSequence(Entity *host, void **states)
{
    if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID)
        return;
    RSDK.ResetEntitySlot(SLOT_CUTSCENESEQ, CutsceneSeq->objectID, 0);
    EntityCutsceneSeq *cutsceneSeq = (EntityCutsceneSeq *)RSDK.GetEntityByID(SLOT_CUTSCENESEQ);
    cutsceneSeq->position.x   = 0;
    cutsceneSeq->position.y   = 0;
    cutsceneSeq->cutsceneCurEntity = SceneInfo->entity;
    cutsceneSeq->hostEntity        = host;
    int32 id                         = 0;
    while (states[id]) {
        LogHelpers_Print("state = %x", states[id]);
        cutsceneSeq->cutsceneStates[id] = states[id];
        id++;
    }
    LogHelpers_Print("Starting sequence with %d states", id);
}

#if RETRO_INCLUDE_EDITOR
void CutsceneSeq_EditorDraw(void) {}

void CutsceneSeq_EditorLoad(void) {}
#endif

void CutsceneSeq_Serialize(void) {}
