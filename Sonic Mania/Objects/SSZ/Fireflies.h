#ifndef OBJ_FIREFLIES_H
#define OBJ_FIREFLIES_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFireflies;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFireflies;

// Object Struct
extern ObjectFireflies *Fireflies;

// Standard Entity Events
void Fireflies_Update(void);
void Fireflies_LateUpdate(void);
void Fireflies_StaticUpdate(void);
void Fireflies_Draw(void);
void Fireflies_Create(void* data);
void Fireflies_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Fireflies_EditorDraw(void);
void Fireflies_EditorLoad(void);
#endif
void Fireflies_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FIREFLIES_H
