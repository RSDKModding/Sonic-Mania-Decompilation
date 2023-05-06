#ifndef OBJ_SP500_H
#define OBJ_SP500_H

#include "Game.h"

// Object Class
struct ObjectSP500 {
    RSDK_OBJECT
    uint16 printLayerID;
    Hitbox hitboxSideL;
    Hitbox hitboxSideR;
    Hitbox hitboxBottom;
    uint16 aniFrames;
    uint16 sfxBeep4;
    uint16 sfxFail;
    uint16 sfxButton2;
};

// Entity Class
struct EntitySP500 {
    RSDK_ENTITY
    StateMachine(state);
    int32 len;
    int32 height;
    int32 offL;
    int32 offR;
    int32 startDir;
    int32 printDir;
    Vector2 srcC;
    Vector2 srcM;
    Vector2 srcY;
    int32 printRowID;
    int32 rowPrintPos;
    int32 timer;
    uint8 activePlayers;
    uint8 playerTimers[2];
    int32 unused;
    bool32 showGreenLight;
    bool32 showRedLight;
    int32 curPrintDir;
    int32 inkColor;
    Vector2 targetPos;
    Entity *storedEntity;
    Animator animator;
};

// Object Struct
extern ObjectSP500 *SP500;

// Standard Entity Events
void SP500_Update(void);
void SP500_LateUpdate(void);
void SP500_StaticUpdate(void);
void SP500_Draw(void);
void SP500_Create(void *data);
void SP500_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SP500_EditorDraw(void);
void SP500_EditorLoad(void);
#endif
void SP500_Serialize(void);

// Extra Entity Functions
void SP500_CheckPlayerCollisions(void);

void SP500_State_WaitForEntry(void);
void SP500_State_Finished(void);
void SP500_State_Activate(void);
void SP500_State_PrintFailed(void);
void SP500_State_PrintDelay(void);
void SP500_State_Printing(void);
void SP500_State_NextPrintRow(void);
void SP500_State_PrintFinished(void);
void SP500_State_MoveToTarget(void);

#endif //! OBJ_SP500_H
