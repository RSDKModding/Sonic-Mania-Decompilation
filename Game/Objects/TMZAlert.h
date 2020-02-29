#ifndef OBJ_TMZALERT_H
#define OBJ_TMZALERT_H

#include "../../SonicMania.h"

//Object Class
class ObjectTMZAlert : public Object {
public:

};

//Entity Class
class EntityTMZAlert : public Entity {
public:

};

//Object Entity
ObjectTMZAlert TMZAlert;

//Entity Functions
void TMZAlert_Update();
void TMZAlert_EarlyUpdate();
void TMZAlert_LateUpdate();
void TMZAlert_Draw();
void TMZAlert_Setup(void* subtype);
void TMZAlert_StageLoad();
void TMZAlert_GetAttributes();

#endif //!OBJ_TMZALERT_H
