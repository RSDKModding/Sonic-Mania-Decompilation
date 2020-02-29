#ifndef OBJ_UFO_ITEMBOX_H
#define OBJ_UFO_ITEMBOX_H

#include "../../SonicMania.h"

//Object Class
class ObjectUFO_ItemBox : public Object {
public:

};

//Entity Class
class EntityUFO_ItemBox : public Entity {
public:

};

//Object Entity
ObjectUFO_ItemBox UFO_ItemBox;

//Entity Functions
void UFO_ItemBox_Update();
void UFO_ItemBox_EarlyUpdate();
void UFO_ItemBox_LateUpdate();
void UFO_ItemBox_Draw();
void UFO_ItemBox_Setup(void* subtype);
void UFO_ItemBox_StageLoad();
void UFO_ItemBox_GetAttributes();

#endif //!OBJ_UFO_ITEMBOX_H
