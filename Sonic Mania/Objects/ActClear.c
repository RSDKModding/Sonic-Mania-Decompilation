#include "../SonicMania.h"

ObjectActClear *ActClear;

void ActClear_Update(void) {}

void ActClear_LateUpdate(void)
{
    RSDK_THIS(ActClear);
    StateMachine_Run(entity->state);
}

void ActClear_StaticUpdate(void) {}

void ActClear_Draw(void)
{
    Vector2 drawPos;
    Vector2 verts[4];
    int millisecsPtr = 0;
    int secsPtr      = 0;
    int minsPtr      = 0;
    int offset       = 0;
    RSDK_THIS(ActClear);

    drawPos.x  = entity->posUnknown3.x;
    drawPos.y  = entity->posUnknown3.y;
    verts[0].y = drawPos.y - 0x140000;
    verts[1].y = drawPos.y - 0x140000;
    verts[2].y = drawPos.y - 0x40000;
    verts[3].y = drawPos.y - 0x40000;
    if ((globals->playerID & 0xFF) == ID_KNUCKLES) {
        int center = (RSDK_screens->centerX << 16) + 0x100000;
        drawPos.x  = center + 2 * entity->posUnknown3.x;
        verts[0].x = drawPos.x - 0x910000;
        verts[1].x = drawPos.x + 0x340000;
        verts[2].x = drawPos.x + 0x440000;
        verts[3].x = drawPos.x - 0x810000;
        RSDK.DrawQuad(verts, 4, 0, 0, 0, 255, 0);
        drawPos.x  = entity->posUnknown.x;
        drawPos.y  = entity->posUnknown.y;
        drawPos.x  = center + 2 * drawPos.x;
        verts[0].x = drawPos.x - 0x7C0000;
        verts[0].y = drawPos.y + 0x1C0000;
        verts[1].x = 0x500000 + drawPos.x;
        verts[2].x = 0x600000 + drawPos.x;
        verts[3].x = drawPos.x - 0x6C0000;
        verts[1].y = drawPos.y + 0x1C0000;
        verts[2].y = drawPos.y + 0x2C0000;
        verts[3].y = drawPos.y + 0x2C0000;
        RSDK.DrawQuad(verts, 4, 0, 0, 0, 255, 0);
        drawPos.x = entity->posUnknown.x;
        drawPos.y = entity->posUnknown.y;
        drawPos.x += center;
        RSDK.DrawSprite(&entity->playerNameData, &drawPos, true);
        entity->gotThroughData.frameID = 2;
        RSDK.DrawSprite(&entity->gotThroughData, &drawPos, true);
        drawPos.x = entity->posUnknown3.x;
        drawPos.y = entity->posUnknown3.y;
        drawPos.x += center;
        entity->gotThroughData.frameID = 3;
        RSDK.DrawSprite(&entity->gotThroughData, &drawPos, true);
        RSDK.DrawSprite(&entity->actNoData, &drawPos, true);

        offset = center - 0xA0000;
    }
    else {
        int center = (RSDK_screens->centerX << 16) + 0x100000;
        drawPos.x  = center + 2 * entity->posUnknown3.x;
        verts[0].x = drawPos.x - 0x6D0000;
        verts[1].x = drawPos.x + 0x340000;
        verts[2].x = drawPos.x + 0x440000;
        verts[3].x = drawPos.x - 0x5D0000;
        RSDK.DrawQuad(verts, 4, 0, 0, 0, 255, 0);
        drawPos.x  = entity->posUnknown.x;
        drawPos.y  = entity->posUnknown.y;
        drawPos.x  = center + 2 * drawPos.x;
        verts[0].x = drawPos.x - 0x580000;
        verts[0].y = drawPos.y + 0x1C0000;
        verts[1].x = 0x500000 + drawPos.x;
        verts[2].x = 0x600000 + drawPos.x + 2;
        verts[3].x = drawPos.x - 0x480000;
        verts[1].y = drawPos.y + 0x1C0000;
        verts[2].y = drawPos.y + 0x2C0000;
        verts[3].y = drawPos.y + 0x2C0000;
        RSDK.DrawQuad(verts, 4, 0, 0, 0, 255, 0);
        drawPos.x = entity->posUnknown.x;
        drawPos.y = entity->posUnknown.y;
        drawPos.x += center;
        RSDK.DrawSprite(&entity->playerNameData, &drawPos, true);
#if RETRO_USE_PLUS
        if ((globals->playerID & 0xFF) == ID_MIGHTY)
            entity->gotThroughData.frameID = 2;
        else
#endif
            entity->gotThroughData.frameID = 0;
        RSDK.DrawSprite(&entity->gotThroughData, &drawPos, true);
        drawPos.x = entity->posUnknown3.x;
        drawPos.y = entity->posUnknown3.y;
        drawPos.x += center;
        entity->gotThroughData.frameID = 1;
        RSDK.DrawSprite(&entity->gotThroughData, &drawPos, true);
        RSDK.DrawSprite(&entity->actNoData, &drawPos, true);

        offset = center + 0x60000;
    }

    drawPos.x             = offset + entity->posUnknown2.x - 0x5C0000;
    drawPos.y             = entity->posUnknown2.y;
    entity->data1.frameID = 1;
    RSDK.DrawSprite(&entity->data1, &drawPos, true);
    drawPos.x += 0x320000;
    if (globals->gameMode != MODE_TIMEATTACK) {
        entity->data1.frameID = 8;
        RSDK.DrawSprite(&entity->data1, &drawPos, true);
    }
    entity->data1.frameID = 10;
    drawPos.x += 0x340000;
    RSDK.DrawSprite(&entity->data1, &drawPos, true);

    int dx = 0x430000 + drawPos.x;
    int dy = drawPos.y + 0xE0000;
    drawPos.x += 0x430000;
    drawPos.y += 0xE0000;
    if (globals->gameMode == MODE_TIMEATTACK) {
        drawPos.x = dx - 0x620000;
        drawPos.y = dy - 0xE0000;
        ActClear_DrawTime(RSDK_sceneInfo->minutes, &drawPos, RSDK_sceneInfo->seconds, RSDK_sceneInfo->milliseconds);
    }
    else {
        ActClear_DrawNumbers(&drawPos, entity->scoreBonus, 0);
    }

    drawPos.x = offset + entity->posUnknown4.x - 0x5C0000;
    drawPos.y = entity->posUnknown4.y;
    if (globals->gameMode == MODE_TIMEATTACK)
        entity->data1.frameID = 17;
    else
        entity->data1.frameID = 5;
    RSDK.DrawSprite(&entity->data1, &drawPos, true);

    dx = drawPos.x + 0x320000;
    drawPos.x += 0x320000;
    if (globals->gameMode == MODE_TIMEATTACK) {
        drawPos.x             = dx - 0xA0000;
        entity->data1.frameID = 1;
        RSDK.DrawSprite(&entity->data1, &drawPos, true);
        drawPos.x += 0xA0000;
    }
    else {
        entity->data1.frameID = 8;
        RSDK.DrawSprite(&entity->data1, &drawPos, true);
    }

    entity->data1.frameID = 10;
    drawPos.x += 0x340000;
    RSDK.DrawSprite(&entity->data1, &drawPos, true);

    drawPos.x += 0x430000;
    drawPos.y += 0xE0000;
    if (globals->gameMode == MODE_TIMEATTACK) {
        TimeAttackData_GetTimeFromValue(entity->time, &minsPtr, &secsPtr, &millisecsPtr);
        drawPos.x -= 0x620000;
        drawPos.y -= 0xE0000;
        if (!entity->field_80 || (entity->field_80 == 1 && (Zone->timer & 8)))
            ActClear_DrawTime(minsPtr, &drawPos, secsPtr, millisecsPtr);
    }
    else {
        ActClear_DrawNumbers(&drawPos, entity->field_68, 0);
    }

    if (entity->field_84 == 1) {
        drawPos.x = entity->posUnknown5.x;
        drawPos.y = entity->posUnknown5.y;
        drawPos.x = offset + drawPos.x - 0x5C0000;
        if (globals->gameMode == MODE_TIMEATTACK)
            entity->data1.frameID = 18;
        else
            entity->data1.frameID = 15;
        RSDK.DrawSprite(&entity->data1, &drawPos, true);

        drawPos.x += 0x320000;
        if (globals->gameMode != MODE_TIMEATTACK) {
            entity->data1.frameID = 8;
            RSDK.DrawSprite(&entity->data1, &drawPos, true);
        }

        entity->data1.frameID = 10;
        drawPos.x += 0x340000;
        RSDK.DrawSprite(&entity->data1, &drawPos, true);

        drawPos.x += 0x430000;
        drawPos.y += 0xE0000;
        if (globals->gameMode != MODE_TIMEATTACK) {
            ActClear_DrawNumbers(&drawPos, entity->dword6C, 0);
        }
        else {
            if (!TimeAttackData->field_14) {
                entity                = (EntityActClear *)RSDK_sceneInfo->entity;
                entity->data2.frameID = 16;
                RSDK.DrawSprite(&entity->data2, &drawPos, true);

                drawPos.x -= 0x90000;
            }
            else if (!entity->field_7C || (entity->field_7C == 1 && (Zone->timer & 8)))
                ActClear_DrawNumbers(&drawPos, TimeAttackData->field_14, 0);
        }
    }

    drawPos.x = entity->posUnknown6.x;
    drawPos.y = entity->posUnknown6.y;

    int val = 0;
    if (globals->gameMode == MODE_TIMEATTACK) {
        drawPos.x             = offset + entity->posUnknown6.x - 0x5C0000;
        entity->data1.frameID = 19;
        RSDK.DrawSprite(&entity->data1, &drawPos, true);

        drawPos.x += 0x320000;
        entity->data1.frameID = 18;
        RSDK.DrawSprite(&entity->data1, &drawPos, true);

        entity->data1.frameID = 10;
        drawPos.x += 0x340000;
        RSDK.DrawSprite(&entity->data1, &drawPos, true);

        drawPos.x += 0x430000;
        drawPos.y += 0xE0000;
        if (!TimeAttackData->rank) {
            entity->data2.frameID = 16;
            RSDK.DrawSprite(&entity->data2, &drawPos, true);
            return;
        }
        else {
            val = TimeAttackData->rank;
        }
    }
    else {
        drawPos.x             = offset + entity->posUnknown6.x - 0x440000;
        entity->data1.frameID = 9;
        RSDK.DrawSprite(&entity->data1, &drawPos, true);

        entity->data1.frameID = 10;
        drawPos.x += 0x340000;
        RSDK.DrawSprite(&entity->data1, &drawPos, true);

        drawPos.x += 0x430000;
        drawPos.y += 0xE0000;
        val = entity->field_70;
    }
    ActClear_DrawNumbers(&drawPos, val, 0);
}

void ActClear_Create(void *data)
{
    RSDK_THIS(ActClear);
    if (!RSDK_sceneInfo->inEditor) {
        ActClear->dword34        = 1;
        entity->active           = ACTIVE_NORMAL;
        entity->visible          = 1;
        entity->drawOrder        = Zone->uiDrawHigh;
        entity->state            = ActClear_Unknown6;
        entity->stageFinishTimer = 0;
        entity->dword78          = 0;
        EntityPlayer *player1    = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        entity->playerPtr        = player1;

        if (Zone_GetZoneID() >= 0) {
            StatInfo stat;
            StatInfo *statPtr = NULL;
            ushort time       = RSDK_sceneInfo->milliseconds + 100 * (RSDK_sceneInfo->seconds + 60 * RSDK_sceneInfo->minutes);
            switch (globals->playerID & 0xFF) {
                case ID_SONIC: TimeAttackData_TrackActClear(Zone->actID, Zone_GetZoneID(), &stat, 1, time, player1->rings, player1->score); break;
                case ID_TAILS: TimeAttackData_TrackActClear(Zone->actID, Zone_GetZoneID(), &stat, 2, time, player1->rings, player1->score); break;
                case ID_KNUCKLES:
                    TimeAttackData_TrackActClear(Zone->actID, Zone_GetZoneID(), &stat, 3, time, player1->rings, player1->score);
                    break;
#if RETRO_USE_PLUS
                case ID_MIGHTY:
                    TimeAttackData_TrackActClear(Zone->actID, Zone_GetZoneID(), &stat, 4, time, player1->rings, player1->score);
                    break;
#endif
                default:
#if RETRO_USE_PLUS
                    if ((globals->playerID & 0xFF) == ID_RAY)
                        TimeAttackData_TrackActClear(Zone->actID, Zone_GetZoneID(), &stat, 5, time, player1->rings, player1->score);
                    else
#endif
                        TimeAttackData_TrackActClear(Zone->actID, Zone_GetZoneID(), &stat, 1, time, player1->rings, player1->score);
                    break;
            }
#if RETRO_USE_PLUS
            User.TryTrackStat(&stat);
#endif
        }

        if (!ActClear->field_30) {
            switch (RSDK_sceneInfo->minutes) {
                case 0:
                    if (RSDK_sceneInfo->seconds >= 30)
                        entity->scoreBonus = RSDK_sceneInfo->seconds < 45 ? 10000 : 5000;
                    else
                        entity->scoreBonus = 50000;
                    break;
                case 1: entity->scoreBonus = RSDK_sceneInfo->seconds < 30 ? 4000 : 3000; break;
                case 2: entity->scoreBonus = 2000; break;
                case 3: entity->scoreBonus = 1000; break;
                case 4: entity->scoreBonus = 500; break;
                case 5: entity->scoreBonus = 100; break;
                case 9:
                    if (!getMod(MEDAL_NOTIMEOVER) && !RSDK_sceneInfo->debugMode && globals->gameMode < MODE_TIMEATTACK
                        && RSDK_sceneInfo->seconds == 59) {
#if RETRO_USE_PLUS
                        if (globals->gameMode != MODE_ENCORE)
#endif
                            entity->scoreBonus = 100000;
                    }
                    break;
                default: break;
            }
        }
        entity->field_68       = 100 * player1->rings;
        entity->dword6C        = globals->coolBonus[0];
        globals->initCoolBonus = 0;
        if (globals->gameMode == MODE_TIMEATTACK) {
#if RETRO_USE_PLUS
            entity->time = TimeAttackData_SetScore(globals->menuParam[92], globals->menuParam[91], globals->menuParam[93],
                                                   RSDK_sceneInfo->filter == SCN_FILTER_ENCORE, 1);
#else

#endif
            entity->field_7C = 0;
            entity->field_80 = 0;
        }
        entity->field_84      = 1;
        entity->posUnknown.x  = 0xE00000;
        entity->posUnknown.y  = 0x580000;
        entity->posUnknown3.x = -0xE00000;
        entity->posUnknown3.y = 0x700000;
        entity->posUnknown2.x = 0x1E80000;
        entity->posUnknown2.y = 0x780000;
        entity->posUnknown4.x = 0x3080000;
        entity->posUnknown4.y = 0x880000;
        entity->posUnknown5.x = 0x4280000;
        entity->posUnknown5.y = 0x980000;
        entity->posUnknown6.x = -0x5480000;
        entity->posUnknown6.y = 0xC00000;
        RSDK.SetSpriteAnimation(ActClear->spriteIndex, 0, &entity->data1, true, 0);
        RSDK.SetSpriteAnimation(ActClear->spriteIndex, 1, &entity->data2, true, 0);
        RSDK.SetSpriteAnimation(ActClear->spriteIndex, 0, &entity->data3, true, 12);
        switch (globals->playerID & 0xFF) {
            case ID_TAILS: RSDK.SetSpriteAnimation(ActClear->spriteIndex, 3, &entity->playerNameData, true, 1); break;
            case ID_KNUCKLES: RSDK.SetSpriteAnimation(ActClear->spriteIndex, 3, &entity->playerNameData, true, 2); break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: RSDK.SetSpriteAnimation(ActClear->spriteIndex, 3, &entity->playerNameData, true, 3); break;
            case ID_RAY: RSDK.SetSpriteAnimation(ActClear->spriteIndex, 3, &entity->playerNameData, true, 4); break;
#endif
            default: RSDK.SetSpriteAnimation(ActClear->spriteIndex, 3, &entity->playerNameData, true, 0); break;
        }
        RSDK.SetSpriteAnimation(ActClear->spriteIndex, 4, &entity->gotThroughData, true, 0);

        if (ActClear->actID <= 0)
            RSDK.SetSpriteAnimation(ActClear->spriteIndex, 5, &entity->actNoData, true, Zone->actID);
        else
            RSDK.SetSpriteAnimation(ActClear->spriteIndex, 5, &entity->actNoData, true, ActClear->actID - 1);
    }
}

void ActClear_StageLoad(void)
{
    ActClear->dword34        = 0;
    ActClear->spriteIndex    = RSDK.LoadSpriteAnimation("Global/HUD.bin", SCOPE_STAGE);
    ActClear->sfx_ScoreAdd   = RSDK.GetSFX("Global/ScoreAdd.wav");
    ActClear->sfx_ScoreTotal = RSDK.GetSFX("Global/ScoreTotal.wav");
    ActClear->sfx_Event      = RSDK.GetSFX("Special/Event.wav");
    ActClear->field_20       = 0;
}

void ActClear_DrawTime(int mins, Vector2 *pos, int secs, int millisecs)
{
    Vector2 drawPos;
    RSDK_THIS(ActClear);

    drawPos.x = pos->x + 0x320000;
    drawPos.y = pos->y - 0x20000;
    RSDK.DrawSprite(&entity->data3, &drawPos, true);

    drawPos.x = 0x610000 + pos->x + 2;
    drawPos.y = pos->y + 0xE0000;
    if (!mins && !secs && !millisecs)
        millisecs = -1;
    ActClear_DrawNumbers(&drawPos, millisecs, 2);

    drawPos.x -= 0x90000;
    if (!mins && !secs && !millisecs)
        secs = -1;
    ActClear_DrawNumbers(&drawPos, secs, 2);

    drawPos.x -= 0x90000;
    if (!mins && !secs && !millisecs)
        mins = -1;
    ActClear_DrawNumbers(&drawPos, mins, 1);
}

void ActClear_DrawNumbers(Vector2 *pos, int value, int maxVals)
{
    RSDK_THIS(ActClear);
    int mult = 1;
    if (value >= 0) {
        if (!maxVals) {
            if (value <= 0) {
                maxVals = 1;
            }
            else {
                int i = value;
                do {
                    ++maxVals;
                    i /= 10;
                } while (i > 0);
            }
        }

        if (maxVals > 0) {
            int digit = 1;
            do {
                entity->data2.frameID = value / mult % 10;
                RSDK.DrawSprite(&entity->data2, pos, true);
                pos->x -= 0x90000;
                mult = 10 * digit;
                digit *= 10;
            } while (--maxVals);
        }
    }
    else if (maxVals > 0) {
        do {
            entity->data2.frameID = 16;
            RSDK.DrawSprite(&entity->data2, pos, true);
            pos->x -= 0x90000;
        } while (--maxVals);
    }
}
void ActClear_CheckPlayerVictory(void)
{
    foreach_active(Player, player)
    {
        if (player->state != Player_State_FlyIn && player->state != Player_State_JumpIn && player->state != Player_State_Victory
            && player->onGround) {
            player->state           = Player_State_Victory;
            player->nextAirState    = 0;
            player->nextGroundState = 0;
            if (Zone->forcePlayerOnScreenFlag)
                player->inputState = StateMachine_None;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_VICTORY, &player->playerAnimData, true, 0);
        }
    }
}
void ActClear_SaveGameCallback(int success)
{
    UIWaitSpinner_Wait2();
    ActClear->field_10 = 0;
}

void ActClear_Unknown5(void)
{
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    if (player2) {
        if (player2->sidekick) {
            if (player2->state != Player_State_FlyIn && player2->state != Player_State_JumpIn) {
                if (player2->position.x <= (RSDK_screens->width + RSDK_screens->position.x) << 16
                    || abs(player2->position.y - player1->position.y) > 0x100000) {
                    Player->jumpInDelay    = 240;
                    Entity *entStore       = RSDK_sceneInfo->entity;
                    RSDK_sceneInfo->entity = (Entity *)player2;
                    Player_P2JumpBackIn();
                    RSDK_sceneInfo->entity = entStore;
                    if (player2->state == Player_State_FlyIn || player2->state == Player_State_JumpIn || player2->state == Player_State_StartJumpIn) {
                        player2->active     = ACTIVE_NORMAL;
                        player2->position.y = ((RSDK_screens->position.y - 16) << 16);
                    }
                }
                else {
                    Player->jumpInDelay = -3600;
                }
            }
        }
    }
}

void ActClear_Unknown6(void)
{
    RSDK_THIS(ActClear);

    if (entity->posUnknown.x > 0)
        entity->posUnknown.x -= 0x100000;

    if (entity->posUnknown3.x < 0)
        entity->posUnknown3.x += 0x100000;
    if (!entity->timer && Zone->forcePlayerOnScreenFlag)
        ActClear_Unknown5();

    if (++entity->timer == 48) {
        entity->timer = 0;
        entity->state = ActClear_Unknown7;
    }
    ActClear_CheckPlayerVictory();
}

void ActClear_Unknown7(void)
{
    RSDK_THIS(ActClear);

    entity->posUnknown.y -= 0x8000;
    entity->posUnknown3.y -= 0x8000;
    if (++entity->timer == 48) {
        entity->timer = 0;
        entity->state = ActClear_State_TAFinish;
    }
    ActClear_CheckPlayerVictory();
}

void ActClear_State_TAFinish(void)
{
    RSDK_THIS(ActClear);

    if (entity->posUnknown2.x > 0)
        entity->posUnknown2.x -= 0x100000;

    if (entity->posUnknown4.x > 0)
        entity->posUnknown4.x -= 0x100000;

    if (entity->posUnknown5.x > 0)
        entity->posUnknown5.x -= 0x100000;

    if (entity->posUnknown6.x >= -0x80000) {
        if (globals->gameMode == MODE_TIMEATTACK) {
            if (ActClear->field_C) {
                if (ActClear->bufferMove_CB)
                    ActClear->bufferMove_CB();
            }
            RSDK.LoadScene("Presentation", "Menu");
            HUD->dwordC        = 1;
            ActClear->field_2C = 0;
            entity->dword78    = 240;
            entity->state      = ActClear_Unknown9;
        }
        else {
            entity->state = ActClear_Unknown8;
        }
    }
    else {
        entity->posUnknown6.x += 0x100000;
    }
    ActClear_CheckPlayerVictory();
}

void ActClear_Unknown8(void)
{
    RSDK_THIS(ActClear);
    if (++entity->timer == 120) {
        entity->timer = 0;
        entity->state = ActClear_TallyScore;
    }
    ActClear_CheckPlayerVictory();
}

void ActClear_TallyScore(void)
{
    RSDK_THIS(ActClear);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    if (entity->scoreBonus > 0) {
        entity->field_70 += 100;
        entity->scoreBonus -= 100;
        Player_GiveScore(player, 100);
    }

    if (entity->dword6C > 0) {
        entity->field_70 += 100;
        entity->field_68 -= 100;
        Player_GiveScore(player, 100);
    }

    if (entity->dword6C > 0) {
        entity->field_70 += 100;
        entity->dword6C -= 100;
        Player_GiveScore(player, 100);
    }

    if (RSDK_controller[player->controllerID].keyA.press || RSDK_controller[player->controllerID].keyStart.press) {
        Player_GiveScore(player, entity->scoreBonus + entity->field_68 + entity->dword6C);
        entity->field_70 += entity->scoreBonus + entity->field_68 + entity->dword6C;
        entity->scoreBonus = 0;
        entity->field_68   = 0;
        entity->dword6C    = 0;
    }

    if (entity->scoreBonus + entity->field_68 + entity->dword6C <= 0) {
        entity->timer = 0;
        entity->state = ActClear_LoadNextScene;
        RSDK.PlaySFX(ActClear->sfx_ScoreTotal, 0, 255);
    }
    else if (++entity->timer == 2) {
        entity->timer = 0;
        RSDK.PlaySFX(ActClear->sfx_ScoreAdd, 0, 255);
    }
    Music->dword250 = -1;
    ActClear_CheckPlayerVictory();
}

void ActClear_LoadNextScene(void)
{
    RSDK_THIS(ActClear);
    if (++entity->timer == 120) {
        entity->timer          = 0;
        globals->specialRingID = 0;
        if (ActClear->actID <= 0) {
            if (globals->gameMode == MODE_COMPETITION) {
                RSDK.LoadScene("Presentation", "Menu");
            }
            else {
                globals->enableIntro = true;
                Player_SaveValues();
                SaveGame_ClearRestartData();
                StarPost_ResetStarPosts();
                if (Zone->actID > 0)
                    SaveGame->saveRAM[32] = 0;
                SaveGame_SaveProgress();
                if (globals->saveSlotID != NO_SAVE_SLOT && !ActClear->field_20) {
                    if (Zone_IsAct2())
                        SaveGame_MarkZoneCompleted(Zone_GetZoneID());
                    ActClear->field_10 = 1;
                    SaveGame_SaveFile(ActClear_SaveGameCallback);
                }
                ++RSDK_sceneInfo->listPos;
                if (!RSDK.CheckValidScene())
                    RSDK.LoadScene("Presentation", "Title Screen");
            }
        }
        else {
            Player_SaveValues();
            SaveGame_ClearRestartData();
            StarPost_ResetStarPosts();
            SaveGame_SaveProgress();
            if (globals->saveSlotID != NO_SAVE_SLOT && !ActClear->field_20) {
                ActClear->field_10 = 1;
                SaveGame_SaveFile(ActClear_SaveGameCallback);
            }
        }
        if (ActClear->field_10)
            UIWaitSpinner_Wait();

        if (ActClear->field_10) {
            entity->state = ActClear_Unknown10;
        }
        else {
            if (ActClear->actID > 0 || Zone->stageFinishCallback) {
                entity->state = ActClear_State_ActFinish;
            }
            else {
                entity->state    = 0;
                EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);
                zone->screenID   = 4;
                zone->timer      = 0;
                zone->fadeSpeed  = 10;
                zone->fadeColour = 0x000000;
                zone->state      = Zone_State_Fadeout;
                zone->stateDraw  = Zone_StateDraw_Fadeout;
                zone->visible    = true;
                zone->drawOrder  = 15;
            }
        }
    }
}

void ActClear_Unknown9(void)
{
    RSDK_THIS(ActClear);

    if (entity->dword78 > 0) {
        if (TimeAttackData->field_14 <= 0 /*|| ReplayRecorder->dword13C*/) {
            --entity->dword78;
        }
        else {
            if (entity->dword78 == 120) {
                if (TimeAttackData->field_14 == 1)
                    entity->field_80 = 1;
                entity->field_7C = 1;
                RSDK.PlaySFX(ActClear->sfx_Event, 0, 255);
            }

            if (entity->dword78 != 30) {
                --entity->dword78;
            }
            else {
                if (TimeAttackData->field_14 == 1) {
                    RSDK.PlaySFX(Announcer->sfx_NewRecordTop, 0, 255);
                    --entity->dword78;
                }
                else {
                    if (TimeAttackData->field_14 > 3) {
                        --entity->dword78;
                    }
                    else {
                        RSDK.PlaySFX(Announcer->sfx_NewRecordMid, 0, 255);
                        --entity->dword78;
                    }
                }
            }
        }
    }

    if (ActClear->field_10 || ActClear->field_14) {
    }
    else {
        if (RSDK_controller->keyY.press) {
            if (!ActClear->field_2C) {
                if (HUD->replaySaveEnabled) {
                    /*if (!UIDialog->activeDialog) {
                        if (User.CheckDLC(DLC_PLUS)) {
                            if (ActClear->saveReplay_CB) {
                                ActClear->saveReplay_CB();
                            }
                            ActClear->field_2C    = 1;
                            ActClear->field_14 = 1;
                            return;
                        }
                    }*/
                }
            }
        }
        if (RSDK_controller->keyStart.press) {
            RSDK.PlaySFX(UIWidgets->sfx_Accept, 0, 255);

            RSDK_THIS(ActClear);
            if (ActClear->actID > 0 || Zone->stageFinishCallback) {
                entity->state = ActClear_State_ActFinish;
            }
            else {
                entity->state    = 0;
                EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);
                zone->screenID   = 4;
                zone->timer      = 0;
                zone->fadeSpeed  = 10;
                zone->fadeColour = 0;
                zone->state      = Zone_State_Fadeout;
                zone->stateDraw  = Zone_StateDraw_Fadeout;
                zone->visible    = true;
                zone->drawOrder  = 15;
            }
        }
    }
}

void ActClear_Unknown10(void)
{
    if (ActClear->field_10) {
        RSDK_THIS(ActClear);
        if (ActClear->actID > 0 || Zone->stageFinishCallback) {
            entity->state = ActClear_State_ActFinish;
        }
        else {
            entity->state    = StateMachine_None;
            EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);
            zone->screenID   = 4;
            zone->timer      = 0;
            zone->fadeSpeed  = 10;
            zone->fadeColour = 0;
            zone->state      = Zone_State_Fadeout;
            zone->stateDraw  = Zone_StateDraw_Fadeout;
            zone->visible    = true;
            zone->drawOrder  = 15;
        }
    }
}

void ActClear_State_ActFinish(void)
{
    RSDK_THIS(ActClear);
    entity->posUnknown.x += 0x200000;
    entity->posUnknown3.x -= 0x200000;
    if (entity->posUnknown.x > 0x400000)
        entity->posUnknown2.x += 0x200000;
    if (entity->posUnknown2.x > 0x400000)
        entity->posUnknown4.x += 0x200000;
    if (entity->posUnknown4.x > 0x400000)
        entity->posUnknown5.x += 0x200000;
    if (entity->posUnknown5.x > 0x400000)
        entity->posUnknown6.x -= 0x200000;

    if (entity->posUnknown6.x < -0x2000000) {
        if (ActClear->actID <= 0) {
            if (Zone->stageFinishCallback) {
                if (Zone->forcePlayerOnScreenFlag) {
                    entity->timer = 0;
                    entity->state = ActClear_ForcePlayerOnScreen;
                }
                else {
                    foreach_active(Animals, animal)
                    {
                        if (animal->behaviour == 1)
                            animal->behaviour = 0;
                    }
                    Zone->stageFinishCallback();
                    Zone->stageFinishCallback = NULL;
                }
            }
        }
        else {
            ActClear->field_1C           = 1;
            ActClear->actID              = 0;
            RSDK_sceneInfo->milliseconds = 0;
            RSDK_sceneInfo->seconds      = 0;
            RSDK_sceneInfo->minutes      = 0;
            foreach_active(Player, player)
            {
                player->ringExtraLife = 100;
                player->rings         = 0;
            }
        }
        if (entity->state != ActClear_ForcePlayerOnScreen)
            RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }
}

void ActClear_ForcePlayerOnScreen(void)
{
    RSDK_THIS(ActClear);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    bool32 flagA          = false;
    bool32 flagB          = false;
    int screenOffX        = RSDK_screens->width - 16 + RSDK_screens->position.x;
    screenOffX <<= 16;
    player1->up        = false;
    player1->down      = false;
    player1->jumpPress = false;
    player1->jumpHold  = false;

    if (player1->position.x >= screenOffX) {
        player1->inputState = StateMachine_None;
        player1->state      = Player_State_Ground;
        player1->groundVel  = -0x40000;
        player1->left       = true;
        player1->right      = false;
    }
    else {
        if ((!player1->onGround || player1->groundVel) && player1->position.x < screenOffX - (RSDK_screens->centerX << 15)) {
            if (!player1->skidding) {
                if (!player1->left) {
                    player1->right = false;
                }
                else {
                    player1->left = false;
                }
            }
            else {
                player1->right = true;
            }
        }

        if (!player1->onGround && !player1->groundVel) {
            flagB = true;
        }
    }

    if (player2->objectID != Player->objectID) {
        flagA = true;
    }
    else {
        player2->up        = false;
        player2->down      = false;
        player2->jumpPress = false;
        player2->jumpHold  = false;
        if (player2->state == Player_State_FlyIn || player2->state == Player_State_JumpIn) {
            if (player2->position.x < screenOffX) {
                if (player2->onGround && !player2->groundVel) {
                    RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_IDLE, &player2->playerAnimData, 0, 0);
                    player2->direction = FLIP_NONE;
                    flagA              = true;
                }
            }
        }
        else if (player2->position.x >= screenOffX) {
            player2->inputState = Player_GetP2Inputs;
            RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_RUN, &player2->playerAnimData, 0, 0);
            player2->state     = Player_State_Ground;
            player2->groundVel = -0x40000;
            player2->left      = true;
            player2->right     = false;

            if (player2->position.x < screenOffX) {
                if (player2->onGround && !player2->groundVel) {
                    RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_IDLE, &player2->playerAnimData, 0, 0);
                    player2->direction = FLIP_NONE;
                    flagA              = true;
                }
            }
        }
        else {
            if (player2->onGround && !player2->groundVel) {
                RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_IDLE, &player2->playerAnimData, 0, 0);
                player2->direction = FLIP_NONE;
                flagA              = true;
            }
        }
    }

    if (flagB) {
        RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_IDLE, &player1->playerAnimData, 0, 0);
        player1->direction = FLIP_NONE;
    }
    ++entity->stageFinishTimer;
    if ((flagB && flagA) || entity->stageFinishTimer >= 900) {
        if (entity->timer >= 10) {
            Player->jumpInDelay = 0;
            Zone->stageFinishCallback();
            Zone->stageFinishCallback = NULL;
            RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
        }
        else {
            entity->timer++;
        }
    }
}

void ActClear_EditorDraw(void) {}

void ActClear_EditorLoad(void) {}

void ActClear_Serialize(void) {}
