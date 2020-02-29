#ifndef OBJ_EGGPISTONSMKII_H
#define OBJ_EGGPISTONSMKII_H

//Object Class
class EggPistonsMKII : Object {

};

//Entity Class
class EntityEggPistonsMKII : Entity {

};

//Entity Functions
void EggPistonsMKII_Update();
void EggPistonsMKII_EarlyUpdate();
void EggPistonsMKII_LateUpdate();
void EggPistonsMKII_Draw();
void EggPistonsMKII_Setup(void* subtype);
void EggPistonsMKII_StageLoad();
void EggPistonsMKII_GetAttributes();

#endif //!OBJ_EGGPISTONSMKII_H
