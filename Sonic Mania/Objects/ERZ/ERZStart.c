#include "SonicMania.h"

ObjectERZStart *ERZStart;

void ERZStart_Update(void) {}

void ERZStart_LateUpdate(void) {}

void ERZStart_StaticUpdate(void) {}

void ERZStart_Draw(void) {}

void ERZStart_Create(void *data) {}

void ERZStart_StageLoad(void) {}

void ERZStart_Unknown16(void)
{
    EntityPhantomRuby *ruby = NULL; // ERZStart->ruby;
    EntityFXRuby *fxRuby    = NULL; // ERZStart->fxRuby;
    ruby->angle += 2;
    ruby->position.y   = (RSDK.Sin256(ruby->angle) << 10) + ruby->startPos.y;
    fxRuby->position.x = ruby->position.x;
    fxRuby->position.y = ruby->position.y;
}

void ERZStart_Unknown17(void)
{
    RSDK_THIS(ERZStart);
    EntityPhantomRuby *ruby = NULL; // ERZStart->ruby;
    if (entity->velocity.x < 0x40000)
        entity->velocity.x += 0x1800;
    ruby->angle += 2;
    ruby->position.x += ruby->velocity.x;
    ruby->position.y = (RSDK.Sin256(ruby->angle) << 10) + ruby->startPos.y;
}

void ERZStart_Player_HandleSuperDash(void *p)
{
    EntityPlayer *player = (EntityPlayer *)p;
    RSDK_THIS(Player);

    uint8 flags = 0;
    if (player->up)
        flags |= 1;
    if (player->down)
        flags |= 2;
    if (player->left)
        flags |= 4;
    if (player->right)
        flags |= 8;

    int angle = 0;
    switch (flags) {
        case 0:
            angle = 0;
            if (player->direction)
                angle = 128;
            break;
        case 1: angle = 192; break;
        case 2: angle = 64; break;
        case 4: angle = 128; break;
        case 5: angle = 160; break;
        case 6: angle = 96; break;
        case 8: angle = 0; break;
        case 9: angle = 224; break;
        case 10: angle = 32; break;
        default: break;
    }

    player->rings -= 5;
    if (player->rings < 0)
        player->rings = 0;
    player->velocity.x = 0x1600 * RSDK.Cos256(angle);
    player->velocity.y = 0x1600 * RSDK.Sin256(angle);
    RSDK.PlaySfx(ItemBox->sfx_HyperRing, false, 255);
    RSDK.PlaySfx(Player->sfx_PeelRelease, false, 255);
    if (player->characterID == ID_KNUCKLES) {
        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FLY, &player->playerAnimator, false, 6);
        player->playerAnimator.rotationFlag = 1;
    }
    else {
        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_RUN, &player->playerAnimator, false, 0);
    }
    player->state         = ERZStart_State_PlayerSuperFly;
    player->abilityValue1 = 60;
    if (FXFade) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
        fxFade->speedIn      = 256;
        fxFade->speedOut     = 64;
    }
    Player->superDashCooldown = 30;
}

void ERZStart_State_PlayerSuperFly(void)
{
    RSDK_THIS(Player);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->onGround) {
        player1->state = Player_State_Ground;
        RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_IDLE, &player1->playerAnimator, false, 0);
    }
    else {
        if (player1->up) {
            if (entity->velocity.y > -player1->topSpeed) {
                entity->velocity.y -= player1->acceleration;
                if (player1->velocity.y > 0)
                    entity->velocity.y -= player1->acceleration;
            }
        }
        else if (player1->down) {
            if (entity->velocity.y < player1->topSpeed) {
                entity->velocity.y += player1->acceleration;
                if (player1->velocity.y < 0)
                    entity->velocity.y += player1->acceleration;
            }
        }
        else {
            if (player1->velocity.y) {
                if (player1->velocity.y < 0) {
                    player1->velocity.y += player1->deceleration;
                    if (player1->velocity.y > 0)
                        player1->velocity.y = 0;
                }
                else {
                    player1->velocity.y -= player1->deceleration;
                    if (player1->velocity.y < 0)
                        player1->velocity.y = 0;
                }
            }
        }

        int velY = player1->velocity.y;
        if (velY >= -player1->topSpeed) {
            if (velY > player1->topSpeed)
                player1->velocity.y = velY - (player1->acceleration >> 1) - player1->acceleration;
        }
        else {
            player1->velocity.y = player1->acceleration + velY + (player1->acceleration >> 1);
        }

        if (player1->left) {
            if (player1->characterID == ID_KNUCKLES && entity->direction == FLIP_NONE)
                RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_FLY, &player1->playerAnimator, true, 0);
            if (entity->velocity.x > -player1->topSpeed) {
                entity->velocity.x -= player1->acceleration;
                if (player1->velocity.x > 0)
                    entity->velocity.x -= player1->acceleration;
            }
            player1->direction = FLIP_X;
        }
        else if (player1->right) {
            if (player1->characterID == ID_KNUCKLES && entity->direction == FLIP_X)
                RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_FLY, &player1->playerAnimator, true, 0);
            if (entity->velocity.x < player1->topSpeed) {
                entity->velocity.x += player1->acceleration;
                if (player1->velocity.x < 0)
                    entity->velocity.x += player1->acceleration;
            }
            player1->direction = FLIP_NONE;
        }
        else {
            if (player1->velocity.x) {
                if (player1->velocity.x < 0) {
                    player1->velocity.x += player1->deceleration;
                    if (player1->velocity.x > 0)
                        player1->velocity.x = 0;
                }
                else {
                    player1->velocity.x -= player1->deceleration;
                    if (player1->velocity.x < 0)
                        player1->velocity.x = 0;
                }
            }
        }

        int velX = player1->velocity.x;
        if (velX >= -player1->topSpeed) {
            if (velX > player1->topSpeed)
                player1->velocity.x = velX - (player1->acceleration >> 1) - player1->acceleration;
        }
        else {
            player1->velocity.x = player1->acceleration + velX + (player1->acceleration >> 1);
        }

        if (player1->abilityValue1) {
            player1->abilityValue1--;
        }
        else if (player1->jumpPress) {
            if (player1->rings > 10)
                ERZStart_Player_HandleSuperDash(player1);
        }
    }
}

void ERZStart_Player_StartSuperFly(void)
{
    RSDK_THIS(Player);

    if (entity->jumpAbilityTimer == 1 && entity->jumpPress) {
        if (entity->characterID == ID_KNUCKLES) {
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLY, &entity->playerAnimator, false, 6);
            entity->playerAnimator.rotationFlag = 1;
        }
        else {
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_RUN, &entity->playerAnimator, false, 0);
        }
        entity->state = ERZStart_State_PlayerSuperFly;
    }
}

void ERZStart_State_PlayerRebound(void)
{
    EntityPlayer *entity = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (entity->characterID == ID_KNUCKLES) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLY, &entity->playerAnimator, false, 6);
        entity->playerAnimator.rotationFlag = 1;
    }
    else {
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_RUN, &entity->playerAnimator, false, 0);
    }

    entity->rotation += 0x20;
    entity->onGround = false;
    if (entity->rotation == 0x600) {
        entity->rotation = 0;
        entity->state    = ERZStart_State_PlayerSuperFly;
    }
}

void ERZStart_EditorDraw(void) {}

void ERZStart_EditorLoad(void) {}

void ERZStart_Serialize(void) {}
