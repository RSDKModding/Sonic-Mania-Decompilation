#ifndef OBJ_CONSTELLATION_H
#define OBJ_CONSTELLATION_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    TileLayer *bg1;
} ObjectConstellation;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 shape;
    Vector2 startPos;
    int32 field_64;
    ScrollInfo *scrollInfo;
    Animator animator;
} EntityConstellation;

// Object Struct
extern ObjectConstellation *Constellation;

// Standard Entity Events
void Constellation_Update(void);
void Constellation_LateUpdate(void);
void Constellation_StaticUpdate(void);
void Constellation_Draw(void);
void Constellation_Create(void* data);
void Constellation_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Constellation_EditorDraw(void);
void Constellation_EditorLoad(void);
#endif
void Constellation_Serialize(void);

// Extra Entity Functions
void Constellation_SetupInfo(void);

#endif //!OBJ_CONSTELLATION_H
