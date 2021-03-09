#ifndef OBJ_MMZ2OUTRO_H
#define OBJ_MMZ2OUTRO_H

#include "../SonicMania.hpp"

#if RETRO_USE_PLUS
// Object Class
struct ObjectMMZ2Outro : Object {

};

// Entity Class
struct EntityMMZ2Outro : Entity {

};

// Object Struct
extern ObjectMMZ2Outro MMZ2Outro;

// Standard Entity Events
void MMZ2Outro_Update();
void MMZ2Outro_LateUpdate();
void MMZ2Outro_StaticUpdate();
void MMZ2Outro_Draw();
void MMZ2Outro_Create(void* data);
void MMZ2Outro_StageLoad();
void MMZ2Outro_EditorDraw();
void MMZ2Outro_EditorLoad();
void MMZ2Outro_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_MMZ2OUTRO_H
