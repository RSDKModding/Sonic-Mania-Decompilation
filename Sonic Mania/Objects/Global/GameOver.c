#include "SonicMania.h"

ObjectGameOver *GameOver;

void GameOver_Update(void)
{
    RSDK_THIS(GameOver);
    StateMachine_Run(entity->state);
}

void GameOver_LateUpdate(void) {}

void GameOver_StaticUpdate(void) {}

void GameOver_Draw(void)
{
    RSDK_THIS(GameOver);
    if (entity->state != GameOver_State_ExitLetters && globals->gameMode == MODE_COMPETITION) {
        if (SceneInfo->currentScreenID != entity->playerID || RSDK.GetEntityCount(PauseMenu->objectID, true) > 0) {
            return;
        }
    }
    EntityPlayer *player = RSDK_GET_ENTITY(SceneInfo->currentScreenID + Player->playerCount, Player);
    if (SceneInfo->currentScreenID == entity->playerID || player->objectID != GameOver->objectID) {
        RSDK.DrawQuad(entity->verts, 4, 0, 0, 0, 255, INK_NONE);
    }

    if (globals->gameMode != MODE_COMPETITION || SceneInfo->currentScreenID == entity->playerID) {
        entity->animator.frameID = 0;
        for (entity->animator.frameID = 0; entity->animator.frameID < GameOver_LetterCount; ++entity->animator.frameID) {
            entity->rotation = entity->letterRotations[entity->animator.frameID];
            RSDK.DrawSprite(&entity->animator, &entity->letterPositions[entity->animator.frameID], true);
        }
    }
}

void GameOver_Create(void *data)
{
    RSDK_THIS(GameOver);
    if (!SceneInfo->inEditor) {
        entity->active  = ACTIVE_ALWAYS;
        entity->visible = true;
        entity->drawFX  = FX_ROTATE | FX_SCALE;
        if (data)
            RSDK.SetSpriteAnimation(GameOver->aniFrames, 7, &entity->animator, true, 1);
        else
            RSDK.SetSpriteAnimation(GameOver->aniFrames, 6, &entity->animator, true, 0);

        entity->unknownPos1[0].x = -0x480000;
        entity->unknownPos1[1].x = -0x370000;
        entity->unknownPos1[2].x = -0x260000;
        entity->unknownPos1[3].x = -0x150000;
        entity->unknownPos1[4].x = 0xC0000;
        entity->unknownPos1[5].x = 0x1D0000;
        entity->unknownPos1[6].x = 0x2E0000;
        entity->unknownPos1[7].x = 0x3F0000;

        int32 posY = -0x200000;
        for (int32 i = 0; i < GameOver_LetterCount; ++i) {
            entity->letterPosMove[i].x = -(entity->unknownPos1[i].x >> 4);
            entity->letterPosMove[i].y = 0x2000;

            entity->unknownPos1[i].y = (ScreenInfo->centerY - 4) << 16;

            entity->letterPositions[i].x = 8 * ((ScreenInfo->centerX << 13) + entity->unknownPos1[i].x);
            entity->letterPositions[i].y = posY;

            posY -= 0x100000;
        }

        entity->barPos.x = 0x1000000;
        entity->barPos.y  = ScreenInfo->centerY << 16;
        entity->scale.x   = 0x800;
        entity->state     = GameOver_State_EnterLetters;
        entity->drawOrder = Zone->hudDrawOrder + 1;

        foreach_all(HUD, hud)
        {
            hud->state  = HUD_State_GoOffScreen;
            hud->active = ACTIVE_ALWAYS;
        }
    }
}

void GameOver_StageLoad(void)
{
    GameOver->aniFrames   = RSDK.LoadSpriteAnimation("Global/HUD.bin", SCOPE_STAGE);
    GameOver->activeScreens = 0;
}

void GameOver_SaveGameCallback(int32 status)
{
    UIWaitSpinner_Wait2();
    RSDK.LoadScene();
}

void GameOver_State_EnterLetters(void)
{
    RSDK_THIS(GameOver);

    if (entity->barPos.x > 0)
        entity->barPos.x -= 0x40000;
    entity->verts[0].x = entity->barPos.x + ((ScreenInfo->centerX - 104) << 16);
    entity->verts[1].x = entity->barPos.x + ((ScreenInfo->centerX + 88) << 16);
    entity->verts[2].x = entity->barPos.x + ((ScreenInfo->centerX + 104) << 16);
    entity->verts[3].x = entity->barPos.x + ((ScreenInfo->centerX - 88) << 16);
    entity->verts[0].y = entity->barPos.y - 0x80000;
    entity->verts[1].y = entity->barPos.y - 0x80000;
    entity->verts[2].y = entity->barPos.y + 0x80000;
    entity->verts[3].y = entity->barPos.y + 0x80000;

    for (int32 i = 0; i < GameOver_LetterCount; ++i) {
        entity->letterPositions[i].x = (ScreenInfo->centerX << 16) + entity->scale.x * (entity->unknownPos1[i].x >> 9);
        if (entity->letterBounceCount[i] < 3) {
            entity->letterPosMove[i].y += 0x4000;
            entity->letterPositions[i].y += entity->letterPosMove[i].y;
            if (entity->letterPosMove[i].y > 0 && entity->letterPositions[i].y > entity->unknownPos1[i].y) {
                entity->letterPositions[i].y = entity->unknownPos1[i].y;
                entity->letterPosMove[i].y   = -(entity->letterPosMove[i].y / 3);
                ++entity->letterBounceCount[i];
            }
        }
    }

    if (entity->scale.x <= 0x200)
        entity->scale.x = 0x200;
    else
        entity->scale.x = entity->scale.x - entity->scale.x / 40;
    entity->scale.y = entity->scale.x;

    if (entity->timer == 0) {
#if RETRO_USE_PLUS
        EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
        if (globals->gameMode != MODE_COMPETITION) {
            Music_TransitionTrack(TRACK_GAMEOVER, 0.025);
        }
        else {
            if (session->playerCount <= 0) {
                bool32 playMusicFlag = true;
                if (!Zone->gotTimeOver)
                    playMusicFlag = false;

                if (playMusicFlag) {
                    Music_TransitionTrack(TRACK_GAMEOVER, 0.025);
                }
            }
            else {
                EntityCompetition *manager = (EntityCompetition *)Competition->activeEntity;
                int32 id                     = 0;
                int32 deadPlayers            = 0;
                for (int32 i = 0; i < session->playerCount; ++i) {
                    Entity *ent = RSDK.GetEntityByID(i + Player->playerCount);

                    if (ent->objectID == GameOver->objectID) {
                        ++id;
                        ++deadPlayers;
                    }
                    else if (manager && manager->playerFlags[i]) {
                        ++deadPlayers;
                    }
                }

                bool32 playMusicFlag = true;
                if (id < session->playerCount - 1 && deadPlayers != session->playerCount) {
                    if (!Zone->gotTimeOver)
                        playMusicFlag = false;
                }

                if (playMusicFlag) {
                    Music_TransitionTrack(TRACK_GAMEOVER, 0.025);
                }
            }
        }
#else
        Music_TransitionTrack(TRACK_GAMEOVER, 0.025);
#endif
    }

    if (++entity->timer == 120) {
        entity->timer = 0;
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_COMPETITION || Zone->gotTimeOver)
            entity->state = GameOver_State_HandleMultiplayer;
        else
#endif
            entity->state = GameOver_State_ShowMessage;
    }
}

#if RETRO_USE_PLUS
void GameOver_State_HandleMultiplayer(void)
{
    RSDK_THIS(GameOver);

    EntityCompetition *manager        = (EntityCompetition *)Competition->activeEntity;
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    int32 id                            = 0;
    int32 deadPlayers                   = 0;
    for (int32 i = 0; i < session->playerCount; ++i) {
        Entity *ent = RSDK.GetEntityByID(i + Player->playerCount);

        if (ent->objectID == GameOver->objectID) {
            ++id;
            ++deadPlayers;
        }
        else if (manager && manager->playerFlags[i]) {
            ++deadPlayers;
        }
    }

    if (id >= session->playerCount - 1 || deadPlayers == session->playerCount || Zone->gotTimeOver)
        entity->state = GameOver_State_ShowMessage;
}
#endif

void GameOver_State_ShowMessage(void)
{
    RSDK_THIS(GameOver);
    ++entity->timer;

    int32 cID = CONT_ANY;
    if (globals->gameMode == MODE_COMPETITION)
        cID = entity->playerID + 1;

    if (ControllerInfo[cID].keyA.press || ControllerInfo[cID].keyB.press || ControllerInfo[cID].keyC.press || ControllerInfo[cID].keyX.press
        || ControllerInfo[cID].keyStart.press)
        entity->timer = 420;

    if (entity->timer == 420) {
        Music_ClearMusicStack();
        Music_FadeOut(0.05);

        foreach_all(GameOver, gameOver)
        {
            int32 angle = 0x88;
            for (int32 i = 0; i < GameOver_LetterCount; ++i) {
                gameOver->letterPosMove[i].x   = RSDK.Cos256(angle) << 11;
                gameOver->letterPosMove[i].y   = RSDK.Sin256(angle) << 11;
                gameOver->letterRotateSpeed[i] = RSDK.Rand(-8, 8);
                angle += 0x10;
            }
            gameOver->timer = 0;
            gameOver->state = GameOver_State_ExitLetters;
        }
    }
}

void GameOver_State_ExitLetters(void)
{
    RSDK_THIS(GameOver);
    if (entity->timer < 120) {
        for (int32 i = 0; i < GameOver_LetterCount; ++i) {
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

        EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
        if (globals->gameMode == MODE_COMPETITION) {
            session->zoneFlags[session->levelIndex] = 1;
#if RETRO_USE_PLUS
            session->matchID = session->prevMatchID + 1;
#else
            session->matchID++;
#endif
            RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
            RSDK.SetScene("Presentation", "Menu");
            RSDK.LoadScene();
        }
        else if (entity->animator.animationID != 6) {
            StarPost->storedMS      = 0;
            StarPost->storedSeconds = 0;
            StarPost->storedMinutes = 0;
            RSDK.LoadScene();
        }
        else {
            EntitySaveGame *saveRAM = SaveGame->saveRAM;
            if (globals->gameMode >= MODE_TIMEATTACK) {
                RSDK.SetScene("Presentation", "Menu");
                RSDK.LoadScene();
            }
            else if (globals->continues > 0) {
                saveRAM->storedStageID = SceneInfo->listPos;
                saveRAM->lives         = 3;
                saveRAM->score         = 0;
                saveRAM->score1UP      = 0;
#if RETRO_USE_PLUS
                if (globals->gameMode == MODE_ENCORE) {
                    globals->playerID &= 0xFF;
                    int32 id                  = -1;
                    saveRAM->characterFlags = -1;
                    for (int32 i = globals->playerID; i > 0; ++id, i >>= 1)
                        ;
                    globals->characterFlags = 1 << id;
                    saveRAM->characterFlags = globals->characterFlags;
                    saveRAM->stock          = globals->stock;
                }
#endif
                RSDK.SetScene("Presentation", "Continue");
                RSDK.LoadScene();
            }
            else {
                saveRAM->lives    = 3;
                saveRAM->score    = 0;
                saveRAM->score1UP = 0;
#if RETRO_USE_PLUS
                if (globals->gameMode == MODE_ENCORE) {
                    globals->playerID &= 0xFF;
                    int32 id                  = -1;
                    saveRAM->characterFlags = -1;
                    for (int32 i = globals->playerID; i > 0; ++id, i >>= 1)
                        ;
                    globals->characterFlags = 1 << id;
                    saveRAM->characterFlags = globals->characterFlags;
                    saveRAM->stock          = globals->stock;
                }
#endif
                UIWaitSpinner_Wait();
                SaveGame_SaveFile(GameOver_SaveGameCallback);
                RSDK.SetScene("Presentation", "Menu");
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void GameOver_EditorDraw(void) {}

void GameOver_EditorLoad(void) {}
#endif

void GameOver_Serialize(void) {}
