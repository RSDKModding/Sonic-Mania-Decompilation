#ifndef OBJ_EGGPRISON_H
#define OBJ_EGGPRISON_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectEggPrison;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityEggPrison;

// Object Struct
extern ObjectEggPrison *EggPrison;

// Standard Entity Events
void EggPrison_Update();
void EggPrison_LateUpdate();
void EggPrison_StaticUpdate();
void EggPrison_Draw();
void EggPrison_Create(void* data);
void EggPrison_StageLoad();
void EggPrison_EditorDraw();
void EggPrison_EditorLoad();
void EggPrison_Serialize();

// Extra Entity Functions


#endif //!OBJ_EGGPRISON_H
