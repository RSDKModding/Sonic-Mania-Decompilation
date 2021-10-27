#ifndef OBJ_CRATE_H
#define OBJ_CRATE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxExplosion2;
} ObjectCrate;

// Entity Class
typedef struct {
    MANIA_PLATFORM_BASE
    bool32 ignoreItemBox;
} EntityCrate;

// Object Struct
extern ObjectCrate *Crate;

// Standard Entity Events
void Crate_Update(void);
void Crate_LateUpdate(void);
void Crate_StaticUpdate(void);
void Crate_Draw(void);
void Crate_Create(void* data);
void Crate_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Crate_EditorDraw(void);
void Crate_EditorLoad(void);
#endif
void Crate_Serialize(void);

// Extra Entity Functions
void Crate_Break(EntityCrate *entity);
void Crate_MoveY(EntityCrate *entity, int32 offset);
bool32 Crate_Collide(void);
void Crate_Null1(void);
void Crate_ApplyGravity(void);
void Crate_WaitToFall(void);
void Crate_Fall(void);

#endif //!OBJ_CRATE_H
