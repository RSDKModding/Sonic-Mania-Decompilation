#ifndef OBJ_SELTZERBOTTLE_H
#define OBJ_SELTZERBOTTLE_H

//Object Class
class SeltzerBottle : Object {

};

//Entity Class
class EntitySeltzerBottle : Entity {

};

//Entity Functions
void SeltzerBottle_Update();
void SeltzerBottle_EarlyUpdate();
void SeltzerBottle_LateUpdate();
void SeltzerBottle_Draw();
void SeltzerBottle_Setup(void* subtype);
void SeltzerBottle_StageLoad();
void SeltzerBottle_GetAttributes();

#endif //!OBJ_SELTZERBOTTLE_H
