// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Technosqueek Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTechnosqueek *Technosqueek;

void Technosqueek_Update(void)
{
    RSDK_THIS(Technosqueek);

    StateMachine_Run(self->state);
}

void Technosqueek_LateUpdate(void) {}

void Technosqueek_StaticUpdate(void) {}

void Technosqueek_Draw(void)
{
    RSDK_THIS(Technosqueek);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;

    if (self->type) {
        if (self->direction & FLIP_Y)
            drawPos.y = self->position.y - self->tailOffset;
        else
            drawPos.y = self->tailOffset + self->position.y;
    }
    else {
        if (self->direction & FLIP_X)
            drawPos.x = self->position.x - self->tailOffset;
        else
            drawPos.x = self->tailOffset + self->position.x;
    }

    RSDK.DrawSprite(&self->tailAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Technosqueek_Create(void *data)
{
    RSDK_THIS(Technosqueek);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->drawFX |= FX_FLIP;

    if (!SceneInfo->inEditor) {
        self->startPos      = self->position;
        self->startDir      = self->direction;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        if (!self->type) {
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 0, &self->animator, true, 0);
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 2, &self->tailAnimator, true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 3, &self->animator, true, 0);
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 5, &self->tailAnimator, true, 0);
        }

        self->state = Technosqueek_State_Init;
        self->distance <<= 1;
    }
}

void Technosqueek_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("FBZ"))
        Technosqueek->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Technosqueek.bin", SCOPE_STAGE);

    Technosqueek->hitboxBadnik.left   = -4;
    Technosqueek->hitboxBadnik.top    = -4;
    Technosqueek->hitboxBadnik.right  = 4;
    Technosqueek->hitboxBadnik.bottom = 4;

    DEBUGMODE_ADD_OBJ(Technosqueek);
}

void Technosqueek_DebugSpawn(void)
{
    RSDK_THIS(Technosqueek);

    CREATE_ENTITY(Technosqueek, NULL, self->position.x, self->position.y);
}

void Technosqueek_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Technosqueek_HandlePlayerCollisions(void)
{
    RSDK_THIS(Technosqueek);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Technosqueek->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Technosqueek_CheckOffScreen(void)
{
    RSDK_THIS(Technosqueek);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->distance >>= 1;
        self->position  = self->startPos;
        self->direction = self->startDir;
        Technosqueek_Create(NULL);
    }
}

void Technosqueek_State_Init(void)
{
    RSDK_THIS(Technosqueek);

    self->active   = ACTIVE_NORMAL;
    bool32 flipped = self->type ? (self->direction & FLIP_Y) : (self->direction & FLIP_X);

    if (!flipped) {
        self->acceleration = 0x2000;
        self->targetVel    = self->speed << 14;
        self->groundVel    = -self->targetVel;
    }
    else {
        self->groundVel    = self->speed << 14;
        self->acceleration = -0x2000;
        self->targetVel    = -self->groundVel;
    }

    self->distRemain = self->distance >> 1;
    self->shouldTurn = true;
    self->tailOffset = 0;

    if (self->type) {
        self->state = Technosqueek_State_MoveVertical;
        Technosqueek_State_MoveVertical();
    }
    else {
        self->state = Technosqueek_State_MoveHorizontal;
        Technosqueek_State_MoveHorizontal();
    }
}

void Technosqueek_State_MoveHorizontal(void)
{
    RSDK_THIS(Technosqueek);

    self->position.x += self->groundVel;
    if (self->distRemain) {
        self->distRemain--;
    }
    else {
        self->groundVel += self->acceleration;
        if (self->groundVel == self->targetVel) {
            self->distRemain   = self->distance;
            self->targetVel    = -self->targetVel;
            self->acceleration = -self->acceleration;
            self->shouldTurn   = true;
        }
    }

    RSDK.ProcessAnimation(&self->tailAnimator);
    RSDK.ProcessAnimation(&self->animator);

    if (self->shouldTurn) {
        if (abs(self->groundVel) <= 0x10000 && self->animator.animationID != 1)
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 1, &self->animator, true, 0);

        if (self->animator.frameID == 1)
            self->tailOffset = -0x80000;
        else if (self->animator.frameID == 2)
            self->tailOffset = -0x120000;
    }
    else {
        if (self->tailOffset < 0)
            self->tailOffset += abs(self->groundVel - abs(self->acceleration));
        else
            self->tailOffset = 0;
    }

    if (!self->groundVel)
        self->state = Technosqueek_State_TurnHorizontal;

    Technosqueek_HandlePlayerCollisions();
    Technosqueek_CheckOffScreen();
}

void Technosqueek_State_TurnHorizontal(void)
{
    RSDK_THIS(Technosqueek);

    RSDK.ProcessAnimation(&self->tailAnimator);
    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        self->shouldTurn = false;
        RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 0, &self->animator, true, 0);
        self->state = Technosqueek_State_MoveHorizontal;
        self->direction ^= FLIP_X;
    }

    Technosqueek_HandlePlayerCollisions();
    Technosqueek_CheckOffScreen();
}

void Technosqueek_State_MoveVertical(void)
{
    RSDK_THIS(Technosqueek);

    self->position.y += self->groundVel;
    if (self->distRemain) {
        self->distRemain--;
    }
    else {
        self->groundVel += self->acceleration;
        if (self->groundVel == self->targetVel) {
            self->distRemain   = self->distance;
            self->targetVel    = -self->targetVel;
            self->acceleration = -self->acceleration;
            self->shouldTurn   = true;
        }
    }

    RSDK.ProcessAnimation(&self->tailAnimator);
    RSDK.ProcessAnimation(&self->animator);

    if (self->shouldTurn) {
        if (abs(self->groundVel) <= 0x10000 && self->animator.animationID != 4)
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 4, &self->animator, true, 0);

        if (self->animator.frameID == 1)
            self->tailOffset = -0x80000;
        else if (self->animator.frameID == 2)
            self->tailOffset = -0x120000;
    }
    else {
        if (self->tailOffset < 0)
            self->tailOffset += abs(self->groundVel - abs(self->acceleration));
        else
            self->tailOffset = 0;
    }

    if (!self->groundVel)
        self->state = Technosqueek_State_TurnVertical;

    Technosqueek_HandlePlayerCollisions();
    Technosqueek_CheckOffScreen();
}

void Technosqueek_State_TurnVertical(void)
{
    RSDK_THIS(Technosqueek);

    RSDK.ProcessAnimation(&self->tailAnimator);
    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        self->shouldTurn = false;
        RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 3, &self->animator, true, 0);
        self->state = Technosqueek_State_MoveVertical;
        self->direction ^= FLIP_Y;
    }

    Technosqueek_HandlePlayerCollisions();
    Technosqueek_CheckOffScreen();
}

void Technosqueek_State_Fall(void)
{
    RSDK_THIS(Technosqueek);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, 8)) {
        self->startPos.x = self->position.x;
        self->startPos.y = self->position.y;
        self->startDir   = self->direction;
        self->velocity.y = 0;
        self->speed      = 16;
        self->state      = Technosqueek_State_Init;
    }

    RSDK.ProcessAnimation(&self->tailAnimator);
    Technosqueek_HandlePlayerCollisions();
}

#if GAME_INCLUDE_EDITOR
void Technosqueek_EditorDraw(void)
{
    RSDK_THIS(Technosqueek);

    if (!self->type) {
        RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 0, &self->animator, true, 0);
        RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 2, &self->tailAnimator, true, 0);
    }
    else {
        RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 3, &self->animator, true, 0);
        RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 5, &self->tailAnimator, true, 0);
    }

    self->startPos = self->position;

    Technosqueek_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);
        bool32 flipped = self->type ? (self->direction & FLIP_Y) : (self->direction & FLIP_X);

        for (int32 i = 0; i < 2; ++i) {
            if (!flipped) {
                self->acceleration = 0x2000;
                self->targetVel    = self->speed << 14;
                self->groundVel    = -self->targetVel;
            }
            else {
                self->groundVel    = self->speed << 14;
                self->acceleration = -0x2000;
                self->targetVel    = -self->groundVel;
            }
            self->distRemain = self->distance;

            while (true) {
                if (!self->type)
                    self->position.x += self->groundVel;
                else
                    self->position.y += self->groundVel;

                if (self->distRemain) {
                    self->distRemain--;
                }
                else {
                    self->groundVel += self->acceleration;
                    if (self->groundVel == self->targetVel) {
                        self->distRemain   = self->distance << 1;
                        self->targetVel    = -self->targetVel;
                        self->acceleration = -self->acceleration;
                    }
                }

                if (!self->groundVel)
                    break;
            }
            DrawHelpers_DrawArrow(self->startPos.x, self->startPos.y, self->position.x, self->position.y, 0x00FF00, INK_NONE, 0xFF);

            self->inkEffect = INK_BLEND;
            Technosqueek_Draw();
            self->inkEffect = INK_NONE;

            flipped ^= 1;
            self->position = self->startPos;
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Technosqueek_EditorLoad(void)
{
    Technosqueek->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Technosqueek.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Technosqueek, type);
    RSDK_ENUM_VAR("Horizontal", TECHNOSQUEEK_H);
    RSDK_ENUM_VAR("Vertical", TECHNOSQUEEK_V);

    RSDK_ACTIVE_VAR(Technosqueek, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
    RSDK_ENUM_VAR("Flip Y", FLIP_Y);
    RSDK_ENUM_VAR("Flip XY", FLIP_XY);
}
#endif

void Technosqueek_Serialize(void)
{
    RSDK_EDITABLE_VAR(Technosqueek, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Technosqueek, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Technosqueek, VAR_UINT8, speed);
    RSDK_EDITABLE_VAR(Technosqueek, VAR_UINT16, distance);
}
