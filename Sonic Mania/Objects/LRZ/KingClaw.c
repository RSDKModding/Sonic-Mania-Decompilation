#include "SonicMania.h"

ObjectKingClaw *KingClaw;

void KingClaw_Update(void)
{
    RSDK_THIS(KingClaw);
    StateMachine_Run(entity->state);
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
    if (entity->flag) {
        RSDK.DrawSprite(&entity->animator3, &entity->drawPos, false);

        for (int i = 0; i < KingClaw_JointCount; ++i) {
            RSDK.DrawSprite(&entity->animator1, &entity->jointPos[i], false);
        }
    }
    else {
        if (RSDK_sceneInfo->currentDrawGroup != Zone->drawOrderLow) {
            RSDK.DrawSprite(&entity->animator3, &entity->drawPos, false);
        }
        else {
            for (int i = 0; i < KingClaw_JointCount; ++i) {
                RSDK.DrawSprite(&entity->animator1, &entity->jointPos[i], false);
            }
        }
    }
    RSDK.DrawSprite(&entity->animator2, &entity->drawPos, false);
    RSDK.DrawSprite(&entity->animator4, &entity->drawPos, false);
}

void KingClaw_Create(void *data)
{
    RSDK_THIS(KingClaw);

    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawFX        = FX_ROTATE;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->targetY       = entity->position.y - 0x300000;
        entity->position.y    = entity->position.y - 0x1000000;
        entity->active        = ACTIVE_XBOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x1000000;
        RSDK.SetSpriteAnimation(KingClaw->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(KingClaw->aniFrames, 1, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(KingClaw->aniFrames, 3, &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(KingClaw->aniFrames, 2, &entity->animator4, true, 0);
    }
}

void KingClaw_StageLoad(void)
{
    KingClaw->aniFrames     = RSDK.LoadSpriteAnimation("LRZ3/Claw.bin", SCOPE_STAGE);
    KingClaw->active        = ACTIVE_NORMAL;
    KingClaw->sfxClack      = RSDK.GetSFX("Stage/Clack.wav");
    KingClaw->sfxWalkerLegs = RSDK.GetSFX("LRZ/WalkerLegs.wav");
}

void KingClaw_HandleJointPositions(void)
{
    RSDK_THIS(KingClaw);

    entity->drawPos.x = entity->position.x;
    entity->drawPos.y = entity->position.y;

    for (int i = 0; i < KingClaw_JointCount; ++i) {
        entity->jointPos[i].x = entity->drawPos.x;
        entity->jointPos[i].y = entity->drawPos.y;
        entity->drawPos.x += RSDK.Sin256(entity->angle) << 12;
        entity->drawPos.y += RSDK.Cos256(entity->angle) << 12;
    }

    entity->drawPos.x += 0xC00 * RSDK.Sin256(entity->angle);
    entity->drawPos.y += 0xC00 * RSDK.Cos256(entity->angle);
}

void KingClaw_Unknown2(void)
{
    RSDK_THIS(KingClaw);

    if (entity->position.y >= entity->targetY)
        entity->state = StateMachine_None;
    else
        entity->position.y += 0x8000;
}

void KingClaw_Unknown3(void)
{
    RSDK_THIS(KingClaw);
    if (!entity->timer)
        RSDK.PlaySfx(KingClaw->sfxClack, false, 0);
    ++entity->timer;
    if (!(entity->timer & 3)) {
        if (entity->animator4.frameID < 3) {
            ++entity->animator3.frameID;
            ++entity->animator4.frameID;
            if (entity->animator4.frameID == 3)
                RSDK.PlaySfx(KingClaw->sfxWalkerLegs, false, 0);
        }
    }
    if (entity->timer == 60) {
        foreach_active(HPZEmerald, emerald)
        {
            if (emerald->type == HPZEMERALD_MASTER)
                entity->masterEmerald = emerald;
        }

        entity->timer = 0;
        entity->state = KingClaw_Unknown4;
    }
}

void KingClaw_Unknown4(void)
{
    RSDK_THIS(KingClaw);
    entity->position.y -= 0x2000;

    if (entity->masterEmerald)
        entity->masterEmerald->position.y = entity->position.y + 0xF80000;

    if (++entity->timer == 480) {
        entity->timer = 0;
        entity->state = 0;
    }
}

void KingClaw_Unknown5(void)
{
    RSDK_THIS(KingClaw);

    if (entity->animator4.frameID > 0) {
        --entity->animator3.frameID;
        --entity->animator4.frameID;
    }

    if (entity->velocity.y > -0x8000)
        entity->velocity.y -= 0x100;
    entity->position.y += entity->velocity.y;
    entity->angle    = RSDK.Sin256(-entity->timer) >> 4;
    entity->rotation = -2 * entity->angle;
    entity->timer += 3;
    if (entity->timer >= 0x800) {
        entity->state = StateMachine_None;
        entity->angle = 0;
        entity->timer = 0;
    }
}

void KingClaw_Unknown6(void)
{
    RSDK_THIS(KingClaw);

    if (entity->velocity.y < 0x40000)
        entity->velocity.y += 0x200;

    if (entity->position.y >= entity->targetY) {
        entity->velocity.y >>= 3;
        entity->state = StateMachine_None;
    }
    else {
        entity->position.y += entity->velocity.y;
    }
}

void KingClaw_Unknown7(void)
{
    RSDK_THIS(KingClaw);
    if (entity->velocity.y > -0x80000)
        entity->velocity.y -= 0x400;
    entity->position.y += entity->velocity.y;
}

#if RETRO_INCLUDE_EDITOR
void KingClaw_EditorDraw(void)
{
    RSDK_THIS(KingClaw);
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x1000000;
    RSDK.SetSpriteAnimation(KingClaw->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(KingClaw->aniFrames, 1, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(KingClaw->aniFrames, 3, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(KingClaw->aniFrames, 2, &entity->animator4, true, 0);
    entity->flag = true;

    KingClaw_HandleJointPositions();
    KingClaw_Draw();

    entity->position.y += SCREEN_YSIZE << 15;
    DrawHelpers_DrawArenaBounds(0x00C0F0, 1 | 2 | 4 | 0, -320, -(SCREEN_YSIZE >> 1), 320, SCREEN_YSIZE >> 1);
    entity->position.y -= SCREEN_YSIZE << 15;
}

void KingClaw_EditorLoad(void) { KingClaw->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/Claw.bin", SCOPE_STAGE); }
#endif

void KingClaw_Serialize(void) {}
