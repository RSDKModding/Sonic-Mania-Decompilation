#include "SonicMania.h"

ObjectTitleCard *TitleCard;

void TitleCard_Update(void)
{
    RSDK_THIS(TitleCard);
    StateMachine_Run(entity->state);
}

void TitleCard_LateUpdate(void) {}

void TitleCard_StaticUpdate(void) {}

void TitleCard_Draw(void)
{
    RSDK_THIS(TitleCard);
    StateMachine_Run(entity->stateDraw);
}

void TitleCard_Create(void *data)
{
    RSDK_THIS(TitleCard);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active      = ACTIVE_ALWAYS;
        entity->visible     = true;
        entity->drawOrder   = Zone->hudDrawOrder;
        entity->enableIntro = globals->enableIntro;
        if (!globals->suppressTitlecard || globals->enableIntro || globals->gameMode == MODE_TIMEATTACK)
            RSDK_sceneInfo->timeEnabled = false;
        TitleCard_SetColours();

        if (globals->suppressTitlecard) {
            if (TitleCard->suppressCallback) {
                TitleCard->suppressCallback();
                TitleCard->suppressCallback = NULL;
            }
            entity->state = TitleCard_Unknown11;
        }
        else {
            if (!globals->atlEnabled)
                Zone->timer2 = 0;
            entity->state     = TitleCard_Unknown6;
            entity->stateDraw = TitleCard_Unknown12;
        }

        entity->dword70 = (RSDK_screens->centerX - 152) << 16;
        entity->dword74 = (RSDK_screens->centerX - 152) << 16;
        entity->dword78 = (RSDK_screens->centerX - 160) << 16;
        entity->dword7C = (RSDK_screens->centerX + 20) << 16;
        TitleCard_Unknown2();
        TitleCard_SetPoints();

        entity->decorationPos.y = -0x340000;
        entity->decorationPos.x = (RSDK_screens->width - 160) << 16;
        RSDK.SetSpriteAnimation(TitleCard->spriteIndex, 0, &entity->decorationData, true, 0);
        RSDK.SetSpriteAnimation(TitleCard->spriteIndex, 1, &entity->nameLetterData, true, 0);
        RSDK.SetSpriteAnimation(TitleCard->spriteIndex, 2, &entity->zoneLetterData, true, 0);
        RSDK.SetSpriteAnimation(TitleCard->spriteIndex, 3, &entity->actNumbersData, true, 0);
        if (entity->actID > 3)
            entity->actID = 3;

        entity->actNumbersData.frameID = entity->actID;
        entity->drawPos2.y             = 0xA80000;
        entity->drawPos2.x             = (RSDK_screens->centerX + 106) << 16;
        entity->dword2F4               = -0x400;
        if (entity->dword2EC - entity->field_2E4 < 0x100000) {
            int dif = (entity->dword2EC - entity->field_2E4) - 0x100000;
            entity->field_2E8 -= dif;
            entity->dword1E8 -= dif;
            entity->drawPos2.x -= dif;
            entity->dword2EC = entity->dword2EC - dif;
        }

#if RETRO_USE_PLUS
        if (Zone->swapGameMode) {
            Zone->swapGameMode           = false;
            globals->enableIntro         = false;
            globals->suppressTitlecard   = false;
            RSDK_sceneInfo->milliseconds = globals->restartMilliseconds;
            RSDK_sceneInfo->seconds      = globals->restartSeconds;
            RSDK_sceneInfo->minutes      = globals->restartMinutes;
            RSDK_sceneInfo->timeEnabled  = true;
            EntityPlayer *player         = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
            RSDK.CopyEntity(player, (Entity *)Zone->entityData, false);
            RSDK.SetSpriteAnimation(player->spriteIndex, player->playerAnimator.animationID, &player->playerAnimator, false,
                                    player->playerAnimator.frameID);

            if (player->camera)
                RSDK.CopyEntity(player->camera, (Entity *)Zone->entityData[8], false);
            Player_ApplyShieldEffect(player);
            if (player->invincibleTimer > 0 && player->superState != 2) {
                RSDK.ResetEntityPtr(RSDK.GetEntityByID(Player->playerCount + RSDK.GetEntityID(player)), InvincibleStars->objectID, player);
            }

            if (player->speedShoesTimer > 0 || player->superState == 2) {
                RSDK.ResetEntityPtr(RSDK.GetEntityByID(2 * Player->playerCount + RSDK.GetEntityID(player)), ImageTrail->objectID, player);
            }

            memset(Zone->entityData, 0, ENTITY_SIZE);
            memset(Zone->entityData[8], 0, ENTITY_SIZE);
        }
#endif
    }
}

void TitleCard_StageLoad(void)
{
    TitleCard->spriteIndex = RSDK.LoadSpriteAnimation("Global/TitleCard.bin", SCOPE_STAGE);

    foreach_all(TitleCard, titleCard) {
        Zone->actID = titleCard->actID;
    }
}

void TitleCard_SetColours(void)
{
    RSDK_THIS(TitleCard);
#if RETRO_USE_PLUS
    if (RSDK_sceneInfo->filter == SCN_FILTER_ENCORE) {
        entity->colours[0] = 0x3751A2;
        entity->colours[1] = 0xC7525B;
        entity->colours[2] = 0x428FC3;
        entity->colours[3] = 0xDFB13F;
        entity->colours[4] = 0x6BAE99;
    }
    else {
#endif
        entity->colours[0] = 0xF08C18;
        entity->colours[1] = 0x60C0A0;
        entity->colours[2] = 0xF05030;
        entity->colours[3] = 0x4060B0;
        entity->colours[4] = 0xF0C800;
#if RETRO_USE_PLUS
    }
#endif
}

void TitleCard_SetPoints(void)
{
    RSDK_THIS(TitleCard);

    entity->points0[0].x = 0xF00000;
    entity->points0[0].y = 0x1F00000;
    entity->points0[1].x = 0x2F00000;
    entity->points0[1].y = 0x3F00000;
    entity->points0[2].x = 0;
    entity->points0[2].y = 0x8A0000;
    entity->points0[3].x = 0x4A0000;
    entity->points0[3].y = 0x700000;

    if (entity->word2Offset > 0) {
        entity->points1[0].x = -entity->field_2E0;
        entity->points1[0].y = 0x520000;
        entity->points1[1].x = 0;
        entity->points1[1].y = 0x520000;
        entity->points1[2].x = 0;
        entity->points1[2].y = 0x620000;
        entity->points1[3].x = -entity->field_2E0;
        entity->points1[3].y = 0x620000;
    }

    entity->points2[0].x = -entity->field_2E4;
    entity->points2[0].y = 0xBA0000;
    entity->points2[1].x = 0;
    entity->points2[1].y = 0xBA0000;
    entity->points2[2].x = 0;
    entity->points2[2].y = 0xCA0000;
    entity->points2[3].x = -entity->field_2E4;
    entity->points2[3].y = 0xCA0000;

    entity->points3[0].x = RSDK_screens->width << 16;
    entity->points3[0].y = 0x9A0000;
    entity->points3[1].x = 0x780000 + entity->points3[0].x;
    entity->points3[1].y = 0x9A0000;
    entity->points3[2].x = 0x780000 + entity->points3[0].x;
    entity->points3[2].y = 0xA20000;
    entity->points3[3].x = entity->points3[0].x;
    entity->points3[3].y = 0xA20000;

    entity->points4[0].x = entity->dword70;
    entity->points4[0].y = 0xF00000;
    entity->points4[1].x = entity->points4[0].x + 0x400000;
    entity->points4[1].y = 0xF00000;
    entity->points4[2].x = entity->points4[0].x + 0x1300000;
    entity->points4[2].y = 0xF00000;
    entity->points4[3].x = entity->points4[0].x + 0xF00000;
    entity->points4[3].y = 0xF00000;

    entity->points5[0].x = entity->dword74;
    entity->points5[0].y = 0xF00000;
    entity->points5[1].x = 0x800000 + entity->points5[0].x;
    entity->points5[1].y = 0xF00000;
    entity->points5[2].x = 0xE60000 + entity->points5[0].x;
    entity->points5[2].y = 0xF00000;
    entity->points5[3].x = 0x660000 + entity->points5[0].x;
    entity->points5[3].y = 0xF00000;

    entity->points6[0].x = entity->dword78;
    entity->points6[0].y = 0xF00000;
    entity->points6[1].x = 0x600000 + entity->points6[0].x;
    entity->points6[1].y = 0xF00000;
    entity->points6[2].x = entity->points6[0].x + 0x1060000;
    entity->points6[2].y = 0xF00000;
    entity->points6[3].x = 0xA60000 + entity->points6[0].x;
    entity->points6[3].y = 0xF00000;

    entity->points7[0].x = entity->dword7C;
    entity->points7[0].y = 0xF00000;
    entity->points7[1].x = entity->points7[0].x + 0x200000;
    entity->points7[1].y = 0xF00000;
    entity->points7[2].x = 0xA00000 + entity->points7[0].x;
    entity->points7[2].y = 0xF00000;
    entity->points7[3].x = 0x800000 + entity->points7[0].x;
    entity->points7[3].y = 0xF00000;

    entity->points8[0].x = 0;
    entity->points8[0].y = 0;
    entity->points8[1].x = (entity->points4[1].x + entity->points4[0].x) >> 1;
    entity->points8[1].y = 0;
    entity->points8[2].x = (entity->points4[3].x + entity->points4[2].x) >> 1;
    entity->points8[2].y = 0xF00000;
    entity->points8[3].x = 0;
    entity->points8[3].y = 0xF00000;

    entity->points9[0].x = (entity->points4[1].x + entity->points4[0].x) >> 1;
    entity->points9[0].y = 0;
    entity->points9[1].x = RSDK_screens->width << 16;
    entity->points9[1].y = 0;
    entity->points9[2].x = RSDK_screens->width << 16;
    entity->points9[2].y = 0xF00000;
    entity->points9[3].x = (entity->points4[3].x + entity->points4[2].x) >> 1;
    entity->points9[3].y = 0xF00000;
}

void TitleCard_Unknown2(void)
{
    RSDK_THIS(TitleCard);
    if (!entity->zoneName.text)
        RSDK.SetText(&entity->zoneName, "UNTITLED", 0);
    RSDK.SetSpriteString(TitleCard->spriteIndex, 1, &entity->zoneName);

    int offset       = 0x280000;
    for (int c = 0; c < entity->zoneName.textLength; ++c) {
        entity->charPos[c].y  = offset;
        entity->charSpeeds[c] = -0x80000;
        offset += 0x100000;
    }

    for (int i = 0; i < 4; ++i) {
        entity->zoneCharPos[i] = ((2 - entity->zoneName.textLength) << 19) - ((i * 2) << 19);
        entity->zoneCharSpeed[i] = 0x40000;
    }

    for (int c = 0; c < entity->zoneName.textLength; ++c) {
        if (entity->zoneName.text[c] == 0xFFFF) {
            entity->word2Offset = c;
        }
    }

    if (entity->word2Offset) {
        entity->field_2E0 = (RSDK.GetStringWidth(TitleCard->spriteIndex, 1, &entity->zoneName, 0, entity->word2Offset - 1, 1) + 24) << 16;
        entity->field_2E4 = (RSDK.GetStringWidth(TitleCard->spriteIndex, 1, &entity->zoneName, entity->word2Offset, 0, 1) + 24) << 16;
    }
    else {
        entity->field_2E4 = (RSDK.GetStringWidth(TitleCard->spriteIndex, 1, &entity->zoneName, 0, 0, 1) + 24) << 16;
    }

    entity->dword1E8 = (RSDK_screens->centerX - ((RSDK_screens->centerX - 160) >> 3) + 72) << 16;
    entity->dword2EC = (RSDK_screens->centerX - ((RSDK_screens->centerX - 160) >> 3) + 72) << 16;
    if (entity->field_2E4 < 0x800000)
        entity->dword2EC -= 0x280000;
    entity->field_2E8 = entity->field_2E0 - entity->field_2E4 + entity->dword2EC - 0x200000;
}

void TitleCard_Unknown3(void)
{
    RSDK_THIS(TitleCard);

    if (entity->word2Offset > 0) {
        entity->points1[1].x -= 0x200000;
        if (entity->points1[1].x < entity->field_2E8 - 0x100000)
            entity->points1[1].x = entity->field_2E8 - 0x100000;

        entity->points1[2].x -= 0x200000;
        entity->points1[0].x = entity->points1[1].x - entity->field_2E0;
        if (entity->points1[2].x < entity->field_2E8)
            entity->points1[2].x = entity->field_2E8;
        entity->points1[3].x = entity->points1[2].x - entity->field_2E0;
    }

    entity->points2[1].x -= 0x200000;
    if (entity->points2[1].x < entity->dword2EC - 0x100000)
        entity->points2[1].x = entity->dword2EC - 0x100000;

    entity->points2[2].x -= 0x200000;
    entity->points2[0].x = entity->points2[1].x - entity->field_2E4;

    if (entity->points2[2].x < entity->dword2EC)
        entity->points2[2].x = entity->dword2EC;

    entity->points3[1].x += 0x200000;
    entity->points2[3].x = entity->points2[2].x - entity->field_2E4;

    if (entity->points3[1].x > entity->dword1E8 - 0x80000)
        entity->points3[1].x = entity->dword1E8 - 0x80000;

    entity->points3[2].x += 0x200000;
    entity->points3[0].x = entity->points3[1].x - 0x780000;

    if (entity->points3[2].x > entity->dword1E8)
        entity->points3[2].x = entity->dword1E8;
    entity->points3[3].x = entity->points3[2].x - 0x780000;

    if (entity->decorationPos.y < 0xC0000) {
        entity->decorationPos.x += 0x20000;
        entity->decorationPos.y += 0x20000;
    }
}

void TitleCard_Unknown4(void)
{
    RSDK_THIS(TitleCard);

    for (int c = 0; c < entity->zoneName.textLength; ++c) {
        if (entity->charPos[c].y < 0)
            entity->charSpeeds[c] += 0x28000;

        entity->charPos[c].y += entity->charSpeeds[c];
        if (entity->charPos[c].y > 0 && entity->charSpeeds[c] > 0)
            entity->charPos[c].y = 0;
    }

    for (int i = 0; i < 4; ++i) {
        if (entity->zoneCharPos[i] > 0)
            entity->zoneCharSpeed[i] -= 0x14000;

        entity->zoneCharPos[i] += entity->zoneCharSpeed[i];
        if (entity->zoneCharPos[i] < 0 && entity->zoneCharSpeed[i] < 0)
            entity->zoneCharPos[i] = 0;
    }
}
void TitleCard_Unknown5(void)
{
    foreach_active(Player, player)
    {
        if (player->camera)
            player->camera->field_90 = 0;
    }
}

void TitleCard_Unknown6(void)
{
    RSDK_THIS(TitleCard);
    if (ActClear && ActClear->dword34)
        ActClear->dword34 = 0;

    Zone_ApplyWorldBounds();
    
    if (!globals->atlEnabled && !globals->suppressTitlecard)
        RSDK.SetGameMode(ENGINESTATE_PAUSED);

    entity->timer += 24;
    if (entity->timer >= 512) {
        entity->points1[0].y += 0x200000;
        entity->points1[1].y += 0x200000;
        entity->points1[2].y += 0x200000;
        entity->points1[3].y += 0x200000;
        entity->points2[0].y -= 0x200000;
        entity->points2[1].y -= 0x200000;
        entity->points2[2].y -= 0x200000;
        entity->points2[3].y -= 0x200000;
        entity->points3[0].y += 0x200000;
        entity->points3[1].y += 0x200000;
        entity->points3[2].y += 0x200000;
        entity->points3[3].y += 0x200000;
        entity->state = TitleCard_Unknown7;
    }
    entity->points1[0].x += 0x280000;
    entity->points1[1].x += 0x280000;
    entity->points1[2].x += 0x280000;
    entity->points1[3].x += 0x280000;
    entity->points2[0].x += 0x280000;
    entity->points2[1].x += 0x280000;
    entity->points2[2].x += 0x280000;
    entity->points2[3].x += 0x280000;
    entity->points3[0].x -= 0x280000;
    entity->points3[1].x -= 0x280000;
    entity->points3[2].x -= 0x280000;
    entity->points3[3].x -= 0x280000;
}

void TitleCard_Unknown7(void)
{
    RSDK_THIS(TitleCard);
    Zone_ApplyWorldBounds();

    if (entity->timer >= 1024) {
        entity->state     = TitleCard_Unknown8;
        entity->stateDraw = TitleCard_Unknown13;
    }
    else {
        entity->timer += 32;
    }
    TitleCard_Unknown3();
}

void TitleCard_Unknown8(void)
{
    RSDK_THIS(TitleCard);
    Zone_ApplyWorldBounds();

    int val = entity->points0[0].x + (entity->points0[2].x - entity->points0[0].x - 0x100000) / 6;
    if (val < entity->points0[2].x)
        val = entity->points0[2].x;
    entity->points0[0].x = val;

    val = entity->points0[0].y + (entity->points0[2].y - entity->points0[0].y - 0x100000) / 6;
    if (val < entity->points0[2].y)
        val = entity->points0[2].y;
    entity->points0[0].y = val;

    val = entity->points0[1].x + (entity->points0[3].x - entity->points0[1].x - 0x100000) / 6;
    if (val < entity->points0[3].x)
        val = entity->points0[3].x;
    entity->points0[1].x = val;

    val = entity->points0[1].y + (entity->points0[3].y - entity->points0[1].y - 0x100000) / 6;
    if (val < entity->points0[3].y)
        val = entity->points0[3].y;
    entity->points0[1].y = val;

    entity->points4[0].x = (entity->points0[0].x - 0xF00000) + entity->points4[3].x;
    entity->points4[1].x = (entity->points0[0].x - 0xF00000) + entity->points4[2].x;

    entity->points4[0].y = entity->points0[0].x;
    entity->points4[1].y = entity->points0[0].x;
    entity->points5[0].x = (entity->points0[0].y - 0xF00000) + entity->points5[3].x;
    entity->points5[1].x = (entity->points0[0].y - 0xF00000) + entity->points5[2].x;

    entity->points5[0].y = entity->points0[0].y;
    entity->points5[1].y = entity->points0[0].y;
    entity->points6[0].x = (entity->points0[1].x - 0xF00000) + entity->points6[3].x;
    entity->points6[1].x = (entity->points0[1].x - 0xF00000) + entity->points6[2].x;

    entity->points6[0].y = entity->points0[1].x;
    entity->points6[1].y = entity->points0[1].x;
    entity->points7[0].x = (entity->points0[1].y - 0xF00000) + entity->points7[3].x;
    entity->points7[1].x = (entity->points0[1].y - 0xF00000) + entity->points7[2].x;
    entity->points7[0].y = entity->points0[1].y;
    entity->points7[1].y = entity->points0[1].y;

    TitleCard_Unknown3();
    TitleCard_Unknown4();
    if (entity->dword2F4 < 0x300)
        entity->dword2F4 += 0x40;

    if (!entity->zoneCharPos[3] && entity->zoneCharSpeed[3] < 0)
        entity->state = TitleCard_Unknown9;
}
void TitleCard_Unknown9(void)
{
    RSDK_THIS(TitleCard);
    Zone_ApplyWorldBounds();
    TitleCard_Unknown5();

    if (entity->field_60 >= 60) {
        entity->field_60  = 0;
        entity->state     = TitleCard_Unknown10;
        entity->stateDraw = TitleCard_Unknown14;
        RSDK.SetGameMode(ENGINESTATE_REGULAR);
        if (TitleCard->funcA)
            TitleCard->funcA();
    }
    else {
        entity->field_60++;
        if (entity->field_60 == 16) {
            if (Zone->field_158) {
                EntityCamera *camera   = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
                camera->state          = Camera_State_Follow;
                EntityPlayer *player   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
                player->camera         = camera;
                camera->targetPtr      = (Entity*)player;
                Camera->centerBounds.x = 0x20000;
                Camera->centerBounds.y = 0x20000;
            }
            Zone->field_158 = false;
        }
    }
}

void TitleCard_Unknown10(void)
{
    RSDK_THIS(TitleCard);
    Zone_ApplyWorldBounds();
    int speed = ++entity->field_60 << 18;
    entity->points7[0].x -= speed;
    entity->points7[0].y -= speed;
    entity->points7[1].x -= speed;
    entity->points7[1].y -= speed;
    entity->points7[2].x -= speed;
    entity->points7[2].y -= speed;
    entity->points7[3].x -= speed;
    entity->points7[3].y -= speed;

    if (entity->field_60 > 6) {
        speed = (entity->field_60 - 6) << 18;
        entity->points6[0].x -= speed;
        entity->points6[0].y -= speed;
        entity->points6[1].x -= speed;
        entity->points6[1].y -= speed;
        entity->points6[2].x -= speed;
        entity->points6[2].y -= speed;
        entity->points6[3].x -= speed;
        entity->points6[3].y -= speed;
        entity->decorationPos.x += speed;
        entity->decorationPos.y += speed;
    }

    if (entity->field_60 > 12) {
        speed = (entity->field_60 - 12) << 18;
        entity->points5[0].x -= speed;
        entity->points5[0].y -= speed;
        entity->points5[1].x -= speed;
        entity->points5[1].y -= speed;
        entity->points5[2].x -= speed;
        entity->points5[2].y -= speed;
        entity->points5[3].x -= speed;
        entity->points5[3].y -= speed;
    }

    if (entity->field_60 > 18) {
        speed = (entity->field_60 - 12) << 18;
        entity->points4[0].x -= speed;
        entity->points4[0].y -= speed;
        entity->points4[1].x -= speed;
        entity->points4[1].y -= speed;
        entity->points4[2].x -= speed;
        entity->points4[2].y -= speed;
        entity->points4[3].x -= speed;
        entity->points4[3].y -= speed;
    }

    if (entity->field_60 > 4) {
        speed = (entity->field_60 - 4) << 17;
        entity->points8[0].x -= speed;
        entity->points8[1].x -= speed;
        entity->points8[2].x -= speed;
        entity->points8[3].x -= speed;
        entity->points9[0].x += speed;
        entity->points9[1].x += speed;
        entity->points9[2].x += speed;
        entity->points9[3].x += speed;
    }

    if (entity->field_60 > 60) {
        speed = 0x200000;
        entity->dword1E8 -= speed;
        entity->field_2E8 -= speed;
        entity->dword2EC += speed;
        entity->drawPos2.x += speed;
        entity->drawPos2.y += speed;
        entity->points1[0].x -= speed;
        entity->points1[1].x -= speed;
        entity->points1[2].x -= speed;
        entity->points1[3].x -= speed;
        entity->points2[0].x += speed;
        entity->points2[1].x += speed;
        entity->points2[2].x += speed;
        entity->points2[3].x += speed;
        entity->points3[0].x -= speed;
        entity->points3[1].x -= speed;
        entity->points3[2].x -= speed;
        entity->points3[3].x -= speed;
    }

    if (entity->field_60 == 6) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            RSDK_sceneInfo->timeEnabled = true;
        }
    }
    if (entity->field_60 > 80) {
        globals->atlEnabled  = false;
        globals->enableIntro = false;
        if (globals->gameMode >= MODE_TIMEATTACK) {
            if (globals->gameMode == MODE_COMPETITION) {
                // Competition_Unknown3();
                Announcer_Unknown1();
            }
            RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
        }
        else {
            globals->suppressTitlecard = false;
            globals->suppressAutoMusic = false;
            RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
        }
    }
}

void TitleCard_Unknown11(void)
{
    RSDK_THIS(TitleCard);
    TitleCard_Unknown5();
    RSDK.SetGameMode(ENGINESTATE_REGULAR);
    globals->atlEnabled = false;
    if (globals->gameMode == MODE_TIMEATTACK || globals->enableIntro)
        RSDK_sceneInfo->timeEnabled = false;

    entity->active = ACTIVE_NEVER;
    if ((globals->suppressTitlecard && !entity->enableIntro) || globals->gameMode == MODE_TIMEATTACK) {
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
        globals->suppressTitlecard = false;
    }

    if (TitleCard->funcA)
        TitleCard->funcA();
}

void TitleCard_Unknown12(void)
{
    RSDK_THIS(TitleCard);
    if (!globals->atlEnabled && !globals->suppressTitlecard) {
        if (entity->timer < 256)
            RSDK.DrawRect(0, 0, RSDK_screens->width, RSDK_screens->height, 0, 0xFF, INK_NONE, true);

        if (entity->timer < 512)
            RSDK.DrawRect(0, RSDK_screens->centerY - (entity->timer >> 1), RSDK_screens->width, entity->timer, entity->colours[3], 0xFF, INK_NONE,
                          true);

        int val = entity->timer - 128;
        if (val > 0) {
            if (val < 512)
                RSDK.DrawRect(0, RSDK_screens->centerY - (val >> 1), RSDK_screens->width, val, entity->colours[2], 0xFF, INK_NONE, true);

            val -= 128;
            if (val > 0) {
                if (val < 512)
                    RSDK.DrawRect(0, RSDK_screens->centerY - (val >> 1), RSDK_screens->width, val, entity->colours[0], 0xFF, INK_NONE, true);

                val -= 128;
                if (val > 0) {
                    if (val < 512)
                        RSDK.DrawRect(0, RSDK_screens->centerY - (val >> 1), RSDK_screens->width, val, entity->colours[1], 0xFF, INK_NONE, true);

                    val -= 128;
                    if (val > 0)
                        RSDK.DrawRect(0, RSDK_screens->centerY - (val >> 1), RSDK_screens->width, val, entity->colours[4], 0xFF, INK_NONE, true);
                }
            }
        }
    }

    if (entity->word2Offset > 0)
        RSDK.DrawQuad(entity->points1, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawQuad(entity->points2, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawQuad(entity->points3, 4, 0xF0, 0xF0, 0xF0, 0xFF, INK_NONE);
#if RETRO_USE_PLUS
    entity->decorationData.frameID = 2 * (RSDK_sceneInfo->filter == SCN_FILTER_ENCORE) + 1;
#else
    entity->decorationData.frameID = 2 * 0 + 1;
#endif
    RSDK.DrawSprite(&entity->decorationData, &entity->decorationPos, true);
}

void TitleCard_Unknown13(void)
{
    RSDK_THIS(TitleCard);
    if (!globals->atlEnabled && !globals->suppressTitlecard)
        RSDK.DrawRect(0, 0, RSDK_screens->width, RSDK_screens->height, entity->colours[4], 0xFF, INK_NONE, true);
    if (entity->points0[1].x < 0xF00000)
        RSDK.DrawQuad(entity->points6, 4, (entity->colours[0] >> 16) & 0xFF, (entity->colours[0] >> 8) & 0xFF, (entity->colours[0] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
    if (entity->points0[1].y < 0xF00000)
        RSDK.DrawQuad(entity->points7, 4, (entity->colours[1] >> 16) & 0xFF, (entity->colours[1] >> 8) & 0xFF, (entity->colours[1] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
    if (entity->points0[0].y < 0xF00000)
        RSDK.DrawQuad(entity->points5, 4, (entity->colours[2] >> 16) & 0xFF, (entity->colours[2] >> 8) & 0xFF, (entity->colours[2] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
    if (entity->points0[0].x < 0xF00000)
        RSDK.DrawQuad(entity->points4, 4, (entity->colours[3] >> 16) & 0xFF, (entity->colours[3] >> 8) & 0xFF, (entity->colours[3] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
    if (!globals->atlEnabled && globals->suppressTitlecard == false) {
#if RETRO_USE_PLUS
        entity->decorationData.frameID = 2 * (RSDK_sceneInfo->filter == SCN_FILTER_ENCORE) + 1;
#else
        entity->decorationData.frameID = 2 * 0 + 1;
#endif
        RSDK.DrawSprite(&entity->decorationData, &entity->decorationPos, true);
    }

    if (entity->word2Offset > 0)
        RSDK.DrawQuad(entity->points1, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawQuad(entity->points2, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawQuad(entity->points3, 4, 0xF0, 0xF0, 0xF0, 0xFF, INK_NONE);

    Vector2 drawPos;
    drawPos.x = entity->dword1E8;
    RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, 0, 170, RSDK_screens[RSDK_sceneInfo->currentScreenID].width, 240);

    for (int i = 0; i < 4; ++i) {
        entity->zoneLetterData.frameID = i;
        drawPos.y                      = 0xBA0000 + entity->zoneCharPos[i];
        RSDK.DrawSprite(&entity->zoneLetterData, &drawPos, true);
    }

    if (entity->word2Offset > 0) {
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, 0, 0, RSDK_screens[RSDK_sceneInfo->currentScreenID].width, 130);
        drawPos.y = 0x720000;
        drawPos.x = entity->field_2E8 - 0x140000;
        RSDK.DrawText(&entity->nameLetterData, &drawPos, &entity->zoneName, 0, entity->word2Offset, ALIGN_RIGHT, 1, 0, entity->charPos, true);
    }

    RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, 0, 0, RSDK_screens[RSDK_sceneInfo->currentScreenID].width, 170);
    drawPos.y = 0x9A0000;
    drawPos.x = entity->dword2EC - 0x140000;
    RSDK.DrawText(&entity->nameLetterData, &drawPos, &entity->zoneName, entity->word2Offset, 0, ALIGN_RIGHT, 1, 0, entity->charPos, true);

    RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, 0, 0, RSDK_screens[RSDK_sceneInfo->currentScreenID].width,
                       RSDK_screens[RSDK_sceneInfo->currentScreenID].height);
    if (entity->actID != 3) {
        if (entity->dword2F4 > 0) {
            entity->drawFX  = FX_SCALE;
            entity->scale.y = 0x200;
            if (entity->dword2F4 > 512)
                entity->scale.x = 512;
            else
                entity->scale.x = entity->dword2F4;

#if RETRO_USE_PLUS
            entity->decorationData.frameID = (RSDK_sceneInfo->filter == SCN_FILTER_ENCORE) ? 2 : 0;
#else
            entity->decorationData.frameID = 0;
#endif
            RSDK.DrawSprite(&entity->decorationData, &entity->drawPos2, true);

            entity->scale.x = entity->dword2F4 - 0x100;
            if (entity->scale.x < 0) {
                entity->scale.x = 0;
            }
            else if (entity->scale.x > 0x200) {
                entity->scale.x = 0x200;
            }

            RSDK.DrawSprite(&entity->actNumbersData, &entity->drawPos2, true);
            entity->drawFX = FX_NONE;
        }
    }
}

void TitleCard_Unknown14(void)
{
    RSDK_THIS(TitleCard);
    if (!globals->atlEnabled && !globals->suppressTitlecard) {
        RSDK.DrawQuad(entity->points8, 4, (entity->colours[4] >> 16) & 0xFF, (entity->colours[4] >> 8) & 0xFF, (entity->colours[4] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
        RSDK.DrawQuad(entity->points9, 4, (entity->colours[4] >> 16) & 0xFF, (entity->colours[4] >> 8) & 0xFF, (entity->colours[4] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
    }
    if (entity->points0[1].x < 0xF00000)
        RSDK.DrawQuad(entity->points6, 4, (entity->colours[0] >> 16) & 0xFF, (entity->colours[0] >> 8) & 0xFF, (entity->colours[0] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
    if (entity->points0[1].y < 0xF00000)
        RSDK.DrawQuad(entity->points7, 4, (entity->colours[1] >> 16) & 0xFF, (entity->colours[1] >> 8) & 0xFF, (entity->colours[1] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
    if (entity->points0[0].y < 0xF00000)
        RSDK.DrawQuad(entity->points5, 4, (entity->colours[2] >> 16) & 0xFF, (entity->colours[2] >> 8) & 0xFF, (entity->colours[2] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
    if (entity->points0[0].x < 0xF00000)
        RSDK.DrawQuad(entity->points4, 4, (entity->colours[3] >> 16) & 0xFF, (entity->colours[3] >> 8) & 0xFF, (entity->colours[3] >> 0) & 0xFF, 0xFF,
                      INK_NONE);

    if (!globals->atlEnabled && !globals->suppressTitlecard) {
#if RETRO_USE_PLUS
        entity->decorationData.frameID = 2 * (RSDK_sceneInfo->filter == SCN_FILTER_ENCORE) + 1;
#else
        entity->decorationData.frameID = 2 * 0 + 1;
#endif
        RSDK.DrawSprite(&entity->decorationData, &entity->decorationPos, true);
    }

    if (entity->actID != 3 && entity->dword2F4 > 0) {
#if RETRO_USE_PLUS
        entity->decorationData.frameID = (RSDK_sceneInfo->filter == SCN_FILTER_ENCORE) ? 2 : 0;
#else
        entity->decorationData.frameID = 0;
#endif
        RSDK.DrawSprite(&entity->decorationData, &entity->drawPos2, true);
        RSDK.DrawSprite(&entity->actNumbersData, &entity->drawPos2, true);
    }
    if (entity->word2Offset > 0)
        RSDK.DrawQuad(entity->points1, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawQuad(entity->points2, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawQuad(entity->points3, 4, 0xF0, 0xF0, 0xF0, 0xFF, INK_NONE);

    Vector2 drawPos;
    drawPos.x                      = entity->dword1E8;
    drawPos.y                      = 0xBA0000;
    entity->zoneLetterData.frameID = 0;
    RSDK.DrawSprite(&entity->zoneLetterData, &drawPos, true);

    entity->zoneLetterData.frameID = 1;
    RSDK.DrawSprite(&entity->zoneLetterData, &drawPos, true);

    entity->zoneLetterData.frameID = 2;
    RSDK.DrawSprite(&entity->zoneLetterData, &drawPos, true);

    entity->zoneLetterData.frameID = 3;
    RSDK.DrawSprite(&entity->zoneLetterData, &drawPos, true);

    if (entity->word2Offset > 0) {
        drawPos.y = 0x720000;
        drawPos.x = entity->field_2E8 - 0x140000;
        RSDK.DrawText(&entity->nameLetterData, &drawPos, &entity->zoneName, 0, entity->word2Offset, ALIGN_RIGHT, 1, 0, 0, true);
    }

    drawPos.y = 0x9A0000;
    drawPos.x = entity->dword2EC - 0x140000;
    RSDK.DrawText(&entity->nameLetterData, &drawPos, &entity->zoneName, entity->word2Offset, 0, ALIGN_RIGHT, 1, 0, 0, true);
}

void TitleCard_EditorDraw(void) {}

void TitleCard_EditorLoad(void) {}

void TitleCard_Serialize(void)
{
    RSDK_EDITABLE_VAR(TitleCard, VAR_STRING, zoneName);
    RSDK_EDITABLE_VAR(TitleCard, VAR_UINT8, actID);
}
