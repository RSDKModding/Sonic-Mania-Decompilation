#ifndef OBJ_JAWZ_H
#define OBJ_JAWZ_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectJawz : Object {

};

// Entity Class
struct EntityJawz : Entity {

};

// Object Struct
extern ObjectJawz *Jawz;

// Standard Entity Events
void Jawz_Update();
void Jawz_LateUpdate();
void Jawz_StaticUpdate();
void Jawz_Draw();
void Jawz_Create(void* data);
void Jawz_StageLoad();
void Jawz_EditorDraw();
void Jawz_EditorLoad();
void Jawz_Serialize();

// Extra Entity Functions


#endif //!OBJ_JAWZ_H
