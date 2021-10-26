#include "SonicMania.h"

ObjectCamera *Camera;

void Camera_Update(void) {}

void Camera_LateUpdate(void)
{
    RSDK_THIS(Camera);
    entity->lastPos.x = entity->position.x;
    entity->lastPos.y = entity->position.y;
    StateMachine_Run(entity->state);
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

    if (!entity->offsetYFlag) {
        entity->offset.y = maxVal(entity->offset.y - (entity->offset.y >> 3), 0);
    }
}

void Camera_StaticUpdate(void)
{
    if (Camera->centerBounds.x < 0x100000)
        Camera->centerBounds.x += 0x4000;
    if (Camera->centerBounds.y < 0x180000)
        Camera->centerBounds.y += 0x8000;
}

void Camera_Draw(void) {}

void Camera_Create(void *data)
{
    int32 screen = voidToInt(data);
    RSDK_THIS(Camera);
    entity->offset.x = 0x80000;
    entity->centerY  = RSDK_screens->centerY - 16;
    if (entity->active != ACTIVE_NORMAL) {
        entity->screenID = screen;
        RSDK.AddCamera(&entity->center, RSDK_screens[screen].centerX << 16, RSDK_screens[screen].centerY << 16, false);
    }
    entity->boundsOffset.x = 3;
    entity->boundsOffset.y = 2;
    entity->active         = ACTIVE_NORMAL;
    if (!Zone->atlReloadFlag) {
        entity->boundsL = Zone->screenBoundsL1[entity->screenID];
        entity->boundsR = Zone->screenBoundsR1[entity->screenID];
        entity->boundsT = Zone->screenBoundsT1[entity->screenID];
        entity->boundsB = Zone->screenBoundsB1[entity->screenID];

        if (entity->targetPtr) {
            entity->position.x = entity->targetPtr->position.x;
            entity->position.y = entity->targetPtr->position.y;
            entity->state      = Camera_State_Follow;
        }
        else {
            entity->state = Camera_State_Roam;
        }
    }
}

void Camera_StageLoad(void)
{
    if (!RSDK.CheckStageFolder("Credits")) {
        for (int32 i = 0; i < RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT); ++i) RSDK.ResetEntitySlot(SLOT_CAMERA1 + i, Camera->objectID, intToVoid(i));
        Camera->centerBounds.x = 0x100000;
        Camera->centerBounds.y = 0x180000;
    }
}

void Camera_SetCameraBounds(EntityCamera *entity)
{
    ScreenInfo *screen = &RSDK_screens[entity->screenID];
    screen->position.x = (entity->position.x >> 0x10) + entity->lookPos.x - screen->centerX;
    screen->position.y = (entity->position.y >> 0x10) + entity->lookPos.y - entity->centerY;

    if (screen->position.x < entity->boundsL)
        screen->position.x = entity->boundsL;

    if (screen->width + screen->position.x > entity->boundsR)
        screen->position.x = entity->boundsR - screen->width;

    if (screen->position.y < entity->boundsT)
        screen->position.y = entity->boundsT;

    if (screen->height + screen->position.y > entity->boundsB)
        screen->position.y = entity->boundsB - screen->height;

    screen->position.x += entity->shakePos.x;
    screen->position.y += entity->shakePos.y;
    entity->center.x = screen->position.x + screen->centerX;
    entity->center.y = screen->position.y + screen->centerY;
}
EntityCamera *Camera_SetTargetEntity(int32 screen, void *t)
{
    Entity *target = (Entity *)t;

    foreach_all(Camera, camera)
    {
        if (camera->screenID == screen) {
            camera->targetPtr  = target;
            camera->position.x = target->position.x;
            camera->position.y = target->position.y;
            foreach_return camera;
        }
    }
    return NULL;
}
void Camera_ShakeScreen(int32 shakeX, int32 screen, int32 shakeY)
{
    foreach_all(Camera, camera)
    {
        if (camera->screenID == screen) {
            camera->shakePos.x = shakeX;
            camera->shakePos.y = shakeY;
            foreach_break;
        }
    }
}
void Camera_HandleHBounds(void)
{
    RSDK_THIS(Camera);
    ScreenInfo *screen = &RSDK_screens[entity->screenID];

    if (Zone->screenBoundsL1[entity->screenID] > entity->boundsL) {
        if (screen->position.x > Zone->screenBoundsL1[entity->screenID])
            entity->boundsL = Zone->screenBoundsL1[entity->screenID];
        else
            entity->boundsL = screen->position.x;
    }

    if (Zone->screenBoundsL1[entity->screenID] < entity->boundsL) {
        if (screen->position.x <= entity->boundsL) {
            int32 off         = entity->boundsL - entity->boundsOffset.x;
            entity->boundsL = off;
            if (entity->velocity.x < 0) {
                entity->boundsL += (entity->velocity.x >> 0x10);
                if (entity->boundsL < Zone->screenBoundsL1[entity->screenID])
                    entity->boundsL = Zone->screenBoundsL1[entity->screenID];
            }
        }
        else {
            entity->boundsL = Zone->screenBoundsL1[entity->screenID];
        }
    }

    if (Zone->screenBoundsR1[entity->screenID] < entity->boundsR) {
        if (screen->width + screen->position.x < Zone->screenBoundsR1[entity->screenID])
            entity->boundsR = Zone->screenBoundsR1[entity->screenID];
        else
            entity->boundsR = screen->width + screen->position.x;
    }

    if (Zone->screenBoundsR1[entity->screenID] > entity->boundsR) {
        if (screen->width + screen->position.x >= entity->boundsR) {
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
void Camera_HandleVBounds(void)
{
    RSDK_THIS(Camera);
    ScreenInfo *screen = &RSDK_screens[entity->screenID];

    if (Zone->screenBoundsT1[entity->screenID] > entity->boundsT) {
        if (screen->position.y <= entity->boundsT)
            entity->boundsT = screen->position.y + entity->boundsOffset.y;
        else
            entity->boundsT = screen->position.y;
    }

    if (Zone->screenBoundsT1[entity->screenID] < entity->boundsT) {
        if (screen->position.y <= entity->boundsT) {
            int32 off         = entity->boundsT - entity->boundsOffset.y;
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
        if (screen->height + screen->position.y >= entity->boundsB)
            entity->boundsB -= 2;
        else
            entity->boundsB = screen->height + screen->position.y;
    }

    if (Zone->screenBoundsB1[entity->screenID] > entity->boundsB) {
        if (screen->height + screen->position.y >= entity->boundsB) {
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

void Camera_SetupLerp(int32 type, int32 screen, int32 x, int32 y, int32 speed)
{
    foreach_all(Camera, camera)
    {
        if (camera->screenID == screen) {
            camera->startLerpPos.x = camera->position.x;
            camera->startLerpPos.y = camera->position.y;
            camera->endLerpPos.x   = x;
            camera->endLerpPos.y   = y;
            camera->lerpType       = type;
            camera->lerpSpeed      = speed;
            camera->state          = Camera_State_HandleLerp;
            foreach_break;
        }
    }
}

// States
void Camera_State_Roam(void)
{
    RSDK_THIS(Camera);
    int32 speed = 0x100000;
    if (!RSDK_controller[CONT_P1].keyA.down)
        speed = 0x40000;

    if (RSDK_controller[CONT_P1].keyUp.down) {
        entity->position.y -= speed;
    }
    else if (RSDK_controller[CONT_P1].keyDown.down) {
        entity->position.y += speed;
    }
    if (RSDK_controller[CONT_P1].keyLeft.down) {
        entity->position.x -= speed;
    }
    else if (RSDK_controller[CONT_P1].keyRight.down) {
        entity->position.x += speed;
    }

    entity->position.x = entity->position.x >> 0x10;
    entity->position.y = entity->position.y >> 0x10;
    ScreenInfo *screen = &RSDK_screens[entity->screenID];

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
void Camera_State_Follow(void)
{
    RSDK_THIS(Camera);
    if (entity->targetPtr) {
        Camera_HandleHBounds();
        Camera_HandleVBounds();
        Entity *target = entity->targetPtr;
        target->position.x += entity->field_6C.x;
        if (target->position.x <= entity->position.x + entity->offset.x) {
            if (target->position.x < entity->position.x - entity->offset.x) {
                int32 pos = target->position.x + entity->offset.x - entity->position.x;
                if (pos < -Camera->centerBounds.x)
                    pos = -Camera->centerBounds.x;
                entity->position.x += pos;
            }
            target->position.x -= entity->field_6C.x;
        }
        else {
            int32 pos = target->position.x - entity->position.x - entity->offset.x;
            if (pos > Camera->centerBounds.x)
                pos = Camera->centerBounds.x;
            entity->position.x += pos;
            target->position.x -= entity->field_6C.x;
        }

        target->position.y += entity->field_6C.y;
        int32 adjust = target->position.y - entity->adjustY;
        if (adjust <= entity->position.y + entity->offset.y) {
            if (adjust < entity->position.y - entity->offset.y) {
                int32 pos = target->position.y + entity->offset.y - entity->position.y - entity->adjustY;
                if (pos < -Camera->centerBounds.y)
                    pos = -Camera->centerBounds.y;
                entity->position.y += pos;
            }
            target->position.y -= entity->field_6C.y;
        }
        else {
            int32 pos = adjust - entity->position.y - entity->offset.y;
            if (pos > Camera->centerBounds.y)
                pos = Camera->centerBounds.y;
            entity->position.y += pos;
            target->position.y -= entity->field_6C.y;
        }
    }
}
void Camera_State_HLock(void)
{
    RSDK_THIS(Camera);
    if (entity->targetPtr) {
        Camera_HandleHBounds();
        Entity *target = entity->targetPtr;
        target->position.x += entity->field_6C.x;
        if (target->position.x <= entity->position.x + entity->offset.x) {
            if (target->position.x < entity->position.x - entity->offset.x) {
                int32 pos = target->position.x + entity->offset.x - entity->position.x;
                if (pos < -Camera->centerBounds.x)
                    pos = -Camera->centerBounds.x;
                entity->position.x = entity->position.x + pos;
            }
            target->position.x -= entity->field_6C.x;
        }
        else {
            int32 pos = target->position.x - entity->position.x - entity->offset.x;
            if (pos > Camera->centerBounds.x)
                pos = Camera->centerBounds.x;
            entity->position.x = entity->position.x + pos;
            target->position.x -= entity->field_6C.x;
        }
    }
}
void Camera_State_VLock(void)
{
    RSDK_THIS(Camera);
    if (entity->targetPtr) {
        Camera_HandleVBounds();
        Entity *target = entity->targetPtr;
        target->position.y += entity->field_6C.y;
        int32 adjust = target->position.y - entity->adjustY;
        if (adjust <= entity->position.y + entity->offset.y) {
            if (adjust < entity->position.y - entity->offset.y) {
                int32 pos = target->position.y + entity->offset.y - entity->position.y - entity->adjustY;
                if (pos < -Camera->centerBounds.y)
                    pos = -Camera->centerBounds.y;
                entity->position.y = entity->position.y + pos;
            }
            target->position.y -= entity->field_6C.y;
        }
        else {
            int32 pos = adjust - entity->position.y - entity->offset.y;
            if (pos > Camera->centerBounds.y)
                pos = Camera->centerBounds.y;
            entity->position.y = entity->position.y + pos;
            target->position.y -= entity->field_6C.y;
        }
    }
}
void Camera_State_HandleLerp(void)
{
    RSDK_THIS(Camera);
    entity->lerpPercent += entity->lerpSpeed;

    switch (entity->lerpType) {
        default: break;
        case 0:
            MathHelpers_Lerp1(&entity->position, entity->lerpPercent, entity->startLerpPos.x, entity->startLerpPos.y, entity->endLerpPos.x,
                                 entity->endLerpPos.y);
            break;
        case 1:
            MathHelpers_Lerp2(&entity->position, entity->lerpPercent, entity->startLerpPos.x, entity->startLerpPos.y, entity->endLerpPos.x,
                                 entity->endLerpPos.y);
            break;
        case 2:
            MathHelpers_Lerp3(&entity->position, entity->lerpPercent, entity->startLerpPos.x, entity->startLerpPos.y, entity->endLerpPos.x,
                                 entity->endLerpPos.y);
            break;
        case 3:
            MathHelpers_Lerp4(&entity->position, entity->lerpPercent, entity->startLerpPos.x, entity->startLerpPos.y, entity->endLerpPos.x,
                                 entity->endLerpPos.y);
            break;
    }

    if (entity->lerpPercent >= 0x100) {
        entity->lerpPercent = 0;
        entity->state       = StateMachine_None;
    }
}

#if RETRO_INCLUDE_EDITOR
void Camera_EditorDraw(void) {}

void Camera_EditorLoad(void) {}
#endif

void Camera_Serialize(void) {}
