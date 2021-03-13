#ifndef OBJ_BOUNDSMARKER_H
#define OBJ_BOUNDSMARKER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectBoundsMarker;

// Entity Class
typedef struct {
    RSDK_ENTITY
    byte type;
    int width;
    bool32 vsDisable;
    int offset;
} EntityBoundsMarker;

// Object Struct
extern ObjectBoundsMarker *BoundsMarker;

// Standard Entity Events
void BoundsMarker_Update();
void BoundsMarker_LateUpdate();
void BoundsMarker_StaticUpdate();
void BoundsMarker_Draw();
void BoundsMarker_Create(void* data);
void BoundsMarker_StageLoad();
void BoundsMarker_EditorDraw();
void BoundsMarker_EditorLoad();
void BoundsMarker_Serialize();

// Extra Entity Functions
void BoundsMarker_CheckBounds(void *player, EntityBoundsMarker *entity, bool32 setPos);
void BoundsMarker_CheckAllBounds(void *player, bool32 setPos);

#endif //!OBJ_BOUNDSMARKER_H
