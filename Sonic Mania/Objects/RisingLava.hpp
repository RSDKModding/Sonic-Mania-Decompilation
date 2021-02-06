#ifndef OBJ_RISINGLAVA_H
#define OBJ_RISINGLAVA_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectRisingLava : Object {

};

// Entity Class
struct EntityRisingLava : Entity {

};

// Object Entity
extern ObjectRisingLava RisingLava;

// Standard Entity Events
void RisingLava_Update();
void RisingLava_LateUpdate();
void RisingLava_StaticUpdate();
void RisingLava_Draw();
void RisingLava_Create(void* data);
void RisingLava_StageLoad();
void RisingLava_EditorDraw();
void RisingLava_EditorLoad();
void RisingLava_Serialize();

// Extra Entity Functions


#endif //!OBJ_RISINGLAVA_H
