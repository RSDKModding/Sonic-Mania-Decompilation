#ifndef OBJ_SPINSIGN_H
#define OBJ_SPINSIGN_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSpinSign;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySpinSign;

// Object Struct
extern ObjectSpinSign *SpinSign;

// Standard Entity Events
void SpinSign_Update();
void SpinSign_LateUpdate();
void SpinSign_StaticUpdate();
void SpinSign_Draw();
void SpinSign_Create(void* data);
void SpinSign_StageLoad();
void SpinSign_EditorDraw();
void SpinSign_EditorLoad();
void SpinSign_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPINSIGN_H
