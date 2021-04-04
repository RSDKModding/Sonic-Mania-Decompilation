#ifndef OBJ_LAVAFALL_H
#define OBJ_LAVAFALL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLavaFall;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLavaFall;

// Object Struct
extern ObjectLavaFall *LavaFall;

// Standard Entity Events
void LavaFall_Update(void);
void LavaFall_LateUpdate(void);
void LavaFall_StaticUpdate(void);
void LavaFall_Draw(void);
void LavaFall_Create(void* data);
void LavaFall_StageLoad(void);
void LavaFall_EditorDraw(void);
void LavaFall_EditorLoad(void);
void LavaFall_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LAVAFALL_H
