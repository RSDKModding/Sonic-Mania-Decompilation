// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: KingClaw Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectKingClaw *KingClaw;

void KingClaw_Update(void)
{
    RSDK_THIS(KingClaw);

    StateMachine_Run(self->state);

    KingClaw_HandleJointPositions();
}

void KingClaw_LateUpdate(void) {}

void KingClaw_StaticUpdate(void)
{
    foreach_active(KingClaw, claw) { RSDK.AddDrawListRef(1, RSDK.GetEntitySlot(claw)); }
}

void KingClaw_Draw(void)
{
    RSDK_THIS(KingClaw);

    if (self->forceHighdrawGroup) {
        RSDK.DrawSprite(&self->clawBackAnimator, &self->drawPos, false);

        for (int32 i = 0; i < KINGCLAW_CHAIN_COUNT; ++i) RSDK.DrawSprite(&self->chainAnimator, &self->chainPos[i], false);
    }
    else {
        if (SceneInfo->currentDrawGroup != Zone->objectDrawGroup[0])
            RSDK.DrawSprite(&self->clawBackAnimator, &self->drawPos, false);
        else
            for (int32 i = 0; i < KINGCLAW_CHAIN_COUNT; ++i) RSDK.DrawSprite(&self->chainAnimator, &self->chainPos[i], false);
    }

    RSDK.DrawSprite(&self->hingeAnimator, &self->drawPos, false);
    RSDK.DrawSprite(&self->clawFrontAnimator, &self->drawPos, false);
}

void KingClaw_Create(void *data)
{
    RSDK_THIS(KingClaw);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawFX        = FX_ROTATE;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->targetY       = self->position.y - 0x300000;
        self->position.y    = self->position.y - 0x1000000;
        self->active        = ACTIVE_XBOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x1000000;

        RSDK.SetSpriteAnimation(KingClaw->aniFrames, 0, &self->chainAnimator, true, 0);
        RSDK.SetSpriteAnimation(KingClaw->aniFrames, 1, &self->hingeAnimator, true, 0);
        RSDK.SetSpriteAnimation(KingClaw->aniFrames, 3, &self->clawBackAnimator, true, 0);
        RSDK.SetSpriteAnimation(KingClaw->aniFrames, 2, &self->clawFrontAnimator, true, 0);
    }
}

void KingClaw_StageLoad(void)
{
    KingClaw->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/Claw.bin", SCOPE_STAGE);

    KingClaw->active = ACTIVE_NORMAL;

    KingClaw->sfxClack      = RSDK.GetSfx("Stage/Clack.wav");
    KingClaw->sfxWalkerLegs = RSDK.GetSfx("LRZ/WalkerLegs.wav");
}

void KingClaw_HandleJointPositions(void)
{
    RSDK_THIS(KingClaw);

    self->drawPos.x = self->position.x;
    self->drawPos.y = self->position.y;

    for (int32 i = 0; i < KINGCLAW_CHAIN_COUNT; ++i) {
        self->chainPos[i].x = self->drawPos.x;
        self->chainPos[i].y = self->drawPos.y;
        self->drawPos.x += RSDK.Sin256(self->angle) << 12;
        self->drawPos.y += RSDK.Cos256(self->angle) << 12;
    }

    self->drawPos.x += 0xC00 * RSDK.Sin256(self->angle);
    self->drawPos.y += 0xC00 * RSDK.Cos256(self->angle);
}

void KingClaw_State_EnterClaw(void)
{
    RSDK_THIS(KingClaw);

    if (self->position.y >= self->targetY)
        self->state = StateMachine_None;
    else
        self->position.y += 0x8000;
}

void KingClaw_State_Grab(void)
{
    RSDK_THIS(KingClaw);

    if (!self->timer)
        RSDK.PlaySfx(KingClaw->sfxClack, false, 0);

    ++self->timer;
    if (!(self->timer & 3)) {
        if (self->clawFrontAnimator.frameID < 3) {
            ++self->clawBackAnimator.frameID;
            ++self->clawFrontAnimator.frameID;

            if (self->clawFrontAnimator.frameID == 3)
                RSDK.PlaySfx(KingClaw->sfxWalkerLegs, false, 0);
        }
    }

    if (self->timer == 60) {
        foreach_active(HPZEmerald, emerald)
        {
            if (emerald->type == HPZEMERALD_MASTER)
                self->masterEmerald = emerald;
        }

        self->timer = 0;
        self->state = KingClaw_State_LiftMasterEmerald;
    }
}

void KingClaw_State_LiftMasterEmerald(void)
{
    RSDK_THIS(KingClaw);

    self->position.y -= 0x2000;

    if (self->masterEmerald)
        self->masterEmerald->position.y = self->position.y + 0xF80000;

    if (++self->timer == 480) {
        self->timer = 0;
        self->state = StateMachine_None;
    }
}

void KingClaw_State_Swinging(void)
{
    RSDK_THIS(KingClaw);

    if (self->clawFrontAnimator.frameID > 0) {
        --self->clawBackAnimator.frameID;
        --self->clawFrontAnimator.frameID;
    }

    if (self->velocity.y > -0x8000)
        self->velocity.y -= 0x100;

    self->position.y += self->velocity.y;
    self->angle    = RSDK.Sin256(-self->timer) >> 4;
    self->rotation = -2 * self->angle;

    self->timer += 3;
    if (self->timer >= 0x800) {
        self->state = StateMachine_None;
        self->angle = 0;
        self->timer = 0;
    }
}

void KingClaw_State_LowerClaw(void)
{
    RSDK_THIS(KingClaw);

    if (self->velocity.y < 0x40000)
        self->velocity.y += 0x200;

    if (self->position.y >= self->targetY) {
        self->velocity.y >>= 3;
        self->state = StateMachine_None;
    }
    else {
        self->position.y += self->velocity.y;
    }
}

void KingClaw_State_RaiseClaw(void)
{
    RSDK_THIS(KingClaw);

    if (self->velocity.y > -0x80000)
        self->velocity.y -= 0x400;

    self->position.y += self->velocity.y;
}

#if GAME_INCLUDE_EDITOR
void KingClaw_EditorDraw(void)
{
    RSDK_THIS(KingClaw);

    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x1000000;

    RSDK.SetSpriteAnimation(KingClaw->aniFrames, 0, &self->chainAnimator, true, 0);
    RSDK.SetSpriteAnimation(KingClaw->aniFrames, 1, &self->hingeAnimator, true, 0);
    RSDK.SetSpriteAnimation(KingClaw->aniFrames, 3, &self->clawBackAnimator, true, 0);
    RSDK.SetSpriteAnimation(KingClaw->aniFrames, 2, &self->clawFrontAnimator, true, 0);

    self->forceHighdrawGroup = true;

    KingClaw_HandleJointPositions();
    KingClaw_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        self->position.y += SCREEN_YSIZE << 15;
        DrawHelpers_DrawArenaBounds(-320, -(SCREEN_YSIZE >> 1), 320, SCREEN_YSIZE >> 1, 1 | 2 | 4 | 0, 0x00C0F0);
        self->position.y -= SCREEN_YSIZE << 15;

        RSDK_DRAWING_OVERLAY(false);
    }
}

void KingClaw_EditorLoad(void) { KingClaw->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/Claw.bin", SCOPE_STAGE); }
#endif

void KingClaw_Serialize(void) {}
