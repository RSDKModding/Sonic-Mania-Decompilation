#ifndef OBJ_PBL_FLIPPER_H
#define OBJ_PBL_FLIPPER_H

#include "../SonicMania.h"

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
void PBL_Flipper_Update();
void PBL_Flipper_LateUpdate();
void PBL_Flipper_StaticUpdate();
void PBL_Flipper_Draw();
void PBL_Flipper_Create(void* data);
void PBL_Flipper_StageLoad();
void PBL_Flipper_EditorDraw();
void PBL_Flipper_EditorLoad();
void PBL_Flipper_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_PBL_FLIPPER_H
