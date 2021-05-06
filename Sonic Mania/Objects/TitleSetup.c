#include "../SonicMania.h"

ObjectTitleSetup *TitleSetup;

void TitleSetup_Update(void)
{
    RSDK_THIS(TitleSetup);
    StateMachine_Run(entity->state);
    RSDK_screens->position.x = 0x100 - RSDK_screens->centerX;
}

void TitleSetup_LateUpdate(void) {}

void TitleSetup_StaticUpdate(void) {}

void TitleSetup_Draw(void)
{
    RSDK_THIS(TitleSetup);
    StateMachine_Run(entity->stateDraw);
}

void TitleSetup_Create(void *data)
{
    RSDK_THIS(TitleSetup);
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(TitleSetup->spriteIndex, 0, &entity->data, true, 0);
        entity->active    = ACTIVE_ALWAYS;
        entity->visible   = true;
        entity->drawOrder = 12;
        entity->drawFX    = FX_FLIP;
        entity->state     = TitleSetup_Unknown4;
        entity->stateDraw = TitleSetup_Unknown13;
        entity->timer     = 1024;
        entity->drawPos.x = 0x1000000;
        entity->drawPos.y = 0x6C0000;
    }
}

void TitleSetup_StageLoad(void)
{
    TextInfo presence;
    Localization_GetString(&presence, STR_RPC_TITLE);
#if RETRO_USE_PLUS
    User.SetRichPresence(PRESENCE_TITLE, &presence);
    User.SetUserStorageNoSave(false);
#else
    APICallback_SetRichPresence(PRESENCE_TITLE, &presence);
    globals->noSave = false;
#endif
    globals->blueSpheresInit = false;
    TimeAttackData_ClearOptions();
#if RETRO_USE_PLUS
    User.UserStorageUnknown8();
    User.ClearUserStorageStatus();
#else
    APICallback_ClearPrerollErrors();
#endif
    globals->saveLoaded = false;
    memset(globals->saveRAM, 0, 0x10000);
    globals->optionsLoaded = 0;
    memset(globals->optionsRAM, 0, 0x200);
#if RETRO_USE_PLUS
    User.ClearUserDB(globals->replayTableID);
    globals->replayTableID     = -1;
    globals->replayTableLoaded = false;
    User.ClearUserDB(globals->taTableID);
    globals->taTableID     = -1;
    globals->taTableLoaded = false;
#endif
    TitleSetup->spriteIndex    = RSDK.LoadSpriteAnimation("Title/Electricity.bin", SCOPE_STAGE);
    TitleSetup->sfx_MenuBleep  = RSDK.GetSFX("Global/MenuBleep.wav");
    TitleSetup->sfx_MenuAccept = RSDK.GetSFX("Global/MenuAccept.wav");
    TitleSetup->sfx_Ring       = RSDK.GetSFX("Global/Ring.wav");
    RSDK.ResetEntitySlot(0, TitleSetup->objectID, 0);
}

void TitleSetup_HandleCheatInputs(void)
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

void TitleSetup_CheckCheatCode(void)
{
    TitleSetup_HandleCheatInputs();
    if (TitleSetup->cheatCode[0] == 1 && TitleSetup->cheatCode[1] == 1 && TitleSetup->cheatCode[2] == 2 && TitleSetup->cheatCode[3] == 2
        && TitleSetup->cheatCode[4] == 1 && TitleSetup->cheatCode[5] == 1 && TitleSetup->cheatCode[6] == 1 && TitleSetup->cheatCode[7] == 1) {
#if RETRO_USE_PLUS
        if (!globals->superSecret) {
            RSDK.PlaySFX(TitleSetup->sfx_Ring, 0, 255);
            globals->superSecret = true;
        }
#endif
    }
}
bool32 TitleSetup_IntroCallback(void)
{
    if (!RSDK_controller->keyA.press && !RSDK_controller->keyB.press && !RSDK_controller->keyStart.press)
        return false;
    RSDK.StopChannel(Music->channelID);
    return true;
}

void TitleSetup_Unknown4(void)
{
    RSDK_THIS(TitleSetup);
    if (entity->timer <= -0x400) {
        entity->timer     = 0;
        entity->state     = TitleSetup_Unknown5;
        entity->stateDraw = TitleSetup_Unknown14;
        Music_PlayTrack(TRACK_STAGE);
    }
    else {
        entity->timer -= 16;
    }
}

void TitleSetup_Unknown5(void)
{
    RSDK_THIS(TitleSetup);
    RSDK.ProcessAnimation(&entity->data);
    if (entity->data.frameID == 31) {
        foreach_all(TitleLogo, titleLogo)
        {
            if (titleLogo->type >= 0) {
                if (titleLogo->type <= 1) {
                    titleLogo->active  = ACTIVE_NORMAL;
                    titleLogo->visible = true;
                }
                else if (titleLogo->type == 3) {
                    RSDK.ResetEntityPtr(titleLogo, TYPE_BLANK, NULL);
                }
            }
        }
        entity->state = TitleSetup_Unknown6;
    }
}

void TitleSetup_Unknown6(void)
{
    RSDK_THIS(TitleSetup);

    RSDK.ProcessAnimation(&entity->data);
    if (entity->data.frameID == entity->data.frameCount - 1) {
        foreach_all(TitleLogo, titleLogo)
        {
            if (titleLogo->type == 7) {
                titleLogo->position.y -= 0x200000;
            }
            else if (titleLogo->type != 6) {
                titleLogo->active  = ACTIVE_NORMAL;
                titleLogo->visible = true;
            }
            if (titleLogo->type == 1) {
                titleLogo->flag = true;
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 2, &titleLogo->data1, true, 0);
            }
        }

        foreach_all(TitleSonic, titleSonic)
        {
            titleSonic->active  = ACTIVE_NORMAL;
            titleSonic->visible = true;
        }

        TitleBG_SetupFX();
        entity->timer     = 0x300;
        entity->state     = TitleSetup_Unknown7;
        entity->stateDraw = TitleSetup_Unknown15;
    }
}

void TitleSetup_Unknown7(void)
{
    RSDK_THIS(TitleSetup);
    TitleSetup_CheckCheatCode();
    if (entity->timer <= 0) {
        entity->stateDraw = StateMachine_None;
#if RETRO_USE_PLUS
        if (User.CheckDLC(DLC_PLUS))
            entity->state = TitleSetup_SetupLogo_Plus;
        else
#endif
            entity->state = TitleSetup_SetupLogo_NoPlus;
    }
    else {
        entity->timer -= 16;
    }
}

void TitleSetup_SetupLogo_NoPlus(void)
{
    RSDK_THIS(TitleSetup);
    if (entity->timer < 120)
        TitleSetup_CheckCheatCode();
    if (++entity->timer == 120) {
        foreach_all(TitleLogo, titleLogo)
        {
            if (titleLogo->type == 6) {
                titleLogo->active      = ACTIVE_NORMAL;
                titleLogo->visible     = true;
                Entity *store          = RSDK_sceneInfo->entity;
                RSDK_sceneInfo->entity = (Entity *)titleLogo;
                TitleLogo_Unknown1();
                RSDK_sceneInfo->entity = store;
            }
        }
        entity->timer = 0;
        entity->state = TitleSetup_Unknown10;
    }
}
void TitleSetup_SetupLogo_Plus(void)
{
    RSDK_THIS(TitleSetup);
    if (entity->timer < 120)
        TitleSetup_CheckCheatCode();
    if (++entity->timer == 120) {
        foreach_all(TitleLogo, titleLogo)
        {
            switch (titleLogo->type) {
                case 1:
                case 2:
                    titleLogo->storeY     = titleLogo->position.y - 0x70000;
                    titleLogo->velocity.y = -0x30000;
                    titleLogo->timer      = 2;
                    titleLogo->state      = TitleLogo_Unknown4;
#if RETRO_USE_PLUS
                    RSDK.PlaySFX(TitleLogo->sfx_Plus, 0, 255);
#endif
                    break;
                case 6: titleLogo->position.y += 0x80000; break;
                case 7:
                    titleLogo->active  = ACTIVE_NORMAL;
                    titleLogo->visible = true;
                    titleLogo->timer   = 2;
                    titleLogo->position.y -= 0x40000;
                    titleLogo->state = TitleLogo_Unknown4;
                    break;
                default: break;
            }
        }

#if RETRO_USE_PLUS
        RSDK.CreateEntity(TitleEggman->objectID, 0, 0, 0xC00000);
#endif
        entity->timer = 0;
        entity->state = TitleSetup_SetupLogo_NoPlus;
    }
}

void TitleSetup_Unknown10(void)
{
    RSDK_THIS(TitleSetup);
    bool32 skipped = RSDK_controller->keyA.press || RSDK_controller->keyB.press || RSDK_controller->keyC.press || RSDK_controller->keyX.press
                     || RSDK_controller->keyY.press || RSDK_controller->keyZ.press | RSDK_controller->keyStart.press
                     || RSDK_controller->keyStart.press || RSDK_controller->keySelect.press;
    bool32 skipped2 = (!RSDK_touchMouse->count && entity->touched)
#if RETRO_USE_PLUS
                      || RSDK_unknown->field_28;
#else
        ;
#endif
    entity->touched = RSDK_touchMouse->count > 0;
    if (skipped2 || skipped) {
        RSDK.PlaySFX(TitleSetup->sfx_MenuAccept, 0, 255);
        entity->timer = 0;
        RSDK.LoadScene("Presentation", "Menu");
#if RETRO_USE_PLUS
        RSDK.ResetControllerAssignments();
        RSDK.AssignControllerID(CONT_P1, RSDK.MostRecentActiveControllerID(0, 0, 5));
#else

#endif
        RSDK.StopChannel(Music->channelID);
        entity->state     = TitleSetup_Unknown11;
        entity->stateDraw = TitleSetup_Unknown13;
    }
    else if (++entity->timer == 800) {
        entity->timer     = 0;
        entity->state     = TitleSetup_Unknown12;
        entity->stateDraw = TitleSetup_Unknown13;
    }
}

void TitleSetup_Unknown11(void)
{
    RSDK_THIS(TitleSetup);
    if (entity->timer >= 1024) {
        RSDK.InitSceneLoad();
    }
    else {
        entity->timer += 8;
    }
}

void TitleSetup_Unknown12(void)
{
    RSDK_THIS(TitleSetup);
    if (entity->timer >= 1024) {
        RSDK.InitSceneLoad();
        RSDK.StopChannel(Music->channelID);
        if (TitleSetup->altMusic) {
            RSDK.PlayStream("IntroTee.ogg", Music->channelID, 0, 0, false);
            RSDK.LoadVideo("Mania.ogv", 1.8, TitleSetup_IntroCallback);
            TitleSetup->altMusic = 0;
        }
        else {
            RSDK.PlayStream("IntroHP.ogg", Music->channelID, 0, 0, false);
            RSDK.LoadVideo("Mania.ogv", 0, TitleSetup_IntroCallback);
            TitleSetup->altMusic = true;
        }
    }
    else {
        entity->timer += 8;
    }
}

void TitleSetup_Unknown13(void)
{
    RSDK_THIS(TitleSetup);
    RSDK.FillScreen(0x000000, entity->timer, entity->timer - 128, entity->timer - 256);
}

void TitleSetup_Unknown14(void)
{
    RSDK_THIS(TitleSetup);
    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->data, &entity->drawPos, 0);
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->data, &entity->drawPos, 0);
}

void TitleSetup_Unknown15(void)
{
    RSDK_THIS(TitleSetup);
    RSDK.FillScreen(0xF0F0F0, entity->timer, entity->timer - 128, entity->timer - 256);
}

void TitleSetup_EditorDraw(void) {}

void TitleSetup_EditorLoad(void) {}

void TitleSetup_Serialize(void) {}
