#ifndef OBJ_TMZALERT_H
#define OBJ_TMZALERT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTMZAlert : Object {

};

// Entity Class
struct EntityTMZAlert : Entity {

};

// Object Entity
extern ObjectTMZAlert TMZAlert;

// Standard Entity Events
void TMZAlert_Update();
void TMZAlert_LateUpdate();
void TMZAlert_StaticUpdate();
void TMZAlert_Draw();
void TMZAlert_Create(void* data);
void TMZAlert_StageLoad();
void TMZAlert_EditorDraw();
void TMZAlert_EditorLoad();
void TMZAlert_Serialize();

// Extra Entity Functions


#endif //!OBJ_TMZALERT_H
