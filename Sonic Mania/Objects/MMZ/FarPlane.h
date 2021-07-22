#ifndef OBJ_FARPLANE_H
#define OBJ_FARPLANE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    Vector2 field_8;
    Vector2 field_10;
    Vector2 field_18;
    Vector2 field_20;
    Vector2 positionList[0x200];
    int field_1028;
    int field_102C;
    int field_1030;
    int field_1034;
    int field_1038;
    ushort layerID;
} ObjectFarPlane;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    Vector2 origin;
    ushort entityIDs[0x20];
    int entityCount;
    int field_26C;
    int field_270;
    int field_274;
    int field_278;
    int field_27C;
    int field_280;
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
void FarPlane_EditorDraw(void);
void FarPlane_EditorLoad(void);
void FarPlane_Serialize(void);

// Extra Entity Functions
void FarPlane_Unknown1(void);
void FarPlane_Unknown2(byte active);
void FarPlane_Unknown3(void);
void FarPlane_Unknown4(void);
void FarPlane_Unknown5(ScanlineInfo *scanline);

#endif //!OBJ_FARPLANE_H
