#ifndef OBJ_PAPERROLLER_H
#define OBJ_PAPERROLLER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPaperRoller : Object {

};

// Entity Class
struct EntityPaperRoller : Entity {

};

// Object Entity
extern ObjectPaperRoller PaperRoller;

// Standard Entity Events
void PaperRoller_Update();
void PaperRoller_LateUpdate();
void PaperRoller_StaticUpdate();
void PaperRoller_Draw();
void PaperRoller_Create(void* data);
void PaperRoller_StageLoad();
void PaperRoller_EditorDraw();
void PaperRoller_EditorLoad();
void PaperRoller_Serialize();

// Extra Entity Functions


#endif //!OBJ_PAPERROLLER_H
