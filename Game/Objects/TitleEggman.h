#ifndef OBJ_TITLEEGGMAN_H
#define OBJ_TITLEEGGMAN_H

#include "../../SonicMania.h"

//Object Class
class ObjectTitleEggman : public Object {
public:

};

//Entity Class
class EntityTitleEggman : public Entity {
public:

};

//Object Entity
ObjectTitleEggman TitleEggman;

//Entity Functions
void TitleEggman_Update();
void TitleEggman_EarlyUpdate();
void TitleEggman_LateUpdate();
void TitleEggman_Draw();
void TitleEggman_Setup(void* subtype);
void TitleEggman_StageLoad();
void TitleEggman_GetAttributes();

#endif //!OBJ_TITLEEGGMAN_H
