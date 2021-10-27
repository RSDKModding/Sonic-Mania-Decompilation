#ifndef OBJ_HOTARU_H
#define OBJ_HOTARU_H

#include "SonicMania.h"

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
void Hotaru_Update(void);
void Hotaru_LateUpdate(void);
void Hotaru_StaticUpdate(void);
void Hotaru_Draw(void);
void Hotaru_Create(void* data);
void Hotaru_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Hotaru_EditorDraw(void);
void Hotaru_EditorLoad(void);
#endif
void Hotaru_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HOTARU_H
