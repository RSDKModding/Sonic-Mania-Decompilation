// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SP500 Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSP500 *SP500;

void SP500_Update(void)
{
    RSDK_THIS(SP500);

    StateMachine_Run(self->state);
}

void SP500_LateUpdate(void) {}

void SP500_StaticUpdate(void) {}

void SP500_Draw(void)
{
    RSDK_THIS(SP500);

    Vector2 drawPos;

    // Edge (L)
    drawPos.x              = self->position.x - 0x180000;
    drawPos.y              = self->position.y;
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Rails
    self->animator.frameID = 3;
    int32 pos              = drawPos.x;
    for (int32 l = 0; l < self->len; ++l) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        pos = drawPos.x + 0x100000;
        drawPos.x += 0x100000;
    }

    // Edge (R)
    drawPos.x = pos - 0x100000;
    self->direction ^= FLIP_X;
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Body
    self->direction ^= FLIP_X;
    drawPos.x              = self->position.x + self->rowPrintPos;
    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Dispenser
    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Green Light
    self->inkEffect = INK_ADD;
    if (self->showGreenLight) {
        self->animator.frameID = 4;
        RSDK.DrawSprite(&self->animator, &drawPos, false);
    }

    // Red Light
    if (self->showRedLight) {
        self->animator.frameID = 5;
        RSDK.DrawSprite(&self->animator, &drawPos, false);
    }

    self->inkEffect = INK_NONE;
}

void SP500_Create(void *data)
{
    RSDK_THIS(SP500);
    self->drawFX = FX_FLIP;

    if (!SceneInfo->inEditor) {
        if (data) {
            self->active  = ACTIVE_NORMAL;
            self->visible = false;
            self->state   = SP500_State_MoveToTarget;
        }
        else {
            self->active  = ACTIVE_BOUNDS;
            self->visible = true;
            self->offL <<= 20;
            self->offR <<= 20;
            self->drawGroup     = Zone->objectDrawGroup[1];
            self->updateRange.x = (self->len + 4) << 20;
            self->updateRange.y = 0x800000;

            self->rowPrintPos = self->startDir == 0 ? self->offL : self->offR;

            if (self->printDir == 0)
                self->printRowID = self->height - 1;

            self->srcC.x = self->srcC.x >> 16;
            self->srcC.y = self->srcC.y >> 16;
            self->srcM.x = self->srcM.x >> 16;
            self->srcM.y = self->srcM.y >> 16;
            self->srcY.x = self->srcY.x >> 16;
            self->srcY.y = self->srcY.y >> 16;

            self->alpha = 0xC0;
            RSDK.SetSpriteAnimation(SP500->aniFrames, 0, &self->animator, true, 0);
            self->state = SP500_State_WaitForEntry;
        }
    }
}

void SP500_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        SP500->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/SP500.bin", SCOPE_STAGE);

    SP500->hitboxSideL.left   = -32;
    SP500->hitboxSideL.top    = -24;
    SP500->hitboxSideL.right  = -16;
    SP500->hitboxSideL.bottom = 24;

    SP500->hitboxSideR.left   = 16;
    SP500->hitboxSideR.top    = -24;
    SP500->hitboxSideR.right  = 32;
    SP500->hitboxSideR.bottom = 24;

    SP500->hitboxBottom.left   = -32;
    SP500->hitboxBottom.top    = 16;
    SP500->hitboxBottom.right  = 32;
    SP500->hitboxBottom.bottom = 24;

    SP500->printLayerID = RSDK.GetTileLayerID("Print Source");
    SP500->sfxBeep4     = RSDK.GetSfx("Stage/Beep4.wav");
    SP500->sfxFail      = RSDK.GetSfx("Stage/Fail.wav");
    SP500->sfxButton2   = RSDK.GetSfx("Stage/Button2.wav");
}

void SP500_CheckPlayerCollisions(void)
{
    RSDK_THIS(SP500);

    self->position.x += self->rowPrintPos;

    int32 offsetX = (self->position.x & 0xFFFF0000) - (self->targetPos.x & 0xFFFF0000);
    int32 offsetY = (self->position.y & 0xFFFF0000) - (self->targetPos.y & 0xFFFF0000);

    // this was a weird one where apparently it used targetPos directly which is very much undefined behavior lol
    // so much so that it breaks as of v5U :]
    Vector2 storePos = self->position;
    self->position   = self->targetPos;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (self->playerTimers[playerID]) {
            self->playerTimers[playerID]--;
        }
        else if (Player_CheckCollisionBox(player, self, &SP500->hitboxSideL) == C_TOP
                 || Player_CheckCollisionBox(player, self, &SP500->hitboxSideR) == C_TOP) {
            player->position.x += offsetX;
            player->position.y += offsetY;
        }
        else if (Player_CheckCollisionBox(player, self, &SP500->hitboxBottom) == C_TOP) {
            if (self->state == SP500_State_WaitForEntry && !player->sidekick) {
                if (!((1 << playerID) & self->activePlayers)) {
                    self->activePlayers |= (1 << playerID);
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->interaction     = false;
                    player->tileCollisions  = TILECOLLISION_NONE;
                    player->groundVel       = 0;
                    player->velocity.x      = 0;
                    player->velocity.y      = 0;
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;
                    player->state           = Player_State_Static;
                }
            }
        }
        else {
            self->activePlayers &= ~(1 << playerID);
        }
    }

    self->position = storePos;

    for (int32 p = 0; p < Player->playerCount; ++p) {
        if ((1 << p) & self->activePlayers) {
            EntityPlayer *playerPtr = RSDK_GET_ENTITY(p, Player);
            playerPtr->position.x   = self->position.x;
            playerPtr->position.y   = self->position.y;
        }
    }

    self->targetPos.x = self->position.x;
    self->targetPos.y = self->position.y;

    self->position.x -= self->rowPrintPos;
}

void SP500_State_WaitForEntry(void)
{
    RSDK_THIS(SP500);

    SP500_CheckPlayerCollisions();

    if (Zone->timer & 0x20) {
        self->showGreenLight = true;
        self->showRedLight   = true;
    }
    else {
        self->showGreenLight = false;
        self->showRedLight   = false;
    }

    if (self->activePlayers) {
        RSDK.PlaySfx(SP500->sfxButton2, false, 255);
        self->showGreenLight = false;
        self->showRedLight   = false;
        self->state          = SP500_State_Activate;
    }
}

void SP500_State_Finished(void) { SP500_CheckPlayerCollisions(); }

void SP500_State_Activate(void)
{
    RSDK_THIS(SP500);

    SP500_CheckPlayerCollisions();

    if (++self->timer >= 60) {
        self->timer = 0;

        if (Ink && Ink->playerColors[0]) {
            self->inkColor = Ink->playerColors[0] - 1;
            RSDK.PlaySfx(SP500->sfxBeep4, false, 255);

            self->showGreenLight = true;
            self->active         = ACTIVE_NORMAL;
            self->state          = SP500_State_PrintDelay;

            EntitySP500 *controller = CREATE_ENTITY(SP500, INT_TO_VOID(true), self->position.x, self->position.y);
            controller->targetPos.x = self->position.x + ((self->len - 3) << 19);
            if (self->printDir)
                controller->targetPos.y = self->position.y + (self->height << 19);
            else
                controller->targetPos.y = self->position.y - (self->height << 19);

            EntityCamera *camera = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->camera;
            camera->target       = (Entity *)controller;

            controller->storedEntity = (Entity *)RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            self->storedEntity       = (Entity *)controller;
        }
        else {
            RSDK.PlaySfx(SP500->sfxFail, false, 0xFF);
            self->showRedLight = true;
            self->state        = SP500_State_PrintFailed;
        }
    }
}

void SP500_State_PrintFailed(void)
{
    RSDK_THIS(SP500);

    SP500_CheckPlayerCollisions();

    if (++self->timer >= 30) {
        self->timer = 0;

        for (int32 p = 0; p < Player->playerCount; ++p) {
            if ((1 << p) & self->activePlayers) {
                RSDK.PlaySfx(Player->sfxRelease, false, 255);
                EntityPlayer *player     = RSDK_GET_ENTITY(p, Player);
                player->visible          = true;
                player->interaction      = true;
                player->tileCollisions   = TILECOLLISION_DOWN;
                player->applyJumpCap     = false;
                player->jumpAbilityState = 0;
                player->onGround         = false;
                player->state            = Player_State_Air;
                player->velocity.y       = self->printDir == 0 ? 0x60000 : -0x60000;
                self->activePlayers &= ~(1 << p);
                self->playerTimers[p] = 15;
            }
        }

        self->state = SP500_State_WaitForEntry;
    }
}

void SP500_State_PrintDelay(void)
{
    RSDK_THIS(SP500);

    SP500_CheckPlayerCollisions();

    if (++self->timer >= 30) {
        self->timer = 0;
        self->state = SP500_State_Printing;
    }
}

void SP500_State_Printing(void)
{
    RSDK_THIS(SP500);

    if (self->curPrintDir == self->startDir) {
        self->rowPrintPos += self->velocity.x;

        if (self->rowPrintPos >= self->offR) {
            self->state          = SP500_State_NextPrintRow;
            self->showGreenLight = true;
        }
    }
    else {
        self->rowPrintPos -= self->velocity.x;

        if (self->rowPrintPos <= self->offL) {
            self->state          = SP500_State_NextPrintRow;
            self->showGreenLight = true;
        }
    }

    uint16 tile = (uint16)-1;
    switch (self->inkColor) {
        default: break;
        case INK_C: tile = RSDK.GetTile(SP500->printLayerID, self->srcC.x + (self->rowPrintPos >> 20), self->srcC.y + self->printRowID); break;
        case INK_M: tile = RSDK.GetTile(SP500->printLayerID, self->srcM.x + (self->rowPrintPos >> 20), self->srcM.y + self->printRowID); break;
        case INK_Y: tile = RSDK.GetTile(SP500->printLayerID, self->srcY.x + (self->rowPrintPos >> 20), self->srcY.y + self->printRowID); break;
    }

    if (tile != (uint16)-1) {
        self->position.y += 0x100000;
        self->showGreenLight = (Zone->timer >> 1) & 1;
        RSDK.SetTile(Zone->fgLayer[0], (self->position.x + self->rowPrintPos) >> 20, self->position.y >> 20, tile);

        int32 posY = self->position.y;
        if (self->position.y >= 0x1800000) {
            if (self->position.y > 0x8800000) {
                posY -= 0xA000000;
                RSDK.SetTile(Zone->fgLayer[0], (self->position.x + self->rowPrintPos) >> 20, posY >> 20, tile);
            }
        }
        else {
            posY += 0xA000000;
            RSDK.SetTile(Zone->fgLayer[0], (self->position.x + self->rowPrintPos) >> 20, posY >> 20, tile);
        }
        self->velocity.x = 0x80000;
        self->position.y -= 0x100000;
    }
    else {
        self->velocity.x     = 0x100000;
        self->showGreenLight = true;
    }

    SP500_CheckPlayerCollisions();
}

void SP500_State_NextPrintRow(void)
{
    RSDK_THIS(SP500);

    if ((self->printDir || self->printRowID > 0) && (self->printDir != 1 || self->printRowID < self->height - 1)) {
        if (self->printDir)
            self->position.y += 0x40000;
        else
            self->position.y -= 0x40000;

        if (++self->timer == 4) {
            self->timer = 0;
            if (self->printDir) {
                ++self->printRowID;
                self->curPrintDir ^= 1;
                self->state = SP500_State_Printing;
            }
            else {
                --self->printRowID;
                self->curPrintDir ^= 1;
                self->state = SP500_State_Printing;
            }
        }
    }
    else {
        RSDK.PlaySfx(SP500->sfxBeep4, false, 0xFF);
        self->state = SP500_State_PrintFinished;
    }

    SP500_CheckPlayerCollisions();
}

void SP500_State_PrintFinished(void)
{
    RSDK_THIS(SP500);

    self->timer++;
    switch (self->timer) {
        case 30: RSDK.PlaySfx(SP500->sfxBeep4, false, 0xFF); break;

        case 60: {
            for (int32 i = 0; i < Player->playerCount; i += 2) {
                EntityPlayer *player = RSDK_GET_ENTITY(i, Player);

                switch (player->characterID) {
                    default: break;

                    // Bug Details:
                    // this is broken for sonic, see Ink for more explanation and how to fix
                    case ID_SONIC: RSDK.CopyPalette(6, PLAYER_PALETTE_INDEX_SONIC_OLD, 0, PLAYER_PALETTE_INDEX_SONIC_OLD, PLAYER_PRIMARY_COLOR_COUNT); break;

                    case ID_TAILS: RSDK.CopyPalette(6, PLAYER_PALETTE_INDEX_TAILS, 0, PLAYER_PALETTE_INDEX_TAILS, PLAYER_PRIMARY_COLOR_COUNT); break;

                    case ID_KNUCKLES:
                        RSDK.CopyPalette(6, PLAYER_PALETTE_INDEX_KNUX, 0, PLAYER_PALETTE_INDEX_KNUX, PLAYER_PRIMARY_COLOR_COUNT);
                        break;

                        // also see the Ink object for a concept on how this would work with mighty & ray
                }
                Ink->playerColors[i] = 0;
            }
            EntityPlayer *player1   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            player1->camera->target = (Entity *)player1;

            EntitySP500 *machine  = (EntitySP500 *)self->storedEntity;
            machine->targetPos.x  = player1->position.x;
            machine->targetPos.y  = player1->position.y;
            machine->state        = SP500_State_MoveToTarget;
            machine->storedEntity = NULL;
            self->storedEntity    = NULL;
            break;
        }

        case 90:
            for (int32 i = SLOT_PLAYER1; i < Player->playerCount; ++i) {
                if ((1 << i) & self->activePlayers) {
                    RSDK.PlaySfx(Player->sfxRelease, false, 0xFF);
                    EntityPlayer *player     = RSDK_GET_ENTITY(i, Player);
                    player->visible          = true;
                    player->interaction      = true;
                    player->tileCollisions   = TILECOLLISION_DOWN;
                    player->applyJumpCap     = false;
                    player->jumpAbilityState = 0;
                    player->onGround         = false;
                    player->state            = Player_State_Air;
                    if (self->printDir == 0)
                        player->velocity.y = -0x100000;
                    else
                        player->velocity.y = 0x100000;
                    self->activePlayers &= ~(1 << i);
                    self->playerTimers[i] = 15;
                }
            }

            self->showGreenLight = false;
            self->state          = SP500_State_Finished;
            break;
    }

    SP500_CheckPlayerCollisions();
}

void SP500_State_MoveToTarget(void)
{
    RSDK_THIS(SP500);

    uint8 finishMasks = 0;
    if (self->position.x == self->targetPos.x) {
        finishMasks = 1;
    }
    else if (self->position.x >= self->targetPos.x) {
        self->position.x -= 0x80000;
        if (self->position.x < self->targetPos.x)
            self->position.x = self->targetPos.x;
    }
    else {
        self->position.x += 0x80000;
        if (self->position.x > self->targetPos.x)
            self->position.x = self->targetPos.x;
    }

    if (self->position.y == self->targetPos.y) {
        ++finishMasks;
    }
    else if (self->position.y >= self->targetPos.y) {
        self->position.y -= 0x80000;
        if (self->position.y < self->targetPos.y)
            self->position.y = self->targetPos.y;
    }
    else {
        self->position.y += 0x80000;
        if (self->position.y > self->targetPos.y)
            self->position.y = self->targetPos.y;
    }

    if (finishMasks == 2) {
        if (self->storedEntity)
            self->state = StateMachine_None;
        else
            destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void SP500_EditorDraw(void)
{
    RSDK_THIS(SP500);

    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = (self->len + 4) << 20;
    self->updateRange.y = 0x800000;
    self->alpha         = 0xC0;
    RSDK.SetSpriteAnimation(SP500->aniFrames, 0, &self->animator, true, 0);

    self->rowPrintPos = self->startDir == FLIP_NONE ? (self->offL << 20) : (self->offR << 20);
    self->printRowID  = self->printDir == FLIP_NONE ? self->height - 1 : 0;

    SP500_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        Vector2 storePos = self->position;

        // other offset preview
        self->rowPrintPos = self->startDir != FLIP_NONE ? (self->offL << 20) : (self->offR << 20);

        self->inkEffect = INK_BLEND;
        SP500_Draw();

        // other offset + distance preview
        if (self->printDir)
            self->position.y += (self->height - 1) << 20;
        else
            self->position.y -= (self->height - 1) << 20;

        self->inkEffect = INK_BLEND;
        SP500_Draw();

        // regular offset + distance preview
        self->rowPrintPos = self->startDir == FLIP_NONE ? (self->offL << 20) : (self->offR << 20);
        self->inkEffect   = INK_BLEND;
        SP500_Draw();

        self->position  = storePos;
        self->inkEffect = INK_NONE;

        // Print Source previews
        int32 sizeX = (1 + abs(self->offR - self->offL)) << 20;

        int32 offsetX = sizeX >> 1;
        int32 offsetY = self->height << 19;

        Vector2 srcC;
        srcC.x = (self->srcC.x >> 16 << 20) + (self->offL << 20);
        srcC.y = self->srcC.y >> 16 << 20;

        Vector2 srcM;
        srcM.x = (self->srcM.x >> 16 << 20) + (self->offL << 20);
        srcM.y = self->srcM.y >> 16 << 20;

        Vector2 srcY;
        srcY.x = (self->srcY.x >> 16 << 20) + (self->offL << 20);
        srcY.y = self->srcY.y >> 16 << 20;

        DrawHelpers_DrawRectOutline(srcC.x + offsetX, srcC.y + offsetY, sizeX, self->height << 20, 0x00FFFF);
        DrawHelpers_DrawArrow(self->position.x, self->position.y, srcC.x + offsetX, srcC.y + offsetY, 0x00FFFF, INK_NONE, 0xFF);

        DrawHelpers_DrawRectOutline(srcM.x + offsetX, srcM.y + offsetY, sizeX, self->height << 20, 0xFF00FF);
        DrawHelpers_DrawArrow(self->position.x, self->position.y, srcM.x + offsetX, srcM.y + offsetY, 0xFF00FF, INK_NONE, 0xFF);

        DrawHelpers_DrawRectOutline(srcY.x + offsetX, srcY.y + offsetY, sizeX, self->height << 20, 0xFFFF00);
        DrawHelpers_DrawArrow(self->position.x, self->position.y, srcY.x + offsetX, srcY.y + offsetY, 0xFFFF00, INK_NONE, 0xFF);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void SP500_EditorLoad(void)
{
    SP500->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/SP500.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(SP500, startDir);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);

    RSDK_ACTIVE_VAR(SP500, printDir);
    RSDK_ENUM_VAR("Upwards", FLIP_NONE);
    RSDK_ENUM_VAR("Downwards", FLIP_X);
}
#endif

void SP500_Serialize(void)
{
    RSDK_EDITABLE_VAR(SP500, VAR_ENUM, len);
    RSDK_EDITABLE_VAR(SP500, VAR_ENUM, height);
    RSDK_EDITABLE_VAR(SP500, VAR_ENUM, offL);
    RSDK_EDITABLE_VAR(SP500, VAR_ENUM, offR);
    RSDK_EDITABLE_VAR(SP500, VAR_ENUM, startDir);
    RSDK_EDITABLE_VAR(SP500, VAR_ENUM, printDir);
    RSDK_EDITABLE_VAR(SP500, VAR_VECTOR2, srcC);
    RSDK_EDITABLE_VAR(SP500, VAR_VECTOR2, srcM);
    RSDK_EDITABLE_VAR(SP500, VAR_VECTOR2, srcY);
}
