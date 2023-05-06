#ifndef OBJ_PLANESWITCH_H
#define OBJ_PLANESWITCH_H

#include "Game.h"

typedef enum {
    PLANESWITCH_LOWLAYER_LEFT   = 0,
    PLANESWITCH_PLANEA_LEFT     = 0,
    PLANESWITCH_LOWLAYER_RIGHT  = 0,
    PLANESWITCH_PLANEA_RIGHT    = 0,
    PLANESWITCH_HIGHLAYER_LEFT  = 1,
    PLANESWITCH_PLANEB_LEFT     = 2,
    PLANESWITCH_HIGHLAYER_RIGHT = 4,
    PLANESWITCH_PLANEB_RIGHT    = 8,
} PlaneSwitchFlags;

// Object Class
struct ObjectPlaneSwitch {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityPlaneSwitch {
    RSDK_ENTITY
    int32 flags;
    int32 size;
    bool32 onPath;
    int32 negAngle;
    Animator animator;
};

// Object Struct
extern ObjectPlaneSwitch *PlaneSwitch;

// Standard Entity Events
void PlaneSwitch_Update(void);
void PlaneSwitch_LateUpdate(void);
void PlaneSwitch_StaticUpdate(void);
void PlaneSwitch_Draw(void);
void PlaneSwitch_Create(void *data);
void PlaneSwitch_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PlaneSwitch_EditorDraw(void);
void PlaneSwitch_EditorLoad(void);
#endif
void PlaneSwitch_Serialize(void);

// Extra Entity Functions
void PlaneSwitch_DrawSprites(void);
void PlaneSwitch_CheckCollisions(EntityPlaneSwitch *self, void *o, int32 flags, int32 size, bool32 switchdrawGroup, uint8 low, uint8 high);

#endif //! OBJ_PLANESWITCH_H
