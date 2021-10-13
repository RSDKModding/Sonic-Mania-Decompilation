#ifndef OBJ_BOUNDSMARKER_H
#define OBJ_BOUNDSMARKER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
} ObjectBoundsMarker;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 type;
    int32 width;
    bool32 vsDisable;
    int32 offset;
} EntityBoundsMarker;

// Object Struct
extern ObjectBoundsMarker *BoundsMarker;

// Standard Entity Events
void BoundsMarker_Update(void);
void BoundsMarker_LateUpdate(void);
void BoundsMarker_StaticUpdate(void);
void BoundsMarker_Draw(void);
void BoundsMarker_Create(void* data);
void BoundsMarker_StageLoad(void);
void BoundsMarker_EditorDraw(void);
void BoundsMarker_EditorLoad(void);
void BoundsMarker_Serialize(void);

// Extra Entity Functions
void BoundsMarker_CheckBounds(void *player, EntityBoundsMarker *entity, bool32 setPos);
void BoundsMarker_CheckAllBounds(void *player, bool32 setPos);

#endif //!OBJ_BOUNDSMARKER_H
