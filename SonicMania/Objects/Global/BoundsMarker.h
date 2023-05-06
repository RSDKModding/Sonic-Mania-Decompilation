#ifndef OBJ_BOUNDSMARKER_H
#define OBJ_BOUNDSMARKER_H

#include "Game.h"

typedef enum {
    BOUNDSMARKER_ANY_Y,
    BOUNDSMARKER_ABOVE_Y,
    BOUNDSMARKER_BELOW_Y,
    BOUNDSMARKER_BELOW_Y_ANY,
} BoundsMarkerTypes;

// Object Class
struct ObjectBoundsMarker {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityBoundsMarker {
    RSDK_ENTITY
    uint8 type;
    int32 width;
    bool32 vsDisable;
    int32 offset;
};

// Object Struct
extern ObjectBoundsMarker *BoundsMarker;

// Standard Entity Events
void BoundsMarker_Update(void);
void BoundsMarker_LateUpdate(void);
void BoundsMarker_StaticUpdate(void);
void BoundsMarker_Draw(void);
void BoundsMarker_Create(void *data);
void BoundsMarker_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BoundsMarker_EditorDraw(void);
void BoundsMarker_EditorLoad(void);
#endif
void BoundsMarker_Serialize(void);

// Extra Entity Functions
void BoundsMarker_ApplyBounds(EntityPlayer *player, EntityBoundsMarker *marker, bool32 setPos);
void BoundsMarker_ApplyAllBounds(EntityPlayer *player, bool32 setPos);

#endif //! OBJ_BOUNDSMARKER_H
