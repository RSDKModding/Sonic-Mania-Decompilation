// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: AIZEncoreTutorial Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectAIZEncoreTutorial *AIZEncoreTutorial;

void AIZEncoreTutorial_Update(void)
{
    RSDK_THIS(AIZEncoreTutorial);
    RSDK.ProcessAnimation(&self->animator1);
    StateMachine_Run(self->state);
}

void AIZEncoreTutorial_LateUpdate(void) {}

void AIZEncoreTutorial_StaticUpdate(void) {}

void AIZEncoreTutorial_Draw(void)
{
    RSDK_THIS(AIZEncoreTutorial);
    Vector2 drawPos;

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->inkEffect = INK_ALPHA;
    drawPos.x         = (RSDK.Cos256(self->angle) << 12) + self->position.x;
    drawPos.y         = ((RSDK.Sin256(self->angle) + 512) << 11) + self->position.y;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    drawPos.x = (RSDK.Cos256(self->angle + 128) << 12) + self->position.x;
    drawPos.y = ((RSDK.Sin256(self->angle + 128) + 512) << 11) + self->position.y;
    RSDK.DrawSprite(&self->animator3, &drawPos, false);

    drawPos.x = self->position.x;
    drawPos.y = self->position.y - 0x100000;
    RSDK.DrawSprite(&self->animator4, &drawPos, false);

    switch (self->animator4.frameID) {
        default: break;
        case 0:
        case 2: drawPos.y -= 0x30000; break;
    }

    RSDK.DrawSprite(&self->animator5, &drawPos, false);
}

void AIZEncoreTutorial_Create(void *data)
{
    RSDK_THIS(AIZEncoreTutorial);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderHigh;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->state         = AIZEncoreTutorial_State_ShowTutBubble;
        RSDK.SetSpriteAnimation(AIZEncoreTutorial->cutsceneFrames, 7, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(AIZEncoreTutorial->cutsceneFrames, 4, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(AIZEncoreTutorial->cutsceneFrames, voidToInt(data), &self->animator3, true, 0);
        RSDK.SetSpriteAnimation(AIZEncoreTutorial->cutsceneFrames, 8, &self->animator4, true, 0);
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
    if (self->animator1.frameID == 6) {
        HUD_GetKeyFrame(&self->animator5, KEY_Y);
        self->state = AIZEncoreTutorial_State_EnterTutorial;
    }
}

void AIZEncoreTutorial_State_EnterTutorial(void)
{
    RSDK_THIS(AIZEncoreTutorial);
    if (self->alpha >= 0x100) {
        self->state = AIZEncoreTutorial_State_ShowSwapTutorial;
    }
    else {
        self->alpha += 8;
    }
}

void AIZEncoreTutorial_State_ShowSwapTutorial(void)
{
    RSDK_THIS(AIZEncoreTutorial);
    RSDK.ProcessAnimation(&self->animator4);
    HUD_GetKeyFrame(&self->animator5, KEY_Y);

    if (self->timer >= 60) {
        self->angle += 4;
        if (self->angle == 128 || self->angle == 256) {
            self->timer = 0;
            if (++self->swapCount == 3)
                self->state = AIZEncoreTutorial_State_ExitTutorial;
        }
        self->angle &= 0xFF;
    }
    else {
        self->timer++;
    }
}

void AIZEncoreTutorial_State_ExitTutorial(void)
{
    RSDK_THIS(AIZEncoreTutorial);
    if (self->alpha <= 0) {
        for (int32 i = 0; i < 8; ++i) {
            EntityDebris *debris =
                CREATE_ENTITY(Debris, NULL, self->position.x + RSDK.Rand(-0x180000, 0x180000), self->position.y + RSDK.Rand(-0x100000, 0x100000));
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
        destroyEntity(self);
    }
    else {
        self->alpha -= 8;
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
