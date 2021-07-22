#ifndef OBJ_HANGGLIDER_H
#define OBJ_HANGGLIDER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectHangGlider;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHangGlider;

// Object Struct
extern ObjectHangGlider *HangGlider;

// Standard Entity Events
void HangGlider_Update(void);
void HangGlider_LateUpdate(void);
void HangGlider_StaticUpdate(void);
void HangGlider_Draw(void);
void HangGlider_Create(void* data);
void HangGlider_StageLoad(void);
void HangGlider_EditorDraw(void);
void HangGlider_EditorLoad(void);
void HangGlider_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HANGGLIDER_H
