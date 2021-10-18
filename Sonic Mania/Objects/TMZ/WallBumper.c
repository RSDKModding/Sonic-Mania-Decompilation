#include "SonicMania.h"

ObjectWallBumper *WallBumper;

void WallBumper_Update(void) { WallBumper_HandleInteractions(); }

void WallBumper_LateUpdate(void) {}

void WallBumper_StaticUpdate(void) {}

void WallBumper_Draw(void)
{
    RSDK_THIS(WallBumper);
    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;

    if (entity->type) {
        entity->direction ^= entity->reverse;
        drawPos.x -= entity->size << 20;
        for (int32 i = 0; i < entity->size; ++i) {
            RSDK.DrawSprite(&entity->animator, &drawPos, false);
            drawPos.x += 0x200000;
        }
        entity->direction ^= entity->reverse;
    }
    else {
        entity->direction ^= FLIP_Y * entity->reverse;
        drawPos.y -= entity->size << 20;
        for (int32 i = 0; i < entity->size; ++i) {
            RSDK.DrawSprite(&entity->animator, &drawPos, false);
            drawPos.y += 0x200000;
        }
        entity->direction ^= FLIP_Y * entity->reverse;
    }
}

void WallBumper_Create(void *data)
{
    RSDK_THIS(WallBumper);
    entity->drawFX        = FX_FLIP;
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x400000;
    entity->updateRange.y = 0x400000;
    if (!entity->type) {
        entity->updateRange.y = (entity->size + 4) << 20;
        entity->hitbox.left   = 0;
        entity->hitbox.top    = -16 - (16 * entity->size);
        entity->hitbox.right  = 8;
        entity->hitbox.bottom = (16 * entity->size) - 16;
    }
    else {
        entity->direction *= FLIP_Y;
        entity->updateRange.x = (entity->size + 4) << 20;
        entity->hitbox.left   = -16 - (16 * entity->size);
        entity->hitbox.top    = 0;
        entity->hitbox.right  = (16 * entity->size) - 16;
        entity->hitbox.bottom = 8;
    }
    RSDK.SetSpriteAnimation(WallBumper->aniFrames, entity->type, &entity->animator, true, 0);
}

void WallBumper_StageLoad(void)
{
    if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2"))
        WallBumper->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/WallBumper.bin", SCOPE_STAGE);
    WallBumper->sfxBouncer = RSDK.GetSFX("TMZ1/Bouncer.wav");

    DEBUGMODE_ADD_OBJ(WallBumper);
}

void WallBumper_DebugSpawn(void)
{
    RSDK_THIS(WallBumper);
    CREATE_ENTITY(WallBumper, NULL, entity->position.x, entity->position.y);
}

void WallBumper_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(WallBumper->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void WallBumper_HandleInteractions(void)
{
    RSDK_THIS(WallBumper);

    foreach_active(Player, player)
    {
        if (player->playerAnimator.animationID != ANI_HURT && Player_CheckBadnikTouch(player, entity, &entity->hitbox)) {
            if (entity->type) {
                if (entity->direction) {
                    if (player->velocity.y < 0)
                        continue;
                    player->velocity.y = -0x80000;
                }
                else {
                    if (player->velocity.y > 0)
                        continue;
                    player->velocity.y = 0x80000;
                }
                if (entity->reverse) {
                    player->velocity.x = -0x80000;
                    player->groundVel  = -0x80000;
                }
                else {
                    player->velocity.x = 0x80000;
                    player->groundVel  = 0x80000;
                }
            }
            else {
                if (entity->direction) {
                    if (player->velocity.x < 0)
                        continue;
                    player->velocity.x = -0x80000;
                    player->groundVel  = -0x80000;
                }
                else {
                    if (player->velocity.x > 0)
                        continue;
                    player->velocity.x = 0x80000;
                    player->groundVel  = 0x80000;
                }

                if (!entity->reverse)
                    player->velocity.y = -0x80000;
                else
                    player->velocity.y = 0x80000;
            }
            RSDK.PlaySfx(WallBumper->sfxBouncer, false, 255);
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimator, false, 0);

            if (player->state == Player_State_FlyCarried) {
                RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;
            }
            player->jumpAbility    = 0;
            player->onGround       = false;
            player->tileCollisions = true;
            player->onGround       = false;
            player->state          = Player_State_Air;
            if (entity->destructible) {
                destroyEntity(entity);
                foreach_break;
            }
        }
    }
}

void WallBumper_EditorDraw(void) {}

void WallBumper_EditorLoad(void) {}

void WallBumper_Serialize(void)
{
    RSDK_EDITABLE_VAR(WallBumper, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(WallBumper, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(WallBumper, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(WallBumper, VAR_BOOL, reverse);
    RSDK_EDITABLE_VAR(WallBumper, VAR_BOOL, destructible);
}
