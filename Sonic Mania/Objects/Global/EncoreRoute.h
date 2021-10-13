#ifndef OBJ_ENCOREROUTE_H
#define OBJ_ENCOREROUTE_H

#include "SonicMania.h"

#if RETRO_USE_PLUS

typedef enum {
    EROUTE_FRAME_UNKNOWN
}EncoreRouteFrameIDs;

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    int32 field_8;
    int32 field_C;
    int32 field_10;
    int32 field_14;
    int32 field_18;
    int32 field_1C;
} ObjectEncoreRoute;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 offset;
    Vector2 size;
    uint8 layerSrc;
    uint8 layerDest;
    EncoreRouteFrameIDs frameID;
} EntityEncoreRoute;

// Object Struct
extern ObjectEncoreRoute *EncoreRoute;

// Standard Entity Events
void EncoreRoute_Update(void);
void EncoreRoute_LateUpdate(void);
void EncoreRoute_StaticUpdate(void);
void EncoreRoute_Draw(void);
void EncoreRoute_Create(void* data);
void EncoreRoute_StageLoad(void);
void EncoreRoute_EditorDraw(void);
void EncoreRoute_EditorLoad(void);
void EncoreRoute_Serialize(void);

// Extra Entity Functions

#endif

#endif //!OBJ_ENCOREROUTE_H
