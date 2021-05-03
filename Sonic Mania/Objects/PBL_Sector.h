#ifndef OBJ_PBL_SECTOR_H
#define OBJ_PBL_SECTOR_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    ushort field_4;
} ObjectPBL_Sector;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    byte craneID;
    int timer;
    bool32 flag;
    Animator animator;
} EntityPBL_Sector;

// Object Struct
extern ObjectPBL_Sector *PBL_Sector;

// Standard Entity Events
void PBL_Sector_Update(void);
void PBL_Sector_LateUpdate(void);
void PBL_Sector_StaticUpdate(void);
void PBL_Sector_Draw(void);
void PBL_Sector_Create(void* data);
void PBL_Sector_StageLoad(void);
void PBL_Sector_EditorDraw(void);
void PBL_Sector_EditorLoad(void);
void PBL_Sector_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_PBL_SECTOR_H
