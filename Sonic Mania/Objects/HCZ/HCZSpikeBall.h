#ifndef OBJ_HCZSPIKEBALL_H
#define OBJ_HCZSPIKEBALL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
} ObjectHCZSpikeBall;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int field_58;
    Vector2 field_5C;
    int field_64;
    int field_68;
    EntityHangConveyor *conveyor;
    Hitbox hitbox;
    Animator animator;
} EntityHCZSpikeBall;

// Object Struct
extern ObjectHCZSpikeBall *HCZSpikeBall;

// Standard Entity Events
void HCZSpikeBall_Update(void);
void HCZSpikeBall_LateUpdate(void);
void HCZSpikeBall_StaticUpdate(void);
void HCZSpikeBall_Draw(void);
void HCZSpikeBall_Create(void* data);
void HCZSpikeBall_StageLoad(void);
void HCZSpikeBall_EditorDraw(void);
void HCZSpikeBall_EditorLoad(void);
void HCZSpikeBall_Serialize(void);

// Extra Entity Functions
void HCZSpikeBall_HandleConveyorSetup(void);
void HCZSpikeBall_Unknown2(void);
void HCZSpikeBall_HandlePlayerInteractions(void);
void HCZSpikeBall_Unknown4(void);

#endif //!OBJ_HCZSPIKEBALL_H
