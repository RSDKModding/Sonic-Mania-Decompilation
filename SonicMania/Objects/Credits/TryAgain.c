// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TryAgain Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTryAgain *TryAgain;

void TryAgain_Update(void)
{
    RSDK_THIS(TryAgain);

    StateMachine_Run(self->state);
}

void TryAgain_LateUpdate(void) {}

void TryAgain_StaticUpdate(void) {}

void TryAgain_Draw(void)
{
    RSDK_THIS(TryAgain);

    RSDK.SetActivePalette(0, 0, ScreenInfo->size.y);
    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->size.x, (self->position.y >> 16));

    RSDK.DrawSprite(&self->eggmanAnimator, &self->eggmanPos, false);
    RSDK.DrawSprite(&self->rubyAnimator, &self->rubyPos, false);

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->size.x, ScreenInfo->size.y);
    RSDK.DrawSprite(&self->debrisAnimator, NULL, false);

    RSDK.SetActivePalette(1, 0, ScreenInfo->size.y);
}

void TryAgain_Create(void *data)
{
    RSDK_THIS(TryAgain);

    if (!SceneInfo->inEditor) {
        self->eggmanPos.x = self->position.x;
        self->eggmanPos.y = self->position.y + 0x100000;
        self->rubyPos.x   = self->eggmanPos.x - 0x340000;
        self->rubyPos.y   = self->eggmanPos.y;

        self->visible       = true;
        self->drawGroup     = 1;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        self->state = TryAgain_State_Init;
        RSDK.SetSpriteAnimation(TryAgain->aniFrames, 0, &self->debrisAnimator, true, 0);
        RSDK.SetSpriteAnimation(TryAgain->aniFrames, 2, &self->eggmanAnimator, true, 0);
        RSDK.SetSpriteAnimation(TryAgain->aniFrames, 4, &self->rubyAnimator, true, 0);
    }
}

void TryAgain_StageLoad(void)
{
    TryAgain->aniFrames = RSDK.LoadSpriteAnimation("Credits/TryAgain.bin", SCOPE_STAGE);

    RSDK.CopyPalette(0, 0, 1, 0, 128);
}

void TryAgain_State_Init(void)
{
    RSDK_THIS(TryAgain);

    if (++self->timer == 60) {
        self->timer           = 0;
        self->eggmanVelocityY = -0x5C000;

#if MANIA_USE_PLUS
        Music_PlayTrack(TRACK_STAGE);
#endif
        self->state = TryAgain_State_EnterEggman;
    }
}

void TryAgain_State_EnterEggman(void)
{
    RSDK_THIS(TryAgain);

    self->eggmanVelocityY += 0x3800;
    int32 targetPos = self->position.y - 0x340000;
    self->eggmanPos.y += self->eggmanVelocityY;

    if (self->eggmanPos.y <= targetPos || self->eggmanVelocityY <= 0) {
        self->rubyPos.y = self->eggmanPos.y;
    }
    else {
        self->eggmanVelocityY = 0;
        self->eggmanPos.y     = targetPos;
        self->rubyPos.y       = targetPos;
        self->state           = TryAgain_State_EggmanLaugh;
    }
}

void TryAgain_State_EggmanLaugh(void)
{
    RSDK_THIS(TryAgain);

    if (self->timer > (MANIA_USE_PLUS ? 15 : 30))
        RSDK.ProcessAnimation(&self->eggmanAnimator);

    if (++self->timer == 120) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(TryAgain->aniFrames, 3, &self->eggmanAnimator, true, 2);
        self->state = TryAgain_State_Stinger;
    }
}

void TryAgain_State_Stinger(void)
{
    RSDK_THIS(TryAgain);

    RSDK.ProcessAnimation(&self->eggmanAnimator);

    if (self->eggmanAnimator.frameID == 2) {
        if (self->eggmanAnimator.timer == 1) {
            self->rubyVelocity.x = 0x22000;
            self->rubyVelocity.y = -0x58000;
        }
    }
    else if (self->eggmanAnimator.frameID == 7 && self->eggmanAnimator.timer == 1) {
        self->rubyVelocity.x = -0x22000;
        self->rubyVelocity.y = -0x58000;
    }

    self->rubyVelocity.y += 0x3800;
    self->rubyPos.x += self->rubyVelocity.x;
    self->rubyPos.y += self->rubyVelocity.y;

    if (self->rubyPos.y <= self->eggmanPos.y + 0x80000) {
        if (self->rubyPos.y < self->position.y)
            RSDK.ProcessAnimation(&self->rubyAnimator);
    }
    else {
        self->rubyPos.y      = self->eggmanPos.y + 0x80000;
        self->rubyVelocity.x = 0;
        self->rubyVelocity.y = 0;
        RSDK.SetSpriteAnimation(TryAgain->aniFrames, 4, &self->rubyAnimator, true, 0);
    }

    ++self->timer;
    if (ControllerInfo->keyA.press || ControllerInfo->keyStart.press)
        self->timer = 600;

    if (self->timer == 600) {
#if MANIA_USE_PLUS
        PhantomRuby_PlaySfx(RUBYSFX_ATTACK4);
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xFFFFFF), self->position.x, self->position.y);
        fxFade->speedIn      = 24;
        fxFade->speedOut     = 24;
        fxFade->fadeOutBlack = true;
#else
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0x000000), self->position.x, self->position.y);
        fxFade->speedIn      = 12;
        fxFade->wait         = 240;
#endif
    }

    if (self->timer >= (MANIA_USE_PLUS ? 740 : 680)) {
#if MANIA_USE_PLUS
        if (API.CheckDLC(DLC_PLUS))
            RSDK.SetScene("Presentation", "Game Summary");
        else
#endif
            RSDK.SetScene("Presentation", "Menu");

        RSDK.LoadScene();
    }
}

#if GAME_INCLUDE_EDITOR
void TryAgain_EditorDraw(void) {}

void TryAgain_EditorLoad(void) {}
#endif

void TryAgain_Serialize(void) {}
