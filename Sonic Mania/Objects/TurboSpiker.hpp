#ifndef OBJ_TURBOSPIKER_H
#define OBJ_TURBOSPIKER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTurboSpiker : Object {

};

// Entity Class
struct EntityTurboSpiker : Entity {

};

// Object Struct
extern ObjectTurboSpiker *TurboSpiker;

// Standard Entity Events
void TurboSpiker_Update();
void TurboSpiker_LateUpdate();
void TurboSpiker_StaticUpdate();
void TurboSpiker_Draw();
void TurboSpiker_Create(void* data);
void TurboSpiker_StageLoad();
void TurboSpiker_EditorDraw();
void TurboSpiker_EditorLoad();
void TurboSpiker_Serialize();

// Extra Entity Functions


#endif //!OBJ_TURBOSPIKER_H
