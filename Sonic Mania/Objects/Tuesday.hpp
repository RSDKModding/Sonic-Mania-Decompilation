#ifndef OBJ_TUESDAY_H
#define OBJ_TUESDAY_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTuesday : Object{

};

// Entity Class
struct EntityTuesday : Entity {

};

// Object Struct
extern ObjectTuesday *Tuesday;

// Standard Entity Events
void Tuesday_Update();
void Tuesday_LateUpdate();
void Tuesday_StaticUpdate();
void Tuesday_Draw();
void Tuesday_Create(void* data);
void Tuesday_StageLoad();
void Tuesday_EditorDraw();
void Tuesday_EditorLoad();
void Tuesday_Serialize();

// Extra Entity Functions


#endif //!OBJ_TUESDAY_H
