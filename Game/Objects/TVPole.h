#ifndef OBJ_TVPOLE_H
#define OBJ_TVPOLE_H

#include "../../SonicMania.h"

//Object Class
class ObjectTVPole : public Object {
public:

};

//Entity Class
class EntityTVPole : public Entity {
public:

};

//Object Entity
ObjectTVPole TVPole;

//Entity Functions
void TVPole_Update();
void TVPole_EarlyUpdate();
void TVPole_LateUpdate();
void TVPole_Draw();
void TVPole_Setup(void* subtype);
void TVPole_StageLoad();
void TVPole_GetAttributes();

#endif //!OBJ_TVPOLE_H
