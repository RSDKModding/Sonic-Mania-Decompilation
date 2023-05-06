#ifndef OBJ_SWINGROPE_H
#define OBJ_SWINGROPE_H

#include "Game.h"

// Object Class
struct ObjectSwingRope {
    RSDK_OBJECT
    Hitbox hitboxGrabHandle;
    Hitbox hitboxHandle;
    uint16 aniFrames;
};

// Entity Class
struct EntitySwingRope {
    RSDK_ENTITY
    uint8 ropeSize;
    int32 ropeGrabDelay;
    int32 rotatedAngle;
    int32 rotatedOffset;
    int32 playerLayers[PLAYER_COUNT];
    int32 angleOffset;
    Vector2 ropePos;
    Vector2 rotatePos;
    Animator ropeAnimator;
    Animator handleAnimator;
    Animator pivotAnimator;
};

// Object Struct
extern ObjectSwingRope *SwingRope;

// Standard Entity Events
void SwingRope_Update(void);
void SwingRope_LateUpdate(void);
void SwingRope_StaticUpdate(void);
void SwingRope_Draw(void);
void SwingRope_Create(void *data);
void SwingRope_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SwingRope_EditorDraw(void);
void SwingRope_EditorLoad(void);
#endif
void SwingRope_Serialize(void);

// Extra Entity Functions
void SwingRope_DebugDraw(void);
void SwingRope_DebugSpawn(void);

#endif //! OBJ_SWINGROPE_H
