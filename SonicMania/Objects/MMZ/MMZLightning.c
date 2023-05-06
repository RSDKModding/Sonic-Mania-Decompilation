// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MMZLightning Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectMMZLightning *MMZLightning;

void MMZLightning_Update(void)
{
    RSDK_THIS(MMZLightning);

    StateMachine_Run(self->state);
}

void MMZLightning_LateUpdate(void) {}

void MMZLightning_StaticUpdate(void) {}

void MMZLightning_Draw(void)
{
    RSDK_THIS(MMZLightning);

    RSDK.SetActivePalette(1, 0, ScreenInfo->size.y);

    RSDK.DrawSprite(&self->animator, NULL, true);

    RSDK.SetActivePalette(0, 0, ScreenInfo->size.y);
}

void MMZLightning_Create(void *data)
{
    RSDK_THIS(MMZLightning);

    if (!SceneInfo->inEditor) {
        self->active = ACTIVE_NORMAL;
        self->state  = (Type_StateMachine)data;
    }
}

void MMZLightning_StageLoad(void) { MMZLightning->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Lightning.bin", SCOPE_STAGE); }

void MMZLightning_State_BeginFadeIn(void)
{
    RSDK_THIS(MMZLightning);

    if (!self->timer)
        RSDK.SetLimitedFade(0, 0, 5, 192, 0, 255);

    if (++self->timer == 4) {
        self->timer = 0;
        self->state = MMZLightning_State_FadeIn;
    }
}

void MMZLightning_State_FadeIn(void)
{
    RSDK_THIS(MMZLightning);

    RSDK.SetLimitedFade(0, 4, 1, self->timer, false, 255);

    self->timer += 4;
    if (self->timer > 256)
        destroyEntity(self);
}

void MMZLightning_State_SetupLightningBig(void)
{
    RSDK_THIS(MMZLightning);

    RSDK.SetSpriteAnimation(MMZLightning->aniFrames, 0, &self->animator, true, 0);

    self->position.x = (ScreenInfo->center.x + RSDK.Rand(-140, 140)) << 16;
    self->position.y = RSDK.Rand(0x200000, 0x400000);
    self->visible    = true;
    self->drawFX     = FX_FLIP;
    self->direction  = RSDK.Rand(0, 2);
    self->alpha      = 0x200;
    self->inkEffect  = INK_ADD;
    self->drawGroup  = 1;

    self->state = MMZLightning_State_ShowLightningBig;
}

void MMZLightning_State_ShowLightningBig(void)
{
    RSDK_THIS(MMZLightning);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1)
        self->state = MMZLightning_State_LightningBigFadeOut;
}

void MMZLightning_State_LightningBigFadeOut(void)
{
    RSDK_THIS(MMZLightning);

    if (self->alpha > 0)
        self->alpha -= 8;
    else
        destroyEntity(self);
}

void MMZLightning_State_SetupLightningSmall(void)
{
    RSDK_THIS(MMZLightning);

    RSDK.SetSpriteAnimation(MMZLightning->aniFrames, 1, &self->animator, true, RSDK.Rand(0, 4));

    self->position.x = (ScreenInfo->center.x + RSDK.Rand(-64, 64)) << 16;
    self->position.y = RSDK.Rand(0x400000, 0x600000);
    self->visible    = true;
    self->drawFX     = FX_FLIP;
    self->direction  = RSDK.Rand(0, 4);
    self->alpha      = 320;
    self->inkEffect  = INK_ADD;
    self->drawGroup  = 1;

    self->state = MMZLightning_State_LightningSmallFadeOut;
}

void MMZLightning_State_LightningSmallFadeOut(void)
{
    RSDK_THIS(MMZLightning);

    if (self->alpha > 0)
        self->alpha -= 0x20;
    else
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void MMZLightning_EditorDraw(void) {}

void MMZLightning_EditorLoad(void) {}
#endif

void MMZLightning_Serialize(void) {}
#endif
