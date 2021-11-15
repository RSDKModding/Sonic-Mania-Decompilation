#include "SonicMania.h"

ObjectRexon *Rexon;

void Rexon_Update(void)
{
    RSDK_THIS(Rexon);

    StateMachine_Run(entity->state);

    entity->positions[Rexon_SegmentCount].x = entity->position.x;
    entity->positions[Rexon_SegmentCount].y = entity->position.y;
    if (entity->direction)
        entity->positions[Rexon_SegmentCount].x += 0x1C0000;
    else
        entity->positions[Rexon_SegmentCount].x -= 0x1C0000;
    entity->positions[Rexon_SegmentCount].y += 0x110000;

    for (int i = Rexon_SegmentCount - 1; i >= 0; --i) {
        int y = entity->field_B4[i] >> 1;
        if (entity->direction == FLIP_X)
            y = (0x80 - y) & 0xFF;
        entity->positions[i].x = entity->field_A0[i] * RSDK.Cos256(y) + entity->positions[i + 1].x;
        entity->positions[i].y = entity->field_A0[i] * RSDK.Sin256(y) + entity->positions[i + 1].y;
    }
}

void Rexon_LateUpdate(void) {}

void Rexon_StaticUpdate(void) {}

void Rexon_Draw(void)
{
    RSDK_THIS(Rexon);

    RSDK.DrawSprite(&entity->animator1, NULL, false);
    if (entity->state != Rexon_State_Destroyed && entity->state != Rexon_State_Explode) {
        RSDK.DrawSprite(&entity->animator3, entity->positions, false);
        for (int i = 1; i < Rexon_SegmentCount; ++i) RSDK.DrawSprite(&entity->animator2, &entity->positions[i], false);
    }
}

void Rexon_Create(void *data)
{
    RSDK_THIS(Rexon);

    entity->drawFX |= FX_FLIP;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->startDir      = entity->direction;
    entity->visible       = true;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->type          = voidToInt(data);

    switch (entity->type) {
        case 0:
            entity->timer = 128;
            if (!entity->noMove)
                entity->velocity.x = -0x2000;

            for (int i = 0; i < Rexon_SegmentCount; ++i) {
                entity->field_A0[i] = 0;
                entity->field_B4[i] = 0x170;
                entity->field_C8[i] = 0;
            }

            entity->segmentID = 0;
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 3, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 1, &entity->animator3, true, 2);
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 2, &entity->animator2, true, 0);
            entity->state = Rexon_State_Setup;
            break;
        case 1:
            entity->active = ACTIVE_NORMAL;
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 2, &entity->animator1, true, 0);
            entity->state = Rexon_State1_Unknown;
            break;
        case 2:
            entity->active = ACTIVE_NORMAL;
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 4, &entity->animator1, true, 0);
            entity->state = Rexon_State2_Unknown;
            break;
    }
}

void Rexon_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Rexon->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Rexon.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        Rexon->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Rexon.bin", SCOPE_STAGE);

    Rexon->hitbox1.left   = -8;
    Rexon->hitbox1.top    = -8;
    Rexon->hitbox1.right  = 8;
    Rexon->hitbox1.bottom = 8;

    Rexon->hitbox2.left   = -16;
    Rexon->hitbox2.top    = -8;
    Rexon->hitbox2.right  = 16;
    Rexon->hitbox2.bottom = 8;

    Rexon->hitbox3.left   = -160;
    Rexon->hitbox3.top    = -256;
    Rexon->hitbox3.right  = 96;
    Rexon->hitbox3.bottom = 256;

    Rexon->hitbox4.left   = -4;
    Rexon->hitbox4.top    = -4;
    Rexon->hitbox4.right  = 4;
    Rexon->hitbox4.bottom = 4;

    Rexon->sfxShot      = RSDK.GetSFX("Stage/Shot.wav");
    Rexon->sfxExplosion = RSDK.GetSFX("Stage/Explosion2.wav");

    DEBUGMODE_ADD_OBJ(Rexon);
}

void Rexon_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Rexon, NULL, entity->position.x, entity->position.y);
}

void Rexon_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Rexon->aniFrames, 3, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Rexon_CheckOnScreen(void)
{
    RSDK_THIS(Rexon);

    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        Rexon_Create(NULL);
    }
}

void Rexon_CheckPlayerCollisions(void)
{
    RSDK_THIS(Rexon);

    if (entity->state != Rexon_State_Explode) {
        int storeX = entity->position.x;
        int storeY = entity->position.y;

        foreach_active(Player, player)
        {
            Player_CheckCollisionBox(player, entity, &Rexon->hitbox2);
            if (entity->state != Rexon_State_Destroyed) {
                entity->position.x = entity->positions[0].x;
                entity->position.y = entity->positions[0].y;
                if (Player_CheckBadnikTouch(player, entity, &Rexon->hitbox1)) {
                    if (Player_CheckBadnikBreak(entity, player, false))
                        Rexon_Destroy(entity, false);
                }
                else {
                    for (int i = 1; i < Rexon_SegmentCount; ++i) {
                        entity->position.x = entity->positions[i].x;
                        entity->position.y = entity->positions[i].y;
                        if (Player_CheckCollisionTouch(player, entity, &Rexon->hitbox1))
                            Player_CheckHit(player, entity);
                    }
                }
                entity->position.x = storeX;
                entity->position.y = storeY;
            }
        }
    }
}

void Rexon_Destroy(EntityRexon *rexon, bool32 crushed)
{
    if (rexon->state != Rexon_State_Destroyed && rexon->state != Rexon_State_Explode) {
        int x             = rexon->position.x;
        int y             = rexon->position.y;
        rexon->position.x = rexon->positions[0].x;
        rexon->position.y = rexon->positions[0].y;
#if RETRO_USE_PLUS
        CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[RSDK.Random(0, 32, &Zone->randKey) >> 4] + 1), rexon->position.x, rexon->position.y);
#else
        CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[RSDK.Rand(0, 32) >> 4] + 1), rexon->position.x, rexon->position.y);
#endif
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), rexon->position.x, rexon->position.y)->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
        rexon->position.x = x;
        rexon->position.y = y;

        int32 velocities[] = { -0x8000, 0x10000, -0x10000, 0x8000 };
        for (int i = 1; i < Rexon_SegmentCount; ++i) {
            EntityRexon *debris = CREATE_ENTITY(Rexon, intToVoid(1), rexon->positions[i].x, rexon->positions[i].y);
            debris->direction   = rexon->direction;
            debris->velocity.x  = velocities[(i - 1) & 3];
        }
    }
    if (crushed) {
        rexon->active       = ACTIVE_NORMAL;
        rexon->destroyDelay = 60;
        rexon->state        = Rexon_State_Explode;
    }
    else {
        rexon->state = Rexon_State_Destroyed;
    }
}

void Rexon_State_Setup(void)
{
    RSDK_THIS(Rexon);

    entity->active = ACTIVE_NORMAL;
    entity->state  = Rexon_State_Unknown1;
    Rexon_State_Unknown1();
}

void Rexon_State_Unknown1(void)
{
    RSDK_THIS(Rexon);

    if (entity->noMove) {
        entity->direction = Player_GetNearestPlayer()->position.x >= entity->position.x;
    }
    else {
        entity->position.x += entity->velocity.x;
        if (--entity->timer <= 0) {
            entity->direction ^= FLIP_X;
            entity->timer      = 128;
            entity->velocity.x = -entity->velocity.x;
        }
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Rexon->hitbox3)) {
            entity->state     = Rexon_State_Unknown2;
            entity->direction = entity->direction = player->position.x >= entity->position.x;
            foreach_break;
        }
    }

    Rexon_CheckPlayerCollisions();
    Rexon_CheckOnScreen();
}

void Rexon_State_Unknown2(void)
{
    RSDK_THIS(Rexon);

    entity->field_A0[entity->segmentID] += 640;
    if (entity->field_A0[entity->segmentID] >= 0xF00) {
        entity->field_A0[entity->segmentID] = 0xF00;
        ++entity->segmentID;
    }

    for (int i = entity->segmentID - 1; i >= 0; --i) {
        entity->field_B4[i] += 2;
    }

    if (entity->segmentID == Rexon_SegmentCount)
        entity->state = Rexon_State_Unknown3;
    Rexon_CheckPlayerCollisions();
    Rexon_CheckOnScreen();
}

void Rexon_State_Unknown3(void)
{
    RSDK_THIS(Rexon);

    RSDK.ProcessAnimation(&entity->animator3);

    for (int i = 0; i < Rexon_SegmentCount; ++i) {
        if (entity->field_C8[i]) {
            if (--entity->field_B4[i] <= 352)
                entity->field_C8[i] = false;
        }
        else if (++entity->field_B4[i] >= 416) {
            entity->field_C8[i] = true;
        }
    }

    if (entity->field_B4[0] == 0x160) {
        RSDK.PlaySfx(Rexon->sfxShot, false, 255);
        EntityRexon *shot = CREATE_ENTITY(Rexon, intToVoid(2), entity->positions[0].x, entity->positions[0].y);
        if (entity->direction) {
            shot->position.x += 0xE0000;
            shot->velocity.x = 0x10000;
        }
        else {
            shot->position.x -= 0xE0000;
            shot->velocity.x = -0x10000;
        }
        shot->position.y += 0x60000;
        shot->velocity.y = 0x8000;
    }
    else if (entity->field_B4[0] == 0x170) {
        if (entity->field_C8[0] == 1)
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 0, &entity->animator3, true, 0);
    }
    else if (entity->field_B4[0] == 0x180 && !entity->field_C8[0]) {
        RSDK.SetSpriteAnimation(Rexon->aniFrames, 1, &entity->animator3, true, 0);
    }
    Rexon_CheckPlayerCollisions();
    Rexon_CheckOnScreen();
}

void Rexon_State_Destroyed(void)
{
    Rexon_CheckPlayerCollisions();
    Rexon_CheckOnScreen();
}

void Rexon_State_Explode(void)
{
    RSDK_THIS(Rexon);

    entity->position.y += 0x4000;
    entity->velocity.y = 0x4000;

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(Rexon->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int x                      = entity->position.x + (RSDK.Rand(Rexon->hitbox2.left, Rexon->hitbox2.right) << 16);
            int y                      = entity->position.y + (RSDK.Rand(Rexon->hitbox2.top, Rexon->hitbox2.bottom) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawOrder       = Zone->drawOrderHigh;
        }
    }
    if (--entity->destroyDelay <= 0)
        destroyEntity(entity);
}

void Rexon_State1_Unknown(void)
{
    RSDK_THIS(Rexon);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->visible ^= true;

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

void Rexon_State2_Unknown(void)
{
    RSDK_THIS(Rexon);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Rexon->hitbox4)) {
            Player_CheckProjectileHit(player, entity);
        }
    }

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, Rexon->hitbox4.bottom << 13, 4)
        || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, Rexon->hitbox4.left << 13, 0, 4)
        || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, Rexon->hitbox4.top << 13, 4)
        || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, Rexon->hitbox4.right << 13, 0, 4) || !RSDK.CheckOnScreen(entity, NULL)) {
        destroyEntity(entity);
    }
}

#if RETRO_INCLUDE_EDITOR
void Rexon_EditorDraw(void)
{
    RSDK_THIS(Rexon);

    for (int i = 0; i < Rexon_SegmentCount; ++i) {
        entity->field_A0[i] = 0xF00;
        entity->field_B4[i] = 0x170;
    }

    entity->segmentID = 0;
    RSDK.SetSpriteAnimation(Rexon->aniFrames, 3, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(Rexon->aniFrames, 1, &entity->animator3, true, 2);
    RSDK.SetSpriteAnimation(Rexon->aniFrames, 2, &entity->animator2, true, 0);

    entity->positions[Rexon_SegmentCount].x = entity->position.x;
    entity->positions[Rexon_SegmentCount].y = entity->position.y;
    if (entity->direction)
        entity->positions[Rexon_SegmentCount].x += 0x1C0000;
    else
        entity->positions[Rexon_SegmentCount].x -= 0x1C0000;
    entity->positions[Rexon_SegmentCount].y += 0x110000;

    for (int i = Rexon_SegmentCount - 1; i >= 0; --i) {
        int y = entity->field_B4[i] >> 1;
        if (entity->direction == FLIP_X)
            y = (0x80 - y) & 0xFF;
        entity->positions[i].x = entity->field_A0[i] * RSDK.Cos256(y) + entity->positions[i + 1].x;
        entity->positions[i].y = entity->field_A0[i] * RSDK.Sin256(y) + entity->positions[i + 1].y;
    }

    Rexon_Draw();
}

void Rexon_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Rexon->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Rexon.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        Rexon->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Rexon.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Rexon, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void Rexon_Serialize(void)
{
    RSDK_EDITABLE_VAR(Rexon, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Rexon, VAR_BOOL, noMove);
}
