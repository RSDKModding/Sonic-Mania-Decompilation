#include "SonicMania.h"

ObjectGameOver *GameOver;

void GameOver_Update(void)
{
    RSDK_THIS(GameOver);
    StateMachine_Run(self->state);
}

void GameOver_LateUpdate(void) {}

void GameOver_StaticUpdate(void) {}

void GameOver_Draw(void)
{
    RSDK_THIS(GameOver);
    if (self->state != GameOver_State_ExitLetters && globals->gameMode == MODE_COMPETITION) {
        if (SceneInfo->currentScreenID != self->playerID || RSDK.GetEntityCount(PauseMenu->objectID, true) > 0) {
            return;
        }
    }
    EntityPlayer *player = RSDK_GET_ENTITY(SceneInfo->currentScreenID + Player->playerCount, Player);
    if (SceneInfo->currentScreenID == self->playerID || player->objectID != GameOver->objectID) {
        RSDK.DrawQuad(self->verts, 4, 0, 0, 0, 255, INK_NONE);
    }

    if (globals->gameMode != MODE_COMPETITION || SceneInfo->currentScreenID == self->playerID) {
        self->animator.frameID = 0;
        for (self->animator.frameID = 0; self->animator.frameID < GameOver_LetterCount; ++self->animator.frameID) {
            self->rotation = self->letterRotations[self->animator.frameID];
            RSDK.DrawSprite(&self->animator, &self->letterPositions[self->animator.frameID], true);
        }
    }
}

void GameOver_Create(void *data)
{
    RSDK_THIS(GameOver);
    if (!SceneInfo->inEditor) {
        self->active  = ACTIVE_ALWAYS;
        self->visible = true;
        self->drawFX  = FX_ROTATE | FX_SCALE;
        if (data)
            RSDK.SetSpriteAnimation(GameOver->aniFrames, 7, &self->animator, true, 1);
        else
            RSDK.SetSpriteAnimation(GameOver->aniFrames, 6, &self->animator, true, 0);

        self->finalOffsets[0].x = -0x480000;
        self->finalOffsets[1].x = -0x370000;
        self->finalOffsets[2].x = -0x260000;
        self->finalOffsets[3].x = -0x150000;
        self->finalOffsets[4].x = 0xC0000;
        self->finalOffsets[5].x = 0x1D0000;
        self->finalOffsets[6].x = 0x2E0000;
        self->finalOffsets[7].x = 0x3F0000;

        int32 posY = -0x200000;
        for (int32 i = 0; i < GameOver_LetterCount; ++i) {
            self->letterPosMove[i].x = -(self->finalOffsets[i].x >> 4);
            self->letterPosMove[i].y = 0x2000;

            self->finalOffsets[i].y = (ScreenInfo->centerY - 4) << 16;

            self->letterPositions[i].x = 8 * ((ScreenInfo->centerX << 13) + self->finalOffsets[i].x);
            self->letterPositions[i].y = posY;

            posY -= 0x100000;
        }

        self->barPos.x = 0x1000000;
        self->barPos.y  = ScreenInfo->centerY << 16;
        self->scale.x   = 0x800;
        self->state     = GameOver_State_EnterLetters;
        self->drawOrder = Zone->hudDrawOrder + 1;

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

    if (self->barPos.x > 0)
        self->barPos.x -= 0x40000;
    self->verts[0].x = self->barPos.x + ((ScreenInfo->centerX - 104) << 16);
    self->verts[1].x = self->barPos.x + ((ScreenInfo->centerX + 88) << 16);
    self->verts[2].x = self->barPos.x + ((ScreenInfo->centerX + 104) << 16);
    self->verts[3].x = self->barPos.x + ((ScreenInfo->centerX - 88) << 16);
    self->verts[0].y = self->barPos.y - 0x80000;
    self->verts[1].y = self->barPos.y - 0x80000;
    self->verts[2].y = self->barPos.y + 0x80000;
    self->verts[3].y = self->barPos.y + 0x80000;

    for (int32 i = 0; i < GameOver_LetterCount; ++i) {
        self->letterPositions[i].x = (ScreenInfo->centerX << 16) + self->scale.x * (self->finalOffsets[i].x >> 9);
        if (self->letterBounceCount[i] < 3) {
            self->letterPosMove[i].y += 0x4000;
            self->letterPositions[i].y += self->letterPosMove[i].y;
            if (self->letterPosMove[i].y > 0 && self->letterPositions[i].y > self->finalOffsets[i].y) {
                self->letterPositions[i].y = self->finalOffsets[i].y;
                self->letterPosMove[i].y   = -(self->letterPosMove[i].y / 3);
                ++self->letterBounceCount[i];
            }
        }
    }

    if (self->scale.x <= 0x200)
        self->scale.x = 0x200;
    else
        self->scale.x = self->scale.x - self->scale.x / 40;
    self->scale.y = self->scale.x;

    if (self->timer == 0) {
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

    if (++self->timer == 120) {
        self->timer = 0;
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_COMPETITION || Zone->gotTimeOver)
            self->state = GameOver_State_HandleMultiplayer;
        else
#endif
            self->state = GameOver_State_ShowMessage;
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
        self->state = GameOver_State_ShowMessage;
}
#endif

void GameOver_State_ShowMessage(void)
{
    RSDK_THIS(GameOver);
    ++self->timer;

    int32 cID = CONT_ANY;
    if (globals->gameMode == MODE_COMPETITION)
        cID = self->playerID + 1;

    if (ControllerInfo[cID].keyA.press || ControllerInfo[cID].keyB.press || ControllerInfo[cID].keyC.press || ControllerInfo[cID].keyX.press
        || ControllerInfo[cID].keyStart.press)
        self->timer = 420;

    if (self->timer == 420) {
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
    if (self->timer < 120) {
        for (int32 i = 0; i < GameOver_LetterCount; ++i) {
            self->letterPositions[i].x += self->letterPosMove[i].x;
            self->letterPositions[i].y += self->letterPosMove[i].y;
            self->letterRotations[i] += self->letterRotateSpeed[i];
        }
        self->verts[0].x -= 0x100000;
        self->verts[0].y -= 0x80000;
        self->verts[1].x += 0x100000;
        self->verts[1].y -= 0x80000;
        self->verts[2].x += 0x100000;
        self->verts[2].y += 0x80000;
        self->verts[3].x -= 0x100000;
        self->verts[3].y += 0x80000;
        self->scale.x += 0x20;
        self->scale.y += 0x20;
        ++self->timer;
    }
    if (self->timer == 90) {
        self->timer = 0;

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
        else if (self->animator.animationID != 6) {
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
