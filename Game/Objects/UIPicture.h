#ifndef OBJ_UIPICTURE_H
#define OBJ_UIPICTURE_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIPicture : public Object {
public:

};

//Entity Class
class EntityUIPicture : public Entity {
public:

};

//Object Entity
ObjectUIPicture UIPicture;

//Entity Functions
void UIPicture_Update();
void UIPicture_EarlyUpdate();
void UIPicture_LateUpdate();
void UIPicture_Draw();
void UIPicture_Setup(void* subtype);
void UIPicture_StageLoad();
void UIPicture_GetAttributes();

#endif //!OBJ_UIPICTURE_H
