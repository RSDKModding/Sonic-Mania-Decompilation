#ifndef OBJ_OOZ2OUTRO_H
#define OBJ_OOZ2OUTRO_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    ushort sfxGlug;
    ushort sfxSubLaunch;
} ObjectOOZ2Outro;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 size;
    Vector2 moveOffset;
    Vector2 scrollOffset;
    int timer;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    TileLayer *moveLayer;
    EntityEggPrison *prisonPtr;
    Vector2 prisonPos;
} EntityOOZ2Outro;

// Object Struct
extern ObjectOOZ2Outro *OOZ2Outro;

// Standard Entity Events
void OOZ2Outro_Update(void);
void OOZ2Outro_LateUpdate(void);
void OOZ2Outro_StaticUpdate(void);
void OOZ2Outro_Draw(void);
void OOZ2Outro_Create(void* data);
void OOZ2Outro_StageLoad(void);
void OOZ2Outro_EditorDraw(void);
void OOZ2Outro_EditorLoad(void);
void OOZ2Outro_Serialize(void);

// Extra Entity Functions
void OOZ2Outro_StageFinishCB_Act2(void);
void OOZ2Outro_Unknown2(void);
void OOZ2Outro_CheckSkip(void);
void OOZ2Outro_Unknown3(void);
void OOZ2Outro_Unknown4(void);
void OOZ2Outro_Unknown5(void);

#endif

#endif //!OBJ_OOZ2OUTRO_H
