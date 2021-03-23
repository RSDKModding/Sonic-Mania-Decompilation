#include "../SonicMania.h"

ObjectGHZCutsceneST *GHZCutsceneST;

void GHZCutsceneST_Update()
{
    void *states[4] = { GHZCutsceneST_CutsceneState_Unknown1, GHZCutsceneST_CutsceneState_Unknown2, GHZCutsceneST_CutsceneState_Unknown3,
                        GHZCutsceneST_CutsceneState_LoadNextStage };

    EntityGHZCutsceneST *entity = (EntityGHZCutsceneST *)RSDK_sceneInfo->entity;
    if (!entity->timer) {
        EntityPlayer *player = 0;
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox) && !player->sidekick) {
            CutsceneSeq_StartSequence((Entity *)entity, states);
            if (((Entity *)RSDK.GetEntityByID(SLOT_CUTSCENESEQ))->objectID) {
                EntityCutsceneSeq *cutsceneSeq = (EntityCutsceneSeq *)RSDK.GetEntityByID(SLOT_CUTSCENESEQ);
                cutsceneSeq->skipState         = 3;
                cutsceneSeq->skipCallback      = GHZCutsceneST_Skip;
            }
            entity->timer = 1;
        }
    }
}

void GHZCutsceneST_LateUpdate() {}

void GHZCutsceneST_StaticUpdate() {}

void GHZCutsceneST_Draw() {}

void GHZCutsceneST_Create(void *data)
{
    EntityGHZCutsceneST *entity = (EntityGHZCutsceneST *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = 0;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        if (!entity->size.x)
            entity->size.x = 0x1A80000;
        if (!entity->size.y)
            entity->size.y = 0xF00000;
        entity->updateRange.x       = 0x800000 + entity->size.x;
        entity->updateRange.y       = 0x800000 + entity->size.y;
        RSDK_sceneInfo->timeEnabled = false;
        entity->hitbox.left         = -entity->size.x >> 17;
        entity->hitbox.right        = entity->size.x >> 17;
        entity->hitbox.top          = -entity->size.y >> 17;
        entity->hitbox.bottom       = entity->size.y >> 17;
        GHZCutsceneST_SetupCutscene();
    }
}

void GHZCutsceneST_StageLoad() {}

void GHZCutsceneST_SetupCutscene()
{
    EntityPlatform *platform = NULL;
    while (RSDK.GetEntities(Platform->objectID, (Entity **)&platform)) {
        if (!platform->frameID) {
            platform->drawOrder     = Zone->drawOrderHigh - 1;
            GHZCutsceneST->platform = (Entity*)platform;
            RSDK.BreakForeachLoop();
        }
    }

    EntityAIZKingClaw *claw = NULL;
    if (RSDK.GetEntities(AIZKingClaw->objectID, (Entity **)&claw)) {
        GHZCutsceneST->claw = (Entity *)claw;
        RSDK.BreakForeachLoop();
    }

    EntityPhantomRuby *phantomRuby = NULL;
    if (RSDK.GetEntities(PhantomRuby->objectID, (Entity **)&phantomRuby)) {
        GHZCutsceneST->phantomRuby = (Entity *)phantomRuby;
        RSDK.BreakForeachLoop();
    }

    EntityFXRuby *fxRuby = NULL;
    if (RSDK.GetEntities(FXRuby->objectID, (Entity **)&fxRuby)) {
        GHZCutsceneST->fxRuby  = (Entity *)fxRuby;
        fxRuby->state          = 0;
        fxRuby->timer          = 512;
        fxRuby->waitForTrigger = 512;
        fxRuby->outerRadius    = RSDK_screens->width;
        fxRuby->fadeWhite      = 64;
        RSDK.BreakForeachLoop();
    }
    EntityCutsceneHBH *cutsceneHBH = 0;
    while (RSDK.GetEntities(CutsceneHBH->objectID, (Entity **)&cutsceneHBH)) {
        // GHZCutsceneST->cutsceneHBH[cutsceneHBH->characterID] = cutsceneHBH;
    }
}

bool32 GHZCutsceneST_CutsceneState_Unknown1() { return true; }
bool32 GHZCutsceneST_CutsceneState_Unknown2() { return true; }
bool32 GHZCutsceneST_CutsceneState_Unknown3() { return true; }

bool32 GHZCutsceneST_CutsceneState_LoadNextStage()
{
    EntityGHZCutsceneST *entity = (EntityGHZCutsceneST *)RSDK_sceneInfo->entity;
    if (globals->gameMode == MODE_ENCORE)
        RSDK.LoadScene("Encore Mode", "");
    else
        RSDK.LoadScene("Mania Mode", "");
    globals->parallaxOffset[0] = entity->timer2;
    EntityPlayer *player       = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
    player->onGround           = true;
    player->state              = Player_State_Ground;
    Zone_StoreEntities((RSDK_screens->position.x + RSDK_screens->centerX) << 16, (RSDK_screens->height + RSDK_screens->position.y) << 16);
    RSDK.InitSceneLoad();
    return true;
}

void GHZCutsceneST_Skip()
{
    if (globals->gameMode == MODE_ENCORE)
        RSDK.LoadScene("Encore Mode", "Green Hill Zone+ 1");
    else
        RSDK.LoadScene("Mania Mode", "Green Hill Zone 1");
}

void GHZCutsceneST_EditorDraw() {}

void GHZCutsceneST_EditorLoad() {}

void GHZCutsceneST_Serialize() { RSDK_EDITABLE_VAR(GHZCutsceneST, VAR_VECTOR2, size); }
