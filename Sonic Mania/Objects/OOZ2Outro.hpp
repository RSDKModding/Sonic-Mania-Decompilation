#ifndef OBJ_OOZ2OUTRO_H
#define OBJ_OOZ2OUTRO_H

#include "../SonicMania.hpp"

#if RETRO_USE_PLUS
// Object Class
struct ObjectOOZ2Outro : Object {

};

// Entity Class
struct EntityOOZ2Outro : Entity {

};

// Object Struct
extern ObjectOOZ2Outro OOZ2Outro;

// Standard Entity Events
void OOZ2Outro_Update();
void OOZ2Outro_LateUpdate();
void OOZ2Outro_StaticUpdate();
void OOZ2Outro_Draw();
void OOZ2Outro_Create(void* data);
void OOZ2Outro_StageLoad();
void OOZ2Outro_EditorDraw();
void OOZ2Outro_EditorLoad();
void OOZ2Outro_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_OOZ2OUTRO_H
