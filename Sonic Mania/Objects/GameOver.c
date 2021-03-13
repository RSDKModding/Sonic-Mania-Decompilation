#include "../SonicMania.h"

ObjectGameOver *GameOver;

void GameOver_Update()
{
    EntityGameOver *entity = (EntityGameOver *)RSDK_sceneInfo->entity;
    if (entity->state)
        entity->state();
}

void GameOver_LateUpdate() {}

void GameOver_StaticUpdate() {}

void GameOver_Draw()
{
    EntityGameOver *entity = (EntityGameOver *)RSDK_sceneInfo->entity;
    if (entity->state != GameOver_Unknown5 && options->gameMode == MODE_COMPETITION) {
        if (RSDK_sceneInfo->currentScreenID != entity->playerID || RSDK.GetEntityCount(PauseMenu->objectID, true) > 0) {
            return;
        }
    }
    EntityPlayer *player = (EntityPlayer *)RSDK.GetEntityByID(RSDK_sceneInfo->currentScreenID + Player->playerCount);
    if (RSDK_sceneInfo->currentScreenID == entity->playerID || player->objectID != GameOver->objectID) {
        RSDK.DrawQuad(entity->verts, 4, 0, 0, 0, 255, INK_NONE);
    }

    if (options->gameMode != MODE_COMPETITION || RSDK_sceneInfo->currentScreenID == entity->playerID) {
        entity->data.frameID = 0;
        for (entity->data.frameID = 0; entity->data.frameID < 8; ++entity->data.frameID) {
            entity->rotation = entity->letterRotations[entity->data.frameID];
            RSDK.DrawSprite(&entity->data, &entity->letterPositions[entity->data.frameID], true);
        }
    }
}

void GameOver_Create(void *data)
{
    EntityGameOver *entity = (EntityGameOver *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active  = ACTIVE_ALWAYS;
        entity->visible = 1;
        entity->drawFX  = 6;
        if (data)
            RSDK.SetSpriteAnimation(GameOver->spriteIndex, 7, &entity->data, true, 1);
        else
            RSDK.SetSpriteAnimation(GameOver->spriteIndex, 6, &entity->data, true, 0);

        ScreenInfo *screen       = RSDK_screens;
        entity->unknownPos1[0].x = -0x480000;
        entity->unknownPos1[1].x = -0x370000;
        entity->unknownPos1[2].x = -0x260000;
        entity->unknownPos1[3].x = -0x150000;
        entity->unknownPos1[4].x = 0xC0000;
        entity->unknownPos1[5].x = 0x1D0000;
        entity->unknownPos1[6].x = 0x2E0000;
        entity->unknownPos1[7].x = 0x3F0000;

        entity->letterPositions[0].y = -0x200000;
        entity->letterPosMove[0].x   = -(entity->unknownPos1[0].x >> 4);
        entity->letterPositions[0].x = 8 * ((screen->centerX << 13) + entity->unknownPos1[0].x);
        entity->letterPosMove[0].y   = 0x2000;
        entity->unknownPos1[0].y     = (screen->centerY - 4) << 16;
        entity->letterPositions[1].y = -0x300000;
        entity->letterPosMove[1].x   = -(entity->unknownPos1[1].x >> 4);
        entity->letterPositions[1].x = 8 * ((screen->centerX << 13) + (screen->centerX << 13) + entity->unknownPos1[0].x);
        entity->letterPosMove[1].y   = 0x2000;
        entity->unknownPos1[1].y     = (screen->centerY - 4) << 16;
        entity->letterPositions[2].y = -0x400000;
        entity->letterPosMove[2].x   = -(entity->unknownPos1[2].x >> 4);
        entity->letterPositions[2].x = 8 * ((screen->centerX << 13) + entity->unknownPos1[2].x);
        entity->letterPosMove[2].y   = 0x2000;
        entity->unknownPos1[2].y     = (screen->centerY - 4) << 16;
        entity->letterPositions[3].y = -0x500000;
        entity->letterPosMove[3].x   = -(entity->unknownPos1[3].x >> 4);
        entity->letterPositions[3].x = 8 * (screen->centerX << 13) + (entity->unknownPos1[3].x);
        entity->letterPosMove[3].y   = 0x2000;
        entity->unknownPos1[3].y     = (screen->centerY - 4) << 16;
        entity->letterPositions[4].y = -0x600000;
        entity->letterPosMove[4].y   = 0x2000;
        entity->letterPosMove[4].x   = -(entity->unknownPos1[4].x >> 4);
        entity->letterPositions[4].x = 8 * ((screen->centerX << 13) + entity->unknownPos1[4].x);
        entity->unknownPos1[4].y     = (screen->centerY - 4) << 16;
        entity->letterPositions[5].y = -0x700000;
        entity->letterPosMove[5].y   = 0x2000;
        entity->letterPosMove[5].x   = -(entity->unknownPos1[5].x >> 4);
        entity->letterPositions[5].x = 8 * ((screen->centerX << 13) + entity->unknownPos1[5].x);
        entity->unknownPos1[5].y     = (screen->centerY - 4) << 16;
        entity->letterPositions[6].y = -0x800000;
        entity->letterPosMove[6].y   = 0x2000;
        entity->letterPosMove[6].x   = -(entity->unknownPos1[6].x >> 4);
        entity->letterPositions[6].x = 8 * ((screen->centerX << 13) + entity->unknownPos1[6].x);
        entity->unknownPos1[6].y     = (screen->centerY - 4) << 16;
        entity->letterPositions[7].y = -0x900000;
        entity->letterPosMove[7].y   = 0x2000;
        entity->letterPosMove[7].x   = -(entity->unknownPos1[7].x >> 4);
        entity->letterPositions[7].x = 8 * ((screen->centerX << 13) + entity->unknownPos1[7].x);
        entity->unknownPos1[7].y     = (screen->centerY - 4) << 16;
        entity->dword64              = 0x1000000;
        entity->dword68              = screen->centerY << 16;
        entity->scale.x              = 0x800;
        entity->state                = GameOver_Unknown2;
        entity->drawOrder            = Zone->uiDrawHigh + 1;

        EntityHUD *hud = NULL;
        while (RSDK.GetEntities(HUD->objectID, (Entity **)&hud)) {
            // hud->state  = HUD_Unknown6;
            // hud->active = ACTIVE_ALWAYS;
        }
    }
}

void GameOver_StageLoad()
{
    GameOver->spriteIndex   = RSDK.LoadAnimation("Global/HUD.bin", SCOPE_STAGE);
    GameOver->activeScreens = 0;
}

void GameOver_SaveGameCallback()
{
    UIWaitSpinner_WaitReplay();
    RSDK.InitSceneLoad();
}

void GameOver_Unknown2()
{
    EntityGameOver *entity = (EntityGameOver *)RSDK_sceneInfo->entity;
    if (entity->dword64 > 0)
        entity->dword64 -= 0x40000;
    ScreenInfo *screen = RSDK_screens;
    entity->verts[0].x = entity->dword64 + ((RSDK_screens->centerX - 104) << 16);
    entity->verts[1].x = entity->dword64 + ((screen->centerX + 88) << 16);
    entity->verts[2].x = entity->dword64 + ((screen->centerX + 104) << 16);
    entity->verts[3].x = entity->dword64 + ((screen->centerX - 88) << 16);
    entity->verts[0].y = entity->dword68 - 0x80000;
    entity->verts[1].y = entity->dword68 - 0x80000;
    entity->verts[2].y = entity->dword68 + 0x80000;
    entity->verts[3].y = entity->dword68 + 0x80000;

    for (int i = 0; i < 8; ++i) {
        entity->letterPositions[i].x = (screen->centerX << 16) + entity->scale.x * (entity->unknownPos1[i].x >> 9);
        if (entity->unknownArray1[i] < 3) {
            entity->letterPosMove[i].y += 0x4000;
            entity->letterPositions[i].y += entity->letterPosMove[i].y;
            if (entity->letterPosMove[i].y > 0) {
                if (entity->letterPositions[i].y > entity->unknownPos1[i].y) {
                    entity->letterPositions[i].y = entity->unknownPos1[i].y;
                    entity->letterPosMove[i].y -= entity->letterPosMove[i].y;
                    ++entity->unknownArray1[i];
                }
            }
        }
    }

    if (entity->scale.x <= 0x200)
        entity->scale.x = 0x200;
    else
        entity->scale.x = entity->scale.x - entity->scale.x / 40;
    entity->scale.y = entity->scale.x;

    if (entity->timer == 0) {
        if (options->gameMode != MODE_COMPETITION) {
            for (int i = 40; i < 0x30; ++i) {
                RSDK.ResetEntityPtr(RSDK.GetEntityByID(i), 0, 0);
            }

            if (Music_Unknown4()) {
                Music->dword250 = 9;
            }
            else {
                EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(SLOT_MUSIC);
                Music->dword250    = 9;
                if (music->objectID != Music->objectID || music->state != Music_Unknown15) {
                    RSDK.ResetEntityPtr(music, Music->objectID, 0);
                    music->field_8C = 0.025;
                    music->volume   = 1.0;
                    music->state    = Music_Unknown15;
                }
            }
        }
        else {
            if (options->competitionSession[23] <= 0) {
                bool32 flag = false;
                if (0 < options->competitionSession[23] - 1 && 0 != options->competitionSession[23]) {
                    if (Zone->field_15C != 1)
                        flag = true;
                }

                if (!flag) {
                    for (int i = 40; i < 0x30; ++i) {
                        RSDK.ResetEntityPtr(RSDK.GetEntityByID(i), 0, 0);
                    }

                    if (Music_Unknown4()) {
                        Music->dword250 = 9;
                    }
                    else {
                        EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(SLOT_MUSIC);
                        Music->dword250    = 9;
                        if (music->objectID != Music->objectID || music->state != Music_Unknown15) {
                            RSDK.ResetEntityPtr(music, Music->objectID, 0);
                            music->field_8C = 0.025;
                            music->volume   = 1.0;
                            music->state    = Music_Unknown15;
                        }
                    }
                }
            }
            else {
                void *ptr    = NULL; // Competition->field_28 + 92;
                int field_28 = 0;    // Competition->field_28;
                int id       = 0;
                int id2      = 0;
                int id3      = 0;
                for (int i = 0; i < options->competitionSession[23]; ++i) {
                    Entity *ent = RSDK.GetEntityByID(i + Player->playerCount);
                    id2         = id;
                    if (ent->objectID == GameOver->objectID) {
                        id2 = ++id;
                        ++id3;
                    }
                    else if (!field_28 || /*!*ptr*/ true) {
                        //++ptr;
                    }
                    else {
                        ++id3;
                        //++ptr;
                    }
                }

                bool32 flag = false;
                if (0 < options->competitionSession[23] - 1 && 0 != options->competitionSession[23]) {
                    if (Zone->field_15C != 1)
                        flag = true;
                }

                if (!flag) {
                    for (int i = 40; i < 0x30; ++i) {
                        RSDK.ResetEntityPtr(RSDK.GetEntityByID(i), 0, 0);
                    }

                    if (Music_Unknown4()) {
                        Music->dword250 = 9;
                    }
                    else {
                        EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(SLOT_MUSIC);
                        Music->dword250    = 9;
                        if (music->objectID != Music->objectID || music->state != Music_Unknown15) {
                            RSDK.ResetEntityPtr(music, Music->objectID, 0);
                            music->field_8C = 0.025;
                            music->volume   = 1.0;
                            music->state    = Music_Unknown15;
                        }
                    }
                }
            }
        }
    }

    if (++entity->timer == 120) {
        entity->timer = 0;
        if (options->gameMode == MODE_COMPETITION || Zone->field_15C)
            entity->state = GameOver_Unknown3;
        else
            entity->state = GameOver_Unknown4;
    }
}

void GameOver_Unknown3()
{
    EntityGameOver *entity = (EntityGameOver *)RSDK_sceneInfo->entity;

    void *ptr    = NULL; // Competition->field_28 + 92;
    int field_28 = 0;    // Competition->field_28;
    int id       = 0;
    int id2      = 0;
    int id3      = 0;
    for (int i = 0; i < options->competitionSession[23]; ++i) {
        Entity *ent = RSDK.GetEntityByID(i + Player->playerCount);
        id2         = id;
        if (ent->objectID == GameOver->objectID) {
            id2 = ++id;
            ++id3;
        }
        else if (!field_28 || /*!*ptr*/ true) {
            //++ptr;
        }
        else {
            ++id3;
            //++ptr;
        }
    }

    if (id2 >= options->competitionSession[23] - 1 || id3 == options->competitionSession[23] || Zone->field_15C)
        entity->state = GameOver_Unknown4;
}

void GameOver_Unknown4()
{
    EntityGameOver *entity = (EntityGameOver *)RSDK_sceneInfo->entity;
    ++entity->timer;

    int cID = 0;
    if (options->gameMode == MODE_COMPETITION)
        cID = entity->playerID;
    else
        cID = 0;

    if (RSDK_controller[cID].keyA.press || RSDK_controller[cID].keyB.press || RSDK_controller[cID].keyC.press || RSDK_controller[cID].keyX.press
        || RSDK_controller[cID].keyStart.press)
        entity->timer = 420;

    if (entity->timer == 420) {
        for (int i = 40; i < 0x30; ++i) {
            RSDK.ResetEntityPtr(RSDK.GetEntityByID(i), 0, 0);
        }

        if (Music->activeTrack != TRACK_DROWNING) {
            EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(SLOT_MUSIC);
            RSDK.ResetEntityPtr(music, Music->objectID, 0);
            music->state    = Music_Unknown14;
            music->volume   = 1.0;
            music->field_8C = 0.05;
        }

        EntityGameOver *gameOver = NULL;
        while (RSDK.GetEntities(GameOver->objectID, (Entity **)&gameOver)) {
            int angle = 0x88;
            for (int i = 0; i < 4; ++i) {
                gameOver->letterPosMove[i].x   = RSDK.Cos256(angle) << 11;
                gameOver->letterPosMove[i].y   = RSDK.Sin256(angle) << 11;
                gameOver->letterRotateSpeed[i] = RSDK.Rand(-8, 8);
                angle += 0x10;
            }
            gameOver->timer = 0;
            gameOver->state = GameOver_Unknown5;
        }
    }
}

void GameOver_Unknown5()
{
    EntityGameOver *entity = (EntityGameOver *)RSDK_sceneInfo->entity;
    if (entity->timer < 120) {
        for (int i = 0; i < 8; ++i) {
            entity->letterPositions[i].x += entity->letterPosMove[i].x;
            entity->letterPositions[i].y += entity->letterPosMove[i].y;
            entity->letterRotations[i] += entity->letterRotateSpeed[i];
        }
        entity->verts[0].x -= 0x100000;
        entity->verts[0].y -= 0x80000;
        entity->verts[1].x += 0x100000;
        entity->verts[1].y -= 0x80000;
        entity->verts[2].x += 0x100000;
        entity->verts[2].y += 0x80000;
        entity->verts[3].x -= 0x100000;
        entity->verts[3].y += 0x80000;
        entity->scale.x += 0x20;
        entity->scale.y += 0x20;
        ++entity->timer;
    }
    if (entity->timer == 90) {
        entity->timer = 0;

        if (options->gameMode == MODE_COMPETITION) {
            options->competitionSession[options->competitionSession[24] + 31] = 1;
            options->competitionSession[27]                                   = options->competitionSession[93] + 1;
            RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
            RSDK.LoadScene("Presentation", "Menu");
            RSDK.InitSceneLoad();
        }
        else if (entity->data.animationID != 6) {
            StarPost->storedMS      = 0;
            StarPost->storedSeconds = 0;
            StarPost->storedMinutes = 0;
            RSDK.InitSceneLoad();
        }
        else {
            int *saveRAM = SaveGame->saveRAM;
            if (options->gameMode >= MODE_TIMEATTACK) {
                RSDK.LoadScene("Presentation", "Menu");
                RSDK.InitSceneLoad();
            }
            else if (options->continues > 0) {
                saveRAM[30] = RSDK_sceneInfo->listPos;
                saveRAM[25] = 3;
                saveRAM[26] = 0;
                saveRAM[27] = 0;
                if (options->gameMode == MODE_ENCORE) {
                    options->playerID &= 0xFF;
                    int id      = -1;
                    saveRAM[66] = -1;
                    for (int i = options->playerID; i > 0; ++id, i >>= 1)
                        ;
                    options->characterFlags = 1 << id;
                    saveRAM[66]             = options->characterFlags;
                    saveRAM[67]             = options->stock;
                }
                RSDK.LoadScene("Presentation", "Continue");
                RSDK.InitSceneLoad();
            }
            else {
                saveRAM[25] = 3;
                saveRAM[26] = 0;
                saveRAM[27] = 0;
                if (options->gameMode == MODE_ENCORE) {
                    options->playerID &= 0xFF;
                    int id      = -1;
                    saveRAM[66] = -1;
                    for (int i = options->playerID; i > 0; ++id, i >>= 1)
                        ;
                    options->characterFlags = 1 << id;
                    saveRAM[66]             = options->characterFlags;
                    saveRAM[67]             = options->stock;
                }
                UIWaitSpinner_Wait();
                // SaveGame_SaveFile(GameOver_SaveGameCallback);
                RSDK.LoadScene("Presentation", "Menu");
            }
        }
    }
}

void GameOver_EditorDraw() {}

void GameOver_EditorLoad() {}

void GameOver_Serialize() {}
