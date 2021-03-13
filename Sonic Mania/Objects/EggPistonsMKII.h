#ifndef OBJ_EGGPISTONSMKII_H
#define OBJ_EGGPISTONSMKII_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectEggPistonsMKII;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityEggPistonsMKII;

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
