#ifndef OBJ_RTELEPORTER_H
#define OBJ_RTELEPORTER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectRTeleporter : Object {

};

// Entity Class
struct EntityRTeleporter : Entity {

};

// Object Struct
extern ObjectRTeleporter *RTeleporter;

// Standard Entity Events
void RTeleporter_Update();
void RTeleporter_LateUpdate();
void RTeleporter_StaticUpdate();
void RTeleporter_Draw();
void RTeleporter_Create(void* data);
void RTeleporter_StageLoad();
void RTeleporter_EditorDraw();
void RTeleporter_EditorLoad();
void RTeleporter_Serialize();

// Extra Entity Functions


#endif //!OBJ_RTELEPORTER_H
