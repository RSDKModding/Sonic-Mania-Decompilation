#ifndef OBJ_ROCKDRILL_H
#define OBJ_ROCKDRILL_H

//Object Class
class RockDrill : Object {

};

//Entity Class
class EntityRockDrill : Entity {

};

//Entity Functions
void RockDrill_Update();
void RockDrill_EarlyUpdate();
void RockDrill_LateUpdate();
void RockDrill_Draw();
void RockDrill_Setup(void* subtype);
void RockDrill_StageLoad();
void RockDrill_GetAttributes();

#endif //!OBJ_ROCKDRILL_H
