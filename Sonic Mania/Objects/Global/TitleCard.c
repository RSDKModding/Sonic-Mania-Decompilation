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
#if RETRO_USE_PLUS
        self->enableIntro = globals->enableIntro;

        TitleCard_SetupColors();
#endif

        if (!globals->suppressTitlecard || globals->enableIntro || globals->gameMode == MODE_TIMEATTACK)
            SceneInfo->timeEnabled = false;

        if (globals->suppressTitlecard) {
#if RETRO_USE_PLUS
            StateMachine_Run(TitleCard->suppressCB);
            TitleCard->suppressCB = NULL;
#endif
            self->state = TitleCard_State_Supressed;
        }
        else {
            if (!globals->atlEnabled)
                Zone->persistentTimer = 0;

            self->state     = TitleCard_State_SetupBGElements;
            self->stateDraw = TitleCard_Draw_SlideIn;
        }

        self->stripPos[0] = (ScreenInfo->centerX - 152) << 16;
        self->stripPos[1] = (ScreenInfo->centerX - 152) << 16;
        self->stripPos[2] = (ScreenInfo->centerX - 160) << 16;
        self->stripPos[3] = (ScreenInfo->centerX + 20) << 16;
        TitleCard_SetupTitleWords();
        TitleCard_SetupVertices();

        self->decorationPos.y = -0x340000;
        self->decorationPos.x = (ScreenInfo->width - 160) << 16;

        RSDK.SetSpriteAnimation(TitleCard->aniFrames, 0, &self->decorationAnimator, true, 0);
        RSDK.SetSpriteAnimation(TitleCard->aniFrames, 1, &self->nameLetterAnimator, true, 0);
        RSDK.SetSpriteAnimation(TitleCard->aniFrames, 2, &self->zoneLetterAnimator, true, 0);
        RSDK.SetSpriteAnimation(TitleCard->aniFrames, 3, &self->actNumbersAnimator, true, 0);

        if (self->actID > 3)
            self->actID = 3;

        self->actNumbersAnimator.frameID = self->actID;
        self->actNumPos.y                = 0xA80000;
        self->actNumPos.x                = (ScreenInfo->centerX + 106) << 16;
        self->actNumScale                = -0x400;
        if (self->word2XPos - self->word2Width < 0x100000) {
            int32 dist = (self->word2XPos - self->word2Width) - 0x100000;
            self->word1XPos -= dist;
            self->zoneXPos -= dist;
            self->actNumPos.x -= dist;
            self->word2XPos = self->word2XPos - dist;
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
            RSDK.CopyEntity(player, (Entity *)Zone->entityStorage[0], false);
            RSDK.SetSpriteAnimation(player->aniFrames, player->animator.animationID, &player->animator, false, player->animator.frameID);

            if (player->camera)
                RSDK.CopyEntity(player->camera, (Entity *)Zone->entityStorage[8], false);

            Player_ApplyShield(player);

            if (player->invincibleTimer > 0 && player->superState != SUPERSTATE_SUPER)
                RSDK.ResetEntityPtr(RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), InvincibleStars), InvincibleStars->classID,
                                    player);

            if (player->speedShoesTimer > 0 || player->superState == SUPERSTATE_SUPER)
                RSDK.ResetEntityPtr(RSDK_GET_ENTITY(2 * Player->playerCount + RSDK.GetEntityID(player), ImageTrail), ImageTrail->classID, player);

            memset(Zone->entityStorage[0], 0, ENTITY_SIZE);
            memset(Zone->entityStorage[8], 0, ENTITY_SIZE);
        }
#endif
    }
}

void TitleCard_StageLoad(void)
{
    TitleCard->aniFrames = RSDK.LoadSpriteAnimation("Global/TitleCard.bin", SCOPE_STAGE);

    foreach_all(TitleCard, titleCard) { Zone->actID = titleCard->actID; }
}

#if RETRO_USE_PLUS
void TitleCard_SetupColors(void)
{
    RSDK_THIS(TitleCard);

    if (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) {
        self->colors[0] = 0x3751A2; // dark blue
        self->colors[1] = 0xC7525B; // light red
        self->colors[2] = 0x428FC3; // light blue
        self->colors[3] = 0xDFB13F; // yellow
        self->colors[4] = 0x6BAE99; // teal
    }
    else {
        self->colors[0] = 0xF08C18; // orange
        self->colors[1] = 0x60C0A0; // green
        self->colors[2] = 0xF05030; // red
        self->colors[3] = 0x4060B0; // blue
        self->colors[4] = 0xF0C800; // yellow
    }
}
#endif

void TitleCard_SetupVertices(void)
{
    RSDK_THIS(TitleCard);

    self->vertMovePos[0].x = 0xF00000;
    self->vertMovePos[0].y = 0x1F00000;
    self->vertMovePos[1].x = 0x2F00000;
    self->vertMovePos[1].y = 0x3F00000;

    self->vertTargetPos[0].x = 0;
    self->vertTargetPos[0].y = 0x8A0000;
    self->vertTargetPos[1].x = 0x4A0000;
    self->vertTargetPos[1].y = 0x700000;

    if (self->titleCardWord2 > 0) {
        self->word2DecorVerts[0].x = -self->word1Width;
        self->word2DecorVerts[0].y = 0x520000;
        self->word2DecorVerts[1].x = 0;
        self->word2DecorVerts[1].y = 0x520000;
        self->word2DecorVerts[2].x = 0;
        self->word2DecorVerts[2].y = 0x620000;
        self->word2DecorVerts[3].x = -self->word1Width;
        self->word2DecorVerts[3].y = 0x620000;
    }

    self->word1DecorVerts[0].x = -self->word2Width;
    self->word1DecorVerts[0].y = 0xBA0000;
    self->word1DecorVerts[1].x = 0;
    self->word1DecorVerts[1].y = 0xBA0000;
    self->word1DecorVerts[2].x = 0;
    self->word1DecorVerts[2].y = 0xCA0000;
    self->word1DecorVerts[3].x = -self->word2Width;
    self->word1DecorVerts[3].y = 0xCA0000;

    self->zoneDecorVerts[0].x = ScreenInfo->width << 16;
    self->zoneDecorVerts[0].y = 0x9A0000;
    self->zoneDecorVerts[1].x = 0x780000 + self->zoneDecorVerts[0].x;
    self->zoneDecorVerts[1].y = 0x9A0000;
    self->zoneDecorVerts[2].x = 0x780000 + self->zoneDecorVerts[0].x;
    self->zoneDecorVerts[2].y = 0xA20000;
    self->zoneDecorVerts[3].x = self->zoneDecorVerts[0].x;
    self->zoneDecorVerts[3].y = 0xA20000;

    self->stripVertsBlue[0].x = self->stripPos[0];
    self->stripVertsBlue[0].y = 0xF00000;
    self->stripVertsBlue[1].x = self->stripVertsBlue[0].x + 0x400000;
    self->stripVertsBlue[1].y = 0xF00000;
    self->stripVertsBlue[2].x = self->stripVertsBlue[0].x + 0x1300000;
    self->stripVertsBlue[2].y = 0xF00000;
    self->stripVertsBlue[3].x = self->stripVertsBlue[0].x + 0xF00000;
    self->stripVertsBlue[3].y = 0xF00000;

    self->stripVertsRed[0].x = self->stripPos[1];
    self->stripVertsRed[0].y = 0xF00000;
    self->stripVertsRed[1].x = 0x800000 + self->stripVertsRed[0].x;
    self->stripVertsRed[1].y = 0xF00000;
    self->stripVertsRed[2].x = 0xE60000 + self->stripVertsRed[0].x;
    self->stripVertsRed[2].y = 0xF00000;
    self->stripVertsRed[3].x = 0x660000 + self->stripVertsRed[0].x;
    self->stripVertsRed[3].y = 0xF00000;

    self->stripVertsOrange[0].x = self->stripPos[2];
    self->stripVertsOrange[0].y = 0xF00000;
    self->stripVertsOrange[1].x = 0x600000 + self->stripVertsOrange[0].x;
    self->stripVertsOrange[1].y = 0xF00000;
    self->stripVertsOrange[2].x = self->stripVertsOrange[0].x + 0x1060000;
    self->stripVertsOrange[2].y = 0xF00000;
    self->stripVertsOrange[3].x = 0xA60000 + self->stripVertsOrange[0].x;
    self->stripVertsOrange[3].y = 0xF00000;

    self->stripVertsGreen[0].x = self->stripPos[3];
    self->stripVertsGreen[0].y = 0xF00000;
    self->stripVertsGreen[1].x = self->stripVertsGreen[0].x + 0x200000;
    self->stripVertsGreen[1].y = 0xF00000;
    self->stripVertsGreen[2].x = 0xA00000 + self->stripVertsGreen[0].x;
    self->stripVertsGreen[2].y = 0xF00000;
    self->stripVertsGreen[3].x = 0x800000 + self->stripVertsGreen[0].x;
    self->stripVertsGreen[3].y = 0xF00000;

    self->bgLCurtainVerts[0].x = 0;
    self->bgLCurtainVerts[0].y = 0;
    self->bgLCurtainVerts[1].x = (self->stripVertsBlue[1].x + self->stripVertsBlue[0].x) >> 1;
    self->bgLCurtainVerts[1].y = 0;
    self->bgLCurtainVerts[2].x = (self->stripVertsBlue[3].x + self->stripVertsBlue[2].x) >> 1;
    self->bgLCurtainVerts[2].y = 0xF00000;
    self->bgLCurtainVerts[3].x = 0;
    self->bgLCurtainVerts[3].y = 0xF00000;

    self->bgRCurtainVerts[0].x = (self->stripVertsBlue[1].x + self->stripVertsBlue[0].x) >> 1;
    self->bgRCurtainVerts[0].y = 0;
    self->bgRCurtainVerts[1].x = ScreenInfo->width << 16;
    self->bgRCurtainVerts[1].y = 0;
    self->bgRCurtainVerts[2].x = ScreenInfo->width << 16;
    self->bgRCurtainVerts[2].y = 0xF00000;
    self->bgRCurtainVerts[3].x = (self->stripVertsBlue[3].x + self->stripVertsBlue[2].x) >> 1;
    self->bgRCurtainVerts[3].y = 0xF00000;
}

void TitleCard_SetupTitleWords(void)
{
    RSDK_THIS(TitleCard);

    if (!self->zoneName.chars)
        RSDK.InitString(&self->zoneName, "UNTITLED", 0);

    RSDK.SetSpriteString(TitleCard->aniFrames, 1, &self->zoneName);

    int32 offset = 0x280000;
    for (int32 c = 0; c < self->zoneName.length; ++c) {
        self->charPos[c].y = offset;
        self->charVel[c]   = -0x80000;
        offset += 0x100000;
    }

    for (int32 i = 0; i < 4; ++i) {
        self->zoneCharPos[i] = ((2 - self->zoneName.length) << 19) - ((i * 2) << 19);
        self->zoneCharVel[i] = 0x40000;
    }

    for (int32 c = 0; c < self->zoneName.length; ++c) {
        if (self->zoneName.chars[c] == (uint16)-1)
            self->titleCardWord2 = c;
    }

    if (self->titleCardWord2) {
        self->word1Width = (RSDK.GetStringWidth(TitleCard->aniFrames, 1, &self->zoneName, 0, self->titleCardWord2 - 1, 1) + 24) << 16;
        self->word2Width = (RSDK.GetStringWidth(TitleCard->aniFrames, 1, &self->zoneName, self->titleCardWord2, 0, 1) + 24) << 16;
    }
    else {
        self->word2Width = (RSDK.GetStringWidth(TitleCard->aniFrames, 1, &self->zoneName, 0, 0, 1) + 24) << 16;
    }

    self->zoneXPos  = (ScreenInfo->centerX - ((ScreenInfo->centerX - 160) >> 3) + 72) << 16;
    self->word2XPos = (ScreenInfo->centerX - ((ScreenInfo->centerX - 160) >> 3) + 72) << 16;

    if (self->word2Width < 0x800000)
        self->word2XPos -= 0x280000;

    self->word1XPos = self->word1Width - self->word2Width + self->word2XPos - 0x200000;
}

void TitleCard_HandleWordMovement(void)
{
    RSDK_THIS(TitleCard);

    if (self->titleCardWord2 > 0) {
        self->word2DecorVerts[1].x -= 0x200000;
        if (self->word2DecorVerts[1].x < self->word1XPos - 0x100000)
            self->word2DecorVerts[1].x = self->word1XPos - 0x100000;

        self->word2DecorVerts[2].x -= 0x200000;
        self->word2DecorVerts[0].x = self->word2DecorVerts[1].x - self->word1Width;
        if (self->word2DecorVerts[2].x < self->word1XPos)
            self->word2DecorVerts[2].x = self->word1XPos;

        self->word2DecorVerts[3].x = self->word2DecorVerts[2].x - self->word1Width;
    }

    self->word1DecorVerts[1].x -= 0x200000;
    if (self->word1DecorVerts[1].x < self->word2XPos - 0x100000)
        self->word1DecorVerts[1].x = self->word2XPos - 0x100000;

    self->word1DecorVerts[2].x -= 0x200000;
    self->word1DecorVerts[0].x = self->word1DecorVerts[1].x - self->word2Width;
    if (self->word1DecorVerts[2].x < self->word2XPos)
        self->word1DecorVerts[2].x = self->word2XPos;

    self->zoneDecorVerts[1].x += 0x200000;
    self->word1DecorVerts[3].x = self->word1DecorVerts[2].x - self->word2Width;
    if (self->zoneDecorVerts[1].x > self->zoneXPos - 0x80000)
        self->zoneDecorVerts[1].x = self->zoneXPos - 0x80000;

    self->zoneDecorVerts[2].x += 0x200000;
    self->zoneDecorVerts[0].x = self->zoneDecorVerts[1].x - 0x780000;
    if (self->zoneDecorVerts[2].x > self->zoneXPos)
        self->zoneDecorVerts[2].x = self->zoneXPos;

    self->zoneDecorVerts[3].x = self->zoneDecorVerts[2].x - 0x780000;

    if (self->decorationPos.y < 0xC0000) {
        self->decorationPos.x += 0x20000;
        self->decorationPos.y += 0x20000;
    }
}

void TitleCard_HandleZoneCharMovement(void)
{
    RSDK_THIS(TitleCard);

    for (int32 c = 0; c < self->zoneName.length; ++c) {
        if (self->charPos[c].y < 0)
            self->charVel[c] += 0x28000;

        self->charPos[c].y += self->charVel[c];
        if (self->charPos[c].y > 0 && self->charVel[c] > 0)
            self->charPos[c].y = 0;
    }

    for (int32 i = 0; i < 4; ++i) {
        if (self->zoneCharPos[i] > 0)
            self->zoneCharVel[i] -= 0x14000;

        self->zoneCharPos[i] += self->zoneCharVel[i];
        if (self->zoneCharPos[i] < 0 && self->zoneCharVel[i] < 0)
            self->zoneCharPos[i] = 0;
    }
}

void TitleCard_HandleCamera(void)
{
    foreach_active(Player, player)
    {
        if (player->camera)
            player->camera->offset.y = 0;
    }
}

void TitleCard_State_SetupBGElements(void)
{
    RSDK_THIS(TitleCard);

#if RETRO_USE_PLUS
    if (ActClear && ActClear->actClearActive)
        ActClear->actClearActive = false;
#endif

    Zone_ApplyWorldBounds();

    if (!globals->atlEnabled && !globals->suppressTitlecard)
        RSDK.SetEngineState(ENGINESTATE_PAUSED);

    self->timer += 24;
    if (self->timer >= 512) {
        self->word2DecorVerts[0].y += 0x200000;
        self->word2DecorVerts[1].y += 0x200000;
        self->word2DecorVerts[2].y += 0x200000;
        self->word2DecorVerts[3].y += 0x200000;

        self->word1DecorVerts[0].y -= 0x200000;
        self->word1DecorVerts[1].y -= 0x200000;
        self->word1DecorVerts[2].y -= 0x200000;
        self->word1DecorVerts[3].y -= 0x200000;

        self->zoneDecorVerts[0].y += 0x200000;
        self->zoneDecorVerts[1].y += 0x200000;
        self->zoneDecorVerts[2].y += 0x200000;
        self->zoneDecorVerts[3].y += 0x200000;

        self->state = TitleCard_State_OpeningBG;
    }

    self->word2DecorVerts[0].x += 0x280000;
    self->word2DecorVerts[1].x += 0x280000;
    self->word2DecorVerts[2].x += 0x280000;
    self->word2DecorVerts[3].x += 0x280000;

    self->word1DecorVerts[0].x += 0x280000;
    self->word1DecorVerts[1].x += 0x280000;
    self->word1DecorVerts[2].x += 0x280000;
    self->word1DecorVerts[3].x += 0x280000;

    self->zoneDecorVerts[0].x -= 0x280000;
    self->zoneDecorVerts[1].x -= 0x280000;
    self->zoneDecorVerts[2].x -= 0x280000;
    self->zoneDecorVerts[3].x -= 0x280000;
}

void TitleCard_State_OpeningBG(void)
{
    RSDK_THIS(TitleCard);

    Zone_ApplyWorldBounds();

    if (self->timer >= 1024) {
        self->state     = TitleCard_State_EnterTitle;
        self->stateDraw = TitleCard_Draw_ShowTitleCard;
    }
    else {
        self->timer += 32;
    }

    TitleCard_HandleWordMovement();
}

void TitleCard_State_EnterTitle(void)
{
    RSDK_THIS(TitleCard);

    Zone_ApplyWorldBounds();

    self->vertMovePos[0].x += (self->vertTargetPos[0].x - self->vertMovePos[0].x - 0x100000) / 6;
    if (self->vertMovePos[0].x < self->vertTargetPos[0].x)
        self->vertMovePos[0].x = self->vertTargetPos[0].x;

    self->vertMovePos[0].y += (self->vertTargetPos[0].y - self->vertMovePos[0].y - 0x100000) / 6;
    if (self->vertMovePos[0].y < self->vertTargetPos[0].y)
        self->vertMovePos[0].y = self->vertTargetPos[0].y;

    self->vertMovePos[1].x += (self->vertTargetPos[1].x - self->vertMovePos[1].x - 0x100000) / 6;
    if (self->vertMovePos[1].x < self->vertTargetPos[1].x)
        self->vertMovePos[1].x = self->vertTargetPos[1].x;

    self->vertMovePos[1].y += (self->vertTargetPos[1].y - self->vertMovePos[1].y - 0x100000) / 6;
    if (self->vertMovePos[1].y < self->vertTargetPos[1].y)
        self->vertMovePos[1].y = self->vertTargetPos[1].y;

    self->stripVertsBlue[0].x = (self->vertMovePos[0].x - 0xF00000) + self->stripVertsBlue[3].x;
    self->stripVertsBlue[0].y = self->vertMovePos[0].x;
    self->stripVertsBlue[1].x = (self->vertMovePos[0].x - 0xF00000) + self->stripVertsBlue[2].x;
    self->stripVertsBlue[1].y = self->vertMovePos[0].x;

    self->stripVertsRed[0].x = (self->vertMovePos[0].y - 0xF00000) + self->stripVertsRed[3].x;
    self->stripVertsRed[0].y = self->vertMovePos[0].y;
    self->stripVertsRed[1].x = (self->vertMovePos[0].y - 0xF00000) + self->stripVertsRed[2].x;
    self->stripVertsRed[1].y = self->vertMovePos[0].y;

    self->stripVertsOrange[0].x = (self->vertMovePos[1].x - 0xF00000) + self->stripVertsOrange[3].x;
    self->stripVertsOrange[0].y = self->vertMovePos[1].x;
    self->stripVertsOrange[1].x = (self->vertMovePos[1].x - 0xF00000) + self->stripVertsOrange[2].x;
    self->stripVertsOrange[1].y = self->vertMovePos[1].x;

    self->stripVertsGreen[0].x = (self->vertMovePos[1].y - 0xF00000) + self->stripVertsGreen[3].x;
    self->stripVertsGreen[0].y = self->vertMovePos[1].y;
    self->stripVertsGreen[1].x = (self->vertMovePos[1].y - 0xF00000) + self->stripVertsGreen[2].x;
    self->stripVertsGreen[1].y = self->vertMovePos[1].y;

    TitleCard_HandleWordMovement();
    TitleCard_HandleZoneCharMovement();

    if (self->actNumScale < 0x300)
        self->actNumScale += 0x40;

    if (!self->zoneCharPos[3] && self->zoneCharVel[3] < 0)
        self->state = TitleCard_State_ShowingTitle;
}
void TitleCard_State_ShowingTitle(void)
{
    RSDK_THIS(TitleCard);

    Zone_ApplyWorldBounds();
    TitleCard_HandleCamera();

    if (self->actionTimer >= 60) {
        self->actionTimer = 0;
        self->state       = TitleCard_State_SlideAway;
        self->stateDraw   = TitleCard_Draw_SlideAway;
        RSDK.SetEngineState(ENGINESTATE_REGULAR);
#if RETRO_USE_PLUS
        StateMachine_Run(TitleCard->finishedCB);
#endif
    }
    else {
        self->actionTimer++;
        if (self->actionTimer == 16) {
            if (Zone->setATLBounds) {
                EntityCamera *camera   = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
                EntityPlayer *player   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
                player->camera         = camera;
                camera->target         = (Entity *)player;
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
    self->stripVertsGreen[0].x -= speed;
    self->stripVertsGreen[0].y -= speed;
    self->stripVertsGreen[1].x -= speed;
    self->stripVertsGreen[1].y -= speed;
    self->stripVertsGreen[2].x -= speed;
    self->stripVertsGreen[2].y -= speed;
    self->stripVertsGreen[3].x -= speed;
    self->stripVertsGreen[3].y -= speed;

    if (self->actionTimer > 6) {
        speed = (self->actionTimer - 6) << 18;
        self->stripVertsOrange[0].x -= speed;
        self->stripVertsOrange[0].y -= speed;
        self->stripVertsOrange[1].x -= speed;
        self->stripVertsOrange[1].y -= speed;
        self->stripVertsOrange[2].x -= speed;
        self->stripVertsOrange[2].y -= speed;
        self->stripVertsOrange[3].x -= speed;
        self->stripVertsOrange[3].y -= speed;
        self->decorationPos.x += speed;
        self->decorationPos.y += speed;
    }

    if (self->actionTimer > 12) {
        speed = (self->actionTimer - 12) << 18;
        self->stripVertsRed[0].x -= speed;
        self->stripVertsRed[0].y -= speed;
        self->stripVertsRed[1].x -= speed;
        self->stripVertsRed[1].y -= speed;
        self->stripVertsRed[2].x -= speed;
        self->stripVertsRed[2].y -= speed;
        self->stripVertsRed[3].x -= speed;
        self->stripVertsRed[3].y -= speed;
    }

    if (self->actionTimer > 18) {
        speed = (self->actionTimer - 12) << 18;
        self->stripVertsBlue[0].x -= speed;
        self->stripVertsBlue[0].y -= speed;
        self->stripVertsBlue[1].x -= speed;
        self->stripVertsBlue[1].y -= speed;
        self->stripVertsBlue[2].x -= speed;
        self->stripVertsBlue[2].y -= speed;
        self->stripVertsBlue[3].x -= speed;
        self->stripVertsBlue[3].y -= speed;
    }

    if (self->actionTimer > 4) {
        speed = (self->actionTimer - 4) << 17;

        self->bgLCurtainVerts[0].x -= speed;
        self->bgLCurtainVerts[1].x -= speed;
        self->bgLCurtainVerts[2].x -= speed;
        self->bgLCurtainVerts[3].x -= speed;

        self->bgRCurtainVerts[0].x += speed;
        self->bgRCurtainVerts[1].x += speed;
        self->bgRCurtainVerts[2].x += speed;
        self->bgRCurtainVerts[3].x += speed;
    }

    if (self->actionTimer > 60) {
        speed = 0x200000;
        self->zoneXPos -= speed;
        self->word1XPos -= speed;
        self->word2XPos += speed;
        self->actNumPos.x += speed;
        self->actNumPos.y += speed;
        self->word2DecorVerts[0].x -= speed;
        self->word2DecorVerts[1].x -= speed;
        self->word2DecorVerts[2].x -= speed;
        self->word2DecorVerts[3].x -= speed;
        self->word1DecorVerts[0].x += speed;
        self->word1DecorVerts[1].x += speed;
        self->word1DecorVerts[2].x += speed;
        self->word1DecorVerts[3].x += speed;
        self->zoneDecorVerts[0].x -= speed;
        self->zoneDecorVerts[1].x -= speed;
        self->zoneDecorVerts[2].x -= speed;
        self->zoneDecorVerts[3].x -= speed;
    }

    if (self->actionTimer == 6 && globals->gameMode < MODE_TIMEATTACK) {
        SceneInfo->timeEnabled = true;
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
        }
        else {
            globals->suppressTitlecard = false;
            globals->suppressAutoMusic = false;
        }
        destroyEntity(self);
    }
}

void TitleCard_State_Supressed(void)
{
    RSDK_THIS(TitleCard);

    TitleCard_HandleCamera();
    RSDK.SetEngineState(ENGINESTATE_REGULAR);

    globals->atlEnabled = false;
    if (globals->gameMode == MODE_TIMEATTACK || globals->enableIntro)
        SceneInfo->timeEnabled = false;

    self->active = ACTIVE_NEVER;
#if RETRO_USE_PLUS
    if ((globals->suppressTitlecard && !self->enableIntro) || globals->gameMode == MODE_TIMEATTACK) {
        destroyEntity(self);
        globals->suppressTitlecard = false;
    }
#else 
    if (globals->suppressTitlecard > true || globals->gameMode == MODE_TIMEATTACK) {
        destroyEntity(self);
        globals->suppressTitlecard = false;
    }
#endif

#if RETRO_USE_PLUS
    StateMachine_Run(TitleCard->finishedCB);
#endif
}

void TitleCard_Draw_SlideIn(void)
{
    RSDK_THIS(TitleCard);

    color colors[5];
#if RETRO_USE_PLUS
    colors[0] = self->colors[0];
    colors[1] = self->colors[1];
    colors[2] = self->colors[2];
    colors[3] = self->colors[3];
    colors[4] = self->colors[4];
#else
    colors[0] = 0xF08C18; // orange
    colors[1] = 0x60C0A0; // green
    colors[2] = 0xF05030; // red
    colors[3] = 0x4060B0; // blue
    colors[4] = 0xF0C800; // yellow
#endif

    // The big ol' BG
    if (!globals->atlEnabled && !globals->suppressTitlecard) {
        if (self->timer < 256)
            RSDK.DrawRect(0, 0, ScreenInfo->width, ScreenInfo->height, 0, 0xFF, INK_NONE, true);

        // Blue
        int32 height = self->timer;
        if (self->timer < 512)
            RSDK.DrawRect(0, ScreenInfo->centerY - (height >> 1), ScreenInfo->width, height, colors[3], 0xFF, INK_NONE, true);

        // Red
        height = self->timer - 128;
        if (self->timer > 128 && self->timer < 640)
            RSDK.DrawRect(0, ScreenInfo->centerY - (height >> 1), ScreenInfo->width, height, colors[2], 0xFF, INK_NONE, true);

        // Orange
        height = self->timer - 256;
        if (self->timer > 256 && self->timer < 768)
            RSDK.DrawRect(0, ScreenInfo->centerY - (height >> 1), ScreenInfo->width, height, colors[0], 0xFF, INK_NONE, true);

        // Green
        height = self->timer - 384;
        if (self->timer > 384 && self->timer < 896)
            RSDK.DrawRect(0, ScreenInfo->centerY - (height >> 1), ScreenInfo->width, height, colors[1], 0xFF, INK_NONE, true);

        // Yellow
        height = self->timer - 512;
        if (self->timer > 512)
            RSDK.DrawRect(0, ScreenInfo->centerY - (height >> 1), ScreenInfo->width, height, colors[4], 0xFF, INK_NONE, true);
    }

    // Draw the BG thingos
    if (self->titleCardWord2 > 0)
        RSDK.DrawQuad(self->word2DecorVerts, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);

    RSDK.DrawQuad(self->word1DecorVerts, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawQuad(self->zoneDecorVerts, 4, 0xF0, 0xF0, 0xF0, 0xFF, INK_NONE);

    // Draw Act Number
#if RETRO_USE_PLUS
    self->decorationAnimator.frameID = 2 * (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) + 1;
#else
    self->decorationAnimator.frameID = 2 * 0 + 1;
#endif
    RSDK.DrawSprite(&self->decorationAnimator, &self->decorationPos, true);
}

void TitleCard_Draw_ShowTitleCard(void)
{
    RSDK_THIS(TitleCard);

    color colors[5];
#if RETRO_USE_PLUS
    colors[0] = self->colors[0];
    colors[1] = self->colors[1];
    colors[2] = self->colors[2];
    colors[3] = self->colors[3];
    colors[4] = self->colors[4];
#else
    colors[0] = 0xF08C18; // orange
    colors[1] = 0x60C0A0; // green
    colors[2] = 0xF05030; // red
    colors[3] = 0x4060B0; // blue
    colors[4] = 0xF0C800; // yellow
#endif

    // Draw Yellow BG
    if (!globals->atlEnabled && !globals->suppressTitlecard)
        RSDK.DrawRect(0, 0, ScreenInfo->width, ScreenInfo->height, colors[4], 0xFF, INK_NONE, true);

    // Draw Orange Strip
    if (self->vertMovePos[1].x < 0xF00000)
        RSDK.DrawQuad(self->stripVertsOrange, 4, (colors[0] >> 16) & 0xFF, (colors[0] >> 8) & 0xFF, (colors[0] >> 0) & 0xFF, 0xFF, INK_NONE);

    // Draw Green Strip
    if (self->vertMovePos[1].y < 0xF00000)
        RSDK.DrawQuad(self->stripVertsGreen, 4, (colors[1] >> 16) & 0xFF, (colors[1] >> 8) & 0xFF, (colors[1] >> 0) & 0xFF, 0xFF, INK_NONE);

    // Draw Red Strip
    if (self->vertMovePos[0].y < 0xF00000)
        RSDK.DrawQuad(self->stripVertsRed, 4, (colors[2] >> 16) & 0xFF, (colors[2] >> 8) & 0xFF, (colors[2] >> 0) & 0xFF, 0xFF, INK_NONE);

    // Draw Blue Strip
    if (self->vertMovePos[0].x < 0xF00000)
        RSDK.DrawQuad(self->stripVertsBlue, 4, (colors[3] >> 16) & 0xFF, (colors[3] >> 8) & 0xFF, (colors[3] >> 0) & 0xFF, 0xFF, INK_NONE);

    // Draw "Sonic Mania"
    if (!globals->atlEnabled && !globals->suppressTitlecard) {
#if RETRO_USE_PLUS
        self->decorationAnimator.frameID = 2 * (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) + 1;
#else
        self->decorationAnimator.frameID = 2 * 0 + 1;
#endif
        RSDK.DrawSprite(&self->decorationAnimator, &self->decorationPos, true);
    }

    // Draw the BG thingos
    if (self->titleCardWord2 > 0)
        RSDK.DrawQuad(self->word2DecorVerts, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);

    RSDK.DrawQuad(self->word1DecorVerts, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawQuad(self->zoneDecorVerts, 4, 0xF0, 0xF0, 0xF0, 0xFF, INK_NONE);

    // Draw "ZONE"
    RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 170, ScreenInfo[SceneInfo->currentScreenID].width, 240);

    Vector2 drawPos;
    drawPos.x = self->zoneXPos;
    for (int32 i = 0; i < 4; ++i) {
        self->zoneLetterAnimator.frameID = i;
        drawPos.y                        = 0xBA0000 + self->zoneCharPos[i];
        RSDK.DrawSprite(&self->zoneLetterAnimator, &drawPos, true);
    }

    // Draw TitleCard Word 1 (if there are 2 words)
    if (self->titleCardWord2 > 0) {
        RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, ScreenInfo[SceneInfo->currentScreenID].width, 130);
        drawPos.y = 0x720000;
        drawPos.x = self->word1XPos - 0x140000;
        RSDK.DrawText(&self->nameLetterAnimator, &drawPos, &self->zoneName, 0, self->titleCardWord2, ALIGN_CENTER, 1, 0, self->charPos, true);
    }

    // Draw TitleCard Word 2 (if there are 2 words, otherwise draw the entire zoneName)
    RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, ScreenInfo[SceneInfo->currentScreenID].width, 170);
    drawPos.y = 0x9A0000;
    drawPos.x = self->word2XPos - 0x140000;
    RSDK.DrawText(&self->nameLetterAnimator, &drawPos, &self->zoneName, self->titleCardWord2, 0, ALIGN_CENTER, 1, 0, self->charPos, true);

    RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, ScreenInfo[SceneInfo->currentScreenID].width, ScreenInfo[SceneInfo->currentScreenID].height);

    // Draw Act Number
    if (self->actID != 3) {
        if (self->actNumScale > 0) {
            self->drawFX  = FX_SCALE;
            self->scale.x = minVal(self->actNumScale, 0x200);
            self->scale.y = 0x200;

#if RETRO_USE_PLUS
            self->decorationAnimator.frameID = (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) ? 2 : 0;
#else
            self->decorationAnimator.frameID = 0;
#endif
            RSDK.DrawSprite(&self->decorationAnimator, &self->actNumPos, true);

            self->scale.x = clampVal(self->actNumScale - 0x100, 0, 0x200);
            RSDK.DrawSprite(&self->actNumbersAnimator, &self->actNumPos, true);
            self->drawFX = FX_NONE;
        }
    }
}

void TitleCard_Draw_SlideAway(void)
{
    RSDK_THIS(TitleCard);

    color colors[5];
#if RETRO_USE_PLUS
    colors[0] = self->colors[0];
    colors[1] = self->colors[1];
    colors[2] = self->colors[2];
    colors[3] = self->colors[3];
    colors[4] = self->colors[4];
#else
    colors[0] = 0xF08C18; // orange
    colors[1] = 0x60C0A0; // green
    colors[2] = 0xF05030; // red
    colors[3] = 0x4060B0; // blue
    colors[4] = 0xF0C800; // yellow
#endif

    if (!globals->atlEnabled && !globals->suppressTitlecard) {
        // Draw Yellow BG curtain "opening"
        RSDK.DrawQuad(self->bgLCurtainVerts, 4, (colors[4] >> 16) & 0xFF, (colors[4] >> 8) & 0xFF, (colors[4] >> 0) & 0xFF, 0xFF, INK_NONE);
        RSDK.DrawQuad(self->bgRCurtainVerts, 4, (colors[4] >> 16) & 0xFF, (colors[4] >> 8) & 0xFF, (colors[4] >> 0) & 0xFF, 0xFF, INK_NONE);
    }

    // Orange Strip
    if (self->vertMovePos[1].x < 0xF00000)
        RSDK.DrawQuad(self->stripVertsOrange, 4, (colors[0] >> 16) & 0xFF, (colors[0] >> 8) & 0xFF, (colors[0] >> 0) & 0xFF, 0xFF, INK_NONE);

    // Green Strip
    if (self->vertMovePos[1].y < 0xF00000)
        RSDK.DrawQuad(self->stripVertsGreen, 4, (colors[1] >> 16) & 0xFF, (colors[1] >> 8) & 0xFF, (colors[1] >> 0) & 0xFF, 0xFF, INK_NONE);

    // Red Strip
    if (self->vertMovePos[0].y < 0xF00000)
        RSDK.DrawQuad(self->stripVertsRed, 4, (colors[2] >> 16) & 0xFF, (colors[2] >> 8) & 0xFF, (colors[2] >> 0) & 0xFF, 0xFF, INK_NONE);

    // Blue Strip
    if (self->vertMovePos[0].x < 0xF00000)
        RSDK.DrawQuad(self->stripVertsBlue, 4, (colors[3] >> 16) & 0xFF, (colors[3] >> 8) & 0xFF, (colors[3] >> 0) & 0xFF, 0xFF, INK_NONE);

    // Draw "Sonic Mania"
    if (!globals->atlEnabled && !globals->suppressTitlecard) {
#if RETRO_USE_PLUS
        self->decorationAnimator.frameID = 2 * (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) + 1;
#else
        self->decorationAnimator.frameID = 2 * 0 + 1;
#endif
        RSDK.DrawSprite(&self->decorationAnimator, &self->decorationPos, true);
    }

    // Draw Act Number
    if (self->actID != 3 && self->actNumScale > 0) {
#if RETRO_USE_PLUS
        self->decorationAnimator.frameID = (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) ? 2 : 0;
#else
        self->decorationAnimator.frameID = 0;
#endif
        RSDK.DrawSprite(&self->decorationAnimator, &self->actNumPos, true);
        RSDK.DrawSprite(&self->actNumbersAnimator, &self->actNumPos, true);
    }

    // Draw the BG thingos
    if (self->titleCardWord2 > 0)
        RSDK.DrawQuad(self->word2DecorVerts, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);

    RSDK.DrawQuad(self->word1DecorVerts, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawQuad(self->zoneDecorVerts, 4, 0xF0, 0xF0, 0xF0, 0xFF, INK_NONE);

    // Draw "ZONE"
    Vector2 drawPos;
    drawPos.x = self->zoneXPos;
    drawPos.y = 0xBA0000;
    for (int32 i = 0; i < 4; ++i) {
        self->zoneLetterAnimator.frameID = i;
        RSDK.DrawSprite(&self->zoneLetterAnimator, &drawPos, true);
    }

    // Draw TitleCard Word 1 (if there are 2 words)
    if (self->titleCardWord2 > 0) {
        drawPos.y = 0x720000;
        drawPos.x = self->word1XPos - 0x140000;
        RSDK.DrawText(&self->nameLetterAnimator, &drawPos, &self->zoneName, 0, self->titleCardWord2, ALIGN_CENTER, 1, 0, 0, true);
    }

    // Draw TitleCard Word 2 (if there are 2 words, otherwise draw the entire zoneName)
    drawPos.y = 0x9A0000;
    drawPos.x = self->word2XPos - 0x140000;
    RSDK.DrawText(&self->nameLetterAnimator, &drawPos, &self->zoneName, self->titleCardWord2, 0, ALIGN_CENTER, 1, 0, 0, true);
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
