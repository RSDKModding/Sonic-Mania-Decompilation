#ifndef OBJ_POPOUT_H
#define OBJ_POPOUT_H

#include "Game.h"

typedef enum {
    POPOUT_CHILD_SPRING_YELLOW,
    POPOUT_CHILD_SPRING_RED,
    POPOUT_CHILD_SPIKES,
} PopOutChildTypes;

// Object Class
struct ObjectPopOut {
    RSDK_OBJECT
    bool32 hasButton;
    uint16 aniFrames;
};

// Entity Class
struct EntityPopOut {
    RSDK_ENTITY
    uint8 orientation;
    uint8 tag;
    bool32 manualTrigger;
    int32 delay;
    Vector2 startPos;
    int32 unused;
    int32 childType;
    bool32 shouldAppear;
    int32 appearTimer;
    int32 timer;
    Vector2 appearVelocity;
    Hitbox hitboxSolid;
    Hitbox hitboxRange;
    Animator mountAnimator;
    Animator childAnimator;
};

// Object Struct
extern ObjectPopOut *PopOut;

// Standard Entity Events
void PopOut_Update(void);
void PopOut_LateUpdate(void);
void PopOut_StaticUpdate(void);
void PopOut_Draw(void);
void PopOut_Create(void *data);
void PopOut_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PopOut_EditorDraw(void);
void PopOut_EditorLoad(void);
#endif
void PopOut_Serialize(void);

// Extra Entity Functions
void PopOut_SetupHitboxes(void);

#endif //! OBJ_POPOUT_H
