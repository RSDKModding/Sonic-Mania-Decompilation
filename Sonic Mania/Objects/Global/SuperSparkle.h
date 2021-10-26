#ifndef OBJ_SUPERSPARKLE_H
#define OBJ_SUPERSPARKLE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
} ObjectSuperSparkle;

// Entity Class
typedef struct {
    RSDK_ENTITY
    EntityPlayer *player;
    int32 timer;
    bool32 activeFlag;
} EntitySuperSparkle;

// Object Struct
extern ObjectSuperSparkle *SuperSparkle;

// Standard Entity Events
void SuperSparkle_Update(void);
void SuperSparkle_LateUpdate(void);
void SuperSparkle_StaticUpdate(void);
void SuperSparkle_Draw(void);
void SuperSparkle_Create(void* data);
void SuperSparkle_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SuperSparkle_EditorDraw(void);
void SuperSparkle_EditorLoad(void);
#endif
void SuperSparkle_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SUPERSPARKLE_H
