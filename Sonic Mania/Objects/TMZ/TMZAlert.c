// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TMZAlert Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
    self->drawFX           = FX_NONE;
    self->inkEffect        = INK_NONE;
    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, NULL, false);
    if (self->scale.y != 0x200)
        self->drawFX = FX_SCALE;
    self->inkEffect        = INK_ALPHA;
    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
}

void TMZAlert_Create(void *data)
{
    RSDK_THIS(TMZAlert);
    if (!SceneInfo->inEditor) {
        self->inkEffect     = INK_ALPHA;
        self->visible       = true;
        self->drawFX        = FX_SCALE;
        self->drawOrder     = Zone->drawOrderLow;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->scale.x       = 0x200;
        self->alpha         = 16 * (RSDK.Rand(-32, -8) - 16);
        RSDK.SetSpriteAnimation(TMZAlert->aniFrames, 0, &self->animator, true, 0);
    }
}

void TMZAlert_StageLoad(void) { TMZAlert->aniFrames = RSDK.LoadSpriteAnimation("Phantom/AlertScreen.bin", SCOPE_STAGE); }

void TMZAlert_Unknown1(void)
{
    RSDK_THIS(TMZAlert);

    if (self->alpha >= 256) {
        self->drawFX = FX_NONE;
        self->state  = TMZAlert_Unknown2;
    }
    else {
        self->alpha += 32;
        if (self->alpha > 256)
            self->alpha = 256;
        self->scale.y = 2 * self->alpha;
    }
}
void TMZAlert_Unknown2(void)
{
    RSDK_THIS(TMZAlert);
    self->alpha = (RSDK.Cos256(8 * ++self->timer) >> 2) + 224;
    if (!(self->timer & 0xF)) {
        RSDK.SetSpriteAnimation(TMZAlert->aniFrames, 1, &self->animator2, true, RSDK.Rand(0, 2));
    }
    if (self->timer == 320) {
        self->timer = 0;
        self->state = TMZAlert_Unknown3;
    }
}
void TMZAlert_Unknown3(void)
{
    RSDK_THIS(TMZAlert);
    if (self->alpha <= 16) {
        self->state = StateMachine_None;
    }
    else {
        self->alpha -= 8;
    }
}

#if RETRO_INCLUDE_EDITOR
void TMZAlert_EditorDraw(void)
{
    RSDK_THIS(TMZAlert);
    RSDK.SetSpriteAnimation(TMZAlert->aniFrames, 0, &self->animator, true, 0);

    TMZAlert_Draw();
}

void TMZAlert_EditorLoad(void) { TMZAlert->aniFrames = RSDK.LoadSpriteAnimation("Phantom/AlertScreen.bin", SCOPE_STAGE); }
#endif

void TMZAlert_Serialize(void) {}
