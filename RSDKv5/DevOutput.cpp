#include "RetroEngine.hpp"

#if RETRO_REV02
ObjectDevOutput *DevOutput;

void DevOutput_Update()
{
    EntityDevOutput *entity = (EntityDevOutput *)sceneInfo.entity;

    switch (entity->type) {
        case 0:
            if (entity->id <= 0)
                entity->type = 1;
            else
                entity->id--;
            break;
        case 1:
            if (entity->position.y >= 0)
                entity->type = 2;
            else
                entity->position.y += 2;
            break;
        case 2:
            if (entity->id >= 120)
                entity->type = 3;
            else
                entity->id++;
            break;
        case 3:
            entity->position.y -= 2;
            if (-entity->position.y > entity->yOffset)
                ResetEntityPtr(entity, 0, NULL);
            break;
        default: break;
    }
}

#if !RETRO_USE_ORIGINAL_CODE
int DevOutput_CheckTouchRect(int x1, int y1, int x2, int y2)
{
    for (int t = 0; t < touchMouseData.count; ++t) {
        int tx = (touchMouseData.x[t] * screens->width);
        int ty = (touchMouseData.y[t] * screens->height);

        if (touchMouseData.down[t]) {
            if (tx >= x1 && ty >= y1 && tx <= x2 && ty <= y2)
                return t;
        }
    }
    return -1;
}

int DevOutput_UpCount    = 0;
int DevOutput_DownCount  = 0;
int DevOutput_LeftCount  = 0;
int DevOutput_RightCount = 0;
int DevOutput_JumpCount  = 0;
#endif

void DevOutput_LateUpdate() {}
void DevOutput_StaticUpdate() {

#if !RETRO_USE_ORIGINAL_CODE
    if (!CheckSceneFolder("Menu")) {
        int touchID = DevOutput_CheckTouchRect(0, 96, screens->centerX, screens->height);
        if (touchID >= 0) {
            int tx = (touchMouseData.x[touchID] * screens->width);
            int ty = (touchMouseData.y[touchID] * screens->height);
            tx -= 64;
            ty -= 192;

            switch (((ArcTanLookup(tx, ty) + 32) & 0xFF) >> 6) {
                case 0:
                    DevOutput_UpCount   = 0;
                    DevOutput_DownCount = 0;
                    DevOutput_LeftCount = 0;
                    ++DevOutput_RightCount;
                    controller->keyRight.press   = DevOutput_RightCount == 1;
                    controller[1].keyRight.press = DevOutput_RightCount == 1;
                    controller->keyRight.down    = DevOutput_RightCount != 0;
                    controller[1].keyRight.down  = DevOutput_RightCount != 0;
                    break;
                case 1:
                    DevOutput_UpCount = 0;
                    ++DevOutput_DownCount;
                    DevOutput_LeftCount         = 0;
                    DevOutput_RightCount        = 0;
                    controller->keyDown.press   = DevOutput_DownCount == 1;
                    controller[1].keyDown.press = DevOutput_DownCount == 1;
                    controller->keyDown.down    = DevOutput_DownCount != 0;
                    controller[1].keyDown.down  = DevOutput_DownCount != 0;
                    break;
                case 2:
                    DevOutput_UpCount   = 0;
                    DevOutput_DownCount = 0;
                    ++DevOutput_LeftCount;
                    DevOutput_RightCount        = 0;
                    controller->keyLeft.press   = DevOutput_LeftCount == 1;
                    controller[1].keyLeft.press = DevOutput_LeftCount == 1;
                    controller->keyLeft.down    = DevOutput_LeftCount != 0;
                    controller[1].keyLeft.down  = DevOutput_LeftCount != 0;
                    break;
                case 3:
                    ++DevOutput_UpCount;
                    DevOutput_DownCount       = 0;
                    DevOutput_LeftCount       = 0;
                    DevOutput_RightCount      = 0;
                    controller->keyUp.press   = DevOutput_UpCount == 1;
                    controller[1].keyUp.press = DevOutput_UpCount == 1;
                    controller->keyUp.down    = DevOutput_UpCount != 0;
                    controller[1].keyUp.down  = DevOutput_UpCount != 0;
                    break;
            }
        }

        touchID = DevOutput_CheckTouchRect(screens->centerX, 96, screens->width, screens->height);
        if (touchID >= 0) {
            DevOutput_JumpCount++;
            controller->keyA.press   = DevOutput_JumpCount == 1;
            controller[1].keyA.press = DevOutput_JumpCount == 1;
            controller->keyA.down    = DevOutput_JumpCount != 0;
            controller[1].keyA.down  = DevOutput_JumpCount != 0;
        }
        else {
            DevOutput_JumpCount = 0;
        }
    }
#endif
}

void DevOutput_Draw()
{
    EntityDevOutput *entity = (EntityDevOutput *)sceneInfo.entity;
    DrawRectangle(0, 0, currentScreen->width, entity->position.y + entity->yOffset, 128, 255, INK_NONE, true);
    DrawDevText(8, entity->message, entity->position.y + 8, 0, 0xF0F0F0);
}

void DevOutput_Create(void *source)
{
    EntityDevOutput *entity = (EntityDevOutput *)sceneInfo.entity;
    entity->active          = ACTIVE_ALWAYS;
    entity->visible         = true;
    entity->isPermanent     = true;
    entity->drawOrder       = 15;
    strncpy(entity->message, (char *)source, 0x3F4);
    entity->id         = 180 * GetEntityCount(DevOutput->objectID, 0);
    entity->yOffset    = DevOutput_GetStringYOffset(entity->message);
    entity->position.y = -entity->yOffset;
}

void DevOutput_StageLoad()
{
#if !RETRO_USE_ORIGINAL_CODE
    DevOutput->active = ACTIVE_ALWAYS;
#endif 
}

int DevOutput_GetStringYOffset(char *string)
{
    char cur = *string;
    int v2   = 0;
    int v4   = 0;

    if (!*string)
        return 24;

    do {
        bool32 isNewLine = cur == '\n';
        cur              = *++string;
        v4               = v2 + 1;
        if (!isNewLine)
            v4 = v2;
        v2 = v4;
    } while (cur);
    if (v4 >= 1)
        return 8 * v4 + 16;
    else
        return 24;
}

void DevOutput_EditorDraw() {}
void DevOutput_EditorLoad() {}
void DevOutput_Serialize() {}
#endif
