#include "../SonicMania.h"

ObjectFXFade *FXFade;

void FXFade_Update(void)
{
    RSDK_THIS(FXFade);
    StateMachine_Run(entity->state);
}

void FXFade_LateUpdate(void) {}

void FXFade_StaticUpdate(void) {}

void FXFade_Draw(void)
{
    RSDK_THIS(FXFade);
    RSDK.FillScreen(entity->color, entity->timer, entity->timer - 128, entity->timer - 256);
}

void FXFade_Create(void *data)
{
    RSDK_THIS(FXFade);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible = true;
        entity->active  = ACTIVE_NORMAL;
        if (RSDK.CheckStageFolder("Menu")) {
            entity->drawOrder = 14;
        }
        else if (Zone) {
            if (!entity->overHUD)
                entity->drawOrder = Zone->hudDrawOrder - 1;
            else
                entity->drawOrder = Zone->hudDrawOrder;
        }
        else {
            entity->drawOrder = 15;
        }
        if (!entity->speedIn)
            entity->speedIn = 32;
        if (!entity->speedOut)
            entity->speedOut = 32;
        if (!entity->color)
            entity->color = voidToInt(data);
        if (!entity->eventOnly) {
#if RETRO_USE_PLUS
            entity->transitionScene = false;
#endif
            if (entity->timer <= 0)
                entity->state = FXFade_State_FadeIn;
            else
                entity->state = FXFade_State_FadeOut;
        }
    }
}

void FXFade_StageLoad(void) {}

void FXFade_StopAll(void)
{
    foreach_all(FXFade, entity) RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
}

void FXFade_State_FadeIn(void)
{
    RSDK_THIS(FXFade);
    if (entity->timer >= 512) {
        if (entity->oneWay) {
            entity->state = StateMachine_None;
#if RETRO_USE_PLUS
            if (entity->transitionScene)
                RSDK.InitSceneLoad();
#endif
        }
        else if (entity->wait <= 0) {
            if (entity->fadeOutBlack)
                entity->state = FXFade_State_FadeOutBlack;
            else
                entity->state = FXFade_State_FadeOut;
        }
        else {
            entity->state = FXFade_State_Wait;
        }
    }
    else {
        entity->timer += entity->speedIn;
    }
}
void FXFade_State_Wait(void)
{
    RSDK_THIS(FXFade);
    if (--entity->wait <= 0) {
        if (entity->fadeOutBlack)
            entity->state = FXFade_State_FadeOutBlack;
        else
            entity->state = FXFade_State_FadeOut;
    }
}
void FXFade_State_FadeOut(void)
{
    RSDK_THIS(FXFade);
    if (entity->timer <= 0) {
        if (entity->oneWay)
            entity->state = StateMachine_None;
        else
            RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
    else {
        entity->timer -= entity->speedOut;
    }
}
void FXFade_State_FadeOutBlack(void)
{
    RSDK_THIS(FXFade);
    if (entity->color >> 16) {
        if (((entity->color >> 16) - entity->speedOut) >= 0)
            entity->color += ((((entity->color >> 8) & 0xFF) + (((entity->color >> 16) - entity->speedOut) << 8)) << 8);
        else
            entity->color = 0;
    }
    else if ((entity->color >> 8) & 0xFF) {
        if ((((entity->color >> 8) & 0xFF) - entity->speedOut) >= 0)
            entity->color += (((entity->color >> 8) & 0xFF) - entity->speedOut) << 8;
        else
            entity->color = 0;
    }
    else if (entity->color & 0xFF) {
        if ((entity->color & 0xFF) - entity->speedOut < 0)
            entity->color = 0;
        else
            entity->color = (entity->color & 0xFF) - entity->speedOut;
    }
    else {
        entity->state = StateMachine_None;
    }
}

void FXFade_EditorDraw(void) {}

void FXFade_EditorLoad(void) {}

void FXFade_Serialize(void)
{
    RSDK_EDITABLE_VAR(FXFade, VAR_ENUM, timer);
    RSDK_EDITABLE_VAR(FXFade, VAR_ENUM, speedIn);
    RSDK_EDITABLE_VAR(FXFade, VAR_ENUM, wait);
    RSDK_EDITABLE_VAR(FXFade, VAR_ENUM, speedOut);
    RSDK_EDITABLE_VAR(FXFade, VAR_COLOUR, color);
    RSDK_EDITABLE_VAR(FXFade, VAR_BOOL, oneWay);
    RSDK_EDITABLE_VAR(FXFade, VAR_BOOL, eventOnly);
    RSDK_EDITABLE_VAR(FXFade, VAR_BOOL, overHUD);
    RSDK_EDITABLE_VAR(FXFade, VAR_BOOL, fadeOutBlack);
}
