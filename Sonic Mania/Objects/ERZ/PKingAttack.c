#include "SonicMania.h"

ObjectPKingAttack *PKingAttack;

void PKingAttack_Update(void)
{
    RSDK_THIS(PKingAttack);
    StateMachine_Run(entity->state);
}

void PKingAttack_LateUpdate(void) {}

void PKingAttack_StaticUpdate(void) {}

void PKingAttack_Draw(void)
{
    RSDK_THIS(PKingAttack);

    if (entity->type == 2)
        RSDK.DrawCircle(entity->position.x, entity->position.y, 32, 0, 255, INK_LOOKUP, false);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void PKingAttack_Create(void *data)
{
    RSDK_THIS(PKingAttack);

    if (!RSDK_sceneInfo->inEditor) {
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->type          = voidToInt(data);
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;

        switch (entity->type) {
            default: break;
            case 1:
                entity->drawFX  = FX_SCALE;
                entity->visible = true;
                RSDK.SetSpriteAnimation(PKingAttack->aniFrames, 10, &entity->animator, true, RSDK.Rand(0, 6));
                entity->state         = PKingAttack_Unknown2;
                entity->hitbox.left   = -10;
                entity->hitbox.top    = -10;
                entity->hitbox.right  = 10;
                entity->hitbox.bottom = 10;
                break;
            case 3:
                entity->drawOrder = Zone->drawOrderLow;
                entity->visible   = true;
                entity->inkEffect = INK_ADD;
                entity->alpha     = 0xC0;
                RSDK.SetSpriteAnimation(PKingAttack->aniFrames, 12, &entity->animator, true, 0);
                entity->state = PKingAttack_Unknown5;
                break;
            case 6:
                entity->drawOrder = Zone->drawOrderLow;
                entity->drawFX    = FX_ROTATE;
                entity->visible   = true;
                RSDK.SetSpriteAnimation(PKingAttack->aniFrames, 11, &entity->animator, true, 0);
                entity->state         = PKingAttack_Unknown6;
                entity->hitbox.left   = -4;
                entity->hitbox.top    = -4;
                entity->hitbox.right  = 4;
                entity->hitbox.bottom = 4;
                break;
        }
    }
}

void PKingAttack_StageLoad(void)
{
    PKingAttack->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomKing.bin", SCOPE_STAGE);
    PKingAttack->sfxPulse  = RSDK.GetSFX("Stage/ElecPulse.wav");
}

void PKingAttack_CheckPlayerCollisions(void)
{
    RSDK_THIS(PKingAttack);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
            if (player->superState == SUPERSTATE_SUPER) {
                if (!player->blinkTimer) {
                    int angle = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
                    if (entity->state == PKingAttack_Unknown4) {
                        player->blinkTimer = 120;
                        Player_LoseRings(player, minVal(player->rings, 8), player->collisionPlane);
                        player->rings -= minVal(player->rings, 8);
                        RSDK.PlaySfx(Player->sfxLoseRings, false, 255);
                    }
                    else {
                        RSDK.PlaySfx(PKingAttack->sfxPulse, false, 255);
                        destroyEntity(entity);
                    }
                    player->rotation   = 0;
                    player->velocity.x = RSDK.Cos256(angle) << 10;
                    player->velocity.y = RSDK.Sin256(angle) << 10;
                    player->groundVel  = player->velocity.x;
                    player->state      = ERZStart_State_PlayerRebound;
                }
            }
            else {
                Player_CheckHit(player, entity);
            }
        }
    }
}

void PKingAttack_Unknown2(void)
{
    RSDK_THIS(PKingAttack);
    RSDK.ProcessAnimation(&entity->animator);

    entity->angle = (entity->angle + 12) & 0x3FF;
    if (entity->scale.x < 512) {
        entity->scale.x += 0x20;
        entity->scale.y = entity->scale.x;
    }

    entity->velocity.x = entity->position.x;
    entity->velocity.y = entity->position.y;
    entity->position.x = (RSDK.Cos1024(entity->angle) << 12) + entity->target->position.x;
    entity->position.y = (RSDK.Sin1024(entity->angle) << 12) + entity->target->position.y;
    entity->velocity.x = entity->position.x - entity->velocity.x;
    entity->velocity.y = entity->position.y - entity->velocity.y;

    PKingAttack_CheckPlayerCollisions();
    if (++entity->timer == 120) {
        entity->timer       = 0;
        entity->targetPos.x = entity->target->position.x;
        entity->targetPos.y = entity->target->position.y;
        entity->state       = PKingAttack_Unknown3;
    }
}

void PKingAttack_Unknown3(void)
{
    RSDK_THIS(PKingAttack);

    RSDK.ProcessAnimation(&entity->animator);

    entity->position.x += entity->target->position.x - entity->targetPos.x;
    entity->position.y += entity->target->position.y - entity->targetPos.y;
    if (entity->position.x <= entity->target->position.x) {
        if (entity->position.x < entity->target->position.x)
            entity->velocity.x += 0x4000;
    }
    else {
        entity->velocity.x -= 0x4000;
    }

    if (entity->position.y <= entity->target->position.y) {
        if (entity->position.y < entity->target->position.y)
            entity->velocity.y += 0x3800;
    }
    else {
        entity->velocity.y -= 0x3800;
    }

    entity->velocity.x = clampVal(entity->velocity.x, -0x50000, 0x50000);
    entity->velocity.y = clampVal(entity->velocity.y, -0x50000, 0x50000);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->targetPos.x = entity->target->position.x;
    entity->targetPos.y = entity->target->position.y;

    entity->angle = (entity->angle + 12) & 0x3FF;
    int inc       = (384 - entity->scale.x - (RSDK.Sin1024(entity->angle) >> 3)) >> 3;
    entity->scale.x += inc;
    entity->scale.y = entity->scale.x;
    PKingAttack_CheckPlayerCollisions();

    if (entity->angle < 0x200)
        entity->drawOrder = Zone->drawOrderLow - 1;
    else
        entity->drawOrder = Zone->drawOrderLow;
}

void PKingAttack_Unknown4(void)
{
    RSDK_THIS(PKingAttack);

    RSDK.ProcessAnimation(&entity->animator);
    ++entity->timer;
    if (!(entity->timer & 3))
        CREATE_ENTITY(PKingAttack, intToVoid(3), entity->position.x, entity->position.y);

    if (entity->scale.x < 512) {
        entity->scale.x += 0x20;
        entity->scale.y = entity->scale.x;
    }

    entity->velocity.x = entity->velocity.x + ((entity->field_70.x - entity->velocity.x) >> 3);
    entity->velocity.y = entity->velocity.y + ((entity->field_70.y - entity->velocity.y) >> 3);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    PKingAttack_CheckPlayerCollisions();

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

void PKingAttack_Unknown5(void)
{
    RSDK_THIS(PKingAttack);

    if (entity->timer >= 8) {
        entity->alpha -= 8;
        if (entity->alpha <= 0)
            destroyEntity(entity);
    }
    else {
        entity->timer++;
    }
}

void PKingAttack_Unknown6(void)
{
    RSDK_THIS(PKingAttack);

    RSDK.ProcessAnimation(&entity->animator);
    entity->rotation = (entity->rotation + 16) & 0x1FF;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    PKingAttack_CheckPlayerCollisions();
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x40000, true)) {
        entity->position.y += 0x40000;
        destroyEntity(entity);
    }

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void PKingAttack_EditorDraw(void)
{
    RSDK_THIS(PKingAttack);

    entity->type      = 1;
    entity->inkEffect = INK_ADD;
    entity->alpha     = 0xC0;
    RSDK.SetSpriteAnimation(PKingAttack->aniFrames, 12, &entity->animator, true, 0);

    PKingAttack_Draw();
}

void PKingAttack_EditorLoad(void) { PKingAttack->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomKing.bin", SCOPE_STAGE); }
#endif

void PKingAttack_Serialize(void) {}
