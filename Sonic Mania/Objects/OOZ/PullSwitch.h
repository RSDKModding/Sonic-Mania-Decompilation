#ifndef OBJ_PULLSWITCH_H
#define OBJ_PULLSWITCH_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    int field_C;
    ushort aniFrames;
    ushort sfxButton;
    ushort sfxSmogClear;
} ObjectPullSwitch;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int field_58;
    int field_5C;
    int sprY;
    int sprHeight;
    bool32 activated;
    byte playerTimers[4];
    byte activePlayers;
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
void PullSwitch_EditorDraw(void);
void PullSwitch_EditorLoad(void);
void PullSwitch_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PULLSWITCH_H
