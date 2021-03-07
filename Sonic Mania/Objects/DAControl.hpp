#ifndef OBJ_DACONTROL_H
#define OBJ_DACONTROL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDAControl : Object {

};

// Entity Class
struct EntityDAControl : Entity {

};

// Object Struct
extern ObjectDAControl *DAControl;

// Standard Entity Events
void DAControl_Update();
void DAControl_LateUpdate();
void DAControl_StaticUpdate();
void DAControl_Draw();
void DAControl_Create(void* data);
void DAControl_StageLoad();
void DAControl_EditorDraw();
void DAControl_EditorLoad();
void DAControl_Serialize();

// Extra Entity Functions


#endif //!OBJ_DACONTROL_H
