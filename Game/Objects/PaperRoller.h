#ifndef OBJ_PAPERROLLER_H
#define OBJ_PAPERROLLER_H

#include "../../SonicMania.h"

//Object Class
class ObjectPaperRoller : public Object {
public:

};

//Entity Class
class EntityPaperRoller : public Entity {
public:

};

//Object Entity
ObjectPaperRoller PaperRoller;

//Entity Functions
void PaperRoller_Update();
void PaperRoller_EarlyUpdate();
void PaperRoller_LateUpdate();
void PaperRoller_Draw();
void PaperRoller_Setup(void* subtype);
void PaperRoller_StageLoad();
void PaperRoller_GetAttributes();

#endif //!OBJ_PAPERROLLER_H
