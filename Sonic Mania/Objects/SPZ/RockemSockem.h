#ifndef OBJ_ROCKEMSOCKEM_H
#define OBJ_ROCKEMSOCKEM_H

#include "SonicMania.h"

//Extras
#define RockemSockem_coilCount 6

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 sfxRockemSockem;
} ObjectRockemSockem;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int timer;
    int timer2;
    Vector2 drawPos;
    Vector2 startPos;
    Vector2 jointPositions[RockemSockem_coilCount];
    Vector2 unknownPositions[RockemSockem_coilCount];
    int jointRotations[RockemSockem_coilCount];
    int field_E8;
    int field_EC;
    Animator baseAnimator;
    Animator jointAnimator;
    Animator ballAnimator;
} EntityRockemSockem;

// Object Struct
extern ObjectRockemSockem *RockemSockem;

// Standard Entity Events
void RockemSockem_Update(void);
void RockemSockem_LateUpdate(void);
void RockemSockem_StaticUpdate(void);
void RockemSockem_Draw(void);
void RockemSockem_Create(void* data);
void RockemSockem_StageLoad(void);
void RockemSockem_EditorDraw(void);
void RockemSockem_EditorLoad(void);
void RockemSockem_Serialize(void);

// Extra Entity Functions
void RockemSockem_Unknown1(void);

#endif //!OBJ_ROCKEMSOCKEM_H
