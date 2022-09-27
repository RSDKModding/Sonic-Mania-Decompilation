// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ThanksSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectThanksSetup *ThanksSetup;

void ThanksSetup_Update(void)
{
    RSDK_THIS(ThanksSetup);

    StateMachine_Run(self->state);

    ScreenInfo->position.x = 0x100 - ScreenInfo->center.x;
}

void ThanksSetup_LateUpdate(void) {}

void ThanksSetup_StaticUpdate(void) {}

void ThanksSetup_Draw(void)
{
    RSDK_THIS(ThanksSetup);

    StateMachine_Run(self->stateDraw);
}

void ThanksSetup_Create(void *data)
{
    RSDK_THIS(ThanksSetup);

    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_ALWAYS;
        self->visible   = true;
        self->drawGroup = 12;
        self->drawFX    = FX_FLIP;
        self->state     = ThanksSetup_State_ThanksForPlaying;
        self->stateDraw = ThanksSetup_Draw_Fade;

        self->timer   = 0x300;
        self->radius  = 0x10000;
        self->scale.x = 0x200;
        self->scale.y = 0x200;

        self->thanksLogo          = RSDK_GET_ENTITY(RESERVE_ENTITY_COUNT + 16, UIPicture);
        self->thanksLogo->scale.x = 0x200;
        self->thanksLogo->scale.y = 0x200;
    }
}

void ThanksSetup_StageLoad(void)
{
    ThanksSetup->sfxSega = RSDK.GetSfx("Stage/Sega.wav");

    RSDK.ResetEntitySlot(0, ThanksSetup->classID, 0);

    UIPicture->aniFrames = RSDK.LoadSpriteAnimation("Thanks/Decorations.bin", SCOPE_STAGE);

    for (int32 c = 0; c < CHANNEL_COUNT; ++c) RSDK.StopChannel(c);
}

void ThanksSetup_HandleIconsPos(void)
{
    RSDK_THIS(ThanksSetup);

    self->angle = (self->angle - 3) & 0x1FF;

    int32 angle = self->angle;
    foreach_active(UIPicture, picture)
    {
        if (!picture->animator.animationID) {
            picture->position.x = 0x1000000 + self->radius * RSDK.Sin512(angle);
            picture->position.y = 0x780000 + self->radius * RSDK.Cos512(angle);
            angle += 32;
        }
    }
}

void ThanksSetup_State_ThanksForPlaying(void)
{
    RSDK_THIS(ThanksSetup);

    if (self->timer <= 0) {
        self->timer = 0;
        self->radius += (0x3000 - self->radius) >> 4;
        self->state     = ThanksSetup_State_FlipOverIcon;
        self->stateDraw = 0;
    }
    else {
        self->radius += (0x3000 - self->radius) >> 4;
        self->timer -= 16;
    }

    ThanksSetup_HandleIconsPos();
}

void ThanksSetup_State_FlipOverIcon(void)
{
    RSDK_THIS(ThanksSetup);

    if (++self->timer > 120) {
        self->rotation += 4;
        EntityUIPicture *picture = self->thanksLogo;

        picture->drawFX  = FX_SCALE;
        picture->scale.x = RSDK.Cos512(self->rotation);

        if (self->rotation == 128) {
            RSDK.SetSpriteAnimation(UIPicture->aniFrames, 2, &picture->animator, true, 0);
            self->timer = 0;
            self->state = ThanksSetup_State_Mania2017;
        }
    }
    self->radius += (0x3000 - self->radius) >> 4;

    ThanksSetup_HandleIconsPos();
}

void ThanksSetup_State_Mania2017(void)
{
    RSDK_THIS(ThanksSetup);

    if (self->rotation <= 0) {
        if (++self->timer > 120) {
            self->timer     = 0;
            self->state     = ThanksSetup_State_FadeOut;
            self->stateDraw = ThanksSetup_Draw_Fade;
        }
    }
    else {
        EntityUIPicture *picture = self->thanksLogo;

        self->rotation -= 4;
        picture->scale.x = RSDK.Cos512(self->rotation);

        if (!self->rotation)
            picture->drawFX = FX_NONE;
    }

    ThanksSetup_HandleIconsPos();
}

void ThanksSetup_State_FadeOut(void)
{
    RSDK_THIS(ThanksSetup);

    if (self->timer >= 1024) {
        SceneInfo->listPos = 0;
        RSDK.LoadScene();
    }
    else {
        self->timer += 16;
    }

    self->radius += (self->radius - 0x2000) >> 4;

    ThanksSetup_HandleIconsPos();
}

void ThanksSetup_Draw_Fade(void)
{
    RSDK_THIS(ThanksSetup);

    RSDK.FillScreen(0x000000, self->timer, self->timer - 128, self->timer - 256);
}

#if RETRO_INCLUDE_EDITOR
void ThanksSetup_EditorDraw(void) {}

void ThanksSetup_EditorLoad(void) { UIPicture->aniFrames = RSDK.LoadSpriteAnimation("Thanks/Decorations.bin", SCOPE_STAGE); }
#endif

void ThanksSetup_Serialize(void) {}
