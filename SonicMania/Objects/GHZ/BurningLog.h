#ifndef OBJ_BURNINGLOG_H
#define OBJ_BURNINGLOG_H

#include "Game.h"

// Object Class
struct ObjectBurningLog {
    RSDK_OBJECT
    int32 aniFrames;
    Hitbox hitboxPlatform;
    Hitbox hitboxFlame;
};

// Entity Class
struct EntityBurningLog {
    RSDK_ENTITY
    Animator animator;
    int32 timer;
};

// Object Struct
extern ObjectBurningLog *BurningLog;

// Standard Entity Events
void BurningLog_Update(void);
void BurningLog_LateUpdate(void);
void BurningLog_StaticUpdate(void);
void BurningLog_Draw(void);
void BurningLog_Create(void *data);
void BurningLog_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BurningLog_EditorDraw(void);
void BurningLog_EditorLoad(void);
#endif
void BurningLog_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_BURNINGLOG_H
