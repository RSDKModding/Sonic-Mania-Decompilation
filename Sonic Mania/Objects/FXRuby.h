#ifndef OBJ_FXRUBY_H
#define OBJ_FXRUBY_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int deformData[0x200];
    ushort lookupTable[0x10000];
    TileLayer *fgLow;
    TileLayer *fgHigh;
} ObjectFXRuby;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int dword5C;
    int radiusSpeed;
    int dword64;
    int field_68;
    int outerRadius;
    int fadeWhite;
    int fadeBlack;
    bool32 waitForTrigger;
    int timer;
    int dword80;
    int field_84;
    int field_88;
    int field_8C;
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
void FXRuby_EditorDraw(void);
void FXRuby_EditorLoad(void);
void FXRuby_Serialize(void);

// Extra Entity Functions
void FXRuby_Unknown1(void);
void FXRuby_Unknown2(void);

void FXRuby_Unknown3(void);
void FXRuby_Unknown4(void);
void FXRuby_Unknown5(void);
void FXRuby_Unknown6(void);
void FXRuby_Unknown7(void);
void FXRuby_Unknown9(void);

#endif //!OBJ_FXRUBY_H
