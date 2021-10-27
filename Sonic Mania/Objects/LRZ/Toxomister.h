#ifndef OBJ_TOXOMISTER_H
#define OBJ_TOXOMISTER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectToxomister;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityToxomister;

// Object Struct
extern ObjectToxomister *Toxomister;

// Standard Entity Events
void Toxomister_Update(void);
void Toxomister_LateUpdate(void);
void Toxomister_StaticUpdate(void);
void Toxomister_Draw(void);
void Toxomister_Create(void* data);
void Toxomister_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Toxomister_EditorDraw(void);
void Toxomister_EditorLoad(void);
#endif
void Toxomister_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TOXOMISTER_H
