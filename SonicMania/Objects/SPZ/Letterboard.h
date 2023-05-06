#ifndef OBJ_LETTERBOARD_H
#define OBJ_LETTERBOARD_H

#include "Game.h"

// Object Class
struct ObjectLetterboard {
    RSDK_OBJECT
    Hitbox hitboxBoard;
    uint16 aniFrames;
    uint16 sfxLetterTurn;
    uint16 sfxWin;
};

// Entity Class
struct EntityLetterboard {
    RSDK_ENTITY
    StateMachine(state);
    bool32 controller;
    uint8 letterID;
    int32 spinSpeed;
    int32 timer;
    Animator animatorFront;
    Animator animatorBack;
};

// Object Struct
extern ObjectLetterboard *Letterboard;

// Standard Entity Events
void Letterboard_Update(void);
void Letterboard_LateUpdate(void);
void Letterboard_StaticUpdate(void);
void Letterboard_Draw(void);
void Letterboard_Create(void *data);
void Letterboard_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Letterboard_EditorDraw(void);
void Letterboard_EditorLoad(void);
#endif
void Letterboard_Serialize(void);

// Extra Entity Functions
void Letterboard_State_Controller(void);
void Letterboard_State_CheckPlayerSpin(void);
void Letterboard_State_Spun(void);

#endif //! OBJ_LETTERBOARD_H
