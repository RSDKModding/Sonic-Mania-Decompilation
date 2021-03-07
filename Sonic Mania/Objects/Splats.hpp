#ifndef OBJ_SPLATS_H
#define OBJ_SPLATS_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSplats : Object {

};

// Entity Class
struct EntitySplats : Entity {

};

// Object Struct
extern ObjectSplats *Splats;

// Standard Entity Events
void Splats_Update();
void Splats_LateUpdate();
void Splats_StaticUpdate();
void Splats_Draw();
void Splats_Create(void* data);
void Splats_StageLoad();
void Splats_EditorDraw();
void Splats_EditorLoad();
void Splats_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPLATS_H
