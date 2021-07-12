#include "../SonicMania.h"

ObjectTurntable *Turntable;

void Turntable_Update(void)
{
    RSDK_THIS(Turntable);
    entity->angle += entity->angleVel;
    if (entity->angle < 0)
        entity->angle += 0x400;
    entity->angle %= 0x3FF;
    foreach_active(Player, player)
    {
        int pID = RSDK.GetEntityID(player);
        if (!((1 << pID) & entity->activePlayers)) {
            if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == 1) {
#if RETRO_USE_PLUS
                if (player->state == Player_State_MightyHammerDrop || player->state == Player_State_BubbleBounce)
                    continue;
#else
                if (player->state == Player_State_BubbleBounce)
                    continue;
#endif
                entity->active          = ACTIVE_NORMAL;
                entity->activePlayers   = entity->activePlayers | (1 << pID);
                player->nextGroundState = StateMachine_None;
                player->nextAirState    = StateMachine_None;
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                player->groundVel       = 0;
                player->onGround        = 1;
                player->state           = Player_State_None;

                int dist = abs(player->position.x - entity->position.x) >> 0x10;

                if (dist >= 0x10) {
                    if (player->position.x <= entity->position.x) {
                        entity->field_78[pID] = 512;
                        entity->field_98[pID] = 12;
                    }
                    else {
                        entity->field_78[pID] = 0;
                        entity->field_98[pID] = 0;
                    }
                }
                else {
                    int distY = 16 * (0x10000 - dist / 16);
                    int angY  = -16 * (0x10000 - dist / 16);
                    if (player->drawOrder != Zone->playerDrawLow)
                        angY = distY;
                    int distX = player->position.x - entity->position.x;
                    int angX  = -distX;
                    if (player->position.x >= entity->position.x)
                        angX = distX;
                    dist                  = 0x10;
                    entity->field_78[pID] = 4 * RSDK.ATan2(angX, angY);
                    entity->field_98[pID] = 0;
                }
                entity->field_88[pID] = dist;
                RSDK.SetSpriteAnimation(player->spriteIndex, 34, &player->playerAnimator, true, entity->field_98[pID]);
                Hitbox *hitbox     = Player_GetHitbox(player);
                player->position.y = entity->position.y - (hitbox->bottom << 16) - (entity->size.y >> 1);
            }
        }
        if ((1 << pID) & entity->activePlayers) {
            player->velocity.x    = 0;
            player->velocity.y    = 0;
            player->groundVel     = 0;
            entity->field_78[pID] = (entity->angleVel + entity->field_78[pID]) & 0x3FF;
            player->position.x    = entity->position.x;
            player->position.x += (RSDK.Cos1024(entity->field_78[pID]) << 6) * entity->field_88[pID];

            int frame = 0;
            if (player->direction)
                frame = 24 - entity->field_78[pID] / 42;
            else
                frame = entity->field_78[pID] / 42 % 24;

            if (entity->field_78[pID] < 512)
                player->drawOrder = Zone->playerDrawHigh;
            else
                player->drawOrder = Zone->playerDrawLow;
            player->playerAnimator.frameID = (entity->field_98[pID] + frame) % -24;
            if (player->jumpPress) {
                Player_StartJump(player);
            }
            else if (player->playerAnimator.animationID == ANI_TWISTER && player->state == Player_State_None) {
                continue;
            }
            entity->activePlayers &= ~(1 << pID);
        }
    }
}

void Turntable_LateUpdate(void) {}

void Turntable_StaticUpdate(void) {}

void Turntable_Draw(void)
{
    RSDK_THIS(Turntable);
    RSDK.SetSpriteAnimation(Turntable->aniFrames, entity->type, &entity->animator, true, 13 - (entity->angle >> 1) / 9 % 14);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Turntable_Create(void *data)
{
    RSDK_THIS(Turntable);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->origin.x      = entity->position.x;
    entity->origin.y      = entity->position.y;
    entity->visible       = true;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (!entity->angleVel)
        entity->angleVel = 6;
    Turntable_SetupSize();
    entity->hitbox.right  = (entity->size.x >> 17);
    entity->hitbox.left   = -(entity->size.x >> 17);
    entity->hitbox.bottom = (entity->size.y >> 17);
    entity->hitbox.top    = -(entity->size.y >> 17);
    RSDK.SetSpriteAnimation(Turntable->aniFrames, 0, &entity->animator, true, 6);
}

void Turntable_StageLoad(void) { Turntable->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Turntable.bin", SCOPE_STAGE); }

void Turntable_SetupSize(void)
{
    RSDK_THIS(Turntable);
    RSDK.SetSpriteAnimation(Turntable->aniFrames, entity->type, &entity->animator, true, 0);

    switch (entity->type) {
        case 0:
            entity->size.x = 0x6A0000;
            entity->size.y = 0x200000;
            break;
        case 1:
            entity->size.x = 0xD40000;
            entity->size.y = 0x300000;
            break;
    }
}

void Turntable_EditorDraw(void) {}

void Turntable_EditorLoad(void) {}

void Turntable_Serialize(void)
{
    RSDK_EDITABLE_VAR(Turntable, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Turntable, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Turntable, VAR_INT32, angleVel);
}
