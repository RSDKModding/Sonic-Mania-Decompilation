#ifndef OBJ_PBL_SECTOR_H
#define OBJ_PBL_SECTOR_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectPBL_Sector {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityPBL_Sector {
    RSDK_ENTITY
    Vector2 size;
    uint8 craneID;
    int32 timer;
    bool32 playerEntered;
    Animator animator;
};

// Object Struct
extern ObjectPBL_Sector *PBL_Sector;

// Standard Entity Events
void PBL_Sector_Update(void);
void PBL_Sector_LateUpdate(void);
void PBL_Sector_StaticUpdate(void);
void PBL_Sector_Draw(void);
void PBL_Sector_Create(void *data);
void PBL_Sector_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PBL_Sector_EditorDraw(void);
void PBL_Sector_EditorLoad(void);
#endif
void PBL_Sector_Serialize(void);

// Extra Entity Functions
#endif

#endif //! OBJ_PBL_SECTOR_H
