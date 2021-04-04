#ifndef OBJ_HILOSIGN_H
#define OBJ_HILOSIGN_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectHiLoSign;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHiLoSign;

// Object Struct
extern ObjectHiLoSign *HiLoSign;

// Standard Entity Events
void HiLoSign_Update(void);
void HiLoSign_LateUpdate(void);
void HiLoSign_StaticUpdate(void);
void HiLoSign_Draw(void);
void HiLoSign_Create(void* data);
void HiLoSign_StageLoad(void);
void HiLoSign_EditorDraw(void);
void HiLoSign_EditorLoad(void);
void HiLoSign_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HILOSIGN_H
