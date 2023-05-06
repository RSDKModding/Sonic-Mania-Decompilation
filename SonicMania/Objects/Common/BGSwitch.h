#ifndef OBJ_BGSWITCH_H
#define OBJ_BGSWITCH_H

#include "Game.h"

// Object Class
struct ObjectBGSwitch {
    RSDK_OBJECT
    int32 screenID;
    int32 layerIDs[8];
    StateMachine(switchCallback[8]);
    uint16 aniFrames;
};

// Entity Class
struct EntityBGSwitch {
    RSDK_ENTITY
    Vector2 size;
    uint8 bgID;
    Animator animator;
};

// Object Struct
extern ObjectBGSwitch *BGSwitch;

// Standard Entity Events
void BGSwitch_Update(void);
void BGSwitch_LateUpdate(void);
void BGSwitch_StaticUpdate(void);
void BGSwitch_Draw(void);
void BGSwitch_Create(void *data);
void BGSwitch_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BGSwitch_EditorDraw(void);
void BGSwitch_EditorLoad(void);
#endif
void BGSwitch_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_BGSWITCH_H
