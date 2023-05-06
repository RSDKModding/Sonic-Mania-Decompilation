#ifndef OBJ_CONSTELLATION_H
#define OBJ_CONSTELLATION_H

#include "Game.h"

typedef enum {
    CONSTELLATION_SONIC,
    CONSTELLATION_VASE,
    CONSTELLATION_BERRIES,
    CONSTELLATION_LYRE,
    CONSTELLATION_HELMET,
} ConstellationShapes;

// Object Class
struct ObjectConstellation {
    RSDK_OBJECT
    uint16 aniFrames;
    TileLayer *background1;
};

// Entity Class
struct EntityConstellation {
    RSDK_ENTITY
    uint8 shape;
    Vector2 originPos;
    int32 unused;
    ScrollInfo *scrollInfo;
    Animator animator;
};

// Object Struct
extern ObjectConstellation *Constellation;

// Standard Entity Events
void Constellation_Update(void);
void Constellation_LateUpdate(void);
void Constellation_StaticUpdate(void);
void Constellation_Draw(void);
void Constellation_Create(void *data);
void Constellation_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Constellation_EditorDraw(void);
void Constellation_EditorLoad(void);
#endif
void Constellation_Serialize(void);

// Extra Entity Functions
void Constellation_SetupInfo(void);

#endif //! OBJ_CONSTELLATION_H
