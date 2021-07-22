#ifndef OBJ_PBL_FLIPPER_H
#define OBJ_PBL_FLIPPER_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPBL_Flipper;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPBL_Flipper;

// Object Struct
extern ObjectPBL_Flipper *PBL_Flipper;

// Standard Entity Events
void PBL_Flipper_Update(void);
void PBL_Flipper_LateUpdate(void);
void PBL_Flipper_StaticUpdate(void);
void PBL_Flipper_Draw(void);
void PBL_Flipper_Create(void* data);
void PBL_Flipper_StageLoad(void);
void PBL_Flipper_EditorDraw(void);
void PBL_Flipper_EditorLoad(void);
void PBL_Flipper_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_PBL_FLIPPER_H
