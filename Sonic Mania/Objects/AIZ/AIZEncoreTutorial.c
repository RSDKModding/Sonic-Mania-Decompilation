#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectAIZEncoreTutorial *AIZEncoreTutorial;

void AIZEncoreTutorial_Update(void)
{
    RSDK_THIS(AIZEncoreTutorial);
    RSDK.ProcessAnimation(&entity->animator1);
    StateMachine_Run(entity->state);
}

void AIZEncoreTutorial_LateUpdate(void) {}

void AIZEncoreTutorial_StaticUpdate(void) {}

void AIZEncoreTutorial_Draw(void)
{
    RSDK_THIS(AIZEncoreTutorial);
    Vector2 drawPos;

    entity->inkEffect = INK_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->inkEffect = INK_ALPHA;
    drawPos.x         = (RSDK.Cos256(entity->angle) << 12) + entity->position.x;
    drawPos.y         = ((RSDK.Sin256(entity->angle) + 512) << 11) + entity->position.y;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    drawPos.x = (RSDK.Cos256(entity->angle + 128) << 12) + entity->position.x;
    drawPos.y = ((RSDK.Sin256(entity->angle + 128) + 512) << 11) + entity->position.y;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y - 0x100000;
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);

    switch (entity->animator4.frameID) {
        default: break;
        case 0:
        case 2: drawPos.y -= 0x30000; break;
    }

    RSDK.DrawSprite(&entity->animator5, &drawPos, false);
}

void AIZEncoreTutorial_Create(void *data)
{
    RSDK_THIS(AIZEncoreTutorial);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->state         = AIZEncoreTutorial_State_ShowTutBubble;
        RSDK.SetSpriteAnimation(AIZEncoreTutorial->cutsceneFrames, 7, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(AIZEncoreTutorial->cutsceneFrames, 4, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(AIZEncoreTutorial->cutsceneFrames, voidToInt(data), &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(AIZEncoreTutorial->cutsceneFrames, 8, &entity->animator4, true, 0);
    }
}

void AIZEncoreTutorial_StageLoad(void)
{
    AIZEncoreTutorial->cutsceneFrames = RSDK.LoadSpriteAnimation("AIZ/Cutscene.bin", SCOPE_STAGE);
    AIZEncoreTutorial->dustFrames     = RSDK.LoadSpriteAnimation("Global/Dust.bin", SCOPE_STAGE);
}

void AIZEncoreTutorial_State_ShowTutBubble(void)
{
    RSDK_THIS(AIZEncoreTutorial);
    if (entity->animator1.frameID == 6) {
        HUD_GetKeyFrame(&entity->animator5, KEY_Y);
        entity->state = AIZEncoreTutorial_State_EnterTutorial;
    }
}

void AIZEncoreTutorial_State_EnterTutorial(void)
{
    RSDK_THIS(AIZEncoreTutorial);
    if (entity->alpha >= 0x100) {
        entity->state = AIZEncoreTutorial_State_ShowSwapTutorial;
    }
    else {
        entity->alpha += 8;
    }
}

void AIZEncoreTutorial_State_ShowSwapTutorial(void)
{
    RSDK_THIS(AIZEncoreTutorial);
    RSDK.ProcessAnimation(&entity->animator4);
    HUD_GetKeyFrame(&entity->animator5, KEY_Y);

    if (entity->timer >= 60) {
        entity->angle += 4;
        if (entity->angle == 128 || entity->angle == 256) {
            entity->timer = 0;
            if (++entity->swapCount == 3)
                entity->state = AIZEncoreTutorial_State_ExitTutorial;
        }
        entity->angle &= 0xFF;
    }
    else {
        entity->timer++;
    }
}

void AIZEncoreTutorial_State_ExitTutorial(void)
{
    RSDK_THIS(AIZEncoreTutorial);
    if (entity->alpha <= 0) {
        for (int32 i = 0; i < 8; ++i) {
            EntityDebris *debris =
                CREATE_ENTITY(Debris, NULL, entity->position.x + RSDK.Rand(-0x180000, 0x180000), entity->position.y + RSDK.Rand(-0x100000, 0x100000));
            debris->state      = Debris_State_Move;
            debris->velocity.x = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, 0x20000);
            debris->drawFX     = FX_FLIP | FX_SCALE;
            debris->direction  = i & 3;
            debris->scale.x    = RSDK.Rand(0x200, 0x400);
            debris->scale.y    = debris->scale.x;
            debris->drawOrder  = Zone->drawOrderHigh;
            RSDK.SetSpriteAnimation(AIZEncoreTutorial->dustFrames, 0, &debris->animator, true, RSDK.Rand(0, 4));
        }
        destroyEntity(entity);
    }
    else {
        entity->alpha -= 8;
    }
}

void AIZEncoreTutorial_State_ReturnToCutscene(void)
{
    EntityCutsceneSeq *cutsceneSeq = RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq);
    EntityFXRuby *fxRuby           = CREATE_ENTITY(FXRuby, NULL, 0, 0);
    fxRuby->drawOrder              = Zone->playerDrawHigh + 1;
    EncoreIntro->fxRuby            = (Entity *)fxRuby;
    PhantomRuby_PlaySFX(RUBYSFX_ATTACK3);
    Music_FadeOut(0.012);

    cutsceneSeq->skipType = SKIPTYPE_DISABLED;
    for (int32 i = 0; i < 64; ++i) {
        if (cutsceneSeq->cutsceneStates[i] == EncoreIntro_CutsceneState_Unknown24) {
            CutsceneSeq_NewState(i, cutsceneSeq);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void AIZEncoreTutorial_EditorDraw(void) {}

void AIZEncoreTutorial_EditorLoad(void) {}
#endif

void AIZEncoreTutorial_Serialize(void) {}
#endif
