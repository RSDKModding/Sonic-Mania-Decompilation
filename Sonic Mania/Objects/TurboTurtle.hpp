#ifndef OBJ_TURBOTURTLE_H
#define OBJ_TURBOTURTLE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTurboTurtle : Object {

};

// Entity Class
struct EntityTurboTurtle : Entity {

};

// Object Struct
extern ObjectTurboTurtle *TurboTurtle;

// Standard Entity Events
void TurboTurtle_Update();
void TurboTurtle_LateUpdate();
void TurboTurtle_StaticUpdate();
void TurboTurtle_Draw();
void TurboTurtle_Create(void* data);
void TurboTurtle_StageLoad();
void TurboTurtle_EditorDraw();
void TurboTurtle_EditorLoad();
void TurboTurtle_Serialize();

// Extra Entity Functions


#endif //!OBJ_TURBOTURTLE_H
