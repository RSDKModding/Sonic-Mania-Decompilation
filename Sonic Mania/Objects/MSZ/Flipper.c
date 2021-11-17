#include "SonicMania.h"

ObjectFlipper *Flipper;

void Flipper_Update(void)
{
    RSDK_THIS(Flipper);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.animationID == 1 && entity->animator.frameID == entity->animator.frameCount - 1)
        RSDK.SetSpriteAnimation(Flipper->aniFrames, 0, &entity->animator, false, 0);

    if (entity->direction == FLIP_NONE) {
        foreach_active(Player, player)
        {
            int playerID = RSDK.GetEntityID(player);
            if (player->position.x >= entity->position.x + 0x40000)
                entity->hitbox.top = ((player->position.x - entity->position.x - 0x40000) >> 17) - 20;
            else
                entity->hitbox.top = -20;
            entity->hitbox.bottom = entity->hitbox.top + 16;
            if (Player_CheckCollisionPlatform(player, entity, &entity->hitbox)) {
                entity->activePlayers |= (1 << playerID);
                player->position.y += 0x80000;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                if (player->jumpPress) {
                    for (int i = SLOT_PLAYER1; i < Player->playerCount; ++i) {
                        if ((1 << i) & entity->activePlayers) {
                            EntityPlayer *playerPtr                  = RSDK_GET_ENTITY(i, Player);
                            int vel                                  = (playerPtr->position.x - entity->position.x) - 0x40000;
                            playerPtr->playerAnimator.animationSpeed = 120;
                            playerPtr->state                         = Player_State_Air;
                            playerPtr->jumpAbilityTimer              = 1;
                            playerPtr->onGround                      = false;
                            playerPtr->velocity.x                    = vel / 7;
                            playerPtr->velocity.y                    = -0x68000 - (vel >> 3);
                        }
                    }
                    RSDK.SetSpriteAnimation(Flipper->aniFrames, 1, &entity->animator, false, 0);
                    RSDK.PlaySfx(Flipper->sfxFlipper, false, 255);
                }
                else {
                    player->tileCollisions = true;
                    player->state          = Player_State_Air;
                    if (player->groundVel >= 0x10000) {
                        if (player->groundVel > 0x20000)
                            player->groundVel = 0x20000;
                        player->groundVel += 4096;
                    }
                    else {
                        player->groundVel = 0x10000;
                        player->groundVel += 4096;
                    }
                }
            }
            else {
                entity->activePlayers &= ~(1 << playerID);
            }
        }
    }
    else {
        foreach_active(Player, player)
        {
            int playerID = RSDK.GetEntityID(player);
            if (player->position.x <= entity->position.x - 0x40000)
                entity->hitbox.top = ((entity->position.x - player->position.x - 0x40000) >> 17) - 20;
            else
                entity->hitbox.top = -20;
            entity->hitbox.bottom = entity->hitbox.top + 16;
            if (Player_CheckCollisionPlatform(player, entity, &entity->hitbox)) {
                entity->activePlayers |= (1 << playerID);
                player->position.y += 0x80000;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                if (player->jumpPress) {
                    for (int i = SLOT_PLAYER1; i < Player->playerCount; ++i) {
                        if ((1 << i) & entity->activePlayers) {
                            EntityPlayer *playerPtr                  = RSDK_GET_ENTITY(i, Player);
                            int vel                                  = (entity->position.x - player->position.x) - 0x40000;
                            playerPtr->playerAnimator.animationSpeed = 120;
                            playerPtr->state                         = Player_State_Air;
                            playerPtr->jumpAbilityTimer              = 1;
                            playerPtr->onGround                      = false;
                            playerPtr->velocity.x                    = -(vel / 7);
                            playerPtr->velocity.y                    = -0x68000 - (vel >> 3);
                        }
                    }
                    RSDK.SetSpriteAnimation(Flipper->aniFrames, 1, &entity->animator, false, 0);
                    RSDK.PlaySfx(Flipper->sfxFlipper, false, 255);
                }
                else {
                    player->tileCollisions = true;
                    player->state          = Player_State_Air;
                    if (player->groundVel <= -0x10000) {
                        if (player->groundVel < -0x20000)
                            player->groundVel = -0x20000;
                        player->groundVel -= 0x1000;
                    }
                    else {
                        player->groundVel = -0x10000;
                        player->groundVel -= 0x1000;
                    }
                }
            }
            else {
                entity->activePlayers &= ~(1 << playerID);
            }
        }
    }
}

void Flipper_LateUpdate(void) {}

void Flipper_StaticUpdate(void) {}

void Flipper_Draw(void)
{
    RSDK_THIS(Flipper);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Flipper_Create(void *data)
{
    RSDK_THIS(Flipper);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(Flipper->aniFrames, 0, &entity->animator, true, 0);
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->hitbox.left   = Flipper->hitbox.left;
        entity->hitbox.right  = Flipper->hitbox.right;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
    }
}

void Flipper_StageLoad(void)
{
    Flipper->aniFrames    = RSDK.LoadSpriteAnimation("MSZ/Flipper.bin", SCOPE_STAGE);
    Flipper->hitbox.left  = -12;
    Flipper->hitbox.right = 56;
    Flipper->sfxFlipper   = RSDK.GetSFX("Stage/Flipper.wav");
}

void Flipper_EditorDraw(void)
{
    RSDK_THIS(Flipper);
    RSDK.SetSpriteAnimation(Flipper->aniFrames, 0, &entity->animator, true, 0);
    Flipper_Draw();
}

void Flipper_EditorLoad(void) { Flipper->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Flipper.bin", SCOPE_STAGE); }

void Flipper_Serialize(void) { RSDK_EDITABLE_VAR(Flipper, VAR_UINT8, direction); }
