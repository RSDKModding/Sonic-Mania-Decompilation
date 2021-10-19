#ifndef OBJ_ERZKING_H
#define OBJ_ERZKING_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 boundsL;
    int32 boundsR;
    int32 boundsM;
    int32 boundsT;
    int32 boundsB;
    uint16 sfxHit;
    uint16 sfxExplosion2;
    uint16 aniFrames;
} ObjectERZKing;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 posUnknown;
    int32 type;
    int32 timer;
    int32 nextType;
    int32 invincibilityTimer;
    int32 health;
    int32 typeChangeTimer;
    int32 angle2;
    int32 frameIDs[10];
    Vector2 framePositions[10];
    Vector2 drawPos;
    int32 field_104;
    int32 field_108;
    Entity *parent;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
    Animator animator7;
    Animator animator8;
    Animator animator9;
    Animator animator10;
    Hitbox hitbox;
    uint8 storeDrawFX;
} EntityERZKing;

// Object Struct
extern ObjectERZKing *ERZKing;

// Standard Entity Events
void ERZKing_Update(void);
void ERZKing_LateUpdate(void);
void ERZKing_StaticUpdate(void);
void ERZKing_Draw(void);
void ERZKing_Create(void* data);
void ERZKing_StageLoad(void);
void ERZKing_EditorDraw(void);
void ERZKing_EditorLoad(void);
void ERZKing_Serialize(void);

// Extra Entity Functions
void ERZKing_CheckPlayerCollisions(void);
void ERZKing_HandleFrames(void);

void ERZKing_StateDraw_Body(void);
void ERZKing_StateDraw_Arm(void);

void ERZKing_State_Unknown1(void);
void ERZKing_State_Unknown2(void);
void ERZKing_State_Unknown3(void);
void ERZKing_State_Unknown4(void);
void ERZKing_State_ChangeHBH(void);
void ERZKing_State_Arm(void);
void ERZKing_State_Explode(void);

#endif //!OBJ_ERZKING_H
