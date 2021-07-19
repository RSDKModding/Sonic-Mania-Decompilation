#include "../SonicMania.h"

ObjectIce *Ice;

void Ice_Update(void)
{

}

void Ice_LateUpdate(void)
{

}

void Ice_StaticUpdate(void)
{

}

void Ice_Draw(void)
{

}

void Ice_Create(void* data)
{

}

void Ice_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1")) {
        Ice->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Ice.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("PSZ2")) {
        Ice->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Ice.bin", SCOPE_STAGE);
    }

    Ice->hitbox1.left      = -15;
    Ice->hitbox1.top       = -24;
    Ice->hitbox1.right     = 15;
    Ice->hitbox1.bottom    = 24;
    Ice->hitbox2.left      = -16;
    Ice->hitbox2.top       = -24;
    Ice->hitbox2.right     = 16;
    Ice->hitbox2.bottom    = 24;
    Ice->playerTimers[0]   = 0;
    Ice->playerTimers[1]   = 0;
    Ice->playerTimers[2]   = 0;
    Ice->playerTimers[3]   = 0;
    Ice->sfx_Freeze        = RSDK.GetSFX("PSZ/Freeze.wav");
    Ice->sfx_LedgeBreak    = RSDK.GetSFX("Stage/LedgeBreak.wav");
    Ice->sfx_WindowShatter = RSDK.GetSFX("Stage/WindowShatter.wav");
    Ice->sfx_Struggle      = RSDK.GetSFX("PSZ/Struggle.wav");
    Zone->timeOverState    = Ice_TimeOverCB;
    Zone_AddCallback(Ice_ZoneCB);
}

void Ice_ZoneCB(void)
{
    EntityPlayer *player = RSDK_GET_ENTITY(Zone->playerID, Player);
    if (player->state == Ice_State_FrozenPlayer) {
        Zone->playerFlags[Zone->playerID] = false;
    }
}

void Ice_State_FrozenPlayer(void)
{
    RSDK_THIS(Player);
    if (entity->onGround) {
        if (!entity->groundVel) {
            //if (Ice_Unknown3() == 1)
            //    return;
        }
        entity->skidding = entity->groundVel;
        if (!entity->groundedStore) {
            //if (Ice_Unknown4() == 1)
            //    return;
        }

        entity->pushing = entity->velocity.y;
        if (entity->shield == SHIELD_FIRE && entity->invincibleTimer <= 0) {
            entity->state      = Player_State_Air;
            entity->velocity.y = -entity->jumpStrength >> 1;
            entity->onGround   = false;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, true, 0);
            // Ice_Unknown6(24, 64, 20, 0, 0, 2);
            RSDK.PlaySFX(Ice->sfx_WindowShatter, 0, 255);
            Ice->playerTimers[RSDK.GetEntityID(entity)] = 30;
            entity->skidding       = 0;
            entity->pushing        = 0;
            entity->timer          = 0;
            entity->abilityTimer   = 0;
            entity->spindashCharge = 0;
        }
        else {
            if (!entity->invincibleTimer)
                entity->invincibleTimer = -1;

            if (entity->spindashCharge > 0) {
                entity->spindashCharge--;
            }
            else if (entity->jumpPress) {
                entity->spindashCharge = 15;
                if (++entity->timer > 4) {
                    entity->state      = Player_State_Air;
                    entity->velocity.y = -entity->jumpStrength >> 1;
                    entity->onGround   = false;
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, true, 0);
                    //Ice_Unknown6(24, 64, 20, 0, 0, 2);
                    RSDK.PlaySFX(Ice->sfx_WindowShatter, 0, 255);
                    Ice->playerTimers[RSDK.GetEntityID(entity)] = 30;
                    entity->skidding                            = 0;
                    entity->pushing                             = 0;
                    entity->timer                               = 0;
                    entity->abilityTimer                        = 0;
                    entity->spindashCharge                      = 0;
                }
                else {
                    //Ice_Unknown6(24, 8, 20, 0, 0, 0);
                    RSDK.PlaySFX(Ice->sfx_Struggle, 0, 255);
                }
            }
        }

        if (entity->state == Ice_State_FrozenPlayer) {
            if (entity->onGround) {
                int rollFric                      = entity->rollingFriction;
                entity->left            = false;
                entity->right           = false;
                entity->rollingFriction = 0;
                Player_HandleRollDeceleration();
                if (!entity->groundVel)
                    entity->groundVel += (5000 * RSDK.Sin256(entity->angle)) >> 8;
                if (entity->camera)
                    entity->camera->field_94 = 0;
                entity->jumpAbilityTimer = 0;
                entity->rollingFriction = rollFric;
                if (entity->state == Player_State_Ground)
                    entity->state = Ice_State_FrozenPlayer;
            }
            else {
                entity->velocity.y += 0x3800;
            }
        }
    }
    else {
        /*if ((abs(entity->skidding) < 0x50000 || !Ice_Unknown3()) || (entity->groundedStore || entity->velocity.x)) {
            entity->skidding = entity->velocity.x;
            if (!entity->velocity.y) {
                if (Ice_Unknown4())
                    return;
            }
        }*/
    }
}

void Ice_Unknown8(Entity *p)
{
    EntityPlayer *player = (EntityPlayer *)p;
    RSDK.PlaySFX(Ice->sfx_WindowShatter, 0, 255);
    // Ice_Unknown6(24, 64, 20, 0, 0, 2);
    Ice->playerTimers[RSDK.GetEntityID(player)] = 30;

    player->skidding       = 0;
    player->pushing        = 0;
    player->timer          = 0;
    player->abilityTimer   = 0;
    player->spindashCharge = 0;
    player->visible        = true;
    if (player->state != Player_State_Die) {
        player->outerbox = NULL;
        player->innerbox = NULL;
        player->state    = Player_State_Hit;
        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HURT, &player->playerAnimator, true, 0);
        player->velocity.y = -0x38000;
        player->onGround   = false;
        if (player->gravityStrength == 0x1000) {
            player->velocity.x >>= 1;
            player->velocity.y = -0x1C000;
        }
    }
}

void Ice_TimeOverCB(void)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player->state == Ice_State_FrozenPlayer)
        Ice_Unknown8((Entity *)player);
}

void Ice_EditorDraw(void)
{

}

void Ice_EditorLoad(void)
{

}

void Ice_Serialize(void)
{

}

