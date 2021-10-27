#ifndef OBJ_FIREWORK_H
#define OBJ_FIREWORK_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFirework;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFirework;

// Object Struct
extern ObjectFirework *Firework;

// Standard Entity Events
void Firework_Update(void);
void Firework_LateUpdate(void);
void Firework_StaticUpdate(void);
void Firework_Draw(void);
void Firework_Create(void* data);
void Firework_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Firework_EditorDraw(void);
void Firework_EditorLoad(void);
#endif
void Firework_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FIREWORK_H
