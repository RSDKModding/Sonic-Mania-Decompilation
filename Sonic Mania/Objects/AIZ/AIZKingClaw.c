#include "SonicMania.h"

ObjectAIZKingClaw *AIZKingClaw;

void AIZKingClaw_Update(void)
{
    RSDK_THIS(AIZKingClaw);
    StateMachine_Run(entity->state);

    for (int32 i = 0; i < (entity->grabCount <= 8 ? entity->grabCount : 8); ++i) {
        Entity *grabbed = entity->grabbedEntities[i];
        if (grabbed) {
            grabbed->position.x = entity->clawPos.x;
            grabbed->position.y = entity->clawPos.y + 0x2C0000;

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
    if (RSDK_sceneInfo->currentDrawGroup == Zone->drawOrderLow) {
        RSDK.DrawSprite(&entity->animator3, &entity->clawPos, false);
    }
    else {
        for (int32 i = 0; i < 12; ++i) {
            RSDK.DrawSprite(&entity->animator1, &entity->ballPos[i], false);
        }
        RSDK.DrawSprite(&entity->animator2, &entity->clawPos, false);
        RSDK.DrawSprite(&entity->animator4, &entity->clawPos, false);
    }
}

void AIZKingClaw_Create(void *data)
{
    RSDK_THIS(AIZKingClaw);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawFX        = FX_ROTATE;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x1000000;
        entity->startY        = entity->position.y - 0x300000;
        if (RSDK.CheckStageFolder("AIZ")) {
            entity->position.y -= 0x1000000;
            RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 3, &entity->animator3, true, 0);
            RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 2, &entity->animator4, true, 0);
        }
        else {
            entity->position.y -= 0x40000;
            RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 3, &entity->animator3, true, 3);
            RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 2, &entity->animator4, true, 3);
        }
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 1, &entity->animator2, true, 0);
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

    entity->clawPos.x = entity->position.x;
    entity->clawPos.y = entity->position.y;
    int32 moveX       = RSDK.Sin256(entity->angle) << 12;
    int32 moveY       = RSDK.Cos256(entity->angle) << 12;

    for (int32 i = 0; i < 12; ++i) {
        entity->ballPos[i].x = entity->clawPos.x;
        entity->ballPos[i].y = entity->clawPos.y;
        entity->clawPos.x += moveX;
        entity->clawPos.y += moveY;
    }

    entity->clawPos.x += 0xC00 * RSDK.Sin256(entity->angle);
    entity->clawPos.y += 0xC00 * RSDK.Cos256(entity->angle);
}

void AIZKingClaw_State_Grab(void)
{
    RSDK_THIS(AIZKingClaw);
    if (!entity->timer)
        RSDK.PlaySfx(AIZKingClaw->sfxClack, false, 0);

    ++entity->timer;
    if (!(entity->timer & 3)) {
        if (entity->animator4.frameID >= 3) {
            entity->state = StateMachine_None;
        }
        else {
            entity->animator3.frameID++;
            entity->animator4.frameID++;
            if (entity->animator4.frameID == 3)
                RSDK.PlaySfx(AIZKingClaw->sfxWalkerLegs, false, 0);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void AIZKingClaw_EditorDraw(void)
{
    RSDK_THIS(AIZKingClaw);
    int y = entity->position.y;
    if (RSDK.CheckStageFolder("AIZ")) {
        entity->position.y -= 0x1000000;
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 3, &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 2, &entity->animator4, true, 0);
    }
    else {
        entity->position.y -= 0x40000;
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 3, &entity->animator3, true, 3);
        RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 2, &entity->animator4, true, 3);
    }
    RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(AIZKingClaw->aniFrames, 1, &entity->animator2, true, 0);

    AIZKingClaw_HandleClawPositions();
    for (int32 i = 0; i < 12; ++i) {
        RSDK.DrawSprite(&entity->animator1, &entity->ballPos[i], false);
    }
    RSDK.DrawSprite(&entity->animator2, &entity->clawPos, false);
    RSDK.DrawSprite(&entity->animator4, &entity->clawPos, false);

    RSDK.DrawSprite(&entity->animator3, &entity->clawPos, false);

    entity->position.y = y;
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
