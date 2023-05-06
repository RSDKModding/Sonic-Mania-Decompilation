#ifndef OBJ_SP500MKII_H
#define OBJ_SP500MKII_H

#include "Game.h"

// Object Class
struct ObjectSP500MkII {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxSolid;
    Hitbox hitboxButton;
    Hitbox hitboxPrinter;
};

// Entity Class
struct EntitySP500MkII {
    RSDK_ENTITY
    StateMachine(state);
    uint8 start;
    uint8 yDir;
    int32 printRows;
    int32 length;
    int32 printRowID;
    int32 xDir;
    bool32 showGreenLight;
    bool32 showRedLight;
    bool32 buttonDown;
    bool32 isActive;
    uint8 activePlayers;
    int32 buttonPos;
    Vector2 printerPosition;
    Vector2 startPos;
    Vector2 printPos;
    Vector2 printMoveOffset;
    int32 timer;
    Animator animator;
};

// Object Struct
extern ObjectSP500MkII *SP500MkII;

// Standard Entity Events
void SP500MkII_Update(void);
void SP500MkII_LateUpdate(void);
void SP500MkII_StaticUpdate(void);
void SP500MkII_Draw(void);
void SP500MkII_Create(void *data);
void SP500MkII_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SP500MkII_EditorDraw(void);
void SP500MkII_EditorLoad(void);
#endif
void SP500MkII_Serialize(void);

// Extra Entity Functions
void SP500MkII_DrawDebugOverlay(void);
void SP500MkII_DrawPrinter(void);
void SP500MkII_DrawRails(void);

void SP500MkII_CheckPlayerCollisions(void);

void SP500MkII_State_Init(void);
void SP500MkII_State_AwaitActivation(void);
void SP500MkII_State_PrintRow(void);
void SP500MkII_State_NextPrintRow(void);

#endif //! OBJ_SP500MKII_H
