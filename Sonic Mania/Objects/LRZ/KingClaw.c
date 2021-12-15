// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: KingClaw Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
    foreach_active(KingClaw, claw) { RSDK.AddDrawListRef(1, RSDK.GetEntityID(claw)); }
}

void KingClaw_Draw(void)
{
    RSDK_THIS(KingClaw);
    if (self->flag) {
        RSDK.DrawSprite(&self->animator3, &self->drawPos, false);

        for (int i = 0; i < KingClaw_JointCount; ++i) {
            RSDK.DrawSprite(&self->animator1, &self->jointPos[i], false);
        }
    }
    else {
        if (SceneInfo->currentDrawGroup != Zone->drawOrderLow) {
            RSDK.DrawSprite(&self->animator3, &self->drawPos, false);
        }
        else {
            for (int i = 0; i < KingClaw_JointCount; ++i) {
                RSDK.DrawSprite(&self->animator1, &self->jointPos[i], false);
            }
        }
    }
    RSDK.DrawSprite(&self->animator2, &self->drawPos, false);
    RSDK.DrawSprite(&self->animator4, &self->drawPos, false);
}

void KingClaw_Create(void *data)
{
    RSDK_THIS(KingClaw);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawFX        = FX_ROTATE;
        self->drawOrder     = Zone->drawOrderLow;
        self->targetY       = self->position.y - 0x300000;
        self->position.y    = self->position.y - 0x1000000;
        self->active        = ACTIVE_XBOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x1000000;
        RSDK.SetSpriteAnimation(KingClaw->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(KingClaw->aniFrames, 1, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(KingClaw->aniFrames, 3, &self->animator3, true, 0);
        RSDK.SetSpriteAnimation(KingClaw->aniFrames, 2, &self->animator4, true, 0);
    }
}

void KingClaw_StageLoad(void)
{
    KingClaw->aniFrames     = RSDK.LoadSpriteAnimation("LRZ3/Claw.bin", SCOPE_STAGE);
    KingClaw->active        = ACTIVE_NORMAL;
    KingClaw->sfxClack      = RSDK.GetSfx("Stage/Clack.wav");
    KingClaw->sfxWalkerLegs = RSDK.GetSfx("LRZ/WalkerLegs.wav");
}

void KingClaw_HandleJointPositions(void)
{
    RSDK_THIS(KingClaw);

    self->drawPos.x = self->position.x;
    self->drawPos.y = self->position.y;

    for (int i = 0; i < KingClaw_JointCount; ++i) {
        self->jointPos[i].x = self->drawPos.x;
        self->jointPos[i].y = self->drawPos.y;
        self->drawPos.x += RSDK.Sin256(self->angle) << 12;
        self->drawPos.y += RSDK.Cos256(self->angle) << 12;
    }

    self->drawPos.x += 0xC00 * RSDK.Sin256(self->angle);
    self->drawPos.y += 0xC00 * RSDK.Cos256(self->angle);
}

void KingClaw_Unknown2(void)
{
    RSDK_THIS(KingClaw);

    if (self->position.y >= self->targetY)
        self->state = StateMachine_None;
    else
        self->position.y += 0x8000;
}

void KingClaw_Unknown3(void)
{
    RSDK_THIS(KingClaw);
    if (!self->timer)
        RSDK.PlaySfx(KingClaw->sfxClack, false, 0);
    ++self->timer;
    if (!(self->timer & 3)) {
        if (self->animator4.frameID < 3) {
            ++self->animator3.frameID;
            ++self->animator4.frameID;
            if (self->animator4.frameID == 3)
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
        self->state = KingClaw_Unknown4;
    }
}

void KingClaw_Unknown4(void)
{
    RSDK_THIS(KingClaw);
    self->position.y -= 0x2000;

    if (self->masterEmerald)
        self->masterEmerald->position.y = self->position.y + 0xF80000;

    if (++self->timer == 480) {
        self->timer = 0;
        self->state = 0;
    }
}

void KingClaw_Unknown5(void)
{
    RSDK_THIS(KingClaw);

    if (self->animator4.frameID > 0) {
        --self->animator3.frameID;
        --self->animator4.frameID;
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

void KingClaw_Unknown6(void)
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

void KingClaw_Unknown7(void)
{
    RSDK_THIS(KingClaw);
    if (self->velocity.y > -0x80000)
        self->velocity.y -= 0x400;
    self->position.y += self->velocity.y;
}

#if RETRO_INCLUDE_EDITOR
void KingClaw_EditorDraw(void)
{
    RSDK_THIS(KingClaw);
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x1000000;
    RSDK.SetSpriteAnimation(KingClaw->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(KingClaw->aniFrames, 1, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(KingClaw->aniFrames, 3, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(KingClaw->aniFrames, 2, &self->animator4, true, 0);
    self->flag = true;

    KingClaw_HandleJointPositions();
    KingClaw_Draw();

    if (showGizmos()) {
        self->position.y += SCREEN_YSIZE << 15;
        DrawHelpers_DrawArenaBounds(0x00C0F0, 1 | 2 | 4 | 0, -320, -(SCREEN_YSIZE >> 1), 320, SCREEN_YSIZE >> 1);
        self->position.y -= SCREEN_YSIZE << 15;
    }
}

void KingClaw_EditorLoad(void) { KingClaw->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/Claw.bin", SCOPE_STAGE); }
#endif

void KingClaw_Serialize(void) {}
