#ifndef OBJ_EGGLOCO_H
#define OBJ_EGGLOCO_H

#include "../SonicMania.h"

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
void EggLoco_Update();
void EggLoco_LateUpdate();
void EggLoco_StaticUpdate();
void EggLoco_Draw();
void EggLoco_Create(void* data);
void EggLoco_StageLoad();
void EggLoco_EditorDraw();
void EggLoco_EditorLoad();
void EggLoco_Serialize();

// Extra Entity Functions


#endif //!OBJ_EGGLOCO_H
