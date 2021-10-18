#include "SonicMania.h"

ObjectPhantomMissile *PhantomMissile;

void PhantomMissile_Update(void) {}

void PhantomMissile_LateUpdate(void)
{
    RSDK_THIS(PhantomMissile);
    StateMachine_Run(entity->state);
}

void PhantomMissile_StaticUpdate(void) {}

void PhantomMissile_Draw(void)
{
    RSDK_THIS(PhantomMissile);
    RSDK.DrawSprite(&entity->animator0, NULL, false);
    RSDK.DrawSprite(&entity->animator1, &entity->drawPos, false);

    if (Zone->timer & 0x10)
        RSDK.DrawSprite(&entity->animator3, &entity->drawPos, false);
    else
        RSDK.DrawSprite(&entity->animator2, &entity->drawPos, false);
}

void PhantomMissile_Create(void *data)
{
    RSDK_THIS(PhantomMissile);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->drawFX        = FX_ROTATE;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->state         = PhantomMissile_Unknown3;
        RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 0, &entity->animator0, true, 0);
    }
}

void PhantomMissile_StageLoad(void)
{
    PhantomMissile->aniFrames = RSDK.LoadSpriteAnimation("Phantom/EggMissile.bin", SCOPE_STAGE);

    PhantomMissile->hitbox.left   = -16;
    PhantomMissile->hitbox.top    = -12;
    PhantomMissile->hitbox.right  = 16;
    PhantomMissile->hitbox.bottom = 12;
}

void PhantomMissile_Unknown1(void)
{
    RSDK_THIS(PhantomMissile);

    int distance = 0x7FFFFFFF;
    for (int p = SLOT_PLAYER1; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
        if (player->sidekick == true && player->stateInput != Player_ProcessP2Input_Player) {
            int storeX         = entity->position.x;
            int storeY         = entity->position.y;
            entity->position.x = entity->drawPos.x;
            entity->position.y = entity->drawPos.y;

            while (!RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, 16)) {
                entity->position.y += 0x100000;
            }

            int y              = entity->position.y;
            entity->position.x = storeX;
            entity->position.y = storeY;
            entity->drawPos.y  = y;
            foreach_break;
        }

        int rx   = (entity->position.x - player->position.x) >> 16;
        int ry   = (entity->position.y - player->position.y) >> 16;
        int dist = rx * rx + ry * ry;
        if (dist < distance) {
            distance          = dist;
            entity->drawPos.x = player->position.x;
            entity->drawPos.y = player->position.y;
        }
    }
}

void PhantomMissile_Unknown2(void)
{
    RSDK_THIS(PhantomMissile);

    if (!(Zone->timer & 3)) {
        int x            = RSDK.Sin512(entity->rotation) << 11;
        int y            = RSDK.Cos512(entity->rotation) << 11;
        EntityDust *dust = CREATE_ENTITY(Dust, NULL, entity->position.x - x, entity->position.y + y);
        dust->state      = Dust_State_DropDash;
        dust->drawOrder  = Zone->drawOrderHigh;
        dust->inkEffect  = INK_BLEND;
    }
}

void PhantomMissile_Unknown3(void)
{
    RSDK_THIS(PhantomMissile);

    if (entity->parent) {
        entity->position.x = entity->parent->position.x;
        entity->position.y = entity->parent->position.y;
        entity->field_60   = (entity->field_60 + 6) & 0xFF;
        int off            = 8 * RSDK.Sin256(entity->field_60) + 0x2000;

        entity->position.x += off * RSDK.Sin256(entity->angle);
        entity->position.y -= off * RSDK.Cos256(entity->angle);
        entity->rotation = 2 * entity->angle;

        if (entity->timer > 0) {
            if (!--entity->timer) {
                entity->velocity.x = 0x300 * RSDK.Sin256(entity->angle) + entity->parent->velocity.x;
                entity->velocity.y = -0x300 * RSDK.Cos256(entity->angle) + entity->parent->velocity.y;
                entity->timer      = 8;
                entity->state      = PhantomMissile_Unknown4;
            }
        }
    }
}

void PhantomMissile_Unknown4(void)
{
    RSDK_THIS(PhantomMissile);

    entity->velocity.y += 0x3800;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (!--entity->timer) {
        entity->field_64 = 0x400;
        entity->state    = PhantomMissile_Unknown5;
        RSDK.PlaySfx(PhantomEgg->sfxRocketJet, false, 255);
    }
}

void PhantomMissile_Unknown5(void)
{
    RSDK_THIS(PhantomMissile);

    RSDK.ProcessAnimation(&entity->animator0);
    if (entity->field_64 < 0x600)
        entity->field_64 += 4;

    entity->position.x += entity->field_64 * RSDK.Sin256(entity->angle);
    entity->position.y -= entity->field_64 * RSDK.Cos256(entity->angle);

    if (++entity->timer == 24) {
        entity->timer = 32;
        entity->field_64 >>= 1;
        PhantomMissile_Unknown1();
        RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 1, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 2, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 3, &entity->animator3, true, entity->field_6C);
        entity->drawOrder = Zone->drawOrderHigh;
        entity->state     = PhantomMissile_Unknown6;
    }
    PhantomMissile_Unknown2();
}

void PhantomMissile_Unknown6(void)
{
    RSDK_THIS(PhantomMissile);

    RSDK.ProcessAnimation(&entity->animator0);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);

    if (entity->field_64 < 0x800)
        entity->field_64 += 4;
    if (entity->timer > 4)
        entity->timer--;

    int shift = entity->timer >> 2;
    int rx    = (entity->drawPos.x - entity->position.x) >> 16;
    int ry    = (entity->drawPos.y - entity->position.y) >> 16;
    int angle = RSDK.ATan2(-ry, rx);
    int rot   = 2 * angle - entity->rotation;

    if (abs(2 * angle - entity->rotation) >= abs(rot - 0x200)) {
        if (abs(rot - 0x200) < abs(rot + 0x200)) {
            entity->rotation += ((rot - 0x200) >> shift);
        }
        else {
            entity->rotation += ((rot + 0x200) >> shift);
        }
    }
    else {
        if (abs(2 * angle - entity->rotation) < abs(rot + 0x200)) {
            entity->rotation += (rot >> shift);
        }
        else {
            entity->rotation += ((rot + 0x200) >> shift);
        }
    }

    entity->rotation &= 0x1FF;
    entity->position.x += entity->field_64 * RSDK.Sin512(entity->rotation);
    entity->position.y -= entity->field_64 * RSDK.Cos512(entity->rotation);
    if (rx * rx + ry * ry < 64) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), entity->drawPos.x, entity->drawPos.y - 0x80000)->drawOrder = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 0, &entity->animator0, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 1, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator3, true, 0);
        entity->timer     = 0;
        entity->field_68  = 0;
        entity->drawOrder = Zone->drawOrderLow;
        entity->visible   = 0;
        entity->state     = PhantomMissile_Unknown7;
        Camera_ShakeScreen(0, 0, 3);
        RSDK.PlaySfx(PhantomEgg->sfxMissile, false, 255);
    }
    PhantomMissile_Unknown2();
}

void PhantomMissile_Unknown7(void)
{
    RSDK_THIS(PhantomMissile);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &PhantomMissile->hitbox) && player->shield != SHIELD_FIRE) {
            Player_CheckHit(player, entity);
        }
    }
    if (++entity->timer >= 16) {
        if (entity->parent) {
            entity->position.x = entity->parent->position.x;
            entity->position.y = entity->parent->position.y;
            entity->timer      = 0;
            entity->visible    = true;
            entity->state      = PhantomMissile_Unknown8;
        }
    }
}

void PhantomMissile_Unknown8(void)
{
    RSDK_THIS(PhantomMissile);

    if (entity->parent) {
        entity->position.x = entity->parent->position.x;
        entity->position.y = entity->parent->position.y;

        if (entity->field_68 >= 0x2000) {
            entity->field_60 = 0;
            entity->state    = PhantomMissile_Unknown3;
        }
        else {
            entity->field_68 += 0x100;
        }

        entity->position.x += entity->field_68 * RSDK.Sin256(entity->angle);
        entity->position.y -= entity->field_68 * RSDK.Cos256(entity->angle);
        entity->rotation = (2 * entity->angle);
    }
}

void PhantomMissile_Unknown9(void)
{
    RSDK_THIS(PhantomMissile);

    entity->velocity.y += 0x3800;
    entity->position.y += entity->velocity.y;
    entity->rotation = (entity->rotation + entity->groundVel) & 0x1FF;
    entity->position.x += entity->velocity.x;

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

void PhantomMissile_EditorDraw(void) {}

void PhantomMissile_EditorLoad(void) {}

void PhantomMissile_Serialize(void) {}
