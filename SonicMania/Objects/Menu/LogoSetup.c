// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LogoSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLogoSetup *LogoSetup;

void LogoSetup_Update(void)
{
    RSDK_THIS(LogoSetup);

    StateMachine_Run(self->state);

    ScreenInfo->position.x = 0x100 - ScreenInfo->center.x;
}

void LogoSetup_LateUpdate(void) {}

void LogoSetup_StaticUpdate(void) {}

void LogoSetup_Draw(void)
{
    RSDK_THIS(LogoSetup);

    StateMachine_Run(self->stateDraw);
}

void LogoSetup_Create(void *data)
{
    RSDK_THIS(LogoSetup);

    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_ALWAYS;
        self->visible   = true;
        self->drawGroup = 12;
        self->state     = sku_region == REGION_JP ? LogoSetup_State_CESAScreen : LogoSetup_State_ShowLogos;
        self->stateDraw = LogoSetup_Draw_Fade;
        self->timer     = 1024;
    }
}

void LogoSetup_StageLoad(void)
{
    LogoSetup->sfxSega = RSDK.GetSfx("Stage/Sega.wav");
    RSDK.ResetEntitySlot(0, LogoSetup->classID, NULL);
    UIPicture->aniFrames = RSDK.LoadSpriteAnimation("Logos/Logos.bin", SCOPE_STAGE);

    // What... is this for...?
    // LogoSetup is never in any other stages besides category 0, scene 0 (Logos) so this should never be used...
    // However it was even updated for plus which means it has to have been some sort of dev/debug feature

    if (SceneInfo->listPos >= 3) {
        switch (SceneInfo->listPos) {
            case 3: globals->playerID = ID_SONIC; break;
            case 4: globals->playerID = ID_TAILS; break;
            case 5: globals->playerID = ID_KNUCKLES; break;
#if MANIA_USE_PLUS
            case 6: globals->playerID = ID_MIGHTY; break;
            case 7: globals->playerID = ID_RAY; break;
#endif
            default: break;
        }
    }
}

bool32 LogoSetup_ImageCallback(void)
{
    if (LogoSetup->timer > 120
        && (ControllerInfo->keyA.press || ControllerInfo->keyB.press || ControllerInfo->keyStart.press || LogoSetup->timer >= 300)) {
        return true;
    }

    LogoSetup->timer++;
    return false;
}

void LogoSetup_State_CESAScreen(void)
{
    RSDK_THIS(LogoSetup);

    LogoSetup->timer = 0;
#if MANIA_USE_PLUS
    RSDK.LoadImage("CESA.png", 60.0, 2.0, LogoSetup_ImageCallback);
#else
    RSDK.LoadImage("CESA.tga", 60.0, 2.0, LogoSetup_ImageCallback);
#endif

    self->timer = 1024;
    self->state = LogoSetup_State_ShowLogos;
}

void LogoSetup_State_ShowLogos(void)
{
    RSDK_THIS(LogoSetup);

    if (self->timer <= 0) {
        if (!ScreenInfo->position.y)
            RSDK.PlaySfx(LogoSetup->sfxSega, false, 0xFF);

        self->timer     = 0;
        self->state     = LogoSetup_State_FadeToNextLogos;
        self->stateDraw = StateMachine_None;
    }
    else {
        self->timer -= 16;
    }
}

void LogoSetup_State_FadeToNextLogos(void)
{
    RSDK_THIS(LogoSetup);

    if (++self->timer > 120 || (self->timer > 30 && ControllerInfo->keyStart.press)) {
        self->timer     = 0;
        self->state     = LogoSetup_State_NextLogos;
        self->stateDraw = LogoSetup_Draw_Fade;
    }
}

void LogoSetup_State_NextLogos(void)
{
    RSDK_THIS(LogoSetup);

    if (self->timer >= 1024) {
        if (ScreenInfo->position.y >= SCREEN_YSIZE) {
            ++SceneInfo->listPos;
            RSDK.LoadScene();
        }
        else {
            ScreenInfo->position.y += SCREEN_YSIZE;
            self->state     = LogoSetup_State_ShowLogos;
            self->stateDraw = LogoSetup_Draw_Fade;
            self->timer     = 0x400;
        }
    }
    else {
        self->timer += 16;
    }
}

void LogoSetup_Draw_Fade(void)
{
    RSDK_THIS(LogoSetup);

    RSDK.FillScreen(0x000000, self->timer, self->timer - 128, self->timer - 256);
}

#if GAME_INCLUDE_EDITOR
void LogoSetup_EditorDraw(void) {}

void LogoSetup_EditorLoad(void) {}
#endif

void LogoSetup_Serialize(void) {}
