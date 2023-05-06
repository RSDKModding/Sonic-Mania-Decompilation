#ifndef OBJ_CRATE_H
#define OBJ_CRATE_H

#include "Game.h"

typedef enum {
    CRATE_FRAME_BLUE,
    CRATE_FRAME_ORANGE_BROKEN,
    CRATE_FRAME_ORANGE,
    CRATE_FRAME_BLUE_BG,
} CrateFrameIDs;

// Object Class
struct ObjectCrate {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxExplosion2;
};

// Entity Class
struct EntityCrate {
    MANIA_PLATFORM_BASE

    bool32 ignoreItemBox;
};

// Object Struct
extern ObjectCrate *Crate;

// Standard Entity Events
void Crate_Update(void);
void Crate_LateUpdate(void);
void Crate_StaticUpdate(void);
void Crate_Draw(void);
void Crate_Create(void *data);
void Crate_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Crate_EditorDraw(void);
void Crate_EditorLoad(void);
#endif
void Crate_Serialize(void);

// Extra Entity Functions
void Crate_Break(EntityCrate *entity);
void Crate_MoveY(EntityCrate *self, int32 offset);
bool32 Crate_Collide(void);

void Crate_State_None(void);
void Crate_State_ApplyGravity(void);
void Crate_State_WaitToFall(void);
void Crate_State_Fall(void);

#endif //! OBJ_CRATE_H
