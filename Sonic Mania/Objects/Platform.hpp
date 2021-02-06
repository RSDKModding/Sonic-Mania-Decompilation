#ifndef OBJ_PLATFORM_H
#define OBJ_PLATFORM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPlatform : Object {

};

// Entity Class
struct EntityPlatform : Entity {

};

// Object Entity
extern ObjectPlatform Platform;

// Standard Entity Events
void Platform_Update();
void Platform_LateUpdate();
void Platform_StaticUpdate();
void Platform_Draw();
void Platform_Create(void* data);
void Platform_StageLoad();
void Platform_EditorDraw();
void Platform_EditorLoad();
void Platform_Serialize();

// Extra Entity Functions


#endif //!OBJ_PLATFORM_H
