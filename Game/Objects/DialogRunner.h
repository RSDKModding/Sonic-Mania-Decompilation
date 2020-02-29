#ifndef OBJ_DIALOGRUNNER_H
#define OBJ_DIALOGRUNNER_H

//Object Class
class DialogRunner : Object {

};

//Entity Class
class EntityDialogRunner : Entity {

};

//Entity Functions
void DialogRunner_Update();
void DialogRunner_EarlyUpdate();
void DialogRunner_LateUpdate();
void DialogRunner_Draw();
void DialogRunner_Setup(void* subtype);
void DialogRunner_StageLoad();
void DialogRunner_GetAttributes();

#endif //!OBJ_DIALOGRUNNER_H
