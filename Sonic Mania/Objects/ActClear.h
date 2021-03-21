#ifndef OBJ_ACTCLEAR_H
#define OBJ_ACTCLEAR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    ushort sfx_ScoreAdd;
    ushort sfx_ScoreTotal;
    ushort sfx_Event;
    bool32 field_C;
    int field_10;
    int field_14;
    int actID;
    int field_1C;
    int field_20;
    void (*bufferMove_CB)();
    void (*saveReplay_CB)();
    int field_2C;
    int field_30;
    bool32 dword34;
} ObjectActClear;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityActClear;

// Object Struct
extern ObjectActClear *ActClear;

// Standard Entity Events
void ActClear_Update();
void ActClear_LateUpdate();
void ActClear_StaticUpdate();
void ActClear_Draw();
void ActClear_Create(void* data);
void ActClear_StageLoad();
void ActClear_EditorDraw();
void ActClear_EditorLoad();
void ActClear_Serialize();

// Extra Entity Functions


#endif //!OBJ_ACTCLEAR_H
