#include "../SonicMania.h"

ObjectLogoSetup *LogoSetup;

void LogoSetup_Update(void)
{
    EntityLogoSetup *entity = (EntityLogoSetup *)RSDK_sceneInfo->entity;
    if (entity->state)
        entity->state();
    RSDK_screens->position.x = 256 - RSDK_screens->centerX;
}

void LogoSetup_LateUpdate(void) {}

void LogoSetup_StaticUpdate(void) {}

void LogoSetup_Draw(void)
{
    EntityLogoSetup *entity = (EntityLogoSetup *)RSDK_sceneInfo->entity;
    if (entity->stateDraw)
        entity->stateDraw();
}

void LogoSetup_Create(void *data)
{
    EntityLogoSetup *entity = (EntityLogoSetup *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_ALWAYS;
        entity->visible   = true;
        entity->drawOrder = 12;
        entity->stateDraw = LogoSetup_Unknown4;
#if RETRO_USE_PLUS
        if (RSDK_sku->region == REGION_JP)
#else
        if (RSDK_info->region == REGION_JP)
#endif
            entity->state = LogoSetup_CESAScreen;
        else
            entity->state = LogoSetup_SegaScreen;
        entity->timer = 1024;
    }
}

void LogoSetup_StageLoad(void)
{
    LogoSetup->sfx_Sega = RSDK.GetSFX("Stage/Sega.wav");
    RSDK.ResetEntitySlot(0, LogoSetup->objectID, 0);
    UIPicture->spriteIndex = RSDK.LoadSpriteAnimation("Logos/Logos.bin", SCOPE_STAGE);

    if (RSDK_sceneInfo->listPos > 2) {
        switch (RSDK_sceneInfo->listPos) {
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

bool32 LogoSetup_PNGCallback(void)
{
    if (LogoSetup->timer > 120
        && (RSDK_controller->keyA.press || RSDK_controller->keyB.press || RSDK_controller->keyStart.press || LogoSetup->timer >= 300)) {
        return true;
    }
    LogoSetup->timer++;
    return false;
}
void LogoSetup_CESAScreen(void)
{
    EntityLogoSetup *entity = (EntityLogoSetup *)RSDK_sceneInfo->entity;
    LogoSetup->timer        = 0;
    RSDK.LoadImage("CESA.png", 2.0, 60.0, LogoSetup_PNGCallback);
    entity->timer = 1024;
    entity->state = LogoSetup_SegaScreen;
}
void LogoSetup_SegaScreen(void)
{
    EntityLogoSetup *entity = (EntityLogoSetup *)RSDK_sceneInfo->entity;

    if (entity->timer <= 0) {
        if (!RSDK_screens->position.y)
            RSDK.PlaySFX(LogoSetup->sfx_Sega, 0, 0xFF);
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
    EntityLogoSetup *entity = (EntityLogoSetup *)RSDK_sceneInfo->entity;

    ++entity->timer;
    if (+entity->timer > 120 || (entity->timer > 30 && RSDK_controller->keyStart.press)) {
        entity->timer     = 0;
        entity->state     = LogoSetup_Unknown3;
        entity->stateDraw = LogoSetup_Unknown4;
    }
}
void LogoSetup_Unknown3(void)
{
    EntityLogoSetup *entity = (EntityLogoSetup *)RSDK_sceneInfo->entity;

    if (entity->timer >= 1024) {
        if (RSDK_screens->position.y >= SCREEN_YSIZE) {
            ++RSDK_sceneInfo->listPos;
            RSDK.InitSceneLoad();
        }
        else {
            RSDK_screens->position.y += SCREEN_YSIZE;
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
    EntityLogoSetup *entity = (EntityLogoSetup *)RSDK_sceneInfo->entity;
    RSDK.FillScreen(0, entity->timer, entity->timer - 0x80, entity->timer - 0x100);
}

void LogoSetup_EditorDraw(void) {}

void LogoSetup_EditorLoad(void) {}

void LogoSetup_Serialize(void) {}
