#include "SonicMania.h"

ObjectTransportTube *TransportTube;

void TransportTube_Update(void)
{
    RSDK_THIS(TransportTube);
    StateMachine_Run(entity->state);
}

void TransportTube_LateUpdate(void) {}

void TransportTube_StaticUpdate(void) {}

void TransportTube_Draw(void) {}

void TransportTube_Create(void *data)
{
    RSDK_THIS(TransportTube);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0xC00000;
        entity->updateRange.y = 0xC00000;
        TransportTube_SetupDirections(entity);
        switch (entity->type) {
            case 0: entity->state = TransportTube_State_Type0; break;
            case 1: entity->state = TransportTube_State_Type1; break;
            case 2:
            case 3: entity->state = TransportTube_State_Type23; break;
            case 4: entity->state = TransportTube_State_Type4; break;
            case 5: entity->state = TransportTube_State_Type5; break;
            case 6: entity->state = TransportTube_State_Type6; break;
            default: break;
        }
    }
}

void TransportTube_StageLoad(void) { TransportTube->sfxTravel = RSDK.GetSFX("Tube/Travel.wav"); }

void TransportTube_SetupDirections(EntityTransportTube *entity)
{
    int velocityX[] = { 0, 0, -16, 16, 12, -12, 12, -12 };
    int velocityY[] = { -16, 16, 0, 0, -12, -12, 12, 12 };

    entity->directionCount = 0;
    for (int i = 0; i < 8; ++i) {
        if (entity->dirMask & (1 << i)) {
            entity->dirVelocity[entity->directionCount].x = velocityX[i];
            entity->dirVelocity[entity->directionCount].y = velocityY[i];
            ++entity->directionCount;
        }
    }
}

void TransportTube_Unknown2(int velX, int velY)
{
    RSDK_THIS(TransportTube);

    int id = 0;
    int velStore = 0xFFFF;
    for (int v = 0; v < entity->directionCount; ++v) {
        int x = abs(velX - entity->dirVelocity[v].x);
        int y = abs(velY - entity->dirVelocity[v].y);

        if (x + y < velStore) {
            id       = v;
            velStore = x + y;
        }
    }   
    
    int pos = 0;
    for (int v = 0; v < entity->directionCount; ++v) {
        if (v != id)
            entity->directionIDs[pos++] = v;
    }

    int dir            = entity->directionIDs[RSDK.Rand(0, entity->directionCount - 1)];
    entity->velocity.x = entity->dirVelocity[dir].x << 16;
    entity->velocity.y = entity->dirVelocity[dir].y << 16;
}

void TransportTube_State_Type0(void)
{
    RSDK_THIS(TransportTube);

    for (int i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
        int rx               = (player->position.x - entity->position.x) >> 16;
        int ry               = (player->position.y - entity->position.y) >> 16;
        if (entity->playerTimers[i]) {
            if (rx * rx + ry * ry >= 192)
                entity->playerTimers[i]--;
        }
        else if (player->state == Player_Unknown7 && !TransportTube->field_4[i] && rx * rx + ry * ry < 192) {
            TransportTube_Unknown2(rx - (player->velocity.x >> 16), ry - (player->velocity.y >> 16));
            player->position.x  = entity->position.x;
            player->position.y  = entity->position.y;
            player->velocity    = entity->velocity;
            entity->playerTimers[i] = 2;
        }
    }
}

void TransportTube_State_Type1(void)
{
    RSDK_THIS(TransportTube);

    for (int i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);

        if (Player_CheckValidState(player)) {
            int rx = (player->position.x - entity->position.x) >> 16;
            int ry = (player->position.y - entity->position.y) >> 16;
            if (entity->playerTimers[i]) {
                if (rx * rx + ry * ry >= 192)
                    entity->playerTimers[i]--;
            }
            else if (rx * rx + ry * ry < 192) {
                player->position.x = entity->position.x;
                player->position.y = entity->position.y;
                if (player->state == Player_Unknown7) {
                    player->drawOrder      = Zone->playerDrawLow;
                    player->tileCollisions = true;
                    player->interaction    = true;
                    player->state          = Player_State_Air;
                    if (player->velocity.x)
                        player->direction = player->velocity.x <= 0;
                    entity->playerTimers[i] = 2;
                }
                else {
                    TransportTube_Unknown2(rx - (player->velocity.x >> 16), ry - (player->velocity.y >> 16));
                    player->velocity = entity->velocity;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                    player->drawOrder         = 1;
                    player->onGround          = false;
                    player->tileCollisions    = false;
                    player->interaction       = false;
                    player->state             = Player_Unknown7;
                    player->nextAirState      = StateMachine_None;
                    player->nextGroundState   = StateMachine_None;
                    TransportTube->field_4[i] = 0;
                    RSDK.PlaySfx(TransportTube->sfxTravel, 0, 255);
                    entity->playerTimers[i] = 2;
                }
            }
        }
    }
}

void TransportTube_State_Type23(void)
{
    RSDK_THIS(TransportTube);

    for (int i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
        int rx               = (player->position.x - entity->position.x) >> 16;
        int ry               = (player->position.y - entity->position.y) >> 16;

        if (entity->playerTimers[i]) {
            if (rx * rx + ry * ry >= 192)
                --entity->playerTimers[i];
        }
        else if (player->state == Player_Unknown7 && rx * rx + ry * ry < 192) {
            if (entity->players[i]) {
                TransportTube->field_4[i] = 0;
                entity->players[i]     = NULL;
                entity->playerTimers[i]       = 2;
            }
            else {
                if (entity->type == 2)
                    TransportTube->field_4[i] = 1;
                else
                    TransportTube->field_4[i] = -1;
                EntityTransportTube *entPtr = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + TransportTube->field_4[i], TransportTube);
                int angle          = RSDK.ATan2((entPtr->position.x - player->position.x) >> 16, (entPtr->position.y - player->position.y) >> 16);
                player->velocity.x = 0xC00 * RSDK.Cos256(angle);
                player->velocity.y = 0xC00 * RSDK.Sin256(angle);
                entPtr->players[i]          = player;
                entity->playerTimers[i] = 2;
            }
        }
    }
}

void TransportTube_State_Type4(void)
{
    RSDK_THIS(TransportTube);

    for (int i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = entity->players[i];
        if (player) {
            if (player->state == Player_Unknown7) {
                int rx = (player->position.x - entity->position.x) >> 16;
                int ry = (player->position.y - entity->position.y) >> 16;
                if (rx * rx + ry * ry < 192) {
                    EntityTransportTube *tube = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + TransportTube->field_4[i], TransportTube);
                    player->position.x        = entity->position.x;
                    player->position.y        = entity->position.y;
                    int angle             = RSDK.ATan2((tube->position.x - player->position.x) >> 16, (tube->position.y - player->position.y) >> 16);
                    player->velocity.x    = 0xC00 * RSDK.Cos256(angle);
                    player->velocity.y    = 0xC00 * RSDK.Sin256(angle);
                    entity->players[i] = NULL;
                    tube->players[i]   = player;
                }
            }
            else {
                entity->players[i] = NULL;
            }
        }
    }
}

void TransportTube_State_Type5(void)
{
    RSDK_THIS(TransportTube);

    for (int i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
        int rx               = (player->position.x - entity->position.x) >> 16;
        int ry               = (player->position.y - entity->position.y) >> 16;

        if (entity->playerTimers[i]) {
            if (rx * rx + ry * ry >= 192)
                --entity->playerTimers[i];
        }
        else if (player->state == Player_Unknown7 && rx * rx + ry * ry < 192) {
            player->position.x = entity->position.x;
            player->position.y = entity->position.y;
            player->velocity.x = 0;
            player->velocity.y = 0;

            uint8 flags = entity->dirMask & ((player->up << 0) | (player->down << 1) | (player->left << 2) | (player->right << 3));
            if (flags & 1) {
                player->velocity.y = -0x100000;
            }
            else if (flags & 2) {
                player->velocity.y = 0x100000;
            }
            else if (flags & 4) {
                player->velocity.x = -0x100000;
            }
            else if (flags & 8) {
                player->velocity.x = 0x100000;
            }

            if (player->velocity.x || player->velocity.y) {
                entity->playerTimers[i] = 2;
                RSDK.PlaySfx(TransportTube->sfxTravel, 0, 255);
            }
        }
    }
}

void TransportTube_State_Type6(void)
{
    RSDK_THIS(TransportTube);

    for (int i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);

        if (Player_CheckValidState(player)) {
            int rx = (player->position.x - entity->position.x) >> 16;
            int ry = (player->position.y - entity->position.y) >> 16;

            if (entity->playerTimers[i]) {
                if (rx * rx + ry * ry >= 192)
                    entity->playerTimers[i]--;
            }
            else if (rx * rx + ry * ry < 192) {
                player->position.x     = entity->position.x;
                player->position.y     = entity->position.y;
                player->drawOrder      = Zone->playerDrawLow;
                player->tileCollisions = true;
                player->interaction    = true;
                player->state          = Player_State_Air;
                entity->playerTimers[i]    = 2;
            }
        }
    }
}

void TransportTube_EditorDraw(void) {}

void TransportTube_EditorLoad(void) {}

void TransportTube_Serialize(void)
{
    RSDK_EDITABLE_VAR(TransportTube, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(TransportTube, VAR_UINT8, dirMask);
}
