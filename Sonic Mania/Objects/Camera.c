#include "../SonicMania.h"

ObjectCamera *Camera;

void Camera_Update()
{
    EntityCamera *entity = (EntityCamera *)RSDK_sceneInfo->entity;
    entity->lastPos.x = entity->position.x;
    entity->lastPos.y = entity->position.y;
    if (entity->state)
        entity->state();
    entity->velocity.x = entity->position.x - entity->lastPos.x;
    entity->velocity.y = entity->position.y - entity->lastPos.y;
    Camera_SetCameraBounds(entity);

    if (entity->shakePos.x) {
        if (entity->shakePos.x <= 0)
            entity->shakePos.x = -1 - entity->shakePos.x;
        else
            entity->shakePos.x = -entity->shakePos.x;
    }

    if (entity->shakePos.y) {
        if (entity->shakePos.y <= 0)
            entity->shakePos.y = -1 - entity->shakePos.y;
        else
            entity->shakePos.y = -entity->shakePos.y;
    }
    if (!entity->field_94) {
        int pos = entity->field_90 - (entity->field_90 >> 3);
        if (pos < 0)
            pos = 0;
        entity->field_90 = pos;
    }
}

void Camera_LateUpdate()
{

}

void Camera_StaticUpdate()
{
    if (Camera->centerBounds.x < 0x100000)
        Camera->centerBounds.x += 0x4000;
    if (Camera->centerBounds.y < 0x180000)
        Camera->centerBounds.y += 0x8000;
}

void Camera_Draw()
{

}

void Camera_Create(void* data)
{
    int screen           = (int)(size_t)data;
    EntityCamera *entity = (EntityCamera *)RSDK_sceneInfo->entity;
    entity->field_8C = 0x80000;
    entity->field_98 = 104;
    if (entity->active != ACTIVE_NORMAL) {
        entity->screenID = screen;
        //RSDK.AddScreen(&entity->center, RSDK_screens[screen].centerX << 16, RSDK_screens[screen].centerY << 16, false);
    }
    entity->boundsOffset.x = 3;
    entity->boundsOffset.y = 2;
    entity->active    = ACTIVE_NORMAL;
    if (!Zone->field_158) {
        entity->boundsL = Zone->screenBoundsL1[entity->screenID];
        entity->boundsR = Zone->screenBoundsR1[entity->screenID];
        entity->boundsT = Zone->screenBoundsT1[entity->screenID];
        entity->boundsB = Zone->screenBoundsB1[entity->screenID];

        if (entity->targetPtr) {
            entity->position.x = entity->targetPtr->position.x;
            entity->position.y = entity->targetPtr->position.y;
            entity->state    = Camera_State_Follow;
        }
        else {
            entity->state = Camera_State_Roam;
        }
    }
}

void Camera_StageLoad()
{
    if (!RSDK.CheckStageFolder("Credits")) {
        for (int i = 0; i < RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT); ++i) RSDK.ResetEntitySlot(i + SLOT_CAMERA1, Camera->objectID, (void *)(size_t)i);
        Camera->centerBounds.x = 0x100000;
        Camera->centerBounds.y = 0x180000;
    }
}

void Camera_SetCameraBounds(EntityCamera *entity)
{
    ScreenInfo *screen                                    = RSDK_screens;
    RSDK_screens[entity->screenID].position.x = (entity->position.x >> 0x10) + entity->lookPos.x - RSDK_screens[entity->screenID].centerX;
    screen[entity->screenID].position.y       = (entity->position.y >> 0x10) + entity->lookPos.y - entity->field_98;

    if (screen[entity->screenID].position.x < entity->boundsL)
        screen[entity->screenID].position.x = entity->boundsL;

    if (screen[entity->screenID].width + screen[entity->screenID].position.x > entity->boundsR)
        screen[entity->screenID].position.x = entity->boundsR - screen[entity->screenID].width;

    if (screen[entity->screenID].position.y < entity->boundsT)
        screen[entity->screenID].position.y = entity->boundsT;

    if (screen[entity->screenID].height + screen[entity->screenID].position.y > entity->boundsB)
        screen[entity->screenID].position.y = entity->boundsB - screen[entity->screenID].height;

    screen[entity->screenID].position.x += entity->shakePos.x;
    screen[entity->screenID].position.y += entity->shakePos.y;
    entity->center.x = screen[entity->screenID].position.x + screen[entity->screenID].centerX;
    entity->center.y = screen[entity->screenID].position.y + screen[entity->screenID].centerY;
}
EntityCamera *Camera_SetTargetEntity(int screen, Entity *target)
{
    EntityCamera *entity = NULL;
    while (RSDK.GetEntities(Camera->objectID, (Entity **)&entity)) {
        if (entity->screenID == screen) {
            entity->targetPtr    = target;
            entity->position.x   = target->position.x;
            entity->position.y   = target->position.y;
            RSDK.BreakForeachLoop();
            return entity;
        }
    }
    return NULL;
}
void Camera_ShakeScreen(int shakeX, int screen, int shakeY)
{
    EntityCamera *entity = NULL;
    while (RSDK.GetEntities(Camera->objectID, (Entity **)&entity)) {
        if (entity->screenID == screen) {
            entity->shakePos.x = shakeX;
            entity->shakePos.y = shakeY;
            RSDK.BreakForeachLoop();
        }
    }
}
void Camera_HandleHBounds()
{
    ScreenInfo *screen = RSDK_screens;
    EntityCamera *entity = (EntityCamera *)RSDK_sceneInfo->entity;

    if (Zone->screenBoundsL1[entity->screenID] > entity->boundsL) {
        if (entity->boundsL > Zone->screenBoundsL1[entity->screenID])
            entity->boundsL = Zone->screenBoundsL1[entity->screenID];
        else
            entity->boundsL = RSDK_screens[entity->screenID].position.x;
    }

    if (Zone->screenBoundsL1[entity->screenID] < entity->boundsL) {
        if (screen[entity->screenID].position.x <= entity->boundsL) {
            int off              = entity->boundsL - entity->boundsOffset.x;
            entity->boundsL      = off;
            if (entity->velocity.x < 0) {
                entity->boundsL = (entity->velocity.x >> 0x10) + off;
                if (entity->boundsL < Zone->screenBoundsL1[entity->screenID])
                    entity->boundsL = Zone->screenBoundsL1[entity->screenID];
            }
        }
        else {
            entity->boundsL = Zone->screenBoundsL1[entity->screenID];
        }
    }

    if (Zone->screenBoundsR1[entity->screenID] < entity->boundsR) {
        if (screen[entity->screenID].width + screen[entity->screenID].position.x < Zone->screenBoundsR1[entity->screenID])
            entity->boundsR = Zone->screenBoundsR1[entity->screenID];
        else
            entity->boundsR = screen[entity->screenID].width + screen[entity->screenID].position.x;
    }

    if (Zone->screenBoundsR1[entity->screenID] > entity->boundsR) {
        if (screen[entity->screenID].width + screen[entity->screenID].position.x >= entity->boundsR) {
            entity->boundsR += entity->boundsOffset.x;
            if (entity->velocity.x > 0) {
                entity->boundsR = (entity->velocity.x >> 0x10) + entity->boundsR;
                if (entity->boundsR > Zone->screenBoundsR1[entity->screenID])
                    entity->boundsR = Zone->screenBoundsR1[entity->screenID];
            }
        }
        else {
            entity->boundsR = Zone->screenBoundsR1[entity->screenID];
        }
    }

    Zone->screenBoundsL2[entity->screenID] = entity->boundsL << 16;
    Zone->screenBoundsR2[entity->screenID] = entity->boundsR << 16;
}
void Camera_HandleVBounds()
{
    ScreenInfo *screen   = RSDK_screens;
    EntityCamera *entity = (EntityCamera *)RSDK_sceneInfo->entity;

    if (Zone->screenBoundsT1[entity->screenID] > entity->boundsT) {
        if (entity->boundsT > Zone->screenBoundsT1[entity->screenID])
            entity->boundsT = Zone->screenBoundsT1[entity->screenID];
        else
            entity->boundsT = RSDK_screens[entity->screenID].position.y;
    }

    if (Zone->screenBoundsT1[entity->screenID] < entity->boundsT) {
        if (screen[entity->screenID].position.y <= entity->boundsT) {
            int off         = entity->boundsT - entity->boundsOffset.y;
            entity->boundsT = off;
            if (entity->velocity.y < 0) {
                entity->boundsT = (entity->velocity.y >> 0x10) + off;
                if (entity->boundsT < Zone->screenBoundsT1[entity->screenID])
                    entity->boundsT = Zone->screenBoundsT1[entity->screenID];
            }
        }
        else {
            entity->boundsT = Zone->screenBoundsT1[entity->screenID];
        }
    }

    if (Zone->screenBoundsB1[entity->screenID] < entity->boundsB) {
        if (screen[entity->screenID].height + screen[entity->screenID].position.y < Zone->screenBoundsB1[entity->screenID])
            entity->boundsB = Zone->screenBoundsB1[entity->screenID];
        else
            entity->boundsB = screen[entity->screenID].height + screen[entity->screenID].position.y;
    }

    if (Zone->screenBoundsB1[entity->screenID] > entity->boundsB) {
        if (screen[entity->screenID].height + screen[entity->screenID].position.y >= entity->boundsB) {
            entity->boundsB += entity->boundsOffset.y;
            if (entity->velocity.y > 0) {
                entity->boundsB = (entity->velocity.y >> 0x10) + entity->boundsB;
                if (entity->boundsB > Zone->screenBoundsB1[entity->screenID])
                    entity->boundsB = Zone->screenBoundsB1[entity->screenID];
            }
        }
        else {
            entity->boundsB = Zone->screenBoundsB1[entity->screenID];
        }
    }

    Zone->screenBoundsT2[entity->screenID] = entity->boundsT << 16;
    Zone->screenBoundsB2[entity->screenID] = entity->boundsB << 16;
}

// States
void Camera_State_Roam()
{
    EntityCamera *entity = (EntityCamera *)RSDK_sceneInfo->entity;
    int speed = 0x100000;
    if (!RSDK_controller[0].keyA.down)
        speed = 0x40000;

    if (RSDK_controller[0].keyUp.down) {
        entity->position.y -= speed;
    }
    else if (RSDK_controller[0].keyDown.down) {
        entity->position.y += speed;
    }
    if (RSDK_controller[0].keyLeft.down) {
        entity->position.x -= speed;
    }
    else if (RSDK_controller[0].keyRight.down) {
        entity->position.x += speed;
    }

    entity->position.x      = entity->position.x >> 0x10;
    entity->position.y      = entity->position.y >> 0x10;
    ScreenInfo *screen      = &RSDK_screens[entity->screenID];

    if (entity->position.x >= screen->centerX) {
        if (entity->position.x > Zone->screenBoundsR1[entity->screenID] - screen->centerX)
            entity->position.x = Zone->screenBoundsR1[entity->screenID] - screen->centerX;
    }
    else {
        entity->position.x = screen->centerX;
    }

    if (entity->position.y >= screen->centerY) {
        if (entity->position.y > Zone->screenBoundsB1[entity->screenID] - screen->centerY)
            entity->position.y = Zone->screenBoundsB1[entity->screenID] - screen->centerY;
        entity->position.x <<= 0x10;
        entity->position.y <<= 0x10;
    }
    else {
        entity->position.x <<= 0x10;
        entity->position.y <<= 0x10;
    }
}
void Camera_State_Follow()
{
    EntityCamera *entity = (EntityCamera *)RSDK_sceneInfo->entity;
    if (entity->targetPtr) {
        Camera_HandleHBounds();
        Camera_HandleVBounds();
        Entity *target = entity->targetPtr;
        target->position.x += entity->field_6C.x;
        if (target->position.x <= entity->position.x + entity->field_8C) {
            if (target->position.x < entity->position.x - entity->field_8C) {
                int pos = target->position.x + entity->field_8C - entity->position.x;
                if (pos < -Camera->centerBounds.x)
                    pos = -Camera->centerBounds.x;
                entity->position.x = entity->position.x + pos;
            }
            target->position.x -= entity->field_6C.x;
        }
        else {
            int pos = target->position.x - entity->position.x - entity->field_8C;
            if (pos > Camera->centerBounds.x)
                pos = Camera->centerBounds.x;
            entity->position.x = entity->position.x + pos;
            target->position.x -= entity->field_6C.x;
        }

        target->position.y += entity->field_6C.y;
        int adjust = target->position.y - entity->adjustY;
        if (adjust <= entity->position.y + entity->field_90) {
            if (adjust < entity->position.y - entity->field_90) {
                int pos = target->position.y + entity->field_90 - entity->position.y - entity->adjustY;
                if (pos < -Camera->centerBounds.y)
                    pos = -Camera->centerBounds.y;
                entity->position.y = entity->position.y + pos;
            }
            target->position.y -= entity->field_6C.y;
        }
        else {
            int pos = adjust - entity->position.y - entity->field_90;
            if (pos > Camera->centerBounds.y)
                pos = Camera->centerBounds.y;
            entity->position.y = entity->position.y + pos;
            target->position.y -= entity->field_6C.y;
        }

    }
}
void Camera_State_HLock()
{
    EntityCamera *entity = (EntityCamera *)RSDK_sceneInfo->entity;
    if (entity->targetPtr) {
        Camera_HandleHBounds();
        Entity *target = entity->targetPtr;
        target->position.x += entity->field_6C.x;
        if (target->position.x <= entity->position.x + entity->field_8C) {
            if (target->position.x < entity->position.x - entity->field_8C) {
                int pos = target->position.x + entity->field_8C - entity->position.x;
                if (pos < -Camera->centerBounds.x)
                    pos = -Camera->centerBounds.x;
                entity->position.x = entity->position.x + pos;
            }
            target->position.x -= entity->field_6C.x;
        }
        else {
            int pos = target->position.x - entity->position.x - entity->field_8C;
            if (pos > Camera->centerBounds.x)
                pos = Camera->centerBounds.x;
            entity->position.x = entity->position.x + pos;
            target->position.x -= entity->field_6C.x;
        }
    }
}
void Camera_State_VLock()
{
    EntityCamera *entity = (EntityCamera *)RSDK_sceneInfo->entity;
    if (entity->targetPtr) {
        Camera_HandleVBounds();
        Entity *target = entity->targetPtr;
        target->position.y += entity->field_6C.y;
        int adjust = target->position.y - entity->adjustY;
        if (adjust <= entity->position.y + entity->field_90) {
            if (adjust < entity->position.y - entity->field_90) {
                int pos = target->position.y + entity->field_90 - entity->position.y - entity->adjustY;
                if (pos < -Camera->centerBounds.y)
                    pos = -Camera->centerBounds.y;
                entity->position.y = entity->position.y + pos;
            }
            target->position.y -= entity->field_6C.y;
        }
        else {
            int pos = adjust - entity->position.y - entity->field_90;
            if (pos > Camera->centerBounds.y)
                pos = Camera->centerBounds.y;
            entity->position.y = entity->position.y + pos;
            target->position.y -= entity->field_6C.y;
        }
    }
}

void Camera_EditorDraw()
{

}

void Camera_EditorLoad()
{

}

void Camera_Serialize()
{

}

