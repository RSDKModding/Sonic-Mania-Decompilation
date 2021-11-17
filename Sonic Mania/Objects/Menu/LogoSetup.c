#include "SonicMania.h"

ObjectLogoSetup *LogoSetup;

void LogoSetup_Update(void)
{
    RSDK_THIS(LogoSetup);
    StateMachine_Run(entity->state);
    ScreenInfo->position.x = 256 - ScreenInfo->centerX;
}

void LogoSetup_LateUpdate(void) {}

void LogoSetup_StaticUpdate(void) {}

void LogoSetup_Draw(void)
{
    RSDK_THIS(LogoSetup);
    StateMachine_Run(entity->stateDraw);
}

void LogoSetup_Create(void *data)
{
    RSDK_THIS(LogoSetup);
    if (!SceneInfo->inEditor) {
        entity->active    = ACTIVE_ALWAYS;
        entity->visible   = true;
        entity->drawOrder = 12;
        entity->stateDraw = LogoSetup_Unknown4;
        if (sku_region == REGION_JP)
            entity->state = LogoSetup_CESAScreen;
        else
            entity->state = LogoSetup_SegaScreen;
        entity->timer = 1024;
    }
}

void LogoSetup_StageLoad(void)
{
    LogoSetup->sfxSega = RSDK.GetSFX("Stage/Sega.wav");
    RSDK.ResetEntitySlot(0, LogoSetup->objectID, NULL);
    UIPicture->aniFrames = RSDK.LoadSpriteAnimation("Logos/Logos.bin", SCOPE_STAGE);

    if (SceneInfo->listPos > 2) {
        switch (SceneInfo->listPos) {
            case 3: globals->playerID = ID_SONIC; break;
            case 4: globals->playerID = ID_TAILS; break;
            case 5: globals->playerID = ID_KNUCKLES; break;
#if RETRO_USE_PLUS
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
void LogoSetup_CESAScreen(void)
{
    RSDK_THIS(LogoSetup);
    LogoSetup->timer = 0;
#if RETRO_USE_PLUS
    RSDK.LoadImage("CESA.png", 60.0, 2.0, LogoSetup_ImageCallback);
#else
    RSDK.LoadImage("CESA.tga", 60.0, 2.0, LogoSetup_ImageCallback);
#endif
    entity->timer = 1024;
    entity->state = LogoSetup_SegaScreen;
}
void LogoSetup_SegaScreen(void)
{
    RSDK_THIS(LogoSetup);
    if (entity->timer <= 0) {
        if (!ScreenInfo->position.y)
            RSDK.PlaySfx(LogoSetup->sfxSega, 0, 0xFF);
        entity->timer     = 0;
        entity->state     = LogoSetup_Unknown2;
        entity->stateDraw = 0;
    }
    else {
        entity->timer -= 16;
    }
}
void LogoSetup_Unknown2(void)
{
    RSDK_THIS(LogoSetup);
    ++entity->timer;
    if (+entity->timer > 120 || (entity->timer > 30 && ControllerInfo->keyStart.press)) {
        entity->timer     = 0;
        entity->state     = LogoSetup_Unknown3;
        entity->stateDraw = LogoSetup_Unknown4;
    }
}
void LogoSetup_Unknown3(void)
{
    RSDK_THIS(LogoSetup);
    if (entity->timer >= 1024) {
        if (ScreenInfo->position.y >= SCREEN_YSIZE) {
            ++SceneInfo->listPos;
            RSDK.LoadScene();
        }
        else {
            ScreenInfo->position.y += SCREEN_YSIZE;
            entity->state     = LogoSetup_SegaScreen;
            entity->stateDraw = LogoSetup_Unknown4;
            entity->timer     = 1024;
        }
    }
    else {
        entity->timer += 16;
    }
}
void LogoSetup_Unknown4(void)
{
    RSDK_THIS(LogoSetup);
    RSDK.FillScreen(0, entity->timer, entity->timer - 0x80, entity->timer - 0x100);
}

#if RETRO_INCLUDE_EDITOR
void LogoSetup_EditorDraw(void) {}

void LogoSetup_EditorLoad(void) {}
#endif

void LogoSetup_Serialize(void) {}
