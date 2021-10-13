#ifndef OBJ_WATERFALLSOUND_H
#define OBJ_WATERFALLSOUND_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int32 activeCount;
    int32 field_8;
    int32 field_C;
    int32 field_10;
    int32 field_14;
} ObjectWaterfallSound;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    Vector2 posUnk;
    bool32 flag;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    int32 field_7C;
    int32 field_80;
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
bool32 WaterfallSound_CheckCB(void);
void WaterfallSound_UpdateCB(int32 sfxID);
#endif

#endif //!OBJ_WATERFALLSOUND_H
