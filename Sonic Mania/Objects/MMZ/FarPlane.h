#ifndef OBJ_FARPLANE_H
#define OBJ_FARPLANE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    Vector2 field_8;
    Vector2 field_10;
    Vector2 field_18;
    Vector2 field_20;
    Vector2 positionList[0x200];
    int32 field_1028;
    int32 field_102C;
    int32 field_1030;
    int32 field_1034;
    int32 field_1038;
    uint16 layerID;
} ObjectFarPlane;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    Vector2 origin;
    uint16 entityIDs[0x100];
    int32 entityCount;
    Animator animator;
} EntityFarPlane;

// Object Struct
extern ObjectFarPlane *FarPlane;

// Standard Entity Events
void FarPlane_Update(void);
void FarPlane_LateUpdate(void);
void FarPlane_StaticUpdate(void);
void FarPlane_Draw(void);
void FarPlane_Create(void* data);
void FarPlane_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void FarPlane_EditorDraw(void);
void FarPlane_EditorLoad(void);
#endif
void FarPlane_Serialize(void);

// Extra Entity Functions
void FarPlane_SetupEntities(void);
void FarPlane_SetEntityActivities(uint8 active);
void FarPlane_DrawLayerCB_Low(void);
void FarPlane_DrawLayerCB_High(void);
void FarPlane_ScanlineCB(ScanlineInfo *scanline);

#endif //!OBJ_FARPLANE_H
