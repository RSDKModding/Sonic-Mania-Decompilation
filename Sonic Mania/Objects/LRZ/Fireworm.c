#include "SonicMania.h"

ObjectFireworm *Fireworm;

void Fireworm_Update(void)
{
    RSDK_THIS(Fireworm);
    StateMachine_Run(entity->state);
}

void Fireworm_LateUpdate(void) {}

void Fireworm_StaticUpdate(void) {}

void Fireworm_Draw(void)
{
    RSDK_THIS(Fireworm);

    entity->drawFX &= ~FX_FLIP;
    Fireworm->animator.frameID = 0;
    RSDK.DrawSprite(&Fireworm->animator, &entity->startPos, false);

    entity->drawFX |= FX_FLIP;
    if (entity->state) {
        for (int i = Fireworm_SegmentCount - 1; i > 0; --i) {
            entity->direction = entity->directions[i];
            RSDK.DrawSprite(&entity->animators1[i], &entity->positions[i], false);
            RSDK.DrawSprite(&entity->animators2[i], &entity->positions[i], false);
        }

        entity->direction = entity->directions[0];
        RSDK.DrawSprite(&entity->animators1[0], NULL, false);
    }
}

void Fireworm_Create(void *data)
{
    RSDK_THIS(Fireworm);
    entity->drawFX |= FX_FLIP;
    RSDK.SetSpriteAnimation(Fireworm->aniFrames, 0, entity->animators1, true, 0);

    if (!SceneInfo->inEditor) {
        for (int i = 0; i < Fireworm_SegmentCount; ++i) {
            entity->positions[i].x = entity->position.x;
            entity->positions[i].y = entity->position.y;
            entity->positionsY[i]  = entity->position.y;
        }

        entity->startPos.x    = entity->position.x;
        entity->startPos.y    = entity->position.y;
        entity->startDir      = entity->direction;
        entity->visible       = true;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->state         = Fireworm_State_Setup;
    }
}

void Fireworm_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Fireworm->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Fireworm.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        Fireworm->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Fireworm.bin", SCOPE_STAGE);

    RSDK.SetSpriteAnimation(Fireworm->aniFrames, 6, &Fireworm->animator, true, 0);

    Fireworm->hitbox1.left   = -8;
    Fireworm->hitbox1.top    = -8;
    Fireworm->hitbox1.right  = 8;
    Fireworm->hitbox1.bottom = 8;
    Fireworm->hitbox2.left   = -128;
    Fireworm->hitbox2.top    = -128;
    Fireworm->hitbox2.right  = 128;
    Fireworm->hitbox2.bottom = 128;

    DEBUGMODE_ADD_OBJ(Fireworm);
}

void Fireworm_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Fireworm, NULL, entity->position.x, entity->position.y);
}

void Fireworm_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Fireworm->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Fireworm_CheckPlayerCollisions(void)
{
    RSDK_THIS(Fireworm);

    foreach_active(Player, player)
    {
        entity->position.x = entity->positions[0].x;
        entity->position.y = entity->positions[0].y;
        if (Player_CheckBadnikTouch(player, entity, &Fireworm->hitbox1) && Player_CheckBadnikBreak(entity, player, false)) {
            for (int i = 1; i < Fireworm_SegmentCount; ++i) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->positions[i].x, entity->positions[i].y);
                RSDK.SetSpriteAnimation(Fireworm->aniFrames, 3, &debris->animator, true, 0);
                debris->velocity.x    = RSDK.Rand(-0x40000, 0x40000);
                debris->velocity.y    = RSDK.Rand(-0x40000, 0x40000);
                debris->gravity       = 0x3800;
                debris->drawOrder     = Zone->drawOrderLow;
                debris->updateRange.x = 0x400000;
                debris->updateRange.y = 0x400000;
            }
            entity->state = StateMachine_None;
            foreach_break;
        }
        else {
            for (int i = 1; i < Fireworm_SegmentCount; ++i) {
                entity->position.x = entity->positions[i].x;
                entity->position.y = entity->positions[i].y;

                if (Player_CheckCollisionTouch(player, entity, &Fireworm->hitbox1)) {
#if RETRO_USE_PLUS
                    if (!Player_CheckMightyUnspin(0x300, player, 2, &player->uncurlTimer))
#endif
                        Player_CheckHit(player, entity);
                }
            }
        }
    }

    if (entity->state) {
        entity->position.x = entity->positions[0].x;
        entity->position.y = entity->positions[0].y;
    }
}

void Fireworm_CheckOnScreen(void)
{
    RSDK_THIS(Fireworm);

    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->timer      = 0;
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        for (int i = 0; i < Fireworm_SegmentCount; ++i) {
            entity->velocities[i].x = 0;
            entity->angles[i]       = 0;
            entity->timers[i]       = 0;
            if (i > 0) {
                RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animators1[i], true, 0);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animators2[i], true, 0);
            }
        }
        Fireworm_Create(NULL);
    }
}

void Fireworm_State_Setup(void)
{
    RSDK_THIS(Fireworm);

    entity->active = ACTIVE_NORMAL;
    entity->state  = Fireworm_State_Unknown1;
    Fireworm_State_Unknown1();
}

void Fireworm_State_Unknown1(void)
{
    RSDK_THIS(Fireworm);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Fireworm->hitbox2)) {
            for (int i = 0; i < Fireworm_SegmentCount; ++i) {
                entity->directions[i] = player->position.x >= entity->position.x;
            }

            entity->startDir = entity->directions[0];
            if (entity->startDir) {
                entity->velocities[0].x = 0x10000;
                entity->field_EC        = entity->position.x - 0x3C0000;
                entity->field_F0        = entity->position.x + 0xBC0000;
            }
            else {
                entity->velocities[0].x = -0x10000;
                entity->field_EC        = entity->position.x - 0xBC0000;
                entity->field_F0        = entity->position.x + 0x3C0000;
            }
            entity->state = Fireworm_State_Unknown2;
            foreach_break;
        }
    }

    if (entity->state == Fireworm_State1_Unknown1)
        Fireworm_CheckOnScreen();
}

void Fireworm_State_Unknown2(void)
{
    RSDK_THIS(Fireworm);

    RSDK.ProcessAnimation(&entity->animators1[0]);
    if (entity->animators1[0].frameID == 3) {
        for (int i = 1; i < Fireworm_SegmentCount; ++i) {
            RSDK.SetSpriteAnimation(Fireworm->aniFrames, 3, &entity->animators1[i], true, 0);
            entity->timers2[i] = RSDK.Rand(0, 60);
        }
        entity->state = Fireworm_State1_Unknown1;
    }
}

void Fireworm_State1_Unknown1(void)
{
    RSDK_THIS(Fireworm);

    ++entity->timer;
    for (int i = 0; i < Fireworm_SegmentCount; ++i) {
        if (entity->timer == (i * 10) + 4) {
            if (!entity->directions[i])
                entity->velocities[i].x = -0x10000;
            else
                entity->velocities[i].x = 0x10000;

            if (i == Fireworm_SegmentCount - 1) {
                entity->timer = 0;
                entity->state = Fireworm_State1_Unknown2;
            }
        }
    }

    Fireworm_State1_Unknown2();
}

void Fireworm_State1_Unknown2(void)
{
    RSDK_THIS(Fireworm);

    for (int i = 0; i < Fireworm_SegmentCount; ++i) {
        if (!entity->velocities[i].x)
            continue;

        RSDK.ProcessAnimation(&entity->animators1[i]);
        RSDK.ProcessAnimation(&entity->animators2[i]);

        entity->positions[i].x += entity->velocities[i].x;

        if (entity->velocities[i].x <= 0) {
            if (entity->positions[i].x > entity->field_EC) {
                entity->velocities[i].x -= 0xC00;
                if (entity->velocities[i].x < -0x10000)
                    entity->velocities[i].x = -0x10000;
            }
            else {
                entity->velocities[i].x += 0xC00;
                if (entity->velocities[i].x > 0x10000)
                    entity->velocities[i].x = 0x10000;

                if (!entity->timers[i]) {
                    if (i)
                        RSDK.SetSpriteAnimation(Fireworm->aniFrames, 4, &entity->animators1[i], true, 0);
                    else
                        RSDK.SetSpriteAnimation(Fireworm->aniFrames, 2, &entity->animators1[i], true, 0);
                    entity->directions[i] = FLIP_X;
                    if (!entity->startDir)
                        entity->velocities[i].y = -0x10000;
                    else
                        entity->velocities[i].y = 0x10000;
                    entity->timers[i] = 32;
                }
            }
        }
        else {
            if (entity->positions[i].x < entity->field_F0) {
                entity->velocities[i].x += 0xC00;
                if (entity->velocities[i].x > 0x10000)
                    entity->velocities[i].x = 0x10000;
            }
            else {
                entity->velocities[i].x -= 0xC00;
                if (entity->velocities[i].x < -0x10000)
                    entity->velocities[i].x = -0x10000;

                if (!entity->timers[i]) {
                    if (i)
                        RSDK.SetSpriteAnimation(Fireworm->aniFrames, 4, &entity->animators1[i], true, 0);
                    else
                        RSDK.SetSpriteAnimation(Fireworm->aniFrames, 2, &entity->animators1[i], true, 0);
                    entity->directions[i] = FLIP_NONE;
                    if (!entity->startDir)
                        entity->velocities[i].y = 0x10000;
                    else
                        entity->velocities[i].y = -0x10000;
                    entity->timers[i] = 32;
                }
            }
        }

        if (entity->timers[i]) {
            entity->timers[i]--;
            entity->positionsY[i] += entity->velocities[i].y;
        }
        if (!--entity->timers2[i]) {
            RSDK.SetSpriteAnimation(Fireworm->aniFrames, 5, &entity->animators2[i], true, 0);
            entity->timers2[i] = RSDK.Rand(0, 60) + 32;
        }
        entity->angles[i] += 16;
        entity->positions[i].y = (RSDK.Sin1024(entity->angles[i]) << 8) + entity->positionsY[i];
        entity->angles[i] %= 0xC00;
    }
    Fireworm_CheckPlayerCollisions();
    Fireworm_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void Fireworm_EditorDraw(void)
{
    RSDK_THIS(Fireworm);

    for (int i = 0; i < Fireworm_SegmentCount; ++i) {
        entity->positions[i].x = entity->position.x + (entity->direction ? 0x100000 : -0x100000) * i;
        entity->positions[i].y = entity->position.y;
        entity->positionsY[i]  = entity->position.y;
    }

    entity->startPos.x = entity->position.x;
    entity->startPos.y = entity->position.y;
    entity->startDir   = entity->direction;

    for (int i = 1; i < Fireworm_SegmentCount; ++i) {
        RSDK.SetSpriteAnimation(Fireworm->aniFrames, 3, &entity->animators1[i], true, 0);
    }

    Fireworm_Draw();
}

void Fireworm_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Fireworm->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Fireworm.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        Fireworm->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Fireworm.bin", SCOPE_STAGE);

    RSDK.SetSpriteAnimation(Fireworm->aniFrames, 6, &Fireworm->animator, true, 0);

    RSDK_ACTIVE_VAR(Fireworm, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void Fireworm_Serialize(void) { RSDK_EDITABLE_VAR(Fireworm, VAR_UINT8, direction); }
