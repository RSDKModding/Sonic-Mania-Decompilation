#ifndef OBJ_HOTARU_H
#define OBJ_HOTARU_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectHotaru;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHotaru;

// Object Struct
extern ObjectHotaru *Hotaru;

// Standard Entity Events
void Hotaru_Update();
void Hotaru_LateUpdate();
void Hotaru_StaticUpdate();
void Hotaru_Draw();
void Hotaru_Create(void* data);
void Hotaru_StageLoad();
void Hotaru_EditorDraw();
void Hotaru_EditorLoad();
void Hotaru_Serialize();

// Extra Entity Functions


#endif //!OBJ_HOTARU_H
