#include "SonicMania.h"

ObjectJunctionWheel *JunctionWheel;

void JunctionWheel_Update(void)
{
    RSDK_THIS(JunctionWheel);
    if (RSDK_GET_ENTITY((RSDK_sceneInfo->entitySlot - 1), Button)->activated)
        entity->field_88 ^= 1;
    if (entity->field_88)
        entity->rotation += 4;
    else
        entity->rotation -= 4;
    entity->rotation &= 0x1FF;

    int rot     = ((entity->rotation + 48) >> 6) & 7;

    foreach_active(Player, player) {
        if (player->state == Player_State_None) {
            if (Player_CheckCollisionTouch(player, entity, &JunctionWheel->hitbox1)) {
                player->position.x += player->groundVel * RSDK.Cos512(entity->rotation) + entity->position.x;
                player->position.y += player->groundVel * RSDK.Sin512(entity->rotation) + entity->position.y;

                if (player->groundVel < -0x1000)
                    player->groundVel += 0x200;

                if (player->groundVel == -0x1000) {
                    if (entity->rotation == 384) {
                        player->state          = Player_State_Air;
                        player->tileCollisions = true;
                        player->groundVel      = 0;
                        player->velocity.x     = 0;
                        player->velocity.y     = 0x80000;
                        player->onGround       = false;
                        RSDK.PlaySfx(Player->sfx_Release, false, 255);
                    }

                    if (entity->field_88 == 1 && entity->rotation == 284) {
                        player->state          = Player_State_Air;
                        player->tileCollisions = true;
                        player->groundVel      = 0x80000;
                        player->velocity.x     = 0x80000;
                        player->velocity.y     = 0x40000;
                        player->onGround       = false;
                        RSDK.PlaySfx(Player->sfx_Release, false, 255);
                    }
                }
            }
        }
        else if (rot == 1) {
            Player_CheckCollisionBox(player, entity, &JunctionWheel->hitbox3);

            if (Player_CheckCollisionTouch(player, entity, &JunctionWheel->hitbox5)) {
                player->state = Player_State_None;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                player->onGround  = false;
                player->groundVel       = -0x1C00;

                player->tileCollisions  = false;
                player->nextAirState    = 0;
                player->nextGroundState = 0;
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                RSDK.PlaySfx(Player->sfx_Roll, false, 255);
            }
            if (player->velocity.y < 0 || (player->onGround && player->collisionMode))
                Player_CheckCollisionBox(player, entity, &JunctionWheel->hitbox4);
        }
        else {
            if (rot != 5) {
                if (player->position.y < entity->position.y)
                    Player_CheckCollisionBox(player, entity, &JunctionWheel->hitbox2);
                Player_CheckCollisionBox(player, entity, &JunctionWheel->hitbox3);
            }
            else if (player->velocity.x <= 0) {
                Player_CheckCollisionBox(player, entity, &JunctionWheel->hitbox2);
                if (Player_CheckCollisionTouch(player, entity, &JunctionWheel->hitbox6)) {
                    player->state = Player_State_None;
                    RSDK.SetSpriteAnimation(player->spriteIndex, 10, &player->playerAnimator, false, 0);
                    player->onGround  = false;
                    player->groundVel = -0x1800;
                }
                player->tileCollisions  = false;
                player->nextAirState    = 0;
                player->nextGroundState = 0;
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                RSDK.PlaySfx(Player->sfx_Roll, false, 255);
            }
            if (player->velocity.y < 0 || (player->onGround && player->collisionMode))
                Player_CheckCollisionBox(player, entity, &JunctionWheel->hitbox4);
        }
    }
}

void JunctionWheel_LateUpdate(void)
{

}

void JunctionWheel_StaticUpdate(void)
{

}

void JunctionWheel_Draw(void)
{
    RSDK_THIS(JunctionWheel);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
}

void JunctionWheel_Create(void* data)
{
    RSDK_THIS(JunctionWheel);
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->drawFX        = FX_ROTATE;
    RSDK.SetSpriteAnimation(JunctionWheel->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(JunctionWheel->aniFrames, 1, &entity->animator2, true, 0);
}

void JunctionWheel_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        JunctionWheel->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/JunctionWheel.bin", SCOPE_STAGE);
    JunctionWheel->hitbox1.left   = -192;
    JunctionWheel->hitbox1.top    = -192;
    JunctionWheel->hitbox1.right  = 192;
    JunctionWheel->hitbox1.bottom = 192;

    JunctionWheel->hitbox2.left   = -40;
    JunctionWheel->hitbox2.top    = -56;
    JunctionWheel->hitbox2.right  = -16;
    JunctionWheel->hitbox2.bottom = 32;

    JunctionWheel->hitbox3.left   = 16;
    JunctionWheel->hitbox3.top    = -56;
    JunctionWheel->hitbox3.right  = 40;
    JunctionWheel->hitbox3.bottom = 32;

    JunctionWheel->hitbox4.left   = -40;
    JunctionWheel->hitbox4.top    = 32;
    JunctionWheel->hitbox4.right  = 40;
    JunctionWheel->hitbox4.bottom = 56;

    JunctionWheel->hitbox5.left   = -40;
    JunctionWheel->hitbox5.top    = -40;
    JunctionWheel->hitbox5.right  = -16;
    JunctionWheel->hitbox5.bottom = -8;

    JunctionWheel->hitbox6.left   = 16;
    JunctionWheel->hitbox6.top    = 8;
    JunctionWheel->hitbox6.right  = 38;
    JunctionWheel->hitbox6.bottom = 40;
}

void JunctionWheel_EditorDraw(void)
{

}

void JunctionWheel_EditorLoad(void)
{

}

void JunctionWheel_Serialize(void)
{

}

