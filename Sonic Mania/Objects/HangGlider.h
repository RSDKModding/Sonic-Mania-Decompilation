#ifndef OBJ_HANGGLIDER_H
#define OBJ_HANGGLIDER_H

#include "../SonicMania.h"

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
void HangGlider_Update();
void HangGlider_LateUpdate();
void HangGlider_StaticUpdate();
void HangGlider_Draw();
void HangGlider_Create(void* data);
void HangGlider_StageLoad();
void HangGlider_EditorDraw();
void HangGlider_EditorLoad();
void HangGlider_Serialize();

// Extra Entity Functions


#endif //!OBJ_HANGGLIDER_H
