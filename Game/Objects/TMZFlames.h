#ifndef OBJ_TMZFLAMES_H
#define OBJ_TMZFLAMES_H

#include "../../SonicMania.h"

//Object Class
class ObjectTMZFlames : public Object {
public:

};

//Entity Class
class EntityTMZFlames : public Entity {
public:

};

//Object Entity
ObjectTMZFlames TMZFlames;

//Entity Functions
void TMZFlames_Update();
void TMZFlames_EarlyUpdate();
void TMZFlames_LateUpdate();
void TMZFlames_Draw();
void TMZFlames_Setup(void* subtype);
void TMZFlames_StageLoad();
void TMZFlames_GetAttributes();

#endif //!OBJ_TMZFLAMES_H
