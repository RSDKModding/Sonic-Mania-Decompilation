#include "SonicMania.h"

ObjectPhantomGunner *PhantomGunner;

void PhantomGunner_Update(void)
{
    RSDK_THIS(PhantomGunner);
    StateMachine_Run(entity->state);
}

void PhantomGunner_LateUpdate(void) {}

void PhantomGunner_StaticUpdate(void) {}

void PhantomGunner_Draw(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.SetActivePalette(4, 0, ScreenInfo[SceneInfo->currentScreenID].height);

    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }

    RSDK.SetActivePalette(0, 0, ScreenInfo[SceneInfo->currentScreenID].height);
}

void PhantomGunner_Create(void *data)
{
    RSDK_THIS(PhantomGunner);

    if (!SceneInfo->inEditor) {
        entity->startPos      = entity->position;
        entity->visible       = true;
        entity->drawOrder     = 2;
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->type          = voidToInt(data);
        switch (entity->type) {
            case 0:
                entity->drawFX = FX_FLIP;
                entity->active = ACTIVE_NEVER;
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 0, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 1, &entity->animator2, true, 0);
                entity->posUnknown1   = entity->position;
                entity->posUnknown2.x = (entity->position.x >> 16) - ScreenInfo->centerX;
                entity->posUnknown2.y = (entity->position.y >> 16) - ScreenInfo->centerY;
                entity->stateDraw     = PhantomGunner_StateDraw_Unknown0;
                entity->state         = PhantomGunner_State_Unknown1;
                break;
            case 1:
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 10, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 11, &entity->animator2, true, 0);
                entity->stateDraw = PhantomGunner_StateDraw_Unknown1;
                entity->state     = PhantomGunner_State1_Unknown1;
                break;
            case 5:
                entity->posUnknown1 = entity->position;
                entity->state       = PhantomGunner_State2_Unknown;
                break;
            case 6:
                entity->posUnknown1 = entity->position;
                entity->state       = PhantomGunner_State3_Unknown;
                break;
            default: break;
        }
    }
}

void PhantomGunner_StageLoad(void)
{
    PhantomGunner->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomGunner.bin", SCOPE_STAGE);

    PhantomGunner->hitbox1.left   = -8;
    PhantomGunner->hitbox1.top    = 4;
    PhantomGunner->hitbox1.right  = 8;
    PhantomGunner->hitbox1.bottom = 26;

    PhantomGunner->hitbox2.left   = -16;
    PhantomGunner->hitbox2.top    = -32;
    PhantomGunner->hitbox2.right  = 16;
    PhantomGunner->hitbox2.bottom = -16;

    PhantomGunner->hitbox3.left   = -8;
    PhantomGunner->hitbox3.top    = -8;
    PhantomGunner->hitbox3.right  = 8;
    PhantomGunner->hitbox3.bottom = 8;

    PhantomGunner->sfxCannonFire = RSDK.GetSFX("Stage/CannonFire.wav");
}

void PhantomGunner_Explode(void)
{
    RSDK_THIS(PhantomGunner);
    if (!(Zone->timer & 7)) {
        int x                = RSDK.Sin512(entity->rotation) << 11;
        int y                = RSDK.Cos512(entity->rotation) << 11;
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, entity->position.x + x, entity->position.y - y);
        RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &debris->animator, true, 0);
        debris->velocity.x = RSDK.Sin512(entity->rotation) << 8;
        debris->velocity.y = RSDK.Sin512(entity->rotation) << 4;
        debris->drawOrder  = Zone->drawOrderLow;
        debris->timer      = 41;
    }
}

void PhantomGunner_Explode2(void)
{
    RSDK_THIS(PhantomGunner);

    if (Zone->timer & 3) {
        if ((Zone->timer & 3) == 2) {
            int x                = 0x600 * RSDK.Sin512(entity->rotation);
            int y                = 0x600 * RSDK.Cos512(entity->rotation);
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, entity->position.x + x, entity->position.y - y);
            RSDK.SetSpriteAnimation(Explosion->aniFrames, 2, &debris->animator, true, 0);
            debris->drawOrder = Zone->drawOrderHigh;
            debris->drawFX    = FX_SCALE;
            debris->timer     = 52;
            debris->scale.x   = (entity->scale.x * RSDK.Rand(128, 384)) >> 9;
            debris->scale.y   = debris->scale.x;
        }
    }
    else {
        int x                = RSDK.Sin512(entity->rotation) << 11;
        int y                = RSDK.Cos512(entity->rotation) << 11;
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, entity->position.x + x, entity->position.y - y);
        RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &debris->animator, true, 0);
        debris->velocity.x = RSDK.Sin512(entity->rotation) << 8;
        debris->velocity.y = RSDK.Sin512(entity->rotation) << 4;
        debris->drawOrder  = Zone->drawOrderLow;
        debris->drawFX     = FX_SCALE;
        debris->timer      = 41;
        debris->scale.y    = entity->scale.x;
    }
}

void PhantomGunner_SpawnDust(void)
{
    RSDK_THIS(PhantomGunner);

    for (int i = 0; i < 4; ++i) {
        int x            = entity->position.x + RSDK.Rand(-0x100000, 0x100000);
        int y            = entity->position.y + RSDK.Rand(-0x280000, -0x180000);
        EntityDust *dust = CREATE_ENTITY(Dust, NULL, x, y);
        dust->state      = Dust_State_DropDash;
        dust->drawOrder  = Zone->drawOrderHigh;
    }
}

void PhantomGunner_HandleRotations(int angle)
{
    RSDK_THIS(PhantomGunner);

    int ang = angle - entity->rotation;
    if (abs(angle - entity->rotation) >= abs(angle - entity->rotation - 0x200)) {
        if (abs(angle - entity->rotation - 0x200) < abs(angle - entity->rotation + 0x200)) {
            entity->rotation += ((ang - 0x200) >> 4);
            entity->rotation &= 0x1FF;
        }
        else {
            entity->rotation += ((ang + 0x200) >> 4);
            entity->rotation &= 0x1FF;
        }
    }
    else if (abs(angle - entity->rotation) < abs(angle - entity->rotation + 0x200)) {
        entity->rotation += (ang >> 4);
        entity->rotation &= 0x1FF;
    }
    else {
        entity->rotation += ((ang + 0x200) >> 4);
        entity->rotation &= 0x1FF;
    }
}

void PhantomGunner_CheckPlayerMissileCollisions(void)
{
    RSDK_THIS(PhantomGunner);

    foreach_active(Player, player)
    {
        if (entity->animator4.frameID > 0 && player->velocity.y >= 0 && Player_CheckBadnikTouch(player, entity, &PhantomGunner->hitbox2)
            && player->playerAnimator.animationID != ANI_HURT) {
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator4, true, 0);
            player->velocity.y = -0x60000;
            PhantomGunner_SpawnDust();
            entity->state = PhantomGunner_State1_Unknown3;
        }
        else {
            if (Player_CheckBadnikTouch(player, entity, &PhantomGunner->hitbox1)) {
                int anim = player->playerAnimator.animationID;
                if (anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH) {
                    EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), entity->position.x, entity->position.y);
                    explosion->interaction     = false;
                    explosion->drawOrder       = Zone->drawOrderHigh;
                    if (entity->animator4.animationID == 12 && entity->animator4.frameID > 0)
                        PhantomGunner_SpawnDust();
                    RSDK.PlaySfx(PhantomEgg->sfxExplosion2, false, 255);
                    destroyEntity(entity);
                    foreach_break;
                }
            }
        }
    }
}

void PhantomGunner_CheckPlayerExplosionCollisions(void)
{
    foreach_active(Player, player)
    {
        foreach_active(Explosion, explosion)
        {
            if (explosion->animator.frameID <= 6) {
                if (Player_CheckCollisionTouch(player, explosion, &PhantomGunner->hitbox3)) {
                    Player_CheckElementalHit(player, explosion, SHIELD_FIRE);
                }
            }
        }
    }
}

void PhantomGunner_StateDraw_Unknown0(void)
{
    RSDK_THIS(PhantomGunner);
    if (entity->invincibilityTimer & 1) {
        RSDK.CopyPalette(6, 128, 4, 128, 128);
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        RSDK.CopyPalette(5, 128, 4, 128, 128);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator2, NULL, false);
    }
}

void PhantomGunner_StateDraw_Unknown1(void)
{
    RSDK_THIS(PhantomGunner);
    EntityPhantomGunner *parent = (EntityPhantomGunner *)entity->parent;

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ((entity->posUnknown1.y + parent->position.y) >> 16) - ScreenInfo->position.y);

    Vector2 drawPos;
    drawPos.x = parent->position.x + entity->posUnknown1.x;
    drawPos.y = entity->position.y;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ScreenInfo->height);
    drawPos.y = entity->posUnknown1.y + parent->position.y;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
}

void PhantomGunner_StateDraw_Unknown2(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
    RSDK.DrawSprite(&entity->animator4, NULL, false);
}

void PhantomGunner_State_ResetState(void)
{
    RSDK_THIS(PhantomGunner);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    entity->angle      = (entity->angle + 3) & 0xFF;
    entity->position.y = (RSDK.Sin256(entity->angle) << 11) + entity->posUnknown1.y;

    entity->position.x += (ScreenInfo->position.x - entity->posUnknown2.x) << 15;
    entity->posUnknown2 = ScreenInfo->position;

    RSDK.ProcessAnimation(&entity->animator2);
    if (++entity->timer2 == 60) {
        entity->position      = entity->startPos;
        entity->posUnknown1   = entity->startPos;
        entity->timer2        = 0;
        entity->timer         = 0;
        entity->posUnknown2.x = (entity->position.x >> 16) - ScreenInfo->centerX;
        entity->posUnknown2.y = (entity->position.y >> 16) - ScreenInfo->centerY;
        entity->stateDraw     = PhantomGunner_StateDraw_Unknown0;
        entity->state         = PhantomGunner_State_Unknown1;
        entity->active        = ACTIVE_NEVER;
    }
}

void PhantomGunner_State_Unknown1(void)
{
    RSDK_THIS(PhantomGunner);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    entity->angle      = (entity->angle + 3) & 0xFF;
    entity->position.y = (RSDK.Sin256(entity->angle) << 11) + entity->posUnknown1.y;

    entity->position.x += (ScreenInfo->position.x - entity->posUnknown2.x) << 15;
    entity->posUnknown2 = ScreenInfo->position;
    RSDK.ProcessAnimation(&entity->animator2);

    entity->timer3 = 8;
    if (++entity->timer2 < 120 || RSDK.GetEntityCount(PhantomGunner->objectID, true) >= 2) {
        PhantomGunner_CheckPlayerExplosionCollisions();
    }
    else {
        if (++entity->timer == 2) {
            PhantomEgg_SetupScanlineCB();
            entity->timer2 = 0;
            entity->state  = PhantomGunner_State_ResetState;
        }
        else {
            PhantomGunner->value2 = 0;
            entity->timer2        = 0;
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 2, &entity->animator1, true, 0);
            entity->state = PhantomGunner_State_Unknown2;
            PhantomGunner_CheckPlayerExplosionCollisions();
        }
    }
}

void PhantomGunner_State_Unknown2(void)
{
    RSDK_THIS(PhantomGunner);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    entity->angle      = (entity->angle + 3) & 0xFF;
    entity->position.y = (RSDK.Sin256(entity->angle) << 11) + entity->posUnknown1.y;

    entity->position.x += (ScreenInfo->position.x - entity->posUnknown2.x) << 15;
    entity->posUnknown2 = ScreenInfo->position;
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);

    if (++entity->timer2 == 8) {
        EntityPhantomGunner *child = CREATE_ENTITY(PhantomGunner, intToVoid(1), entity->position.x, entity->position.y);
        child->parent              = (Entity *)entity;
        child->posUnknown1.x       = PhantomGunner->value1[entity->field_8C] << 16;
        child->posUnknown1.y       = PhantomGunner->value1[entity->field_8C + 1] << 16;
        child->position.x += child->posUnknown1.x;
        child->position.y += child->posUnknown1.y + 0x100000;
        entity->field_8C = (entity->field_8C - 14) & 0x1F;
        RSDK.PlaySfx(PhantomGunner->sfxCannonFire, false, 255);
    }

    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        entity->timer2 = 0;
        if (--entity->timer3 > 0) {
            if (!(entity->timer3 & 1))
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 2, &entity->animator1, true, 0);
            else
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 3, &entity->animator1, true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 0, &entity->animator1, true, 0);
            entity->state = PhantomGunner_State_Unknown1;
        }
    }
    PhantomGunner_CheckPlayerExplosionCollisions();
}

void PhantomGunner_State1_Unknown1(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    entity->position.y -= 0x80000;
    if (++entity->timer2 == 60) {
        EntityPhantomGunner *parent = (EntityPhantomGunner *)entity->parent;

        entity->timer2 = 0;
        if ((PhantomGunner->value2 & 3) == 3)
            entity->type = 4;
        else
            entity->type = (PhantomGunner->value2 & 1) + 2;
        ++PhantomGunner->value2;
        entity->drawOrder  = Zone->drawOrderLow;
        entity->position.y = (ScreenInfo->position.y - 64) << 16;

        bool32 flag = false;
        while (!flag) {
            flag               = true;
            entity->position.x = parent->position.x + RSDK.Rand(-0x1000000, 0x1000000);

            foreach_active(PhantomGunner, gunner)
            {
                if (gunner != entity && gunner->type >= 2) {
                    int dist = abs(gunner->position.x - entity->position.x);
                    if (dist < 0x180000 && gunner->position.y - entity->position.y < 0x800000)
                        flag = false;
                }
            }
        }

        if (entity->type == 2) {
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 4, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 5, &entity->animator3, true, 0);
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 12, &entity->animator4, true, 0);
            entity->state     = PhantomGunner_State1_Unknown2;
            entity->timer2    = RSDK.Rand(0, 2) << 8;
            entity->stateDraw = PhantomGunner_StateDraw_Unknown2;
        }
        else if (entity->type == 3) {
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 6, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 7, &entity->animator3, true, 0);
            entity->state     = PhantomGunner_State1_Unknown3;
            entity->stateDraw = PhantomGunner_StateDraw_Unknown2;
        }
        else if (entity->type == 4) {
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 8, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 9, &entity->animator3, true, 0);
            entity->position.x = parent->position.x;
            entity->drawFX     = FX_ROTATE;
            entity->state      = PhantomGunner_State1_Unknown4;
            if (!(Zone->timer & 1))
                entity->velocity.x = 0x40000;
            else
                entity->velocity.x = -0x40000;
            entity->stateDraw = PhantomGunner_StateDraw_Unknown2;
        }
    }
}

void PhantomGunner_State1_Unknown2(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessAnimation(&entity->animator3);

    if (entity->animator4.frameID) {
        entity->drawFX = FX_ROTATE;
        if (entity->velocity.y > 0xC000)
            entity->velocity.y -= 0x3800;
        entity->rotation = RSDK.Sin512(entity->timer2) >> 6;
        entity->timer2   = (entity->timer2 + 4) & 0x1FF;
    }
    else if (entity->velocity.y < 0x40000)
        entity->velocity.y += 0x3800;
    entity->position.y += entity->velocity.y;

    PhantomGunner_CheckPlayerMissileCollisions();
    if (entity->objectID) {
        if (RSDK.ObjectTileCollision(SceneInfo->entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true)) {
            if (entity->type == 3) {
                EntityPhantomGunner *child = CREATE_ENTITY(PhantomGunner, intToVoid(5), entity->position.x, entity->position.y);
                if (entity->velocity.y > 0x20000)
                    child->velocity.x = 0x80000;
                else
                    child->velocity.x = 0x40000;
                destroyEntity(entity);
            }
            else {
                CREATE_ENTITY(PhantomGunner, intToVoid(6), entity->position.x, entity->position.y);
                destroyEntity(entity);
            }
        }
    }
}

void PhantomGunner_State1_Unknown3(void)
{
    RSDK_THIS(PhantomGunner);
    RSDK.ProcessAnimation(&entity->animator3);

    if (entity->velocity.y < 0x40000)
        entity->velocity.y += 0x1800;
    entity->position.y += entity->velocity.y;

    PhantomGunner_CheckPlayerMissileCollisions();
    if (entity->objectID) {
        if (RSDK.ObjectTileCollision(SceneInfo->entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true)) {
            if (entity->type == 3) {
                EntityPhantomGunner *child = CREATE_ENTITY(PhantomGunner, intToVoid(5), entity->position.x, entity->position.y);
                if (entity->velocity.y > 0x20000)
                    child->velocity.x = 0x80000;
                else
                    child->velocity.x = 0x40000;
                destroyEntity(entity);
            }
            else {
                CREATE_ENTITY(PhantomGunner, intToVoid(6), entity->position.x, entity->position.y);
                destroyEntity(entity);
            }
        }
    }
}

void PhantomGunner_State1_Unknown4(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.ProcessAnimation(&entity->animator3);
    EntityPhantomGunner *parent = (EntityPhantomGunner *)entity->parent;

    ++entity->timer2;

    entity->velocity.x = RSDK.Cos256(entity->timer2) << 11;
    if (entity->position.y < parent->position.y - 0x800000) {
        if (entity->velocity.y < 0x20000)
            entity->velocity.y += 0x2000;
    }
    if (entity->position.y > parent->position.y - 0x200000) {
        if (entity->velocity.y > -0x20000)
            entity->velocity.y -= 0x2000;
    }

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    PhantomGunner_Explode();

    int angle = RSDK.ATan2(entity->velocity.y, -entity->velocity.x);
    PhantomGunner_HandleRotations(2 * angle);
    if (entity->timer2 == 320) {
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), entity->position.x, entity->position.y);
        explosion->interaction     = false;
        explosion->drawOrder       = Zone->drawOrderHigh;
        destroyEntity(entity);
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, entity, &PhantomGunner->hitbox3)) {
                entity->angle     = entity->rotation << 8;
                entity->timer2    = 0;
                entity->drawFX    = FX_SCALE | FX_ROTATE;
                entity->groundVel = 0x4000;
                entity->scale.x   = 0x200;
                entity->scale.y   = 0x200;
                entity->state     = PhantomGunner_State1_Unknown5;
                RSDK.PlaySfx(PhantomEgg->sfxHit, false, 255);
            }
        }
    }
}

void PhantomGunner_State1_Unknown5(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.ProcessAnimation(&entity->animator3);

    entity->angle += entity->groundVel;
    entity->rotation = entity->angle >> 8;
    entity->groundVel -= (entity->groundVel >> 3);

    PhantomGunner_Explode();
    if (entity->groundVel < 128) {
        RSDK.PlaySfx(PhantomEgg->sfxRocketJet, false, 255);
        entity->state = PhantomGunner_State1_Unknown6;
    }
}

void PhantomGunner_State1_Unknown6(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.ProcessAnimation(&entity->animator3);
    EntityPhantomGunner *parent = (EntityPhantomGunner *)entity->parent;

    int angle = RSDK.ATan2((parent->position.y - entity->position.y) >> 16, -((parent->position.x - entity->position.x) >> 16));
    PhantomGunner_HandleRotations(2 * angle);
    entity->rotation &= 0x1FF;
    entity->position.x -= RSDK.Sin512(entity->rotation) << 9;
    entity->scale.x -= 6;
    entity->position.y += RSDK.Cos512(entity->rotation) << 9;
    entity->scale.y = entity->scale.x;

    PhantomGunner_Explode2();
    if (entity->scale.x < 128) {
        RSDK.PlaySfx(PhantomEgg->sfxHit, false, 255);
        parent->invincibilityTimer = 48;
        entity->position.x         = parent->position.x;
        entity->position.y         = parent->position.y;
        entity->visible            = false;
        entity->state              = PhantomGunner_State1_Unknown7;
    }
}

void PhantomGunner_State1_Unknown7(void)
{
    RSDK_THIS(PhantomGunner);

    if (!(Zone->timer % 3) && !(Zone->timer & 4)) {
        int x                      = entity->position.x + RSDK.Rand(-0x100000, 0x100000);
        int y                      = entity->position.y + RSDK.Rand(-0x100000, 0x100000);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
        explosion->interaction     = false;
        explosion->drawOrder       = 1;
        explosion->scale           = entity->scale;
    }
    if (++entity->timer2 == 16)
        destroyEntity(entity);
}

void PhantomGunner_State2_Unknown(void)
{
    RSDK_THIS(PhantomGunner);

    if (!(Zone->timer & 3)) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), entity->posUnknown1.x - entity->field_90, entity->position.y);
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), entity->posUnknown1.x + entity->field_90, entity->position.y);
    }
    entity->field_90 += entity->velocity.x;
    if (++entity->timer2 == 16)
        destroyEntity(entity);
}

void PhantomGunner_State3_Unknown(void)
{
    RSDK_THIS(PhantomGunner);

    if (!(Zone->timer & 3))
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), entity->position.x, entity->position.y);
    entity->position.y -= 0x40000;
    if (++entity->timer2 == 16)
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void PhantomGunner_EditorDraw(void)
{
    RSDK_THIS(PhantomGunner);
    entity->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 0, &entity->animator1, false, 0);
    RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 1, &entity->animator2, false, 0);
    entity->posUnknown1   = entity->position;
    entity->posUnknown2.x = (entity->position.x >> 16) - ScreenInfo->centerX;
    entity->posUnknown2.y = (entity->position.y >> 16) - ScreenInfo->centerY;

    PhantomGunner_StateDraw_Unknown0();
}

void PhantomGunner_EditorLoad(void) { PhantomGunner->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomGunner.bin", SCOPE_STAGE); }
#endif

void PhantomGunner_Serialize(void) {}
