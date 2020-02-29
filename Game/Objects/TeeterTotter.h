#ifndef OBJ_TEETERTOTTER_H
#define OBJ_TEETERTOTTER_H

#include "../../SonicMania.h"

//Object Class
class ObjectTeeterTotter : public Object {
public:

};

//Entity Class
class EntityTeeterTotter : public Entity {
public:

};

//Object Entity
ObjectTeeterTotter TeeterTotter;

//Entity Functions
void TeeterTotter_Update();
void TeeterTotter_EarlyUpdate();
void TeeterTotter_LateUpdate();
void TeeterTotter_Draw();
void TeeterTotter_Setup(void* subtype);
void TeeterTotter_StageLoad();
void TeeterTotter_GetAttributes();

#endif //!OBJ_TEETERTOTTER_H
