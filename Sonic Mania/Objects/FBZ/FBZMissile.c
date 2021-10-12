#include "SonicMania.h"

ObjectFBZMissile *FBZMissile;

void FBZMissile_Update(void)
{
    RSDK_THIS(FBZMissile);
    StateMachine_Run(entity->state);
}

void FBZMissile_LateUpdate(void) {}

void FBZMissile_StaticUpdate(void) {}

void FBZMissile_Draw(void)
{
    RSDK_THIS(FBZMissile);
    StateMachine_Run(entity->stateDraw);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void FBZMissile_Create(void *data)
{
    RSDK_THIS(FBZMissile);
    if (entity->type != 2 && !entity->interval)
        entity->interval = -16;
    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        if (data)
            entity->type = voidToInt(data);

        switch (entity->type) {
            case 0:
                RSDK.SetSpriteAnimation(FBZMissile->aniFrames, 0, &entity->animator, true, 0);
                entity->direction *= 2;
                entity->drawOrder = Zone->drawOrderHigh;
                entity->state     = FBZMissile_Unknown1;
                break;
            case 1:
                RSDK.SetSpriteAnimation(FBZMissile->aniFrames, 1, &entity->animator, true, 0);
                entity->state = FBZMissile_Unknown3;
                break;
            case 2:
                RSDK.SetSpriteAnimation(FBZMissile->aniFrames, 3, &entity->animator, true, 0);
                entity->drawOrder = Zone->drawOrderHigh;
                entity->timer     = entity->interval;
                entity->state     = FBZMissile_Unknown8;
                break;
            case 3:
                RSDK.SetSpriteAnimation(FBZMissile->aniFrames, 2, &entity->animator, true, 0);
                entity->state = FBZMissile_Unknown5;
                break;
            case 4:
                RSDK.SetSpriteAnimation(FBZMissile->aniFrames, 1, &entity->animator, true, 0);
                entity->state = FBZMissile_Unknown7;
                break;
            default: break;
        }
    }
}

void FBZMissile_StageLoad(void)
{
    FBZMissile->aniFrames      = RSDK.LoadSpriteAnimation("FBZ/Missile.bin", SCOPE_STAGE);
    FBZMissile->hitbox2.left   = -16;
    FBZMissile->hitbox2.top    = -3;
    FBZMissile->hitbox2.right  = 16;
    FBZMissile->hitbox2.bottom = 3;
    FBZMissile->hitbox1.left   = -5;
    FBZMissile->hitbox1.top    = -8;
    FBZMissile->hitbox1.right  = 5;
    FBZMissile->hitbox1.bottom = 8;
    FBZMissile->hitbox3.left   = -32;
    FBZMissile->hitbox3.top    = -8;
    FBZMissile->hitbox3.right  = 32;
    FBZMissile->hitbox3.bottom = 8;
    FBZMissile->sfxPush        = RSDK.GetSFX("Stage/Push.wav");
    FBZMissile->sfxPush2       = RSDK.GetSFX("Stage/Push.wav");
    FBZMissile->sfxExplosion   = RSDK.GetSFX("Stage/Explosion2.wav");
}

void FBZMissile_Unknown1(void)
{
    RSDK_THIS(FBZMissile);

    if (!((Zone->timer + entity->intervalOffset) % entity->interval)) {
        entity->timer = 42;
        entity->state = FBZMissile_Unknown2;
    }
    if (!(Zone->timer & 1)) {
        if (entity->animator.frameID > 0)
            entity->animator.frameID--;
    }
}

void FBZMissile_Unknown2(void)
{
    RSDK_THIS(FBZMissile);

    if (!(entity->timer & 1)) {
        if (entity->animator.frameID < 3)
            entity->animator.frameID++;
    }

    entity->timer--;
    switch (entity->timer) {
        case 1: entity->state = FBZMissile_Unknown1; break;
        case 9:
        case 25:
        case 41: {
            RSDK.PlaySfx(FBZMissile->sfxPush, false, 255);
            EntityFBZMissile *missile = CREATE_ENTITY(FBZMissile, intToVoid(3), entity->position.x, entity->position.y);
            missile->isPermanent      = true;
            missile->velocity.y       = -0x60000;
            missile->drawOrder        = entity->drawOrder - 1;
            missile->velocity.x       = FBZMissile->velocities[FBZMissile->velocityID++];
            FBZMissile->velocityID &= 7;
            break;
        }
        default: break;
    }
}

void FBZMissile_Unknown3(void)
{
    RSDK_THIS(FBZMissile);

    if (!((Zone->timer + entity->intervalOffset) % entity->interval)) {
        RSDK.PlaySfx(FBZMissile->sfxPush2, false, 255);
        EntityFBZMissile *missile = CREATE_ENTITY(FBZMissile, intToVoid(4), entity->position.x, entity->position.y);
        missile->drawOrder        = entity->drawOrder;
        missile->direction        = entity->direction;
        if (entity->direction) {
            missile->velocity.x = 0x40000;
            entity->position.x -= 500000;
        }
        else {
            missile->velocity.x = -0x40000;
            entity->position.x += 500000;
        }
        entity->timer = 62;
        entity->state = FBZMissile_Unknown4;
    }
}

void FBZMissile_Unknown4(void)
{
    RSDK_THIS(FBZMissile);
    if (--entity->timer > 0) {
        if (entity->direction)
            entity->position.x += 0x2000;
        else
            entity->position.x -= 0x2000;
    }
    else {
        entity->state = FBZMissile_Unknown3;
    }
}

void FBZMissile_Unknown5(void)
{
    RSDK_THIS(FBZMissile);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1800;
    if (entity->velocity.y >= 0) {
        entity->drawOrder = Zone->drawOrderLow;
        entity->state     = FBZMissile_Unknown6;
    }
    if (entity->velocity.y > -0x1D000)
        entity->position.x += entity->velocity.x;

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

void FBZMissile_Unknown6(void)
{
    RSDK_THIS(FBZMissile);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1000;
    if (!entity->timer) {
        if (entity->animator.frameID < 5)
            entity->animator.frameID++;
    }
    entity->timer = (entity->timer - 1) & 1;
    if (entity->velocity.y < 0x1D000)
        entity->position.x += entity->velocity.x;

    if (RSDK.CheckOnScreen(entity, NULL)) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &FBZMissile->hitbox1)) {
#if RETRO_USE_PLUS
                if (!Player_CheckMightyUnspin(0x300, player, 2, &player->uncurlTimer))
#endif
                    Player_CheckHit(player, entity);

                CREATE_ENTITY(Explosion, intToVoid(3), entity->position.x, entity->position.y + 0x30000)->drawOrder = Zone->drawOrderHigh;
                RSDK.PlaySfx(FBZMissile->sfxExplosion, false, 255);
                destroyEntity(entity);

                foreach_break;
            }
        }

        if (!RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xC0000, false)) {
            foreach_active(FBZMissile, missile)
            {
                if (missile->type == 2 && RSDK.CheckObjectCollisionTouchBox(missile, &FBZMissile->hitbox3, entity, &FBZMissile->hitbox1)) {
                    if (--missile->timer <= 0) {
                        destroyEntity(missile);
                        RSDK.PlaySfx(Player->sfx_Release, false, 255);
                    }
                    CREATE_ENTITY(Explosion, intToVoid(3), entity->position.x, entity->position.y + 0x30000)->drawOrder = Zone->drawOrderHigh;
                    RSDK.PlaySfx(FBZMissile->sfxExplosion, false, 255);
                    destroyEntity(entity);
                }
            }
        }
        else {
            CREATE_ENTITY(Explosion, intToVoid(3), entity->position.x, entity->position.y + 0x30000)->drawOrder = Zone->drawOrderHigh;
            RSDK.PlaySfx(FBZMissile->sfxExplosion, false, 255);
            destroyEntity(entity);
        }
    }
    else {
        destroyEntity(entity);
    }
}

void FBZMissile_Unknown7(void)
{
    RSDK_THIS(FBZMissile);
    entity->position.x += entity->velocity.x;
    if (RSDK.CheckOnScreen(entity, 0)) {
        RSDK.ProcessAnimation(&entity->animator);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &FBZMissile->hitbox2)
#if RETRO_USE_PLUS
                && !Player_CheckMightyUnspin(768, player, 2, &player->uncurlTimer)
#endif
            ) {
                Player_CheckHit(player, entity);
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void FBZMissile_Unknown8(void)
{
    RSDK_THIS(FBZMissile);
    entity->position.x += entity->velocity.x;
    RSDK.ProcessAnimation(&entity->animator);

    foreach_active(Player, player)
    {
        int velY = player->velocity.y;
        if (Player_CheckCollisionBox(player, entity, &FBZMissile->hitbox3)
#if RETRO_USE_PLUS
            && player->state == Player_State_MightyHammerDrop 
#endif
            && !player->sidekick) {
            RSDK.PlaySfx(FBZMissile->sfxExplosion, false, 255);
            RSDK.PlaySfx(Player->sfx_Release, false, 255);
            player->velocity.y = velY - 0x10000;
            player->onGround   = false;
            destroyEntity(entity);
            foreach_break;
        }
    }
}

void FBZMissile_EditorDraw(void) {}

void FBZMissile_EditorLoad(void) {}

void FBZMissile_Serialize(void)
{
    RSDK_EDITABLE_VAR(FBZMissile, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(FBZMissile, VAR_UINT8, interval);
    RSDK_EDITABLE_VAR(FBZMissile, VAR_UINT8, intervalOffset);
    RSDK_EDITABLE_VAR(FBZMissile, VAR_UINT8, direction);
}
