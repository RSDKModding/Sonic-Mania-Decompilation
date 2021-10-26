#ifndef OBJ_EGGPISTONSMKII_H
#define OBJ_EGGPISTONSMKII_H

#include "SonicMania.h"

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
void EggPistonsMKII_Update(void);
void EggPistonsMKII_LateUpdate(void);
void EggPistonsMKII_StaticUpdate(void);
void EggPistonsMKII_Draw(void);
void EggPistonsMKII_Create(void* data);
void EggPistonsMKII_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void EggPistonsMKII_EditorDraw(void);
void EggPistonsMKII_EditorLoad(void);
#endif
void EggPistonsMKII_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_EGGPISTONSMKII_H
