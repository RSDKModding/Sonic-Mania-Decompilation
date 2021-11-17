#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectAIZRockPile *AIZRockPile;

void AIZRockPile_Update(void)
{
    RSDK_THIS(AIZRockPile);
    Hitbox *hitbox = RSDK.GetHitbox(&entity->animator, 1);

    foreach_active(Player, player)
    {
        if (!entity->onlyMighty || player->characterID == ID_MIGHTY) {
            if (!entity->onlyKnux || player->characterID == ID_KNUCKLES) {
                int32 cMode     = player->collisionMode;
                int32 playerX   = player->position.x;
                int32 playerY   = player->position.y;
                int32 xVelocity = player->velocity.x;
                int32 yVelocity = player->velocity.y;
                int32 jumping   = player->playerAnimator.animationID == ANI_JUMP;
                int32 groundVel = player->groundVel;

                
                if (entity->smashSides || entity->smashTop) {
                    int32 side = Player_CheckCollisionBox(player, entity, hitbox);
                    if (entity->smashSides && (side == 2 || side == 3)) {
                        if (side == 2 || side == 3) {
                            bool32 flag = jumping && player->onGround && abs(groundVel) >= 0x48000;
                            if (player->shield == SHIELD_FIRE) {
                                EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield);
                                flag |= shield->animator.animationID == 2;
                            }

                            flag |= player->characterID == ID_SONIC && player->superState == SUPERSTATE_SUPER;
                            if (!player->sidekick && (flag || (player->characterID == ID_KNUCKLES))) {
                                player->position.x = playerX;
                                player->position.y = playerY;
                                player->velocity.x = xVelocity;
                                player->velocity.y = yVelocity;
                                player->groundVel  = groundVel;
                                if (xVelocity <= 0)
                                    AIZRockPile_SpawnRocks(entity->rockSpeedsL);
                                else
                                    AIZRockPile_SpawnRocks(entity->rockSpeedsR);
                                foreach_return;
                            }
                        }
                    }

                    if (entity->smashTop && side == 1) {
                        bool32 flag = jumping;
                        flag |= player->characterID == ID_SONIC && player->playerAnimator.animationID == ANI_DROPDASH;
                        flag |= player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop;
                        if (player->groundedStore && cMode != CMODE_FLOOR && cMode != CMODE_ROOF)
                            flag = 0;

                        if (flag && !player->sidekick) {
                            player->onGround = false;
                            if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                                player->velocity.y = yVelocity - 0x10000;
                            else
                                player->velocity.y = -0x30000;
                            AIZRockPile_SpawnRocks(entity->rockSpeedsT);
                            foreach_return;
                        }
                    }
                }
            }
        }
        Player_CheckCollisionBox(player, entity, hitbox);
    }
}

void AIZRockPile_LateUpdate(void) {}

void AIZRockPile_StaticUpdate(void) {}

void AIZRockPile_Draw(void)
{
    RSDK_THIS(AIZRockPile);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void AIZRockPile_Create(void *data)
{
    RSDK_THIS(AIZRockPile);
    RSDK.SetSpriteAnimation(AIZRockPile->aniFrames, 0, &entity->animator, true, entity->size + 3);
    if (!RSDK_sceneInfo->inEditor) {
        switch (entity->size) {
            case 0:
                entity->size          = 4;
                entity->rockPositions = AIZRockPile->rockPositions_small;
                entity->rockSpeedsT   = AIZRockPile->rockSpeedsT_small;
                entity->rockSpeedsL   = AIZRockPile->rockSpeedsL_small;
                entity->rockSpeedsR   = AIZRockPile->rockSpeedsR_small;
                break;
            case 1:
                entity->size          = 5;
                entity->rockPositions = AIZRockPile->rockPositions_med;
                entity->rockSpeedsT   = AIZRockPile->rockSpeedsT_med;
                entity->rockSpeedsL   = AIZRockPile->rockSpeedsL_med;
                entity->rockSpeedsR   = AIZRockPile->rockSpeedsR_med;
                break;
            case 2:
                entity->size          = 8;
                entity->rockPositions = AIZRockPile->rockPositions_large;
                entity->rockSpeedsT   = AIZRockPile->rockSpeedsT_large;
                entity->rockSpeedsL   = AIZRockPile->rockSpeedsL_large;
                entity->rockSpeedsR   = AIZRockPile->rockSpeedsR_large;
                break;
        }

        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
    }
}

void AIZRockPile_StageLoad(void)
{
    AIZRockPile->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Platform.bin", SCOPE_STAGE);
    AIZRockPile->sfxBreak   = RSDK.GetSFX("Stage/LedgeBreak3.wav");
}

void AIZRockPile_SpawnRocks(int32 *speeds)
{
    RSDK_THIS(AIZRockPile);

    for (int32 i = 0; i < entity->size; ++i) {
        EntityDebris *debris =
            (EntityDebris *)RSDK.CreateEntity(Debris->objectID, Debris_State_Fall, entity->position.x + entity->rockPositions[2 * i],
                                              entity->position.y + entity->rockPositions[(2 * i) + 1]);
        RSDK.SetSpriteAnimation(AIZRockPile->aniFrames, 1, &debris->animator, true, 0);
        debris->velocity.x    = speeds[2 * i];
        debris->velocity.y    = speeds[(2 * i) + 1];
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x800000;
        debris->updateRange.y = 0x800000;
        debris->gravity       = 0x1800;
    }

    RSDK.PlaySfx(AIZRockPile->sfxBreak, 0, 255);
    destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void AIZRockPile_EditorDraw(void) { AIZRockPile_Draw(); }

void AIZRockPile_EditorLoad(void) { AIZRockPile->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Platform.bin", SCOPE_STAGE); }
#endif

void AIZRockPile_Serialize(void)
{
    RSDK_EDITABLE_VAR(AIZRockPile, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(AIZRockPile, VAR_BOOL, smashTop);
    RSDK_EDITABLE_VAR(AIZRockPile, VAR_BOOL, smashSides);
    RSDK_EDITABLE_VAR(AIZRockPile, VAR_BOOL, onlyKnux);
    RSDK_EDITABLE_VAR(AIZRockPile, VAR_BOOL, onlyMighty);
}
#endif
