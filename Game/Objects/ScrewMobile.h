#ifndef OBJ_SCREWMOBILE_H
#define OBJ_SCREWMOBILE_H

//Object Class
class ScrewMobile : Object {

};

//Entity Class
class EntityScrewMobile : Entity {

};

//Entity Functions
void ScrewMobile_Update();
void ScrewMobile_EarlyUpdate();
void ScrewMobile_LateUpdate();
void ScrewMobile_Draw();
void ScrewMobile_Setup(void* subtype);
void ScrewMobile_StageLoad();
void ScrewMobile_GetAttributes();

#endif //!OBJ_SCREWMOBILE_H
