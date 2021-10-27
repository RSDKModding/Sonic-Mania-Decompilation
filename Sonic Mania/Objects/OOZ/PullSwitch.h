#ifndef OBJ_PULLSWITCH_H
#define OBJ_PULLSWITCH_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    int32 field_C;
    uint16 aniFrames;
    uint16 sfxButton;
    uint16 sfxSmogClear;
} ObjectPullSwitch;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
    int32 field_5C;
    int32 sprY;
    int32 sprHeight;
    bool32 activated;
    uint8 playerTimers[4];
    uint8 activePlayers;
    Vector2 drawPos;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityPullSwitch;

// Object Struct
extern ObjectPullSwitch *PullSwitch;

// Standard Entity Events
void PullSwitch_Update(void);
void PullSwitch_LateUpdate(void);
void PullSwitch_StaticUpdate(void);
void PullSwitch_Draw(void);
void PullSwitch_Create(void* data);
void PullSwitch_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PullSwitch_EditorDraw(void);
void PullSwitch_EditorLoad(void);
#endif
void PullSwitch_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PULLSWITCH_H
