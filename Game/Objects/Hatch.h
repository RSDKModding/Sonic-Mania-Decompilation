#ifndef OBJ_HATCH_H
#define OBJ_HATCH_H

//Object Class
class Hatch : Object {

};

//Entity Class
class EntityHatch : Entity {

};

//Entity Functions
void Hatch_Update();
void Hatch_EarlyUpdate();
void Hatch_LateUpdate();
void Hatch_Draw();
void Hatch_Setup(void* subtype);
void Hatch_StageLoad();
void Hatch_GetAttributes();

#endif //!OBJ_HATCH_H
