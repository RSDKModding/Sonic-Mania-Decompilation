#ifndef OBJ_EGGPISTONSMKII_H
#define OBJ_EGGPISTONSMKII_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectEggPistonsMKII : Object {

};

// Entity Class
struct EntityEggPistonsMKII : Entity {

};

// Object Struct
extern ObjectEggPistonsMKII *EggPistonsMKII;

// Standard Entity Events
void EggPistonsMKII_Update();
void EggPistonsMKII_LateUpdate();
void EggPistonsMKII_StaticUpdate();
void EggPistonsMKII_Draw();
void EggPistonsMKII_Create(void* data);
void EggPistonsMKII_StageLoad();
void EggPistonsMKII_EditorDraw();
void EggPistonsMKII_EditorLoad();
void EggPistonsMKII_Serialize();

// Extra Entity Functions


#endif //!OBJ_EGGPISTONSMKII_H
