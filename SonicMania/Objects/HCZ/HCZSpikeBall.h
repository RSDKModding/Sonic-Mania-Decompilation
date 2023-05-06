#ifndef OBJ_HCZSPIKEBALL_H
#define OBJ_HCZSPIKEBALL_H

#include "Game.h"

// Object Class
struct ObjectHCZSpikeBall {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityHCZSpikeBall {
    RSDK_ENTITY
    int32 unused1;
    Vector2 startPos;
    int32 unused2;
    int32 intervalOffset;
    EntityHangConveyor *conveyor;
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectHCZSpikeBall *HCZSpikeBall;

// Standard Entity Events
void HCZSpikeBall_Update(void);
void HCZSpikeBall_LateUpdate(void);
void HCZSpikeBall_StaticUpdate(void);
void HCZSpikeBall_Draw(void);
void HCZSpikeBall_Create(void *data);
void HCZSpikeBall_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HCZSpikeBall_EditorDraw(void);
void HCZSpikeBall_EditorLoad(void);
#endif
void HCZSpikeBall_Serialize(void);

// Extra Entity Functions
void HCZSpikeBall_HandleConveyorSetup(void);
void HCZSpikeBall_LinkToConveyor(void);
void HCZSpikeBall_HandlePlayerInteractions(void);
void HCZSpikeBall_HandleConveyorMovement(void);

#endif //! OBJ_HCZSPIKEBALL_H
