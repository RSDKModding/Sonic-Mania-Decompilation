#ifndef OBJ_ZONE_H
#define OBJ_ZONE_H

//Object Class
class Zone : Object {

};

//Entity Class
class EntityZone : Entity {

};

//Entity Functions
void Zone_Update();
void Zone_EarlyUpdate();
void Zone_LateUpdate();
void Zone_Draw();
void Zone_Setup(void* subtype);
void Zone_StageLoad();
void Zone_GetAttributes();

#endif //!OBJ_ZONE_H
