#ifndef OBJ_RPLANESHIFTER_H
#define OBJ_RPLANESHIFTER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectRPlaneShifter : Object {

};

// Entity Class
struct EntityRPlaneShifter : Entity {

};

// Object Struct
extern ObjectRPlaneShifter *RPlaneShifter;

// Standard Entity Events
void RPlaneShifter_Update();
void RPlaneShifter_LateUpdate();
void RPlaneShifter_StaticUpdate();
void RPlaneShifter_Draw();
void RPlaneShifter_Create(void* data);
void RPlaneShifter_StageLoad();
void RPlaneShifter_EditorDraw();
void RPlaneShifter_EditorLoad();
void RPlaneShifter_Serialize();

// Extra Entity Functions


#endif //!OBJ_RPLANESHIFTER_H
