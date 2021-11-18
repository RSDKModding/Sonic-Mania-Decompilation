#include "SonicMania.h"

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
    RSDK.SetActivePalette(0, 0, ScreenInfo->height);
    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, (self->position.y >> 16));
    RSDK.DrawSprite(&self->animator2, &self->unknownPos1, false);
    RSDK.DrawSprite(&self->animator3, &self->unknownPos2, false);
    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ScreenInfo->height);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.SetActivePalette(1, 0, ScreenInfo->height);
}

void TryAgain_Create(void *data)
{
    RSDK_THIS(TryAgain);
    if (!SceneInfo->inEditor) {
        self->unknownPos1.x = self->position.x;
        self->unknownPos1.y = self->position.y;
        self->unknownPos1.y += 0x100000;
        self->unknownPos2.x = self->unknownPos1.x;
        self->unknownPos2.x -= 0x340000;
        self->unknownPos2.y = self->unknownPos1.y;
        self->visible       = true;
        self->drawOrder     = 1;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->state         = TryAgain_Unknown1;
        RSDK.SetSpriteAnimation(TryAgain->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(TryAgain->aniFrames, 2, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(TryAgain->aniFrames, 4, &self->animator3, true, 0);
    }
}

void TryAgain_StageLoad(void)
{
    TryAgain->aniFrames = RSDK.LoadSpriteAnimation("Credits/TryAgain.bin", SCOPE_STAGE);
    RSDK.CopyPalette(0, 0, 1, 0, 128);
}

void TryAgain_Unknown1(void)
{
    RSDK_THIS(TryAgain);
    if (++self->timer == 60) {
        self->timer    = 0;
        self->field_74 = -0x5C000;
#if RETRO_USE_PLUS
        Music_PlayTrack(TRACK_STAGE);
#endif
        self->state = TryAgain_Unknown2;
    }
}

void TryAgain_Unknown2(void)
{
    RSDK_THIS(TryAgain);
    self->field_74 += 0x3800;
    int32 val = self->position.y - 0x340000;
    self->unknownPos1.y += self->field_74;
    if (self->unknownPos1.y <= val || self->field_74 <= 0) {
        self->unknownPos2.y = self->unknownPos1.y;
    }
    else {
        self->unknownPos1.y = val;
        self->field_74      = 0;
        self->state         = TryAgain_Unknown3;
        self->unknownPos2.y = val;
    }
}

void TryAgain_Unknown3(void)
{
    RSDK_THIS(TryAgain);
    if (self->timer > (RETRO_USE_PLUS ? 15 : 30))
        RSDK.ProcessAnimation(&self->animator2);
    if (++self->timer == 120) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(TryAgain->aniFrames, 3, &self->animator2, true, 2);
        self->state = TryAgain_Unknown4;
    }
}

void TryAgain_Unknown4(void)
{
    RSDK_THIS(TryAgain);
    RSDK.ProcessAnimation(&self->animator2);
    if (self->animator2.frameID == 2) {
        if (self->animator2.animationTimer == 1) {
            self->emeraldVelocity.x = 0x22000;
            self->emeraldVelocity.y = -0x58000;
        }
    }
    else if (self->animator2.frameID == 7 && self->animator2.animationTimer == 1) {
        self->emeraldVelocity.x = -0x22000;
        self->emeraldVelocity.y = -0x58000;
    }

    self->unknownPos2.x += self->emeraldVelocity.x;
    self->emeraldVelocity.y += 0x3800;
    self->unknownPos2.y += self->emeraldVelocity.y;

    if (self->unknownPos2.y <= self->unknownPos1.y + 0x80000) {
        if (self->unknownPos2.y < self->position.y)
            RSDK.ProcessAnimation(&self->animator3);
    }
    else {
        self->unknownPos2.y     = self->unknownPos1.y + 0x80000;
        self->emeraldVelocity.x = 0;
        self->emeraldVelocity.y = 0;
        RSDK.SetSpriteAnimation(TryAgain->aniFrames, 4, &self->animator3, true, 0);
    }

    ++self->timer;
    if (ControllerInfo->keyA.press || ControllerInfo->keyStart.press)
        self->timer = 600;
#if RETRO_USE_TOUCH_CONTROLS
    else if (TouchInfo->count)
        self->timer = 600;
#endif

    if (self->timer == 600) {
#if RETRO_USE_PLUS
        PhantomRuby_PlaySFX(RUBYSFX_ATTACK4);
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xFFFFFF), self->position.x, self->position.y);
        fxFade->speedIn      = 24;
        fxFade->speedOut     = 24;
        fxFade->fadeOutBlack = true;
#else
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0x000000), self->position.x, self->position.y);
        fxFade->speedIn = 12;
        fxFade->wait    = 240;
#endif
    }
    if (self->timer >= (RETRO_USE_PLUS ? 740 : 680)) {
#if RETRO_USE_PLUS
        if (API.CheckDLC(DLC_PLUS))
            RSDK.SetScene("Presentation", "Game Summary");
        else
#endif
            RSDK.SetScene("Presentation", "Menu");
        RSDK.LoadScene();
    }
}

#if RETRO_INCLUDE_EDITOR
void TryAgain_EditorDraw(void) {}

void TryAgain_EditorLoad(void) {}
#endif

void TryAgain_Serialize(void) {}
