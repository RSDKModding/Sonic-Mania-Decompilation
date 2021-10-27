#ifndef OBJ_FXRUBY_H
#define OBJ_FXRUBY_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 deformData[0x200];
    uint16 lookupTable[0x10000];
    TileLayer *fgLow;
    TileLayer *fgHigh;
} ObjectFXRuby;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 dword5C;
    int32 radiusSpeed;
    int32 dword64;
    int32 innerRadius;
    int32 outerRadius;
    int32 field_70;
    int32 field_74;
    int32 fadeWhite;
    int32 fadeBlack;
    bool32 waitForTrigger;
    int32 field_84;
    int32 field_88;
    int32 field_8C;
    bool32 flag;
} EntityFXRuby;

// Object Struct
extern ObjectFXRuby *FXRuby;

// Standard Entity Events
void FXRuby_Update(void);
void FXRuby_LateUpdate(void);
void FXRuby_StaticUpdate(void);
void FXRuby_Draw(void);
void FXRuby_Create(void* data);
void FXRuby_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void FXRuby_EditorDraw(void);
void FXRuby_EditorLoad(void);
#endif
void FXRuby_Serialize(void);

// Extra Entity Functions
void FXRuby_SetupLayerDeformation(void);
void FXRuby_Unknown2(void);

void FXRuby_Unknown3(void);
void FXRuby_Unknown4(void);
void FXRuby_Unknown5(void);
void FXRuby_Unknown6(void);
void FXRuby_Unknown7(void);
void FXRuby_Unknown9(void);

#endif //!OBJ_FXRUBY_H
