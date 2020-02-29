#ifndef OBJ_DIALOGRUNNER_H
#define OBJ_DIALOGRUNNER_H

#include "../../SonicMania.h"

//Object Class
class ObjectDialogRunner : public Object {
public:

};

//Entity Class
class EntityDialogRunner : public Entity {
public:

};

//Object Entity
ObjectDialogRunner DialogRunner;

//Entity Functions
void DialogRunner_Update();
void DialogRunner_EarlyUpdate();
void DialogRunner_LateUpdate();
void DialogRunner_Draw();
void DialogRunner_Setup(void* subtype);
void DialogRunner_StageLoad();
void DialogRunner_GetAttributes();

#endif //!OBJ_DIALOGRUNNER_H
