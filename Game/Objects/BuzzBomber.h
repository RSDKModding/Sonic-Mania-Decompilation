#ifndef OBJ_BUZZBOMBER_H
#define OBJ_BUZZBOMBER_H

#include "../../SonicMania.h"

//Object Class
class ObjectBuzzBomber : public Object {
public:

};

//Entity Class
class EntityBuzzBomber : public Entity {
public:

};

//Object Entity
ObjectBuzzBomber BuzzBomber;

//Entity Functions
void BuzzBomber_Update();
void BuzzBomber_EarlyUpdate();
void BuzzBomber_LateUpdate();
void BuzzBomber_Draw();
void BuzzBomber_Setup(void* subtype);
void BuzzBomber_StageLoad();
void BuzzBomber_GetAttributes();

#endif //!OBJ_BUZZBOMBER_H
