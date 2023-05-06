#ifndef OBJ_UFO_DUST_H
#define OBJ_UFO_DUST_H

#include "Game.h"

// Object Class
struct ObjectUFO_Dust {
    RSDK_OBJECT
    int32 unused1;
    int32 unused2;
    uint16 aniFrames;
};

// Entity Class
struct EntityUFO_Dust {
    RSDK_ENTITY
    int32 unused1;
    int32 unused2;
    int32 height;
    int32 unused3;
    int32 worldX;
    int32 worldY;
    Animator animator;
};

// Object Struct
extern ObjectUFO_Dust *UFO_Dust;

// Standard Entity Events
void UFO_Dust_Update(void);
void UFO_Dust_LateUpdate(void);
void UFO_Dust_StaticUpdate(void);
void UFO_Dust_Draw(void);
void UFO_Dust_Create(void *data);
void UFO_Dust_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UFO_Dust_EditorDraw(void);
void UFO_Dust_EditorLoad(void);
#endif
void UFO_Dust_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_UFO_DUST_H
