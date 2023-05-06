#ifndef OBJ_RPLANESHIFTER_H
#define OBJ_RPLANESHIFTER_H

#include "Game.h"

// Object Class
struct ObjectRPlaneShifter {
    RSDK_OBJECT
    Animator barAnimator;
    uint16 aniFrames;
    uint16 sfxTicTock;
};

// Entity Class
struct EntityRPlaneShifter {
    RSDK_ENTITY
    StateMachine(state);
    uint32 height;
    uint8 flags;
    int32 rotateDir;
    uint8 activePlayers; // if you're in the hitbox and it's spinning
    uint8 prevActivePlayers;
    uint8 stoodPlayers; // if you're in the hitbox at all
    int32 unused;
    int32 spinSpeed;
    int32 startAngle;
    int32 spinAngle;
    Animator baseAnimator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectRPlaneShifter *RPlaneShifter;

// Standard Entity Events
void RPlaneShifter_Update(void);
void RPlaneShifter_LateUpdate(void);
void RPlaneShifter_StaticUpdate(void);
void RPlaneShifter_Draw(void);
void RPlaneShifter_Create(void *data);
void RPlaneShifter_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void RPlaneShifter_EditorDraw(void);
void RPlaneShifter_EditorLoad(void);
#endif
void RPlaneShifter_Serialize(void);

// Extra Entity Functions
void RPlaneShifter_DrawSprites(void);

void RPlaneShifter_HandlePlaneShift(EntityPlayer *player);

void RPlaneShifter_State_Init(void);
void RPlaneShifter_State_AwaitPlayer(void);
void RPlaneShifter_State_Spinning(void);
void RPlaneShifter_State_FinishSpin(void);

#endif //! OBJ_RPLANESHIFTER_H
