#ifndef OBJ_LRZ3SETUP_H
#define OBJ_LRZ3SETUP_H

#include "Game.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 unused1;
    int32 unused2;
    int32 unused3;
    Entity *cutsceneOutroK;
} ObjectLRZ3Setup;

// Entity Class
typedef struct {
    RSDK_ENTITY
} EntityLRZ3Setup;

// Object Struct
extern ObjectLRZ3Setup *LRZ3Setup;

// Standard Entity Events
void LRZ3Setup_Update(void);
void LRZ3Setup_LateUpdate(void);
void LRZ3Setup_StaticUpdate(void);
void LRZ3Setup_Draw(void);
void LRZ3Setup_Create(void *data);
void LRZ3Setup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LRZ3Setup_EditorDraw(void);
void LRZ3Setup_EditorLoad(void);
#endif
void LRZ3Setup_Serialize(void);

// Extra Entity Functions
void LRZ3Setup_StageFinish_EndAct2K(void);

#endif //! OBJ_LRZ3SETUP_H
