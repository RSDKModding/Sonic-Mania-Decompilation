#ifndef OBJ_SEESAW_H
#define OBJ_SEESAW_H

#include "../../SonicMania.h"

//Object Class
class ObjectSeeSaw : public Object {
public:

};

//Entity Class
class EntitySeeSaw : public Entity {
public:

};

//Object Entity
ObjectSeeSaw SeeSaw;

//Entity Functions
void SeeSaw_Update();
void SeeSaw_EarlyUpdate();
void SeeSaw_LateUpdate();
void SeeSaw_Draw();
void SeeSaw_Setup(void* subtype);
void SeeSaw_StageLoad();
void SeeSaw_GetAttributes();

#endif //!OBJ_SEESAW_H
