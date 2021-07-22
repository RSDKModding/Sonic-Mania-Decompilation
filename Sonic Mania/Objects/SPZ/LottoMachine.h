#ifndef OBJ_LOTTOMACHINE_H
#define OBJ_LOTTOMACHINE_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLottoMachine;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLottoMachine;

// Object Struct
extern ObjectLottoMachine *LottoMachine;

// Standard Entity Events
void LottoMachine_Update(void);
void LottoMachine_LateUpdate(void);
void LottoMachine_StaticUpdate(void);
void LottoMachine_Draw(void);
void LottoMachine_Create(void* data);
void LottoMachine_StageLoad(void);
void LottoMachine_EditorDraw(void);
void LottoMachine_EditorLoad(void);
void LottoMachine_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LOTTOMACHINE_H
