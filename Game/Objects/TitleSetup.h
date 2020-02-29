#ifndef OBJ_TITLESETUP_H
#define OBJ_TITLESETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectTitleSetup : public Object {
public:

};

//Entity Class
class EntityTitleSetup : public Entity {
public:

};

//Object Entity
ObjectTitleSetup TitleSetup;

//Entity Functions
void TitleSetup_Update();
void TitleSetup_EarlyUpdate();
void TitleSetup_LateUpdate();
void TitleSetup_Draw();
void TitleSetup_Setup(void* subtype);
void TitleSetup_StageLoad();
void TitleSetup_GetAttributes();

#endif //!OBJ_TITLESETUP_H
