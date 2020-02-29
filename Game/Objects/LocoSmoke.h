#ifndef OBJ_LOCOSMOKE_H
#define OBJ_LOCOSMOKE_H

//Object Class
class LocoSmoke : Object {

};

//Entity Class
class EntityLocoSmoke : Entity {

};

//Entity Functions
void LocoSmoke_Update();
void LocoSmoke_EarlyUpdate();
void LocoSmoke_LateUpdate();
void LocoSmoke_Draw();
void LocoSmoke_Setup(void* subtype);
void LocoSmoke_StageLoad();
void LocoSmoke_GetAttributes();

#endif //!OBJ_LOCOSMOKE_H
