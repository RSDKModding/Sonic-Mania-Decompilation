#include "SonicMania.h"

ObjectThanksSetup *ThanksSetup;

void ThanksSetup_Update(void)
{
    RSDK_THIS(ThanksSetup);
    StateMachine_Run(self->state);
    ScreenInfo->position.x = 0x100 - ScreenInfo->centerX;
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
        self->active           = ACTIVE_ALWAYS;
        self->visible          = true;
        self->drawOrder        = 12;
        self->drawFX           = FX_FLIP;
        self->state            = ThanksSetup_Unknown2;
        self->stateDraw        = ThanksSetup_Unknown6;
        self->timer            = 768;
        self->offset           = 0x10000;
        self->scale.x          = 0x200;
        self->scale.y          = 0x200;
        EntityUIPicture *picture = RSDK_GET_ENTITY(80, UIPicture);
        picture->scale.x         = 0x200;
        picture->scale.y         = 0x200;
        self->picture          = (Entity*)picture;
    }
}

void ThanksSetup_StageLoad(void)
{
    ThanksSetup->sfxSega = RSDK.GetSFX("Stage/Sega.wav");
    RSDK.ResetEntitySlot(0, ThanksSetup->objectID, 0);
    UIPicture->aniFrames = RSDK.LoadSpriteAnimation("Thanks/Decorations.bin", SCOPE_STAGE);
    for (int32 i = 0; i < 16; ++i) {
        RSDK.StopChannel(i);
    }
}

void ThanksSetup_Unknown1(void)
{
    RSDK_THIS(ThanksSetup);
    self->angle = (self->angle - 3) & 0x1FF;

    int32 angle = self->angle;
    foreach_active(UIPicture, picture)
    {
        if (!picture->animator.animationID) {
            picture->position.x = 0x1000000 + self->offset * RSDK.Sin512(angle);
            picture->position.y = 0x780000 + self->offset * RSDK.Cos512(angle);
            angle += 32;
        }
    }
}

void ThanksSetup_Unknown2(void)
{
    RSDK_THIS(ThanksSetup);

    if (self->timer <= 0) {
        self->timer = 0;
        self->offset += (0x3000 - self->offset) >> 4;
        self->state     = ThanksSetup_Unknown3;
        self->stateDraw = 0;
    }
    else {
        self->offset += (0x3000 - self->offset) >> 4;
        self->timer -= 16;
    }
    ThanksSetup_Unknown1();
}

void ThanksSetup_Unknown3(void)
{
    RSDK_THIS(ThanksSetup);
    if (++self->timer > 120) {
        self->rotation += 4;
        EntityUIPicture *picture = (EntityUIPicture *)self->picture;
        picture->drawFX          = FX_SCALE;
        picture->scale.x         = RSDK.Cos512(self->rotation);
        if (self->rotation == 128) {
            RSDK.SetSpriteAnimation(UIPicture->aniFrames, 2, &picture->animator, true, 0);
            self->timer = 0;
            self->state = ThanksSetup_Unknown4;
        }
    }
    self->offset += (0x3000 - self->offset) >> 4;
    ThanksSetup_Unknown1();
}

void ThanksSetup_Unknown4(void)
{
    RSDK_THIS(ThanksSetup);

    if (self->rotation <= 0) {
        if (++self->timer > 120) {
            self->timer     = 0;
            self->state     = ThanksSetup_Unknown5;
            self->stateDraw = ThanksSetup_Unknown6;
        }
    }
    else {
        EntityUIPicture *picture = (EntityUIPicture *)self->picture;
        self->rotation -= 4;
        picture->scale.x = RSDK.Cos512(self->rotation);
        if (!self->rotation) {
            picture->drawFX = FX_NONE;
        }
    }
    ThanksSetup_Unknown1();
}

void ThanksSetup_Unknown5(void)
{
    RSDK_THIS(ThanksSetup);

    if (self->timer >= 1024) {
        SceneInfo->listPos = 0;
        RSDK.LoadScene();
    }
    else {
        self->timer += 16;
    }
    self->offset += (self->offset - 0x2000) >> 4;
    ThanksSetup_Unknown1();
}

void ThanksSetup_Unknown6(void)
{
    RSDK_THIS(ThanksSetup);
    RSDK.FillScreen(0, self->timer, self->timer - 128, self->timer - 256);
}

#if RETRO_INCLUDE_EDITOR
void ThanksSetup_EditorDraw(void) {}

void ThanksSetup_EditorLoad(void) {}
#endif

void ThanksSetup_Serialize(void) {}
