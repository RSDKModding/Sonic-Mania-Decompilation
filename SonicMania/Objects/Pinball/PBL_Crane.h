#ifndef OBJ_PBL_CRANE_H
#define OBJ_PBL_CRANE_H

#include "Game.h"

#if MANIA_USE_PLUS

typedef enum {
    PBL_CRANE_MANAGER,
    PBL_CRANE_CRANE,
    PBL_CRANE_PRIZEDISPLAY,
    PBL_CRANE_PRIZE,
} PBL_CraneTypes;

typedef enum {
    PBL_CRANE_PRIZE_SONIC,
    PBL_CRANE_PRIZE_TAILS,
    PBL_CRANE_PRIZE_KNUCKLES,
    PBL_CRANE_PRIZE_MIGHTY,
    PBL_CRANE_PRIZE_RAY,
    PBL_CRANE_PRIZE_EGGMAN,
    PBL_CRANE_PRIZE_RINGS,
    PBL_CRANE_PRIZE_SHIELD_BLUE,
    PBL_CRANE_PRIZE_SHIELD_BUBBLE,
    PBL_CRANE_PRIZE_SHIELD_FIRE,
    PBL_CRANE_PRIZE_SHIELD_ELECTRIC,
    PBL_CRANE_PRIZE_TABLE_RESTORE,
    PBL_CRANE_PRIZE_1UP,
} PBL_CranePrizes;

typedef enum {
    PBL_CRANE_PRIZEID_NONE,
    PBL_CRANE_PRIZEID_NOTHING,
    PBL_CRANE_PRIZEID_BAD,
    PBL_CRANE_PRIZEID_BUDDY,
    PBL_CRANE_PRIZEID_RINGS,
    PBL_CRANE_PRIZEID_ITEM,
    PBL_CRANE_PRIZEID_TBLRESTORE,
    PBL_CRANE_PRIZEID_1UP,
} PBL_CranePrizeIDs;

// Object Class
struct ObjectPBL_Crane {
    RSDK_OBJECT
    int32 prizeID;
    uint16 aniFrames;
    uint16 sfxBackGlass;
    uint16 sfxCraneMove;
    uint16 sfxCraneDrop;
    uint16 sfxCraneGrab;
    uint16 sfxCraneRise;
    uint16 sfxPrizeGood;
    uint16 sfxPrizeBad;
};

// Entity Class
struct EntityPBL_Crane {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 type;
    int32 unused1;
    int32 timer;
    int32 unused2;
    int32 unused3;
    EntityPBL_Crane *parent;
    Entity *cameraTarget;
    Animator displayAnimator;
    Animator bgAnimator;
    Animator craneBackAnimator;
    Animator craneFrontAnimator;
    Animator machineAnimator;
};

// Object Struct
extern ObjectPBL_Crane *PBL_Crane;

// Standard Entity Events
void PBL_Crane_Update(void);
void PBL_Crane_LateUpdate(void);
void PBL_Crane_StaticUpdate(void);
void PBL_Crane_Draw(void);
void PBL_Crane_Create(void *data);
void PBL_Crane_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PBL_Crane_EditorDraw(void);
void PBL_Crane_EditorLoad(void);
#endif
void PBL_Crane_Serialize(void);

// Extra Entity Functions
void PBL_Crane_HandlePrizes(void);

void PBL_Crane_Draw_CraneMachine(void);
void PBL_Crane_Draw_PrizeDisplay(void);
void PBL_Crane_Draw_Crane(void);

void PBL_Crane_State_CreatePrizes(void);
void PBL_Crane_State_DisplayPrizes(void);
void PBL_Crane_StatePrizeDisplay_Move(void);

void PBL_Crane_StateCrane_Lower(void);
void PBL_Crane_StateCrane_Move(void);
void PBL_Crane_StateCrane_Controlled(void);
void PBL_Crane_StateCrane_Drop(void);
void PBL_Crane_StateCrane_Grab(void);
void PBL_Crane_StateCrane_Rise(void);

void PBL_Crane_StatePrize_Bounce(void);
void PBL_Crane_StatePrize_Flash(void);
void PBL_Crane_StatePrize_PrizeGet(void);

#endif

#endif //! OBJ_PBL_CRANE_H
