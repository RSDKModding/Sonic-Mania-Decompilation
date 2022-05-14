// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FXFade Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectFXFade *FXFade;

void FXFade_Update(void)
{
    RSDK_THIS(FXFade);

    StateMachine_Run(self->state);
}

void FXFade_LateUpdate(void) {}

void FXFade_StaticUpdate(void) {}

void FXFade_Draw(void)
{
    RSDK_THIS(FXFade);

    RSDK.FillScreen(self->color, self->timer, self->timer - 128, self->timer - 256);
}

void FXFade_Create(void *data)
{
    RSDK_THIS(FXFade);

    if (!SceneInfo->inEditor) {
        self->visible = true;
        self->active  = ACTIVE_NORMAL;

        if (RSDK.CheckStageFolder("Menu"))
            self->drawOrder = 14;
        else if (Zone)
            self->drawOrder = self->overHUD ? Zone->hudDrawOrder : Zone->hudDrawOrder - 1;
        else
            self->drawOrder = 15;

        if (!self->speedIn)
            self->speedIn = 32;

        if (!self->speedOut)
            self->speedOut = 32;

        if (!self->color)
            self->color = voidToInt(data);

        if (!self->eventOnly) {
#if RETRO_USE_PLUS
            self->transitionScene = false;
#endif
            self->state = self->timer <= 0 ? FXFade_State_FadeIn : FXFade_State_FadeOut;
        }
    }
}

void FXFade_StageLoad(void) {}

void FXFade_StopAll(void)
{
    foreach_all(FXFade, entity) { destroyEntity(entity); }
}

void FXFade_State_FadeIn(void)
{
    RSDK_THIS(FXFade);

    if (self->timer >= 512) {
        if (self->oneWay) {
            self->state = StateMachine_None;

#if RETRO_USE_PLUS
            if (self->transitionScene)
                RSDK.LoadScene();
#endif
        }
        else if (self->wait <= 0) {
            self->state = self->fadeOutBlack ? FXFade_State_FadeOutBlack : FXFade_State_FadeOut;
        }
        else {
            self->state = FXFade_State_Wait;
        }
    }
    else {
        self->timer += self->speedIn;
    }
}
void FXFade_State_Wait(void)
{
    RSDK_THIS(FXFade);

    if (--self->wait <= 0) {
        self->state = self->fadeOutBlack ? FXFade_State_FadeOutBlack : FXFade_State_FadeOut;
    }
}
void FXFade_State_FadeOut(void)
{
    RSDK_THIS(FXFade);

    if (self->timer <= 0) {
        if (self->oneWay)
            self->state = StateMachine_None;
        else
            destroyEntity(self);
    }
    else {
        self->timer -= self->speedOut;
    }
}
void FXFade_State_FadeOutBlack(void)
{
    RSDK_THIS(FXFade);

    int32 r = (self->color >> 16) & 0xFF;
    int32 g = (self->color >> 8) & 0xFF;
    int32 b = (self->color >> 0) & 0xFF;

    if (r)
        r -= self->speedOut;
    else if (g)
        g -= self->speedOut;
    else if (b)
        b -= self->speedOut;
    else
        self->state = StateMachine_None;

    self->color = (maxVal(r, 0) << 16) | (maxVal(g, 0) << 8) | (maxVal(b, 0) << 0);
}

#if RETRO_INCLUDE_EDITOR
void FXFade_EditorDraw(void)
{
    RSDK_THIS(FXFade);

    RSDK.DrawRect(self->position.x - 0x100000, self->position.y - 0x100000, 0x200000, 0x200000, self->color, 0xFF, INK_NONE, false);
}

void FXFade_EditorLoad(void) {}
#endif

void FXFade_Serialize(void)
{
    RSDK_EDITABLE_VAR(FXFade, VAR_ENUM, timer);
    RSDK_EDITABLE_VAR(FXFade, VAR_ENUM, speedIn);
    RSDK_EDITABLE_VAR(FXFade, VAR_ENUM, wait);
    RSDK_EDITABLE_VAR(FXFade, VAR_ENUM, speedOut);
    RSDK_EDITABLE_VAR(FXFade, VAR_COLOR, color);
    RSDK_EDITABLE_VAR(FXFade, VAR_BOOL, oneWay);
    RSDK_EDITABLE_VAR(FXFade, VAR_BOOL, eventOnly);
    RSDK_EDITABLE_VAR(FXFade, VAR_BOOL, overHUD);
    RSDK_EDITABLE_VAR(FXFade, VAR_BOOL, fadeOutBlack);
}
