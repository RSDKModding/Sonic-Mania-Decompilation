#ifndef OBJ_ZONE_H
#define OBJ_ZONE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectZone : Object{

};

// Entity Class
struct EntityZone : Entity {

};

// Object Struct
extern ObjectZone *Zone;

// Standard Entity Events
void Zone_Update();
void Zone_LateUpdate();
void Zone_StaticUpdate();
void Zone_Draw();
void Zone_Create(void* data);
void Zone_StageLoad();
void Zone_EditorDraw();
void Zone_EditorLoad();
void Zone_Serialize();

// Extra Entity Functions


#endif //!OBJ_ZONE_H
