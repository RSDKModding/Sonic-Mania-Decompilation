#include "../SonicMania.h"

ObjectTitleSetup *TitleSetup;

void TitleSetup_Update()
{
    RSDK_THIS(TitleSetup);
    if (entity->state)
        entity->state();
}

void TitleSetup_LateUpdate() {}

void TitleSetup_StaticUpdate() {}

void TitleSetup_Draw()
{
    RSDK_THIS(TitleSetup);
    if (entity->stateDraw)
        entity->stateDraw();
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
        entity->state          = TitleSetup_Unknown4;
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
    User.SetUserStorageNoSave(0);
    globals->blueSpheresInit = 0;
    Game_ClearOptions();
    User.UserStorageUnknown8();
    User.ClearUserStorageStatus();
    globals->saveLoaded = false;
    memset(globals->saveRAM, 0, 0x10000);
    globals->optionsLoaded = 0;
    memset(globals->optionsRAM, 0, 0x200);
    User.ClearUserDB(globals->replayTableID);
    globals->replayTableID     = -1;
    globals->replayTableLoaded = false;
    User.ClearUserDB(globals->taTableID);
    globals->taTableID         = -1;
    globals->taTableLoaded     = false;
    TitleSetup->spriteIndex    = RSDK.LoadSpriteAnimation("Title/Electricity.bin", SCOPE_STAGE);
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
        if (!globals->superSecret) {
            RSDK.PlaySFX(TitleSetup->sfx_Ring, 0, 255);
            globals->superSecret = 1;
        }
    }
}
bool32 TitleSetup_IntroCallback()
{
    if (!RSDK_controller->keyA.press && !RSDK_controller->keyB.press && !RSDK_controller->keyStart.press)
        return false;
    RSDK.SoundUnknown1(Music->slotID);
    return true;
}

void TitleSetup_Unknown4()
{
    RSDK_THIS(TitleSetup);
    if (entity->timer <= -0x400) {
        entity->timer     = 0;
        entity->state     = TitleSetup_Unknown5;
        entity->stateDraw = TitleSetup_Unknown14;
        Music_PlayTrack(0);
    }
    else {
        entity->timer -= 16;
    }
}

void TitleSetup_Unknown5()
{
    RSDK_THIS(TitleSetup);
    RSDK.ProcessAnimation(&entity->data);
    if (entity->data.frameID == 31) {
        EntityTitleLogo *titleLogo = NULL;
        while (RSDK.GetEntities(TitleLogo->objectID, (Entity **)&titleLogo)) {
            if (titleLogo->type >= 0) {
                if (titleLogo->type <= 1) {
                    titleLogo->active  = ACTIVE_NORMAL;
                    titleLogo->visible = true;
                }
                else if (titleLogo->type == 3) {
                    RSDK.ResetEntityPtr(titleLogo, 0, 0);
                }
            }
        }
        entity->state = TitleSetup_Unknown6;
    }
}

void TitleSetup_Unknown6()
{
    RSDK_THIS(TitleSetup);

    RSDK.ProcessAnimation(&entity->data);
    if (entity->data.frameID == entity->data.frameCount - 1) {
        EntityTitleLogo *titleLogo = NULL;
        while (RSDK.GetEntities(TitleLogo->objectID, (Entity **)&titleLogo)) {
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

        Entity *titleSonic = NULL;
        while (RSDK.GetEntities(TitleSonic->objectID, &titleSonic)) {
            titleSonic->active = ACTIVE_NORMAL;
            titleSonic->visible = true;
        }
        
        TitleBG_SetupFX();
        entity->timer     = 0x300;
        entity->state     = TitleSetup_Unknown7;
        entity->stateDraw = TitleSetup_Unknown15;
    }
}

void TitleSetup_Unknown7()
{
    RSDK_THIS(TitleSetup);
    TitleSetup_CheckCheatCode();
    if (entity->timer <= 0) {
        entity->stateDraw = NULL;
        if (User.CheckDLC(DLC_PLUS))
           entity->state = TitleSetup_SetupLogo_Plus;
        else
           entity->state = TitleSetup_SetupLogo_NoPlus;
    }
    else {
        entity->timer -= 16;
    }
}

void TitleSetup_SetupLogo_NoPlus()
{
    RSDK_THIS(TitleSetup);
    if (entity->timer < 120)
        TitleSetup_CheckCheatCode();
    if (++entity->timer == 120) {
        EntityTitleLogo *titleLogo = NULL;
        while (RSDK.GetEntities(TitleLogo->objectID, (Entity **)&titleLogo)) {
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
void TitleSetup_SetupLogo_Plus()
{
    RSDK_THIS(TitleSetup);
    if (entity->timer < 120)
        TitleSetup_CheckCheatCode();
    if (++entity->timer == 120) {
        EntityTitleLogo *titleLogo = NULL;
        while (RSDK.GetEntities(TitleLogo->objectID, (Entity **)&titleLogo)) {
            switch (titleLogo->type) {
                case 1:
                case 2:
                    titleLogo->storeY     = titleLogo->position.y - 0x70000;
                    titleLogo->velocity.y = -0x30000;
                    titleLogo->timer      = 2;
                    titleLogo->state      = TitleLogo_Unknown4;
                    RSDK.PlaySFX(TitleLogo->sfx_Plus, 0, 255);
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
        
        RSDK.CreateEntity(TitleEggman->objectID, 0, 0, 0xC00000);
        entity->timer = 0;
        entity->state = TitleSetup_SetupLogo_NoPlus;
    }
}

void TitleSetup_Unknown10()
{
    RSDK_THIS(TitleSetup);
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
    RSDK_THIS(TitleSetup);
    if (entity->timer >= 1024) {
        RSDK.InitSceneLoad();
    }
    else {
        entity->timer += 8;
    }
}

void TitleSetup_Unknown12()
{
    RSDK_THIS(TitleSetup);
    if (entity->timer >= 1024) {
        RSDK.InitSceneLoad();
        RSDK.SoundUnknown1(Music->slotID);
        if (TitleSetup->altMusic) {
            RSDK.PlayMusic("IntroTee.ogg", Music->slotID, 0, 0, 0);
            RSDK.LoadVideo("Mania.ogv", 1.8, TitleSetup_IntroCallback);
            TitleSetup->altMusic = 0;
        }
        else {
            RSDK.PlayMusic("IntroHP.ogg", Music->slotID, 0, 0, 0);
            RSDK.LoadVideo("Mania.ogv", 0, TitleSetup_IntroCallback);
            TitleSetup->altMusic = true;
        }
    }
    else {
        entity->timer += 8;
    }
}

void TitleSetup_Unknown13()
{
    RSDK_THIS(TitleSetup);
    RSDK.FillScreen(0x000000, entity->timer, entity->timer - 128, entity->timer - 256);
}

void TitleSetup_Unknown14()
{
    RSDK_THIS(TitleSetup);
    entity->direction        = FLIP_NONE;
    RSDK.DrawSprite(&entity->data, &entity->drawPos, 0);
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->data, &entity->drawPos, 0);
}

void TitleSetup_Unknown15()
{
    RSDK_THIS(TitleSetup);
    RSDK.FillScreen(0xF0F0F0, entity->timer, entity->timer - 128, entity->timer - 256);
}

void TitleSetup_EditorDraw() {}

void TitleSetup_EditorLoad() {}

void TitleSetup_Serialize() {}
