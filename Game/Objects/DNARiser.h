#ifndef OBJ_DNARISER_H
#define OBJ_DNARISER_H

//Object Class
class DNARiser : Object {

};

//Entity Class
class EntityDNARiser : Entity {

};

//Entity Functions
void DNARiser_Update();
void DNARiser_EarlyUpdate();
void DNARiser_LateUpdate();
void DNARiser_Draw();
void DNARiser_Setup(void* subtype);
void DNARiser_StageLoad();
void DNARiser_GetAttributes();

#endif //!OBJ_DNARISER_H
