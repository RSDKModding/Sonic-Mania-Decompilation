#include "RSDK/Core/RetroEngine.hpp"

using namespace RSDK;

ObjectDefaultObject *RSDK::DefaultObject;

void RSDK::DefaultObject_Update()
{
    if (controller[CONT_ANY].keyUp.down) {
        if (screens[CONT_ANY].position.y > 0)
            screens[CONT_ANY].position.y -= 4;
    }
    else if (controller[CONT_ANY].keyDown.down) {
        screens[CONT_ANY].position.y += 4;
    }

    if (controller[CONT_ANY].keyLeft.down) {
        if (screens[CONT_ANY].position.x > 0)
            screens[CONT_ANY].position.x -= 4;
    }
    else if (controller[CONT_ANY].keyRight.down) {
        screens[CONT_ANY].position.x += 4;
    }
}

void RSDK::DefaultObject_LateUpdate() {}

void RSDK::DefaultObject_StaticUpdate() {}

void RSDK::DefaultObject_Draw() {}

void RSDK::DefaultObject_Create(void *data)
{
    RSDK_THIS(DefaultObject);

    self->active                = ACTIVE_ALWAYS;
    DefaultObject->active = ACTIVE_ALWAYS;
}

void RSDK::DefaultObject_StageLoad() {}

void RSDK::DefaultObject_EditorDraw() {}

void RSDK::DefaultObject_EditorLoad() {}

void RSDK::DefaultObject_Serialize() {}
