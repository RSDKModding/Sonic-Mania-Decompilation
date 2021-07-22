#ifndef OBJ_HANGCONVEYOR_H
#define OBJ_HANGCONVEYOR_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectHangConveyor;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHangConveyor;

// Object Struct
extern ObjectHangConveyor *HangConveyor;

// Standard Entity Events
void HangConveyor_Update(void);
void HangConveyor_LateUpdate(void);
void HangConveyor_StaticUpdate(void);
void HangConveyor_Draw(void);
void HangConveyor_Create(void* data);
void HangConveyor_StageLoad(void);
void HangConveyor_EditorDraw(void);
void HangConveyor_EditorLoad(void);
void HangConveyor_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HANGCONVEYOR_H
