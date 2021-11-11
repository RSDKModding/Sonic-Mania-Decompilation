#include "SonicMania.h"

ObjectBuckwildBall *BuckwildBall;

void BuckwildBall_Update(void)
{
    RSDK_THIS(BuckwildBall);
    StateMachine_Run(entity->state);
    if (entity->state != BuckwildBall_State_Unknown2 && entity->state != BuckwildBall_State_Unknown5 && entity->state != BuckwildBall_State_Setup) {
        if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x1C0000, 2)) {
            if (entity->particleDelay-- <= 0) {
                BuckwildBall_SpawnDebris();
                BuckwildBall_HandleTimerSfx();
                entity->particleDelay = RSDK.Rand(8, 15);
            }
        }
        BuckwildBall_CheckPlayerCollisions();
        BuckwildBall_HandleRollCrush();
        RSDK.ProcessAnimation(&entity->animator);

        if (entity->timerSfx > 0)
            entity->timerSfx--;
    }
}

void BuckwildBall_LateUpdate(void) {}

void BuckwildBall_StaticUpdate(void) {}

void BuckwildBall_Draw(void)
{
    RSDK_THIS(BuckwildBall);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void BuckwildBall_Create(void *data)
{
    RSDK_THIS(BuckwildBall);

    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->startPos.x    = entity->position.x;
        entity->startPos.y    = entity->position.y;
        entity->visible       = true;
        entity->drawFX        = FX_FLIP;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->field_B8      = -1;
        if (entity->mode != 1)
            entity->updateRange.x = (entity->amplitude + 128) << 16;
        entity->state = BuckwildBall_State_Setup;
        if (!entity->speed)
            entity->speed = 2;
    }
}

void BuckwildBall_StageLoad(void)
{
    BuckwildBall->aniFrames      = RSDK.LoadSpriteAnimation("LRZ1/BuckwildBall.bin", SCOPE_STAGE);
    BuckwildBall->particleFrames = RSDK.LoadSpriteAnimation("LRZ1/Particles.bin", SCOPE_STAGE);
    BuckwildBall->sfxImpact      = RSDK.GetSFX("Stage/Impact2.wav");
    BuckwildBall->sfxSharp       = RSDK.GetSFX("Stage/Sharp.wav");
    BuckwildBall->hitbox.left    = -28;
    BuckwildBall->hitbox.top     = -28;
    BuckwildBall->hitbox.right   = 28;
    BuckwildBall->hitbox.bottom  = 28;
}

void BuckwildBall_HandleTimerSfx(void)
{
    RSDK_THIS(BuckwildBall);

    if (entity->timerSfx > 0) {
        LogHelpers_Print("timerSfx = %d", entity->timerSfx);
    }
    else {
        RSDK.PlaySfx(BuckwildBall->sfxImpact, false, 255);
        entity->timerSfx = 8;
    }
}

void BuckwildBall_CheckOnScreen(void)
{
    RSDK_THIS(BuckwildBall);

    if (!RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange) && !RSDK.CheckPosOnScreen(&entity->position, &entity->updateRange)) {
        if (entity->respawn) {
            entity->position.x = entity->startPos.x;
            entity->position.y = entity->startPos.y;
            entity->state      = BuckwildBall_State_Setup;
            entity->active     = ACTIVE_BOUNDS;
            entity->visible    = false;
        }
        else {
            destroyEntity(entity);
        }
    }
}

void BuckwildBall_SpawnDebris(void)
{
    RSDK_THIS(BuckwildBall);

    int x = entity->position.x;
    int y = entity->position.y + 0x1C0000;

    int sizeX  = (BuckwildBall->hitbox.right - BuckwildBall->hitbox.left) >> 1;
    int spawnX = x + ((RSDK.Rand(0, 64) - 32) << 16);
    int spawnY = ((RSDK.Rand(0, 8) - 4) << 15) + y;

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, spawnX, spawnY);
    RSDK.SetSpriteAnimation(BuckwildBall->particleFrames, 1, &debris->animator, true, 0);
    debris->drawOrder  = Zone->drawOrderHigh;
    debris->gravity    = 0x3800;
    debris->velocity.x = 0x180 * (abs(spawnX - x) >> 8) / sizeX;
    if (debris->position.x < entity->position.x) {
        debris->direction  = FLIP_X;
        debris->velocity.x = -debris->velocity.x;
    }
    debris->velocity.y = -0x1000 * RSDK.Rand(32, 54);
}

void BuckwildBall_CheckPlayerCollisions(void)
{
    RSDK_THIS(BuckwildBall);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &BuckwildBall->hitbox)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x600, player, false, &player->uncurlTimer))
#endif
                Player_CheckHit(player, entity);
        }
    }
}

void BuckwildBall_HandleRollCrush(void)
{
    RSDK_THIS(BuckwildBall);

    Hitbox crushHitbox;
    crushHitbox.left   = -8;
    crushHitbox.top    = -8;
    crushHitbox.right  = 8;
    crushHitbox.bottom = 8;

    foreach_active(Iwamodoki, iwamodoki)
    {
        if (RSDK.CheckObjectCollisionTouchBox(entity, &BuckwildBall->hitbox, iwamodoki, &crushHitbox)) {
            CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[RSDK.Random(0, 32, &Zone->randKey) >> 4] + 1), iwamodoki->position.x,
                          iwamodoki->position.y);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), iwamodoki->position.x, iwamodoki->position.y)->drawOrder = Zone->drawOrderHigh;
            RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
            destroyEntity(iwamodoki);
        }
    }

    foreach_active(Fireworm, fireworm)
    {
        if (RSDK.CheckObjectCollisionTouchBox(entity, &BuckwildBall->hitbox, fireworm, &crushHitbox)) {
            CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[RSDK.Random(0, 32, &Zone->randKey) >> 4] + 1), fireworm->position.x,
                          fireworm->position.y);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), fireworm->position.x, fireworm->position.y)->drawOrder = Zone->drawOrderHigh;
            RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
            destroyEntity(fireworm);
        }
    }

    foreach_active(Toxomister, toxomister)
    {
        if (RSDK.CheckObjectCollisionTouchBox(entity, &BuckwildBall->hitbox, toxomister, &crushHitbox)) {
            if (toxomister->state == Toxomister_State_Unknown1) {
                CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[RSDK.Random(0, 32, &Zone->randKey) >> 4] + 1), toxomister->position.x,
                              toxomister->position.y);
                CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), toxomister->position.x, toxomister->position.y)->drawOrder = Zone->drawOrderHigh;
                RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
                destroyEntity(toxomister);
            }
            else if (!toxomister->grabbedPlayer) {
                destroyEntity(toxomister);
            }
        }
    }

    Hitbox spikeHitbox;
    spikeHitbox.left   = -16;
    spikeHitbox.top    = -16;
    spikeHitbox.right  = 16;
    spikeHitbox.bottom = 16;

    foreach_active(Spikes, spikes)
    {
        if (RSDK.CheckObjectCollisionTouchBox(entity, &BuckwildBall->hitbox, spikes, &spikeHitbox)) {
            for (int i = 0; i < 2; ++i) {
                EntityDebris *debris =
                    CREATE_ENTITY(Debris, Debris_State_Fall, (((2 * (i != 0) - 1) * (spikes->type == 0)) << 19) + spikes->position.x,
                                  (((2 * (i != 0) - 1) * (spikes->type != 0)) << 19) + spikes->position.y);
                RSDK.SetSpriteAnimation(BuckwildBall->particleFrames, 4, &debris->animator, true, spikes->type >> 1);
                debris->drawOrder = Zone->drawOrderHigh;
                debris->direction = spikes->direction;
                debris->drawFX |= FX_ROTATE;
                debris->gravity    = 0x3800;
                debris->rotSpeed   = RSDK.Rand(-32, 32);
                debris->velocity.x = RSDK.Rand(-0x28000, 0x28000);
                debris->velocity.y = -0x1000 * RSDK.Rand(32, 96);
            }
            destroyEntity(spikes);
            RSDK.PlaySfx(BuckwildBall->sfxSharp, false, 255);
            RSDK.PlaySfx(BuckwildBall->sfxImpact, false, 255);
            entity->timerSfx = 8;
        }
    }
}

void BuckwildBall_State_Setup(void)
{
    RSDK_THIS(BuckwildBall);

    RSDK.SetSpriteAnimation(BuckwildBall->aniFrames, 0, &entity->animator, true, 0);

    entity->timerSfx = 0;
    if (entity->mode) {
        if (!--entity->mode) {
            int sizeX                   = entity->detectSize.x;
            int sizeY                   = entity->detectSize.y;
            entity->visible             = false;
            entity->state               = BuckwildBall_State_Unknown2;
            entity->drawOrder           = Zone->drawOrderLow;
            entity->detectHitbox.left   = -(sizeX >> 17);
            entity->detectHitbox.top    = -(sizeY >> 17);
            entity->detectHitbox.right  = sizeX >> 17;
            entity->detectHitbox.bottom = sizeY >> 17;
            entity->velocity.x          = 0;
            entity->ballPos             = entity->startPos;
            entity->ballPos.x += entity->detectOffset.x;
            entity->ballPos.y += entity->detectOffset.y;
            entity->velocity.y = 0;
        }
    }
    else {
        entity->state = BuckwildBall_State_Unknown1;
    }
}

void BuckwildBall_State_Unknown1(void)
{
    RSDK_THIS(BuckwildBall);

    int angle          = ((entity->speed & 0xFFFF) * (Zone->timer & 0xFFFF)) & 0x3FF;
    entity->position.x = (entity->amplitude << 6) * RSDK.Sin1024(angle) + entity->startPos.x;
    entity->direction  = (angle - 0x100) > 0x200;
}

void BuckwildBall_State_Unknown2(void)
{
    RSDK_THIS(BuckwildBall);

    entity->position.x = entity->ballPos.x;
    entity->position.y = entity->ballPos.y;

    foreach_active(Player, player)
    {
        if (!player->sidekick) {
            if (Player_CheckCollisionTouch(player, entity, &entity->detectHitbox)) {
                entity->visible = true;
                entity->active  = ACTIVE_NORMAL;
                entity->state   = BuckwildBall_State_Unknown3;
            }
        }
    }

    entity->position.x = entity->startPos.x;
    entity->position.y = entity->startPos.y;
}

void BuckwildBall_State_Unknown3(void)
{
    RSDK_THIS(BuckwildBall);

    entity->velocity.y += 0x3800;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x1C0000, true)) {
        if (!RSDK.GetEntityCount(Drillerdroid->objectID, true) || (Drillerdroid->field_5C[entity->field_B8] == 1)) {
            entity->velocity.y = 0;
            entity->state      = BuckwildBall_State_Unknown4;
            entity->velocity.x = abs(entity->speed << 15) * (2 * (entity->direction != FLIP_NONE) - 1);
        }
        else if (Drillerdroid->field_5C[entity->field_B8]) {
            entity->state                   = BuckwildBall_State_Unknown5;
            entity->velocity.x              = 0;
            entity->velocity.y              = -0x40000;
            entity->animator.animationSpeed = 0;
            entity->drawOrder               = Zone->drawOrderHigh;
        }
        else {
            //TODO: ???
            int slot                                                           = RSDK.GetEntityID(Drillerdroid->field_74);
            RSDK_GET_ENTITY(entity->field_B8 + slot + 6, )[1].alpha = 1;
            Drillerdroid->field_5C[entity->field_B8]                           = -1;
            entity->velocity.y                                                 = -0x40000;
        }

        BuckwildBall_HandleTimerSfx();
        Camera_ShakeScreen(0, 0, 5);
    }

    if (entity->field_B8 > -1) {
        BuckwildBall_CheckOnScreen();
    }
}

void BuckwildBall_State_Unknown4(void)
{
    RSDK_THIS(BuckwildBall);

    entity->velocity.y += 0x3800;
    entity->position.y += entity->velocity.y;
    entity->velocity.x += abs(entity->speed << 10) * (2 * !(entity->direction == FLIP_NONE) - 1);
    entity->position.x += entity->velocity.x;
    if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x1C0000, 2)) {
        entity->velocity.y = 0;
        if (abs(entity->velocity.x) > 0x20000 && RSDK.Rand(0, 100) > 0x50) {
            BuckwildBall_SpawnDebris();
            BuckwildBall_HandleTimerSfx();
            entity->velocity.y = -0x18000;
        }
    }

    bool32 collided = false;
    if (entity->direction == FLIP_X) {
        collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x1C0000, 0, true);
    }
    else if (entity->direction == FLIP_NONE) {
        collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x1C0000, 0, true);
    }

    if (collided) {
        entity->state = BuckwildBall_State_Unknown5;
        BuckwildBall_HandleTimerSfx();
        Camera_ShakeScreen(0, 0, 5);
        entity->velocity.x              = 0;
        entity->velocity.y              = -0x40000;
        entity->animator.animationSpeed = 0;
        entity->drawOrder               = Zone->drawOrderHigh;
    }

    if (entity->field_B8 > -1) {
        BuckwildBall_CheckOnScreen();
    }
}

void BuckwildBall_State_Unknown5(void)
{
    RSDK_THIS(BuckwildBall);
    entity->velocity.y += 0x3800;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->visible = Zone->timer % 4 < 2;

    BuckwildBall_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void BuckwildBall_EditorDraw(void)
{
    RSDK_THIS(BuckwildBall);

    RSDK.SetSpriteAnimation(BuckwildBall->aniFrames, 0, &entity->animator, true, 0);

    int sizeX                   = entity->detectSize.x;
    int sizeY                   = entity->detectSize.y;
    entity->detectHitbox.left   = -(sizeX >> 17);
    entity->detectHitbox.top    = -(sizeY >> 17);
    entity->detectHitbox.right  = sizeX >> 17;
    entity->detectHitbox.bottom = sizeY >> 17;
    entity->ballPos             = entity->position;
    entity->ballPos.x += entity->detectOffset.x;
    entity->ballPos.y += entity->detectOffset.y;

    BuckwildBall_Draw();

    DrawHelpers_DrawHitboxOutline(0xFF0000, FLIP_NONE, entity->ballPos.x, entity->ballPos.y, &entity->detectHitbox);
}

void BuckwildBall_EditorLoad(void)
{
    BuckwildBall->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/BuckwildBall.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(BuckwildBall, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void BuckwildBall_Serialize(void)
{
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_UINT32, amplitude);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_UINT8, mode);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_VECTOR2, detectOffset);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_VECTOR2, detectSize);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_BOOL, respawn);
}
