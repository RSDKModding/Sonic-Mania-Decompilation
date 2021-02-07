#ifndef OBJ_BLASTER_H
#define OBJ_BLASTER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBlaster : Object{

};

// Entity Class
struct EntityBlaster : Entity {

};

// Object Struct
extern ObjectBlaster *Blaster;

// Standard Entity Events
void Blaster_Update();
void Blaster_LateUpdate();
void Blaster_StaticUpdate();
void Blaster_Draw();
void Blaster_Create(void* data);
void Blaster_StageLoad();
void Blaster_EditorDraw();
void Blaster_EditorLoad();
void Blaster_Serialize();

// Extra Entity Functions


#endif //!OBJ_BLASTER_H
