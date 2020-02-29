#ifndef OBJ_EGGPISTONSMKII_H
#define OBJ_EGGPISTONSMKII_H

#include "../../SonicMania.h"

//Object Class
class ObjectEggPistonsMKII : public Object {
public:

};

//Entity Class
class EntityEggPistonsMKII : public Entity {
public:

};

//Object Entity
ObjectEggPistonsMKII EggPistonsMKII;

//Entity Functions
void EggPistonsMKII_Update();
void EggPistonsMKII_EarlyUpdate();
void EggPistonsMKII_LateUpdate();
void EggPistonsMKII_Draw();
void EggPistonsMKII_Setup(void* subtype);
void EggPistonsMKII_StageLoad();
void EggPistonsMKII_GetAttributes();

#endif //!OBJ_EGGPISTONSMKII_H
