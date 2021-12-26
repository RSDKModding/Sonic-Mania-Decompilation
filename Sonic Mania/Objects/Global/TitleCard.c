// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TitleCard Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectTitleCard *TitleCard;

void TitleCard_Update(void)
{
    RSDK_THIS(TitleCard);
    StateMachine_Run(self->state);
}

void TitleCard_LateUpdate(void) {}

void TitleCard_StaticUpdate(void) {}

void TitleCard_Draw(void)
{
    RSDK_THIS(TitleCard);
    StateMachine_Run(self->stateDraw);
}

void TitleCard_Create(void *data)
{
    RSDK_THIS(TitleCard);
    if (!SceneInfo->inEditor) {
        self->active      = ACTIVE_ALWAYS;
        self->visible     = true;
        self->drawOrder   = Zone->hudDrawOrder;
        self->enableIntro = globals->enableIntro;
        if (!globals->suppressTitlecard || globals->enableIntro || globals->gameMode == MODE_TIMEATTACK)
            SceneInfo->timeEnabled = false;
        TitleCard_SetColours();

        if (globals->suppressTitlecard) {
            StateMachine_Run(TitleCard->suppressCB);
            TitleCard->suppressCB = NULL;
            self->state           = TitleCard_State_Supressed;
        }
        else {
            if (!globals->atlEnabled)
                Zone->timer2 = 0;
            self->state     = TitleCard_State_Initial;
            self->stateDraw = TitleCard_Draw_Default;
        }

        self->barPos[0] = (ScreenInfo->centerX - 152) << 16;
        self->barPos[1] = (ScreenInfo->centerX - 152) << 16;
        self->barPos[2] = (ScreenInfo->centerX - 160) << 16;
        self->barPos[3] = (ScreenInfo->centerX + 20) << 16;
        TitleCard_SetWordPositions();
        TitleCard_SetPoints();

        self->decorationPos.y = -0x340000;
        self->decorationPos.x = (ScreenInfo->width - 160) << 16;
        RSDK.SetSpriteAnimation(TitleCard->aniFrames, 0, &self->decorationAnimator, true, 0);
        RSDK.SetSpriteAnimation(TitleCard->aniFrames, 1, &self->nameLetterAnimator, true, 0);
        RSDK.SetSpriteAnimation(TitleCard->aniFrames, 2, &self->zoneLetterAnimator, true, 0);
        RSDK.SetSpriteAnimation(TitleCard->aniFrames, 3, &self->actNumbersAnimator, true, 0);
        if (self->actID > 3)
            self->actID = 3;

        self->actNumbersAnimator.frameID = self->actID;
        self->actNumPos.y             = 0xA80000;
        self->actNumPos.x             = (ScreenInfo->centerX + 106) << 16;
        self->actNumScale               = -0x400;
        if (self->wordStopPos - self->bottomWordOffset < 0x100000) {
            int32 dif = (self->wordStopPos - self->bottomWordOffset) - 0x100000;
            self->currentWordPos -= dif;
            self->zoneStopPos -= dif;
            self->actNumPos.x -= dif;
            self->wordStopPos = self->wordStopPos - dif;
        }

#if RETRO_USE_PLUS
        if (Zone->swapGameMode) {
            Zone->swapGameMode         = false;
            globals->enableIntro       = false;
            globals->suppressTitlecard = false;
            SceneInfo->milliseconds    = globals->restartMilliseconds;
            SceneInfo->seconds         = globals->restartSeconds;
            SceneInfo->minutes         = globals->restartMinutes;
            SceneInfo->timeEnabled     = true;
            EntityPlayer *player       = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            RSDK.CopyEntity(player, (Entity *)Zone->entityData, false);
            RSDK.SetSpriteAnimation(player->aniFrames, player->animator.animationID, &player->animator, false, player->animator.frameID);

            if (player->camera)
                RSDK.CopyEntity(player->camera, (Entity *)Zone->entityData[8], false);
            Player_ApplyShieldEffect(player);
            if (player->invincibleTimer > 0 && player->superState != SUPERSTATE_SUPER) {
                RSDK.ResetEntityPtr(RSDK.GetEntityByID(Player->playerCount + RSDK.GetEntityID(player)), InvincibleStars->objectID, player);
            }

            if (player->speedShoesTimer > 0 || player->superState == SUPERSTATE_SUPER) {
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
    TitleCard->aniFrames = RSDK.LoadSpriteAnimation("Global/TitleCard.bin", SCOPE_STAGE);

    foreach_all(TitleCard, titleCard) { Zone->actID = titleCard->actID; }
}

void TitleCard_SetColours(void)
{
    RSDK_THIS(TitleCard);
#if RETRO_USE_PLUS
    if (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) {
        self->colours[0] = 0x3751A2;
        self->colours[1] = 0xC7525B;
        self->colours[2] = 0x428FC3;
        self->colours[3] = 0xDFB13F;
        self->colours[4] = 0x6BAE99;
    }
    else {
#endif
        self->colours[0] = 0xF08C18;
        self->colours[1] = 0x60C0A0;
        self->colours[2] = 0xF05030;
        self->colours[3] = 0x4060B0;
        self->colours[4] = 0xF0C800;
#if RETRO_USE_PLUS
    }
#endif
}

void TitleCard_SetPoints(void)
{
    RSDK_THIS(TitleCard);

    self->points0[0].x = 0xF00000;
    self->points0[0].y = 0x1F00000;
    self->points0[1].x = 0x2F00000;
    self->points0[1].y = 0x3F00000;
    self->points0[2].x = 0;
    self->points0[2].y = 0x8A0000;
    self->points0[3].x = 0x4A0000;
    self->points0[3].y = 0x700000;

    if (self->word2Offset > 0) {
        self->points1[0].x = -self->topWordOffset;
        self->points1[0].y = 0x520000;
        self->points1[1].x = 0;
        self->points1[1].y = 0x520000;
        self->points1[2].x = 0;
        self->points1[2].y = 0x620000;
        self->points1[3].x = -self->topWordOffset;
        self->points1[3].y = 0x620000;
    }

    self->points2[0].x = -self->bottomWordOffset;
    self->points2[0].y = 0xBA0000;
    self->points2[1].x = 0;
    self->points2[1].y = 0xBA0000;
    self->points2[2].x = 0;
    self->points2[2].y = 0xCA0000;
    self->points2[3].x = -self->bottomWordOffset;
    self->points2[3].y = 0xCA0000;

    self->points3[0].x = ScreenInfo->width << 16;
    self->points3[0].y = 0x9A0000;
    self->points3[1].x = 0x780000 + self->points3[0].x;
    self->points3[1].y = 0x9A0000;
    self->points3[2].x = 0x780000 + self->points3[0].x;
    self->points3[2].y = 0xA20000;
    self->points3[3].x = self->points3[0].x;
    self->points3[3].y = 0xA20000;

    self->points4[0].x = self->barPos[0];
    self->points4[0].y = 0xF00000;
    self->points4[1].x = self->points4[0].x + 0x400000;
    self->points4[1].y = 0xF00000;
    self->points4[2].x = self->points4[0].x + 0x1300000;
    self->points4[2].y = 0xF00000;
    self->points4[3].x = self->points4[0].x + 0xF00000;
    self->points4[3].y = 0xF00000;

    self->points5[0].x = self->barPos[1];
    self->points5[0].y = 0xF00000;
    self->points5[1].x = 0x800000 + self->points5[0].x;
    self->points5[1].y = 0xF00000;
    self->points5[2].x = 0xE60000 + self->points5[0].x;
    self->points5[2].y = 0xF00000;
    self->points5[3].x = 0x660000 + self->points5[0].x;
    self->points5[3].y = 0xF00000;

    self->points6[0].x = self->barPos[2];
    self->points6[0].y = 0xF00000;
    self->points6[1].x = 0x600000 + self->points6[0].x;
    self->points6[1].y = 0xF00000;
    self->points6[2].x = self->points6[0].x + 0x1060000;
    self->points6[2].y = 0xF00000;
    self->points6[3].x = 0xA60000 + self->points6[0].x;
    self->points6[3].y = 0xF00000;

    self->points7[0].x = self->barPos[3];
    self->points7[0].y = 0xF00000;
    self->points7[1].x = self->points7[0].x + 0x200000;
    self->points7[1].y = 0xF00000;
    self->points7[2].x = 0xA00000 + self->points7[0].x;
    self->points7[2].y = 0xF00000;
    self->points7[3].x = 0x800000 + self->points7[0].x;
    self->points7[3].y = 0xF00000;

    self->points8[0].x = 0;
    self->points8[0].y = 0;
    self->points8[1].x = (self->points4[1].x + self->points4[0].x) >> 1;
    self->points8[1].y = 0;
    self->points8[2].x = (self->points4[3].x + self->points4[2].x) >> 1;
    self->points8[2].y = 0xF00000;
    self->points8[3].x = 0;
    self->points8[3].y = 0xF00000;

    self->points9[0].x = (self->points4[1].x + self->points4[0].x) >> 1;
    self->points9[0].y = 0;
    self->points9[1].x = ScreenInfo->width << 16;
    self->points9[1].y = 0;
    self->points9[2].x = ScreenInfo->width << 16;
    self->points9[2].y = 0xF00000;
    self->points9[3].x = (self->points4[3].x + self->points4[2].x) >> 1;
    self->points9[3].y = 0xF00000;
}

void TitleCard_SetWordPositions(void)
{
    RSDK_THIS(TitleCard);
    if (!self->zoneName.text)
        RSDK.SetText(&self->zoneName, "UNTITLED", 0);
    RSDK.SetSpriteString(TitleCard->aniFrames, 1, &self->zoneName);

    int32 offset = 0x280000;
    for (int32 c = 0; c < self->zoneName.length; ++c) {
        self->charPos[c].y  = offset;
        self->charSpeeds[c] = -0x80000;
        offset += 0x100000;
    }

    for (int32 i = 0; i < 4; ++i) {
        self->zoneCharPos[i]   = ((2 - self->zoneName.length) << 19) - ((i * 2) << 19);
        self->zoneCharSpeed[i] = 0x40000;
    }

    for (int32 c = 0; c < self->zoneName.length; ++c) {
        if (self->zoneName.text[c] == -1) {
            self->word2Offset = c;
        }
    }

    if (self->word2Offset) {
        self->topWordOffset    = (RSDK.GetStringWidth(TitleCard->aniFrames, 1, &self->zoneName, 0, self->word2Offset - 1, 1) + 24) << 16;
        self->bottomWordOffset = (RSDK.GetStringWidth(TitleCard->aniFrames, 1, &self->zoneName, self->word2Offset, 0, 1) + 24) << 16;
    }
    else {
        self->bottomWordOffset = (RSDK.GetStringWidth(TitleCard->aniFrames, 1, &self->zoneName, 0, 0, 1) + 24) << 16;
    }

    self->zoneStopPos = (ScreenInfo->centerX - ((ScreenInfo->centerX - 160) >> 3) + 72) << 16;
    self->wordStopPos = (ScreenInfo->centerX - ((ScreenInfo->centerX - 160) >> 3) + 72) << 16;
    if (self->bottomWordOffset < 0x800000)
        self->wordStopPos -= 0x280000;
    self->currentWordPos = self->topWordOffset - self->bottomWordOffset + self->wordStopPos - 0x200000;
}

void TitleCard_CheckPointBoundaries(void)
{
    RSDK_THIS(TitleCard);

    if (self->word2Offset > 0) {
        self->points1[1].x -= 0x200000;
        if (self->points1[1].x < self->currentWordPos - 0x100000)
            self->points1[1].x = self->currentWordPos - 0x100000;

        self->points1[2].x -= 0x200000;
        self->points1[0].x = self->points1[1].x - self->topWordOffset;
        if (self->points1[2].x < self->currentWordPos)
            self->points1[2].x = self->currentWordPos;
        self->points1[3].x = self->points1[2].x - self->topWordOffset;
    }

    self->points2[1].x -= 0x200000;
    if (self->points2[1].x < self->wordStopPos - 0x100000)
        self->points2[1].x = self->wordStopPos - 0x100000;

    self->points2[2].x -= 0x200000;
    self->points2[0].x = self->points2[1].x - self->bottomWordOffset;

    if (self->points2[2].x < self->wordStopPos)
        self->points2[2].x = self->wordStopPos;

    self->points3[1].x += 0x200000;
    self->points2[3].x = self->points2[2].x - self->bottomWordOffset;

    if (self->points3[1].x > self->zoneStopPos - 0x80000)
        self->points3[1].x = self->zoneStopPos - 0x80000;

    self->points3[2].x += 0x200000;
    self->points3[0].x = self->points3[1].x - 0x780000;

    if (self->points3[2].x > self->zoneStopPos)
        self->points3[2].x = self->zoneStopPos;
    self->points3[3].x = self->points3[2].x - 0x780000;

    if (self->decorationPos.y < 0xC0000) {
        self->decorationPos.x += 0x20000;
        self->decorationPos.y += 0x20000;
    }
}

void TitleCard_HandleZoneCharacters(void)
{
    RSDK_THIS(TitleCard);

    for (int32 c = 0; c < self->zoneName.length; ++c) {
        if (self->charPos[c].y < 0)
            self->charSpeeds[c] += 0x28000;

        self->charPos[c].y += self->charSpeeds[c];
        if (self->charPos[c].y > 0 && self->charSpeeds[c] > 0)
            self->charPos[c].y = 0;
    }

    for (int32 i = 0; i < 4; ++i) {
        if (self->zoneCharPos[i] > 0)
            self->zoneCharSpeed[i] -= 0x14000;

        self->zoneCharPos[i] += self->zoneCharSpeed[i];
        if (self->zoneCharPos[i] < 0 && self->zoneCharSpeed[i] < 0)
            self->zoneCharPos[i] = 0;
    }
}
void TitleCard_SetCamera(void)
{
    foreach_active(Player, player)
    {
        if (player->camera)
            player->camera->offset.y = 0;
    }
}

void TitleCard_State_Initial(void)
{
    RSDK_THIS(TitleCard);
#if RETRO_USE_PLUS
    if (ActClear && ActClear->actClearActive)
        ActClear->actClearActive = false;
#endif

    Zone_ApplyWorldBounds();

    if (!globals->atlEnabled && !globals->suppressTitlecard)
        RSDK.SetGameMode(ENGINESTATE_PAUSED);

    self->timer += 24;
    if (self->timer >= 512) {
        self->points1[0].y += 0x200000;
        self->points1[1].y += 0x200000;
        self->points1[2].y += 0x200000;
        self->points1[3].y += 0x200000;
        self->points2[0].y -= 0x200000;
        self->points2[1].y -= 0x200000;
        self->points2[2].y -= 0x200000;
        self->points2[3].y -= 0x200000;
        self->points3[0].y += 0x200000;
        self->points3[1].y += 0x200000;
        self->points3[2].y += 0x200000;
        self->points3[3].y += 0x200000;
        self->state = TitleCard_State_OpeningBG;
    }
    self->points1[0].x += 0x280000;
    self->points1[1].x += 0x280000;
    self->points1[2].x += 0x280000;
    self->points1[3].x += 0x280000;
    self->points2[0].x += 0x280000;
    self->points2[1].x += 0x280000;
    self->points2[2].x += 0x280000;
    self->points2[3].x += 0x280000;
    self->points3[0].x -= 0x280000;
    self->points3[1].x -= 0x280000;
    self->points3[2].x -= 0x280000;
    self->points3[3].x -= 0x280000;
}

void TitleCard_State_OpeningBG(void)
{
    RSDK_THIS(TitleCard);
    Zone_ApplyWorldBounds();

    if (self->timer >= 1024) {
        self->state     = TitleCard_State_ShowTitle;
        self->stateDraw = TitleCard_Draw_SolidBG;
    }
    else {
        self->timer += 32;
    }
    TitleCard_CheckPointBoundaries();
}

void TitleCard_State_ShowTitle(void)
{
    RSDK_THIS(TitleCard);
    Zone_ApplyWorldBounds();

    int32 val = self->points0[0].x + (self->points0[2].x - self->points0[0].x - 0x100000) / 6;
    if (val < self->points0[2].x)
        val = self->points0[2].x;
    self->points0[0].x = val;

    val = self->points0[0].y + (self->points0[2].y - self->points0[0].y - 0x100000) / 6;
    if (val < self->points0[2].y)
        val = self->points0[2].y;
    self->points0[0].y = val;

    val = self->points0[1].x + (self->points0[3].x - self->points0[1].x - 0x100000) / 6;
    if (val < self->points0[3].x)
        val = self->points0[3].x;
    self->points0[1].x = val;

    val = self->points0[1].y + (self->points0[3].y - self->points0[1].y - 0x100000) / 6;
    if (val < self->points0[3].y)
        val = self->points0[3].y;
    self->points0[1].y = val;

    self->points4[0].x = (self->points0[0].x - 0xF00000) + self->points4[3].x;
    self->points4[1].x = (self->points0[0].x - 0xF00000) + self->points4[2].x;

    self->points4[0].y = self->points0[0].x;
    self->points4[1].y = self->points0[0].x;
    self->points5[0].x = (self->points0[0].y - 0xF00000) + self->points5[3].x;
    self->points5[1].x = (self->points0[0].y - 0xF00000) + self->points5[2].x;

    self->points5[0].y = self->points0[0].y;
    self->points5[1].y = self->points0[0].y;
    self->points6[0].x = (self->points0[1].x - 0xF00000) + self->points6[3].x;
    self->points6[1].x = (self->points0[1].x - 0xF00000) + self->points6[2].x;

    self->points6[0].y = self->points0[1].x;
    self->points6[1].y = self->points0[1].x;
    self->points7[0].x = (self->points0[1].y - 0xF00000) + self->points7[3].x;
    self->points7[1].x = (self->points0[1].y - 0xF00000) + self->points7[2].x;
    self->points7[0].y = self->points0[1].y;
    self->points7[1].y = self->points0[1].y;

    TitleCard_CheckPointBoundaries();
    TitleCard_HandleZoneCharacters();
    if (self->actNumScale < 0x300)
        self->actNumScale += 0x40;

    if (!self->zoneCharPos[3] && self->zoneCharSpeed[3] < 0)
        self->state = TitleCard_State_Idle;
}
void TitleCard_State_Idle(void)
{
    RSDK_THIS(TitleCard);
    Zone_ApplyWorldBounds();
    TitleCard_SetCamera();

    if (self->actionTimer >= 60) {
        self->actionTimer  = 0;
        self->state     = TitleCard_State_SlideAway;
        self->stateDraw = TitleCard_Draw_SlideAway;
        RSDK.SetGameMode(ENGINESTATE_REGULAR);
        StateMachine_Run(TitleCard->finishedCB);
    }
    else {
        self->actionTimer++;
        if (self->actionTimer == 16) {
            if (Zone->setATLBounds) {
                EntityCamera *camera   = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
                EntityPlayer *player   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
                player->camera         = camera;
                camera->targetPtr      = (Entity *)player;
                camera->state          = Camera_State_Follow;
                Camera->centerBounds.x = 0x20000;
                Camera->centerBounds.y = 0x20000;
            }
            Zone->setATLBounds = false;
        }
    }
}

void TitleCard_State_SlideAway(void)
{
    RSDK_THIS(TitleCard);
    Zone_ApplyWorldBounds();
    int32 speed = ++self->actionTimer << 18;
    self->points7[0].x -= speed;
    self->points7[0].y -= speed;
    self->points7[1].x -= speed;
    self->points7[1].y -= speed;
    self->points7[2].x -= speed;
    self->points7[2].y -= speed;
    self->points7[3].x -= speed;
    self->points7[3].y -= speed;

    if (self->actionTimer > 6) {
        speed = (self->actionTimer - 6) << 18;
        self->points6[0].x -= speed;
        self->points6[0].y -= speed;
        self->points6[1].x -= speed;
        self->points6[1].y -= speed;
        self->points6[2].x -= speed;
        self->points6[2].y -= speed;
        self->points6[3].x -= speed;
        self->points6[3].y -= speed;
        self->decorationPos.x += speed;
        self->decorationPos.y += speed;
    }

    if (self->actionTimer > 12) {
        speed = (self->actionTimer - 12) << 18;
        self->points5[0].x -= speed;
        self->points5[0].y -= speed;
        self->points5[1].x -= speed;
        self->points5[1].y -= speed;
        self->points5[2].x -= speed;
        self->points5[2].y -= speed;
        self->points5[3].x -= speed;
        self->points5[3].y -= speed;
    }

    if (self->actionTimer > 18) {
        speed = (self->actionTimer - 12) << 18;
        self->points4[0].x -= speed;
        self->points4[0].y -= speed;
        self->points4[1].x -= speed;
        self->points4[1].y -= speed;
        self->points4[2].x -= speed;
        self->points4[2].y -= speed;
        self->points4[3].x -= speed;
        self->points4[3].y -= speed;
    }

    if (self->actionTimer > 4) {
        speed = (self->actionTimer - 4) << 17;
        self->points8[0].x -= speed;
        self->points8[1].x -= speed;
        self->points8[2].x -= speed;
        self->points8[3].x -= speed;
        self->points9[0].x += speed;
        self->points9[1].x += speed;
        self->points9[2].x += speed;
        self->points9[3].x += speed;
    }

    if (self->actionTimer > 60) {
        speed = 0x200000;
        self->zoneStopPos -= speed;
        self->currentWordPos -= speed;
        self->wordStopPos += speed;
        self->actNumPos.x += speed;
        self->actNumPos.y += speed;
        self->points1[0].x -= speed;
        self->points1[1].x -= speed;
        self->points1[2].x -= speed;
        self->points1[3].x -= speed;
        self->points2[0].x += speed;
        self->points2[1].x += speed;
        self->points2[2].x += speed;
        self->points2[3].x += speed;
        self->points3[0].x -= speed;
        self->points3[1].x -= speed;
        self->points3[2].x -= speed;
        self->points3[3].x -= speed;
    }

    if (self->actionTimer == 6) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            SceneInfo->timeEnabled = true;
        }
    }
    if (self->actionTimer > 80) {
        globals->atlEnabled  = false;
        globals->enableIntro = false;
        if (globals->gameMode >= MODE_TIMEATTACK) {
            if (globals->gameMode == MODE_COMPETITION) {
#if RETRO_USE_PLUS
                Competition_ClearMatchData();
#else
                CompetitionSession_ClearMatchData();
#endif
                Announcer_StartCountdown();
            }
            destroyEntity(self);
        }
        else {
            globals->suppressTitlecard = false;
            globals->suppressAutoMusic = false;
            destroyEntity(self);
        }
    }
}

void TitleCard_State_Supressed(void)
{
    RSDK_THIS(TitleCard);
    TitleCard_SetCamera();
    RSDK.SetGameMode(ENGINESTATE_REGULAR);
    globals->atlEnabled = false;
    if (globals->gameMode == MODE_TIMEATTACK || globals->enableIntro)
        SceneInfo->timeEnabled = false;

    self->active = ACTIVE_NEVER;
    if ((globals->suppressTitlecard && !self->enableIntro) || globals->gameMode == MODE_TIMEATTACK) {
        destroyEntity(self);
        globals->suppressTitlecard = false;
    }

    StateMachine_Run(TitleCard->finishedCB);
}

void TitleCard_Draw_Default(void)
{
    RSDK_THIS(TitleCard);
    if (!globals->atlEnabled && !globals->suppressTitlecard) {
        if (self->timer < 256)
            RSDK.DrawRect(0, 0, ScreenInfo->width, ScreenInfo->height, 0, 0xFF, INK_NONE, true);

        if (self->timer < 512)
            RSDK.DrawRect(0, ScreenInfo->centerY - (self->timer >> 1), ScreenInfo->width, self->timer, self->colours[3], 0xFF, INK_NONE, true);

        int32 val = self->timer - 128;
        if (val > 0) {
            if (val < 512)
                RSDK.DrawRect(0, ScreenInfo->centerY - (val >> 1), ScreenInfo->width, val, self->colours[2], 0xFF, INK_NONE, true);

            val -= 128;
            if (val > 0) {
                if (val < 512)
                    RSDK.DrawRect(0, ScreenInfo->centerY - (val >> 1), ScreenInfo->width, val, self->colours[0], 0xFF, INK_NONE, true);

                val -= 128;
                if (val > 0) {
                    if (val < 512)
                        RSDK.DrawRect(0, ScreenInfo->centerY - (val >> 1), ScreenInfo->width, val, self->colours[1], 0xFF, INK_NONE, true);

                    val -= 128;
                    if (val > 0)
                        RSDK.DrawRect(0, ScreenInfo->centerY - (val >> 1), ScreenInfo->width, val, self->colours[4], 0xFF, INK_NONE, true);
                }
            }
        }
    }

    if (self->word2Offset > 0)
        RSDK.DrawQuad(self->points1, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawQuad(self->points2, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawQuad(self->points3, 4, 0xF0, 0xF0, 0xF0, 0xFF, INK_NONE);
#if RETRO_USE_PLUS
    self->decorationAnimator.frameID = 2 * (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) + 1;
#else
    self->decorationAnimator.frameID = 2 * 0 + 1;
#endif
    RSDK.DrawSprite(&self->decorationAnimator, &self->decorationPos, true);
}

void TitleCard_Draw_SolidBG(void)
{
    RSDK_THIS(TitleCard);
    if (!globals->atlEnabled && !globals->suppressTitlecard)
        RSDK.DrawRect(0, 0, ScreenInfo->width, ScreenInfo->height, self->colours[4], 0xFF, INK_NONE, true);
    if (self->points0[1].x < 0xF00000)
        RSDK.DrawQuad(self->points6, 4, (self->colours[0] >> 16) & 0xFF, (self->colours[0] >> 8) & 0xFF, (self->colours[0] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
    if (self->points0[1].y < 0xF00000)
        RSDK.DrawQuad(self->points7, 4, (self->colours[1] >> 16) & 0xFF, (self->colours[1] >> 8) & 0xFF, (self->colours[1] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
    if (self->points0[0].y < 0xF00000)
        RSDK.DrawQuad(self->points5, 4, (self->colours[2] >> 16) & 0xFF, (self->colours[2] >> 8) & 0xFF, (self->colours[2] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
    if (self->points0[0].x < 0xF00000)
        RSDK.DrawQuad(self->points4, 4, (self->colours[3] >> 16) & 0xFF, (self->colours[3] >> 8) & 0xFF, (self->colours[3] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
    if (!globals->atlEnabled && globals->suppressTitlecard == false) {
#if RETRO_USE_PLUS
        self->decorationAnimator.frameID = 2 * (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) + 1;
#else
        self->decorationAnimator.frameID = 2 * 0 + 1;
#endif
        RSDK.DrawSprite(&self->decorationAnimator, &self->decorationPos, true);
    }

    if (self->word2Offset > 0)
        RSDK.DrawQuad(self->points1, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawQuad(self->points2, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawQuad(self->points3, 4, 0xF0, 0xF0, 0xF0, 0xFF, INK_NONE);

    Vector2 drawPos;
    drawPos.x = self->zoneStopPos;
    RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 170, ScreenInfo[SceneInfo->currentScreenID].width, 240);

    for (int32 i = 0; i < 4; ++i) {
        self->zoneLetterAnimator.frameID = i;
        drawPos.y                    = 0xBA0000 + self->zoneCharPos[i];
        RSDK.DrawSprite(&self->zoneLetterAnimator, &drawPos, true);
    }

    if (self->word2Offset > 0) {
        RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, ScreenInfo[SceneInfo->currentScreenID].width, 130);
        drawPos.y = 0x720000;
        drawPos.x = self->currentWordPos - 0x140000;
        RSDK.DrawText(&self->nameLetterAnimator, &drawPos, &self->zoneName, 0, self->word2Offset, ALIGN_RIGHT, 1, 0, self->charPos, true);
    }

    RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, ScreenInfo[SceneInfo->currentScreenID].width, 170);
    drawPos.y = 0x9A0000;
    drawPos.x = self->wordStopPos - 0x140000;
    RSDK.DrawText(&self->nameLetterAnimator, &drawPos, &self->zoneName, self->word2Offset, 0, ALIGN_RIGHT, 1, 0, self->charPos, true);

    RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, ScreenInfo[SceneInfo->currentScreenID].width, ScreenInfo[SceneInfo->currentScreenID].height);
    if (self->actID != 3) {
        if (self->actNumScale > 0) {
            self->drawFX  = FX_SCALE;
            self->scale.y = 0x200;
            if (self->actNumScale > 512)
                self->scale.x = 512;
            else
                self->scale.x = self->actNumScale;

#if RETRO_USE_PLUS
            self->decorationAnimator.frameID = (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) ? 2 : 0;
#else
            self->decorationAnimator.frameID = 0;
#endif
            RSDK.DrawSprite(&self->decorationAnimator, &self->actNumPos, true);

            self->scale.x = self->actNumScale - 0x100;
            if (self->scale.x < 0) 
                self->scale.x = 0;
            else if (self->scale.x > 0x200) 
                self->scale.x = 0x200;

            RSDK.DrawSprite(&self->actNumbersAnimator, &self->actNumPos, true);
            self->drawFX = FX_NONE;
        }
    }
}

void TitleCard_Draw_SlideAway(void)
{
    RSDK_THIS(TitleCard);
    if (!globals->atlEnabled && !globals->suppressTitlecard) {
        RSDK.DrawQuad(self->points8, 4, (self->colours[4] >> 16) & 0xFF, (self->colours[4] >> 8) & 0xFF, (self->colours[4] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
        RSDK.DrawQuad(self->points9, 4, (self->colours[4] >> 16) & 0xFF, (self->colours[4] >> 8) & 0xFF, (self->colours[4] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
    }
    if (self->points0[1].x < 0xF00000)
        RSDK.DrawQuad(self->points6, 4, (self->colours[0] >> 16) & 0xFF, (self->colours[0] >> 8) & 0xFF, (self->colours[0] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
    if (self->points0[1].y < 0xF00000)
        RSDK.DrawQuad(self->points7, 4, (self->colours[1] >> 16) & 0xFF, (self->colours[1] >> 8) & 0xFF, (self->colours[1] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
    if (self->points0[0].y < 0xF00000)
        RSDK.DrawQuad(self->points5, 4, (self->colours[2] >> 16) & 0xFF, (self->colours[2] >> 8) & 0xFF, (self->colours[2] >> 0) & 0xFF, 0xFF,
                      INK_NONE);
    if (self->points0[0].x < 0xF00000)
        RSDK.DrawQuad(self->points4, 4, (self->colours[3] >> 16) & 0xFF, (self->colours[3] >> 8) & 0xFF, (self->colours[3] >> 0) & 0xFF, 0xFF,
                      INK_NONE);

    if (!globals->atlEnabled && !globals->suppressTitlecard) {
#if RETRO_USE_PLUS
        self->decorationAnimator.frameID = 2 * (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) + 1;
#else
        self->decorationAnimator.frameID = 2 * 0 + 1;
#endif
        RSDK.DrawSprite(&self->decorationAnimator, &self->decorationPos, true);
    }

    if (self->actID != 3 && self->actNumScale > 0) {
#if RETRO_USE_PLUS
        self->decorationAnimator.frameID = (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) ? 2 : 0;
#else
        self->decorationAnimator.frameID = 0;
#endif
        RSDK.DrawSprite(&self->decorationAnimator, &self->actNumPos, true);
        RSDK.DrawSprite(&self->actNumbersAnimator, &self->actNumPos, true);
    }
    if (self->word2Offset > 0)
        RSDK.DrawQuad(self->points1, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawQuad(self->points2, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawQuad(self->points3, 4, 0xF0, 0xF0, 0xF0, 0xFF, INK_NONE);

    Vector2 drawPos;
    drawPos.x                    = self->zoneStopPos;
    drawPos.y                    = 0xBA0000;
    self->zoneLetterAnimator.frameID = 0;
    RSDK.DrawSprite(&self->zoneLetterAnimator, &drawPos, true);

    self->zoneLetterAnimator.frameID = 1;
    RSDK.DrawSprite(&self->zoneLetterAnimator, &drawPos, true);

    self->zoneLetterAnimator.frameID = 2;
    RSDK.DrawSprite(&self->zoneLetterAnimator, &drawPos, true);

    self->zoneLetterAnimator.frameID = 3;
    RSDK.DrawSprite(&self->zoneLetterAnimator, &drawPos, true);

    if (self->word2Offset > 0) {
        drawPos.y = 0x720000;
        drawPos.x = self->currentWordPos - 0x140000;
        RSDK.DrawText(&self->nameLetterAnimator, &drawPos, &self->zoneName, 0, self->word2Offset, ALIGN_RIGHT, 1, 0, 0, true);
    }

    drawPos.y = 0x9A0000;
    drawPos.x = self->wordStopPos - 0x140000;
    RSDK.DrawText(&self->nameLetterAnimator, &drawPos, &self->zoneName, self->word2Offset, 0, ALIGN_RIGHT, 1, 0, 0, true);
}

#if RETRO_INCLUDE_EDITOR
void TitleCard_EditorDraw(void)
{
    RSDK_THIS(TitleCard);
    RSDK.SetSpriteAnimation(TitleCard->aniFrames, 0, &self->decorationAnimator, true, 3);
    RSDK.DrawSprite(&self->decorationAnimator, NULL, false);
}

void TitleCard_EditorLoad(void) { TitleCard->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }
#endif

void TitleCard_Serialize(void)
{
    RSDK_EDITABLE_VAR(TitleCard, VAR_STRING, zoneName);
    RSDK_EDITABLE_VAR(TitleCard, VAR_UINT8, actID);
}
