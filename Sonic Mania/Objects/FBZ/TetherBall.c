#include "SonicMania.h"

ObjectTetherBall *TetherBall;

void TetherBall_Update(void)
{
    RSDK_THIS(TetherBall);
    StateMachine_Run(entity->state);
}

void TetherBall_LateUpdate(void) {}

void TetherBall_StaticUpdate(void) {}

void TetherBall_Draw(void)
{
    RSDK_THIS(TetherBall);
    Vector2 drawPos;

    int i = 0x200;
    for (; i < entity->size; i += 0x400) {
        TetherBall->animator.frameID = 2;
        drawPos                      = entity->position;
        drawPos.x += i * RSDK.Cos1024(entity->angle);
        drawPos.y += i * RSDK.Sin1024(entity->angle);
        RSDK.DrawSprite(&TetherBall->animator, &drawPos, false);
    }

    TetherBall->animator.frameID = 3;
    drawPos                      = entity->position;
    drawPos.x += (i + 512) * RSDK.Cos1024(entity->angle);
    drawPos.y += (i + 512) * RSDK.Sin1024(entity->angle);
    RSDK.DrawSprite(&TetherBall->animator, &drawPos, false);

    entity->posUnknown           = drawPos;
    TetherBall->animator.frameID = entity->type >> 1;
    entity->drawFX               = FX_FLIP;
    RSDK.DrawSprite(&TetherBall->animator, 0, false);

    entity->drawFX = FX_NONE;
}

void TetherBall_Create(void *data)
{
    RSDK_THIS(TetherBall);
    switch (entity->type) {
        case 0:
        case 2: entity->direction = FLIP_NONE; break;
        case 1: entity->direction = FLIP_Y; break;
        case 3: entity->direction = FLIP_X; break;
        default: break;
    }
    if (!RSDK_sceneInfo->inEditor) {
        entity->active            = ACTIVE_BOUNDS;
        entity->size              = (entity->chainCount << 10) + 512;
        entity->updateRange.x     = 0x1000000;
        entity->updateRange.y     = 0x1000000;
        entity->visible           = 1;
        entity->drawOrder         = Zone->drawOrderLow;
        TetherBall->hitbox.top    = -40;
        TetherBall->hitbox.left   = -40;
        TetherBall->hitbox.right  = 40;
        TetherBall->hitbox.bottom = 40;
        entity->angle             = entity->angleStart;
        if (entity->angleStart > entity->angleEnd) {
            entity->angleEnd   = entity->angleStart;
            entity->angleStart = entity->angleEnd;
        }
        entity->state = TetherBall_Unknown1;
    }
}

void TetherBall_StageLoad(void)
{
    TetherBall->aniFrames = RSDK.LoadSpriteAnimation("FBZ/TetherBall.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(TetherBall->aniFrames, 0, &TetherBall->animator, true, 0);
    TetherBall->sfxWhack = RSDK.GetSFX("Stage/Whack.wav");
}

void TetherBall_Unknown1(void)
{
    RSDK_THIS(TetherBall);
    int storeX       = entity->position.x;
    int storeY       = entity->position.y;
    entity->position = entity->posUnknown;
    entity->rotation = entity->angle << 16;
    foreach_active(Player, player)
    {
        if (!player->sidekick) {
            if (Player_CheckCollisionTouch(player, entity, &TetherBall->hitbox)) {
                switch (entity->angle >> 8) {
                    case 0:
                        if (!player->onGround && player->groundedStore) {
                            if (player->rotation > 112 && player->rotation < 144) {
                                entity->field_70        = 8 * player->velocity.y / entity->chainCount;
                                entity->playerPtr       = player;
                                player->state           = Player_State_None;
                                player->nextGroundState = StateMachine_None;
                                player->nextAirState    = StateMachine_None;
                                entity->state           = TetherBall_Unknown2;
                            }
                        }
                        break;
                    case 1:
                        if (!player->onGround && player->groundedStore) {
                            if (player->rotation > 240 && player->rotation < 272) {
                                entity->field_70        = 8 * -player->velocity.x / entity->chainCount;
                                entity->playerPtr       = player;
                                player->state           = Player_State_None;
                                player->nextGroundState = StateMachine_None;
                                player->nextAirState    = StateMachine_None;
                                entity->state           = TetherBall_Unknown2;
                            }
                        }
                        break;
                    case 2:
                        if (!player->onGround && player->groundedStore) {
                            if (player->rotation > 368 && player->rotation < 400) {
                                entity->field_70        = 8 * -player->velocity.y / entity->chainCount;
                                entity->playerPtr       = player;
                                player->state           = Player_State_None;
                                player->nextGroundState = StateMachine_None;
                                player->nextAirState    = StateMachine_None;
                                entity->state           = TetherBall_Unknown2;
                            }
                        }
                        break;
                    case 3:
                        if (!player->onGround && player->groundedStore) {
                            if (player->rotation < 16 || player->rotation > 496) {
                                entity->field_70        = 8 * player->velocity.x / entity->chainCount;
                                entity->playerPtr       = player;
                                player->state           = Player_State_None;
                                player->nextGroundState = StateMachine_None;
                                player->nextAirState    = StateMachine_None;
                                entity->state           = TetherBall_Unknown2;
                            }
                        }
                        break;
                }
            }
        }
    }
    entity->position.x = storeX;
    entity->position.y = storeY;
}
void TetherBall_Unknown2(void)
{
    RSDK_THIS(TetherBall);
    entity->rotation += entity->field_70;
    entity->angle = entity->rotation >> 16;
    if (entity->angle > 0x2FF)
        entity->field_70 += 0x1800;
    else
        entity->field_70 -= 0x1800;

    if (entity->field_70 >= 0) {
        if (entity->angle >= entity->angleEnd) {
            entity->angle    = entity->angleEnd;
            entity->rotation = entity->angleEnd << 16;
            entity->state    = TetherBall_Unknown3;
            RSDK.PlaySFX(TetherBall->sfxWhack, 0, 255);
        }
    }
    else {
        if (entity->angle <= entity->angleStart) {
            entity->angle    = entity->angleStart;
            entity->rotation = entity->angleStart << 16;
            entity->state    = TetherBall_Unknown3;
            RSDK.PlaySFX(TetherBall->sfxWhack, 0, 255);
        }
    }

    EntityPlayer *player = entity->playerPtr;
    if (player) {
        if (Player_CheckValidState(player)) {
            Hitbox *playerHitbox = Player_GetHitbox(player);
            player->angle        = ((entity->angle >> 2) + 64);
            player->rotation     = 2 * player->angle;
            player->position.x   = entity->position.x;
            player->position.y   = entity->position.y;
            player->position.x += (entity->size + ((playerHitbox->bottom + 24) << 6)) * RSDK.Cos1024(entity->angle);
            player->position.y += (entity->size + ((playerHitbox->bottom + 24) << 6)) * RSDK.Sin1024(entity->angle);
            player->onGround        = true;
            player->nextGroundState = StateMachine_None;
            player->nextAirState    = StateMachine_None;
            player->groundVel       = (entity->field_70 * entity->chainCount) >> 3;
            if (entity->state == TetherBall_Unknown2) {
                if (player->jumpPress) {
                    entity->playerPtr = NULL;
                    Player_StartJump(player);
                }
            }
            else {
                int vel = 0;
                if (player->groundVel <= 0)
                    vel = -0x60000;
                else
                    vel = 0x60000;
                switch (entity->angle >> 8) {
                    case 0:
                        player->collisionMode = CMODE_RWALL;
                        player->position.y += vel;
                        break;
                    case 1:
                        player->collisionMode = CMODE_ROOF;
                        player->position.x -= vel;
                        break;
                    case 2:
                        player->collisionMode = CMODE_LWALL;
                        player->position.y -= vel;
                        break;
                    case 3:
                        player->collisionMode = CMODE_FLOOR;
                        player->position.x += vel;
                        break;
                }
                entity->playerPtr = NULL;
                entity->timer     = 0;
                if (player->playerAnimator.animationID == ANI_JUMP)
                    player->state = Player_State_Roll;
                else
                    player->state = Player_State_Ground;
            }
        }
        else {
            entity->playerPtr = NULL;
        }
    }
}
void TetherBall_Unknown3(void)
{
    RSDK_THIS(TetherBall);
    if (++entity->timer == 8) {
        entity->timer = 0;
        entity->state = TetherBall_Unknown1;
    }
}

void TetherBall_EditorDraw(void) {}

void TetherBall_EditorLoad(void) {}

void TetherBall_Serialize(void)
{
    RSDK_EDITABLE_VAR(TetherBall, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(TetherBall, VAR_UINT8, chainCount);
    RSDK_EDITABLE_VAR(TetherBall, VAR_ENUM, angleStart);
    RSDK_EDITABLE_VAR(TetherBall, VAR_ENUM, angleEnd);
}
