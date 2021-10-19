#ifndef OBJ_BGSWITCH_H
#define OBJ_BGSWITCH_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 screenID;
    int32 layerIDs[8];
    StateMachine(switchCallback[8]);
    uint16 aniFrames;
} ObjectBGSwitch;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    uint8 bgID;
    Animator animator;
} EntityBGSwitch;

// Object Struct
extern ObjectBGSwitch *BGSwitch;

// Standard Entity Events
void BGSwitch_Update(void);
void BGSwitch_LateUpdate(void);
void BGSwitch_StaticUpdate(void);
void BGSwitch_Draw(void);
void BGSwitch_Create(void* data);
void BGSwitch_StageLoad(void);
void BGSwitch_EditorDraw(void);
void BGSwitch_EditorLoad(void);
void BGSwitch_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BGSWITCH_H
