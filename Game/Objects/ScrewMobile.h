#ifndef OBJ_SCREWMOBILE_H
#define OBJ_SCREWMOBILE_H

#include "../../SonicMania.h"

//Object Class
class ObjectScrewMobile : public Object {
public:

};

//Entity Class
class EntityScrewMobile : public Entity {
public:

};

//Object Entity
ObjectScrewMobile ScrewMobile;

//Entity Functions
void ScrewMobile_Update();
void ScrewMobile_EarlyUpdate();
void ScrewMobile_LateUpdate();
void ScrewMobile_Draw();
void ScrewMobile_Setup(void* subtype);
void ScrewMobile_StageLoad();
void ScrewMobile_GetAttributes();

#endif //!OBJ_SCREWMOBILE_H
