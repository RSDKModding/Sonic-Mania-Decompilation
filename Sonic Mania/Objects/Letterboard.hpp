#ifndef OBJ_LETTERBOARD_H
#define OBJ_LETTERBOARD_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLetterboard : Object {

};

// Entity Class
struct EntityLetterboard : Entity {

};

// Object Struct
extern ObjectLetterboard *Letterboard;

// Standard Entity Events
void Letterboard_Update();
void Letterboard_LateUpdate();
void Letterboard_StaticUpdate();
void Letterboard_Draw();
void Letterboard_Create(void* data);
void Letterboard_StageLoad();
void Letterboard_EditorDraw();
void Letterboard_EditorLoad();
void Letterboard_Serialize();

// Extra Entity Functions


#endif //!OBJ_LETTERBOARD_H
