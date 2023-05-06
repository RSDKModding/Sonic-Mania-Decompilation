// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SP500MkII Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSP500MkII *SP500MkII;

void SP500MkII_Update(void)
{
    RSDK_THIS(SP500MkII);

    self->printerPosition.x = self->position.x + self->printPos.x;
    self->printerPosition.y = self->position.y + self->printPos.y;

    StateMachine_Run(self->state);

    self->showRedLight   = false;
    self->showGreenLight = false;

    if (self->isActive && self->printRowID < self->printRows)
        self->showGreenLight = true;

    if (self->buttonDown && !self->isActive && self->state != SP500MkII_State_NextPrintRow && self->printRowID < self->printRows)
        self->showRedLight = true;

    RSDK.ProcessAnimation(&self->animator);
}

void SP500MkII_LateUpdate(void) {}

void SP500MkII_StaticUpdate(void) {}

void SP500MkII_Draw(void)
{
    SP500MkII_DrawRails();
    SP500MkII_DrawPrinter();
    SP500MkII_DrawDebugOverlay();
}

void SP500MkII_Create(void *data)
{
    RSDK_THIS(SP500MkII);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->drawFX        = FX_FLIP;
    self->visible       = true;
    self->updateRange.x = (self->length + 16) << 19;
    self->updateRange.y = (self->printRows + 4) << 21;
    self->state         = SP500MkII_State_Init;
}

void SP500MkII_StageLoad(void)
{
    SP500MkII->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/SP500MkII.bin", SCOPE_STAGE);

    SP500MkII->hitboxSolid.left   = -32;
    SP500MkII->hitboxSolid.top    = -24;
    SP500MkII->hitboxSolid.right  = 32;
    SP500MkII->hitboxSolid.bottom = 23;

    SP500MkII->hitboxButton.left   = -32;
    SP500MkII->hitboxButton.top    = -38;
    SP500MkII->hitboxButton.right  = 32;
    SP500MkII->hitboxButton.bottom = -24;

    SP500MkII->hitboxPrinter.left   = -23;
    SP500MkII->hitboxPrinter.top    = 24;
    SP500MkII->hitboxPrinter.right  = 22;
    SP500MkII->hitboxPrinter.bottom = 39;
}

void SP500MkII_DrawDebugOverlay(void)
{
    RSDK_THIS(SP500MkII);

    if (SceneInfo->inEditor || DebugMode->debugActive) {
        int32 storeX = self->position.x;
        int32 storeY = self->position.y;

        self->position.x += self->printPos.x;
        self->position.y += self->printPos.y;
        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &SP500MkII->hitboxButton, self->direction, 0xFFFF00);
        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &SP500MkII->hitboxSolid, self->direction, 0xFFFFFF);
        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &SP500MkII->hitboxPrinter, self->direction, 0xFFFFFF);

        self->position.x = storeX;
        self->position.y = storeY;
    }
}

void SP500MkII_DrawPrinter(void)
{
    RSDK_THIS(SP500MkII);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position.x += self->printPos.x;
    self->position.y += self->printPos.y;

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y + self->buttonPos;
    RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 1, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.y = self->position.y;
    RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 0, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    if (self->showGreenLight) {
        self->inkEffect = INK_ADD;
        self->alpha     = 0xFF;
        RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 2, &self->animator, true, 0);
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->inkEffect = INK_NONE;
    }

    if (self->showRedLight) {
        self->inkEffect = INK_ADD;
        self->alpha     = 0xFF;
        RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 3, &self->animator, true, 0);
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->inkEffect = INK_NONE;
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void SP500MkII_DrawRails(void)
{
    RSDK_THIS(SP500MkII);

    Vector2 drawPos;
    drawPos.x = self->position.x - 0x280000;
    drawPos.y = self->position.y + self->printPos.y;
    RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 4, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 5, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x += 0x80000;
    self->animator.frameID = 1;
    for (int32 l = 0; l < self->length + 8; ++l) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += 0x80000;
    }

    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x += 0x80000;
    self->direction = FLIP_X;
    RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 4, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->direction = FLIP_NONE;
}

void SP500MkII_CheckPlayerCollisions(void)
{
    RSDK_THIS(SP500MkII);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position.x = self->printerPosition.x;
    self->position.y = self->printerPosition.y;
    int32 top        = SP500MkII->hitboxButton.top << 16;

    int32 buttonPos = 0;
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        int32 bottom   = player->position.y + (Player_GetHitbox(player)->bottom << 16);

        Player_CheckCollisionBox(player, self, &SP500MkII->hitboxPrinter);

        int32 side     = Player_CheckCollisionBox(player, self, &SP500MkII->hitboxSolid);
        bool32 touched = Player_CheckCollisionTouch(player, self, &SP500MkII->hitboxButton);

        if (side == C_TOP) {
            player->position.x += self->printMoveOffset.x;
            player->position.y += self->printMoveOffset.y;
        }

        if (touched) {
            int32 distY = MIN(bottom - (self->position.y + top), 0xD0000);

            if (buttonPos <= distY)
                buttonPos = distY;

            buttonPos = MIN(buttonPos, 0xD0000);

            if (side == C_TOP)
                self->activePlayers |= 1 << playerID;
            else
                self->activePlayers &= ~(1 << playerID);
        }
        else {
            self->activePlayers &= ~(1 << playerID);
        }
    }

    self->buttonPos  = buttonPos;
    self->buttonDown = self->activePlayers != 0;

    self->position.x = storeX;
    self->position.y = storeY;
}

void SP500MkII_State_Init(void)
{
    RSDK_THIS(SP500MkII);

    self->printPos.x = 0;
    self->printPos.y = 0;

    if (self->start == 1)
        self->printPos.x = self->length << 19;

    self->timer          = 0;
    self->activePlayers  = 0;
    self->buttonPos      = 0;
    self->buttonDown     = false;
    self->isActive       = false;
    self->xDir           = 2 * (self->start == 0) - 1;
    self->showGreenLight = false;
    self->showRedLight   = false;

    self->state = SP500MkII_State_AwaitActivation;
}

void SP500MkII_State_AwaitActivation(void)
{
    RSDK_THIS(SP500MkII);

    self->printMoveOffset.x = 0;
    self->printMoveOffset.y = 0;

    SP500MkII_CheckPlayerCollisions();

    if (self->activePlayers & 1) {
        ++self->timer;
    }
    else {
        if (self->isActive && self->printRowID < self->printRows) {
            self->state = SP500MkII_State_PrintRow;
        }
        self->timer = 0;
    }

    if (self->timer == 30)
        self->isActive = true;
}

void SP500MkII_State_PrintRow(void)
{
    RSDK_THIS(SP500MkII);

    self->printMoveOffset = self->printPos;

    if (self->xDir <= 0) {
        if (self->printPos.x > 0)
            self->printPos.x -= 0x40000;

        if (self->printPos.x < 0)
            self->printPos.x = 0;

        if (!self->printPos.x) {
            self->xDir = 1;
            ++self->printRowID;
            self->state    = SP500MkII_State_NextPrintRow;
            self->isActive = false;
        }
    }
    else {
        if (self->printPos.x < self->length << 19)
            self->printPos.x += 0x40000;

        if (self->printPos.x > self->length << 19)
            self->printPos.x = self->length << 19;

        if (self->printPos.x == self->length << 19) {
            self->xDir = -1;
            ++self->printRowID;
            self->state    = SP500MkII_State_NextPrintRow;
            self->isActive = false;
        }
    }

    self->printMoveOffset.x = self->printPos.x - self->printMoveOffset.x;
    self->printMoveOffset.y = 0;

    SP500MkII_CheckPlayerCollisions();
}

void SP500MkII_State_NextPrintRow(void)
{
    RSDK_THIS(SP500MkII);

    self->printMoveOffset.x = self->printPos.x;
    self->printMoveOffset.y = self->printPos.y;

    int32 dist = (self->printRowID * (2 * (self->yDir != 0) - 1)) << 21;

    if (self->yDir == FLIP_X) {
        if (self->printPos.y < dist)
            self->printPos.y += 0x40000;

        if (self->printPos.y > dist)
            self->printPos.y = dist;
    }
    else {
        if (self->printPos.y > dist)
            self->printPos.y -= 0x40000;

        if (self->printPos.y < dist)
            self->printPos.y = dist;
    }

    if (self->printPos.y == dist)
        self->state = SP500MkII_State_AwaitActivation;

    self->printMoveOffset.x = 0;
    self->printMoveOffset.y = self->printPos.y - self->printMoveOffset.y;

    SP500MkII_CheckPlayerCollisions();
}

#if GAME_INCLUDE_EDITOR
void SP500MkII_EditorDraw(void)
{
    RSDK_THIS(SP500MkII);

    self->updateRange.x = (self->length + 16) << 19;
    self->updateRange.y = (self->printRows + 4) << 21;

    self->inkEffect = INK_NONE;
    SP500MkII_State_Init();

    SP500MkII_DrawRails();
    SP500MkII_DrawPrinter();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        SP500MkII_DrawDebugOverlay();

        if (self->printRows) {
            self->printPos.y = (self->printRows * (2 * (self->yDir != 0) - 1)) << 21;

            self->inkEffect = INK_BLEND;
            SP500MkII_DrawRails();

            self->inkEffect = INK_BLEND;
            SP500MkII_DrawPrinter();

            DrawHelpers_DrawArrow(self->position.x + self->printPos.x, self->position.y, self->position.x + self->printPos.x,
                                  self->position.y + self->printPos.y, 0x00FF00, INK_NONE, 0xFF);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void SP500MkII_EditorLoad(void)
{
    SP500MkII->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/SP500MkII.bin", SCOPE_STAGE);

    SP500MkII->hitboxSolid.left   = -32;
    SP500MkII->hitboxSolid.top    = -24;
    SP500MkII->hitboxSolid.right  = 32;
    SP500MkII->hitboxSolid.bottom = 23;

    SP500MkII->hitboxButton.left   = -32;
    SP500MkII->hitboxButton.top    = -38;
    SP500MkII->hitboxButton.right  = 32;
    SP500MkII->hitboxButton.bottom = -24;

    SP500MkII->hitboxPrinter.left   = -23;
    SP500MkII->hitboxPrinter.top    = 24;
    SP500MkII->hitboxPrinter.right  = 22;
    SP500MkII->hitboxPrinter.bottom = 39;

    RSDK_ACTIVE_VAR(SP500MkII, start);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);

    RSDK_ACTIVE_VAR(SP500MkII, yDir);
    RSDK_ENUM_VAR("Upwards", FLIP_NONE);
    RSDK_ENUM_VAR("Downwards", FLIP_X);
}
#endif

void SP500MkII_Serialize(void)
{
    RSDK_EDITABLE_VAR(SP500MkII, VAR_UINT8, start);
    RSDK_EDITABLE_VAR(SP500MkII, VAR_UINT8, yDir);
    RSDK_EDITABLE_VAR(SP500MkII, VAR_UINT32, printRows);
    RSDK_EDITABLE_VAR(SP500MkII, VAR_UINT32, length);
}
