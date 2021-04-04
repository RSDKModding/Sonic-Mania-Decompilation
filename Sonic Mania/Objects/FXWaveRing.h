#ifndef OBJ_FXWAVERING_H
#define OBJ_FXWAVERING_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFXWaveRing;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFXWaveRing;

// Object Struct
extern ObjectFXWaveRing *FXWaveRing;

// Standard Entity Events
void FXWaveRing_Update(void);
void FXWaveRing_LateUpdate(void);
void FXWaveRing_StaticUpdate(void);
void FXWaveRing_Draw(void);
void FXWaveRing_Create(void* data);
void FXWaveRing_StageLoad(void);
void FXWaveRing_EditorDraw(void);
void FXWaveRing_EditorLoad(void);
void FXWaveRing_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_FXWAVERING_H
