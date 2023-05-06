#ifndef OBJ_FARPLANE_H
#define OBJ_FARPLANE_H

#include "Game.h"

#define FARPLANE_ENTITY_COUNT (256)

// Object Class
struct ObjectFarPlane {
    RSDK_OBJECT
    uint32 aniFrames;
    Vector2 screenPos;
    Vector2 worldPos;
    Vector2 originPos;
    Vector2 position;
    Vector2 positionList[0x200];
    int32 unused1;
    int32 unused2;
    int32 unused3;
    int32 unused4;
    int32 unused5;
    uint16 layerID;
};

// Entity Class
struct EntityFarPlane {
    RSDK_ENTITY
    Vector2 size;
    Vector2 origin;
    uint16 entitySlots[FARPLANE_ENTITY_COUNT];
    int32 entityCount;
    Animator animator;
};

// Object Struct
extern ObjectFarPlane *FarPlane;

// Standard Entity Events
void FarPlane_Update(void);
void FarPlane_LateUpdate(void);
void FarPlane_StaticUpdate(void);
void FarPlane_Draw(void);
void FarPlane_Create(void *data);
void FarPlane_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FarPlane_EditorDraw(void);
void FarPlane_EditorLoad(void);
#endif
void FarPlane_Serialize(void);

// Extra Entity Functions
void FarPlane_SetupEntities(void);
void FarPlane_SetEntityActivities(uint8 active);
void FarPlane_DrawHook_ApplyFarPlane(void);
void FarPlane_DrawHook_RemoveFarPlane(void);
void FarPlane_Scanline_FarPlaneView(ScanlineInfo *scanline);

#endif //! OBJ_FARPLANE_H
