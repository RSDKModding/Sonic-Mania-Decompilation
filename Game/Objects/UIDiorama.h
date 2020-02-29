#ifndef OBJ_UIDIORAMA_H
#define OBJ_UIDIORAMA_H

//Object Class
class UIDiorama : Object {

};

//Entity Class
class EntityUIDiorama : Entity {

};

//Entity Functions
void UIDiorama_Update();
void UIDiorama_EarlyUpdate();
void UIDiorama_LateUpdate();
void UIDiorama_Draw();
void UIDiorama_Setup(void* subtype);
void UIDiorama_StageLoad();
void UIDiorama_GetAttributes();

#endif //!OBJ_UIDIORAMA_H
