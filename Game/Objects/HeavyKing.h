#ifndef OBJ_HEAVYKING_H
#define OBJ_HEAVYKING_H

#include "../../SonicMania.h"

//Object Class
class ObjectHeavyKing : public Object {
public:

};

//Entity Class
class EntityHeavyKing : public Entity {
public:

};

//Object Entity
ObjectHeavyKing HeavyKing;

//Entity Functions
void HeavyKing_Update();
void HeavyKing_EarlyUpdate();
void HeavyKing_LateUpdate();
void HeavyKing_Draw();
void HeavyKing_Setup(void* subtype);
void HeavyKing_StageLoad();
void HeavyKing_GetAttributes();

#endif //!OBJ_HEAVYKING_H
