// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TransportTube Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
        self->updateRange.x = TO_FIXED(192);
        self->updateRange.y = TO_FIXED(192);
        TransportTube_SetupDirections(self);

        switch (self->type) {
            case TRANSPORTTUBE_CHANGEDIR: self->state = TransportTube_State_ChangeDir; break;
            case TRANSPORTTUBE_ENTRY: self->state = TransportTube_State_Entry; break;

            case TRANSPORTTUBE_TOTARGET_NEXT:
            case TRANSPORTTUBE_TOTARGET_PREV: self->state = TransportTube_State_ToTargetEntity; break;

            case TRANSPORTTUBE_TOTARGET_NODE: self->state = TransportTube_State_TargetSeqNode; break;
            case TRANSPORTTUBE_JUNCTION: self->state = TransportTube_State_ChooseDir; break;
            case TRANSPORTTUBE_EXIT: self->state = TransportTube_State_Exit; break;
            default: break;
        }
    }
}

void TransportTube_StageLoad(void) { TransportTube->sfxTravel = RSDK.GetSfx("Tube/Travel.wav"); }

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

void TransportTube_HandleVelocityChange(int32 velX, int32 velY)
{
    RSDK_THIS(TransportTube);

    int32 id       = 0;
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

    int32 dir        = self->directionIDs[RSDK.Rand(0, self->directionCount - 1)];
    self->velocity.x = TO_FIXED(self->dirVelocity[dir].x);
    self->velocity.y = TO_FIXED(self->dirVelocity[dir].y);
}

void TransportTube_State_ChangeDir(void)
{
    RSDK_THIS(TransportTube);

    for (int32 i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
        int32 rx             = FROM_FIXED(player->position.x - self->position.x);
        int32 ry             = FROM_FIXED(player->position.y - self->position.y);

        if (self->playerTimers[i]) {
            if (rx * rx + ry * ry >= 0xC0)
                self->playerTimers[i]--;
        }
        else if (player->state == Player_State_TransportTube && !TransportTube->nextSlot[i] && rx * rx + ry * ry < 0xC0) {
            TransportTube_HandleVelocityChange(rx - FROM_FIXED(player->velocity.x), ry - FROM_FIXED(player->velocity.y));
            player->position.x    = self->position.x;
            player->position.y    = self->position.y;
            player->velocity      = self->velocity;
            self->playerTimers[i] = 2;
        }
    }
}

void TransportTube_State_Entry(void)
{
    RSDK_THIS(TransportTube);

    for (int32 i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);

        if (Player_CheckValidState(player)) {
            int32 rx = FROM_FIXED(player->position.x - self->position.x);
            int32 ry = FROM_FIXED(player->position.y - self->position.y);
            if (self->playerTimers[i]) {
                if (rx * rx + ry * ry >= 0xC0)
                    self->playerTimers[i]--;
            }
            else if (rx * rx + ry * ry < 0xC0) {
                player->position.x = self->position.x;
                player->position.y = self->position.y;
                if (player->state == Player_State_TransportTube) {
                    player->drawGroup      = Zone->playerDrawGroup[0];
                    player->tileCollisions = TILECOLLISION_DOWN;
                    player->interaction    = true;
                    player->state          = Player_State_Air;
                    if (player->velocity.x)
                        player->direction = player->velocity.x <= 0;
                    self->playerTimers[i] = 2;
                }
                else {
                    TransportTube_HandleVelocityChange(rx - FROM_FIXED(player->velocity.x), ry - FROM_FIXED(player->velocity.y));
                    player->velocity = self->velocity;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->drawGroup          = 1;
                    player->onGround           = false;
                    player->tileCollisions     = TILECOLLISION_NONE;
                    player->interaction        = false;
                    player->state              = Player_State_TransportTube;
                    player->nextAirState       = StateMachine_None;
                    player->nextGroundState    = StateMachine_None;
                    TransportTube->nextSlot[i] = 0;
                    RSDK.PlaySfx(TransportTube->sfxTravel, false, 255);
                    self->playerTimers[i] = 2;
                }
            }
        }
    }
}

void TransportTube_State_ToTargetEntity(void)
{
    RSDK_THIS(TransportTube);

    for (int32 i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
        int32 rx             = FROM_FIXED(player->position.x - self->position.x);
        int32 ry             = FROM_FIXED(player->position.y - self->position.y);

        if (self->playerTimers[i]) {
            if (rx * rx + ry * ry >= 0xC0)
                --self->playerTimers[i];
        }
        else if (player->state == Player_State_TransportTube && rx * rx + ry * ry < 0xC0) {
            if (self->players[i]) {
                TransportTube->nextSlot[i] = 0;
                self->players[i]           = NULL;
                self->playerTimers[i]      = 2;
            }
            else {
                if (self->type == TRANSPORTTUBE_TOTARGET_NEXT)
                    TransportTube->nextSlot[i] = 1;
                else
                    TransportTube->nextSlot[i] = -1;

                EntityTransportTube *node = RSDK_GET_ENTITY(SceneInfo->entitySlot + TransportTube->nextSlot[i], TransportTube);
                int32 angle        = RSDK.ATan2(FROM_FIXED(node->position.x - player->position.x), FROM_FIXED(node->position.y - player->position.y));
                player->velocity.x        = 0xC00 * RSDK.Cos256(angle);
                player->velocity.y        = 0xC00 * RSDK.Sin256(angle);
                node->players[i]          = player;
                self->playerTimers[i]     = 2;
            }
        }
    }
}

void TransportTube_State_TargetSeqNode(void)
{
    RSDK_THIS(TransportTube);

    for (int32 i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = self->players[i];
        if (player) {
            if (player->state == Player_State_TransportTube) {
                int32 rx = FROM_FIXED(player->position.x - self->position.x);
                int32 ry = FROM_FIXED(player->position.y - self->position.y);
                if (rx * rx + ry * ry < 0xC0) {
                    player->position.x = self->position.x;
                    player->position.y = self->position.y;

                    EntityTransportTube *node = RSDK_GET_ENTITY(SceneInfo->entitySlot + TransportTube->nextSlot[i], TransportTube);
                    int32 angle = RSDK.ATan2(FROM_FIXED(node->position.x - player->position.x), FROM_FIXED(node->position.y - player->position.y));
                    player->velocity.x = 0xC00 * RSDK.Cos256(angle);
                    player->velocity.y = 0xC00 * RSDK.Sin256(angle);
                    self->players[i]   = NULL;
                    node->players[i]   = player;
                }
            }
            else {
                self->players[i] = NULL;
            }
        }
    }
}

void TransportTube_State_ChooseDir(void)
{
    RSDK_THIS(TransportTube);

    for (int32 i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
        int32 rx             = FROM_FIXED(player->position.x - self->position.x);
        int32 ry             = FROM_FIXED(player->position.y - self->position.y);

        if (self->playerTimers[i]) {
            if (rx * rx + ry * ry >= 0xC0)
                --self->playerTimers[i];
        }
        else if (player->state == Player_State_TransportTube && rx * rx + ry * ry < 0xC0) {
            player->position.x = self->position.x;
            player->position.y = self->position.y;
            player->velocity.x = 0;
            player->velocity.y = 0;

            uint8 moveMask = self->dirMask & ((player->up << 0) | (player->down << 1) | (player->left << 2) | (player->right << 3));
            if (moveMask & 1)
                player->velocity.y = -0x100000;
            else if (moveMask & 2)
                player->velocity.y = 0x100000;
            else if (moveMask & 4)
                player->velocity.x = -0x100000;
            else if (moveMask & 8)
                player->velocity.x = 0x100000;

            if (player->velocity.x || player->velocity.y) {
                self->playerTimers[i] = 2;
                RSDK.PlaySfx(TransportTube->sfxTravel, false, 255);
            }
        }
    }
}

void TransportTube_State_Exit(void)
{
    RSDK_THIS(TransportTube);

    for (int32 i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);

        if (Player_CheckValidState(player)) {
            int32 rx = FROM_FIXED(player->position.x - self->position.x);
            int32 ry = FROM_FIXED(player->position.y - self->position.y);

            if (self->playerTimers[i]) {
                if (rx * rx + ry * ry >= 0xC0)
                    self->playerTimers[i]--;
            }
            else if (rx * rx + ry * ry < 0xC0) {
                player->position.x     = self->position.x;
                player->position.y     = self->position.y;
                player->drawGroup      = Zone->playerDrawGroup[0];
                player->tileCollisions = TILECOLLISION_DOWN;
                player->interaction    = true;
                player->state          = Player_State_Air;
                self->playerTimers[i]  = 2;
            }
        }
    }
}

#if GAME_INCLUDE_EDITOR
void TransportTube_EditorDraw(void)
{
    RSDK_THIS(TransportTube);
    self->updateRange.x = 0xC00000;
    self->updateRange.y = 0xC00000;

    RSDK.SetSpriteAnimation(TransportTube->aniFrames, 0, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, NULL, false);

    RSDK_DRAWING_OVERLAY(true);

    TransportTube_SetupDirections(self);
    for (int32 v = 0; v < self->directionCount; ++v) {
        DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x + (self->dirVelocity[v].x << 18),
                              self->position.y + (self->dirVelocity[v].y << 18), 0xFF0000, INK_NONE, 0xFF);
    }

    RSDK_DRAWING_OVERLAY(false);

    RSDK.SetSpriteAnimation(TransportTube->aniFrames, self->type == TRANSPORTTUBE_JUNCTION ? 1 : 2, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void TransportTube_EditorLoad(void)
{
    TransportTube->aniFrames = RSDK.LoadSpriteAnimation("CPZ/TransportTube.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(TransportTube, type);
    RSDK_ENUM_VAR("Change Direction", TRANSPORTTUBE_CHANGEDIR);
    RSDK_ENUM_VAR("Entrance", TRANSPORTTUBE_ENTRY);
    RSDK_ENUM_VAR("Start Sequence (Next Slot)", TRANSPORTTUBE_TOTARGET_NEXT);
    RSDK_ENUM_VAR("Start Sequence (Prev Slot)", TRANSPORTTUBE_TOTARGET_PREV);
    RSDK_ENUM_VAR("Sequence Node", TRANSPORTTUBE_TOTARGET_NODE);
    RSDK_ENUM_VAR("Junction", TRANSPORTTUBE_JUNCTION);
    RSDK_ENUM_VAR("Exit", TRANSPORTTUBE_EXIT);
}
#endif

void TransportTube_Serialize(void)
{
    RSDK_EDITABLE_VAR(TransportTube, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(TransportTube, VAR_UINT8, dirMask);
}
