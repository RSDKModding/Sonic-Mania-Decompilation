#ifndef OBJ_TRYAGAINE_H
#define OBJ_TRYAGAINE_H

//Object Class
class TryAgainE : Object {

};

//Entity Class
class EntityTryAgainE : Entity {

};

//Entity Functions
void TryAgainE_Update();
void TryAgainE_EarlyUpdate();
void TryAgainE_LateUpdate();
void TryAgainE_Draw();
void TryAgainE_Setup(void* subtype);
void TryAgainE_StageLoad();
void TryAgainE_GetAttributes();

#endif //!OBJ_TRYAGAINE_H
