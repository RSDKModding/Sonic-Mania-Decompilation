#ifndef OBJ_WATERGUSH_H
#define OBJ_WATERGUSH_H

#include "Game.h"

typedef enum {
    WATERGUSH_UP,
    WATERGUSH_RIGHT,
    WATERGUSH_LEFT,
} WaterGushOrientations;

// Object Class
struct ObjectWaterGush {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxGush;
};

// Entity Class
struct EntityWaterGush {
    RSDK_ENTITY
    StateMachine(state);
    uint8 orientation;
    int32 length;
    int32 speed;
    uint8 activePlayers;
    Vector2 startPos;
    int32 unused;
    int32 gushPos;
    int32 gravityStrength;
    bool32 activated;
    bool32 finishedExtending;
    Hitbox hitboxRange;
    Hitbox hitboxGush;
    Animator plumeAnimator;
    Animator topAnimator;
};

// Object Struct
extern ObjectWaterGush *WaterGush;

// Standard Entity Events
void WaterGush_Update(void);
void WaterGush_LateUpdate(void);
void WaterGush_StaticUpdate(void);
void WaterGush_Draw(void);
void WaterGush_Create(void *data);
void WaterGush_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void WaterGush_EditorDraw(void);
void WaterGush_EditorLoad(void);
#endif
void WaterGush_Serialize(void);

// Extra Entity Functions
void WaterGush_SetupHitboxes(void);
void WaterGush_DrawSprites(void);
void WaterGush_SpawnBrickDebris(void);

#endif //! OBJ_WATERGUSH_H
