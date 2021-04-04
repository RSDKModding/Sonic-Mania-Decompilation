#ifndef OBJ_WATERFALLSOUND_H
#define OBJ_WATERFALLSOUND_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectWaterfallSound;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityWaterfallSound;

// Object Struct
extern ObjectWaterfallSound *WaterfallSound;

// Standard Entity Events
void WaterfallSound_Update(void);
void WaterfallSound_LateUpdate(void);
void WaterfallSound_StaticUpdate(void);
void WaterfallSound_Draw(void);
void WaterfallSound_Create(void* data);
void WaterfallSound_StageLoad(void);
void WaterfallSound_EditorDraw(void);
void WaterfallSound_EditorLoad(void);
void WaterfallSound_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_WATERFALLSOUND_H
