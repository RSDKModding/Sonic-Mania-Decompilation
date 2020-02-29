#ifndef OBJ_TITLEBG_H
#define OBJ_TITLEBG_H

#include "../../SonicMania.h"

//Object Class
class ObjectTitleBG : public Object {
public:

};

//Entity Class
class EntityTitleBG : public Entity {
public:

};

//Object Entity
ObjectTitleBG TitleBG;

//Entity Functions
void TitleBG_Update();
void TitleBG_EarlyUpdate();
void TitleBG_LateUpdate();
void TitleBG_Draw();
void TitleBG_Setup(void* subtype);
void TitleBG_StageLoad();
void TitleBG_GetAttributes();

#endif //!OBJ_TITLEBG_H
