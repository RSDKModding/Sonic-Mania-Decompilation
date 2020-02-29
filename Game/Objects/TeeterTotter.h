#ifndef OBJ_TEETERTOTTER_H
#define OBJ_TEETERTOTTER_H

//Object Class
class TeeterTotter : Object {

};

//Entity Class
class EntityTeeterTotter : Entity {

};

//Entity Functions
void TeeterTotter_Update();
void TeeterTotter_EarlyUpdate();
void TeeterTotter_LateUpdate();
void TeeterTotter_Draw();
void TeeterTotter_Setup(void* subtype);
void TeeterTotter_StageLoad();
void TeeterTotter_GetAttributes();

#endif //!OBJ_TEETERTOTTER_H
