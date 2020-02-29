#ifndef OBJ_TRYAGAIN_H
#define OBJ_TRYAGAIN_H

//Object Class
class TryAgain : Object {

};

//Entity Class
class EntityTryAgain : Entity {

};

//Entity Functions
void TryAgain_Update();
void TryAgain_EarlyUpdate();
void TryAgain_LateUpdate();
void TryAgain_Draw();
void TryAgain_Setup(void* subtype);
void TryAgain_StageLoad();
void TryAgain_GetAttributes();

#endif //!OBJ_TRYAGAIN_H
