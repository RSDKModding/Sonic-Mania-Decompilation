#include "SonicMania.h"

ObjectJacobsLadder *JacobsLadder;

void JacobsLadder_Update(void)
{
    RSDK_THIS(JacobsLadder);
    entity->field_84  = 0;
    entity->field_80  = false;
    entity->direction = entity->flip ? FLIP_Y : FLIP_NONE;

    int32 val = (entity->intervalOffset + Zone->timer) % entity->interval;
    if (val >= entity->duration) {
        if (val < entity->duration + 12)
            entity->field_84 = 1;
    }
    else {
        if (!entity->field_80 && entity->activeScreens == 1)
            RSDK.PlaySfx(JacobsLadder->sfxLadder, 0, 255);
        entity->field_80 = true;
    }

    if (entity->field_84) {
        entity->field_7C = entity->height << 20;
        RSDK.SetSpriteAnimation(JacobsLadder->aniFrames, 3, &entity->animator, true, 10);
    }

    if (entity->field_80) {
        int32 storeX       = entity->position.x;
        int32 storeY       = entity->position.y;
        entity->field_7C = val * ((entity->height << 20) / entity->duration);
        entity->position.y += (2 * entity->flip - 1) * (entity->field_7C + 0x300000);

        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntityID(player);

            if (((1 << playerID) & entity->activePlayers)) {
                if (player->state == Player_State_None && player->shield == SHIELD_LIGHTNING) {
                    if (player->jumpPress) {
                        entity->playerTimers[playerID] = 15;
                        entity->activePlayers &= ~(1 << playerID);
                        player->jumpAbilityTimer = 1;
                        player->state            = Player_State_Air;
                        RSDK_sceneInfo->entity   = (Entity *)player;
                        player->movesetState();
                        RSDK_sceneInfo->entity = (Entity *)entity;
                    }
                    else {
                        player->position.x = entity->position.x + entity->playerPositions[playerID].x;
                        player->position.y = entity->position.y + entity->playerPositions[playerID].y;
                    }
                }
                else {
                    entity->activePlayers &= ~(1 << playerID);
                }
            }
            else {
                if (entity->playerTimers[playerID]) {
                    entity->playerTimers[playerID]--;
                }
                else {
                    if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                        if (player->shield == SHIELD_LIGHTNING) {
                            if (player->state != Player_State_None) {
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                                player->state                       = Player_State_None;
                                player->nextAirState                = StateMachine_None;
                                player->nextGroundState             = StateMachine_None;
                                entity->playerPositions[playerID].x = player->position.x - entity->position.x;
                                entity->playerPositions[playerID].y = player->position.y - entity->position.y;
                                player->velocity.x                  = 0;
                                player->velocity.y                  = 0;
                                player->groundVel                   = 0;
                                entity->activePlayers |= 1 << playerID;
                            }
                        }
                        else {
                            Player_CheckHit(player, entity);
                        }
                    }
                }
            }
        }

        entity->position.x = storeX;
        entity->position.y = storeY;
        RSDK.ProcessAnimation(&entity->animator);
    }
    else {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntityID(player);

            if (((1 << playerID) & entity->activePlayers)) {
                entity->activePlayers &= ~(1 << playerID);
                if (player->state == Player_State_None) {
                    player->velocity.x       = 0;
                    player->velocity.y       = 0;
                    player->groundVel        = 0;
                    player->jumpAbilityTimer = 1;
                    player->state            = Player_State_Air;
                }
            }
        }
    }
}

void JacobsLadder_LateUpdate(void) {}

void JacobsLadder_StaticUpdate(void) {}

void JacobsLadder_Draw(void)
{
    RSDK_THIS(JacobsLadder);
    Vector2 drawPos;
    entity->drawFX    = FX_FLIP;
    entity->inkEffect = INK_NONE;
    entity->alpha     = 255;
    int32 val           = -(int32)entity->flip;
    val               = -(val != 0);
    int32 val2          = val & 0xFFFFFF02;

    int32 yOff = 0x300000 * (2 * (entity->flip != false) - 1);
    int32 id   = 0;
    for (int32 i = 0; i < 2; ++i) {
        entity->direction = val2 ^ id;
        drawPos.y         = entity->position.y;
        drawPos.x         = ((2 * (id != 0) - 1) << 21) + entity->position.x;
        RSDK.SetSpriteAnimation(JacobsLadder->aniFrames, 0, &entity->animator2, true, 0);
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        drawPos.y += yOff;
        for (int32 y = 0; y < entity->height; ++y) {
            RSDK.SetSpriteAnimation(JacobsLadder->aniFrames, 1, &entity->animator2, true, 0);
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);
            drawPos.y += (2 * (entity->flip != false) - 1) << 20;
        }
        RSDK.SetSpriteAnimation(JacobsLadder->aniFrames, 2, &entity->animator2, true, 0);
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        id++;
    }

    entity->direction = val2;
    if (entity->field_80 || entity->field_84) {
        entity->inkEffect = INK_ADD;
        if (entity->field_84)
            entity->alpha = 21 * (entity->duration - (entity->intervalOffset + Zone->timer) % entity->interval) + 255;
        drawPos.x = entity->position.x;
        drawPos.y = (2 * (entity->flip != false) - 1) * (entity->field_7C + 0x300000) + entity->position.y;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
    }
}

void JacobsLadder_Create(void *data)
{
    RSDK_THIS(JacobsLadder);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->field_70      = entity->position;
    entity->visible       = true;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = (entity->height + 8) << 20;
    entity->drawFX        = FX_FLIP;
    entity->direction     = entity->flip ? FLIP_Y : FLIP_NONE;
    if (!entity->interval)
        entity->interval = 120;
    if (!entity->duration)
        entity->duration = 60;
    RSDK.SetSpriteAnimation(JacobsLadder->aniFrames, 3, &entity->animator, true, 0);
    entity->hitbox.left   = -35;
    entity->hitbox.top    = -4;
    entity->hitbox.right  = 35;
    entity->hitbox.bottom = 4;
}

void JacobsLadder_StageLoad(void)
{
    JacobsLadder->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/JacobsLadder.bin", SCOPE_STAGE);
    JacobsLadder->sfxLadder = RSDK.GetSFX("TMZ1/JacobsLadder.wav");
}

void JacobsLadder_EditorDraw(void) {}

void JacobsLadder_EditorLoad(void) {}

void JacobsLadder_Serialize(void)
{
    RSDK_EDITABLE_VAR(JacobsLadder, VAR_UINT32, height);
    RSDK_EDITABLE_VAR(JacobsLadder, VAR_ENUM, interval);
    RSDK_EDITABLE_VAR(JacobsLadder, VAR_ENUM, intervalOffset);
    RSDK_EDITABLE_VAR(JacobsLadder, VAR_ENUM, duration);
    RSDK_EDITABLE_VAR(JacobsLadder, VAR_BOOL, flip);
}
