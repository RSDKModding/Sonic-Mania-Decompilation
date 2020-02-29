#ifndef OBJ_AMOEBADROID_H
#define OBJ_AMOEBADROID_H

#include "../../SonicMania.h"

//Object Class
class ObjectAmoebaDroid : public Object {
public:

};

//Entity Class
class EntityAmoebaDroid : public Entity {
public:

};

//Object Entity
ObjectAmoebaDroid AmoebaDroid;

//Entity Functions
void AmoebaDroid_Update();
void AmoebaDroid_EarlyUpdate();
void AmoebaDroid_LateUpdate();
void AmoebaDroid_Draw();
void AmoebaDroid_Setup(void* subtype);
void AmoebaDroid_StageLoad();
void AmoebaDroid_GetAttributes();

#endif //!OBJ_AMOEBADROID_H
