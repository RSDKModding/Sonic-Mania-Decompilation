#include "../SonicMania.hpp"

ObjectLogoSetup *LogoSetup;

void LogoSetup_Update()
{
    EntityLogoSetup *entity = (EntityLogoSetup *)RSDK_sceneInfo->entity;
    if (entity->state)
        entity->state();
}

void LogoSetup_LateUpdate()
{

}

void LogoSetup_StaticUpdate()
{

}

void LogoSetup_Draw()
{
    EntityLogoSetup *entity = (EntityLogoSetup *)RSDK_sceneInfo->entity;
    if (entity->stateDraw)
        entity->stateDraw();
}

void LogoSetup_Create(void* data)
{
    EntityLogoSetup *entity = (EntityLogoSetup *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        entity->priority  = ACTIVE_ALWAYS;
        entity->visible   = 1;
        entity->drawOrder = 12;
        entity->stateDraw      = LogoSetup_Unknown4;
        if (RSDK_sku->region == 1)
            entity->state = LogoSetup_CESAScreen;
        else
            entity->state = LogoSetup_SegaScreen;
        entity->timer = 1024;
    }
}

void LogoSetup_StageLoad()
{
    LogoSetup->sfx_Sega = RSDK.GetSFX("Stage/Sega.wav");
    RSDK.ResetEntity(0, LogoSetup->objectID, 0);
    UIPicture->spriteIndex = RSDK.LoadAnimation("Logos/Logos.bin", SCOPE_STAGE);

    if (RSDK_sceneInfo->listPos > 2) {
        switch (RSDK_sceneInfo->listPos) {
            case 3: options->playerID = ID_SONIC; break;
            case 4: options->playerID = ID_TAILS; break;
            case 5: options->playerID = ID_KNUCKLES; break;
            case 6: options->playerID = ID_MIGHTY; break;
            case 7: options->playerID = ID_RAY; break;
            default: break;
        }
    }
}

bool32 LogoSetup_PNGCallback()
{
    if (LogoSetup->timer > 120
        && (RSDK_controller->keyA.press || RSDK_controller->keyB.press || RSDK_controller->keyStart.press || LogoSetup->timer >= 300)) {
        return true;
    }
    LogoSetup->timer++;
    return false;
}
void LogoSetup_CESAScreen()
{
    EntityLogoSetup *entity = (EntityLogoSetup *)RSDK_sceneInfo->entity;
    LogoSetup->timer = 0;
    //RSDK.LoadPNG("CESA.png", xmmword_661DB0, *(&xmmword_661DB0 + 1), *(&xmmword_661DB0 + 2), *(&xmmword_661DB0 + 3), LogoSetup_PNGCallback);
    entity->timer = 1024;
    entity->state = LogoSetup_SegaScreen;
}
void LogoSetup_SegaScreen()
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
void LogoSetup_Unknown2()
{
    EntityLogoSetup *entity = (EntityLogoSetup *)RSDK_sceneInfo->entity;
    
    ++entity->timer;
    if (+entity->timer > 120 || (entity->timer > 30 && RSDK_controller->keyStart.press)) {
        entity->timer     = 0;
        entity->state     = LogoSetup_Unknown3;
        entity->stateDraw = LogoSetup_Unknown4;
    }
}
void LogoSetup_Unknown3()
{
    EntityLogoSetup *entity = (EntityLogoSetup *)RSDK_sceneInfo->entity;

    if (entity->timer >= 1024) {
        if (RSDK_screens->position.y >= SCREEN_YSIZE) {
            ++RSDK_sceneInfo->listPos;
            RSDK.InitSceneLoad();
        }
        else {
            RSDK_screens->position.y += SCREEN_YSIZE;
            entity->state            = LogoSetup_SegaScreen;
            entity->stateDraw        = LogoSetup_Unknown4;
            entity->timer            = 1024;
        }
    }
    else {
        entity->timer += 16;
    }
}
void LogoSetup_Unknown4()
{
    EntityLogoSetup *entity = (EntityLogoSetup *)RSDK_sceneInfo->entity;
    RSDK.FillScreen(0, entity->timer, entity->timer - 0x80, entity->timer - 0x100);
}

void LogoSetup_EditorDraw()
{

}

void LogoSetup_EditorLoad()
{

}

void LogoSetup_Serialize()
{

}

