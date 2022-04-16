// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SP500 Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
    drawPos.x                = self->position.x - 0x180000;
    drawPos.y                = self->position.y;
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Rails
    self->animator.frameID = 3;
    int32 pos                = drawPos.x;
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
    drawPos.x                = self->position.x + self->offset;
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
            self->drawOrder     = Zone->drawOrderHigh;
            self->updateRange.x = (self->len + 4) << 20;
            self->updateRange.y = 0x800000;

            if (self->startDir == 0)
                self->offset = self->offL;
            else
                self->offset = self->offR;

            if (self->printDir == 0)
                self->printRowID = self->height - 1;
            self->srcC.x = self->srcC.x >> 16;
            self->srcC.y = self->srcC.y >> 16;
            self->srcM.x = self->srcM.x >> 16;
            self->srcM.y = self->srcM.y >> 16;
            self->srcY.x = self->srcY.x >> 16;
            self->srcY.y = self->srcY.y >> 16;
            self->alpha  = 0xC0;
            RSDK.SetSpriteAnimation(SP500->aniFrames, 0, &self->animator, true, 0);
            self->state = SP500_State_WaitForEntry;
        }
    }
}

void SP500_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        SP500->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/SP500.bin", SCOPE_STAGE);

    SP500->hitbox1.left   = -32;
    SP500->hitbox1.top    = -24;
    SP500->hitbox1.right  = -16;
    SP500->hitbox1.bottom = 24;

    SP500->hitboxBody.left   = 16;
    SP500->hitboxBody.top    = -24;
    SP500->hitboxBody.right  = 32;
    SP500->hitboxBody.bottom = 24;

    SP500->hitboxEntry.left   = -32;
    SP500->hitboxEntry.top    = 16;
    SP500->hitboxEntry.right  = 32;
    SP500->hitboxEntry.bottom = 24;

    SP500->printLayerID   = RSDK.GetSceneLayerID("Print Source");
    SP500->sfxBeep4       = RSDK.GetSfx("Stage/Beep4.wav");
    SP500->sfxFail        = RSDK.GetSfx("Stage/Fail.wav");
    SP500->sfxButton2     = RSDK.GetSfx("Stage/Button2.wav");
}

void SP500_CheckPlayerCollisions(void)
{
    RSDK_THIS(SP500);
    self->position.x += self->offset;
    int32 offsetX = (self->position.x & 0xFFFF0000) - (self->targetPos.x & 0xFFFF0000);
    int32 offsetY = (self->position.y & 0xFFFF0000) - (self->targetPos.y & 0xFFFF0000);

    // weird but ok sure
    Entity *offset = (Entity *)&self->targetPos;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if (self->playerTimers[playerID]) {
            self->playerTimers[playerID]--;
        }
        else if (Player_CheckCollisionBox(player, offset, &SP500->hitbox1) == C_TOP
                 || Player_CheckCollisionBox(player, offset, &SP500->hitboxBody) == C_TOP) {
            player->position.x += offsetX;
            player->position.y += offsetY;
        }
        else if (Player_CheckCollisionBox(player, offset, &SP500->hitboxEntry) == C_TOP) {
            if (self->state == SP500_State_WaitForEntry && !player->sidekick) {
                if (!((1 << playerID) & self->activePlayers)) {
                    self->activePlayers |= (1 << playerID);
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->interaction     = false;
                    player->tileCollisions  = false;
                    player->groundVel       = 0;
                    player->velocity.x      = 0;
                    player->velocity.y      = 0;
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;
                    player->state           = Player_State_None;
                }
            }
        }
        else {
            self->activePlayers &= ~(1 << playerID);
        }
    }

    for (int32 i = 0; i < Player->playerCount; ++i) {
        if ((1 << i) & self->activePlayers) {
            EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
            player->position.x   = self->position.x;
            player->position.y   = self->position.y;
        }
    }
    offset->position.x = self->position.x;
    offset->position.y = self->position.y;
    self->position.x -= self->offset;
}

void SP500_State_WaitForEntry(void)
{
    RSDK_THIS(SP500);

    SP500_CheckPlayerCollisions();

    if (Zone->timer & 0x20) {
        self->showGreenLight = true;
        self->showRedLight    = true;
    }
    else {
        self->showGreenLight = false;
        self->showRedLight    = false;
    }

    if (self->activePlayers) {
        RSDK.PlaySfx(SP500->sfxButton2, false, 255);
        self->showGreenLight = false;
        self->showRedLight    = false;
        self->state       = SP500_State_Activate;
    }
}

void SP500_State_Finished(void) { SP500_CheckPlayerCollisions(); }

void SP500_State_Activate(void)
{
    RSDK_THIS(SP500);

    SP500_CheckPlayerCollisions();

    if (++self->timer >= 60) {
        self->timer = 0;
        if (Ink && Ink->playerColours[0]) {
            self->inkColour = Ink->playerColours[0] - 1;
            RSDK.PlaySfx(SP500->sfxBeep4, false, 255);
            self->showGreenLight = true;
            self->active      = ACTIVE_NORMAL;
            self->state       = SP500_State_PrintDelay;
            EntitySP500 *child  = CREATE_ENTITY(SP500, intToVoid(true), self->position.x, self->position.y);
            child->targetPos.x      = self->position.x + ((self->len - 3) << 19);
            if (self->printDir)
                child->targetPos.y = self->position.y + (self->height << 19);
            else
                child->targetPos.y = self->position.y - (self->height << 19);
            EntityCamera *camera = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->camera;
            camera->target    = (Entity *)child;
            child->storedEntity  = (Entity *)RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            self->storedEntity = (Entity *)child;
        }
        else {
            RSDK.PlaySfx(SP500->sfxFail, false, 0xFF);
            self->showRedLight = true;
            self->state    = SP500_State_PrintFailed;
        }
    }
}

void SP500_State_PrintFailed(void)
{
    RSDK_THIS(SP500);

    SP500_CheckPlayerCollisions();

    if (++self->timer >= 30) {
        self->timer = 0;
        for (int32 i = 0; i < Player->playerCount; ++i) {
            if ((1 << i) & self->activePlayers) {
                RSDK.PlaySfx(Player->sfxRelease, false, 255);
                EntityPlayer *player     = RSDK_GET_ENTITY(i, Player);
                player->visible          = true;
                player->interaction      = true;
                player->tileCollisions   = true;
                player->applyJumpCap     = false;
                player->jumpAbilityState = 0;
                player->onGround         = false;
                player->state            = Player_State_Air;
                if (self->printDir == 0)
                    player->velocity.y = 0x60000;
                else
                    player->velocity.y = -0x60000;
                self->activePlayers &= ~(1 << i);
                self->playerTimers[i] = 15;
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
        self->offset += self->velocity.x;
        if (self->offset >= self->offR) {
            self->state       = SP500_State_NextPrintRow;
            self->showGreenLight = true;
        }
    }
    else {
        self->offset -= self->velocity.x;
        if (self->offset <= self->offL) {
            self->state       = SP500_State_NextPrintRow;
            self->showGreenLight = true;
        }
    }

    uint16 tile = (uint16)-1;

    switch (self->inkColour) {
        default: break;
        case 0: tile = RSDK.GetTileInfo(SP500->printLayerID, self->srcC.x + (self->offset >> 20), self->srcC.y + self->printRowID); break;
        case 1: tile = RSDK.GetTileInfo(SP500->printLayerID, self->srcM.x + (self->offset >> 20), self->srcM.y + self->printRowID); break;
        case 2: tile = RSDK.GetTileInfo(SP500->printLayerID, self->srcY.x + (self->offset >> 20), self->srcY.y + self->printRowID); break;
    }

    if (tile != (uint16)-1) {
        self->position.y += 0x100000;
        self->showGreenLight = (Zone->timer >> 1) & 1;
        RSDK.SetTileInfo(Zone->fgLow, (self->position.x + self->offset) >> 20, self->position.y >> 20, tile);

        int32 posY = self->position.y;
        if (self->position.y >= 0x1800000) {
            if (self->position.y > 0x8800000) {
                posY -= 0xA000000;
                RSDK.SetTileInfo(Zone->fgLow, (self->position.x + self->offset) >> 20, posY >> 20, tile);
            }
        }
        else {
            posY += 0xA000000;
            RSDK.SetTileInfo(Zone->fgLow, (self->position.x + self->offset) >> 20, posY >> 20, tile);
        }
        self->position.y -= 0x100000;
        self->velocity.x = 0x80000;
        SP500_CheckPlayerCollisions();
    }
    else {
        self->velocity.x  = 0x100000;
        self->showGreenLight = true;
        SP500_CheckPlayerCollisions();
    }
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
                    case ID_SONIC: RSDK.CopyPalette(6, 2, 0, 2, 6); break;
                    case ID_TAILS: RSDK.CopyPalette(6, 70, 0, 70, 6); break;
                    case ID_KNUCKLES: RSDK.CopyPalette(6, 80, 0, 80, 6); break;
                    // also see the Ink object for an concept on how this would work with mighty & ray
                }
                Ink->playerColours[i] = 0;
            }
            EntityPlayer *player1      = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            player1->camera->target = (Entity *)player1;
            EntitySP500 *machine       = (EntitySP500 *)self->storedEntity;
            machine->targetPos.x       = player1->position.x;
            machine->targetPos.y       = player1->position.y;
            machine->state             = SP500_State_MoveToTarget;
            machine->storedEntity      = NULL;
            self->storedEntity         = NULL;
            break;
        }
        case 90:
            for (int32 i = SLOT_PLAYER1; i < Player->playerCount; ++i) {
                if ((1 << i) & self->activePlayers) {
                    RSDK.PlaySfx(Player->sfxRelease, false, 0xFF);
                    EntityPlayer *player     = RSDK_GET_ENTITY(i, Player);
                    player->visible          = true;
                    player->interaction      = true;
                    player->tileCollisions   = true;
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
            self->state       = SP500_State_Finished;
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

#if RETRO_INCLUDE_EDITOR
void SP500_EditorDraw(void)
{
    RSDK_THIS(SP500);
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = (self->len + 4) << 20;
    self->updateRange.y = 0x800000;
    self->alpha         = 0xC0;
    RSDK.SetSpriteAnimation(SP500->aniFrames, 0, &self->animator, true, 0);

    SP500_Draw();

    if (showGizmos()) {
        self->active = ACTIVE_NORMAL;

        DrawHelpers_DrawRectOutline(self->srcC.x, self->srcC.y, self->len << 16, self->height << 16, 0x00FFFF);
        DrawHelpers_DrawArrow(self->position.x, self->position.y, self->srcC.x, self->srcC.y, 0x00FFFF);

        DrawHelpers_DrawRectOutline(self->srcM.x, self->srcM.y, self->len << 16, self->height << 16, 0x00FF00);
        DrawHelpers_DrawArrow(self->position.x, self->position.y, self->srcM.x, self->srcM.y, 0x00FF00);

        DrawHelpers_DrawRectOutline(self->srcY.x, self->srcY.y, self->len << 16, self->height << 16, 0xFFFF00);
        DrawHelpers_DrawArrow(self->position.x, self->position.y, self->srcY.x, self->srcY.y, 0xFFFF00);
    }
}

void SP500_EditorLoad(void) { SP500->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/SP500.bin", SCOPE_STAGE); }
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
