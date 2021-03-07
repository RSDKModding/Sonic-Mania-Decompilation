#ifndef OBJ_TMZFLAMES_H
#define OBJ_TMZFLAMES_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTMZFlames : Object {

};

// Entity Class
struct EntityTMZFlames : Entity {

};

// Object Struct
extern ObjectTMZFlames *TMZFlames;

// Standard Entity Events
void TMZFlames_Update();
void TMZFlames_LateUpdate();
void TMZFlames_StaticUpdate();
void TMZFlames_Draw();
void TMZFlames_Create(void* data);
void TMZFlames_StageLoad();
void TMZFlames_EditorDraw();
void TMZFlames_EditorLoad();
void TMZFlames_Serialize();

// Extra Entity Functions


#endif //!OBJ_TMZFLAMES_H
