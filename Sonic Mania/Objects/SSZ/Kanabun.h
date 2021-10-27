#ifndef OBJ_KANABUN_H
#define OBJ_KANABUN_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectKanabun;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityKanabun;

// Object Struct
extern ObjectKanabun *Kanabun;

// Standard Entity Events
void Kanabun_Update(void);
void Kanabun_LateUpdate(void);
void Kanabun_StaticUpdate(void);
void Kanabun_Draw(void);
void Kanabun_Create(void* data);
void Kanabun_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Kanabun_EditorDraw(void);
void Kanabun_EditorLoad(void);
#endif
void Kanabun_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_KANABUN_H
