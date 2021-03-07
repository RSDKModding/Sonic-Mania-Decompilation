#ifndef OBJ_HANDLAUNCHER_H
#define OBJ_HANDLAUNCHER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHandLauncher : Object {

};

// Entity Class
struct EntityHandLauncher : Entity {

};

// Object Struct
extern ObjectHandLauncher *HandLauncher;

// Standard Entity Events
void HandLauncher_Update();
void HandLauncher_LateUpdate();
void HandLauncher_StaticUpdate();
void HandLauncher_Draw();
void HandLauncher_Create(void* data);
void HandLauncher_StageLoad();
void HandLauncher_EditorDraw();
void HandLauncher_EditorLoad();
void HandLauncher_Serialize();

// Extra Entity Functions


#endif //!OBJ_HANDLAUNCHER_H
