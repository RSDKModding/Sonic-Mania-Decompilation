#ifndef OBJ_MONARCHBG_H
#define OBJ_MONARCHBG_H

#include "../../SonicMania.h"

//Object Class
class ObjectMonarchBG : public Object {
public:

};

//Entity Class
class EntityMonarchBG : public Entity {
public:

};

//Object Entity
ObjectMonarchBG MonarchBG;

//Entity Functions
void MonarchBG_Update();
void MonarchBG_EarlyUpdate();
void MonarchBG_LateUpdate();
void MonarchBG_Draw();
void MonarchBG_Setup(void* subtype);
void MonarchBG_StageLoad();
void MonarchBG_GetAttributes();

#endif //!OBJ_MONARCHBG_H
