// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: WaterGush Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectWaterGush *WaterGush;

void WaterGush_Update(void)
{
    RSDK_THIS(WaterGush);

    WaterGush_SetupHitboxes();

    bool32 wasActivated = self->activated;
    self->direction     = FLIP_NONE;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        if (!((1 << playerID) & self->activePlayers)) {
            if (Player_CheckCollisionTouch(player, self, &self->hitboxGush)) {
                self->active = ACTIVE_NORMAL;

                if (!player->sidekick)
                    self->activated = true;

                RSDK.PlaySfx(WaterGush->sfxGush, false, 255);
                self->activePlayers |= 1 << playerID;

                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, true, 6);
                player->nextGroundState = StateMachine_None;
                player->nextAirState    = StateMachine_None;
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                player->tileCollisions  = TILECOLLISION_NONE;
                player->onGround        = false;
                player->state           = Player_State_Static;
            }
        }

        if (((1 << playerID) & self->activePlayers)) {
            int32 xDir = 0;
            int32 yDir = 0;

            int32 offsetX = 0;
            int32 offsetY = 0;
            switch (self->orientation) {
                default: break;

                case WATERGUSH_UP:
                    yDir    = -1;
                    offsetX = self->position.x;
                    offsetY = self->position.y - self->gushPos - 0x140000;
                    break;

                case WATERGUSH_RIGHT:
                    xDir    = 1;
                    offsetX = self->gushPos + 0x140000 + self->position.x;
                    offsetY = self->position.y;
                    break;

                case WATERGUSH_LEFT:
                    xDir    = -1;
                    offsetX = self->position.x - self->gushPos - 0x140000;
                    offsetY = self->position.y;
                    break;
            }

            player->position.x += (offsetX - player->position.x) >> 2;
            player->position.y += (offsetY - player->position.y) >> 2;
            player->state = Player_State_Static;

            if ((!Player_CheckCollisionTouch(player, self, &self->hitboxRange) && !Player_CheckCollisionTouch(player, self, &self->hitboxGush))
                || self->finishedExtending) {
                self->activePlayers &= ~(1 << playerID);
                player->state          = Player_State_Air;
                player->tileCollisions = TILECOLLISION_DOWN;
                player->onGround       = false;
                player->velocity.x     = xDir * (abs(self->speed) << 15);
                player->velocity.y     = yDir * (abs(self->speed) << 15);
            }
        }
    }

    if (self->activated) {
        if (!wasActivated) {
            RSDK.PlaySfx(Water->sfxSplash, false, 255);
            WaterGush_SpawnBrickDebris();
        }

        if (self->finishedExtending) {
            if (self->orientation == WATERGUSH_UP) {
                self->gravityStrength += 0x3800;

                if (self->gushPos > 0)
                    self->gushPos -= self->gravityStrength;

                self->gushPos = MAX(self->gushPos, 0);
                if (!self->gushPos) {
                    self->activated         = false;
                    self->finishedExtending = false;
                    self->gravityStrength   = 0;
                }
            }
        }
        else {
            if (self->gushPos < self->length << 22)
                self->gushPos += (abs(self->speed) << 15);

            self->gushPos = MIN(self->length << 22, self->gushPos);
            if (self->gushPos == self->length << 22)
                self->finishedExtending = true;
        }
    }

    if (!RSDK.CheckOnScreen(self, NULL)) {
        self->gushPos           = 0;
        self->activated         = false;
        self->finishedExtending = false;
        self->gravityStrength   = 0;
        self->active            = ACTIVE_BOUNDS;
    }

    RSDK.ProcessAnimation(&self->plumeAnimator);
    RSDK.ProcessAnimation(&self->topAnimator);
}

void WaterGush_LateUpdate(void) {}

void WaterGush_StaticUpdate(void) {}

void WaterGush_Draw(void)
{
    RSDK_THIS(WaterGush);

    if (self->gushPos > 0)
        WaterGush_DrawSprites();
}

void WaterGush_Create(void *data)
{
    RSDK_THIS(WaterGush);

    self->active     = ACTIVE_BOUNDS;
    self->drawGroup  = Zone->objectDrawGroup[0];
    self->startPos.x = self->position.x;
    self->startPos.y = self->position.y;
    self->visible    = true;
    self->drawFX     = FX_FLIP;

    self->updateRange.x = 0x800000;
    if (self->orientation == WATERGUSH_LEFT || self->orientation == WATERGUSH_RIGHT)
        self->updateRange.x = (self->length + 2) << 22;

    self->updateRange.y = 0x800000;
    if (self->orientation == WATERGUSH_UP)
        self->updateRange.y = (self->length + 2) << 22;

    if (!self->speed)
        self->speed = 16;

    WaterGush_SetupHitboxes();

    switch (self->orientation) {
        default: break;

        case WATERGUSH_UP:
            self->direction = FLIP_NONE;
            RSDK.SetSpriteAnimation(WaterGush->aniFrames, 0, &self->plumeAnimator, true, 0);
            RSDK.SetSpriteAnimation(WaterGush->aniFrames, 2, &self->topAnimator, true, 0);
            break;

        case WATERGUSH_RIGHT:
        case WATERGUSH_LEFT:
            if (self->orientation == WATERGUSH_LEFT)
                self->direction = FLIP_X;
            else
                self->direction = FLIP_NONE;

            RSDK.SetSpriteAnimation(WaterGush->aniFrames, 1, &self->plumeAnimator, true, 0);
            RSDK.SetSpriteAnimation(WaterGush->aniFrames, 3, &self->topAnimator, true, 0);
            break;
    }
}

void WaterGush_StageLoad(void)
{
    WaterGush->aniFrames = RSDK.LoadSpriteAnimation("HCZ/WaterGush.bin", SCOPE_STAGE);
    WaterGush->sfxGush   = RSDK.GetSfx("HCZ/WaterGush.wav");
}

void WaterGush_SetupHitboxes(void)
{
    RSDK_THIS(WaterGush);

    switch (self->orientation) {
        default: break;

        case WATERGUSH_UP:
            self->hitboxRange.left   = -32;
            self->hitboxRange.top    = -20 - (self->gushPos >> 16);
            self->hitboxRange.right  = 32;
            self->hitboxRange.bottom = 0;

            self->hitboxGush.left   = -32;
            self->hitboxGush.top    = 0;
            self->hitboxGush.right  = 32;
            self->hitboxGush.bottom = 16;
            break;

        case WATERGUSH_RIGHT:
            self->hitboxRange.left   = 0;
            self->hitboxRange.top    = -32;
            self->hitboxRange.right  = (self->gushPos >> 16) + 20;
            self->hitboxRange.bottom = 32;

            if (Music->activeTrack == TRACK_EGGMAN1) {
                self->hitboxGush.left   = -192;
                self->hitboxGush.top    = -16;
                self->hitboxGush.right  = 0;
                self->hitboxGush.bottom = 16;
            }
            else {
                self->hitboxGush.left   = -16;
                self->hitboxGush.top    = -32;
                self->hitboxGush.right  = 0;
                self->hitboxGush.bottom = 32;
            }
            break;

        case WATERGUSH_LEFT:
            self->hitboxRange.left   = -20 - (self->gushPos >> 16);
            self->hitboxRange.top    = -32;
            self->hitboxRange.right  = 0;
            self->hitboxRange.bottom = 32;

            self->hitboxGush.left   = 0;
            self->hitboxGush.top    = -32;
            self->hitboxGush.right  = 16;
            self->hitboxGush.bottom = 32;
            break;
    }
}

void WaterGush_DrawSprites(void)
{
    RSDK_THIS(WaterGush);

    uint8 storeDir     = self->direction;
    Vector2 drawPosTop = self->position;

    int32 offsetX = 0;
    int32 offsetY = 0;
    switch (self->orientation) {
        default: break;

        case WATERGUSH_UP:
            self->direction = FLIP_NONE;
            drawPosTop.y -= self->gushPos;
            offsetX = 0;
            offsetY = 0x400000;
            break;

        case WATERGUSH_RIGHT:
            self->direction = FLIP_NONE;
            offsetX         = -0x400000;
            offsetY         = 0;
            drawPosTop.x += self->gushPos;
            break;

        case WATERGUSH_LEFT:
            self->direction = FLIP_X;
            offsetX         = 0x400000;
            offsetY         = 0;
            drawPosTop.x -= self->gushPos;
            break;
    }

    Vector2 drawPos = drawPosTop;
    if (self->gushPos > 0) {
        int32 count = ((self->gushPos - 1) >> 22) + 1;
        for (int32 i = 0; i < count; ++i) {
            drawPos.x += offsetX;
            drawPos.y += offsetY;
            RSDK.DrawSprite(&self->plumeAnimator, &drawPos, false);
        }
    }

    RSDK.DrawSprite(&self->topAnimator, &drawPosTop, false);

    self->direction = storeDir;
}

void WaterGush_SpawnBrickDebris(void)
{
    RSDK_THIS(WaterGush);

    Vector2 brickPos[4];
    Vector2 brickVel[4];
    int32 brickDir[4];

    switch (self->orientation) {
        default: break;

        case WATERGUSH_UP:
            brickPos[0].x = self->position.x - 0x200000;
            brickPos[0].y = self->position.y;
            brickVel[0].x = -0x30000;
            brickVel[0].y = -0x4000 * abs(self->speed);
            brickDir[0]   = FLIP_X;

            brickPos[1].x = self->position.x - 0x100000;
            brickPos[1].y = self->position.y;
            brickVel[1].x = -0x28000;
            brickVel[1].y = -0x8000 * abs(self->speed);
            brickDir[1]   = FLIP_X;

            brickPos[2].x = self->position.x + 0x100000;
            brickPos[2].y = self->position.x - 0x200000;
            brickVel[2].x = 0x28000;
            brickVel[2].y = -0x8000 * abs(self->speed);
            brickDir[2]   = FLIP_NONE;

            brickPos[3].x = self->position.x + 0x200000;
            brickPos[3].y = self->position.y;
            brickVel[3].x = 0x30000;
            brickVel[3].y = -0x4000 * abs(self->speed);
            brickDir[3]   = FLIP_NONE;
            break;

        case WATERGUSH_RIGHT:
            brickPos[0].x = self->position.x;
            brickPos[0].y = self->position.y - 0x200000;
            brickVel[0].x = abs(self->speed) << 14;
            brickVel[0].y = -0x30000;
            brickDir[0]   = FLIP_NONE;

            brickPos[1].x = self->position.x;
            brickPos[1].y = self->position.y - 0x100000;
            brickVel[1].x = abs(self->speed) << 15;
            brickVel[1].y = -0x28000;
            brickDir[1]   = FLIP_NONE;

            brickPos[2].x = self->position.x;
            brickPos[2].y = self->position.y + 0x100000;
            brickVel[2].x = abs(self->speed) << 15;
            brickVel[2].y = 0x28000;
            brickDir[2]   = FLIP_NONE;

            brickPos[3].x = self->position.x;
            brickPos[3].y = self->position.y + 0x200000;
            brickVel[3].x = abs(self->speed) << 14;
            brickVel[3].y = 0x30000;
            brickDir[3]   = FLIP_NONE;
            break;

        case WATERGUSH_LEFT:
            brickPos[0].x = self->position.x;
            brickPos[0].y = self->position.y - 0x200000;
            brickVel[0].x = -0x4000 * abs(self->speed);
            brickVel[0].y = -0x30000;
            brickDir[0]   = FLIP_X;

            brickPos[1].x = self->position.x;
            brickPos[1].y = self->position.y - 0x100000;
            brickVel[1].x = -0x8000 * abs(self->speed);
            brickVel[1].y = -0x28000;
            brickDir[1]   = FLIP_X;

            brickPos[2].x = self->position.x;
            brickPos[2].y = self->position.y + 0x100000;
            brickVel[2].x = -0x8000 * abs(self->speed);
            brickVel[2].y = 0x28000;
            brickDir[2]   = FLIP_X;

            brickPos[3].x = self->position.x;
            brickPos[3].y = self->position.y + 0x200000;
            brickVel[3].x = -0x4000 * abs(self->speed);
            brickVel[3].y = 0x30000;
            brickDir[3]   = FLIP_X;
            break;
    }

    for (int32 i = 0; i < 2; ++i) {
        EntityDebris *brick = CREATE_ENTITY(Debris, Debris_State_Fall, brickPos[0].x, brickPos[0].y);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 4, &brick->animator, true, 0);
        brick->position.x += RSDK.Rand(-0x80000, 0x80000);
        brick->position.y += RSDK.Rand(-0x80000, 0x80000);
        brick->velocity.x      = brickVel[0].x;
        brick->velocity.y      = brickVel[0].y >> i;
        brick->direction       = brickDir[0];
        brick->drawFX          = FX_FLIP;
        brick->drawGroup       = Zone->objectDrawGroup[1];
        brick->gravityStrength = 0x3800;

        brick = CREATE_ENTITY(Debris, Debris_State_Fall, brickPos[1].x, brickPos[1].y);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 4, &brick->animator, true, 0);
        brick->position.x += RSDK.Rand(-0x80000, 0x80000);
        brick->position.y += RSDK.Rand(-0x80000, 0x80000);
        brick->velocity.x      = brickVel[1].x;
        brick->velocity.y      = brickVel[1].y >> i;
        brick->direction       = brickDir[1];
        brick->drawFX          = FX_SCALE | FX_FLIP;
        brick->drawGroup       = Zone->objectDrawGroup[1];
        brick->gravityStrength = 0x3800;
        brick->scale.x         = 0x200;
        brick->scale.y         = 0x200;
        brick->scaleSpeed.x    = 8 >> i;
        brick->scaleSpeed.y    = 8 >> i;

        brick = CREATE_ENTITY(Debris, Debris_State_Fall, brickPos[2].x, brickPos[2].y);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 4, &brick->animator, true, 0);
        brick->position.x += RSDK.Rand(-0x80000, 0x80000);
        brick->position.y += RSDK.Rand(-0x80000, 0x80000);
        brick->velocity.x      = brickVel[2].x;
        brick->velocity.y      = brickVel[2].y >> i;
        brick->direction       = brickDir[2];
        brick->drawFX          = FX_SCALE | FX_FLIP;
        brick->drawGroup       = Zone->objectDrawGroup[1];
        brick->gravityStrength = 0x3800;
        brick->scale.x         = 512;
        brick->scale.y         = 512;
        brick->scaleSpeed.x    = 8 >> i;
        brick->scaleSpeed.y    = 8 >> i;

        brick = CREATE_ENTITY(Debris, Debris_State_Fall, brickPos[3].x, brickPos[3].y);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 4, &brick->animator, true, 0);
        brick->position.x += RSDK.Rand(-0x80000, 0x80000);
        brick->position.y += RSDK.Rand(-0x80000, 0x80000);
        brick->velocity.x      = brickVel[3].x;
        brick->velocity.y      = brickVel[3].y >> i;
        brick->direction       = brickDir[3];
        brick->drawFX          = FX_FLIP;
        brick->drawGroup       = Zone->objectDrawGroup[1];
        brick->gravityStrength = 0x3800;
    }
}

#if GAME_INCLUDE_EDITOR
void WaterGush_EditorDraw(void)
{
    RSDK_THIS(WaterGush);

    self->updateRange.x = 0x800000;
    if (self->orientation == WATERGUSH_LEFT || self->orientation == WATERGUSH_RIGHT)
        self->updateRange.x = (self->length + 2) << 22;

    self->updateRange.y = 0x800000;
    if (self->orientation == WATERGUSH_UP)
        self->updateRange.y = (self->length + 2) << 22;

    switch (self->orientation) {
        default: break;

        case WATERGUSH_UP:
            self->direction = FLIP_NONE;
            RSDK.SetSpriteAnimation(WaterGush->aniFrames, 0, &self->plumeAnimator, true, 0);
            RSDK.SetSpriteAnimation(WaterGush->aniFrames, 2, &self->topAnimator, true, 0);
            break;

        case WATERGUSH_RIGHT:
        case WATERGUSH_LEFT:
            self->direction = self->orientation == WATERGUSH_LEFT ? FLIP_X : FLIP_NONE;

            RSDK.SetSpriteAnimation(WaterGush->aniFrames, 1, &self->plumeAnimator, true, 0);
            RSDK.SetSpriteAnimation(WaterGush->aniFrames, 3, &self->topAnimator, true, 0);
            break;
    }

    self->gushPos = self->length << 22;

    WaterGush_DrawSprites();
}

void WaterGush_EditorLoad(void)
{
    WaterGush->aniFrames = RSDK.LoadSpriteAnimation("HCZ/WaterGush.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(WaterGush, orientation);
    RSDK_ENUM_VAR("Up", WATERGUSH_UP);
    RSDK_ENUM_VAR("Right", WATERGUSH_RIGHT);
    RSDK_ENUM_VAR("Left", WATERGUSH_LEFT);
}
#endif

void WaterGush_Serialize(void)
{
    RSDK_EDITABLE_VAR(WaterGush, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(WaterGush, VAR_UINT32, length);
    RSDK_EDITABLE_VAR(WaterGush, VAR_INT32, speed);
}
