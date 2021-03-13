#ifndef OBJ_EGGJANKEN_H
#define OBJ_EGGJANKEN_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectEggJanken;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityEggJanken;

// Object Entity
extern ObjectEggJanken *EggJanken;

// Standard Entity Events
void EggJanken_Update();
void EggJanken_LateUpdate();
void EggJanken_StaticUpdate();
void EggJanken_Draw();
void EggJanken_Create(void* data);
void EggJanken_StageLoad();
void EggJanken_EditorDraw();
void EggJanken_EditorLoad();
void EggJanken_Serialize();

// Extra Entity Functions


#endif //!OBJ_EGGJANKEN_H
