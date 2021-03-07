#ifndef OBJ_JUGGLESAW_H
#define OBJ_JUGGLESAW_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectJuggleSaw : Object {

};

// Entity Class
struct EntityJuggleSaw : Entity {

};

// Object Struct
extern ObjectJuggleSaw *JuggleSaw;

// Standard Entity Events
void JuggleSaw_Update();
void JuggleSaw_LateUpdate();
void JuggleSaw_StaticUpdate();
void JuggleSaw_Draw();
void JuggleSaw_Create(void* data);
void JuggleSaw_StageLoad();
void JuggleSaw_EditorDraw();
void JuggleSaw_EditorLoad();
void JuggleSaw_Serialize();

// Extra Entity Functions


#endif //!OBJ_JUGGLESAW_H
