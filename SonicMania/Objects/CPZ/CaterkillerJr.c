// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CaterkillerJr Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCaterkillerJr *CaterkillerJr;

void CaterkillerJr_Update(void)
{
    RSDK_THIS(CaterkillerJr);

    self->active = ACTIVE_NORMAL;
    StateMachine_Run(self->state);

    if (self->classID == CaterkillerJr->classID && !RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position = self->startPos;
        CaterkillerJr_SetupPositions();
    }
}

void CaterkillerJr_LateUpdate(void) {}

void CaterkillerJr_StaticUpdate(void) {}

void CaterkillerJr_Draw(void)
{
    RSDK_THIS(CaterkillerJr);
    int32 storeDir = self->direction;

    for (int32 i = CATERKILLERJR_SEGMENT_COUNT - 1; i >= 0; --i) {
        self->direction = self->bodyDirection[i];
        RSDK.DrawSprite(&self->bodyAnimators[i], &self->bodyPosition[i], false);
    }

    self->direction = self->bodyDirection[3];
    RSDK.DrawSprite(&self->smokePuffAnimators[2], &self->bodyPosition[3], false);

    self->direction = self->bodyDirection[2];
    RSDK.DrawSprite(&self->smokePuffAnimators[1], &self->bodyPosition[2], false);

    self->direction = self->bodyDirection[1];
    RSDK.DrawSprite(&self->smokePuffAnimators[0], &self->bodyPosition[1], false);

    self->direction = storeDir;
}

void CaterkillerJr_Create(void *data)
{
    RSDK_THIS(CaterkillerJr);

    if (!SceneInfo->inEditor) {
        self->visible           = true;
        self->drawGroup         = Zone->objectDrawGroup[1];
        self->startPos          = self->position;
        self->drawFX            = FX_FLIP;
        self->updateRange.x     = 0x800000;
        self->updateRange.y     = 0x800000;
        self->bodyVelocity[0].x = (2 * (self->direction != FLIP_NONE) - 1) << 16;

        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 0, &self->bodyAnimators[0], true, 6);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 1, &self->bodyAnimators[1], true, 6);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 1, &self->bodyAnimators[2], true, 6);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 1, &self->bodyAnimators[3], true, 6);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 2, &self->bodyAnimators[4], true, 0);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 3, &self->bodyAnimators[5], true, 0);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 3, &self->bodyAnimators[6], true, 0);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &self->smokePuffAnimators[0], true, 0);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &self->smokePuffAnimators[1], true, 0);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &self->smokePuffAnimators[2], true, 0);

        CaterkillerJr_SetupPositions();
    }
}

void CaterkillerJr_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("CPZ"))
        CaterkillerJr->aniFrames = RSDK.LoadSpriteAnimation("CPZ/CaterkillerJr.bin", SCOPE_STAGE);
#if MANIA_USE_PLUS
    else if (RSDK.CheckSceneFolder("AIZ"))
        CaterkillerJr->aniFrames = RSDK.LoadSpriteAnimation("AIZ/CaterkillerJr.bin", SCOPE_STAGE);
#endif

    CaterkillerJr->hitbox.left   = -16;
    CaterkillerJr->hitbox.top    = 8;
    CaterkillerJr->hitbox.right  = 0;
    CaterkillerJr->hitbox.bottom = 8;

    DEBUGMODE_ADD_OBJ(CaterkillerJr);
}

void CaterkillerJr_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void CaterkillerJr_DebugSpawn(void)
{
    RSDK_THIS(CaterkillerJr);

    CREATE_ENTITY(CaterkillerJr, NULL, self->position.x, self->position.y);
}

void CaterkillerJr_SetupPositions(void)
{
    RSDK_THIS(CaterkillerJr);

    for (int32 i = 0; i < CATERKILLERJR_SEGMENT_COUNT; ++i) {
        self->bodyPosition[i].x = self->position.x;
        self->bodyPosition[i].y = self->position.y;
        self->bodyVelocity[i].x = 0;
        self->bodyVelocity[i].y = 0;
        self->bodyDirection[i]  = self->direction;
        self->bodyTimer[i]      = 0;
    }

    if (self->direction) {
        self->boundsL = self->position.x - 0x3C0000;
        self->boundsR = self->position.x + 0xBC0000;
    }
    else {
        self->boundsL = self->position.x - 0xBC0000;
        self->boundsR = self->position.x + 0x3C0000;
    }

    self->timer = 0;

    self->smokePuffAnimators[0].timer   = 14;
    self->smokePuffAnimators[1].timer   = 10;
    self->smokePuffAnimators[2].timer   = 6;
    self->smokePuffAnimators[0].frameID = 8;
    self->smokePuffAnimators[1].frameID = 8;
    self->smokePuffAnimators[2].frameID = 8;

    self->bodyVelocity[0].x = (2 * (self->direction != FLIP_NONE) - 1) << 16;
    self->active            = ACTIVE_BOUNDS;

    self->state = CaterkillerJr_State_SetupVelocities;
}

void CaterkillerJr_State_SetupVelocities(void)
{
    RSDK_THIS(CaterkillerJr);

    int32 velocity = (2 * (self->direction != FLIP_NONE) - 1) << 16;

    switch (++self->timer) {
        case 12: self->bodyVelocity[1].x = velocity; break;
        case 24: self->bodyVelocity[2].x = velocity; break;
        case 36: self->bodyVelocity[3].x = velocity; break;
        case 48: self->bodyVelocity[4].x = velocity; break;
        case 56: self->bodyVelocity[5].x = velocity; break;
        case 64:
            self->timer             = 0;
            self->bodyVelocity[6].x = velocity;
            self->state             = CaterkillerJr_State_Move;
            break;
    }

    CaterkillerJr_State_Move();
}

void CaterkillerJr_State_Move(void)
{
    RSDK_THIS(CaterkillerJr);

    for (int32 i = 0; i < CATERKILLERJR_SEGMENT_COUNT; ++i) {
        RSDK.ProcessAnimation(&self->bodyAnimators[i]);

        self->bodyPosition[i].x += self->bodyVelocity[i].x;
        if (self->bodyDirection[i] && self->bodyPosition[i].x >= self->boundsR - 0x80000) {
            self->bodyDirection[i] ^= FLIP_X;
            self->bodyAnimators[i].timer   = 0;
            self->bodyAnimators[i].frameID = 0;
        }
        else if (!self->bodyDirection[i] && self->bodyPosition[i].x <= self->boundsL + 0x80000) {
            self->bodyDirection[i] ^= FLIP_X;
            self->bodyAnimators[i].timer   = 0;
            self->bodyAnimators[i].frameID = 0;
        }

        if (self->bodyVelocity[i].x <= 0 && self->bodyPosition[i].x <= self->boundsL) {
            self->bodyVelocity[i].x = -self->bodyVelocity[i].x;
            self->bodyTimer[i]      = 0xA00;
        }
        else if (self->bodyVelocity[i].x >= 0 && self->bodyPosition[i].x >= self->boundsR) {
            self->bodyVelocity[i].x = -self->bodyVelocity[i].x;
            self->bodyTimer[i]      = 0xA00;
        }

        if (self->bodyVelocity[i].x) {
            int32 sine = 0;
            if (self->bodyTimer[i] >= 0x800) {
                self->bodyTimer[i] += 8;
                sine = RSDK.Sin1024(self->bodyTimer[i]) << 10;
            }
            else {
                self->bodyTimer[i] += 17;
                sine = RSDK.Sin1024(self->bodyTimer[i]) << 8;
            }
            self->bodyPosition[i].y = sine + self->startPos.y;
            self->bodyTimer[i] %= 0xC00;
        }
    }

    RSDK.ProcessAnimation(&self->smokePuffAnimators[0]);
    RSDK.ProcessAnimation(&self->smokePuffAnimators[1]);
    RSDK.ProcessAnimation(&self->smokePuffAnimators[2]);

    foreach_active(Player, player)
    {
        self->position = self->bodyPosition[0];
        if (Player_CheckBadnikTouch(player, self, &CaterkillerJr->hitbox) && Player_CheckBadnikBreak(player, self, false)) {
            for (int32 i = 1; i < CATERKILLERJR_SEGMENT_COUNT; ++i) {
                EntityDebris *debris    = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->bodyPosition[i].x, self->bodyPosition[i].y);
                debris->animator        = self->bodyAnimators[i];
                debris->velocity.x      = RSDK.Rand(-16, 16) << 14;
                debris->velocity.y      = RSDK.Rand(-8, 8) << 14;
                debris->gravityStrength = 0x3800;
                debris->drawGroup       = Zone->objectDrawGroup[0];
                debris->updateRange.x   = 0x400000;
                debris->updateRange.y   = 0x400000;
            }

            destroyEntity(self);
            self->active = ACTIVE_DISABLED;
            foreach_break;
        }

        if (self->classID != TYPE_BLANK) {
            for (int32 i = 1; i < CATERKILLERJR_SEGMENT_COUNT; ++i) {
                self->position.x = self->bodyPosition[i].x;
                self->position.y = self->bodyPosition[i].y;
                if (Player_CheckCollisionTouch(player, self, &CaterkillerJr->hitbox)) {
                    Player_Hurt(player, self);
                }
            }
            self->position.x = self->bodyPosition[0].x;
            self->position.y = self->bodyPosition[0].y;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void CaterkillerJr_EditorDraw(void)
{
    RSDK_THIS(CaterkillerJr);

    self->visible  = true;
    self->drawFX   = FX_FLIP;
    self->startPos = self->position;

    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 0, &self->bodyAnimators[0], true, 6);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 1, &self->bodyAnimators[1], true, 6);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 1, &self->bodyAnimators[2], true, 6);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 1, &self->bodyAnimators[3], true, 6);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 2, &self->bodyAnimators[4], true, 0);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 3, &self->bodyAnimators[5], true, 0);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 3, &self->bodyAnimators[6], true, 0);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &self->smokePuffAnimators[0], true, 0);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &self->smokePuffAnimators[1], true, 0);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &self->smokePuffAnimators[2], true, 0);
    CaterkillerJr_SetupPositions();

    CaterkillerJr_Draw();
}

void CaterkillerJr_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("CPZ"))
        CaterkillerJr->aniFrames = RSDK.LoadSpriteAnimation("CPZ/CaterkillerJr.bin", SCOPE_STAGE);
#if MANIA_USE_PLUS
    else if (RSDK.CheckSceneFolder("AIZ"))
        CaterkillerJr->aniFrames = RSDK.LoadSpriteAnimation("AIZ/CaterkillerJr.bin", SCOPE_STAGE);
#endif

    RSDK_ACTIVE_VAR(CaterkillerJr, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void CaterkillerJr_Serialize(void) { RSDK_EDITABLE_VAR(CaterkillerJr, VAR_UINT8, direction); }
