#ifndef OBJ_WATERFALLSOUND_H
#define OBJ_WATERFALLSOUND_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectWaterfallSound {
    RSDK_OBJECT
    int32 activeCount;
    int32 unused1;
    int32 unused2;
    int32 unused3;
    uint16 aniFrames;
};

// Entity Class
struct EntityWaterfallSound {
    RSDK_ENTITY
    Vector2 size;
    Vector2 sfxPos;
    bool32 sfxActive;
    Animator animator;
};

// Object Struct
extern ObjectWaterfallSound *WaterfallSound;

// Standard Entity Events
void WaterfallSound_Update(void);
void WaterfallSound_LateUpdate(void);
void WaterfallSound_StaticUpdate(void);
void WaterfallSound_Draw(void);
void WaterfallSound_Create(void *data);
void WaterfallSound_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void WaterfallSound_EditorDraw(void);
void WaterfallSound_EditorLoad(void);
#endif
void WaterfallSound_Serialize(void);

// Extra Entity Functions
bool32 WaterfallSound_SfxCheck_WaterfallLoop(void);
void WaterfallSound_SfxUpdate_WaterfallLoop(int32 sfxID);
#endif

#endif //! OBJ_WATERFALLSOUND_H
