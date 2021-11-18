#include "SonicMania.h"

ObjectTransportTube *TransportTube;

void TransportTube_Update(void)
{
    RSDK_THIS(TransportTube);
    StateMachine_Run(self->state);
}

void TransportTube_LateUpdate(void) {}

void TransportTube_StaticUpdate(void) {}

void TransportTube_Draw(void) {}

void TransportTube_Create(void *data)
{
    RSDK_THIS(TransportTube);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0xC00000;
        self->updateRange.y = 0xC00000;
        TransportTube_SetupDirections(self);
        switch (self->type) {
            case 0: self->state = TransportTube_State_Type0; break;
            case 1: self->state = TransportTube_State_Type1; break;
            case 2:
            case 3: self->state = TransportTube_State_Type23; break;
            case 4: self->state = TransportTube_State_Type4; break;
            case 5: self->state = TransportTube_State_Type5; break;
            case 6: self->state = TransportTube_State_Type6; break;
            default: break;
        }
    }
}

void TransportTube_StageLoad(void) { TransportTube->sfxTravel = RSDK.GetSFX("Tube/Travel.wav"); }

void TransportTube_SetupDirections(EntityTransportTube *entity)
{
    int32 velocityX[] = { 0, 0, -16, 16, 12, -12, 12, -12 };
    int32 velocityY[] = { -16, 16, 0, 0, -12, -12, 12, 12 };

    entity->directionCount = 0;
    for (int32 i = 0; i < 8; ++i) {
        if (entity->dirMask & (1 << i)) {
            entity->dirVelocity[entity->directionCount].x = velocityX[i];
            entity->dirVelocity[entity->directionCount].y = velocityY[i];
            ++entity->directionCount;
        }
    }
}

void TransportTube_Unknown2(int32 velX, int32 velY)
{
    RSDK_THIS(TransportTube);

    int32 id = 0;
    int32 velStore = 0xFFFF;
    for (int32 v = 0; v < self->directionCount; ++v) {
        int32 x = abs(velX - self->dirVelocity[v].x);
        int32 y = abs(velY - self->dirVelocity[v].y);

        if (x + y < velStore) {
            id       = v;
            velStore = x + y;
        }
    }   
    
    int32 pos = 0;
    for (int32 v = 0; v < self->directionCount; ++v) {
        if (v != id)
            self->directionIDs[pos++] = v;
    }

    int32 dir            = self->directionIDs[RSDK.Rand(0, self->directionCount - 1)];
    self->velocity.x = self->dirVelocity[dir].x << 16;
    self->velocity.y = self->dirVelocity[dir].y << 16;
}

void TransportTube_State_Type0(void)
{
    RSDK_THIS(TransportTube);

    for (int32 i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
        int32 rx               = (player->position.x - self->position.x) >> 16;
        int32 ry               = (player->position.y - self->position.y) >> 16;
        if (self->playerTimers[i]) {
            if (rx * rx + ry * ry >= 192)
                self->playerTimers[i]--;
        }
        else if (player->state == Player_State_TransportTube && !TransportTube->field_4[i] && rx * rx + ry * ry < 192) {
            TransportTube_Unknown2(rx - (player->velocity.x >> 16), ry - (player->velocity.y >> 16));
            player->position.x  = self->position.x;
            player->position.y  = self->position.y;
            player->velocity    = self->velocity;
            self->playerTimers[i] = 2;
        }
    }
}

void TransportTube_State_Type1(void)
{
    RSDK_THIS(TransportTube);

    for (int32 i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);

        if (Player_CheckValidState(player)) {
            int32 rx = (player->position.x - self->position.x) >> 16;
            int32 ry = (player->position.y - self->position.y) >> 16;
            if (self->playerTimers[i]) {
                if (rx * rx + ry * ry >= 192)
                    self->playerTimers[i]--;
            }
            else if (rx * rx + ry * ry < 192) {
                player->position.x = self->position.x;
                player->position.y = self->position.y;
                if (player->state == Player_State_TransportTube) {
                    player->drawOrder      = Zone->playerDrawLow;
                    player->tileCollisions = true;
                    player->interaction    = true;
                    player->state          = Player_State_Air;
                    if (player->velocity.x)
                        player->direction = player->velocity.x <= 0;
                    self->playerTimers[i] = 2;
                }
                else {
                    TransportTube_Unknown2(rx - (player->velocity.x >> 16), ry - (player->velocity.y >> 16));
                    player->velocity = self->velocity;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->drawOrder         = 1;
                    player->onGround          = false;
                    player->tileCollisions    = false;
                    player->interaction       = false;
                    player->state             = Player_State_TransportTube;
                    player->nextAirState      = StateMachine_None;
                    player->nextGroundState   = StateMachine_None;
                    TransportTube->field_4[i] = 0;
                    RSDK.PlaySfx(TransportTube->sfxTravel, 0, 255);
                    self->playerTimers[i] = 2;
                }
            }
        }
    }
}

void TransportTube_State_Type23(void)
{
    RSDK_THIS(TransportTube);

    for (int32 i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
        int32 rx               = (player->position.x - self->position.x) >> 16;
        int32 ry               = (player->position.y - self->position.y) >> 16;

        if (self->playerTimers[i]) {
            if (rx * rx + ry * ry >= 192)
                --self->playerTimers[i];
        }
        else if (player->state == Player_State_TransportTube && rx * rx + ry * ry < 192) {
            if (self->players[i]) {
                TransportTube->field_4[i] = 0;
                self->players[i]     = NULL;
                self->playerTimers[i]       = 2;
            }
            else {
                if (self->type == 2)
                    TransportTube->field_4[i] = 1;
                else
                    TransportTube->field_4[i] = -1;
                EntityTransportTube *entPtr = RSDK_GET_ENTITY(SceneInfo->entitySlot + TransportTube->field_4[i], TransportTube);
                int32 angle          = RSDK.ATan2((entPtr->position.x - player->position.x) >> 16, (entPtr->position.y - player->position.y) >> 16);
                player->velocity.x = 0xC00 * RSDK.Cos256(angle);
                player->velocity.y = 0xC00 * RSDK.Sin256(angle);
                entPtr->players[i]          = player;
                self->playerTimers[i] = 2;
            }
        }
    }
}

void TransportTube_State_Type4(void)
{
    RSDK_THIS(TransportTube);

    for (int32 i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = self->players[i];
        if (player) {
            if (player->state == Player_State_TransportTube) {
                int32 rx = (player->position.x - self->position.x) >> 16;
                int32 ry = (player->position.y - self->position.y) >> 16;
                if (rx * rx + ry * ry < 192) {
                    EntityTransportTube *tube = RSDK_GET_ENTITY(SceneInfo->entitySlot + TransportTube->field_4[i], TransportTube);
                    player->position.x        = self->position.x;
                    player->position.y        = self->position.y;
                    int32 angle             = RSDK.ATan2((tube->position.x - player->position.x) >> 16, (tube->position.y - player->position.y) >> 16);
                    player->velocity.x    = 0xC00 * RSDK.Cos256(angle);
                    player->velocity.y    = 0xC00 * RSDK.Sin256(angle);
                    self->players[i] = NULL;
                    tube->players[i]   = player;
                }
            }
            else {
                self->players[i] = NULL;
            }
        }
    }
}

void TransportTube_State_Type5(void)
{
    RSDK_THIS(TransportTube);

    for (int32 i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
        int32 rx               = (player->position.x - self->position.x) >> 16;
        int32 ry               = (player->position.y - self->position.y) >> 16;

        if (self->playerTimers[i]) {
            if (rx * rx + ry * ry >= 192)
                --self->playerTimers[i];
        }
        else if (player->state == Player_State_TransportTube && rx * rx + ry * ry < 192) {
            player->position.x = self->position.x;
            player->position.y = self->position.y;
            player->velocity.x = 0;
            player->velocity.y = 0;

            uint8 flags = self->dirMask & ((player->up << 0) | (player->down << 1) | (player->left << 2) | (player->right << 3));
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
                self->playerTimers[i] = 2;
                RSDK.PlaySfx(TransportTube->sfxTravel, 0, 255);
            }
        }
    }
}

void TransportTube_State_Type6(void)
{
    RSDK_THIS(TransportTube);

    for (int32 i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);

        if (Player_CheckValidState(player)) {
            int32 rx = (player->position.x - self->position.x) >> 16;
            int32 ry = (player->position.y - self->position.y) >> 16;

            if (self->playerTimers[i]) {
                if (rx * rx + ry * ry >= 192)
                    self->playerTimers[i]--;
            }
            else if (rx * rx + ry * ry < 192) {
                player->position.x     = self->position.x;
                player->position.y     = self->position.y;
                player->drawOrder      = Zone->playerDrawLow;
                player->tileCollisions = true;
                player->interaction    = true;
                player->state          = Player_State_Air;
                self->playerTimers[i]    = 2;
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void TransportTube_EditorDraw(void)
{
    RSDK_THIS(TransportTube);
    self->updateRange.x = 0xC00000;
    self->updateRange.y = 0xC00000;
    TransportTube_SetupDirections(self);

    for (int32 v = 0; v < self->directionCount; ++v) {
        DrawHelpers_DrawArrow(0xFF0000, self->position.x, self->position.y, self->position.x + (self->dirVelocity[v].x << 18),
                              self->position.y + (self->dirVelocity[v].y << 18));
    }
}

void TransportTube_EditorLoad(void) {}
#endif

void TransportTube_Serialize(void)
{
    RSDK_EDITABLE_VAR(TransportTube, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(TransportTube, VAR_UINT8, dirMask);
}
