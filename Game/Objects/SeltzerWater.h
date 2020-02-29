#ifndef OBJ_SELTZERWATER_H
#define OBJ_SELTZERWATER_H

//Object Class
class SeltzerWater : Object {

};

//Entity Class
class EntitySeltzerWater : Entity {

};

//Entity Functions
void SeltzerWater_Update();
void SeltzerWater_EarlyUpdate();
void SeltzerWater_LateUpdate();
void SeltzerWater_Draw();
void SeltzerWater_Setup(void* subtype);
void SeltzerWater_StageLoad();
void SeltzerWater_GetAttributes();

#endif //!OBJ_SELTZERWATER_H
