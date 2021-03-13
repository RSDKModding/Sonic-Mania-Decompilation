#ifndef OBJ_SUPERSPARKLE_H
#define OBJ_SUPERSPARKLE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectSuperSparkle;

// Entity Class
typedef struct {
    RSDK_ENTITY
    EntityPlayer *player;
    int timer;
    bool32 activeFlag;
} EntitySuperSparkle;

// Object Struct
extern ObjectSuperSparkle *SuperSparkle;

// Standard Entity Events
void SuperSparkle_Update();
void SuperSparkle_LateUpdate();
void SuperSparkle_StaticUpdate();
void SuperSparkle_Draw();
void SuperSparkle_Create(void* data);
void SuperSparkle_StageLoad();
void SuperSparkle_EditorDraw();
void SuperSparkle_EditorLoad();
void SuperSparkle_Serialize();

// Extra Entity Functions


#endif //!OBJ_SUPERSPARKLE_H
