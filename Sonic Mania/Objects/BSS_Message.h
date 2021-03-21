#ifndef OBJ_BSS_MESSAGE_H
#define OBJ_BSS_MESSAGE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectBSS_Message;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void(*state)();
    int timer;
    int timer2;
    bool32 flag;
    int colour;
    int field_6C;
    AnimationData leftData;
    AnimationData rightData;
} EntityBSS_Message;

// Object Struct
extern ObjectBSS_Message *BSS_Message;

// Standard Entity Events
void BSS_Message_Update();
void BSS_Message_LateUpdate();
void BSS_Message_StaticUpdate();
void BSS_Message_Draw();
void BSS_Message_Create(void* data);
void BSS_Message_StageLoad();
void BSS_Message_EditorDraw();
void BSS_Message_EditorLoad();
void BSS_Message_Serialize();

// Extra Entity Functions
void BSS_Message_State_GetBS();
void BSS_Message_State_GetBSWait();
void BSS_Message_State_Unknown();
void BSS_Message_State_Perfect();
void BSS_Message_State_Idle();
void BSS_Message_State_Finish();
int BSS_Message_TrackProgress_CB(int a1);
void BSS_Message_LoadPrevScene();
void BSS_Message_LoadGameState();

#endif //!OBJ_BSS_MESSAGE_H
