#ifndef OBJ_FXTRAIL_H
#define OBJ_FXTRAIL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFXTrail : Object {

};

// Entity Class
struct EntityFXTrail : Entity {

};

// Object Struct
extern ObjectFXTrail *FXTrail;

// Standard Entity Events
void FXTrail_Update();
void FXTrail_LateUpdate();
void FXTrail_StaticUpdate();
void FXTrail_Draw();
void FXTrail_Create(void* data);
void FXTrail_StageLoad();
void FXTrail_EditorDraw();
void FXTrail_EditorLoad();
void FXTrail_Serialize();

// Extra Entity Functions


#endif //!OBJ_FXTRAIL_H
