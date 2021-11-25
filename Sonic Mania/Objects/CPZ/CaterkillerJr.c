#include "SonicMania.h"

ObjectCaterkillerJr *CaterkillerJr;

void CaterkillerJr_Update(void)
{
    RSDK_THIS(CaterkillerJr);
    self->active = ACTIVE_NORMAL;
    StateMachine_Run(self->state);
    if (self->objectID == CaterkillerJr->objectID && !RSDK.CheckOnScreen(self, NULL)
        && !RSDK.CheckPosOnScreen(&self->headStartPos, &self->updateRange)) {
        self->position = self->headStartPos;
        CaterkillerJr_SetupPositions();
    }
}

void CaterkillerJr_LateUpdate(void) {}

void CaterkillerJr_StaticUpdate(void) {}

void CaterkillerJr_Draw(void)
{
    RSDK_THIS(CaterkillerJr);
    int32 storeDir = self->direction;

    for (int32 i = 6; i >= 0; --i) {
        self->direction = self->bodyDirection[i];
        RSDK.DrawSprite(&self->bodyAnimators[i], &self->bodyPosition[i], false);
    }

    self->direction = self->bodyDirection[3];
    RSDK.DrawSprite(&self->animator2, &self->bodyPosition[3], false);

    self->direction = self->bodyDirection[2];
    RSDK.DrawSprite(&self->animator1, &self->bodyPosition[2], false);

    self->direction = self->bodyDirection[1];
    RSDK.DrawSprite(&self->bodyAnimators[7], &self->bodyPosition[1], false);

    self->direction = storeDir;
}

void CaterkillerJr_Create(void *data)
{
    RSDK_THIS(CaterkillerJr);
    if (!SceneInfo->inEditor) {
        self->visible           = true;
        self->drawOrder         = Zone->drawOrderHigh;
        self->headStartPos.x    = self->position.x;
        self->headStartPos.y    = self->position.y;
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
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &self->bodyAnimators[7], true, 0);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &self->animator2, true, 0);
        CaterkillerJr_SetupPositions();
    }
}

void CaterkillerJr_StageLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ"))
        CaterkillerJr->aniFrames = RSDK.LoadSpriteAnimation("CPZ/CaterkillerJr.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("AIZ"))
        CaterkillerJr->aniFrames = RSDK.LoadSpriteAnimation("AIZ/CaterkillerJr.bin", SCOPE_STAGE);

    CaterkillerJr->hitbox.left   = -16;
    CaterkillerJr->hitbox.top    = 8;
    CaterkillerJr->hitbox.right  = 0;
    CaterkillerJr->hitbox.bottom = 8;

    DEBUGMODE_ADD_OBJ(CaterkillerJr);
}

void CaterkillerJr_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void CaterkillerJr_DebugSpawn(void)
{
    RSDK_THIS(CaterkillerJr);
    CREATE_ENTITY(CaterkillerJr, NULL, self->position.x, self->position.y);
}

void CaterkillerJr_SetupPositions(void)
{
    RSDK_THIS(CaterkillerJr);
    for (int32 i = 0; i < 7; ++i) {
        self->bodyPosition[i].x = self->position.x;
        self->bodyPosition[i].y = self->position.y;
        self->bodyVelocity[i].x = 0;
        self->bodyVelocity[i].y = 0;
        self->bodyDirection[i]  = self->direction;
        self->bodyTimer[i]      = 0;
    }

    if (self->direction) {
        self->boundL = self->position.x - 0x3C0000;
        self->boundR = self->position.x + 0xBC0000;
    }
    else {
        self->boundL = self->position.x - 0xBC0000;
        self->boundR = self->position.x + 0x3C0000;
    }

    self->bodyAnimators[7].timer = 14;
    self->animator1.timer        = 10;
    self->animator2.timer        = 6;
    self->timer                           = 0;
    self->bodyAnimators[7].frameID        = 8;
    self->animator1.frameID               = 8;
    self->animator2.frameID               = 8;
    self->bodyVelocity[0].x               = (2 * (self->direction != FLIP_NONE) - 1) << 16;
    self->active                          = ACTIVE_BOUNDS;
    self->state                           = CaterkillerJr_State1;
}

void CaterkillerJr_State1(void)
{
    RSDK_THIS(CaterkillerJr);

    int32 pos = 2 * (self->direction != FLIP_NONE) - 1;
    switch (++self->timer) {
        case 12: self->bodyVelocity[1].x = pos << 16; break;
        case 24: self->bodyVelocity[2].x = pos << 16; break;
        case 36: self->bodyVelocity[3].x = pos << 16; break;
        case 48: self->bodyVelocity[4].x = pos << 16; break;
        case 56: self->bodyVelocity[5].x = pos << 16; break;
        case 64:
            self->timer             = 0;
            self->bodyVelocity[6].x = pos << 16;
            self->state             = CaterkillerJr_State2;
            break;
    }
    CaterkillerJr_State2();
}

void CaterkillerJr_State2(void)
{
    RSDK_THIS(CaterkillerJr);
    for (int32 i = 0; i < 7; ++i) {
        RSDK.ProcessAnimation(&self->bodyAnimators[i]);
        self->bodyPosition[i].x += self->bodyVelocity[i].x;
        if (self->bodyDirection[i] && self->bodyPosition[i].x >= self->boundR - 0x80000) {
            self->bodyDirection[i] ^= 1;
            self->bodyAnimators[i].timer = 0;
            self->bodyAnimators[i].frameID        = 0;
        }
        else if (!self->bodyDirection[i] && self->bodyPosition[i].x <= self->boundL + 0x80000) {
            self->bodyDirection[i] ^= 1;
            self->bodyAnimators[i].timer = 0;
            self->bodyAnimators[i].frameID        = 0;
        }

        if (self->bodyVelocity[i].x <= 0 && self->bodyPosition[i].x <= self->boundL) {
            self->bodyVelocity[i].x = -self->bodyVelocity[i].x;
            self->bodyTimer[i]      = 0xA00;
        }
        else if (self->bodyVelocity[i].x >= 0 && self->bodyPosition[i].x >= self->boundR) {
            self->bodyVelocity[i].x = -self->bodyVelocity[i].x;
            self->bodyTimer[i]      = 0xA00;
        }

        if (self->bodyVelocity[i].x) {
            int32 val = 0;
            if (self->bodyTimer[i] >= 2048) {
                self->bodyTimer[i] += 8;
                val = RSDK.Sin1024(self->bodyTimer[i]) << 10;
            }
            else {
                self->bodyTimer[i] += 17;
                val = RSDK.Sin1024(self->bodyTimer[i]) << 8;
            }
            self->bodyPosition[i].y = val + self->headStartPos.y;
            self->bodyTimer[i] %= 3072;
        }
    }

    RSDK.ProcessAnimation(&self->bodyAnimators[7]);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);

    foreach_active(Player, player)
    {
        self->position = self->bodyPosition[0];
        if (Player_CheckBadnikTouch(player, self, &CaterkillerJr->hitbox) && Player_CheckBadnikBreak(self, player, false)) {
            for (int32 i = 0; i < 6; ++i) {
                EntityDebris *debris =
                    CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->bodyPosition[i + 1].x, self->bodyPosition[i + 1].y);
                debris->animator      = self->bodyAnimators[i + 1];
                debris->velocity.x    = RSDK.Rand(-16, 16) << 14;
                debris->velocity.y    = RSDK.Rand(-8, 8) << 14;
                debris->gravity       = 0x3800;
                debris->drawOrder     = Zone->drawOrderLow;
                debris->updateRange.x = 0x400000;
                debris->updateRange.y = 0x400000;
            }

            destroyEntity(self);
            self->active = -1;
            foreach_break;
        }

        if (self->objectID != TYPE_BLANK) {
            for (int32 i = 0; i < 6; ++i) {
                self->position.x = self->bodyPosition[i + 1].x;
                self->position.y = self->bodyPosition[i + 1].y;
                if (Player_CheckCollisionTouch(player, self, &CaterkillerJr->hitbox)) {
                    Player_CheckHit(player, self);
                }
            }
            self->position.x = self->bodyPosition[0].x;
            self->position.y = self->bodyPosition[0].y;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void CaterkillerJr_EditorDraw(void)
{
    RSDK_THIS(CaterkillerJr);
    self->visible           = true;
    self->drawFX            = FX_FLIP;

    self->headStartPos.x = self->position.x;
    self->headStartPos.y = self->position.y;
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 0, &self->bodyAnimators[0], true, 6);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 1, &self->bodyAnimators[1], true, 6);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 1, &self->bodyAnimators[2], true, 6);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 1, &self->bodyAnimators[3], true, 6);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 2, &self->bodyAnimators[4], true, 0);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 3, &self->bodyAnimators[5], true, 0);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 3, &self->bodyAnimators[6], true, 0);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &self->bodyAnimators[7], true, 0);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &self->animator2, true, 0);
    CaterkillerJr_SetupPositions();

    CaterkillerJr_Draw();
}

void CaterkillerJr_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ"))
        CaterkillerJr->aniFrames = RSDK.LoadSpriteAnimation("CPZ/CaterkillerJr.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("AIZ"))
        CaterkillerJr->aniFrames = RSDK.LoadSpriteAnimation("AIZ/CaterkillerJr.bin", SCOPE_STAGE);
}
#endif

void CaterkillerJr_Serialize(void) { RSDK_EDITABLE_VAR(CaterkillerJr, VAR_UINT8, direction); }
