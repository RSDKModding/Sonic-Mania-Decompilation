#ifndef OBJ_WATER_H
#define OBJ_WATER_H

//Object Class
class Water : Object {

};

//Entity Class
class EntityWater : Entity {

};

//Entity Functions
void Water_Update();
void Water_EarlyUpdate();
void Water_LateUpdate();
void Water_Draw();
void Water_Setup(void* subtype);
void Water_StageLoad();
void Water_GetAttributes();

#endif //!OBJ_WATER_H
