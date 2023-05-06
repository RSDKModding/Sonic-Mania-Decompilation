#ifndef OBJ_DACONTROL_H
#define OBJ_DACONTROL_H

#include "Game.h"

// Object Class
struct ObjectDAControl {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityDAControl {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused1;
    int32 unused2;
    int32 unused3;
    String text;
    Animator backPlateAnimator;
    Animator buttonAnimator;
    Animator optionsAnimator[5];
    Animator textAnimator;
};

// Object Struct
extern ObjectDAControl *DAControl;

// Standard Entity Events
void DAControl_Update(void);
void DAControl_LateUpdate(void);
void DAControl_StaticUpdate(void);
void DAControl_Draw(void);
void DAControl_Create(void *data);
void DAControl_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void DAControl_EditorDraw(void);
void DAControl_EditorLoad(void);
#endif
void DAControl_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_DACONTROL_H
