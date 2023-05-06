#ifndef OBJ_TWISTEDTUBES_H
#define OBJ_TWISTEDTUBES_H

#include "Game.h"

// Object Class
struct ObjectTwistedTubes {
    RSDK_OBJECT
    Hitbox hitboxSolid;
    Hitbox hitboxEntryL;
    Hitbox hitboxEntryR;
    bool32 playerActive[PLAYER_COUNT];
    uint16 aniFrames;
    uint16 sfxTravel;
};

// Entity Class
struct EntityTwistedTubes {
    RSDK_ENTITY
    StateMachine(state);
    uint8 height;
    int32 timer;
    EntityPlayer *player;
    Animator entranceAnimator;
    Animator tubeAnimator;
    Animator topGlassAnimator;
    Animator sideAnimator;
    Animator bottomGlassAnimator;
};

// Object Struct
extern ObjectTwistedTubes *TwistedTubes;

// Standard Entity Events
void TwistedTubes_Update(void);
void TwistedTubes_LateUpdate(void);
void TwistedTubes_StaticUpdate(void);
void TwistedTubes_Draw(void);
void TwistedTubes_Create(void *data);
void TwistedTubes_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TwistedTubes_EditorDraw(void);
void TwistedTubes_EditorLoad(void);
#endif
void TwistedTubes_Serialize(void);

// Extra Entity Functions
void TwistedTubes_State_HandleInteractions(void);

void TwistedTubes_State_PlayerEntryL(void);
void TwistedTubes_State_PlayerEntryR(void);
void TwistedTubes_State_FirstLoopR(void);
void TwistedTubes_State_TubeLoops(void);
void TwistedTubes_State_FirstLoopL(void);
void TwistedTubes_State_ExitL(void);
void TwistedTubes_State_ExitR(void);

#endif //! OBJ_TWISTEDTUBES_H
