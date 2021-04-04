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
void SpinSign_Update(void);
void SpinSign_LateUpdate(void);
void SpinSign_StaticUpdate(void);
void SpinSign_Draw(void);
void SpinSign_Create(void* data);
void SpinSign_StageLoad(void);
void SpinSign_EditorDraw(void);
void SpinSign_EditorLoad(void);
void SpinSign_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SPINSIGN_H
