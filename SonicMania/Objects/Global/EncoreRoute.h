#ifndef OBJ_ENCOREROUTE_H
#define OBJ_ENCOREROUTE_H

#include "Game.h"

#if MANIA_USE_PLUS

typedef enum { EROUTE_LAYER_FGLOW, EROUTE_LAYER_FGHIGH, EROUTE_LAYER_MOVE, EROUTE_LAYER_SCRATCH } EncoreRouteLayerIDs;

typedef enum { EROUTE_FRAME_UNUSED } EncoreRouteFrameIDs;

// Object Class
struct ObjectEncoreRoute {
    RSDK_OBJECT
    uint16 aniFrames;
    Animator animator;
};

// Entity Class
struct EntityEncoreRoute {
    RSDK_ENTITY
    Vector2 offset;
    Vector2 size;
    uint8 layerSrc;
    uint8 layerDest;
    EncoreRouteFrameIDs frameID;
};

// Object Struct
extern ObjectEncoreRoute *EncoreRoute;

// Standard Entity Events
void EncoreRoute_Update(void);
void EncoreRoute_LateUpdate(void);
void EncoreRoute_StaticUpdate(void);
void EncoreRoute_Draw(void);
void EncoreRoute_Create(void *data);
void EncoreRoute_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void EncoreRoute_EditorDraw(void);
void EncoreRoute_EditorLoad(void);
#endif
void EncoreRoute_Serialize(void);

// Extra Entity Functions

#endif

#endif //! OBJ_ENCOREROUTE_H
