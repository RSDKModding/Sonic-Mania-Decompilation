#ifndef OBJ_UIDIORAMA_H
#define OBJ_UIDIORAMA_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIDiorama : public Object {
public:

};

//Entity Class
class EntityUIDiorama : public Entity {
public:

};

//Object Entity
ObjectUIDiorama UIDiorama;

//Entity Functions
void UIDiorama_Update();
void UIDiorama_EarlyUpdate();
void UIDiorama_LateUpdate();
void UIDiorama_Draw();
void UIDiorama_Setup(void* subtype);
void UIDiorama_StageLoad();
void UIDiorama_GetAttributes();

#endif //!OBJ_UIDIORAMA_H
