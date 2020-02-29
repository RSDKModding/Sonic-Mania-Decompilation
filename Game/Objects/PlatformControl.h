#ifndef OBJ_PLATFORMCONTROL_H
#define OBJ_PLATFORMCONTROL_H

#include "../../SonicMania.h"

//Object Class
class ObjectPlatformControl : public Object {
public:

};

//Entity Class
class EntityPlatformControl : public Entity {
public:

};

//Object Entity
ObjectPlatformControl PlatformControl;

//Entity Functions
void PlatformControl_Update();
void PlatformControl_EarlyUpdate();
void PlatformControl_LateUpdate();
void PlatformControl_Draw();
void PlatformControl_Setup(void* subtype);
void PlatformControl_StageLoad();
void PlatformControl_GetAttributes();

#endif //!OBJ_PLATFORMCONTROL_H
