#ifndef OBJ_PLATFORMCONTROL_H
#define OBJ_PLATFORMCONTROL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPlatformControl : Object {

};

// Entity Class
struct EntityPlatformControl : Entity {

};

// Object Entity
extern ObjectPlatformControl PlatformControl;

// Standard Entity Events
void PlatformControl_Update();
void PlatformControl_LateUpdate();
void PlatformControl_StaticUpdate();
void PlatformControl_Draw();
void PlatformControl_Create(void* data);
void PlatformControl_StageLoad();
void PlatformControl_EditorDraw();
void PlatformControl_EditorLoad();
void PlatformControl_Serialize();

// Extra Entity Functions


#endif //!OBJ_PLATFORMCONTROL_H
