// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TitleCard Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
        self->active    = ACTIVE_ALWAYS;
        self->visible   = true;
        self->drawGroup = Zone->hudDrawGroup;
#if MANIA_USE_PLUS
        self->enableIntro = globals->enableIntro;

        TitleCard_SetupColors();
#endif

        if (!globals->suppressTitlecard || globals->enableIntro || globals->gameMode == MODE_TIMEATTACK)
            SceneInfo->timeEnabled = false;

        if (globals->suppressTitlecard) {
#if MANIA_USE_PLUS
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

        self->stripPos[0] = TO_FIXED(ScreenInfo->center.x - 152);
        self->stripPos[1] = TO_FIXED(ScreenInfo->center.x - 152);
        self->stripPos[2] = TO_FIXED(ScreenInfo->center.x - 160);
        self->stripPos[3] = TO_FIXED(ScreenInfo->center.x + 20);
        TitleCard_SetupTitleWords();
        TitleCard_SetupVertices();

        self->decorationPos.y = -TO_FIXED(52);
        self->decorationPos.x = TO_FIXED(ScreenInfo->size.x - 160);

        RSDK.SetSpriteAnimation(TitleCard->aniFrames, 0, &self->decorationAnimator, true, 0);
        RSDK.SetSpriteAnimation(TitleCard->aniFrames, 1, &self->nameLetterAnimator, true, 0);
        RSDK.SetSpriteAnimation(TitleCard->aniFrames, 2, &self->zoneLetterAnimator, true, 0);
        RSDK.SetSpriteAnimation(TitleCard->aniFrames, 3, &self->actNumbersAnimator, true, 0);

        if (self->actID > ACT_NONE)
            self->actID = ACT_NONE;

        self->actNumbersAnimator.frameID = self->actID;
        self->actNumPos.y                = TO_FIXED(168);
        self->actNumPos.x                = TO_FIXED(ScreenInfo->center.x + 106);
        self->actNumScale                = -0x400;
        if (self->word2XPos - self->word2Width < TO_FIXED(16)) {
            int32 dist = (self->word2XPos - self->word2Width) - TO_FIXED(16);
            self->word1XPos -= dist;
            self->zoneXPos -= dist;
            self->actNumPos.x -= dist;
            self->word2XPos = self->word2XPos - dist;
        }

#if MANIA_USE_PLUS
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
                RSDK.ResetEntity(RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntitySlot(player), InvincibleStars), InvincibleStars->classID,
                                 player);

            if (player->speedShoesTimer > 0 || player->superState == SUPERSTATE_SUPER)
                RSDK.ResetEntity(RSDK_GET_ENTITY(2 * Player->playerCount + RSDK.GetEntitySlot(player), ImageTrail), ImageTrail->classID, player);

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

#if MANIA_USE_PLUS
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

    self->vertMovePos[0].x = TO_FIXED(240);
    self->vertMovePos[0].y = TO_FIXED(496);
    self->vertMovePos[1].x = TO_FIXED(752);
    self->vertMovePos[1].y = TO_FIXED(1008);

    self->vertTargetPos[0].x = TO_FIXED(0);
    self->vertTargetPos[0].y = TO_FIXED(138);
    self->vertTargetPos[1].x = TO_FIXED(74);
    self->vertTargetPos[1].y = TO_FIXED(112);

    if (self->titleCardWord2 > 0) {
        self->word1DecorVerts[0].x = -self->word1Width;
        self->word1DecorVerts[0].y = TO_FIXED(82);
        self->word1DecorVerts[1].x = TO_FIXED(0);
        self->word1DecorVerts[1].y = TO_FIXED(82);
        self->word1DecorVerts[2].x = TO_FIXED(0);
        self->word1DecorVerts[2].y = TO_FIXED(98);
        self->word1DecorVerts[3].x = -self->word1Width;
        self->word1DecorVerts[3].y = TO_FIXED(98);
    }

    self->word2DecorVerts[0].x = -self->word2Width;
    self->word2DecorVerts[0].y = TO_FIXED(186);
    self->word2DecorVerts[1].x = TO_FIXED(0);
    self->word2DecorVerts[1].y = TO_FIXED(186);
    self->word2DecorVerts[2].x = TO_FIXED(0);
    self->word2DecorVerts[2].y = TO_FIXED(202);
    self->word2DecorVerts[3].x = -self->word2Width;
    self->word2DecorVerts[3].y = TO_FIXED(202);

    self->zoneDecorVerts[0].x = TO_FIXED(ScreenInfo->size.x);
    self->zoneDecorVerts[0].y = TO_FIXED(154);
    self->zoneDecorVerts[1].x = TO_FIXED(120) + self->zoneDecorVerts[0].x;
    self->zoneDecorVerts[1].y = TO_FIXED(154);
    self->zoneDecorVerts[2].x = TO_FIXED(120) + self->zoneDecorVerts[0].x;
    self->zoneDecorVerts[2].y = TO_FIXED(162);
    self->zoneDecorVerts[3].x = self->zoneDecorVerts[0].x;
    self->zoneDecorVerts[3].y = TO_FIXED(162);

    self->stripVertsBlue[0].x = self->stripPos[0];
    self->stripVertsBlue[0].y = TO_FIXED(240);
    self->stripVertsBlue[1].x = TO_FIXED(64) + self->stripVertsBlue[0].x;
    self->stripVertsBlue[1].y = TO_FIXED(240);
    self->stripVertsBlue[2].x = TO_FIXED(304) + self->stripVertsBlue[0].x;
    self->stripVertsBlue[2].y = TO_FIXED(240);
    self->stripVertsBlue[3].x = TO_FIXED(240) + self->stripVertsBlue[0].x;
    self->stripVertsBlue[3].y = TO_FIXED(240);

    self->stripVertsRed[0].x = self->stripPos[1];
    self->stripVertsRed[0].y = TO_FIXED(240);
    self->stripVertsRed[1].x = TO_FIXED(128) + self->stripVertsRed[0].x;
    self->stripVertsRed[1].y = TO_FIXED(240);
    self->stripVertsRed[2].x = TO_FIXED(230) + self->stripVertsRed[0].x;
    self->stripVertsRed[2].y = TO_FIXED(240);
    self->stripVertsRed[3].x = TO_FIXED(102) + self->stripVertsRed[0].x;
    self->stripVertsRed[3].y = TO_FIXED(240);

    self->stripVertsOrange[0].x = self->stripPos[2];
    self->stripVertsOrange[0].y = TO_FIXED(240);
    self->stripVertsOrange[1].x = TO_FIXED(240) + self->stripVertsOrange[0].x;
    self->stripVertsOrange[1].y = TO_FIXED(240);
    self->stripVertsOrange[2].x = TO_FIXED(262) + self->stripVertsOrange[0].x;
    self->stripVertsOrange[2].y = TO_FIXED(240);
    self->stripVertsOrange[3].x = TO_FIXED(166) + self->stripVertsOrange[0].x;
    self->stripVertsOrange[3].y = TO_FIXED(240);

    self->stripVertsGreen[0].x = self->stripPos[3];
    self->stripVertsGreen[0].y = TO_FIXED(240);
    self->stripVertsGreen[1].x = TO_FIXED(32) + self->stripVertsGreen[0].x;
    self->stripVertsGreen[1].y = TO_FIXED(240);
    self->stripVertsGreen[2].x = TO_FIXED(160) + self->stripVertsGreen[0].x;
    self->stripVertsGreen[2].y = TO_FIXED(240);
    self->stripVertsGreen[3].x = TO_FIXED(128) + self->stripVertsGreen[0].x;
    self->stripVertsGreen[3].y = TO_FIXED(240);

    self->bgLCurtainVerts[0].x = TO_FIXED(0);
    self->bgLCurtainVerts[0].y = TO_FIXED(0);
    self->bgLCurtainVerts[1].x = (self->stripVertsBlue[1].x + self->stripVertsBlue[0].x) >> 1;
    self->bgLCurtainVerts[1].y = TO_FIXED(0);
    self->bgLCurtainVerts[2].x = (self->stripVertsBlue[3].x + self->stripVertsBlue[2].x) >> 1;
    self->bgLCurtainVerts[2].y = TO_FIXED(240);
    self->bgLCurtainVerts[3].x = TO_FIXED(0);
    self->bgLCurtainVerts[3].y = TO_FIXED(240);

    self->bgRCurtainVerts[0].x = (self->stripVertsBlue[1].x + self->stripVertsBlue[0].x) >> 1;
    self->bgRCurtainVerts[0].y = TO_FIXED(0);
    self->bgRCurtainVerts[1].x = TO_FIXED(ScreenInfo->size.x);
    self->bgRCurtainVerts[1].y = TO_FIXED(0);
    self->bgRCurtainVerts[2].x = TO_FIXED(ScreenInfo->size.x);
    self->bgRCurtainVerts[2].y = TO_FIXED(240);
    self->bgRCurtainVerts[3].x = (self->stripVertsBlue[3].x + self->stripVertsBlue[2].x) >> 1;
    self->bgRCurtainVerts[3].y = TO_FIXED(240);
}

void TitleCard_SetupTitleWords(void)
{
    RSDK_THIS(TitleCard);

    if (!self->zoneName.chars)
        RSDK.InitString(&self->zoneName, "UNTITLED", 0);

    RSDK.SetSpriteString(TitleCard->aniFrames, 1, &self->zoneName);

    int32 offset = TO_FIXED(40);
    for (int32 c = 0; c < self->zoneName.length; ++c) {
        self->charPos[c].y = offset;
        self->charVel[c]   = -TO_FIXED(8);
        offset += TO_FIXED(16);
    }

    for (int32 i = 0; i < 4; ++i) {
        self->zoneCharPos[i] = ((2 - self->zoneName.length) << 19) - ((i * 2) << 19);
        self->zoneCharVel[i] = TO_FIXED(4);
    }

    for (int32 c = 0; c < self->zoneName.length; ++c) {
        if (self->zoneName.chars[c] == (uint16)-1)
            self->titleCardWord2 = c + 1;
    }

    if (self->titleCardWord2) {
        self->word1Width = TO_FIXED(RSDK.GetStringWidth(TitleCard->aniFrames, 1, &self->zoneName, 0, self->titleCardWord2 - 1, 1) + 24);
        self->word2Width = TO_FIXED(RSDK.GetStringWidth(TitleCard->aniFrames, 1, &self->zoneName, self->titleCardWord2, 0, 1) + 24);
    }
    else {
        self->word2Width = TO_FIXED(RSDK.GetStringWidth(TitleCard->aniFrames, 1, &self->zoneName, 0, 0, 1) + 24);
    }

    self->zoneXPos  = TO_FIXED(ScreenInfo->center.x - ((ScreenInfo->center.x - 160) >> 3) + 72);
    self->word2XPos = TO_FIXED(ScreenInfo->center.x - ((ScreenInfo->center.x - 160) >> 3) + 72);

    if (self->word2Width < TO_FIXED(128))
        self->word2XPos -= TO_FIXED(40);

    self->word1XPos = self->word1Width - self->word2Width + self->word2XPos - TO_FIXED(32);
}

void TitleCard_HandleWordMovement(void)
{
    RSDK_THIS(TitleCard);

    if (self->titleCardWord2 > 0) {
        self->word1DecorVerts[1].x -= TO_FIXED(32);
        if (self->word1DecorVerts[1].x < self->word1XPos - TO_FIXED(16))
            self->word1DecorVerts[1].x = self->word1XPos - TO_FIXED(16);
        self->word1DecorVerts[0].x = self->word1DecorVerts[1].x - self->word1Width;

        self->word1DecorVerts[2].x -= TO_FIXED(32);
        if (self->word1DecorVerts[2].x < self->word1XPos)
            self->word1DecorVerts[2].x = self->word1XPos;
        self->word1DecorVerts[3].x = self->word1DecorVerts[2].x - self->word1Width;
    }

    self->word2DecorVerts[1].x -= TO_FIXED(32);
    if (self->word2DecorVerts[1].x < self->word2XPos - TO_FIXED(16))
        self->word2DecorVerts[1].x = self->word2XPos - TO_FIXED(16);
    self->word2DecorVerts[0].x = self->word2DecorVerts[1].x - self->word2Width;

    self->word2DecorVerts[2].x -= TO_FIXED(32);
    if (self->word2DecorVerts[2].x < self->word2XPos)
        self->word2DecorVerts[2].x = self->word2XPos;
    self->word2DecorVerts[3].x = self->word2DecorVerts[2].x - self->word2Width;

    self->zoneDecorVerts[1].x += TO_FIXED(32);
    if (self->zoneDecorVerts[1].x > self->zoneXPos - TO_FIXED(8))
        self->zoneDecorVerts[1].x = self->zoneXPos - TO_FIXED(8);
    self->zoneDecorVerts[0].x = self->zoneDecorVerts[1].x - TO_FIXED(120);

    self->zoneDecorVerts[2].x += TO_FIXED(32);
    if (self->zoneDecorVerts[2].x > self->zoneXPos)
        self->zoneDecorVerts[2].x = self->zoneXPos;
    self->zoneDecorVerts[3].x = self->zoneDecorVerts[2].x - TO_FIXED(120);

    if (self->decorationPos.y < TO_FIXED(12)) {
        self->decorationPos.x += TO_FIXED(2);
        self->decorationPos.y += TO_FIXED(2);
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

#if MANIA_USE_PLUS
    if (ActClear && ActClear->actClearActive)
        ActClear->actClearActive = false;
#endif

    Zone_ApplyWorldBounds();

    if (!globals->atlEnabled && !globals->suppressTitlecard)
        RSDK.SetEngineState(ENGINESTATE_PAUSED);

    self->timer += 24;
    if (self->timer >= 512) {
        self->word1DecorVerts[0].y += TO_FIXED(32);
        self->word1DecorVerts[1].y += TO_FIXED(32);
        self->word1DecorVerts[2].y += TO_FIXED(32);
        self->word1DecorVerts[3].y += TO_FIXED(32);

        self->word2DecorVerts[0].y -= TO_FIXED(32);
        self->word2DecorVerts[1].y -= TO_FIXED(32);
        self->word2DecorVerts[2].y -= TO_FIXED(32);
        self->word2DecorVerts[3].y -= TO_FIXED(32);

        self->zoneDecorVerts[0].y += TO_FIXED(32);
        self->zoneDecorVerts[1].y += TO_FIXED(32);
        self->zoneDecorVerts[2].y += TO_FIXED(32);
        self->zoneDecorVerts[3].y += TO_FIXED(32);

        self->state = TitleCard_State_OpeningBG;
    }

    self->word1DecorVerts[0].x += TO_FIXED(40);
    self->word1DecorVerts[1].x += TO_FIXED(40);
    self->word1DecorVerts[2].x += TO_FIXED(40);
    self->word1DecorVerts[3].x += TO_FIXED(40);

    self->word2DecorVerts[0].x += TO_FIXED(40);
    self->word2DecorVerts[1].x += TO_FIXED(40);
    self->word2DecorVerts[2].x += TO_FIXED(40);
    self->word2DecorVerts[3].x += TO_FIXED(40);

    self->zoneDecorVerts[0].x -= TO_FIXED(40);
    self->zoneDecorVerts[1].x -= TO_FIXED(40);
    self->zoneDecorVerts[2].x -= TO_FIXED(40);
    self->zoneDecorVerts[3].x -= TO_FIXED(40);
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

    self->vertMovePos[0].x += (self->vertTargetPos[0].x - self->vertMovePos[0].x - TO_FIXED(16)) / 6;
    if (self->vertMovePos[0].x < self->vertTargetPos[0].x)
        self->vertMovePos[0].x = self->vertTargetPos[0].x;

    self->vertMovePos[0].y += (self->vertTargetPos[0].y - self->vertMovePos[0].y - TO_FIXED(16)) / 6;
    if (self->vertMovePos[0].y < self->vertTargetPos[0].y)
        self->vertMovePos[0].y = self->vertTargetPos[0].y;

    self->vertMovePos[1].x += (self->vertTargetPos[1].x - self->vertMovePos[1].x - TO_FIXED(16)) / 6;
    if (self->vertMovePos[1].x < self->vertTargetPos[1].x)
        self->vertMovePos[1].x = self->vertTargetPos[1].x;

    self->vertMovePos[1].y += (self->vertTargetPos[1].y - self->vertMovePos[1].y - TO_FIXED(16)) / 6;
    if (self->vertMovePos[1].y < self->vertTargetPos[1].y)
        self->vertMovePos[1].y = self->vertTargetPos[1].y;

    self->stripVertsBlue[0].x = (self->vertMovePos[0].x - TO_FIXED(240)) + self->stripVertsBlue[3].x;
    self->stripVertsBlue[0].y = self->vertMovePos[0].x;
    self->stripVertsBlue[1].x = (self->vertMovePos[0].x - TO_FIXED(240)) + self->stripVertsBlue[2].x;
    self->stripVertsBlue[1].y = self->vertMovePos[0].x;

    self->stripVertsRed[0].x = (self->vertMovePos[0].y - TO_FIXED(240)) + self->stripVertsRed[3].x;
    self->stripVertsRed[0].y = self->vertMovePos[0].y;
    self->stripVertsRed[1].x = (self->vertMovePos[0].y - TO_FIXED(240)) + self->stripVertsRed[2].x;
    self->stripVertsRed[1].y = self->vertMovePos[0].y;

    self->stripVertsOrange[0].x = (self->vertMovePos[1].x - TO_FIXED(240)) + self->stripVertsOrange[3].x;
    self->stripVertsOrange[0].y = self->vertMovePos[1].x;
    self->stripVertsOrange[1].x = (self->vertMovePos[1].x - TO_FIXED(240)) + self->stripVertsOrange[2].x;
    self->stripVertsOrange[1].y = self->vertMovePos[1].x;

    self->stripVertsGreen[0].x = (self->vertMovePos[1].y - TO_FIXED(240)) + self->stripVertsGreen[3].x;
    self->stripVertsGreen[0].y = self->vertMovePos[1].y;
    self->stripVertsGreen[1].x = (self->vertMovePos[1].y - TO_FIXED(240)) + self->stripVertsGreen[2].x;
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
#if MANIA_USE_PLUS
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
                camera->state          = Camera_State_FollowXY;
                Camera->centerBounds.x = TO_FIXED(2);
                Camera->centerBounds.y = TO_FIXED(2);
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
        speed = TO_FIXED(32);

        self->zoneXPos -= speed;
        self->word1XPos -= speed;
        self->word2XPos += speed;
        self->actNumPos.x += speed;
        self->actNumPos.y += speed;

        self->word1DecorVerts[0].x -= speed;
        self->word1DecorVerts[1].x -= speed;
        self->word1DecorVerts[2].x -= speed;
        self->word1DecorVerts[3].x -= speed;

        self->word2DecorVerts[0].x += speed;
        self->word2DecorVerts[1].x += speed;
        self->word2DecorVerts[2].x += speed;
        self->word2DecorVerts[3].x += speed;

        self->zoneDecorVerts[0].x -= speed;
        self->zoneDecorVerts[1].x -= speed;
        self->zoneDecorVerts[2].x -= speed;
        self->zoneDecorVerts[3].x -= speed;
    }

    if (self->actionTimer == 60 && globals->gameMode < MODE_TIMEATTACK) {
        SceneInfo->timeEnabled = true;
    }

    if (self->actionTimer > 80) {
        globals->atlEnabled  = false;
        globals->enableIntro = false;
        if (globals->gameMode >= MODE_TIMEATTACK) {
            if (globals->gameMode == MODE_COMPETITION) {
#if MANIA_USE_PLUS
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
#if MANIA_USE_PLUS
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

#if MANIA_USE_PLUS
    StateMachine_Run(TitleCard->finishedCB);
#endif
}

void TitleCard_Draw_SlideIn(void)
{
    RSDK_THIS(TitleCard);

    color colors[5];
#if MANIA_USE_PLUS
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
            RSDK.DrawRect(0, 0, ScreenInfo->size.x, ScreenInfo->size.y, 0, 0xFF, INK_NONE, true);

        // Blue
        int32 height = self->timer;
        if (self->timer < 512)
            RSDK.DrawRect(0, ScreenInfo->center.y - (height >> 1), ScreenInfo->size.x, height, colors[3], 0xFF, INK_NONE, true);

        // Red
        height = self->timer - 128;
        if (self->timer > 128 && self->timer < 640)
            RSDK.DrawRect(0, ScreenInfo->center.y - (height >> 1), ScreenInfo->size.x, height, colors[2], 0xFF, INK_NONE, true);

        // Orange
        height = self->timer - 256;
        if (self->timer > 256 && self->timer < 768)
            RSDK.DrawRect(0, ScreenInfo->center.y - (height >> 1), ScreenInfo->size.x, height, colors[0], 0xFF, INK_NONE, true);

        // Green
        height = self->timer - 384;
        if (self->timer > 384 && self->timer < 896)
            RSDK.DrawRect(0, ScreenInfo->center.y - (height >> 1), ScreenInfo->size.x, height, colors[1], 0xFF, INK_NONE, true);

        // Yellow
        height = self->timer - 512;
        if (self->timer > 512)
            RSDK.DrawRect(0, ScreenInfo->center.y - (height >> 1), ScreenInfo->size.x, height, colors[4], 0xFF, INK_NONE, true);
    }

    // Draw the BG thingos
    if (self->titleCardWord2 > 0)
        RSDK.DrawFace(self->word1DecorVerts, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);

    RSDK.DrawFace(self->word2DecorVerts, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawFace(self->zoneDecorVerts, 4, 0xF0, 0xF0, 0xF0, 0xFF, INK_NONE);

    // Draw Act Number
#if MANIA_USE_PLUS
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
#if MANIA_USE_PLUS
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

    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];

    // Draw Yellow BG
    if (!globals->atlEnabled && !globals->suppressTitlecard)
        RSDK.DrawRect(0, 0, ScreenInfo->size.x, ScreenInfo->size.y, colors[4], 0xFF, INK_NONE, true);

    // Draw Orange Strip
    if (self->vertMovePos[1].x < TO_FIXED(240))
        RSDK.DrawFace(self->stripVertsOrange, 4, (colors[0] >> 16) & 0xFF, (colors[0] >> 8) & 0xFF, (colors[0] >> 0) & 0xFF, 0xFF, INK_NONE);

    // Draw Green Strip
    if (self->vertMovePos[1].y < TO_FIXED(240))
        RSDK.DrawFace(self->stripVertsGreen, 4, (colors[1] >> 16) & 0xFF, (colors[1] >> 8) & 0xFF, (colors[1] >> 0) & 0xFF, 0xFF, INK_NONE);

    // Draw Red Strip
    if (self->vertMovePos[0].y < TO_FIXED(240))
        RSDK.DrawFace(self->stripVertsRed, 4, (colors[2] >> 16) & 0xFF, (colors[2] >> 8) & 0xFF, (colors[2] >> 0) & 0xFF, 0xFF, INK_NONE);

    // Draw Blue Strip
    if (self->vertMovePos[0].x < TO_FIXED(240))
        RSDK.DrawFace(self->stripVertsBlue, 4, (colors[3] >> 16) & 0xFF, (colors[3] >> 8) & 0xFF, (colors[3] >> 0) & 0xFF, 0xFF, INK_NONE);

    // Draw "Sonic Mania"
    if (!globals->atlEnabled && !globals->suppressTitlecard) {
#if MANIA_USE_PLUS
        self->decorationAnimator.frameID = 2 * (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) + 1;
#else
        self->decorationAnimator.frameID = 2 * 0 + 1;
#endif
        RSDK.DrawSprite(&self->decorationAnimator, &self->decorationPos, true);
    }

    // Draw the BG thingos
    if (self->titleCardWord2 > 0)
        RSDK.DrawFace(self->word1DecorVerts, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);

    RSDK.DrawFace(self->word2DecorVerts, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawFace(self->zoneDecorVerts, 4, 0xF0, 0xF0, 0xF0, 0xFF, INK_NONE);

    // Draw "ZONE"
    RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 170, screen->size.x, SCREEN_YSIZE);

    Vector2 drawPos;
    drawPos.x = self->zoneXPos;
    for (int32 i = 0; i < 4; ++i) {
        self->zoneLetterAnimator.frameID = i;
        drawPos.y                        = TO_FIXED(186) + self->zoneCharPos[i];
        RSDK.DrawSprite(&self->zoneLetterAnimator, &drawPos, true);
    }

    // Draw TitleCard Word 1 (if there are 2 words)
    if (self->titleCardWord2 > 0) {
        RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, screen->size.x, 130);
        drawPos.x = self->word1XPos - TO_FIXED(20);
        drawPos.y = TO_FIXED(114);
        RSDK.DrawText(&self->nameLetterAnimator, &drawPos, &self->zoneName, 0, self->titleCardWord2, ALIGN_CENTER, 1, 0, self->charPos, true);
    }

    // Draw TitleCard Word 2 (if there are 2 words, otherwise draw the entire zoneName)
    RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, screen->size.x, 170);
    drawPos.y = TO_FIXED(154);
    drawPos.x = self->word2XPos - TO_FIXED(20);
    RSDK.DrawText(&self->nameLetterAnimator, &drawPos, &self->zoneName, self->titleCardWord2, 0, ALIGN_CENTER, 1, 0, self->charPos, true);

    RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, screen->size.x, screen->size.y);

    // Draw Act Number
    if (self->actID != 3) {
        if (self->actNumScale > 0) {
            self->drawFX  = FX_SCALE;
            self->scale.x = MIN(self->actNumScale, 0x200);
            self->scale.y = 0x200;

#if MANIA_USE_PLUS
            self->decorationAnimator.frameID = (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) ? 2 : 0;
#else
            self->decorationAnimator.frameID = 0;
#endif
            RSDK.DrawSprite(&self->decorationAnimator, &self->actNumPos, true);

            self->scale.x = CLAMP(self->actNumScale - 0x100, 0, 0x200);
            RSDK.DrawSprite(&self->actNumbersAnimator, &self->actNumPos, true);
            self->drawFX = FX_NONE;
        }
    }
}

void TitleCard_Draw_SlideAway(void)
{
    RSDK_THIS(TitleCard);

    color colors[5];
#if MANIA_USE_PLUS
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
        RSDK.DrawFace(self->bgLCurtainVerts, 4, (colors[4] >> 16) & 0xFF, (colors[4] >> 8) & 0xFF, (colors[4] >> 0) & 0xFF, 0xFF, INK_NONE);
        RSDK.DrawFace(self->bgRCurtainVerts, 4, (colors[4] >> 16) & 0xFF, (colors[4] >> 8) & 0xFF, (colors[4] >> 0) & 0xFF, 0xFF, INK_NONE);
    }

    // Orange Strip
    if (self->vertMovePos[1].x < TO_FIXED(240))
        RSDK.DrawFace(self->stripVertsOrange, 4, (colors[0] >> 16) & 0xFF, (colors[0] >> 8) & 0xFF, (colors[0] >> 0) & 0xFF, 0xFF, INK_NONE);

    // Green Strip
    if (self->vertMovePos[1].y < TO_FIXED(240))
        RSDK.DrawFace(self->stripVertsGreen, 4, (colors[1] >> 16) & 0xFF, (colors[1] >> 8) & 0xFF, (colors[1] >> 0) & 0xFF, 0xFF, INK_NONE);

    // Red Strip
    if (self->vertMovePos[0].y < TO_FIXED(240))
        RSDK.DrawFace(self->stripVertsRed, 4, (colors[2] >> 16) & 0xFF, (colors[2] >> 8) & 0xFF, (colors[2] >> 0) & 0xFF, 0xFF, INK_NONE);

    // Blue Strip
    if (self->vertMovePos[0].x < TO_FIXED(240))
        RSDK.DrawFace(self->stripVertsBlue, 4, (colors[3] >> 16) & 0xFF, (colors[3] >> 8) & 0xFF, (colors[3] >> 0) & 0xFF, 0xFF, INK_NONE);

    // Draw "Sonic Mania"
    if (!globals->atlEnabled && !globals->suppressTitlecard) {
#if MANIA_USE_PLUS
        self->decorationAnimator.frameID = 2 * (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) + 1;
#else
        self->decorationAnimator.frameID = 2 * 0 + 1;
#endif
        RSDK.DrawSprite(&self->decorationAnimator, &self->decorationPos, true);
    }

    // Draw Act Number
    if (self->actID != 3 && self->actNumScale > 0) {
#if MANIA_USE_PLUS
        self->decorationAnimator.frameID = (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) ? 2 : 0;
#else
        self->decorationAnimator.frameID = 0;
#endif
        RSDK.DrawSprite(&self->decorationAnimator, &self->actNumPos, true);
        RSDK.DrawSprite(&self->actNumbersAnimator, &self->actNumPos, true);
    }

    // Draw the BG thingos
    if (self->titleCardWord2 > 0)
        RSDK.DrawFace(self->word1DecorVerts, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);

    RSDK.DrawFace(self->word2DecorVerts, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    RSDK.DrawFace(self->zoneDecorVerts, 4, 0xF0, 0xF0, 0xF0, 0xFF, INK_NONE);

    // Draw "ZONE"
    Vector2 drawPos;
    drawPos.x = self->zoneXPos;
    drawPos.y = TO_FIXED(186);
    for (int32 i = 0; i < 4; ++i) {
        self->zoneLetterAnimator.frameID = i;
        RSDK.DrawSprite(&self->zoneLetterAnimator, &drawPos, true);
    }

    // Draw TitleCard Word 1 (if there are 2 words)
    if (self->titleCardWord2 > 0) {
        drawPos.y = TO_FIXED(114);
        drawPos.x = self->word1XPos - TO_FIXED(20);
        RSDK.DrawText(&self->nameLetterAnimator, &drawPos, &self->zoneName, 0, self->titleCardWord2, ALIGN_CENTER, 1, 0, 0, true);
    }

    // Draw TitleCard Word 2 (if there are 2 words, otherwise draw the entire zoneName)
    drawPos.y = TO_FIXED(154);
    drawPos.x = self->word2XPos - TO_FIXED(20);
    RSDK.DrawText(&self->nameLetterAnimator, &drawPos, &self->zoneName, self->titleCardWord2, 0, ALIGN_CENTER, 1, 0, 0, true);
}

#if RETRO_INCLUDE_EDITOR
void TitleCard_EditorDraw(void)
{
    RSDK_THIS(TitleCard);
    RSDK.SetSpriteAnimation(TitleCard->aniFrames, 0, &self->decorationAnimator, true, 3);

    RSDK.DrawSprite(&self->decorationAnimator, NULL, false);
}

void TitleCard_EditorLoad(void)
{
    TitleCard->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(TitleCard, actID);
    RSDK_ENUM_VAR("Act 1", ACT_1);
    RSDK_ENUM_VAR("Act 2", ACT_2);
    RSDK_ENUM_VAR("Act 3", ACT_3);
    RSDK_ENUM_VAR("No Act", ACT_NONE);
}
#endif

void TitleCard_Serialize(void)
{
    RSDK_EDITABLE_VAR(TitleCard, VAR_STRING, zoneName);
    RSDK_EDITABLE_VAR(TitleCard, VAR_UINT8, actID);
}
