#ifndef OBJ_OOZ2OUTRO_H
#define OBJ_OOZ2OUTRO_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
struct ObjectOOZ2Outro {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxGlug;
    uint16 sfxSubLaunch;
};

// Entity Class
struct EntityOOZ2Outro {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 size;
    Vector2 moveOffset;
    Vector2 scrollOffset;
    int32 timer;
    int32 field_78;
    int32 field_7C;
    int32 field_80;
    int32 field_84;
    int32 field_88;
    int32 field_8C;
    TileLayer *moveLayer;
    EntityEggPrison *prisonPtr;
    Vector2 prisonPos;
};

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
