// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: AIZKingClaw Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectAIZKingClaw *AIZKingClaw;

void AIZKingClaw_Update(void)
{
    RSDK_THIS(AIZKingClaw);
    StateMachine_Run(self->state);

    for (int32 i = 0; i < MIN(self->grabCount, 8); ++i) {
        Entity *grabbed = self->grabbedEntities[i];
        if (grabbed) {
            grabbed->position.x = self->clawPos.x;
            grabbed->position.y = self->clawPos.y + 0x2C0000;

            if (grabbed->classID == Platform->classID) {
                EntityPlatform *plat = (EntityPlatform *)grabbed;
                plat->drawPos.x      = plat->position.x;
                plat->drawPos.y      = plat->position.y;
                plat->centerPos.x    = plat->position.x;
                plat->centerPos.y    = plat->position.y;
            }
        }
    }

    AIZKingClaw_HandleClawPositions();
}

void AIZKingClaw_LateUpdate(void) {}

void AIZKingClaw_StaticUpdate(void)
{
    foreach_active(AIZKingClaw, claw) { RSDK.AddDrawListRef(Zone->objectDrawGroup[1], RSDK.GetEntitySlot(claw)); }
}

void AIZKingClaw_Draw(void)
{
    RSDK_THIS(AIZKingClaw);
    if (SceneInfo->currentDrawGroup == Zone->objectDrawGroup[0]) {
        RSDK.DrawSprite(&self->clawBackAnimator, &self->clawPos, false);
    }
    else {
        for (int32 i = 0; i < AIZKingClaw_ChainCount; ++i) RSDK.DrawSprite(&self->chainAnimator, &self->chainPos[i], false);

        RSDK.DrawSprite(&self->hingeAnimator, &self->clawPos, false);
        RSDK.DrawSprite(&self->clawFrontAnimator, &self->clawPos, false);
    }
}

void AIZKingClaw_Create(void *data)
{
    RSDK_THIS(AIZKingClaw);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawFX        = FX_ROTATE;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x1000000;
        self->startY        = self->position.y - 0x300000;
        if (RSDK.CheckSceneFolder("AIZ")) {
            self->position.y -= 0x1000000;
            RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 3, &self->clawBackAnimator, true, 0);
            RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 2, &self->clawFrontAnimator, true, 0);
        }
        else {
            self->position.y -= 0x40000;
            RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 3, &self->clawBackAnimator, true, 3);
            RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 2, &self->clawFrontAnimator, true, 3);
        }
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 0, &self->chainAnimator, true, 0);
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 1, &self->hingeAnimator, true, 0);
    }
}

void AIZKingClaw_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("AIZ")) {
        AIZKingClaw->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Claw.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("GHZCutscene")) {
        AIZKingClaw->aniFrames = RSDK.LoadSpriteAnimation("GHZCutscene/Claw.bin", SCOPE_STAGE);
    }

    AIZKingClaw->active        = ACTIVE_NORMAL;
    AIZKingClaw->sfxClack      = RSDK.GetSfx("Stage/Clack.wav");
    AIZKingClaw->sfxWalkerLegs = RSDK.GetSfx("LRZ/WalkerLegs.wav");
}

void AIZKingClaw_HandleClawPositions(void)
{
    RSDK_THIS(AIZKingClaw);

    self->clawPos.x = self->position.x;
    self->clawPos.y = self->position.y;
    int32 moveX     = RSDK.Sin256(self->angle) << 12;
    int32 moveY     = RSDK.Cos256(self->angle) << 12;

    for (int32 i = 0; i < AIZKingClaw_ChainCount; ++i) {
        self->chainPos[i].x = self->clawPos.x;
        self->chainPos[i].y = self->clawPos.y;
        self->clawPos.x += moveX;
        self->clawPos.y += moveY;
    }

    self->clawPos.x += 0xC00 * RSDK.Sin256(self->angle);
    self->clawPos.y += 0xC00 * RSDK.Cos256(self->angle);
}

void AIZKingClaw_State_Grab(void)
{
    RSDK_THIS(AIZKingClaw);
    if (!self->timer)
        RSDK.PlaySfx(AIZKingClaw->sfxClack, false, 0);

    ++self->timer;
    if (!(self->timer & 3)) {
        if (self->clawFrontAnimator.frameID >= 3) {
            self->state = StateMachine_None;
        }
        else {
            self->clawBackAnimator.frameID++;
            self->clawFrontAnimator.frameID++;
            if (self->clawFrontAnimator.frameID == 3)
                RSDK.PlaySfx(AIZKingClaw->sfxWalkerLegs, false, 0);
        }
    }
}

#if GAME_INCLUDE_EDITOR
void AIZKingClaw_EditorDraw(void)
{
    RSDK_THIS(AIZKingClaw);
    int32 y = self->position.y;
    if (RSDK.CheckSceneFolder("AIZ")) {
        self->position.y -= 0x1000000;
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 3, &self->clawBackAnimator, true, 0);
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 2, &self->clawFrontAnimator, true, 0);
    }
    else {
        self->position.y -= 0x40000;
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 3, &self->clawBackAnimator, true, 3);
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 2, &self->clawFrontAnimator, true, 3);
    }
    RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 0, &self->chainAnimator, true, 0);
    RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 1, &self->hingeAnimator, true, 0);

    AIZKingClaw_HandleClawPositions();

    for (int32 i = 0; i < AIZKingClaw_ChainCount; ++i) RSDK.DrawSprite(&self->chainAnimator, &self->chainPos[i], false);

    RSDK.DrawSprite(&self->hingeAnimator, &self->clawPos, false);
    RSDK.DrawSprite(&self->clawFrontAnimator, &self->clawPos, false);

    RSDK.DrawSprite(&self->clawBackAnimator, &self->clawPos, false);

    self->position.y = y;
}

void AIZKingClaw_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("AIZ"))
        AIZKingClaw->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Claw.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("GHZCutscene"))
        AIZKingClaw->aniFrames = RSDK.LoadSpriteAnimation("GHZCutscene/Claw.bin", SCOPE_STAGE);
}
#endif

void AIZKingClaw_Serialize(void) {}
