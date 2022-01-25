#include "RSDK/Core/RetroEngine.hpp"

RSDK::ObjectDefaultObject *RSDK::DefaultObject;

void RSDK::DefaultObject_Update()
{
    if (controller[0].keyUp.down) {
        if (screens[0].position.y > 0)
            screens[0].position.y -= 4;
    }
    else if (controller[0].keyDown.down) {
        screens[0].position.y += 4;
    }

    if (controller[0].keyLeft.down) {
        if (screens[0].position.x > 0)
            screens[0].position.x -= 4;
    }
    else if (controller[0].keyRight.down) {
        screens[0].position.x += 4;
    }
}

void RSDK::DefaultObject_LateUpdate() {}
void RSDK::DefaultObject_StaticUpdate() {}
void RSDK::DefaultObject_Draw() {}

void RSDK::DefaultObject_Create(void *data)
{
    EntityDefaultObject *entity = (EntityDefaultObject *)sceneInfo.entity;
    entity->active              = ACTIVE_ALWAYS;
    RSDK::DefaultObject->active = ACTIVE_ALWAYS;
}

void RSDK::DefaultObject_StageLoad() {}
void RSDK::DefaultObject_EditorDraw() {}
void RSDK::DefaultObject_EditorLoad() {}
void RSDK::DefaultObject_Serialize() {}
