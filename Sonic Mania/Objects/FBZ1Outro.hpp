#ifndef OBJ_FBZ1OUTRO_H
#define OBJ_FBZ1OUTRO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFBZ1Outro : Object {

};

// Entity Class
struct EntityFBZ1Outro : Entity {

};

// Object Struct
extern ObjectFBZ1Outro FBZ1Outro;

// Standard Entity Events
void FBZ1Outro_Update();
void FBZ1Outro_LateUpdate();
void FBZ1Outro_StaticUpdate();
void FBZ1Outro_Draw();
void FBZ1Outro_Create(void* data);
void FBZ1Outro_StageLoad();
void FBZ1Outro_EditorDraw();
void FBZ1Outro_EditorLoad();
void FBZ1Outro_Serialize();

// Extra Entity Functions


#endif //!OBJ_FBZ1OUTRO_H
