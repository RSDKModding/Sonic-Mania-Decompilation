#ifndef OBJ_FXRUBY_H
#define OBJ_FXRUBY_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFXRuby : Object {
    int deadzone[0x200];
    ushort lookupTable[0x10000];
    TileLayer *fgLow;
    TileLayer *fgHigh;
};

// Entity Class
struct EntityFXRuby : Entity {
    void (*state)();
    int dword5C;
    int dword60;
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
};

// Object Struct
extern ObjectFXRuby *FXRuby;

// Standard Entity Events
void FXRuby_Update();
void FXRuby_LateUpdate();
void FXRuby_StaticUpdate();
void FXRuby_Draw();
void FXRuby_Create(void* data);
void FXRuby_StageLoad();
void FXRuby_EditorDraw();
void FXRuby_EditorLoad();
void FXRuby_Serialize();

// Extra Entity Functions
void FXRuby_Unknown1();
void FXRuby_Unknown2();

void FXRuby_Unknown3();
void FXRuby_Unknown4();
void FXRuby_Unknown5();
void FXRuby_Unknown6();
void FXRuby_Unknown7();
void FXRuby_Unknown9();

#endif //!OBJ_FXRUBY_H
