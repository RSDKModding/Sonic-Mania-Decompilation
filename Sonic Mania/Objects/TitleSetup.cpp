#include "../SonicMania.hpp"

ObjectTitleSetup *TitleSetup;

void TitleSetup_Update()
{
    EntityTitleSetup *entity = (EntityTitleSetup *)RSDK_sceneInfo->entity;
    if (entity->state)
        entity->state();
}

void TitleSetup_LateUpdate() {}

void TitleSetup_StaticUpdate() {}

void TitleSetup_Draw()
{
    EntityTitleSetup *entity = (EntityTitleSetup *)RSDK_sceneInfo->entity;
    if (entity->stateDraw)
        entity->stateDraw();
}

void TitleSetup_Create(void *data)
{
    EntityTitleSetup *entity = (EntityTitleSetup *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(TitleSetup->spriteIndex, 0, &entity->data, true, 0);
        entity->active    = ACTIVE_ALWAYS;
        entity->visible   = true;
        entity->drawOrder = 12;
        entity->drawFX    = FX_FLIP;
        // entity->state          = TitleSetup_Unknown4;
        entity->stateDraw = TitleSetup_Unknown13;
        entity->timer     = 1024;
        entity->drawPos.x = 0x1000000;
        entity->drawPos.y = 0x6C0000;
    }
}

void TitleSetup_StageLoad()
{
    TextInfo presence;
    Localization_GetString(&presence, 0x43);
    User.SetRichPresence(6, &presence);
    User.Unknown27(0);
    options->blueSpheresInit = 0;
    Game_ClearOptions();
    User.Unknown16();
    User.Unknown23();
    options->saveLoaded = false;
    memset(options->saveRAM, 0, 0x10000);
    options->optionsLoaded = 0;
    memset(options->optionsRAM, 0, 0x200);
    User.Unknown30(options->replayTableID);
    options->replayTableID     = 0xFFFF;
    options->replayTableLoaded = false;
    User.Unknown30(options->taTableID);
    options->taTableID         = 0xFFFF;
    options->taTableLoaded     = false;
    TitleSetup->spriteIndex    = RSDK.LoadAnimation("Title/Electricity.bin", SCOPE_STAGE);
    TitleSetup->sfx_MenuBleep  = RSDK.GetSFX("Global/MenuBleep.wav");
    TitleSetup->sfx_MenuAccept = RSDK.GetSFX("Global/MenuAccept.wav");
    TitleSetup->sfx_Ring       = RSDK.GetSFX("Global/Ring.wav");
    RSDK.ResetEntitySlot(0, TitleSetup->objectID, 0);
}

void TitleSetup_HandleCheatInputs()
{
    byte keyState = 0;
    if (RSDK_controller->keyUp.press || RSDK_stickL->keyUp.press) {
        keyState = 1;
    }
    if (RSDK_controller->keyDown.press || RSDK_stickL->keyDown.press) {
        keyState = 2;
    }

    if (keyState) {
        TitleSetup->cheatCode[0] = TitleSetup->cheatCode[1];
        TitleSetup->cheatCode[1] = TitleSetup->cheatCode[2];
        TitleSetup->cheatCode[2] = TitleSetup->cheatCode[3];
        TitleSetup->cheatCode[3] = TitleSetup->cheatCode[4];
        TitleSetup->cheatCode[4] = TitleSetup->cheatCode[5];
        TitleSetup->cheatCode[5] = TitleSetup->cheatCode[6];
        TitleSetup->cheatCode[6] = TitleSetup->cheatCode[7];
        TitleSetup->cheatCode[7] = keyState;
    }
}

void TitleSetup_CheckCheatCode()
{
    TitleSetup_HandleCheatInputs();
    if (TitleSetup->cheatCode[0] == 1 && TitleSetup->cheatCode[1] == 1 && TitleSetup->cheatCode[2] == 2 && TitleSetup->cheatCode[3] == 2
        && TitleSetup->cheatCode[4] == 1 && TitleSetup->cheatCode[5] == 1 && TitleSetup->cheatCode[6] == 1 && TitleSetup->cheatCode[7] == 1) {
        if (!options->superSecret) {
            RSDK.PlaySFX(TitleSetup->sfx_Ring, 0, 255);
            options->superSecret = 1;
        }
    }
}
bool32 TitleSetup_IntroCallback(void)
{
    if (!RSDK_controller->keyA.press && !RSDK_controller->keyB.press && !RSDK_controller->keyStart.press)
        return false;
    RSDK.SoundUnknown1(Music->slotID);
    return true;
}

void TitleSetup_Unknown7()
{
    EntityTitleSetup *entity = (EntityTitleSetup *)RSDK_sceneInfo->entity;
    TitleSetup_CheckCheatCode();
    if (entity->timer <= 0) {
        entity->stateDraw = NULL;
        // if (User.CheckDLC(DLC_PLUS))
        //    entity->state = TitleSetup_SetupLogo_Plus;
        // else
        //    entity->state = TitleSetup_SetupLogo_NoPlus;
    }
    else {
        entity->timer -= 16;
    }
}

void TitleSetup_Unknown10()
{
    EntityTitleSetup *entity = (EntityTitleSetup *)RSDK_sceneInfo->entity;
    bool32 skipped = RSDK_controller->keyA.press || RSDK_controller->keyB.press || RSDK_controller->keyC.press || RSDK_controller->keyX.press
                     || RSDK_controller->keyY.press || RSDK_controller->keyZ.press | RSDK_controller->keyStart.press
                     || RSDK_controller->keyStart.press || RSDK_controller->keySelect.press;
    bool32 skipped2 = (!RSDK_touchMouse->count && entity->touched) | RSDK_unknown->field_28;
    entity->touched = RSDK_touchMouse->count > 0;
    if (skipped2 || skipped) {
        RSDK.PlaySFX(TitleSetup->sfx_MenuAccept, 0, 255);
        entity->timer = 0;
        RSDK.LoadScene("Presentation", "Menu");
        RSDK.ResetControllerAssignments();
        RSDK.AssignControllerID(1, RSDK.MostRecentActiveControllerID(0, 0, 5));
        RSDK.SoundUnknown1(Music->slotID);
        entity->state     = TitleSetup_Unknown11;
        entity->stateDraw = TitleSetup_Unknown13;
    }
    else if (++entity->timer == 800) {
        entity->timer     = 0;
        entity->state     = TitleSetup_Unknown12;
        entity->stateDraw = TitleSetup_Unknown13;
    }
}

void TitleSetup_Unknown11()
{
    EntityTitleSetup *entity = (EntityTitleSetup *)RSDK_sceneInfo->entity;

    if (entity->timer >= 1024) {
        RSDK.InitSceneLoad();
    }
    else {
        entity->timer += 8;
    }
}

void TitleSetup_Unknown12()
{
    EntityTitleSetup *entity = (EntityTitleSetup *)RSDK_sceneInfo->entity;
    if (entity->timer >= 1024) {
        RSDK.InitSceneLoad();
        RSDK.SoundUnknown1(Music->slotID);
        if (TitleSetup->altMusic) {
            RSDK.PlayMusic("IntroTee.ogg", Music->slotID, 0, 0, 0);
            // RSDK.LoadVideo("Mania.ogv", qword_661940, *(&qword_661940 + 1), TitleSetup_Unknown3);
            TitleSetup->altMusic = 0;
        }
        else {
            RSDK.PlayMusic("IntroHP.ogg", Music->slotID, 0, 0, 0);
            // RSDK.LoadVideo("Mania.ogv", 0, 0, TitleSetup_Unknown3);
            TitleSetup->altMusic = true;
        }
    }
    else {
        entity->timer += 8;
    }
}

void TitleSetup_Unknown13()
{
    EntityTitleSetup *entity = (EntityTitleSetup *)RSDK_sceneInfo->entity;
    RSDK.FillScreen(0x000000, entity->timer, entity->timer - 128, entity->timer - 256);
}

void TitleSetup_Unknown14()
{
    EntityTitleSetup *entity = (EntityTitleSetup *)RSDK_sceneInfo->entity;
    entity->direction        = FLIP_NONE;
    RSDK.DrawSprite(&entity->data, &entity->drawPos, 0);
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->data, &entity->drawPos, 0);
}

void TitleSetup_Unknown15()
{
    EntityTitleSetup *entity = (EntityTitleSetup *)RSDK_sceneInfo->entity;
    RSDK.FillScreen(0xF0F0F0, entity->timer, entity->timer - 128, entity->timer - 256);
}

void TitleSetup_EditorDraw() {}

void TitleSetup_EditorLoad() {}

void TitleSetup_Serialize() {}
