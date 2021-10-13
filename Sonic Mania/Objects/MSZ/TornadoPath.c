#include "SonicMania.h"

ObjectTornadoPath *TornadoPath;

void TornadoPath_Update(void)
{
    RSDK_THIS(TornadoPath);
    StateMachine_Run(entity->state);
}

void TornadoPath_LateUpdate(void) {}

void TornadoPath_StaticUpdate(void) {}

void TornadoPath_Draw(void) {}

void TornadoPath_Create(void *data)
{
    RSDK_THIS(TornadoPath);
    if (!RSDK_sceneInfo->inEditor) {
        switch (entity->type) {
            case 0:
                if (!StarPost->postIDs[0]
                    && PlayerHelpers_CheckPlayerPos(entity->position.x - (entity->size.x >> 1), entity->position.y - (entity->size.y >> 1), 
                                                    entity->position.x + (entity->size.x >> 1), entity->position.y + (entity->size.y >> 1))) {
                    TornadoPath_SetupHitbox();
                    entity->active       = ACTIVE_NORMAL;
                    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
                    if (camera) {
                        camera->state          = 0;
                        camera->position.x     = entity->position.x;
                        camera->position.y     = entity->position.y;
                        TornadoPath->cameraPtr = camera;
                    }
                    foreach_all(Player, player) { player->camera = NULL; }
                    RSDK_screens->position.y = (entity->position.y >> 16) - RSDK_screens->centerY;
                    entity->state            = TornadoPath_Unknown3;
                    entity->targetSpeedStore = entity->targetSpeed;
                }
                break;
            case 1: entity->active = ACTIVE_NEVER; break;
            case 2:
            case 10:
                entity->active = ACTIVE_NEVER;
                entity->timer  = 1;
                entity->state  = TornadoPath_Unknown3;
                break;
            case 3:
                entity->active = ACTIVE_NEVER;
                entity->state  = TornadoPath_State_ReturnCamera;
                break;
            case 4:
                entity->active = ACTIVE_NEVER;
                entity->state  = TornadoPath_Unknown5;
                break;
            case 5:
            case 6:
                entity->active = ACTIVE_NEVER;
                entity->state  = TornadoPath_Unknown6;
                break;
            case 7:
            case 8:
                if (StarPost->postIDs[0]
                    || !PlayerHelpers_CheckPlayerPos(entity->position.y - (entity->size.y >> 1), entity->position.x - (entity->size.x >> 1),
                                                     entity->position.x + (entity->size.x >> 1), entity->position.y + (entity->size.y >> 1))) {
                    entity->active           = ACTIVE_XBOUNDS;
                    entity->state            = TornadoPath_Unknown7;
                    entity->targetSpeedStore = entity->targetSpeed;
                }
                else {
                    entity->active           = ACTIVE_NEVER;
                    entity->state            = TornadoPath_Unknown7;
                    entity->targetSpeedStore = entity->targetSpeed;
                }
                break;
            case 9:
                entity->active           = ACTIVE_NEVER;
                entity->timer            = 1;
                entity->state            = TornadoPath_Unknown9;
                entity->targetSpeedStore = entity->targetSpeed;
                break;
            case 11:
                entity->active = ACTIVE_NEVER;
                entity->state  = TornadoPath_Unknown11;
                break;
            case 12:
                entity->active = ACTIVE_NEVER;
                entity->state  = TornadoPath_Unknown12;
                break;
            case 13:
                entity->active = ACTIVE_NEVER;
                entity->state  = TornadoPath_Unknown13;
                break;
            default: break;
        }
    }
}

void TornadoPath_StageLoad(void)
{
    TornadoPath->field_8.x = 0;
    TornadoPath->field_8.y = 0;
    TornadoPath->hitboxID  = 0;
}

void TornadoPath_SetupHitbox(void)
{
    RSDK_THIS(TornadoPath);
    entity->hitbox.left   = -entity->size.x >> 17;
    entity->hitbox.right  = entity->size.x >> 17;
    entity->hitbox.top    = -(entity->size.y >> 17);
    entity->hitbox.bottom = entity->size.y >> 17;
}

void TornadoPath_Unknown2(void)
{
    RSDK_THIS(TornadoPath);
    EntityCamera *camera = TornadoPath->cameraPtr;
    if (camera) {
        EntityTornadoPath *node = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, TornadoPath);
        int32 y                   = (camera->position.y - node->position.y) >> 16;
        int32 x                   = (camera->position.x - node->position.x) >> 16;
        entity->angle           = RSDK.ATan2(x, y);
        int32 cx                  = camera->position.x & 0xFFFF0000;
        int32 cy                  = camera->position.y & 0xFFFF0000;
        camera->position.x -= entity->targetSpeedStore * RSDK.Cos256(entity->angle);
        camera->position.y -= entity->targetSpeedStore * RSDK.Sin256(entity->angle);
        TornadoPath->hitboxID = entity->fastMode;
        int32 radius            = entity->targetSpeedStore >> 3;
        if (x * x + y * y < radius) {
            entity->active = ACTIVE_NEVER;
            node->active   = ACTIVE_NORMAL;
            if (node->easeToSpeed)
                node->targetSpeedStore = entity->targetSpeedStore;
            else
                node->targetSpeedStore = entity->targetSpeed;
        }
        TornadoPath->field_8.x = (camera->position.x & 0xFFFF0000) - cx;
        TornadoPath->field_8.y = (camera->position.y & 0xFFFF0000) - cy;
    }
}

void TornadoPath_Unknown3(void)
{
    RSDK_THIS(TornadoPath);
    if (entity->type == 10) {
        foreach_active(Tornado, tornado) { tornado->drawOrder = Zone->playerDrawHigh; }
    }

    if (entity->targetSpeedStore >= entity->targetSpeed) {
        if (entity->targetSpeedStore > entity->targetSpeed) {
            entity->targetSpeedStore -= 16;
            if (entity->targetSpeedStore < entity->targetSpeed)
                entity->targetSpeedStore = entity->targetSpeed;
        }
    }
    else {
        entity->targetSpeedStore += 16;
        if (entity->targetSpeedStore > entity->targetSpeed)
            entity->targetSpeedStore = entity->targetSpeed;
    }

    if (entity->forceSpeed > 0)
        entity->targetSpeedStore = entity->forceSpeed;

    if (entity->timer <= 0) {
        TornadoPath_Unknown2();
    }
    else {
        entity->timer--;
    }
}

void TornadoPath_State_ReturnCamera(void)
{
    EntityPlayer *player1         = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    TornadoPath->cameraPtr->state = Camera_State_Follow;
    player1->camera               = TornadoPath->cameraPtr;
}

void TornadoPath_Unknown5(void)
{
    RSDK_THIS(TornadoPath);
    EntityPlayer *player1   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    player1->collisionPlane = 1;
    player1->interaction    = false;

    foreach_active(Tornado, tornado) { tornado->drawOrder = Zone->playerDrawHigh; }

    TornadoPath_Unknown2();
    entity->state = TornadoPath_Unknown3;
}

void TornadoPath_Unknown6(void)
{
    RSDK_THIS(TornadoPath);
    EntityPlayer *player1         = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    TornadoPath->cameraPtr->state = Camera_State_Follow;

    player1->camera         = TornadoPath->cameraPtr;
    player1->collisionPlane = 0;
    player1->interaction    = true;
    player1->jumpAbility    = 0;
    player1->drawOrder      = Zone->playerDrawLow;
    if (player1->groundedStore)
        Player_StartJump(player1);

    foreach_active(Tornado, tornado)
    {
        if (entity->type == 5) {
            tornado->drawOrder = Zone->drawOrderHigh;
            MSZSetup_Unknown4(0);
            tornado->state = Tornado_Unknown8;
        }
        else {
            tornado->drawOrder = Zone->drawOrderLow;
            tornado->state     = Tornado_Unknown9;
        }
    }

    foreach_all(TornadoPath, node)
    {
        bool32 flag = false;
        if (entity->type == 5)
            flag = node->type == 7;
        else
            flag = node->type == 8;
        if (flag)
            node->active = ACTIVE_XBOUNDS;
    }

    TornadoPath->cameraPtr = NULL;
    entity->active         = ACTIVE_NEVER;
}

void TornadoPath_Unknown7(void)
{
    RSDK_THIS(TornadoPath);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    bool32 flag = false;
    foreach_active(Tornado, tornado)
    {
        if (tornado->state == Tornado_Unknown6)
            flag = true;
    }

    if (!flag) {
        if (player1->position.x > entity->position.x && player1->position.y < entity->position.y) {
            entity->active      = ACTIVE_NORMAL;
            player1->stateInput = 0;
            player1->left       = false;
            player1->right      = true;
            if (player1->pushing > 0) {
                if (player1->onGround)
                    player1->jumpPress = true;
                player1->jumpHold = true;
            }
            if (player1->onGround == false) {
                entity->state          = TornadoPath_Unknown8;
                TornadoPath->field_8.x = 0;
                TornadoPath->field_8.y = 0;
            }
        }
    }
}

void TornadoPath_Unknown8(void)
{
    RSDK_THIS(TornadoPath);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    ++entity->timer;
    if (entity->timer == 60) {
        foreach_active(Tornado, tornado)
        {
            tornado->position.x = player1->position.x - 0x1400000;
            tornado->position.y = player1->position.y + 0x400000;
            tornado->field_9C   = 0xA0000;
            tornado->field_70   = 1;
            tornado->state      = Tornado_Unknown6;
            tornado->active     = ACTIVE_NORMAL;
        }
    }

    if (entity->timer == 90) {
        player1->right      = false;
        player1->velocity.x = 0;
        player1->groundVel  = 0;
    }
    if (!player1->velocity.y && player1->state != Player_State_None) {
        player1->stateInput = Player_ProcessP1Input;
        entity->timer       = 0;
        int32 velX            = 0;
        int32 velY            = 0;
        if (entity->type == 8) {
            foreach_all(TornadoPath, node)
            {
                if (node->type == 9) {
                    velX         = node->position.x - player1->position.x;
                    velY         = node->position.y - player1->position.y;
                    node->active = ACTIVE_NORMAL;
                }
            }
        }
        player1->position.x += velX;
        player1->position.y += velY;
        EntityCamera *camera = player1->camera;
        if (camera) {
            camera->position.x += velX;
            camera->position.y += velY;
            camera->state          = StateMachine_None;
            TornadoPath->cameraPtr = camera;
            player1->camera        = NULL;
        }

        foreach_active(Tornado, tornado)
        {
            tornado->position.x += velX;
            tornado->position.y += velY;
            tornado->field_9C = 0x80000;
            tornado->field_70 = 0;
        }
        entity->state = TornadoPath_Unknown3;
    }
}

void TornadoPath_Unknown9(void)
{
    RSDK_THIS(TornadoPath);
    entity->state            = TornadoPath_Unknown10;
    entity->targetSpeedStore = 0;
    TornadoPath->field_8.x   = 0;
    TornadoPath->field_8.y   = 0;
    // if (UberCaterkiller->field_C)
    //    entity->state = TornadoPath_Unknown3;
}

void TornadoPath_Unknown10(void)
{
    RSDK_THIS(TornadoPath);
    entity->targetSpeedStore = 0;
    TornadoPath->field_8.x   = 0;
    TornadoPath->field_8.y   = 0;
    // if (UberCaterkiller->field_C)
    //    entity->state = TornadoPath_Unknown3;
}

void TornadoPath_Unknown11(void)
{
    RSDK_THIS(TornadoPath);
    EntityPlayer *player1    = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    entity->targetSpeedStore = 0;
    TornadoPath->field_8.x   = 0;
    TornadoPath->field_8.y   = 0;
    if (player1->onGround) {
        int32 velocityX = 0;
        int32 velocityY = 0;
        foreach_all(TornadoPath, node)
        {
            if (node->type == 9) {
                velocityX    = node->position.x - player1->position.x;
                velocityY    = node->position.y - player1->position.y;
                node->active = ACTIVE_NORMAL;
            }
        }
        player1->position.x += velocityX;
        player1->position.y += velocityY;
        EntityCamera *camera = TornadoPath->cameraPtr;
        camera->position.x += velocityX;
        camera->position.y += velocityY;
        foreach_active(Tornado, tornado)
        {
            tornado->position.x += velocityX;
            tornado->position.y += velocityY;
            tornado->field_9C = 0x80000;
            tornado->field_70 = 0;
        }
    }
}

void TornadoPath_Unknown12(void)
{
    RSDK_THIS(TornadoPath);
    if (!entity->timer) {
        foreach_all(MSZCutsceneST, cutscene)
        {
            cutscene->active = ACTIVE_NORMAL;
            // cutscene->nodePtr = entity;
        }
    }
}

void TornadoPath_Unknown13(void)
{
    TornadoPath->field_8.x = 0;
    TornadoPath->field_8.y = 0;
    TornadoPath->field_30  = 1;
}

void TornadoPath_EditorDraw(void) {}

void TornadoPath_EditorLoad(void) {}

void TornadoPath_Serialize(void)
{
    RSDK_EDITABLE_VAR(TornadoPath, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(TornadoPath, VAR_ENUM, targetSpeed);
    RSDK_EDITABLE_VAR(TornadoPath, VAR_ENUM, timer);
    RSDK_EDITABLE_VAR(TornadoPath, VAR_BOOL, easeToSpeed);
    RSDK_EDITABLE_VAR(TornadoPath, VAR_BOOL, fastMode);
    RSDK_EDITABLE_VAR(TornadoPath, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(TornadoPath, VAR_ENUM, forceSpeed);
}
