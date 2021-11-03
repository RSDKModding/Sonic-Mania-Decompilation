#include "SonicMania.h"

ObjectERZGunner *ERZGunner;

void ERZGunner_Update(void)
{
    RSDK_THIS(ERZGunner);

    StateMachine_Run(entity->state);
}

void ERZGunner_LateUpdate(void) {}

void ERZGunner_StaticUpdate(void) {}

void ERZGunner_Draw(void)
{
    RSDK_THIS(ERZGunner);

    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else
        RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void ERZGunner_Create(void *data)
{
    RSDK_THIS(ERZGunner);

    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = 1;
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->type          = voidToInt(data);
        switch (entity->type) {
            case 0:
                entity->drawFX = FX_FLIP;
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 0, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 1, &entity->animator2, true, 0);

                entity->posUnknown1.x = entity->position.x;
                entity->posUnknown1.y = entity->position.y;
                entity->posUnknown2   = RSDK_screens->position;
                entity->stateDraw     = ERZGunner_StateDraw_Unknown0;
                entity->state         = ERZGunner_State_Unknown1;
                break;
            case 1:
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 10, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 11, &entity->animator2, true, 0);
                entity->stateDraw = ERZGunner_StateDraw_Unknown1;
                entity->state     = ERZGunner_State1_Unknown1;
                break;
            case 5:
                entity->posUnknown1.x = entity->position.x;
                entity->posUnknown1.y = entity->position.y;
                entity->state         = ERZGunner_State2_Unknown;
                break;
            case 6:
                entity->posUnknown1.x = entity->position.x;
                entity->posUnknown1.y = entity->position.y;
                entity->state         = ERZGunner_State3_Unknown;
                break;
            default: return;
        }
    }
}

void ERZGunner_StageLoad(void)
{
    ERZGunner->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomGunner.bin", SCOPE_STAGE);

    ERZGunner->hitbox1.left   = -8;
    ERZGunner->hitbox1.top    = -4;
    ERZGunner->hitbox1.right  = 4;
    ERZGunner->hitbox1.bottom = 26;

    ERZGunner->hitbox2.left   = -16;
    ERZGunner->hitbox2.top    = -32;
    ERZGunner->hitbox2.right  = 16;
    ERZGunner->hitbox2.bottom = -16;

    ERZGunner->hitbox3.left   = -8;
    ERZGunner->hitbox3.top    = -8;
    ERZGunner->hitbox3.right  = 8;
    ERZGunner->hitbox3.bottom = 8;
}

void ERZGunner_Explode(void)
{
    RSDK_THIS(ERZGunner);
    if (!(Zone->timer & 7)) {
        int x                = RSDK.Sin512(entity->rotation) << 11;
        int y                = RSDK.Cos512(entity->rotation) << 11;
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_LightningSpark, entity->position.x + x, entity->position.y - y);
        RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &debris->animator, true, 0);
        debris->velocity.x = RSDK.Sin512(entity->rotation) << 8;
        debris->velocity.y = RSDK.Sin512(entity->rotation) << 4;
        debris->drawOrder  = Zone->drawOrderLow;
        debris->timer      = 41;
    }
}

void ERZGunner_Explode2(void)
{
    RSDK_THIS(ERZGunner);

    if (Zone->timer & 3) {
        if ((Zone->timer & 3) == 2) {
            int x                = 0x600 * RSDK.Sin512(entity->rotation);
            int y                = 0x600 * RSDK.Cos512(entity->rotation);
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_LightningSpark, entity->position.x + x, entity->position.y - y);
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
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_LightningSpark, entity->position.x + x, entity->position.y - y);
        RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &debris->animator, true, 0);
        debris->velocity.x = RSDK.Sin512(entity->rotation) << 8;
        debris->velocity.y = RSDK.Sin512(entity->rotation) << 4;
        debris->drawOrder  = Zone->drawOrderLow;
        debris->drawFX     = FX_SCALE;
        debris->timer      = 41;
        debris->scale.y    = entity->scale.x;
    }
}

void ERZGunner_SpawnDust(void)
{
    RSDK_THIS(ERZGunner);

    for (int i = 0; i < 4; ++i) {
        int x            = entity->position.x + RSDK.Rand(-0x100000, 0x100000);
        int y            = entity->position.y + RSDK.Rand(-0x280000, -0x180000);
        EntityDust *dust = CREATE_ENTITY(Dust, NULL, x, y);
        dust->state      = Dust_State_DropDash;
        dust->drawOrder  = Zone->drawOrderHigh;
    }
}

void ERZGunner_HandleRotations(int angle)
{
    RSDK_THIS(ERZGunner);

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

void ERZGunner_CheckPlayerMissileCollisions(void)
{
    RSDK_THIS(ERZGunner);

    foreach_active(Player, player)
    {
        if (entity->animator4.frameID > 0 && player->velocity.y >= 0 && Player_CheckBadnikTouch(player, entity, &ERZGunner->hitbox2)
            && player->playerAnimator.animationID != ANI_HURT) {
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator4, true, 0);
            player->velocity.y = -0x60000;
            ERZGunner_SpawnDust();
            entity->state = ERZGunner_State1_Unknown3;
        }
        else {
            if (Player_CheckBadnikTouch(player, entity, &ERZGunner->hitbox1)) {
                int anim = player->playerAnimator.animationID;
                if (anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH) {
                    EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), entity->position.x, entity->position.y);
                    explosion->interaction     = false;
                    explosion->drawOrder       = Zone->drawOrderHigh;
                    if (entity->animator4.animationID == 12 && entity->animator4.frameID > 0)
                        ERZGunner_SpawnDust();
                    RSDK.PlaySfx(ERZKing->sfxExplosion2, false, 255);
                    destroyEntity(entity);
                    foreach_break;
                }
            }
        }
    }
}

void ERZGunner_CheckPlayerExplosionCollisions(void)
{
    foreach_active(Player, player)
    {
        foreach_active(Explosion, explosion)
        {
            if (explosion->animator.frameID <= 6) {
                if (Player_CheckCollisionTouch(player, explosion, &ERZGunner->hitbox3)) {
                    Player_CheckElementalHit(player, explosion, SHIELD_FIRE);
                }
            }
        }
    }
}

void ERZGunner_StateDraw_Unknown0(void)
{
    RSDK_THIS(ERZGunner);
    if (entity->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 128, 0, 128, 128);
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator2, NULL, false);
    }
}

void ERZGunner_StateDraw_Unknown1(void)
{
    RSDK_THIS(ERZGunner);
    EntityERZGunner *parent = (EntityERZGunner *)entity->parent;

    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, ((entity->posUnknown1.y + parent->position.y) >> 16) - RSDK_screens->position.y);

    Vector2 drawPos;
    drawPos.x = parent->position.x + entity->posUnknown1.x;
    drawPos.y = entity->position.y;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, RSDK_screens->height);
    drawPos.y = entity->posUnknown1.y + parent->position.y;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
}

void ERZGunner_StateDraw_Unknown2(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
    RSDK.DrawSprite(&entity->animator4, NULL, false);
}

void ERZGunner_State_Unknown1(void)
{
    RSDK_THIS(ERZGunner);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    entity->angle      = (entity->angle + 3) & 0xFF;
    entity->position.y = (RSDK.Sin256(entity->angle) << 11) + entity->posUnknown1.y;

    entity->position.x += (RSDK_screens->position.x - entity->posUnknown2.x) << 15;
    entity->posUnknown2 = RSDK_screens->position;
    RSDK.ProcessAnimation(&entity->animator2);

    entity->timer3 = 8;
    if (++entity->timer2 < 120 || RSDK.GetEntityCount(ERZGunner->objectID, true) >= 2) {
        ERZGunner_CheckPlayerExplosionCollisions();
    }
    else {
        if (++entity->timer == 2) {
            foreach_all(ERZKing, king) { king->active = ACTIVE_NORMAL; }
            destroyEntity(entity);
        }
        else {
            ERZGunner->value2 = 0;
            entity->timer2    = 0;
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 2, &entity->animator1, true, 0);
            entity->state = ERZGunner_State_Unknown2;
            ERZGunner_CheckPlayerExplosionCollisions();
        }
    }
}

void ERZGunner_State_Unknown2(void)
{
    RSDK_THIS(ERZGunner);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    entity->angle      = (entity->angle + 3) & 0xFF;
    entity->position.y = (RSDK.Sin256(entity->angle) << 11) + entity->posUnknown1.y;

    entity->position.x += (RSDK_screens->position.x - entity->posUnknown2.x) << 15;
    entity->posUnknown2 = RSDK_screens->position;
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);

    if (++entity->timer2 == 8) {
        EntityERZGunner *child = CREATE_ENTITY(ERZGunner, intToVoid(1), entity->position.x, entity->position.y);
        child->parent          = (Entity *)entity;
        child->posUnknown1.x   = ERZGunner->value1[entity->field_80] << 16;
        child->posUnknown1.y   = ERZGunner->value1[entity->field_80 + 1] << 16;
        child->position.x += child->posUnknown1.x;
        child->position.y += child->posUnknown1.y + 0x100000;
        entity->field_80 = (entity->field_80 - 14) & 0x1F;
    }

    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        entity->timer2 = 0;
        if (--entity->timer3 > 0) {
            if (!(entity->timer3 & 1))
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 2, &entity->animator1, true, 0);
            else
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 3, &entity->animator1, true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 0, &entity->animator1, true, 0);
            entity->state = ERZGunner_State_Unknown1;
        }
    }
    ERZGunner_CheckPlayerExplosionCollisions();
}

void ERZGunner_State1_Unknown1(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    entity->position.y -= 0x80000;
    if (++entity->timer2 == 60) {
        EntityERZGunner *parent = (EntityERZGunner *)entity->parent;

        entity->timer2 = 0;
        if ((ERZGunner->value2 & 3) == 3)
            entity->type = 4;
        else
            entity->type = (ERZGunner->value2 & 1) + 2;
        ++ERZGunner->value2;
        entity->drawOrder  = Zone->drawOrderLow;
        entity->position.y = (RSDK_screens->position.y - 64) << 16;

        bool32 flag = false;
        while (!flag) {
            flag               = true;
            entity->position.x = parent->position.x + RSDK.Rand(-0x1000000, 0x1000000);

            foreach_active(ERZGunner, gunner)
            {
                if (gunner != entity && gunner->type >= 2) {
                    int dist = abs(gunner->position.x - entity->position.x);
                    if (dist < 0x180000 && gunner->position.y - entity->position.y < 0x800000)
                        flag = false;
                }
            }
        }

        if (entity->type == 2) {
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 4, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 5, &entity->animator3, true, 0);
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 12, &entity->animator4, true, 0);
            entity->state     = ERZGunner_State1_Unknown2;
            entity->timer2    = RSDK.Rand(0, 2) << 8;
            entity->stateDraw = ERZGunner_StateDraw_Unknown2;
        }
        else if (entity->type == 3) {
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 6, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 7, &entity->animator3, true, 0);
            entity->state     = ERZGunner_State1_Unknown3;
            entity->stateDraw = ERZGunner_StateDraw_Unknown2;
        }
        else if (entity->type == 4) {
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 8, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 9, &entity->animator3, true, 0);
            entity->position.x = parent->position.x;
            entity->drawFX     = FX_ROTATE;
            entity->state      = ERZGunner_State1_Unknown4;
            if (!(Zone->timer & 1))
                entity->velocity.x = 0x40000;
            else
                entity->velocity.x = -0x40000;
            entity->stateDraw = ERZGunner_StateDraw_Unknown2;
        }
    }
}

void ERZGunner_State1_Unknown2(void)
{
    RSDK_THIS(ERZGunner);

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

    ERZGunner_CheckPlayerMissileCollisions();
    if (entity->objectID) {
        if (RSDK.ObjectTileCollision(RSDK_sceneInfo->entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true)) {
            if (entity->type == 3) {
                EntityERZGunner *child = CREATE_ENTITY(ERZGunner, intToVoid(5), entity->position.x, entity->position.y);
                if (entity->velocity.y > 0x20000)
                    child->velocity.x = 0x80000;
                else
                    child->velocity.x = 0x40000;
                destroyEntity(entity);
            }
            else {
                CREATE_ENTITY(ERZGunner, intToVoid(6), entity->position.x, entity->position.y);
                destroyEntity(entity);
            }
        }
    }
}

void ERZGunner_State1_Unknown3(void)
{
    RSDK_THIS(ERZGunner);
    RSDK.ProcessAnimation(&entity->animator3);

    if (entity->velocity.y < 0x40000)
        entity->velocity.y += 0x1800;
    entity->position.y += entity->velocity.y;

    ERZGunner_CheckPlayerMissileCollisions();
    if (entity->objectID) {
        if (RSDK.ObjectTileCollision(RSDK_sceneInfo->entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true)) {
            if (entity->type == 3) {
                EntityERZGunner *child = CREATE_ENTITY(ERZGunner, intToVoid(5), entity->position.x, entity->position.y);
                if (entity->velocity.y > 0x20000)
                    child->velocity.x = 0x80000;
                else
                    child->velocity.x = 0x40000;
                destroyEntity(entity);
            }
            else {
                CREATE_ENTITY(ERZGunner, intToVoid(6), entity->position.x, entity->position.y);
                destroyEntity(entity);
            }
        }
    }
}

void ERZGunner_State1_Unknown4(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.ProcessAnimation(&entity->animator3);
    EntityERZGunner *parent = (EntityERZGunner *)entity->parent;

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
    ERZGunner_Explode();

    int angle = RSDK.ATan2(entity->velocity.y, -entity->velocity.x);
    ERZGunner_HandleRotations(2 * angle);
    if (entity->timer2 == 320) {
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), entity->position.x, entity->position.y);
        explosion->interaction     = false;
        explosion->drawOrder       = Zone->drawOrderHigh;
        destroyEntity(entity);
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, entity, &ERZGunner->hitbox3)) {
                entity->angle     = entity->rotation << 8;
                entity->timer2    = 0;
                entity->drawFX    = FX_SCALE | FX_ROTATE;
                entity->groundVel = 0x4000;
                entity->scale.x   = 0x200;
                entity->scale.y   = 0x200;
                entity->state     = ERZGunner_State1_Unknown5;
                RSDK.PlaySfx(ERZKing->sfxHit, false, 255);
            }
        }
    }
}

void ERZGunner_State1_Unknown5(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.ProcessAnimation(&entity->animator3);

    entity->angle += entity->groundVel;
    entity->rotation = entity->angle >> 8;
    entity->groundVel -= (entity->groundVel >> 3);

    ERZGunner_Explode();
    if (entity->groundVel < 128)
        entity->state = ERZGunner_State1_Unknown6;
}

void ERZGunner_State1_Unknown6(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.ProcessAnimation(&entity->animator3);
    EntityERZGunner *parent = (EntityERZGunner *)entity->parent;

    int angle = RSDK.ATan2((parent->position.y - entity->position.y) >> 16, -((parent->position.x - entity->position.x) >> 16));
    ERZGunner_HandleRotations(2 * angle);
    entity->rotation &= 0x1FF;
    entity->position.x -= RSDK.Sin512(entity->rotation) << 9;
    entity->scale.x -= 6;
    entity->position.y += RSDK.Cos512(entity->rotation) << 9;
    entity->scale.y = entity->scale.x;

    ERZGunner_Explode2();
    if (entity->scale.x < 128) {
        RSDK.PlaySfx(ERZKing->sfxHit, false, 255);
        parent->invincibilityTimer = 48;
        entity->position.x         = parent->position.x;
        entity->position.y         = parent->position.y;
        entity->visible            = false;
        entity->state              = ERZGunner_State1_Unknown7;
    }
}

void ERZGunner_State1_Unknown7(void)
{
    RSDK_THIS(ERZGunner);

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

void ERZGunner_State2_Unknown(void)
{
    RSDK_THIS(ERZGunner);

    if (!(Zone->timer & 3)) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), entity->posUnknown1.x - entity->field_84, entity->position.y);
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), entity->posUnknown1.x + entity->field_84, entity->position.y);
    }
    entity->field_84 += entity->velocity.x;
    if (++entity->timer2 == 16)
        destroyEntity(entity);
}

void ERZGunner_State3_Unknown(void)
{
    RSDK_THIS(ERZGunner);

    if (!(Zone->timer & 3))
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), entity->position.x, entity->position.y);
    entity->position.y -= 0x40000;
    if (++entity->timer2 == 16)
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void ERZGunner_EditorDraw(void) {}

void ERZGunner_EditorLoad(void) {}
#endif

void ERZGunner_Serialize(void) {}
