#ifndef OBJ_HANGCONVEYOR_H
#define OBJ_HANGCONVEYOR_H

#include "../SonicMania.h"

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
void HangConveyor_Update();
void HangConveyor_LateUpdate();
void HangConveyor_StaticUpdate();
void HangConveyor_Draw();
void HangConveyor_Create(void* data);
void HangConveyor_StageLoad();
void HangConveyor_EditorDraw();
void HangConveyor_EditorLoad();
void HangConveyor_Serialize();

// Extra Entity Functions


#endif //!OBJ_HANGCONVEYOR_H
