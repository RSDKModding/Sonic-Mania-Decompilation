#ifndef OBJ_FBZ2OUTRO_H
#define OBJ_FBZ2OUTRO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFBZ2Outro : Object {

};

// Entity Class
struct EntityFBZ2Outro : Entity {

};

// Object Entity
extern ObjectFBZ2Outro FBZ2Outro;

// Standard Entity Events
void FBZ2Outro_Update();
void FBZ2Outro_LateUpdate();
void FBZ2Outro_StaticUpdate();
void FBZ2Outro_Draw();
void FBZ2Outro_Create(void* data);
void FBZ2Outro_StageLoad();
void FBZ2Outro_EditorDraw();
void FBZ2Outro_EditorLoad();
void FBZ2Outro_Serialize();

// Extra Entity Functions


#endif //!OBJ_FBZ2OUTRO_H
