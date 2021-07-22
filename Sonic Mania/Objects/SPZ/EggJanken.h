#ifndef OBJ_EGGJANKEN_H
#define OBJ_EGGJANKEN_H

#include "SonicMania.h"

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
void EggJanken_Update(void);
void EggJanken_LateUpdate(void);
void EggJanken_StaticUpdate(void);
void EggJanken_Draw(void);
void EggJanken_Create(void* data);
void EggJanken_StageLoad(void);
void EggJanken_EditorDraw(void);
void EggJanken_EditorLoad(void);
void EggJanken_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_EGGJANKEN_H
