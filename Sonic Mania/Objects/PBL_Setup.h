#ifndef OBJ_PBL_SETUP_H
#define OBJ_PBL_SETUP_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPBL_Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPBL_Setup;

// Object Struct
extern ObjectPBL_Setup *PBL_Setup;

// Standard Entity Events
void PBL_Setup_Update(void);
void PBL_Setup_LateUpdate(void);
void PBL_Setup_StaticUpdate(void);
void PBL_Setup_Draw(void);
void PBL_Setup_Create(void* data);
void PBL_Setup_StageLoad(void);
void PBL_Setup_EditorDraw(void);
void PBL_Setup_EditorLoad(void);
void PBL_Setup_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_PBL_SETUP_H
