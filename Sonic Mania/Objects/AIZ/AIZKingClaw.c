#include "SonicMania.h"

ObjectAIZKingClaw *AIZKingClaw;

void AIZKingClaw_Update(void)
{
    RSDK_THIS(AIZKingClaw);
    StateMachine_Run(self->state);

    for (int32 i = 0; i < (self->grabCount <= 8 ? self->grabCount : 8); ++i) {
        Entity *grabbed = self->grabbedEntities[i];
        if (grabbed) {
            grabbed->position.x = self->clawPos.x;
            grabbed->position.y = self->clawPos.y + 0x2C0000;

            if (grabbed->objectID == Platform->objectID) {
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
    foreach_active(AIZKingClaw, claw) { RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(claw)); }
}

void AIZKingClaw_Draw(void)
{
    RSDK_THIS(AIZKingClaw);
    if (SceneInfo->currentDrawGroup == Zone->drawOrderLow) {
        RSDK.DrawSprite(&self->animator3, &self->clawPos, false);
    }
    else {
        for (int32 i = 0; i < 12; ++i) {
            RSDK.DrawSprite(&self->animator1, &self->ballPos[i], false);
        }
        RSDK.DrawSprite(&self->animator2, &self->clawPos, false);
        RSDK.DrawSprite(&self->animator4, &self->clawPos, false);
    }
}

void AIZKingClaw_Create(void *data)
{
    RSDK_THIS(AIZKingClaw);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawFX        = FX_ROTATE;
        self->drawOrder     = Zone->drawOrderLow;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x1000000;
        self->startY        = self->position.y - 0x300000;
        if (RSDK.CheckStageFolder("AIZ")) {
            self->position.y -= 0x1000000;
            RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 3, &self->animator3, true, 0);
            RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 2, &self->animator4, true, 0);
        }
        else {
            self->position.y -= 0x40000;
            RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 3, &self->animator3, true, 3);
            RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 2, &self->animator4, true, 3);
        }
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 1, &self->animator2, true, 0);
    }
}

void AIZKingClaw_StageLoad(void)
{
    if (RSDK.CheckStageFolder("AIZ")) {
        AIZKingClaw->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Claw.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("GHZCutscene")) {
        AIZKingClaw->aniFrames = RSDK.LoadSpriteAnimation("GHZCutscene/Claw.bin", SCOPE_STAGE);
    }

    AIZKingClaw->active        = ACTIVE_NORMAL;
    AIZKingClaw->sfxClack      = RSDK.GetSFX("Stage/Clack.wav");
    AIZKingClaw->sfxWalkerLegs = RSDK.GetSFX("LRZ/WalkerLegs.wav");
}

void AIZKingClaw_HandleClawPositions(void)
{
    RSDK_THIS(AIZKingClaw);

    self->clawPos.x = self->position.x;
    self->clawPos.y = self->position.y;
    int32 moveX       = RSDK.Sin256(self->angle) << 12;
    int32 moveY       = RSDK.Cos256(self->angle) << 12;

    for (int32 i = 0; i < 12; ++i) {
        self->ballPos[i].x = self->clawPos.x;
        self->ballPos[i].y = self->clawPos.y;
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
        if (self->animator4.frameID >= 3) {
            self->state = StateMachine_None;
        }
        else {
            self->animator3.frameID++;
            self->animator4.frameID++;
            if (self->animator4.frameID == 3)
                RSDK.PlaySfx(AIZKingClaw->sfxWalkerLegs, false, 0);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void AIZKingClaw_EditorDraw(void)
{
    RSDK_THIS(AIZKingClaw);
    int y = self->position.y;
    if (RSDK.CheckStageFolder("AIZ")) {
        self->position.y -= 0x1000000;
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 3, &self->animator3, true, 0);
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 2, &self->animator4, true, 0);
    }
    else {
        self->position.y -= 0x40000;
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 3, &self->animator3, true, 3);
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 2, &self->animator4, true, 3);
    }
    RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 1, &self->animator2, true, 0);

    AIZKingClaw_HandleClawPositions();
    for (int32 i = 0; i < 12; ++i) {
        RSDK.DrawSprite(&self->animator1, &self->ballPos[i], false);
    }
    RSDK.DrawSprite(&self->animator2, &self->clawPos, false);
    RSDK.DrawSprite(&self->animator4, &self->clawPos, false);

    RSDK.DrawSprite(&self->animator3, &self->clawPos, false);

    self->position.y = y;
}

void AIZKingClaw_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("AIZ"))
        AIZKingClaw->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Claw.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("GHZCutscene"))
        AIZKingClaw->aniFrames = RSDK.LoadSpriteAnimation("GHZCutscene/Claw.bin", SCOPE_STAGE);
}
#endif

void AIZKingClaw_Serialize(void) {}
