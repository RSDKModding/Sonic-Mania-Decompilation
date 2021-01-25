#ifndef OBJ_DEBUGMODE_H
#define OBJ_DEBUGMODE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDebugMode : Object {

};

// Entity Class
struct EntityDebugMode : Entity {

};

// Object Entity
extern ObjectDebugMode DebugMode;

// Standard Entity Events
void DebugMode_Update();
void DebugMode_LateUpdate();
void DebugMode_StaticUpdate();
void DebugMode_Draw();
void DebugMode_Create(void* data);
void DebugMode_StageLoad();
void DebugMode_EditorDraw();
void DebugMode_EditorLoad();
void DebugMode_Serialize();

// Extra Entity Functions


#endif //!OBJ_DEBUGMODE_H
