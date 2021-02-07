#ifndef OBJ_DIALOGRUNNER_H
#define OBJ_DIALOGRUNNER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDialogRunner : Object{

};

// Entity Class
struct EntityDialogRunner : Entity {

};

// Object Struct
extern ObjectDialogRunner *DialogRunner;

// Standard Entity Events
void DialogRunner_Update();
void DialogRunner_LateUpdate();
void DialogRunner_StaticUpdate();
void DialogRunner_Draw();
void DialogRunner_Create(void* data);
void DialogRunner_StageLoad();
void DialogRunner_EditorDraw();
void DialogRunner_EditorLoad();
void DialogRunner_Serialize();

// Extra Entity Functions


#endif //!OBJ_DIALOGRUNNER_H
