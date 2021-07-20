#include "../SonicMania.h"

ObjectSpringboard *Springboard;

void Springboard_Update(void)
{
    RSDK_THIS(Springboard);
    RSDK.ProcessAnimation(&entity->animator);

    foreach_active(Player, playerPtr)
    {
        if (playerPtr->velocity.y >= 0 && ((1 << RSDK.GetEntityID(playerPtr)) & entity->playerBits)) {
            int val  = minVal((playerPtr->position.x - entity->position.x + 0x1C0000) >> 17, 28);
            int val2 = maxVal(val, 0);

            int val3 = val2;
            if ((entity->direction & 1))
                val3 = 28 - val2;
            if (val3 >= 8) {
                if (entity->animator.frameID == 3)
                    RSDK.SetSpriteAnimation(Springboard->aniFrames, 0, &entity->animator, true, 0);
                if (entity->animator.frameID == 2) {
                    int anim = playerPtr->playerAnimator.animationID;
                    if (anim == ANI_WALK || (anim > ANI_AIRWALK && anim <= ANI_DASH))
                        playerPtr->storedAnim = playerPtr->playerAnimator.animationID;
                    else
                        playerPtr->storedAnim = ANI_WALK;
                    playerPtr->state          = Player_State_Air;
                    playerPtr->onGround       = false;
                    playerPtr->tileCollisions = true;
                    RSDK.SetSpriteAnimation(playerPtr->spriteIndex, ANI_SPRINGCS, &playerPtr->playerAnimator, true, 1);
                    playerPtr->groundVel   = playerPtr->velocity.x;
                    int pos                = minVal(2 * val3 - 16, 39);
                    playerPtr->velocity.y  = Springboard->array2[pos] - playerPtr->gravityStrength - entity->force;
                    playerPtr->jumpAbility = 0;
                    RSDK.PlaySFX(Springboard->sfxSpring, 0, 255);
                }
            }
        }
    }

    foreach_active(Player, player)
    {
        int playerID   = RSDK.GetEntityID(player);
        int playerVel  = player->groundVel;
        int playerVelX = player->velocity.x;
        bool32 flag    = false;
        int val        = clampVal((player->position.x - entity->position.x + 0x1C0000) >> 17, 0, 27);

        if (!entity->direction) {
            int hitboxTop = 0;

            if (entity->animator.frameID <= 2) {
                hitboxTop = Springboard->array4[val];
            }
            else if (entity->animator.frameID == 3) {
                hitboxTop = Springboard->array3[val];
            }

            Hitbox hitbox;
            hitbox.left   = -28;
            hitbox.top    = -hitboxTop;
            hitbox.right  = 28;
            hitbox.bottom = 8;

            byte collision = 0;
            if (!((1 << playerID) & entity->playerBits))
                flag = collision = Player_CheckCollisionBox(player, entity, &hitbox);
            else
                flag = collision = Player_CheckCollisionPlatform(player, entity, &hitbox);

            flag = collision == 1;
            switch (collision) {
                case 2: player->groundVel = playerVel; player->velocity.x = playerVelX;
                case 0:
                case 3:
                case 4:
                    if (player->velocity.y >= 0 && ((1 << playerID) & entity->playerBits)) {
                        Hitbox *playerHitbox = Player_GetHitbox(player);
                        player->position.y   = entity->position.y - (playerHitbox->bottom << 16) - (hitboxTop << 16);
                        if (!flag)
                            flag = player->position.x > entity->position.x;
                    }
                    break;
                case 1:
#if RETRO_USE_PLUS 
                    if (player->state == Player_State_MightyHammerDrop)
                        player->state = Player_State_Air;
#endif
                    break;
                default: break;
            }
            player->flailing = false;
        }
        else if (entity->direction == FLIP_X) {
            int pos       = abs(val - 27);
            int hitboxTop = 0;

            if (entity->animator.frameID <= 2) {
                hitboxTop = Springboard->array4[pos];
            }
            else if (entity->animator.frameID != 3) {
                hitboxTop = Springboard->array3[pos];
            }

            Hitbox hitbox;
            hitbox.left   = -28;
            hitbox.top    = -hitboxTop;
            hitbox.right  = 28;
            hitbox.bottom = 8;

            byte collision = 0;
            if (!((1 << playerID) & entity->playerBits))
                collision = Player_CheckCollisionBox(player, entity, &hitbox);
            else
                 collision = Player_CheckCollisionPlatform(player, entity, &hitbox);
            flag = collision == 1;
            switch (collision) {
                case 0:
                case 2:
                case 4: break;
                case 1:
#if RETRO_USE_PLUS 
                    if (player->state == Player_State_MightyHammerDrop) {
                        player->state = Player_State_Air;
                    }
#endif
                    break;
                case 3:
                    player->groundVel  = playerVel;
                    player->velocity.x = playerVelX;
                    break;
                default: break;
            }

            if (player->velocity.y >= 0 && ((1 << playerID) & entity->playerBits)) {
                Hitbox *playerHitbox = Player_GetHitbox(player);
                player->position.y   = entity->position.y - (playerHitbox->bottom << 16) - (hitboxTop << 16);
                if (!flag)
                    flag = player->position.x < entity->position.x;
            }
            player->flailing = false;
        }

        if (flag) {
            entity->playerBits |= (1 << playerID);
            if (val >= 8 && !entity->field_5C)
                entity->field_5C = 6;
        }
        else {
            entity->playerBits &= ~(1 << playerID);
        }
    }
}

void Springboard_LateUpdate(void) {}

void Springboard_StaticUpdate(void) {}

void Springboard_Draw(void)
{
    RSDK_THIS(Springboard);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Springboard_Create(void *data)
{
    RSDK_THIS(Springboard);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX |= FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x400000;
    entity->updateRange.y = 0x400000;
    if (!RSDK_sceneInfo->inEditor) {
        entity->force = (entity->force + 8) << 15;
        RSDK.SetSpriteAnimation(Springboard->aniFrames, 0, &entity->animator, true, 3);
    }
}

void Springboard_StageLoad(void)
{
    Springboard->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Springboard.bin", SCOPE_STAGE);
    Springboard->sfxSpring = RSDK.GetSFX("Global/Spring.wav");
    DEBUGMODE_ADD_OBJ(Springboard);
}

void Springboard_DebugSpawn(void)
{
    RSDK_THIS(Springboard);
    CREATE_ENTITY(Springboard, NULL, entity->position.x, entity->position.y);
}
void Springboard_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Springboard->aniFrames, 1, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Springboard_EditorDraw(void) {}

void Springboard_EditorLoad(void) {}

void Springboard_Serialize(void)
{
    RSDK_EDITABLE_VAR(Springboard, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Springboard, VAR_ENUM, force);
}
