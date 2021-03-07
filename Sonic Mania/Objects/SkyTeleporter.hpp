#ifndef OBJ_SKYTELEPORTER_H
#define OBJ_SKYTELEPORTER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSkyTeleporter : Object {

};

// Entity Class
struct EntitySkyTeleporter : Entity {

};

// Object Struct
extern ObjectSkyTeleporter *SkyTeleporter;

// Standard Entity Events
void SkyTeleporter_Update();
void SkyTeleporter_LateUpdate();
void SkyTeleporter_StaticUpdate();
void SkyTeleporter_Draw();
void SkyTeleporter_Create(void* data);
void SkyTeleporter_StageLoad();
void SkyTeleporter_EditorDraw();
void SkyTeleporter_EditorLoad();
void SkyTeleporter_Serialize();

// Extra Entity Functions


#endif //!OBJ_SKYTELEPORTER_H
