#ifndef OBJ_TITLESONIC_H
#define OBJ_TITLESONIC_H

#include "../../SonicMania.h"

//Object Class
class ObjectTitleSonic : public Object {
public:

};

//Entity Class
class EntityTitleSonic : public Entity {
public:

};

//Object Entity
ObjectTitleSonic TitleSonic;

//Entity Functions
void TitleSonic_Update();
void TitleSonic_EarlyUpdate();
void TitleSonic_LateUpdate();
void TitleSonic_Draw();
void TitleSonic_Setup(void* subtype);
void TitleSonic_StageLoad();
void TitleSonic_GetAttributes();

#endif //!OBJ_TITLESONIC_H
