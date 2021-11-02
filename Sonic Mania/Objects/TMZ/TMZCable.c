#include "SonicMania.h"

ObjectTMZCable *TMZCable;

void TMZCable_Update(void)
{
    RSDK_THIS(TMZCable);

    StateMachine_Run(entity->state);
}

void TMZCable_LateUpdate(void) {}

void TMZCable_StaticUpdate(void) {}

void TMZCable_Draw(void)
{
    RSDK_THIS(TMZCable);

    for (int32 i = 0; i < 8; ++i) {
        if (!entity->drawFlags[i])
            RSDK.DrawSprite(&entity->animator, &entity->drawPos[i], false);
    }
}

void TMZCable_Create(void *data)
{
    RSDK_THIS(TMZCable);

    if (!RSDK_sceneInfo->inEditor) {
        entity->visible   = true;
        entity->drawOrder = Zone->drawOrderLow;
        entity->cableID &= 3;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        switch (entity->cableID) {
            case 0:
                entity->field_64.x = -0x1C0000;
                entity->field_64.y = 0x100000;
                entity->angle      = 0;
                break;
            case 1:
                entity->field_64.x = 0x1C0000;
                entity->field_64.y = 0x100000;
                entity->angle      = 64;
                break;
            case 2:
                entity->field_64.x = -0x1C0000;
                entity->field_64.y = -0x100000;
                entity->angle      = 128;
                break;
            case 3:
                entity->field_64.x = 0x1C0000;
                entity->field_64.y = -0x100000;
                entity->angle      = 0xC0;
                break;
            default: break;
        }
        entity->posPtr = (Vector2 *)data;
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 9, &entity->animator, true, 0);
    }
}

void TMZCable_StageLoad(void)
{
    TMZCable->hitbox.left   = -10;
    TMZCable->hitbox.top    = -10;
    TMZCable->hitbox.right  = 10;
    TMZCable->hitbox.bottom = 10;
}

void TMZCable_Unknown1(void)
{
    RSDK_THIS(TMZCable);

    if (entity->posPtr) {
        int32 x           = entity->posPtr->x + entity->field_64.x;
        int32 y           = entity->posPtr->y + entity->field_64.y;
        int32 entityAngle = entity->angle;
        int32 angle       = RSDK.ATan2((entity->position.x - x) >> 16, (entity->position.y - y) >> 16) + 64;

        int32 id = 0;
        for (int32 i = 0; i < 144; i += 18) {
            int32 off = (RSDK.Sin256(entityAngle) * RSDK.Sin256(i)) >> 5;

            entity->drawPos[0].x = x + 32 * id * ((entity->position.x - x) >> 8);
            entity->drawPos[0].y = y + 32 * id * ((entity->position.y - y) >> 8);
            entity->drawPos[0].x += off * RSDK.Cos256(angle);
            entity->drawPos[0].y += off * RSDK.Sin256(angle);

            entityAngle += 32;
            id++;
        }
    }
}

void TMZCable_Unknown2(void)
{
    RSDK_THIS(TMZCable);

    TMZCable_Unknown1();
    entity->angle = (entity->angle + 4) & 0xFF;
}

void TMZCable_Unknown3(void)
{
    RSDK_THIS(TMZCable);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 11, &entity->animator, true, RSDK.Rand(0, 8));
        entity->state = TMZCable_Unknown4;
    }
}

void TMZCable_Unknown4(void)
{
    RSDK_THIS(TMZCable);

    RSDK.ProcessAnimation(&entity->animator);
    int32 storeX = entity->position.x;
    int32 storeY = entity->position.y;

    for (int32 i = 1; i < 8; ++i) {
        entity->position.x = entity->drawPos[i].x;
        entity->position.y = entity->drawPos[i].y;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &TMZCable->hitbox)) {
                Player_CheckHit(player, entity);
            }
        }
    }

    entity->position.x = storeX;
    entity->position.y = storeY;
    if (++entity->timer == 120) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 12, &entity->animator, true, 0);
        entity->state = TMZCable_Unknown5;
    }
}

void TMZCable_Unknown5(void)
{
    RSDK_THIS(TMZCable);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 9, &entity->animator, true, 0);
        entity->state = TMZCable_Unknown2;
    }
}

void TMZCable_Unknown6(void)
{
    RSDK_THIS(TMZCable);
    TMZCable_Unknown1();

    int32 id = entity->timer >> 5;
    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(PhantomEgg->sfxExplosion2, false, 255);
        if (Zone->timer & 4) {
            int32 y = entity->drawPos[id].y + RSDK.Rand(-0x100000, 0x100000);
            int32 x = entity->drawPos[id].x + RSDK.Rand(-0x100000, 0x100000);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }
    entity->timer += 3;
    if (entity->timer < 256) {
        int32 timer = entity->timer & 0x1F;
        if (timer >= 29) {
            entity->drawFlags[id] = true;

            EntityDebris *debris = CREATE_ENTITY(Debris, NULL, entity->drawPos[id].x, entity->drawPos[id].y);
            debris->state        = Debris_State_Fall;
            debris->gravity      = 0x4000;
            debris->velocity.x   = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y   = RSDK.Rand(-0x40000, -0x10000);
            debris->drawOrder    = Zone->drawOrderLow;
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 9, &debris->animator, true, 0);
        }
    }
    else {
        destroyEntity(entity);
    }
}

#if RETRO_INCLUDE_EDITOR
void TMZCable_EditorDraw(void)
{
    RSDK_THIS(TMZCable);
    entity->cableID &= 3;
    switch (entity->cableID) {
        case 0:
            entity->field_64.x = -0x1C0000;
            entity->field_64.y = 0x100000;
            entity->angle      = 0;
            break;
        case 1:
            entity->field_64.x = 0x1C0000;
            entity->field_64.y = 0x100000;
            entity->angle      = 64;
            break;
        case 2:
            entity->field_64.x = -0x1C0000;
            entity->field_64.y = -0x100000;
            entity->angle      = 128;
            break;
        case 3:
            entity->field_64.x = 0x1C0000;
            entity->field_64.y = -0x100000;
            entity->angle      = 0xC0;
            break;
        default: break;
    }

    RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 9, &entity->animator, true, 0);

    TMZCable_Draw();
}

void TMZCable_EditorLoad(void) {}
#endif

void TMZCable_Serialize(void) { RSDK_EDITABLE_VAR(TMZCable, VAR_ENUM, cableID); }
