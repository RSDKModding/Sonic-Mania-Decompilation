#ifndef OBJ_SPLATS_H
#define OBJ_SPLATS_H

//Object Class
class Splats : Object {

};

//Entity Class
class EntitySplats : Entity {

};

//Entity Functions
void Splats_Update();
void Splats_EarlyUpdate();
void Splats_LateUpdate();
void Splats_Draw();
void Splats_Setup(void* subtype);
void Splats_StageLoad();
void Splats_GetAttributes();

#endif //!OBJ_SPLATS_H
