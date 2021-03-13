#include "../SonicMania.h"

ObjectDebugMode *DebugMode;

void DebugMode_Update()
{
    EntityDebugMode *entity = (EntityDebugMode *)RSDK_sceneInfo->entity;

    bool32 flag = false;
#if RETRO_USE_PLUS
    User.Unknown6(0);
#endif
    if (Zone)
        Zone->stageFinishCallback = NULL;
    if (RSDK_controller[0].keyUp.down || (RSDK_stickL[0].vDelta > 0.30000001)) {
        entity->position.y -= entity->velocity.y;
        flag = true;
    }
    else if (RSDK_controller[0].keyDown.down || (RSDK_stickL[0].vDelta < -0.30000001)) {
        entity->position.y += entity->velocity.y;
        flag = true;
    }

    if (RSDK_controller[0].keyLeft.down || (RSDK_stickL[0].hDelta < -0.30000001)) {
        entity->position.x -= entity->velocity.y;
        flag = true;
    }
    else if (RSDK_controller[0].keyRight.down || (RSDK_stickL[0].hDelta > 0.30000001)) {
        entity->position.x += entity->velocity.y;
        flag = true;
    }
    
    if (!flag) {
        entity->velocity.y = 0;
    }
    else {
        entity->velocity.y += 0xC00;
        if (entity->velocity.y > 0x100000)
            entity->velocity.y = 0x100000;
    }

    if (RSDK_controller[0].keyA.press) {
        ++DebugMode->objID;
        DebugMode->objID %= DebugMode->itemCount;
        DebugMode->itemSubType = 0;
    }
    else if (RSDK_controller[0].keyX.press) {
        EntityPlayer *player        = (EntityPlayer *)RSDK_sceneInfo->entity;
        player->objectID            = Player->objectID;
        player->groundVel           = 0;
        player->velocity.x          = 0;
        player->velocity.y          = 0;
        player->state               = Player_State_Air;
        player->collisionPlane      = 0;
        player->tileCollisions      = 1;
        player->interaction         = 1;
        player->visible             = 1;
        player->drawOrder           = Zone->playerDrawLow;
        RSDK_sceneInfo->timeEnabled = true;
        if (RSDK_sceneInfo->minutes == 9 && RSDK_sceneInfo->seconds == 59 && RSDK_sceneInfo->milliseconds == 99) {
            RSDK_sceneInfo->minutes      = 0;
            RSDK_sceneInfo->seconds      = 0;
            RSDK_sceneInfo->milliseconds = 0;
        }
        DebugMode->debugActive = false;
    }
    else if (RSDK_controller[0].keyA.down) {
        if (RSDK_controller[0].keyC.press || RSDK_controller[0].keyY.press) {
            --DebugMode->objID;
            if (DebugMode->objID < 0) {
                DebugMode->objID = DebugMode->itemCount - 1;
            }
            DebugMode->itemSubType = 0;
        }
        if (RSDK_controller[0].keyB.press) {
            int val = DebugMode->itemSubType;
            if (!DebugMode->itemSubType)
                DebugMode->itemSubType = DebugMode->subtypeCount;
            DebugMode->itemSubType = val - 1;
        }
    }
    else if (RSDK_controller[0].keyC.press || RSDK_controller[0].keyY.press) {
        if (DebugMode->spawn[DebugMode->objID])
            DebugMode->spawn[DebugMode->objID]();
    }
    else if (RSDK_controller[0].keyB.press) {
        if (DebugMode->itemSubType >= DebugMode->subtypeCount - 1)
            DebugMode->itemSubType = 0;
        else
            DebugMode->itemSubType++;
    }
}

void DebugMode_LateUpdate()
{

}

void DebugMode_StaticUpdate()
{

}

void DebugMode_Draw()
{
    if (DebugMode->draw[DebugMode->objID])
         DebugMode->draw[DebugMode->objID]();
}

void DebugMode_Create(void* data)
{
    EntityDebugMode *entity = (EntityDebugMode *)RSDK_sceneInfo->entity;
    entity->active        = ACTIVE_NORMAL;
    entity->visible         = true;
}

void DebugMode_StageLoad()
{
    DebugMode->objID       = 0;
    DebugMode->itemCount   = 0;
    DebugMode->debugActive = false;

    for (int i = 0; i < 0x100; ++i) {
        DebugMode->objectIDs[i] = 0;
        DebugMode->draw[i]      = NULL;
        DebugMode->spawn[i]     = DebugMode_NullState;
    }
}

void DebugMode_NullState() {}
void DebugMode_AddObject(ushort id, void (*draw)(), void (*spawn)())
{
    if (DebugMode->itemCount < 0x100) {
        DebugMode->objectIDs[DebugMode->itemCount] = id;
        DebugMode->draw[DebugMode->itemCount]      = draw;
        DebugMode->spawn[DebugMode->itemCount]     = spawn;
        DebugMode->itemCount++;
    }
}

void DebugMode_EditorDraw()
{

}

void DebugMode_EditorLoad()
{

}

void DebugMode_Serialize()
{

}

