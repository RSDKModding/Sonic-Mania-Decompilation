#include "../SonicMania.h"

ObjectAIZTornadoPath *AIZTornadoPath;

void AIZTornadoPath_Update(void)
{
    RSDK_THIS(AIZTornadoPath);
    StateMachine_Run(entity->state);
}

void AIZTornadoPath_LateUpdate(void) {}

void AIZTornadoPath_StaticUpdate(void) {}

void AIZTornadoPath_Draw(void) {}

void AIZTornadoPath_Create(void *data)
{
    RSDK_THIS(AIZTornadoPath);
    if (!RSDK_sceneInfo->inEditor) {
        switch (entity->type) {
            case 0:
                if (!StarPost->postIDs[0]) {
                    entity->active       = ACTIVE_NORMAL;
                    EntityCamera *camera = (EntityCamera *)RSDK.GetEntityByID(SLOT_CAMERA1);
                    if (camera) {
                        camera->state          = StateMachine_None;
                        camera->position.x     = entity->position.x;
                        camera->position.y     = entity->position.y;
                        AIZTornadoPath->camera = (Entity *)camera;
                    }

                    foreach_all(Player, player) { player->camera = NULL; }

                    foreach_all(AIZTornado, tornado) { AIZTornadoPath->tornado = tornado; }
                    RSDK_screens->position.y = (entity->position.y >> 0x10) - RSDK_screens->centerY;
                    entity->speed            = entity->targetSpeed;
                    entity->state            = AIZTornadoPath_Unknown2;
                }
                break;
            case 1: entity->active = ACTIVE_NEVER; break;
            case 2:
                entity->active = ACTIVE_NEVER;
                entity->timer  = 1;
                entity->state  = AIZTornadoPath_Unknown2;
                break;
            case 3:
                entity->active = ACTIVE_NEVER;
                entity->state  = AIZTornadoPath_Unknown3;
                break;
            case 4:
                entity->active = ACTIVE_NEVER;
                entity->state  = AIZTornadoPath_Unknown4;
                break;
            case 5:
                entity->active = ACTIVE_NEVER;
                entity->timer  = 1;
                entity->state  = AIZTornadoPath_Unknown5;
                break;
            case 6:
                entity->state  = AIZTornadoPath_Unknown6;
                entity->active = (StarPost->postIDs[0] > 0) ? ACTIVE_XBOUNDS : ACTIVE_NEVER;
                entity->speed  = entity->targetSpeed;
                break;
            case 7:
                entity->active = ACTIVE_NEVER;
                entity->timer  = 1;
                entity->speed  = entity->targetSpeed;
                entity->state  = AIZTornadoPath_Unknown2;
                break;
            default: return;
        }
    }
}

void AIZTornadoPath_StageLoad(void)
{
    AIZTornadoPath->field_C.x = 0;
    AIZTornadoPath->field_C.y = 0;
}

void AIZTornadoPath_Unknown1(void)
{
    RSDK_THIS(AIZTornadoPath);
    bool32 flag               = false;
    EntityCamera *camera      = (EntityCamera *)AIZTornadoPath->camera;
    EntityAIZTornado *tornado = AIZTornadoPath->tornado;

    int x = 0, y = 0;
    if (camera && camera->position.x >= RSDK_screens->width << 16) {
        x    = camera->position.x;
        y    = camera->position.y;
        flag = 1;
    }
    else {
        x = tornado->field_84;
        y = tornado->field_88;
    }
    EntityAIZTornadoPath *node = (EntityAIZTornadoPath *)RSDK.GetEntityByID(RSDK_sceneInfo->entitySlot + 1);
    int xDist                  = (x - node->position.x) >> 16;
    int yDist                  = (y - node->position.y) >> 16;
    entity->angle              = RSDK.ATan2(xDist, yDist);
    int newPosX                = x - entity->speed * RSDK.Cos256(entity->angle);
    int newPosY                = y - entity->speed * RSDK.Sin256(entity->angle);
    if (flag) {
        camera->position.x = newPosX;
        camera->position.y = newPosY;
    }

    int spd = entity->speed >> 3;
    if (xDist * xDist + yDist * yDist < spd) {
        entity->active = ACTIVE_NEVER;
        node->active   = ACTIVE_NORMAL;
        if (node->easeToSpeed)
            node->speed = entity->speed;
        else
            node->speed = entity->targetSpeed;
    }
    AIZTornadoPath->field_C.x = newPosX - x;
    AIZTornadoPath->field_C.y = newPosY - y;
}

void AIZTornadoPath_Unknown2(void)
{
    RSDK_THIS(AIZTornadoPath);
    if (entity->speed >= entity->targetSpeed) {
        if (entity->speed > entity->targetSpeed) {
            entity->speed -= 16;
            if (entity->speed < entity->targetSpeed)
                entity->speed = entity->targetSpeed;
        }
    }
    else {
        entity->speed += 16;
        if (entity->speed > entity->targetSpeed)
            entity->speed = entity->targetSpeed;
    }

    if (entity->timer > 0) {
        entity->timer--;
    }
    else {
        AIZTornadoPath_Unknown1();
    }
}

void AIZTornadoPath_Unknown3(void)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityCamera *camera = (EntityCamera *)AIZTornadoPath->camera;
    if (camera) {
        camera->state          = Camera_State_Follow;
        player->camera         = camera;
        AIZTornadoPath->camera = NULL;
    }
}

void AIZTornadoPath_Unknown4(void)
{
    RSDK_THIS(AIZTornadoPath);
    EntityPlayer *player   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    player->collisionPlane = 1;
    player->interaction    = false;

    foreach_active(Player, playerPtr) { playerPtr->drawOrder = Zone->playerDrawHigh; }

    AIZTornadoPath_Unknown1();
    entity->state = AIZTornadoPath_Unknown2;
}

void AIZTornadoPath_Unknown5(void)
{
    RSDK_THIS(AIZTornadoPath);

    foreach_active(AIZTornado, tornado) { tornado->drawOrder = Zone->drawOrderLow; }

    if (!tornado->field_94) {
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        EntityCamera *camera = (EntityCamera *)AIZTornadoPath->camera;
        if (camera) {
            camera->state          = Camera_State_Follow;
            player->camera         = camera;
            player->collisionPlane = 0;
            player->interaction    = true;
            player->jumpAbility    = 0;
            player->drawOrder      = Zone->playerDrawLow;
            AIZTornadoPath->camera = NULL;
        }
        player->groundVel = AIZTornadoPath->field_C.x;
        Player_StartJump(player);
        player->right     = true;
        tornado->field_94 = true;
    }

    AIZTornadoPath_Unknown2();
}

void AIZTornadoPath_Unknown6(void)
{
    RSDK_THIS(AIZTornadoPath);
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player->position.x > entity->position.x) {
        player->stateInput = StateMachine_None;
        player->left       = false;
        player->right      = true;
        if (player->pushing > 0) {
            player->jumpPress         = true;
            player->jumpHold          = true;
            entity->state             = AIZTornadoPath_Unknown7;
            AIZTornadoPath->field_C.x = 0;
            AIZTornadoPath->field_C.y = 0;
        }
    }
}

void AIZTornadoPath_Unknown7(void)
{
    RSDK_THIS(AIZTornadoPath);
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    ++entity->timer;
    if (entity->timer == 60) {
        foreach_all(AIZTornado, tornado)
        {
            tornado->position.x = player->position.x - 0x1400000;
            tornado->position.y = player->position.y + 0x400000;
            tornado->dword98    = 0x40000;
            tornado->state      = AIZTornado_Unknown3;
            tornado->active     = ACTIVE_NORMAL;
        }
    }

    if (entity->timer == 90) {
        entity->timer      = 0;
        player->stateInput = Player_ProcessP1Input;
        int xOffset        = 0;
        foreach_all(AIZTornadoPath, node)
        {
            if (node->type == 7) {
                xOffset      = node->position.x - player->position.x;
                node->active = ACTIVE_NORMAL;
            }
        }

        player->position.x += xOffset;
        if (player->camera) {
            player->camera->position.x += xOffset;
            player->camera->state  = StateMachine_None;
            AIZTornadoPath->camera = (Entity *)player->camera;
            player->camera         = NULL;
        }

        foreach_active(AIZTornado, tornado)
        {
            tornado->position.x += xOffset;
            tornado->dword98 = 0x80000;
        }
    }
}

void AIZTornadoPath_EditorDraw(void) {}

void AIZTornadoPath_EditorLoad(void) {}

void AIZTornadoPath_Serialize(void)
{
    RSDK_EDITABLE_VAR(AIZTornadoPath, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(AIZTornadoPath, VAR_ENUM, targetSpeed);
    RSDK_EDITABLE_VAR(AIZTornadoPath, VAR_ENUM, timer);
    RSDK_EDITABLE_VAR(AIZTornadoPath, VAR_BOOL, easeToSpeed);
}
