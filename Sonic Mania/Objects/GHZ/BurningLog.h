#ifndef OBJ_BURNINGLOG_H
#define OBJ_BURNINGLOG_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 spriteIndex;
    Hitbox hitbox;
    Hitbox hitbox2;
} ObjectBurningLog;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Animator animator;
    int32 timer;
} EntityBurningLog;

// Object Struct
extern ObjectBurningLog *BurningLog;

// Standard Entity Events
void BurningLog_Update(void);
void BurningLog_LateUpdate(void);
void BurningLog_StaticUpdate(void);
void BurningLog_Draw(void);
void BurningLog_Create(void* data);
void BurningLog_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void BurningLog_EditorDraw(void);
void BurningLog_EditorLoad(void);
#endif
void BurningLog_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BURNINGLOG_H
