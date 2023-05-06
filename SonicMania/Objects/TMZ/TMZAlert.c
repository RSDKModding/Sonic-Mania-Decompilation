// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TMZAlert Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTMZAlert *TMZAlert;

void TMZAlert_Update(void)
{
    RSDK_THIS(TMZAlert);

    StateMachine_Run(self->state);
}

void TMZAlert_LateUpdate(void) {}

void TMZAlert_StaticUpdate(void) {}

void TMZAlert_Draw(void)
{
    RSDK_THIS(TMZAlert);

    self->drawFX                 = FX_NONE;
    self->inkEffect              = INK_NONE;
    self->screenAnimator.frameID = 0;
    RSDK.DrawSprite(&self->screenAnimator, NULL, false);

    if (self->scale.y != 0x200)
        self->drawFX = FX_SCALE;

    self->inkEffect              = INK_ALPHA;
    self->screenAnimator.frameID = 1;
    RSDK.DrawSprite(&self->screenAnimator, NULL, false);

    RSDK.DrawSprite(&self->messageAnimator, NULL, false);
}

void TMZAlert_Create(void *data)
{
    RSDK_THIS(TMZAlert);

    if (!SceneInfo->inEditor) {
        self->inkEffect     = INK_ALPHA;
        self->visible       = true;
        self->drawFX        = FX_SCALE;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->scale.x       = 0x200;
        self->alpha         = 0x10 * (RSDK.Rand(-32, -8) - 16);

        RSDK.SetSpriteAnimation(TMZAlert->aniFrames, 0, &self->screenAnimator, true, 0);
    }
}

void TMZAlert_StageLoad(void) { TMZAlert->aniFrames = RSDK.LoadSpriteAnimation("Phantom/AlertScreen.bin", SCOPE_STAGE); }

void TMZAlert_State_Activating(void)
{
    RSDK_THIS(TMZAlert);

    if (self->alpha >= 0x100) {
        self->drawFX = FX_NONE;
        self->state  = TMZAlert_State_Alerting;
    }
    else {
        self->alpha += 0x20;
        if (self->alpha > 0x100)
            self->alpha = 0x100;

        self->scale.y = 2 * self->alpha;
    }
}
void TMZAlert_State_Alerting(void)
{
    RSDK_THIS(TMZAlert);

    self->alpha = 0xE0 + (RSDK.Cos256(8 * ++self->timer) >> 2);

    if (!(self->timer & 0xF))
        RSDK.SetSpriteAnimation(TMZAlert->aniFrames, 1, &self->messageAnimator, true, RSDK.Rand(0, 2));

    if (self->timer == 320) {
        self->timer = 0;
        self->state = TMZAlert_State_ShuttingDown;
    }
}

void TMZAlert_State_ShuttingDown(void)
{
    RSDK_THIS(TMZAlert);

    if (self->alpha <= 0x10)
        self->state = StateMachine_None;
    else
        self->alpha -= 8;
}

#if GAME_INCLUDE_EDITOR
void TMZAlert_EditorDraw(void)
{
    RSDK_THIS(TMZAlert);

    RSDK.SetSpriteAnimation(TMZAlert->aniFrames, 0, &self->screenAnimator, true, 0);

    TMZAlert_Draw();
}

void TMZAlert_EditorLoad(void) { TMZAlert->aniFrames = RSDK.LoadSpriteAnimation("Phantom/AlertScreen.bin", SCOPE_STAGE); }
#endif

void TMZAlert_Serialize(void) {}
