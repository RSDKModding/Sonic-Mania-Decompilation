#ifndef OBJ_TIMEPOST_H
#define OBJ_TIMEPOST_H

#include "../../SonicMania.h"

//Object Class
class ObjectTimePost : public Object {
public:

};

//Entity Class
class EntityTimePost : public Entity {
public:

};

//Object Entity
ObjectTimePost TimePost;

//Entity Functions
void TimePost_Update();
void TimePost_EarlyUpdate();
void TimePost_LateUpdate();
void TimePost_Draw();
void TimePost_Setup(void* subtype);
void TimePost_StageLoad();
void TimePost_GetAttributes();

#endif //!OBJ_TIMEPOST_H
