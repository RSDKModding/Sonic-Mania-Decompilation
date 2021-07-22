#ifndef OBJ_EGGLOCO_H
#define OBJ_EGGLOCO_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectEggLoco;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityEggLoco;

// Object Struct
extern ObjectEggLoco *EggLoco;

// Standard Entity Events
void EggLoco_Update(void);
void EggLoco_LateUpdate(void);
void EggLoco_StaticUpdate(void);
void EggLoco_Draw(void);
void EggLoco_Create(void* data);
void EggLoco_StageLoad(void);
void EggLoco_EditorDraw(void);
void EggLoco_EditorLoad(void);
void EggLoco_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_EGGLOCO_H
