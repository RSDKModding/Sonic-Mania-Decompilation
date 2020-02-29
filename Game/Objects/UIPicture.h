#ifndef OBJ_UIPICTURE_H
#define OBJ_UIPICTURE_H

//Object Class
class UIPicture : Object {

};

//Entity Class
class EntityUIPicture : Entity {

};

//Entity Functions
void UIPicture_Update();
void UIPicture_EarlyUpdate();
void UIPicture_LateUpdate();
void UIPicture_Draw();
void UIPicture_Setup(void* subtype);
void UIPicture_StageLoad();
void UIPicture_GetAttributes();

#endif //!OBJ_UIPICTURE_H
