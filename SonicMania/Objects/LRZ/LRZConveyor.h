#ifndef OBJ_LRZCONVEYOR_H
#define OBJ_LRZCONVEYOR_H

#include "Game.h"

typedef enum {
    LRZCONVEYOR_BEHAVIOR_CHANGEACTIVE,
    LRZCONVEYOR_BEHAVIOR_CHANGEACTIVE_TOGGLE,
    LRZCONVEYOR_BEHAVIOR_CHANGEDIR,
    LRZCONVEYOR_BEHAVIOR_CHANGEDIR_TOGGLE,
} LRZConveyorButtonBehaviours;

// Object Class
struct ObjectLRZConveyor {
    RSDK_OBJECT
    uint16 aniFrames;
    color lineColors[64];
};

// Entity Class
struct EntityLRZConveyor {
    RSDK_ENTITY
    int32 slope;
    int32 length;
    int32 speed;
    bool32 globalControl;
    int32 buttonTag;
    uint8 buttonBehavior;
    bool32 off;
    uint8 isOff;
    uint8 startDir;
    Vector2 startPos;
    int32 unused;
    EntityButton *taggedButton;
    Hitbox hitbox;
    Animator wheelAnimator;
    Animator smallWheelAnimator;
};

// Object Struct
extern ObjectLRZConveyor *LRZConveyor;

// Standard Entity Events
void LRZConveyor_Update(void);
void LRZConveyor_LateUpdate(void);
void LRZConveyor_StaticUpdate(void);
void LRZConveyor_Draw(void);
void LRZConveyor_Create(void *data);
void LRZConveyor_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LRZConveyor_EditorDraw(void);
void LRZConveyor_EditorLoad(void);
#endif
void LRZConveyor_Serialize(void);

// Extra Entity Functions
void LRZConveyor_HandleBehaviour(void);
void LRZConveyor_HandlePlayerInteractions(void);
int32 LRZConveyor_HandlePlayerCollisions(EntityLRZConveyor *conveyor, void *p, Hitbox *hitboxPtr);
void LRZConveyor_DrawWheels(void);
void LRZConveyor_DrawDeformedLine(int32 startY, int32 startX, int32 endX, int32 endY, int32 offsetX, int32 offsetY, int32 len, color *color);
void LRZConveyor_DrawConveyorLines(void);

#endif //! OBJ_LRZCONVEYOR_H
