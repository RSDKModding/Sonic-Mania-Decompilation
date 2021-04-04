#ifndef OBJ_CPZBOSS_H
#define OBJ_CPZBOSS_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCPZBoss;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCPZBoss;

// Object Struct
extern ObjectCPZBoss *CPZBoss;

// Standard Entity Events
void CPZBoss_Update(void);
void CPZBoss_LateUpdate(void);
void CPZBoss_StaticUpdate(void);
void CPZBoss_Draw(void);
void CPZBoss_Create(void* data);
void CPZBoss_StageLoad(void);
void CPZBoss_EditorDraw(void);
void CPZBoss_EditorLoad(void);
void CPZBoss_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CPZBOSS_H
