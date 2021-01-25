#ifndef OBJ_TORNADOPATH_H
#define OBJ_TORNADOPATH_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTornadoPath : Object {

};

// Entity Class
struct EntityTornadoPath : Entity {

};

// Object Entity
extern ObjectTornadoPath TornadoPath;

// Standard Entity Events
void TornadoPath_Update();
void TornadoPath_LateUpdate();
void TornadoPath_StaticUpdate();
void TornadoPath_Draw();
void TornadoPath_Create(void* data);
void TornadoPath_StageLoad();
void TornadoPath_EditorDraw();
void TornadoPath_EditorLoad();
void TornadoPath_Serialize();

// Extra Entity Functions


#endif //!OBJ_TORNADOPATH_H
