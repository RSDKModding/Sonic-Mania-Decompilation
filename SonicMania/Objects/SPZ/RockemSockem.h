#ifndef OBJ_ROCKEMSOCKEM_H
#define OBJ_ROCKEMSOCKEM_H

#include "Game.h"

// Extras
#define ROCKEMSOCKEM_COIL_COUNT (6)

// Object Class
struct ObjectRockemSockem {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 sfxRockemSockem;
};

// Entity Class
struct EntityRockemSockem {
    RSDK_ENTITY
    int32 bounceTimer;
    int32 reboundTimer;
    Vector2 drawPos;
    Vector2 startPos;
    Vector2 jointPositions[ROCKEMSOCKEM_COIL_COUNT];
    Vector2 unused1[ROCKEMSOCKEM_COIL_COUNT];
    int32 jointRotations[ROCKEMSOCKEM_COIL_COUNT];
    int32 angleVel;
    int32 unused2;
    Animator baseAnimator;
    Animator jointAnimator;
    Animator ballAnimator;
};

// Object Struct
extern ObjectRockemSockem *RockemSockem;

// Standard Entity Events
void RockemSockem_Update(void);
void RockemSockem_LateUpdate(void);
void RockemSockem_StaticUpdate(void);
void RockemSockem_Draw(void);
void RockemSockem_Create(void *data);
void RockemSockem_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void RockemSockem_EditorDraw(void);
void RockemSockem_EditorLoad(void);
#endif
void RockemSockem_Serialize(void);

// Extra Entity Functions
void RockemSockem_HandleJointPositions(void);

#endif //! OBJ_ROCKEMSOCKEM_H
