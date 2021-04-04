#ifndef OBJ_TIMEATTACKGATE_H
#define OBJ_TIMEATTACKGATE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTimeAttackGate;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTimeAttackGate;

// Object Struct
extern ObjectTimeAttackGate *TimeAttackGate;

// Standard Entity Events
void TimeAttackGate_Update(void);
void TimeAttackGate_LateUpdate(void);
void TimeAttackGate_StaticUpdate(void);
void TimeAttackGate_Draw(void);
void TimeAttackGate_Create(void* data);
void TimeAttackGate_StageLoad(void);
void TimeAttackGate_EditorDraw(void);
void TimeAttackGate_EditorLoad(void);
void TimeAttackGate_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TIMEATTACKGATE_H
