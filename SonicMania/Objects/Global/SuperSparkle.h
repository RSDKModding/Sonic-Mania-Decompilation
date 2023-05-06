#ifndef OBJ_SUPERSPARKLE_H
#define OBJ_SUPERSPARKLE_H

#include "Game.h"

// Object Class
struct ObjectSuperSparkle {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntitySuperSparkle {
    RSDK_ENTITY
    EntityPlayer *player;
    int32 timer;
    bool32 canSpawnSparkle;
};

// Object Struct
extern ObjectSuperSparkle *SuperSparkle;

// Standard Entity Events
void SuperSparkle_Update(void);
void SuperSparkle_LateUpdate(void);
void SuperSparkle_StaticUpdate(void);
void SuperSparkle_Draw(void);
void SuperSparkle_Create(void *data);
void SuperSparkle_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SuperSparkle_EditorDraw(void);
void SuperSparkle_EditorLoad(void);
#endif
void SuperSparkle_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_SUPERSPARKLE_H
