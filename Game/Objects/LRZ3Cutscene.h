#ifndef OBJ_LRZ3CUTSCENE_H
#define OBJ_LRZ3CUTSCENE_H

#include "../../SonicMania.h"

//Object Class
class ObjectLRZ3Cutscene : public Object {
public:

};

//Entity Class
class EntityLRZ3Cutscene : public Entity {
public:

};

//Object Entity
ObjectLRZ3Cutscene LRZ3Cutscene;

//Entity Functions
void LRZ3Cutscene_Update();
void LRZ3Cutscene_EarlyUpdate();
void LRZ3Cutscene_LateUpdate();
void LRZ3Cutscene_Draw();
void LRZ3Cutscene_Setup(void* subtype);
void LRZ3Cutscene_StageLoad();
void LRZ3Cutscene_GetAttributes();

#endif //!OBJ_LRZ3CUTSCENE_H
