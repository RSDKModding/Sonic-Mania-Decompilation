// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LoveTester Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLoveTester *LoveTester;

void LoveTester_Update(void)
{
    RSDK_THIS(LoveTester);

    StateMachine_Run(self->state);
    StateMachine_Run(self->stateLights);

    for (int32 i = 0; i < 10; ++i) RSDK.ProcessAnimation(&self->lightAnimator[i]);
}

void LoveTester_LateUpdate(void) {}

void LoveTester_StaticUpdate(void)
{
    foreach_active(LoveTester, loveTester) { RSDK.AddDrawListRef(Zone->objectDrawGroup[1], RSDK.GetEntitySlot(loveTester)); }
}

void LoveTester_Draw(void)
{
    RSDK_THIS(LoveTester);

    if (self->state == LoveTester_State_HeartParticles)
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    else
        LoveTester_DrawSprites();
}

void LoveTester_Create(void *data)
{
    RSDK_THIS(LoveTester);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    LoveTester_SetupHitboxes();

    if (data) {
        self->updateRange.x = 0x100000;
        self->updateRange.y = 0x100000;
        self->active        = ACTIVE_NORMAL;
        self->drawGroup     = Zone->objectDrawGroup[1];
        self->state         = LoveTester_State_HeartParticles;
    }
    else {
        self->state = LoveTester_State_Init;
    }
}

void LoveTester_StageLoad(void)
{
    LoveTester->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/LoveTester.bin", SCOPE_STAGE);

    LoveTester_SetupHitboxes();
    LoveTester_SetupLightOffsets();

    LoveTester->tvOffsetTop.x    = 0;
    LoveTester->tvOffsetTop.y    = -0x300000;
    LoveTester->tvOffsetBottom.x = 0;
    LoveTester->tvOffsetBottom.y = 0x320000;

    LoveTester->active   = ACTIVE_ALWAYS;
    LoveTester->sfxScore = RSDK.GetSfx("SPZ/Score.wav");
}

void LoveTester_SetupHitboxes(void)
{
    LoveTester->hitboxEntry.left   = -16;
    LoveTester->hitboxEntry.top    = -16;
    LoveTester->hitboxEntry.right  = 16;
    LoveTester->hitboxEntry.bottom = 16;

    LoveTester->hitboxL.left   = -24;
    LoveTester->hitboxL.top    = -20;
    LoveTester->hitboxL.right  = -18;
    LoveTester->hitboxL.bottom = 20;

    LoveTester->hitboxR.left   = 18;
    LoveTester->hitboxR.top    = -20;
    LoveTester->hitboxR.right  = 24;
    LoveTester->hitboxR.bottom = 20;
}

void LoveTester_SetupLightOffsets(void)
{
    LoveTester->lightOffset[0].x = -0x1C0000;
    LoveTester->lightOffset[0].y = -0x1F0000;
    LoveTester->lightOffset[1].x = 0x1C0000;
    LoveTester->lightOffset[1].y = -0x1F0000;
    LoveTester->lightOffset[2].x = -0x1C0000;
    LoveTester->lightOffset[2].y = -0xF0000;
    LoveTester->lightOffset[3].x = 0x1C0000;
    LoveTester->lightOffset[3].y = -0xF0000;
    LoveTester->lightOffset[4].x = -0x1C0000;
    LoveTester->lightOffset[4].y = 0x10000;
    LoveTester->lightOffset[5].x = 0x1C0000;
    LoveTester->lightOffset[5].y = 0x10000;
    LoveTester->lightOffset[6].x = -0x1C0000;
    LoveTester->lightOffset[6].y = 0x110000;
    LoveTester->lightOffset[7].x = 0x1C0000;
    LoveTester->lightOffset[7].y = 0x110000;
    LoveTester->lightOffset[8].x = -0x1C0000;
    LoveTester->lightOffset[8].y = 0x210000;
    LoveTester->lightOffset[9].x = 0x1C0000;
    LoveTester->lightOffset[9].y = 0x210000;
}

void LoveTester_DrawSprites(void)
{
    RSDK_THIS(LoveTester);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    if (SceneInfo->currentDrawGroup != Zone->objectDrawGroup[1] || SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(LoveTester->aniFrames, 1, &self->mainAnimator, true, 0);
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);

        for (int32 i = 0; i < 10; ++i) {
            self->position.x = storeX + LoveTester->lightOffset[i].x;
            self->position.y = storeY + LoveTester->lightOffset[i].y;
            RSDK.DrawSprite(&self->lightAnimator[i], NULL, false);
        }

        self->position.x = storeX + LoveTester->tvOffsetTop.x;
        self->position.y = storeY + LoveTester->tvOffsetTop.y;
        LoveTester_DrawTVDisplay(self->tvDisplayTop, self->tvFrameTop, self->isTVActiveTop);

        self->position.x = storeX + LoveTester->tvOffsetBottom.x;
        self->position.y = storeY + LoveTester->tvOffsetBottom.y;
        LoveTester_DrawTVDisplay(self->tvDisplayBottom, self->tvFrameBottom, self->isTVActiveBottom);

        self->position.x = storeX;
        self->position.y = storeY;
    }

    if (SceneInfo->currentDrawGroup == Zone->objectDrawGroup[1] || SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(LoveTester->aniFrames, 0, &self->mainAnimator, true, 0);
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    }
}

void LoveTester_DrawTVDisplay(uint8 displayList, uint8 frame, bool32 isTVActive)
{
    RSDK_THIS(LoveTester);

    // Draw TV (off)
    RSDK.SetSpriteAnimation(LoveTester->aniFrames, 3, &self->mainAnimator, true, 0);
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    if (isTVActive) {
        // Draw TV (on)
        RSDK.SetSpriteAnimation(LoveTester->aniFrames, 3, &self->mainAnimator, true, 1);
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    }

    // Draw Extras
    switch (displayList) {
        case LOVETESTER_LIST_SONIC:
        case LOVETESTER_LIST_TAILS:
        case LOVETESTER_LIST_KNUX:
        case LOVETESTER_LIST_EGGMAN:
        case LOVETESTER_LIST_AMY:
        case LOVETESTER_LIST_HEART:
        case LOVETESTER_LIST_HEARTBROKEN:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 4 + displayList, &self->mainAnimator, true, frame);
            RSDK.DrawSprite(&self->mainAnimator, NULL, false);
            break;

        default: break;
    }
}

void LoveTester_CheckPlayerCollisions_Solid(void)
{
    RSDK_THIS(LoveTester);

    foreach_active(Player, player)
    {
        int32 storeX = player->position.x;
        int32 storeY = player->position.y;
        int32 velX   = player->velocity.x;
        int32 velY   = player->velocity.y;
        int32 sideL  = Player_CheckCollisionBox(player, self, &LoveTester->hitboxL);
        int32 sideR  = Player_CheckCollisionBox(player, self, &LoveTester->hitboxR);

        if (sideL == C_TOP || sideL == C_BOTTOM || sideR == C_TOP || sideR == C_BOTTOM) {
            player->onGround   = false;
            player->velocity.x = velX;
            player->velocity.y = velY;
            player->position.x = storeX;
            player->position.y = storeY;
        }
    }
}

void LoveTester_CheckPlayerCollisions_Entry(bool32 allowSidekick)
{
    RSDK_THIS(LoveTester);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        if (allowSidekick || !player->sidekick) {
            if (!((1 << playerID) & self->activePlayers)) {
                if (Player_CheckBadnikTouch(player, self, &LoveTester->hitboxEntry)) {
                    self->activePlayers |= 1 << playerID;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);

                    player->nextGroundState = StateMachine_None;
                    player->nextAirState    = StateMachine_None;
                    player->velocity.x >>= 15;
                    player->velocity.y >>= 15;
                    player->onGround = false;
                    player->state    = Player_State_Static;

                    if (!self->playerPtr) {
                        self->playerPtr = player;
                        if (player->camera) {
                            player->camera->target         = (Entity *)self;
                            player->camera->disableYOffset = false;
                        }
                    }
                }
            }

            if ((1 << playerID) & self->activePlayers) {
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                player->position.x += (self->position.x - player->position.x) >> 1;
                player->position.y += (self->position.y + 0x10000 - player->position.y) >> 1;
                player->state = Player_State_Static;

                if (self->matchingFinished) {
                    self->activePlayers &= ~(1 << playerID);
                    player->state          = Player_State_Air;
                    player->tileCollisions = TILECOLLISION_DOWN;
                    player->onGround       = false;
                    player->velocity.x     = 0;
                    player->velocity.y     = 0;
                    if (player->camera && self->playerPtr == player) {
                        player->camera->target         = (Entity *)player;
                        player->camera->disableYOffset = true;
                    }
                }
            }
        }
    }
}

void LoveTester_GiveScore(EntityPlayer *player)
{
    EntityScoreBonus *bonus = CREATE_ENTITY(ScoreBonus, NULL, player->position.x, player->position.y);
    bonus->drawGroup        = Zone->objectDrawGroup[1];
    bonus->animator.frameID = 0;

    Player_GiveScore(player, 100);
    RSDK.PlaySfx(LoveTester->sfxScore, false, 255);
}

void LoveTester_CreateHeartParticles(void)
{
    RSDK_THIS(LoveTester);

    for (int32 velX = 0, frame = 0; velX < 0x10000; velX += 0x4000, ++frame) {
        EntityLoveTester *child = CREATE_ENTITY(LoveTester, INT_TO_VOID(true), self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(LoveTester->aniFrames, 4, &child->mainAnimator, true, frame & 1);
        child->velocity.x = velX - 0x6000;
    }
}

void LoveTester_State_Init(void)
{
    RSDK_THIS(LoveTester);

    for (int32 i = 0; i < 10; ++i) RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[i], true, 5);

    self->playerPtr        = NULL;
    self->matchingFinished = false;
    self->activePlayers    = 0;

    self->state = LoveTester_State_WaitForActivated;
    LoveTester_State_WaitForActivated();
}

void LoveTester_State_WaitForActivated(void)
{
    RSDK_THIS(LoveTester);

    LoveTester_CheckPlayerCollisions_Solid();
    LoveTester_CheckPlayerCollisions_Entry(false);

    if (self->playerPtr) {
        self->timer            = 0;
        self->isTVActiveTop    = true;
        self->isTVActiveBottom = true;
        self->tvDisplayTop     = LOVETESTER_LIST_NONE;
        self->tvFrameTop       = 0;
        self->tvDisplayBottom  = LOVETESTER_LIST_NONE;
        self->tvFrameBottom    = 0;
        self->state            = LoveTester_State_SetupTopDisplay;
    }
}

void LoveTester_State_SetupTopDisplay(void)
{
    RSDK_THIS(LoveTester);

    LoveTester_CheckPlayerCollisions_Solid();
    LoveTester_CheckPlayerCollisions_Entry(true);

    if (self->timer >= 8) {
        EntityPlayer *player   = self->playerPtr;
        self->timer            = 0;
        self->isTVActiveTop    = true;
        self->isTVActiveBottom = true;

        switch (player->characterID) {
            case ID_SONIC: self->tvDisplayTop = LOVETESTER_LIST_SONIC; break;
            case ID_TAILS: self->tvDisplayTop = LOVETESTER_LIST_TAILS; break;
            case ID_KNUCKLES: self->tvDisplayTop = LOVETESTER_LIST_KNUX; break;
        }

        self->nextDisplayBottom = self->tvDisplayTop;
        while (self->nextDisplayBottom == self->tvDisplayTop) {
            self->nextDisplayBottom = RSDK.Rand(LOVETESTER_LIST_SONIC, LOVETESTER_LIST_HEART);
        }

        self->state = LoveTester_State_SetupMatching;
    }
    else {
        self->isTVActiveTop    = !(self->timer & 2);
        self->isTVActiveBottom = !(self->timer & 2);
        self->timer++;
    }
}

void LoveTester_State_SetupMatching(void)
{
    RSDK_THIS(LoveTester);

    LoveTester_CheckPlayerCollisions_Solid();
    LoveTester_CheckPlayerCollisions_Entry(true);

    if (self->timer < 168) {
        int32 timer = self->timer & 0b10000000000000000000000000000011;

        bool32 shouldChangeDisplay = !timer;
        if (timer < 0)
            shouldChangeDisplay = (int32)((timer - 1) | 0xFFFFFFFC) == -1;

        if (shouldChangeDisplay) {
            int32 displayBottom = self->tvDisplayBottom;
            int32 displayTop    = self->tvDisplayTop;

            while (true) {
                self->tvDisplayBottom = displayTop;
                if (displayTop != self->tvDisplayTop && displayTop != displayBottom)
                    break;
                displayTop = RSDK.Rand(LOVETESTER_LIST_SONIC, LOVETESTER_LIST_HEART);
            }
        }

        if (self->timer >= 128)
            self->tvDisplayBottom = LOVETESTER_LIST_NONE;

        switch (self->timer++) {
            default: break;

            case 0:
                self->lightsID    = 0;
                self->stateLights = LoveTester_StateLights_FlashSlow;
                break;

            case 80:
                self->lightsID    = 0;
                self->stateLights = LoveTester_StateLights_FlashMed;
                break;

            case 128:
                self->lightsID    = 0;
                self->stateLights = LoveTester_StateLights_FlashFast;
                break;
        }
    }
    else {
        EntityPlayer *player = self->playerPtr;
        self->lightsID       = 0;
        self->stateLights    = StateMachine_None;

        switch (player->characterID) {
            case ID_SONIC:
                if (self->nextDisplayBottom == LOVETESTER_LIST_EGGMAN)
                    self->state = LoveTester_State_BadMatch;
                else if (self->nextDisplayBottom == LOVETESTER_LIST_AMY)
                    self->state = LoveTester_State_UnluckyMatch;
                else
                    self->state = LoveTester_State_GoodMatch;
                break;

            case ID_TAILS:
                if (self->nextDisplayBottom == LOVETESTER_LIST_SONIC)
                    self->state = LoveTester_State_UnluckyMatch;
                else if (self->nextDisplayBottom == LOVETESTER_LIST_EGGMAN)
                    self->state = LoveTester_State_UnluckyMatch;
                else
                    self->state = LoveTester_State_GoodMatch;
                break;

            case ID_KNUCKLES:
                if (self->nextDisplayBottom == LOVETESTER_LIST_SONIC)
                    self->state = LoveTester_State_BadMatch;
                else if (self->nextDisplayBottom == LOVETESTER_LIST_EGGMAN)
                    self->state = LoveTester_State_UnluckyMatch;
                else
                    self->state = LoveTester_State_GoodMatch;
                break;
        }

        if (self->state == LoveTester_State_UnluckyMatch) {
            self->tvFrameTop = 1;
            LoveTester_CreateHeartParticles();
        }
        else if (self->state == LoveTester_State_BadMatch) {
            self->tvFrameTop = 2;
        }

        self->tvDisplayBottom = self->nextDisplayBottom;
        self->timer           = 0;
    }
}

void LoveTester_State_UnluckyMatch(void)
{
    RSDK_THIS(LoveTester);
    EntityPlayer *player = self->playerPtr;

    if (self->timer >= 64) {
        self->timer            = 0;
        self->tvDisplayBottom  = self->nextDisplayBottom;
        self->matchingFinished = true;
        self->state            = LoveTester_State_ReleasePlayers;
    }
    else {
        switch (self->timer++ % 32) {
            default: break;

            case 0:
                self->tvDisplayBottom = self->nextDisplayBottom;
                LoveTester_GiveScore(player);
                break;

            case 16:
                self->tvDisplayBottom = 6;
                LoveTester_GiveScore(player);
                break;
        }
    }

    LoveTester_CheckPlayerCollisions_Solid();
    LoveTester_CheckPlayerCollisions_Entry(true);
}

void LoveTester_State_GoodMatch(void)
{
    RSDK_THIS(LoveTester);

    if (self->timer >= 24) {
        self->timer            = 0;
        self->matchingFinished = true;
        self->state            = LoveTester_State_ReleasePlayers;
    }
    else {
        if (!self->timer)
            LoveTester_GiveScore(self->playerPtr);
        ++self->timer;
    }

    LoveTester_CheckPlayerCollisions_Solid();
    LoveTester_CheckPlayerCollisions_Entry(true);
}

void LoveTester_State_BadMatch(void)
{
    RSDK_THIS(LoveTester);

    if (self->timer >= 64) {
        self->timer            = 0;
        self->tvDisplayBottom  = self->nextDisplayBottom;
        self->matchingFinished = true;
        self->state            = LoveTester_State_ReleasePlayers;
    }
    else {
        switch (self->timer++ % 32) {
            default: break;
            case 0: self->tvDisplayBottom = self->nextDisplayBottom; break;
            case 16: self->tvDisplayBottom = 7; break;
        }
    }

    LoveTester_CheckPlayerCollisions_Solid();
    LoveTester_CheckPlayerCollisions_Entry(true);
}

void LoveTester_State_ReleasePlayers(void)
{
    RSDK_THIS(LoveTester);

    LoveTester_CheckPlayerCollisions_Solid();

    foreach_active(Player, player)
    {
        if (player == self->playerPtr) {
            if (!Player_CheckCollisionTouch(player, self, &LoveTester->hitboxEntry)) {
                self->activePlayers = 0;
                self->state         = LoveTester_State_Init;
            }
        }
    }
}

void LoveTester_State_HeartParticles(void)
{
    RSDK_THIS(LoveTester);

    self->velocity.y -= 0x700;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void LoveTester_StateLights_FlashSlow(void)
{
    RSDK_THIS(LoveTester);

    switch (self->lightsID % 40) {
        case 0:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[1], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[8], true, 0);
            break;

        case 8:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[3], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[6], true, 0);
            break;

        case 16:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[5], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[4], true, 0);
            break;

        case 24:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[7], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[2], true, 0);
            break;

        case 32:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[9], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[0], true, 0);
            break;
    }

    ++self->lightsID;
}

void LoveTester_StateLights_FlashMed(void)
{
    RSDK_THIS(LoveTester);

    switch (self->lightsID % 24) {
        case 0:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[0], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[1], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[8], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[9], true, 0);
            break;

        case 8:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[2], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[3], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[6], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[7], true, 0);
            break;

        case 16:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[4], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[5], true, 0);
            break;
    }

    ++self->lightsID;
}

void LoveTester_StateLights_FlashFast(void)
{
    RSDK_THIS(LoveTester);

    switch (self->lightsID % 20) {
        case 0:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[0], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[1], true, 0);
            break;

        case 2:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[2], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[3], true, 0);
            break;

        case 4:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[4], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[5], true, 0);
            break;

        case 6:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[6], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[7], true, 0);
            break;

        case 8:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[8], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[9], true, 0);
            break;
    }

    ++self->lightsID;
}

#if GAME_INCLUDE_EDITOR
void LoveTester_EditorDraw(void)
{
    RSDK_THIS(LoveTester);

    for (int32 i = 0; i < 10; ++i) RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->lightAnimator[i], true, 5);

    LoveTester_Draw();
}

void LoveTester_EditorLoad(void)
{
    LoveTester->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/LoveTester.bin", SCOPE_STAGE);

    LoveTester_SetupHitboxes();
    LoveTester_SetupLightOffsets();
}
#endif

void LoveTester_Serialize(void) {}
