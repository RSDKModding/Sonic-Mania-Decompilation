#ifndef OBJ_SSZ3CUTSCENE_H
#define OBJ_SSZ3CUTSCENE_H

#include "../../SonicMania.h"

//Object Class
class ObjectSSZ3Cutscene : public Object {
public:

};

//Entity Class
class EntitySSZ3Cutscene : public Entity {
public:

};

//Object Entity
ObjectSSZ3Cutscene SSZ3Cutscene;

//Entity Functions
void SSZ3Cutscene_Update();
void SSZ3Cutscene_EarlyUpdate();
void SSZ3Cutscene_LateUpdate();
void SSZ3Cutscene_Draw();
void SSZ3Cutscene_Setup(void* subtype);
void SSZ3Cutscene_StageLoad();
void SSZ3Cutscene_GetAttributes();

#endif //!OBJ_SSZ3CUTSCENE_H
